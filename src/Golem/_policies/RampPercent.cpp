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

#include "Golem/RampPercent.h"
#include "Cpl/System/Trace.h"

#define SECT_   "golem::ramp"


/// Namespaces
using namespace Golem;


/////////////////////////////////
RampPercent::RampPercent( float rampTimeAsPercentageOfBitTime )
    : m_rampTime( rampTimeAsPercentageOfBitTime<0.0? 0.0: rampTimeAsPercentageOfBitTime > 0.5? 0.5: rampTimeAsPercentageOfBitTime )
{
}


/////////////////////////////////
uint8_t RampPercent::getIntensity( uint32_t elapsedBitTimeMsec, uint32_t bitSizeInMsec )
{
    uint8_t  rampIntensity = OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY;

    if ( m_rampTime > 0.0 )
    {
        uint32_t rampDuration  = (uint32_t) (bitSizeInMsec * m_rampTime + 0.5);

        if ( elapsedBitTimeMsec < rampDuration )
        {
            rampIntensity = (uint8_t)(((float) elapsedBitTimeMsec / ((float) rampDuration)) * OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY +0.5);
        }
        else if ( elapsedBitTimeMsec > bitSizeInMsec - rampDuration )
        {
            rampIntensity = (uint8_t)(((float) (bitSizeInMsec - elapsedBitTimeMsec) / ((float) rampDuration)) * OPTION_GOLEM_INTENSITY_RAMP_MAX_INTENSITY+0.5);
        }

        CPL_SYSTEM_TRACE_MSG( SECT_, ("getIntensity(). rampTime=%d, rampDuration=%ld, elapsedTime=%ld, bitTime=%ld, intensity=%d",
                              (int)(m_rampTime*100 + 0.5) ,
                              rampDuration,
                              elapsedBitTimeMsec,
                              bitSizeInMsec,
                              rampIntensity));
    }

    return rampIntensity;
}

const char* RampPercent::getDescription( Cpl::Text::String& brief )
{
    brief.format( "RampPercent(%d)", (int)(m_rampTime*100 + 0.5) );
    return brief;
}
