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

#include "Driver/Imu/Bno055/Adafruit.h"


// Calibration constants 
extern Driver::Imu::Bno055::Adafruit::calibration_offsets_t imuCalibrationConstants;
Driver::Imu::Bno055::Adafruit::calibration_offsets_t imuCalibrationConstants = {
    65517,  // accel_offset_x 
    30,     // accel_offset_y 
    17,     // accel_offset_x 
    1,      // gyro_offset_x  
    65535,  // gyro_offset_y  
    3,      // gyro_offset_z  
    65114,  // mag_offset_x   
    168,    // mag_offset_y   
    354,    // mag_offset_z   
    1000,   // accel_radius   
    589,    // mag_radius     
};
