#ifndef Golem_IntensityRamp_h_
#define Golem_IntensityRamp_h_
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

#include "colony_config.h"


/** This constant defines the minimum possible intensity value for the
    system.
 */
#ifndef OPTION_GOLEM_INTENSITY_RAMP_MIN_INTENSITY
#define OPTION_GOLEM_INTENSITY_RAMP_MIN_INTENSITY       0
#endif

/** This constant defines the maximum possible intensity value for the
    system.
 */
#ifndef OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY
#define OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY       255
#endif

/** This constant defines the 'mid-point' intensity value for the
    system.
 */
#ifndef OPTION_GOLEM_INTENSITY_RAMP_MID_INTENSITY
#define OPTION_GOLEM_INTENSITY_RAMP_MID_INTENSITY       127
#endif


/// Namespaces
namespace Golem {


/** This class defines the interface for calculating the color intensity of
    current bit in the data frame.

    NOTES:
    Ramp
    -----------
    off
    10%
    25%
    33%
 */
class IntensityRamp
{
public:
    /** Returns the intensity value based on how much 'bit time' has elapsed.
        A return value of 0 is 'off' and value of 255 is 'max intensity'
     */
    virtual uint8_t getIntensity( uint32_t elapsedBitTimeMsec, uint32_t bitSizeInMsec ) = 0;

public:
    /// Virtual Destructor
    virtual ~IntensityRamp(){}
};


};      // end namespace
#endif  // end header latch
