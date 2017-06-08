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

#include "ColorSingle.h"

/// Namespaces
using namespace Golem;


//////////////////////////////////
ColorSingle::ColorSingle( Color_T onColor, Color_T offColor )
    : m_onColor( onColor )
    , m_offColor( offColor )
{
}


//////////////////////////////////
FrameBitColor::Color_t getColor( Frame::Bit_T bitType, bool bitValue, bool invert )
{
    return bitValue? m_onColor: m_offColor;
}

