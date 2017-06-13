#ifndef Golem_OutputNeoPixel_h_
#define Golem_OutputNeoPixel_h_
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
/** @file */

#include "Golem/Output.h"
#include "Adafruit_NeoPixel.h"


/// Namespaces
namespace Golem {


/** The concrete class define implements the Golem::Output interface using the
    one of a NeoPixel LED Strip. It treats ALL LEDs in the strip as a single
    LED, i.e. all the same color/intensity.  
    
    Note: This class uses the NeoPixel driver provided by Adafruit which has
          direct dependencies on the Arduino framework as well as having a 
          GPL Lesser license.
 */
class OutputNeoPixel : public Output
{
public:
    /// LED Output options for which LEDs are turned on/off
    typedef enum 
    {
        eALL,               //!< Use all LEDs
        ePAIRS,             //!< Use half of LEDs -- in groups of two (on and off)
        eQUARTER,           //!< Use only every fourth LED.
        ePAIRS_SPIN_C,      //!< Use half (in pairs) of the LEDs and spin clockwise on each bit
        ePAIRS_SPIN_CC,     //!< Use half (in pairs) of the LEDs and spin counter clockwise on each bit
        eQUARTER_SPIN_C,    //!< Use Quartets (4 LEDs at a time) of the LEDs and spin clockwise on each bit
        eQUARTER_SPIN_CC    //!< Use Quartets (4 LEDs at a time) of the LEDs and spin counter clockwise on each bit
    } Options_T;

    
protected:
    /// NeoPixel driver
    Adafruit_NeoPixel   m_ledDriver;

    /// Output option
    Options_T           m_option;

    /// Current starting LED position
    uint16_t            m_currentLed;

    /// Configuration.  Set to false if LED strip is RGB; else set to true for RGBW
    bool                m_rgbw;

    /// Keeps track if I have been stopped
    bool                m_stopped;

public:
    /// Constructor
    OutputNeoPixel( Options_T option, uint16_t numberOfLEDs, uint8_t pinNumber, bool isRGBW=false, neoPixelType ledType=NEO_GRB + NEO_KHZ800 );

    /// Destructor
    ~OutputNeoPixel();

public:
    /// See Golem::Output
    void write( bool newBit, FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType );

    /// See Golem::Output
    void stop( void );

    /// See Golem::Output
    void getDescription( Cpl::Text::String& brief );

protected:
    /// Helper method
    virtual void setAllLEDs( uint32_t wrgb  );
    
    /// Helper method
    virtual uint32_t convertToWRGB( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType );

    /// Helper method
    void setPairSpinLED( uint16_t baseLed, uint16_t phase, uint32_t color );
    
    /// Helper method
    void setQuarterSpinLED( uint16_t baseLed, uint16_t phase, uint32_t color );

};


};      // end namespace
#endif  // end header latch
