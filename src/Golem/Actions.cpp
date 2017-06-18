/*-----------------------------------------------------------------------------
* This file is part of the Arduino Project.  The Arduino Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/arduino/license.txt
*
* Copyright (c) 2017 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/


#include "gestures.h"
#include "Cpl/System/ElapsedTime.h"
#include "Cpl/System/Trace.h"
#include "Imu/Motion/Cube/Tilt.h"
#include "Actions.h"

#define SECT_  "actions"


/// Namespaces
using namespace Golem;



///////////////////////////////
Actions::Actions( Cpl::System::Mutex& lock, Adafruit_NeoPixel& ledDriver )
    : m_ledDriver( ledDriver )
    , m_lock( lock )
    , m_feedbackMode( false )
    , m_feedbackOption( eSOLID_ON )
    , m_feedbackWrgbColor( OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR )
    , m_feedbackTimeout( 0 )
    , m_multiTimeout( OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC )
    , m_tiltCount( 0 )
    , m_currentAction( eINVALID )
    , m_tilted( false )
{
}


///////////////////////////////
void Actions::process( void )
{
    runGestures();
 
    if ( m_feedbackMode )
    {
        runFeedbackMode();
    }

}

bool Actions::isFeedbackModeEnabled( void )
{
    Cpl::System::Mutex::ScopeBlock lock( m_lock );
    return m_feedbackMode;
}

/////////////////////////////////////////////    
void Actions::runFeedbackMode( void )
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    m_lock.lock();
    FeedbackOption_T    option  = m_feedbackOption;
    uint32_t            rgbw    = m_feedbackWrgbColor;
    unsigned long       timeout = m_feedbackTimeout;
    m_lock.unlock();

    // Option: SOLID_ON
    if ( option == eSOLID_ON )
    {
        if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_feedbackTimeMarker, timeout ) )
        {
            disableFeedbackMode_();
        }
        else
        {
            setAllLEDs( rgbw );
            m_ledDriver.show();
        }
    }
}


void Actions::enableFeedbackMode_( FeedbackOption_T initialOption, uint32_t initialWrgbColor, unsigned long  timeoutInMsec )
{
    m_lock.lock();
    m_feedbackWrgbColor = initialWrgbColor;
    m_feedbackOption    = initialOption == eNO_CHANGE ? eSOLID_ON : initialOption;
    m_feedbackMode      = true;
    m_feedbackTimeout   = timeoutInMsec;
    m_lock.unlock();

    m_feedbackTimeMarker = Cpl::System::ElapsedTime::milliseconds();
    setAllLEDs( OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR );
    m_ledDriver.show();
    CPL_SYSTEM_TRACE_MSG( SECT_, ("enableFeedbackMode(). option=%d, wrgb=%04X, time=%ld", initialOption, initialWrgbColor, timeoutInMsec) );

}

void Actions::updateFeedback_( uint32_t newWrgbColor, FeedbackOption_T option, unsigned long  timeoutInMsec )
{
    Cpl::System::Mutex::ScopeBlock lock( m_lock );
    m_feedbackWrgbColor = newWrgbColor;
    if ( option != eNO_CHANGE )
    {
        m_feedbackOption = option;
    }
    if ( timeoutInMsec != 0 )
    {
        m_feedbackTimeout    = timeoutInMsec;
        m_feedbackTimeMarker = Cpl::System::ElapsedTime::milliseconds();
    }
}
void Actions::disableFeedbackMode_( void )
{
    m_lock.lock();
    m_feedbackMode = false;
    m_lock.unlock();

    setAllLEDs( OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR );
    m_ledDriver.show();
    CPL_SYSTEM_TRACE_MSG( SECT_, ("disableFeedbackMode_()") );

}

void Actions::setAllLEDs( uint32_t wrgb )
{
    for ( uint16_t i=0; i < m_ledDriver.numPixels(); i++ )
    {
        m_ledDriver.setPixelColor( i, wrgb );
    }
}

/////////////////////////////////////
void Actions::runGestures( void )
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    GestureEvent_t event;

    // Was there a gesture?
    if ( getNextGestureEvent( event ) )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Gesture Event occurred") );

        // Did a tilt event occur?
        if ( event.m_tiltEventOccurred )
        {
            CPL_SYSTEM_TRACE_MSG( SECT_, ("Tilt Event occurred. aspect=%d, angle=%d, currentTop=%d", event.m_tiltEvent.m_currentState, (int) event.m_tiltEvent.m_tiltAngle, event.m_tiltEvent.m_currentTop) );

            // I am oriented correctly, i.e. the top IS the top
            if ( event.m_tiltEvent.m_currentTop == Imu::Motion::Cube::Tilt::eTOP )
            {
                CPL_SYSTEM_TRACE_MSG( SECT_, ("Taking action on Tilt Event") );

                // React to the actual tilt
                switch ( event.m_tiltEvent.m_currentState )
                {
                case Imu::Motion::Cube::Tilt::eNORTH:
                case Imu::Motion::Cube::Tilt::eSOUTH:
                case Imu::Motion::Cube::Tilt::eWEST:
                case Imu::Motion::Cube::Tilt::eEAST:
                    if ( !m_feedbackMode )
                    {
                        // FIXME: Enter feedback mode, What exits me (timeout)?  Multi-tilt actions. When start Spin actions? Call spinner
                        // FIXME: Enter feedback mode, What exits me (timeout)?  Multi-tilt actions. When start Spin actions? Call spinner
                        // FIXME: Enter feedback mode, What exits me (timeout)?  Multi-tilt actions. When start Spin actions? Call spinner
                        // FIXME: Enter feedback mode, What exits me (timeout)?  Multi-tilt actions. When start Spin actions? Call spinner
                        // FIXME: Enter feedback mode, What exits me (timeout)?  Multi-tilt actions. When start Spin actions? Call spinner
                        m_feedbackMode = true;
                        enableFeedbackMode_( event.m_tiltEvent.m_currentState );
                        break;
                    }
                    else
                    {
                        updateFeedback_( event.m_tiltEvent.m_currentState );
                    }
                    break;

                case Imu::Motion::Cube::Tilt::eHOMED:
                    if ( m_tilted )
                    {
                        m_tilted = false;
                        updateFeedback_( 0x0, FeedbackOption_T::eNO_CHANGE, OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC );
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
}

/////////////////////////////////////

void Actions::startAction_( Tilt_T action )
{
}

void Actions::updateAction( uint32_t spinnerAdjustment )
{
}

void Actions::stopAction( void )
{
}
