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

#include "Golem/FrameSimple.h"


/// Namespaces
using namespace Golem;


//////////////////////////////////////
FrameSimple::FrameSimple( uin32_t bitTimeInMsec, uint8_t numDataBits, uint8_t stopBits, ParityBit_T parity )
    : m_bitTime( bitTimeInMsec > MAXIMUM_BIT_TIME: MAXIMUM_BIT_TIME: numDataBits < MINIMUM_BIT_TIME? MINIMUM_BIT_TIME: bitTimeInMsec )
    , m_stopBits( stopBits > MAXIMUM_STOP_BITS? MAXIMUM_STOP_BITS: stopBits )
    , m_parityBit( parity )
    , m_numDataBits( numDataBits > MAXIMUM_DATA_BITS: MAXIMUM_DATA_BITS: numDataBits < MINIMUM_DATA_BITS? MINIMUM_DATA_BITS: numDataBits )
{
}



//////////////////////////////////////
uint8_t FrameSimple::getNumberOfDataBits( void ) const
{
    return m_numDataBits;
}

uint8_t FrameSimple::getStopBits( void ) const
{
    return m_stopBits;
}

Frame::ParityBit_T FrameSimple::getParity( void ) const
{
    return m_parityBit;
}

uint32_t Frame::getBitTime( void ) const
{
    return m_bitTime;
}
