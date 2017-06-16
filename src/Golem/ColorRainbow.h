#ifndef Golem_ColorRainbow_h_
#define Golem_ColorRainbow_h_
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

#include "Golem/FrameBitColor.h"

/// Namespaces
namespace Golem {


/** This concrete class implements the Golem::FrameBitColor using a single
    color.  A '1' bit is the color and '0' bit is off.
 */
class ColorRainbow : public FrameBitColor
{
protected:
    /// Color for a start bit
    Color_T m_startColor;

    /// Color for a '1' data bit
    Color_T m_oneColor;
    
    /// Color for a '0' data bit
    Color_T m_zeroColor;
    
    /// Color for a stop bit
    Color_T m_stopColor;

    /// Color for a parity bit
    Color_T m_parityColor;
public:
    /** Constructor. The 'offColor' argument is intended to allow the
        application to 'invert' the On/Off logic.
     */
    ColorRainbow( Color_T startColor=eRED, Color_T zeroColor=eGREEN, Color_T oneColor=eBLUE, Color_T parityColor=eWHITE, Color_T stopColor=eOFF );

public:
    /// See Golem::FrameBitColor
    Color_T getColor( Frame::Bit_T bitType, bool bitValue, bool newBit );

    /// See Golem::FrameBitColor
    const char* getDescription( Cpl::Text::String& brief );

};


};      // end namespace
#endif  // end header latch
