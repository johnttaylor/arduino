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


////////////////////////////////////////
OutputNeoPixel::OutputNeoPixel( uint16_t numberOfLEDs, uint8_t pinNumber, bool isRGBW, neoPixelType ledType )
    : m_ledDriver( numberOfLEDs, pinNumber, ledType )
    , m_rgbw( isRGBW )
    , m_stopped( false )
{
    m_ledDriver.begin();
    setAllLEDs( 0x0000 );
    m_ledDriver.show();
}


////////////////////////////////////////
void OutputNeoPixel::write( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType )
{
    setAllLEDs( convertToWRGB( bitColor, colorIntensity, bitType ) );
    m_ledDriver.show();
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
