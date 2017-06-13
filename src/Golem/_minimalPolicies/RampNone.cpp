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

#include "Golem/RampNone.h"

/// Namespaces
using namespace Golem;


/////////////////////////////////
RampNone::RampNone(){}

/////////////////////////////////
uint8_t RampNone::getIntensity( uint32_t elapsedBitTimeMsec, uint32_t bitSizeInMsec )
{
    return OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY;
}

const char* RampNone::getDescription( Cpl::Text::String& brief )
{
    brief = "RampNone()";
    return brief;
}
