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
#include "Imu/Motion/Cube/Tilt.h"
#include "Imu/Motion/Cube/Spinner.h"
#include <stdlib.h>
#include "debug.h"


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


//extern uint32_t setLoopStacksize( void );
//uint32_t setLoopStacksize( void )
//{
//    return 512 * 6;
//}


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

      dbgMemInfo();

    CPL_SYSTEM_TRACE_MSG( SECT_, ("Calibration status values: 0=uncalibrated, 3=fully calibrated") );
    Serial.println( "Time (msec), Gyro x, y, z,  Spinner,  Filtered Gravity x, y, z,  Aspect scaled, Surface scaled,  Aspect, Surface, Tilt Angle, Changed,  Sampling Time (msec)" );
}




static Imu::Motion::Cube::Tilt myGestures( 200, 10, 2000 );
static Imu::Motion::Cube::Tilt::Event_T gesturesResult;
static Imu::Motion::Cube::Spinner mySpinner( 0.17F, 2500, 5000, 10000 );


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
    Driver::Imu::Bno055::Adafruit::raw_vector_t vgrav  = bno.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_GRAVITY );
    
    /* Display the floating point data */
    int32_t spin = mySpinner.process( vgyro );
    int32_t x = vgyro.x;
    int32_t y = vgyro.y;
    int32_t z = vgyro.z;
    Serial.printf( "%15ld, %6d, %6d, %6d,  %6d,", timestamp, x, y, z, spin );

    bool changed = myGestures.process( vgrav, gesturesResult );
    x =  gesturesResult.m_filteredGravity.x;
    y =  gesturesResult.m_filteredGravity.y;
    z =  gesturesResult.m_filteredGravity.z;
    Serial.printf( "  %6d, %6d, %6d,  %d, %d,  %d,%d,%6d,%d,", x, y, z, (int) (gesturesResult.m_currentState * 50 + 1000), (int) (gesturesResult.m_currentTop * -50 - 1000), (int) (gesturesResult.m_currentState), (int)(gesturesResult.m_currentTop), (int) (gesturesResult.m_tiltAngle * 100 + 0.5), (int)changed );


    /* Display calibration status for each sensor. */
    static unsigned long duration = 0;
    //uint8_t system, gyro, accel, mag = 0;
    //bno.getCalibration( &system, &gyro, &accel, &mag );
    //Serial.printf( "  %d, %d, %d, %d,   %ld\r\n", (int) system, (int) gyro, (int) accel, (int) mag, duration );
    Serial.printf( "  %ld\r\n", duration );


    // Enforce monotonic sampling 
    duration = Cpl::System::ElaspedTime::deltaMilliseconds( timestamp );
    if ( duration < BNO055_SAMPLERATE_DELAY_MS )
    {
        Cpl::System::Api::sleep( BNO055_SAMPLERATE_DELAY_MS - duration );
    }
}
