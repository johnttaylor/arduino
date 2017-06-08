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

#include "StreamAddress.h"

using namespace Golem;


/////////////////////////////////////////
StreamAddress::StreamAddress( void* startAddress, void* endAddress )
    : m_endP( (uint8_t*) endAddress )
    , m_curP( (uint8_t*) startAddress )
    , m_bitPos( 0 )
{
}

/////////////////////////////////////////
bool StreamAddress::bitsAreAvailable( void ) const
{
    return m_curP <= m_endP; 
}


bool StreamAddress::getNextBit( void )
{

    uint8_t currentBit = *m_curP & (1 << m_bitPos);

    // Advance my byte/bit position as long as I have NOT exhausted the memory region
    if ( m_curP <= m_endP )
    {
        if ( ++m_bitPos > 7 )
        {
            m_bitPos = 0;
            m_curP++;
        }
    }
    
    return currentBit? true: false;
}

