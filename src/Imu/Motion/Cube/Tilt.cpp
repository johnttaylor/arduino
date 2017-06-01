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

#include "Tilt.h"
#include "Cpl/System/ElaspedTime.h"
#include "Cpl/System/Trace.h"
#include <stdlib.h>
#include <math.h>

#define SECT_   "imu::motion::cube::gestures"
//#define SECT_   "sketch"

#define GRAVITY_CONSTANT        980.0
#define CONVERT_RADS_TO_DEGREE  ((float)(180.0/3.14159265358979323846))

using namespace Imu::Motion::Cube;

///////////////////////////////////////////////////////////////////
namespace {

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the North surface is the 'top'
class Transpose4NorthTop : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgX = original.x;
        int16_t orgZ = original.z;

        original.x   = orgZ;
        original.z   = -orgX;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the South surface is the 'top'
class Transpose4SouthTop : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgX = original.x;
        int16_t orgZ = original.z;

        original.x   = -orgZ;
        original.z   = orgX;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the East surface is the 'top'
class Transpose4EastTop : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgY = original.y;
        int16_t orgZ = original.z;

        original.y   = -orgZ;
        original.z   = orgY;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the West surface is the 'top'
class Transpose4WestTop : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgY = original.y;
        int16_t orgZ = original.z;

        original.y   = orgZ;
        original.z   = -orgY;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the Bottom surface is the 'top'
class Transpose4BottomTopFlippedXAxis : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgX = original.x;
        int16_t orgZ = original.z;

        original.x   = -orgX;
        original.z   = -orgZ;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the Bottom surface is the 'top'
class Transpose4BottomTopFlippedYAxis : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original )
    {
        int16_t orgY = original.y;
        int16_t orgZ = original.z;

        original.y   = -orgY;
        original.z   = -orgZ;
    }
};

/// Transpose function (to normalize the IMU vector with respect to the tilt algorithm) for the Top surface is the 'top'
class Transpose4TopTop : public Driver::Imu::TransposeAxises<int16_t>
{
public:
    void transpose( Driver::Imu::Vector<int16_t>& original ) {}
};

}; // end namespace


static Transpose4NorthTop               transposeNorth_;
static Transpose4SouthTop               transposeSouth_;
static Transpose4EastTop                transposeEast_;
static Transpose4WestTop                transposeWest_;
static Transpose4BottomTopFlippedYAxis  transposeBottomFlippedY_;
static Transpose4BottomTopFlippedXAxis  transposeBottomFlippedX_;
static Transpose4TopTop                 transposeTop_;


///////////////////////////////////////////////////////////////////
Tilt::Tilt( int16_t tiltThreshold, int16_t verticalThreshold, unsigned long surfaceSwitchDurationInMsec )
    : m_surfaceOrientationPtr( &transposeTop_ )
    , m_filter( 10, 1, 2, true )
    , m_tiltThreshold( tiltThreshold )
    , m_verticalThreshold( verticalThreshold )
    , m_aspectState( eUNKNOWN )
    , m_tiltAngle( 0 )
    , m_currentTop( eTOP )
    , m_surfaceSwitchDuration( surfaceSwitchDurationInMsec )
    , m_haveBeenCalledAtLeastOnce( false )
{
}


///////////////////////////////////////////////////////////////////
bool Tilt::process( Driver::Imu::Vector<int16_t>& gravityVector, Event_T& result )
{
    Driver::Imu::Vector<int16_t>  filteredVector = gravityVector;
    AspectState_T                 newAspect      = eUNKNOWN;
    float                         newTiltAngle   = 0.0;
    bool                          changed        = false;

    // First time I am called
    if ( !m_haveBeenCalledAtLeastOnce )
    {
        m_haveBeenCalledAtLeastOnce = true;
        changed                     = true;
    }

    // Run the algorithm
    else
    {
        // Debounce and only use stable readings
        filteredVector = m_filter.filterValue( gravityVector );

        // Normalize the raw IMU data for my 'top' orientation
        Driver::Imu::Vector<int16_t> normalizedVector = filteredVector;
        m_surfaceOrientationPtr->transpose( normalizedVector );

        // Calculate change
        int16_t absX = abs( normalizedVector.x );
        int16_t absY = abs( normalizedVector.y );

        // Was there sufficient change to take action on?
        if ( absX > m_tiltThreshold || absY > m_tiltThreshold || normalizedVector.z < -m_verticalThreshold )
        {
            // Change was a 'tilt'
            if ( normalizedVector.z >= -m_tiltThreshold && normalizedVector.z < ((int) GRAVITY_CONSTANT - m_verticalThreshold) )
            {
                // X axis had the strongest change
                if ( absX >= absY )
                {
                    newAspect    = normalizedVector.x < 0 ? eTILT_NORTH : eTILT_SOUTH;
                    newTiltAngle = asin( abs( normalizedVector.x ) / GRAVITY_CONSTANT ) * CONVERT_RADS_TO_DEGREE;
                    changed      = updateState( newAspect, newTiltAngle );
                }

                // Y axis had the strongest change
                else
                {
                    newAspect    = normalizedVector.y < 0 ? eTILT_WEST : eTILT_EAST;
                    newTiltAngle = asin( abs( normalizedVector.y ) / GRAVITY_CONSTANT ) * CONVERT_RADS_TO_DEGREE;
                    changed      = updateState( newAspect, newTiltAngle );
                }
            }

            // Z axis had the strongest change
            else
            {
                newAspect = normalizedVector.z < 0 ? eFLIP_BOTTOM : eHOMED;
                changed   = updateState( newAspect, 0.0F );
            }
        }

        // Trap return to vertical
        else if ( normalizedVector.z >= ((int) GRAVITY_CONSTANT - m_verticalThreshold) )
        {
            newAspect = normalizedVector.z < 0 ? eFLIP_BOTTOM : eHOMED;
            changed   = updateState( newAspect, 0.0F );
        }


        // Check for change in the 'top surface'
        if ( m_aspectState != eHOMED )
        {
            Driver::Imu::TransposeAxises<int16_t>* currentOrientationPtr = m_surfaceOrientationPtr;
            Surface_T                              currentSurface        = m_currentTop;
            AspectState_T                          currentAspect         = m_aspectState;
            bool                                   currentChanged        = changed;

            changed = checkForTopSurface( abs(filteredVector.x), filteredVector.x, eSOUTH, eNORTH, transposeSouth_, transposeNorth_, m_topXSurfaceFlag, m_topXStartTime, m_topYSurfaceFlag, m_topZSurfaceFlag ) ? true : changed;
            changed = checkForTopSurface( abs(filteredVector.y), filteredVector.y, eEAST, eWEST, transposeEast_, transposeWest_, m_topYSurfaceFlag, m_topYStartTime, m_topXSurfaceFlag, m_topZSurfaceFlag ) ? true : changed;
            if ( checkForTopSurface( abs(filteredVector.z), filteredVector.z, eTOP, eBOTTOM, transposeTop_, transposeBottomFlippedY_, m_topZSurfaceFlag, m_topZStartTime, m_topYSurfaceFlag, m_topXSurfaceFlag ) )
            {
                changed = true;

                // Handle the special case of flipping to the bottom because it matters which axis I flipped on
                if ( m_currentTop == eBOTTOM )
                {
                    // Flipped over the X-Axis
                    if ( filteredVector.x < 0 && filteredVector.y >= 0 )
                    {
                        m_surfaceOrientationPtr = &transposeBottomFlippedX_;
                    }
                    
                    // Flipped over the Y-Axis
                    
                    else if ( filteredVector.x >= 0 && filteredVector.y < 0 ) 
                    {
                      m_surfaceOrientationPtr = &transposeBottomFlippedY_;
                    }

                    // Can't distinguish (e.g. the 'bottom' is not level) -->stay with current surface and try again
                    else
                    {
                        m_currentTop            = currentSurface;
                        m_surfaceOrientationPtr = currentOrientationPtr;
                        m_aspectState           = currentAspect;
                        changed                 = currentChanged;
                    }
                }

            }
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
bool Tilt::checkForTopSurface( int16_t                                 absAxisValue,
                                   int16_t                                 axisValue,
                                   Surface_T                               positiveSurface,
                                   Surface_T                               negativeSurface,
                                   Driver::Imu::TransposeAxises<int16_t>&  positiveTransponse,
                                   Driver::Imu::TransposeAxises<int16_t>&  negativeTransponse,
                                   SurfaceTracking_T&                      surfaceFlag,
                                   unsigned long&                          startTime,
                                   SurfaceTracking_T&                      otherSurfaceFlag1,
                                   SurfaceTracking_T&                      otherSurfaceFlag2 )
{
    bool result = false;
    if ( absAxisValue >= ((int) GRAVITY_CONSTANT - m_verticalThreshold) )
    {
        // Reset the other surface flags
        otherSurfaceFlag1 = eSTOPPED;
        otherSurfaceFlag2 = eSTOPPED;

        // Start my time on the transition to vertical
        if ( surfaceFlag == eSTOPPED )
        {
            startTime   = Cpl::System::ElaspedTime::milliseconds();
            surfaceFlag = axisValue < 0 ? eTRACKING_NEGATIVE : eTRACKING_POSITIVE;
            CPL_SYSTEM_TRACE_MSG( SECT_, ("Starting TopSurface timer. abs=%d, ax=%d, flag=%d, posSurface=%d, negSurface=%d", absAxisValue, axisValue, surfaceFlag, positiveSurface, negativeSurface) );
        }


        // Has the timer expired?
        if ( surfaceFlag != eSTOPPED && Cpl::System::ElaspedTime::expiredMilliseconds( startTime, m_surfaceSwitchDuration ) )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ("TopSurface timer Expired. abs=%d, ax=%d, posSurface=%d, negSurface=%d", absAxisValue, axisValue, positiveSurface, negativeSurface) );

            // Default to the current surface
            Surface_T newSurface = m_currentTop;

            // Negative AXIS
            if ( axisValue < 0 )
            {
                // Filter out the use case of a 180' degree flip on the same axis
                if ( surfaceFlag == eTRACKING_NEGATIVE )
                {
                    m_surfaceOrientationPtr = &negativeTransponse;
                    newSurface              = negativeSurface;
                }
            }

            // Positive AXIS
            else
            {
                // Filter out the use case of a 180' degree flip on the same axis
                if ( surfaceFlag == eTRACKING_POSITIVE )
                {
                    m_surfaceOrientationPtr = &positiveTransponse;
                    newSurface              = positiveSurface;
                }
            }

            if ( newSurface != m_currentTop )
            {
                result          = true;
                m_aspectState   = eHOMED;
            }
            m_currentTop = newSurface;
            surfaceFlag  = eSTOPPED;
        }
    }
    else
    {
        surfaceFlag = eSTOPPED;
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Stopping TopSurface timer. abs=%d, ax=%d, posSurface=%d, negSurface=%d", absAxisValue, axisValue, positiveSurface, negativeSurface) );
    }

    return result;
}

bool Tilt::updateState( AspectState_T newState, float newTiltAngle )
{
    bool result = false;
    if ( newState != m_aspectState || newTiltAngle != m_tiltAngle )
    {
        result = true;
    }
    m_aspectState    = newState;
    m_tiltAngle      = newTiltAngle;

    return result;
}

