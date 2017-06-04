#ifndef Imu_Motion_Cube_Spinner_h_
#define Imu_Motion_Cube_Spinner_h_
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
#include "Driver/Imu/LowPassFilter.h"
#include <stdint.h>


namespace Imu {
namespace Motion {
namespace Cube {


/** This concrete class converts the gyroscope data (from an IMU) into 'spinner'
    that returns a signed unit of change as the IMU is rotated about a single
    axis. When the IMU is rotating, the raw gyroscope data is converted into
    one of three strengths - slow, normal, and fast.  Constructor arguments
    allow the application to define the ranges and the 'weight' associated
    with the three strengths.
 */
class Spinner
{
protected:
    /// Minimum turn rate to be consider the IMU is 'turning'
    int16_t m_minimumChangeThreshold;

    /// Maximum turn rate for the 'slow strength'
    int16_t m_slowMaximumThreshold;

    /// Weight/amount of change associated with the 'slow strength'        
    int32_t m_slowWeight;

    /// Maximum turn rate for the 'normal strength'
    int16_t m_normalMaxThreshold;

    /// Weight/amount of change associated with the 'normal strength'        
    int32_t m_normalWeight;

    /// Weight/amount of change associated with the 'fast strength'        
    int32_t m_fastWeight;

    /// Filter for the raw gyro data
    Driver::Imu::LowPassFilter<int16_t,float>   m_gyroFilter;


public:
    /** Constructor. The 'filterConstant' is the alpha/smoothing-factor to
        be used for Spinner's internal low-pass filter applied to the gyroscope
        data. The constant must be between 0 and 1. The 'thresholds' are in 
        terms of raw gyroscope readings from the IMU. The 'weights' are the +/-
        amounts the Spinner instance returns for each corresponding 'strength' 
        when the IMU is rotating.
     */
    Spinner( float   filterConstant         = 0.2,
             int32_t slowWeight             = 1,
             int32_t normalWeight           = 5,
             int32_t fastWeight             = 50,
             int16_t minimumChangeThreshold = 30,
             int16_t slowMaximumThreshold   = 400,
             int16_t normalMaxThreshold     = 4000
             );


public:
    /** This method consume the raw IMU data and outputs a 'strength' that 
        corresponds the rotation of the IMU.  If the IMU is not being rotated, 
        then zero is returned.  A non-zero positive return value indicates that
        the IMU is being rotated counter-clockwise about the specified 
        rotatinalAxis. A negative value indicates a clockwise rotation.  The 
        'rotationalAxis' parameter is a zero based index that specifics which of
        three IMU axis in the 'gyroVector' to monitor (i.e. '0'=X-Axis, 
        '1'=Y-Axis, '2'=Z-Axis).
     */
    int32_t process( Driver::Imu::Vector<int16_t>& gravityVector, unsigned rotationalAxis=2 );


protected:

};

};      // end Namespaces
};
};
#endif  // end Header latch