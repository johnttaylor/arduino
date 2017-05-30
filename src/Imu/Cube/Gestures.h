#ifndef Imu_Cube_Gestures_h_
#define Imu_Cube_Gestures_h_
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

#include "Driver/Imu/Vector.h"
#include "Driver/Imu/VectorDebounceWithStability.h"
#include "Driver/Imu/TransposeAxises.h"
#include <stdint.h>


namespace Imu {
namespace Cube {


/** This detects certain gestures/motion from an IMU that is assumed to be
    inside some cube/rectangle object.  In addition, the assumption is made that
    the object is manipulated while resting a on horizontal surface.

    Notes: 
        1. Gravity is measured as force the Cube/IMU is exerting AGAINST
           gravitational force of the earth.
        2. Changes in Aspect State require the 'cube' be stationary in the new 
           state for 3/4 of seconds-ish
        3. A change in reference surface requires that the 'cube' be stationary
           in the new orientation for at least 2 seconds.
        4. When the reference surface is 'eBOTTOM' the tilt Aspect States
           for NORTH/EAST/SOUTH/WEST aren't 100% reliable.
 */
class Gestures
{
public:
    /// Possible aspect states.  
    enum AspectState_T
    {
        eUNKNOWN = 0,       //!< Current state is unknown (e.g. in process of determining the initial state)
        eTILT_NORTH,        //!< Unit is tilted to the north, i.e. negative gravity on the X-axis
        eTILT_EAST,         //!< Unit is tilted to the east, i.e. positive gravity on the Y-axis
        eTILT_SOUTH,        //!< Unit is tilted to the south, i.e. positive gravity on the X-axis
        eTILT_WEST,         //!< Unit is tilted to the west, i.e. negative gravity on the Y-axis
        eFLIP_BOTTOM,       //!< Unit has flipped upside down, i.e. -9.80m/s^2 on the Z-axis
        eHOMED              //!< Restored nominal aspect, i.e. +9.80m/s^2 on the Z-axis
    };

    /// Defines the sides of the 'cube' than contains the IMU
    enum Surface_T
    {
        eNORTH = 0,         //!< The north surface is the side perpendicular to the positive X-Axis, i.e. the positive X-Axis points to the north surface 
        eEAST,              //!< The east surface is the side perpendicular to the negative Y-Axis
        eSOUTH,             //!< The south surface is the side perpendicular to the negative X-Axis
        eWEST,              //!< The west surface is the side perpendicular to the positive Y-Axis
        eBOTTOM,            //!< The bottom surface is the side perpendicular to the negative Z-Axis
        eTOP                //!< The top surface is the side perpendicular to the positive Y-Axis
    };

    typedef struct
    {
        AspectState_T                   m_currentState;     //!< Current aspect
        float                           m_tiltAngle;        //!< Tilt Angle in degrees (from vertical), only valid if the current aspect state is a 'Tilt' state
        Surface_T                       m_currentTop;       //!< Which side/surface (with reference to the nominal aspect/orientation is now currently the 'top' surface)
        Driver::Imu::Vector<int16_t>    m_filteredGravity;  //!< The raw filtered gravity readings from the IMU
    } Event_T;

protected:
    /// States used when tracking surface/axises changes
    enum SurfaceTracking_T
    {
        eSTOPPED=0,                 //!< The current
        eTRACKING_POSITIVE,
        eTRACKING_NEGATIVE
    };

protected:
    /// Transpose function for normalize the absolute surface as the 'top' surface
    Driver::Imu::TransposeAxises<int16_t>*              m_surfaceOrientationPtr;

    /// Filter for IMU data
    Driver::Imu::VectorDebounceWithStability<int16_t>   m_filter;

    /// The minimum change on the X/Y axises to be consider a Tilt Action
    int16_t                     m_tiltThreshold;

    /// The minimum amount off of vertical on the Z-Axis is to be considered a Tilt (in raw IMU units)
    int16_t                     m_verticalThreshold;

    /// Current Aspect state
    AspectState_T               m_aspectState;

    /// Current Tilt Angle
    float                       m_tiltAngle;

    /// Aspect reference
    Surface_T                   m_currentTop;

    /// The amount of time, in milliseconds, to wait before declaring a new 'top' surface
    unsigned long               m_surfaceSwitchDuration;

    /// Timer used to delay declaring the X-Axis the 'new top'
    unsigned long               m_topXStartTime;

    /// Timer used to delay declaring the Y-Axis the 'new top'
    unsigned long               m_topYStartTime;

    /// Timer used to delay declaring the Z-Axis the 'new top'
    unsigned long               m_topZStartTime;

    /// Flag use track the transition to vertical for the X-Axis
    SurfaceTracking_T           m_topXSurfaceFlag;

    /// Flag use track the transition to vertical for the Y-Axis
    SurfaceTracking_T           m_topYSurfaceFlag;

    /// Flag use track the transition to vertical for the Z-Axis
    SurfaceTracking_T           m_topZSurfaceFlag;

    /// Flag used to keep track of the initial do() call
    bool                        m_haveBeenCalledAtLeastOnce;


public:
    /// Constructor
    Gestures( int16_t tiltThreshold, int16_t verticalThreshold, unsigned long surfaceSwitchDurationInMsec );


public:
    /** This method consume the raw IMU data and outputs a new aspect state
        when detected.  The method returns true when there has been change
        in aspect state, tilt angle, etc. Note: A change in only the
        Filtered Gravity measurement will NOT return true.

        This method is assumed that it called at fixed period and provided
        the latest gravity vector from the IMU.
     */
    bool process( Driver::Imu::Vector<int16_t>& gravityVector, Event_T& result );


protected:
    /// Helper method
    bool updateState( AspectState_T newState, float newTiltAngle );

    /// Helper method
    bool checkForTopSurface( int16_t                                 absAxisValue, 
                             int16_t                                 axisValue, 
                             Surface_T                               positiveSurface, 
                             Surface_T                               negativeSurface, 
                             Driver::Imu::TransposeAxises<int16_t>&  positiveTransponse, 
                             Driver::Imu::TransposeAxises<int16_t>&  negativeTransponse, 
                             SurfaceTracking_T&                      surfaceFlag, 
                             unsigned long&                          startTime, 
                             SurfaceTracking_T&                      otherSurfaceFlag1, 
                             SurfaceTracking_T&                      otherSurfaceFlag2  );

};

};      // end Namespaces
};
#endif  // end Header latch