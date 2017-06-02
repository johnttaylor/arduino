/*-----------------------------------------------------------------------------
* This file is part of the Arduino Project.  The Arduino Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/arduino/license.txt
*
* Copyright (c) 2017  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Spinner.h"
#include "Cpl/System/Trace.h"
#include <stdlib.h>

#define SECT_   "imu::motion::cube::spinner"
//#define SECT_   "sketch"


using namespace Imu::Motion::Cube;


///////////////////////////////////////////////////////////////////
Spinner::Spinner( int32_t slowWeight,
                  int32_t normalWeight,
                  int32_t fastWeight,
                  int16_t minimumChangeThreshold,
                  int16_t slowMaximumThreshold,
                  int16_t normalMaxThreshold
                  )
    : m_minimumChangeThreshold( minimumChangeThreshold )
    , m_slowMaximumThreshold( slowMaximumThreshold )
    , m_slowWeight( slowWeight )
    , m_normalMaxThreshold( normalMaxThreshold )
    , m_normalWeight( normalWeight )
    , m_fastWeight( fastWeight )
{
}


///////////////////////////////////////////////////////////////////
int32_t Spinner::process( Driver::Imu::Vector<int16_t>& gyroVector, unsigned rotationalAxis )
{
    int32_t result  = 0;
    int16_t axis    = gyroVector[rotationalAxis];
    int16_t absAxis = abs( axis );

    // Is there sufficient rotation to consider the spinner spinning
    if ( absAxis > m_minimumChangeThreshold )
    {
        // Determine the strength of the rotation
        result = m_slowWeight;
        if ( absAxis > m_slowMaximumThreshold )
        {
            result = m_normalWeight;
            if ( absAxis > m_normalMaxThreshold )
            {
                result = m_fastWeight;
            }
        }

        // Set the direction of rotation
        if ( axis < 0 )
        {
            result *= -1;
        }
    }

    return result;
}



