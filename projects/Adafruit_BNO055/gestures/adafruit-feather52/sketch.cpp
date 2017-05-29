#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/ElaspedTime.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include "Driver/Imu/Bno055/Adafruit.h"
#include "Imu/Cube/Gestures.h"
#include <stdlib.h>


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
/*---------------------------------------------------------------------------*/


// Cpl::System::Trace section identifier
#define SECT_ "sketch"

// Sampling interval in msecs
#define BNO055_SAMPLERATE_DELAY_MS      50

static Driver::Imu::Bno055::Adafruit bno = Driver::Imu::Bno055::Adafruit();
static Driver::Imu::Bno055::Adafruit::calibration_offsets_t my_sensors_calibration_constants ={
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

static unsigned long lastSampleTime_ = 0;

extern uint32_t setLoopStacksize( void );
uint32_t setLoopStacksize( void )
{
    return 512 * 6;
}


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup( void )
{
    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    // Initialize CPL
    Cpl::System::Api::initialize();

    // Use the Trace engine for UART output
    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( SECT_ );
    //CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eNONE );
    CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eINFO );
    //CPL_SYSTEM_TRACE_SET_INFO_LEVEL( Cpl::System::Trace::eBRIEF );

    CPL_SYSTEM_TRACE_MSG( SECT_, ("Orientation Sensor Raw Data Test - IMU/Fusion mode") );


    /* Initialize the IMU sensor */
    if ( !bno.start( Driver::Imu::Bno055::Adafruit::OPERATION_MODE_IMUPLUS ) )
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!") );
        while ( 1 );
    }


    delay( 1000 );

    CPL_SYSTEM_TRACE_MSG( SECT_, ("Setting calibration offsets....") );
    bno.setSensorOffsets( my_sensors_calibration_constants );

    /* Display the current temperature */
    int8_t temp = bno.getTemp();
    CPL_SYSTEM_TRACE_MSG( SECT_, ("Current Temperature: %d 'C\r\n", temp) );

    bno.setExtCrystalUse( true );

    CPL_SYSTEM_TRACE_MSG( SECT_, ("Calibration status values: 0=uncalibrated, 3=fully calibrated") );
    Serial.println( "Time (msec), Gyro x, y, z,  Accel x, y, z,  Filtered Gravity x, y, z,  Action Aspect, Aspect,  Tilt Angle,  Calibration Sys, Gyro, Accel, Mag,  Sampling Time (msec)" );
}




static Imu::Cube::Gestures myGestures( 200, 10 );
static Imu::Cube::Gestures::Event_T gesturesResult;

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop( void )
{
    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    unsigned long timestamp = Cpl::System::ElaspedTime::milliseconds();

    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    Driver::Imu::Bno055::Adafruit::raw_vector_t vgyro  = bno.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_GYROSCOPE );
    Driver::Imu::Bno055::Adafruit::raw_vector_t vaccel = bno.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_LINEARACCEL );
    Driver::Imu::Bno055::Adafruit::raw_vector_t vgrav  = bno.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_GRAVITY );

    /* Display the floating point data */
    int32_t x = (vgyro.x * 100.0 + 0.5);
    int32_t y = (vgyro.y * 100.0 + 0.5);
    int32_t z = (vgyro.z * 100.0 + 0.5);
    Serial.printf( "%15ld, %6d, %6d, %6d,", timestamp, x, y, z );
    //x =  accelFilterX.filterValue( vaccel.x );
    //y =  accelFilterY.filterValue( vaccel.y );
    //z =  accelFilterZ.filterValue( vaccel.z );
    //Serial.printf( "  %6d, %6d, %6d,", x, y, z );

    x = (vaccel.x * 100.0 + 0.5);
    y = (vaccel.y * 100.0 + 0.5);
    z = (vaccel.z * 100.0 + 0.5);
    Serial.printf( "  %6d, %6d, %6d,", x, y, z );
    //x = vgrav.x;
    //y = vgrav.y;
    //z = vgrav.z;
    //Serial.printf( "  %6d, %6d, %6d,", x, y, z );

    bool changed = myGestures.process( vgrav, gesturesResult );
    x =  gesturesResult.m_filteredGravity.x;
    y =  gesturesResult.m_filteredGravity.y;
    z =  gesturesResult.m_filteredGravity.z;
    Serial.printf( "  %6d, %6d, %6d,  %d, %d,  %6d,", x, y, z, (int) (gesturesResult.m_currentState * 50 + 1000), (int) (gesturesResult.m_currentState), (int) (gesturesResult.m_tiltAngle * 100 + 0.5) );


    /* Display calibration status for each sensor. */
    static unsigned long duration = 0;
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration( &system, &gyro, &accel, &mag );
    Serial.printf( "  %d, %d, %d, %d,   %ld\r\n", (int) system, (int) gyro, (int) accel, (int) mag, duration );


    // Enforce monotonic sampling 
    duration = Cpl::System::ElaspedTime::deltaMilliseconds( timestamp );
    if ( duration < BNO055_SAMPLERATE_DELAY_MS )
    {
        Cpl::System::Api::sleep( BNO055_SAMPLERATE_DELAY_MS - duration );
    }
}
