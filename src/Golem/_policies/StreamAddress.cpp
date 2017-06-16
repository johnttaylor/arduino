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

#include "Golem/StreamAddress.h"
#include "Cpl/System/Trace.h"
#include <stdlib.h>

using namespace Golem;

#define SECT_   "streamAddress"

/////////////////////////////////////////
StreamAddress::StreamAddress( const void* startAddress, const void* endAddress, bool loop, bool shuffle )
    : m_startP( (const uint8_t*) startAddress )
    , m_endP( (const uint8_t*) endAddress )
    , m_curP( (const uint8_t*) startAddress )
    , m_loop( loop )
    , m_shuffle( shuffle )
    , m_bitPos( 0 )
{
}

/////////////////////////////////////////
bool StreamAddress::bitsAreAvailable( void ) const
{
    if ( m_loop || m_shuffle )
    {
        return true;
    }
    else
    {
        return m_curP <= m_endP;
    }
}


bool StreamAddress::getNextBit( void )
{

    // Generate a random position if shuffle is on
    if ( m_shuffle )
    {
        // Calculate a random change in bits
        int changeInBits = rand() % ((m_endP - m_startP +1) * 8);
        
        // Adjust Byte offset
        m_curP += changeInBits / 8;
        if ( m_curP > m_endP )
        {
            m_curP = m_startP + (m_curP - m_endP);
        }

        // Adjust bit offset
        m_bitPos += changeInBits % 8;
        if ( m_bitPos >= 8 )
        {
            m_bitPos -=8;
        }
 
        CPL_SYSTEM_TRACE_MSG( SECT_, ("new bit=%d, curP=%p, bitPos=%d", *m_curP & (1 << m_bitPos)?true:false, m_curP, m_bitPos) );
    }

    // Fetch the current bit
    uint8_t currentBit = *m_curP & (1 << m_bitPos);

    // Advance my byte/bit position as long as I have NOT exhausted the memory region
    if ( !m_shuffle && m_curP <= m_endP )
    {
        if ( ++m_bitPos > 7 )
        {
            m_bitPos = 0;
            m_curP++;
            CPL_SYSTEM_TRACE_MSG( SECT_, ("new byte=%X, curP=%p, bitPos=%d", *m_curP, m_curP, m_bitPos) );
        }
    }
    
    // Start over if looping is on
    else if ( m_loop )
    {
        m_curP = m_startP;
    }


    return currentBit ? true : false;
}

const char* StreamAddress::getDescription( Cpl::Text::String& brief )
{
    brief.format( "StreamAddress(%p,%p,%s,%s)", m_startP, m_endP, m_loop?"loop":"once", m_shuffle?"shuffle":"seq" );
    return brief;
}

