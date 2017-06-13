#ifndef Golem_FrameSimple_h_
#define Golem_FrameSimple_h_
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

#include "Golem/Frame.h"


/// Namespaces
namespace Golem {


/** This concrete class provide a basic/simple implementation of the Golem::Frame
    interface. A class instance is fully configured via the constructor.
 */
class FrameSimple : public Frame
{
protected:
    ///
    FrameConfig_T m_config;

public:
    /// Constructor
    FrameSimple( uint32_t bitTimeInMsec, uint8_t numDataBits=8, uint8_t stopBits=1, ParityBit_T parity=eNONE );

public:
    /// See Golem::Frame
    uint8_t getNumberOfDataBits( void ) const;

    /// See Golem::Frame
    uint8_t getStopBits( void ) const;

    /// See Golem::Frame
    ParityBit_T getParity( void ) const;

    /// See Golem::Frame
    uint32_t getBitTime( void ) const;
    
    /// See Golem::Frame
    FrameConfig_T getConfig( void ) const;
    
    /// See Golem::Frame
    const char* getDescription( Cpl::Text::String& brief );
};


};      // end namespace
#endif  // end header latch
