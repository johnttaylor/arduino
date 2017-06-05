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

#include "gestures.h"
#include "Bsp/Api.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Mutex.h"
#include "Cpl/Container/RingBuffer.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/ElaspedTime.h"
#include "Driver/Imu/Bno055/Adafruit.h"
#include "Imu/Motion/Cube/Spinner.h"
#include <stdlib.h>
#include "Arduino.h"
#include <Wire.h>



// Get access to the serial port
extern Cpl::Io::InputOutput& Bsp_Serial( void );

// Cpl::System::Trace section identifier
#define SECT_ "gestures"

// Calibration constants are stored in the own file...
extern Driver::Imu::Bno055::Adafruit::calibration_offsets_t imuCalibrationConstants;



////////////////////////////////////////////////////////////////////////////////
namespace {


class SubSystem : public Cpl::System::Runnable
{
protected:
    ///
    Driver::Imu::Bno055::Adafruit       m_imu;
    ///
    Imu::Motion::Cube::Tilt             m_tiltMotions;
    ///
    Imu::Motion::Cube::Spinner          m_spinner;
    ///
    Cpl::Container::RingBuffer<GestureEvent_t>  m_events;
    ///
    Cpl::System::Mutex                  m_lock;
    ///
    GestureEvent_t                      m_memoryEvents[OPTION_GESTURES_EVENT_BUFFER_SIZE];


public:
    ///
    SubSystem()
        : m_tiltMotions( OPTION_GESTURES_TILT_THRESHOLD, OPTION_GESTURES_TILT_VERTICAL_THRESHOLD, OPTION_GESTURES_SURFACE_SWITCH_DURATION_MSEC )
        , m_spinner( OPTION_GESTURES_SPINNER_FILTER_CONSTANT, OPTION_GESTURES_SPINNER_SLOW_WEIGHT, OPTION_GESTURES_SPINNER_NORMAL_WEIGHT, OPTION_GESTURES_SPINNER_FAST_WEIGHT )
        , m_events( OPTION_GESTURES_EVENT_BUFFER_SIZE, m_memoryEvents )
    {
    }

public:
    ///
    void start( void )
    {
        /* Initialize the IMU sensor */
        if ( !m_imu.start( Driver::Imu::Bno055::Adafruit::OPERATION_MODE_IMUPLUS ) )
        {
            /* There was a problem detecting the BNO055 ... check your connections */
            Bsp_Serial().write( "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\r\n" );
            while ( 1 );
        }

        // Finish setup of the IMU sensor
        m_imu.setExtCrystalUse( true );
        m_imu.setSensorOffsets( imuCalibrationConstants );
    }


public:
    ///
    void appRun()
    {
        for ( ;;)
        {
            unsigned long timestamp = Cpl::System::ElaspedTime::milliseconds();
            Bsp_Api_toggle_debug1();

            // Sample the IMU
            Driver::Imu::Bno055::Adafruit::raw_vector_t vgyro  = m_imu.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_GYROSCOPE );
            Driver::Imu::Bno055::Adafruit::raw_vector_t vgrav  = m_imu.getRawVector( Driver::Imu::Bno055::Adafruit::VECTOR_GRAVITY );

            // Process the IMU data
            GestureEvent_t myEvent;
            myEvent.m_spinnerCount      = m_spinner.process( vgyro );
            myEvent.m_tiltEventOccurred = m_tiltMotions.process( vgrav, myEvent.m_tiltEvent );

            // Buffer the IMU result (with thread safety)
            m_lock.lock();
            m_events.add( myEvent );
            m_lock.unlock();

            // Enforce monotonic sampling 
            static unsigned long duration = 0;
            duration = Cpl::System::ElaspedTime::deltaMilliseconds( timestamp );
            if ( duration < OPTION_GESTURES_SAMPLING_DELAY_MSEC )
            {
                Cpl::System::Api::sleep( OPTION_GESTURES_SAMPLING_DELAY_MSEC - duration );
            }
        }
    }

public:
    ///
    bool getNextGestureEvent( GestureEvent_t& nextevent )
    {
        bool result;

        // Get the next event (with thread safety)
        m_lock.lock();
        result = m_events.remove( nextevent );
        m_lock.unlock();

        return result;
    }

};

};  // end namespace

///////////////////////////////////////////////////////////////////////////////

static SubSystem mySubSystem;

void setupGestures( void )
{
    // Initailize the IMU
    mySubSystem.start();

    // Create the thread to run the gesture sub-system in
    Cpl::System::Thread::create( mySubSystem, "Gesture", OPTION_GESTURES_THREAD_PRIORITY, OPTION_GESTURES_STACK_SIZE );
}

bool getNextGestureEvent( GestureEvent_t& nextevent )
{
    return mySubSystem.getNextGestureEvent( nextevent );
}

