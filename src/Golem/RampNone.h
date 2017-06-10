#ifndef Golem_RampNone_h_
#define Golem_RampNone_h_
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

#include "Golem/IntensityRamp.h"

/// Namespaces
namespace Golem {


/** This concrete class implements the Golem::IntensityRamp interface with no 
    ramp, i.e. the bit is at 100% intensity for entire duration of the bit time.
 */
class RampNone : public IntensityRamp
{
public:
    /// Constructor
    RampNone();

public:
    /// See Golem::IntensityRampe
    uint8_t getIntensity( uint32_t elapsedBitTimeMsec, uint32_t bitSizeInMsec );
};


};      // end namespace
#endif  // end header latch
