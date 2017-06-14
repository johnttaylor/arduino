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


/** This concrete class implements the Golem::IntensityRamp interface that
    provides a symmetrical ramp (i.e. ramp up, ramp down) where the ramp
    period is a percentage of the total bit time.
 */
class RampPercent : public IntensityRamp
{
protected:
    /// Ramp time (of a single edge) as percentage of total bit time
    float       m_rampTime;


public:
    /// Constructor.  Valid range for 'rampTimeAsPercentageOfBitTime' is [0.0, 0.5]
    RampPercent( float rampTimeAsPercentageOfBitTime );


public:
    /// See Golem::IntensityRamp
    uint8_t getIntensity( uint32_t elapsedBitTimeMsec, uint32_t bitSizeInMsec );

    /// See Golem::IntensityRamp
    const char* getDescription( Cpl::Text::String& brief );

};


};      // end namespace
#endif  // end header latch
