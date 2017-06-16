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

#include "Golem/ColorStream.h"

/// Namespaces
using namespace Golem;

static const char* toString_( ColorStream::Sequence_T value );

//////////////////////////////////
ColorStream::ColorStream( Sequence_T  option, bool fast )
    : m_option( option )
    , m_currentColor( Color_T::eOFF )
    , m_fast( fast )
{
}


//////////////////////////////////
FrameBitColor::Color_T ColorStream::getColor( Frame::Bit_T bitType, bool bitValue, bool newBit )
{
    Color_T color        = m_currentColor;
    bool    advanceColor = false;

    if ( bitValue )
    {
        if ( m_option == eON_BITS || m_option == eALL_BITS )
        {
            advanceColor = true;
        }
        else
        {
            color = eOFF;
        }
    }
    else
    {
        if ( m_option == eOFF_BITS || m_option == eALL_BITS )
        {
            advanceColor = true;
        }
        else
        {
            color = eOFF;
        }
    }

    if ( (newBit || m_fast) && advanceColor )
    {
        switch ( m_currentColor )
        {
        case eRED: m_currentColor = eGREEN; break;
        case eGREEN: m_currentColor = eBLUE; break;
        case eBLUE: m_currentColor = eWHITE; break;
        case eWHITE: m_currentColor = eOFF; break;
        case eOFF: m_currentColor = eRED; break;
        }
        if ( m_option != eALL_BITS && m_currentColor == eOFF )
        {
            m_currentColor = eRED;
        }
    }

    return color;
}

const char* ColorStream::getDescription( Cpl::Text::String& brief )
{
    brief.format( "ColorStream(%s,%s)", toString_( m_option ), m_fast?"fast":"slow" );
    return brief;
}

const char* toString_( ColorStream::Sequence_T value )
{
    switch( value )
    {
    case ColorStream::Sequence_T::eON_BITS: return "onbits";    
    case ColorStream::Sequence_T::eOFF_BITS: return "offbits";    
    case ColorStream::Sequence_T::eALL_BITS: return "allbits";    
    }

    // Should never get here!
    return "??";
}
