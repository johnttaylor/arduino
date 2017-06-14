/*-----------------------------------------------------------------------------
* This file is part of the Arduino Project.  The Arduino Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/arduino/license.txt
*
* Copyright (c) 2017 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Golem/OutputNeoPixel.h"

/// Namespaces
using namespace Golem;

#define SPIN_IDX_LIMIT          3
#define QTR_SPIN_IDX_LIMIT      7



////////////////////////////////////////
OutputNeoPixel::OutputNeoPixel( Options_T option, uint16_t numberOfLEDs, uint8_t pinNumber, bool isRGBW, neoPixelType ledType )
    : m_ledDriver( numberOfLEDs, pinNumber, ledType )
    , m_option( option )
    , m_currentLed( option == ePAIRS_SPIN_C ? SPIN_IDX_LIMIT : option == eQUARTER_SPIN_C ? QTR_SPIN_IDX_LIMIT : 0 )
    , m_rgbw( isRGBW )
    , m_stopped( false )
{
    m_ledDriver.begin();
    setAllLEDs( 0x0000 );
    m_ledDriver.show();
}

OutputNeoPixel::~OutputNeoPixel()
{
    setAllLEDs( 0x0000 );
    m_ledDriver.show();
}

////////////////////////////////////////
const char* OutputNeoPixel::getDescription( Cpl::Text::String& brief )
{
    brief.format( "OutpuNeoPixel(%s)", toString( m_option ) );
    return brief;
}



////////////////////////////////////////
void OutputNeoPixel::write( bool newBit, FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType )
{
    uint32_t color;
    uint16_t led;

    // Start the output will all LEDs off
    setAllLEDs( 0x0000 );
    color = convertToWRGB( bitColor, colorIntensity, bitType );

    switch ( m_option )
    {
    case ePAIRS:
        for ( led=0; led < m_ledDriver.numPixels(); led+=4 )
        {
            m_ledDriver.setPixelColor( led, color );
            m_ledDriver.setPixelColor( led + 1, color );
        }
        break;

    case eQUARTER:
        for ( led=0; led < m_ledDriver.numPixels(); led+=4 )
        {
            m_ledDriver.setPixelColor( led, color );
        }
        break;


    case ePAIRS_SPIN_CC:
    case ePAIRS_FAST_SPIN_CC:
        for ( led=0; led < m_ledDriver.numPixels(); led += SPIN_IDX_LIMIT + 1 )
        {
            setPairSpinLED( led, m_currentLed, color );
        }
        if ( (newBit || m_option == ePAIRS_FAST_SPIN_CC) && bitColor != FrameBitColor::eOFF && ++m_currentLed > SPIN_IDX_LIMIT )
        {
            m_currentLed = 0;
        }
        break;

    case ePAIRS_SPIN_C:
    case ePAIRS_FAST_SPIN_C:
        for ( led=0; led < m_ledDriver.numPixels(); led += SPIN_IDX_LIMIT + 1 )
        {
            setPairSpinLED( led, m_currentLed, color );
        }
        if ( (newBit || m_option == ePAIRS_FAST_SPIN_C)  && bitColor != FrameBitColor::eOFF && --m_currentLed == UINT16_MAX )
        {
            m_currentLed = SPIN_IDX_LIMIT;
        }
        break;

    case eQUARTER_SPIN_CC:
    case eQUARTER_FAST_SPIN_CC:
        for ( led=0; led < m_ledDriver.numPixels(); led += QTR_SPIN_IDX_LIMIT + 1 )
        {
            setQuarterSpinLED( led, m_currentLed, color );
        }
        if ( (newBit || m_option == eQUARTER_FAST_SPIN_CC)  && bitColor != FrameBitColor::eOFF && ++m_currentLed > QTR_SPIN_IDX_LIMIT )
        {
            m_currentLed = 0;
        }
        break;

    case eQUARTER_SPIN_C:
    case eQUARTER_FAST_SPIN_C:
        for ( led=0; led < m_ledDriver.numPixels(); led += QTR_SPIN_IDX_LIMIT + 1 )
        {
            setQuarterSpinLED( led, m_currentLed, color );
        }
        if ( (newBit || m_option == eQUARTER_FAST_SPIN_C)  && bitColor != FrameBitColor::eOFF && --m_currentLed == UINT16_MAX )
        {
            m_currentLed = QTR_SPIN_IDX_LIMIT;
        }
        break;

    case eALL:
    default:
        setAllLEDs( convertToWRGB( bitColor, colorIntensity, bitType ) );
        break;
    }

    m_ledDriver.show();
}

void OutputNeoPixel::setPairSpinLED( uint16_t baseLed, uint16_t phase, uint32_t color )
{
    switch ( phase )
    {
    case 0:
        m_ledDriver.setPixelColor( baseLed + 0, color );
        m_ledDriver.setPixelColor( baseLed + 1, color );
        break;
    case 1:
        m_ledDriver.setPixelColor( baseLed + 1, color );
        m_ledDriver.setPixelColor( baseLed + 2, color );
        break;
    case 2:
        m_ledDriver.setPixelColor( baseLed + 2, color );
        m_ledDriver.setPixelColor( baseLed + 3, color );
        break;
    case 3:
        m_ledDriver.setPixelColor( baseLed + 3, color );
        m_ledDriver.setPixelColor( baseLed + 0, color );
        break;
    }
}

void OutputNeoPixel::setQuarterSpinLED( uint16_t baseLed, uint16_t phase, uint32_t color )
{
    switch ( phase )
    {
    case 0:
        m_ledDriver.setPixelColor( baseLed + 0, color );
        m_ledDriver.setPixelColor( baseLed + 1, color );
        m_ledDriver.setPixelColor( baseLed + 2, color );
        m_ledDriver.setPixelColor( baseLed + 3, color );
        break;
    case 1:
        m_ledDriver.setPixelColor( baseLed + 1, color );
        m_ledDriver.setPixelColor( baseLed + 2, color );
        m_ledDriver.setPixelColor( baseLed + 3, color );
        m_ledDriver.setPixelColor( baseLed + 4, color );
        break;
    case 2:
        m_ledDriver.setPixelColor( baseLed + 2, color );
        m_ledDriver.setPixelColor( baseLed + 3, color );
        m_ledDriver.setPixelColor( baseLed + 4, color );
        m_ledDriver.setPixelColor( baseLed + 5, color );
        break;
    case 3:
        m_ledDriver.setPixelColor( baseLed + 3, color );
        m_ledDriver.setPixelColor( baseLed + 4, color );
        m_ledDriver.setPixelColor( baseLed + 5, color );
        m_ledDriver.setPixelColor( baseLed + 6, color );
        break;
    case 4:
        m_ledDriver.setPixelColor( baseLed + 4, color );
        m_ledDriver.setPixelColor( baseLed + 5, color );
        m_ledDriver.setPixelColor( baseLed + 6, color );
        m_ledDriver.setPixelColor( baseLed + 7, color );
        break;
    case 5:
        m_ledDriver.setPixelColor( baseLed + 5, color );
        m_ledDriver.setPixelColor( baseLed + 6, color );
        m_ledDriver.setPixelColor( baseLed + 7, color );
        m_ledDriver.setPixelColor( baseLed + 0, color );

        break;
    case 6:
        m_ledDriver.setPixelColor( baseLed + 6, color );
        m_ledDriver.setPixelColor( baseLed + 7, color );
        m_ledDriver.setPixelColor( baseLed + 0, color );
        m_ledDriver.setPixelColor( baseLed + 1, color );

        break;
    case 7:
        m_ledDriver.setPixelColor( baseLed + 7, color );
        m_ledDriver.setPixelColor( baseLed + 0, color );
        m_ledDriver.setPixelColor( baseLed + 1, color );
        m_ledDriver.setPixelColor( baseLed + 2, color );
        break;
    }
}


void OutputNeoPixel::stop( void )
{
    // Only turn the LEDs off once (not every pass of the main loop)
    if ( !m_stopped )
    {
        m_stopped = true;
        setAllLEDs( 0x0000 );
        m_ledDriver.show();
    }
}


////////////////////////////////////////
void OutputNeoPixel::setAllLEDs( uint32_t wrgb )
{
    for ( uint16_t i=0; i < m_ledDriver.numPixels(); i++ )
    {
        m_ledDriver.setPixelColor( i, wrgb );
    }
}

uint32_t OutputNeoPixel::convertToWRGB( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType )
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t w = 0;

    switch ( bitColor )
    {
    case FrameBitColor::eRED:
        r = colorIntensity;
        break;

    case FrameBitColor::eGREEN:
        g = colorIntensity;
        break;

    case FrameBitColor::eBLUE:
        b = colorIntensity;
        break;

    case FrameBitColor::eWHITE:
        w = colorIntensity;
        if ( !m_rgbw )
        {
            w = 0;
            r = g = b = colorIntensity;
        }
        break;

    case FrameBitColor::eOFF:
        break;
    }

    return m_ledDriver.Color( r, g, b, w );
}

///////////////////////////////////////////////////////////
const char* OutputNeoPixel::toString( OutputNeoPixel::Options_T option )
{
    switch ( option )
    {
    case eALL:                   return "all";
    case ePAIRS:                 return "pairs";
    case eQUARTER:               return "quarter";
    case ePAIRS_SPIN_C:          return "2spin";
    case ePAIRS_SPIN_CC:         return "2spincc";
    case eQUARTER_SPIN_CC:       return "4spincc";
    case eQUARTER_SPIN_C:        return "4spin";
    case ePAIRS_FAST_SPIN_C:     return "f2spin";
    case ePAIRS_FAST_SPIN_CC:    return "f2spincc";
    case eQUARTER_FAST_SPIN_CC:  return "f4spincc";
    case eQUARTER_FAST_SPIN_C:   return "f4spin";
    default: break;
    }
    return "unknown";
}