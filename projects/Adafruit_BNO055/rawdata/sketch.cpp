#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (50)

Adafruit_BNO055 bno = Adafruit_BNO055();

#if BUILD_OPT_CALIBRATED
static adafruit_bno055_offsets_t my_sensors_calibration_constants ={
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
#endif

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup( void )
    {
    Serial.begin( 115200 );
    Serial.println( "Orientation Sensor Raw Data Test - IMU/Fusion mode" ); Serial.println( "" );

    /* Initialise the sensor */
    if (!bno.begin( Adafruit_BNO055::OPERATION_MODE_IMUPLUS ))
        {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print( "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!" );
        while (1);
        }


    delay( 1000 );

#if BUILD_OPT_CALIBRATED
    Serial.println( "Setting calibration offsets...." );
    bno.setSensorOffsets( my_sensors_calibration_constants );
#endif

    /* Display the current temperature */
    int8_t temp = bno.getTemp();
    Serial.print( "Current Temperature: " );
    Serial.print( temp );
    Serial.println( " C" );
    Serial.println( "" );

    bno.setExtCrystalUse( true );

    Serial.println( "Calibration status values: 0=uncalibrated, 3=fully calibrated" );
    Serial.println( "Time (msec), Gyro x, y, z, Accel x, y, z, Gravity x, y, z, Calbiration Sys, Gyro, Accel, Mag" );
    }

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop( void )
    {

    uint32_t timestamp = millis();

    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> vgyro  = bno.getVector( Adafruit_BNO055::VECTOR_GYROSCOPE );
    imu::Vector<3> vaccel = bno.getVector( Adafruit_BNO055::VECTOR_LINEARACCEL );
    imu::Vector<3> vgrav  = bno.getVector( Adafruit_BNO055::VECTOR_GRAVITY );

    /* Display the floating point data */
    int32_t x = (vgyro.x() * 100.0 + 0.5);
    int32_t y = (vgyro.y() * 100.0 + 0.5);
    int32_t z = (vgyro.z() * 100.0 + 0.5);
    Serial.printf( "%15ld, %6d, %6d, %6d,", timestamp, x, y, z );
    x = (vaccel.x() * 100.0 + 0.5);
    y = (vaccel.y() * 100.0 + 0.5);
    z = (vaccel.z() * 100.0 + 0.5);
    Serial.printf( "  %6d, %6d, %6d,", x, y, z );
    x = (vgrav.x() * 100.0 + 0.5);
    y = (vgrav.y() * 100.0 + 0.5);
    z = (vgrav.z() * 100.0 + 0.5);
    Serial.printf( "  %6d, %6d, %6d,", x, y, z );


    /* Display calibration status for each sensor. */
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration( &system, &gyro, &accel, &mag );
    Serial.printf( "  %d, %d, %d, %d\r\n", system, gyro, accel, mag );

#ifndef BUILD_OPT_DATA_ONLY
    static bool calibrated = false;
    if (bno.isFullyCalibrated()) {
        if (!calibrated) {
            calibrated = true;
            adafruit_bno055_offsets_t cal_offset;
            bno.getSensorOffsets( cal_offset );
            Serial.printf( "accel_offset_x = %d\r\n", cal_offset.accel_offset_x );
            Serial.printf( "accel_offset_y = %d\r\n", cal_offset.accel_offset_y );
            Serial.printf( "accel_offset_x = %d\r\n", cal_offset.accel_offset_z );
            Serial.printf( "gyro_offset_x  = %d\r\n", cal_offset.gyro_offset_x );
            Serial.printf( "gyro_offset_y  = %d\r\n", cal_offset.gyro_offset_y );
            Serial.printf( "gyro_offset_z  = %d\r\n", cal_offset.gyro_offset_z );
            Serial.printf( "mag_offset_x   = %d\r\n", cal_offset.mag_offset_x );
            Serial.printf( "mag_offset_y   = %d\r\n", cal_offset.mag_offset_y );
            Serial.printf( "mag_offset_z   = %d\r\n", cal_offset.mag_offset_z );
            Serial.printf( "accel_radius   = %d\r\n", cal_offset.accel_radius );
            Serial.printf( "mag_radius     = %d\r\n", cal_offset.mag_radius );
            }
        }
    else {
        calibrated = false;
        }
#endif

    delay( BNO055_SAMPLERATE_DELAY_MS );
    }
