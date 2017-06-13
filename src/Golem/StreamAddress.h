#ifndef Golem_StreamAddress_h_
#define Golem_StreamAddress_h_
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

#include "Golem/DataStream.h"
#include <stdint.h>


/// Namespaces
namespace Golem {


/** This concrete class implements the Golem::DataStream by creating an
    finite data stream from a region of memory specified by a
    start and stop addresses.
 */
class StreamAddress : public DataStream
{
protected:
    ///
    const uint8_t*  m_startP;
    ///
    const uint8_t*  m_endP;
    ///
    const uint8_t*  m_curP;
    ///
    uint8_t         m_bitPos;

public:
    /// Constructor
    StreamAddress( const void* startAddress, const void* endAddress );

public:
    /// See Golem::DataStream
    bool bitsAreAvailable( void ) const;

    /// See Golem::DataStream
    bool getNextBit( void );

    /// See Golem::DataStream
    void getDescription( Cpl::Text::String& brief );

};


};      // end namespace
#endif  // end header latch
