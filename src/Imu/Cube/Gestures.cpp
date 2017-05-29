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

#include "Gestures.h"
#include <stdlib.h>
#include <math.h>


#define GRAVITY_CONSTANT        980.0
#define CONVERT_RADS_TO_DEGREE  ((float)(180.0/3.14159265358979323846))

using namespace Imu::Cube;


///////////////////////////////////////////////////////////////////

Gestures::Gestures( int16_t tiltThreshold, int16_t verticalThreshold )
    : m_filter( 10, 1, 2, true )
    , m_tiltThreshold( tiltThreshold )
    , m_verticalThreshold( verticalThreshold )
    , m_aspectState( eUNKNOWN )
    , m_tiltAngle( 0 )
    , m_currentTop( eTOP )
    , m_haveBeenCalledAtLeastOnce( false )
{
}


///////////////////////////////////////////////////////////////////
bool Gestures::process( Driver::Imu::Vector<int16_t>& gravityVector, Event_T& result )
{
    Driver::Imu::Vector<int16_t>  filteredVector = gravityVector;
    AspectState_T                 newAspect      = eUNKNOWN;
    float                         newTiltAngle   = 0.0;
    Surface_T                     newTop         = m_currentTop; // TODO: Need to implement 'new top' feature!
    bool                          changed        = false;

    // First time I am called
    if ( !m_haveBeenCalledAtLeastOnce )
    {
        m_haveBeenCalledAtLeastOnce = true;
        m_currentTop                = eTOP;
        changed                     = true;
    }

    // Run the algorithm
    else
    {
        // Debounce and only use stable readings
        filteredVector = m_filter.filterValue( gravityVector );

        // Calculate change
        int16_t absX    = abs( filteredVector.x );
        int16_t absY    = abs( filteredVector.y );
        int16_t absZ    = abs( filteredVector.z );

        // Was there sufficient change to take action on?
        if ( absX > m_tiltThreshold || absY > m_tiltThreshold || filteredVector.z < -m_verticalThreshold )
        {
            // Change was a 'tilt'
            if ( filteredVector.z >= -m_verticalThreshold && filteredVector.z < ((int) GRAVITY_CONSTANT - m_verticalThreshold) )
            {
                // X axis had the strongest change
                if ( absX >= absY )
                {
                    newAspect    = filteredVector.x < 0 ? eTILT_NORTH : eTILT_SOUTH;
                    newTiltAngle = asin( abs( filteredVector.x ) / GRAVITY_CONSTANT ) * CONVERT_RADS_TO_DEGREE;
                    updateState( newAspect, newTiltAngle );
                }

                // Y axis had the strongest change
                else
                {
                    newAspect    = filteredVector.y < 0 ? eTILT_WEST : eTILT_EAST;
                    newTiltAngle = asin( abs( filteredVector.y ) / GRAVITY_CONSTANT ) * CONVERT_RADS_TO_DEGREE;
                    updateState( newAspect, newTiltAngle );
                }
            }

            // Z axis had the strongest change
            else
            {
                newAspect = filteredVector.z < 0 ? eFLIP_BOTTOM : eHOMED;
                updateState( newAspect, 0.0F );
            }
        }

        // Trap return to vertical
        else if ( filteredVector.z >= ((int) GRAVITY_CONSTANT - m_verticalThreshold) )
        {
            newAspect = filteredVector.z < 0 ? eFLIP_BOTTOM : eHOMED;
            updateState( newAspect, 0.0F );
        }
    }

    // Return the results
    result.m_currentState    = m_aspectState;
    result.m_tiltAngle       = m_tiltAngle;
    result.m_currentTop      = m_currentTop;
    result.m_filteredGravity = filteredVector;
    return changed;
}

///////////////////////////////////////////////////////////////////
bool Gestures::updateState( AspectState_T newState, float newTiltAngle )
{
    bool result = false;
    if ( newState != m_aspectState || newTiltAngle != m_tiltAngle )
    {
        result = true;
    }
    m_aspectState    = newState;
    m_tiltAngle      = newTiltAngle;
}

