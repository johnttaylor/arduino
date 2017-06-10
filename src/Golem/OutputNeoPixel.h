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
protected:
    /// NeoPixel driver
    Adafruit_NeoPixel   m_ledDriver;

    /// Configuration.  Set to false if LED strip is RGB; else set to true for RGBW
    bool                m_rgbw;

    /// Keeps track if I have been stopped
    bool                m_stopped;

public:
    /// Constructor
    OutputNeoPixel( uint16_t numberOfLEDs, uint8_t pinNumber, bool isRGBW=false, neoPixelType ledType=NEO_GRB + NEO_KHZ800 );


public:
    /// See Golem::Output
    void write( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType );

    /// See Golem::Output
    void stop( void );

protected:
    /// Helper method
    virtual void setAllLEDs( uint32_t wrgb  );
    
    /// Helper method
    virtual uint32_t convertToWRGB( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType );
};


};      // end namespace
#endif  // end header latch
