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

#include "Golem/ColorRainbow.h"

/// Namespaces
using namespace Golem;


//////////////////////////////////
ColorRainbow::ColorRainbow( Color_T startColor, Color_T zeroColor, Color_T oneColor, Color_T parityColor, Color_T stopColor)
    : m_startColor( startColor )
    , m_oneColor( oneColor )
    , m_zeroColor( zeroColor )
    , m_stopColor( stopColor )
    , m_parityColor( parityColor )
{
}


//////////////////////////////////
FrameBitColor::Color_T ColorRainbow::getColor( Frame::Bit_T bitType, bool bitValue, bool newBit )
{
 Color_T color = FrameBitColor::eOFF;

    switch( bitType )
    {
    case Frame::eSTART_BIT: color = m_startColor; break;
    case Frame::eDATA_BIT: color = bitValue? m_oneColor: m_zeroColor; break;
    case Frame::ePARITY_BIT: color = bitValue? m_parityColor: eOFF; break;
    case Frame::eSTOP_BIT: color = m_stopColor; break;
    }

    return color;
}

const char* ColorRainbow::getDescription( Cpl::Text::String& brief )
{
    brief.format( "ColorRainbow(%s,%s,%s,%s,%s)", toString(m_startColor), toString(m_zeroColor), toString(m_oneColor), toString(m_parityColor), toString(m_stopColor));
    return brief;
}
