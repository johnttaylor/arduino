#ifndef Golem_ColorSingle_h_
#define Golem_ColorSingle_h_
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
class ColorSingle : public FrameBitColor
{
protected:
    /// Color for an '1' bit
    Color_T m_onColor;

    /// Color for an '0' bit
    Color_T m_offColor;

public:
    /** Constructor. The 'offColor' argument is intended to allow the
        application to 'invert' the On/Off logic.
     */
    ColorSingle( Color_T onColor, Color_T offColor=eOFF );

public:
    /// See Golem::FrameBitColor
    Color_T getColor( Frame::Bit_T bitType, bool bitValue );
};


};      // end namespace
#endif  // end header latch
