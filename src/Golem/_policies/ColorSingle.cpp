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

#include "Golem/ColorSingle.h"

/// Namespaces
using namespace Golem;


//////////////////////////////////
ColorSingle::ColorSingle( Color_T onColor, Color_T offColor )
    : m_onColor( onColor )
    , m_offColor( offColor )
{
}


//////////////////////////////////
FrameBitColor::Color_T ColorSingle::getColor( Frame::Bit_T bitType, bool bitValue )
{
    return bitValue? m_onColor: m_offColor;
}

const char* ColorSingle::getDescription( Cpl::Text::String& brief )
{
    brief.format( "ColorSingle(%s,%s)", toString(m_onColor), toString(m_offColor) );
    return brief;
}
