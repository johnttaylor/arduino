#ifndef Golem_DataStream_h_
#define Golem_DataStream_h_
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

#include "Cpl/Text/String.h"


/// Namespaces
namespace Golem {


/** This class defines the interface for accessing bits in the Golem data
    stream.

    NOTES:
    Frame
    Data: 	4 - 16
    Parity: N,E,O
    Stop:   1,2


    Data Stream:
    start address, end address
    sequence - once
    sequence - loop
    shuffle - n
    shuffle - loop


 */
class DataStream
{
public:
    /// Returns true if there is at least one data bit remaining the data stream
    virtual bool bitsAreAvailable( void ) const = 0;

    /// Gets the next bit in the data stream
    virtual bool getNextBit( void ) = 0;

public:
    /** This method returns a brief description of the current policy. The
     call is responsible for provide the memory/string that returns
     the description.
     */
    virtual const char* getDescription( Cpl::Text::String& brief ) = 0;


public:
    /// Virtual destructor
    virtual ~DataStream() {}
};


};      // end namespace
#endif  // end header latch
