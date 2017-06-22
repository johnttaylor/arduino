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


#include "Cpl/System/ElapsedTime.h"
#include "Cpl/System/Trace.h"
#include "Imu/Motion/Cube/Tilt.h"
#include "Actions.h"
#include "Main.h"
#include "FrameSimple.h"

#define SECT_     "actions"
#define SECT_DO   "actions-do"


/// Namespaces
using namespace Golem;



///////////////////////////////
Actions::Actions( Adafruit_NeoPixel& ledDriver )
    : m_ledDriver( ledDriver )
    , m_tiltCount( 0 )
    , m_tiltAction( eINVALID )
    , m_currentTilt( Imu::Motion::Cube::Tilt::AspectState_T::eUNKNOWN )
    , m_timeoutTimerActive( false )
    , m_multiTimerActive( false )
    , m_ackTimerActive( false )
    , m_spinnerTimerActive( false )
{
    // Initialize my FSM
    initialize();
}


bool Actions::isActionDetectionInProgress( void )
{
    return isInControllingLEDOutputs();
}

void Actions::process( void )
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    bool newEvent = getNextGestureEvent( m_currentGestureEvent );
    if ( !newEvent )
    {
        // 'Nullify' my current gesture event when there is NO gesture event
        m_currentGestureEvent.m_spinnerCount             = 0;
        m_currentGestureEvent.m_tiltEventOccurred        = false;
        m_currentGestureEvent.m_tiltEvent.m_currentState = Imu::Motion::Cube::Tilt::AspectState_T::eUNKNOWN;
        m_currentGestureEvent.m_tiltEvent.m_currentTop   = Imu::Motion::Cube::Tilt::Surface_T::eTOP;
    }

    CPL_SYSTEM_TRACE_MSG( SECT_, ("process(). newEvent=%d, curState=%s, aspect=%d, angle=%d, currentTop=%d, tiltAction=%d, spinner=%ld",
                          newEvent, getNameByState( getInnermostActiveState() ), m_currentGestureEvent.m_tiltEvent.m_currentState, (int) m_currentGestureEvent.m_tiltEvent.m_tiltAngle, m_currentGestureEvent.m_tiltEvent.m_currentTop, m_tiltAction, m_currentGestureEvent.m_spinnerCount) );

    m_now = Cpl::System::ElapsedTime::milliseconds();
    processEvent( ACTIONSFSM_NO_MSG );
}



/////////////////////////////////////
void Actions::doSingleAction( void )
{
    Main*                   golemP        = Main::getApplicationPointer();
    Frame::FrameConfig_T    currentConfig = golemP->getFrameConfig();
    if ( m_currentGestureEvent.m_spinnerCount != 0 )
    {
        uint32_t      newBitTime = currentConfig.m_bitTime + m_currentGestureEvent.m_spinnerCount; 
        Golem::Frame* newPolicyP = new Golem::FrameSimple( newBitTime, currentConfig.m_numDataBits, currentConfig.m_stopBits, currentConfig.m_parity );
        golemP->setPolicies( newPolicyP, 0, 0, 0, 0 );
        CPL_SYSTEM_TRACE_MSG( SECT_DO, ("Action::Single.  New bit time=%ld, old bit time=%ld", newBitTime, currentConfig.m_bitTime) );
    }
}

void Actions::doDoubleAction( void )
{
}

void Actions::doTripleAction( void )
{
}

void Actions::doRockerAction( void )
{
}



/////////////////////////////////////
bool Actions::isAckTimerExpired()
{
    bool expired = m_ackTimerActive && Cpl::System::ElapsedTime::expiredMilliseconds( m_ackTimerMarker, OPTION_GOLEM_FEEDBACK_ACK_TILTS_TIMEOUT_MSEC, m_now );
    if ( expired )
    {
        m_ackTimerActive = false;
    }
    return expired;
}

bool Actions::isMultiTimerExpired()
{
    bool expired = m_multiTimerActive && Cpl::System::ElapsedTime::expiredMilliseconds( m_multiTimerMarker, OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC, m_now );
    if ( expired )
    {
        m_multiTimerActive = false;
    }
    return expired;
}

bool Actions::isSpinnerTimerExpired()
{
    bool expired =  m_spinnerTimerActive && Cpl::System::ElapsedTime::expiredMilliseconds( m_spinnerTimerMarker, OPTION_GOLEM_FEEDBACK_SPINNER_TIMEOUT_MSEC, m_now );
    if ( expired )
    {
        m_spinnerTimerActive = false;
    }
    return expired;
}

bool Actions::isTimeoutExpired()
{
    bool expired = m_timeoutTimerActive && Cpl::System::ElapsedTime::expiredMilliseconds( m_timeoutTimerMarker, OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC, m_now );
    if ( expired )
    {
        m_timeoutTimerActive = false;
    }
    return expired;
}

bool Actions::isSpinnerChange()
{
    // Don't consume spinner changes if I am tilted
    return !m_currentGestureEvent.m_tiltEventOccurred && m_currentGestureEvent.m_spinnerCount != 0;
}

bool Actions::isHomed()
{
    bool homed = false;
    if ( m_currentGestureEvent.m_tiltEvent.m_currentTop == Imu::Motion::Cube::Tilt::eTOP )
    {
        homed = m_currentGestureEvent.m_tiltEvent.m_currentState == Imu::Motion::Cube::Tilt::eHOMED;
    }
    return homed;
}

bool Actions::isTilt()
{
    bool tilted = false;
    if ( m_currentGestureEvent.m_tiltEvent.m_currentTop == Imu::Motion::Cube::Tilt::eTOP )
    {
        switch ( m_currentGestureEvent.m_tiltEvent.m_currentState )
        {
        case Imu::Motion::Cube::Tilt::eNORTH:
        case Imu::Motion::Cube::Tilt::eSOUTH:
        case Imu::Motion::Cube::Tilt::eWEST:
        case Imu::Motion::Cube::Tilt::eEAST:
            tilted = true;
            break;
        default:
            break;
        }
    }

    return tilted;
}

bool Actions::isValidAction()
{
    if ( m_tiltAction == eROCKER )
    {
        return true;
    }

    // If I get here, then there is NO valid action!
    return false;
}

bool Actions::isValidMultiAction() throw()
{
    if ( m_tiltCount == 1 )
    {
        m_tiltAction = eSINGLE;
        return true;
    }
    else if ( m_tiltCount == 2 )
    {
        m_tiltAction = eDOUBLE;
        return true;
    }
    else if ( m_tiltCount == 3 )
    {
        m_tiltAction = eTRIPLE;
        return true;
    }

    // If I get here, then there is NO valid action!
    return false;
}

/////////////////////////////////////
void Actions::beginAction() throw()
{
    updateAction();
}

void Actions::beginTilt() throw()
{
    m_tiltCount     = 1;
    m_tiltAction    = eINVALID;
    m_currentTilt   = m_currentGestureEvent.m_tiltEvent.m_currentState;
}

void Actions::setAckExitVisualCue() throw()
{
    setAllLEDs( OPTION_GOLEM_FEEDBACK_EXIT_WRGB_COLOR );
    m_ledDriver.show();
}

void Actions::setAckTiltVisualCue() throw()
{
    setAllLEDs( OPTION_GOLEM_FEEDBACK_ACK_TILT_WRGB_COLOR );
    m_ledDriver.show();
}

void Actions::setHomedVisualCue() throw()
{
    setAllLEDs( OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR );
    m_ledDriver.show();
}

void Actions::setTiltVisualCue() throw()
{
    setAllLEDs( OPTION_GOLEM_FEEDBACK_ON_WRGB_COLOR );
    m_ledDriver.show();
}

void Actions::startAckTimer() throw()
{
    m_ackTimerMarker = m_now;
    m_ackTimerActive = true;
}

void Actions::startSpinnerTimer() throw()
{
    m_spinnerTimerMarker = m_now;
    m_spinnerTimerActive = true;
}

void Actions::startMultiTimer() throw()
{
    m_multiTimerMarker = m_now;
    m_multiTimerActive = true;
}

void Actions::startTimeoutTimer() throw()
{
    m_timeoutTimerMarker = m_now;
    m_timeoutTimerActive = true;
}

void Actions::stopMultiTimer() throw()
{
    m_multiTimerActive = false;
}

void Actions::stopSpinnerTimer() throw()
{
    m_spinnerTimerActive = false;
}

void Actions::stopTimeoutTimer() throw()
{
    m_timeoutTimerActive = false;
}

void Actions::updateAction() throw()
{
    switch ( m_tiltAction )
    {
    case eSINGLE:
        doSingleAction();
        break;
    case eDOUBLE:
        doDoubleAction();
        break;
    case eTRIPLE:
        doTripleAction();
        break;
    case eROCKER:
        doRockerAction();
        break;
    default:
        break;
    }
}


void Actions::updateHomed() throw()
{
    // No actions currently needed (6/18/2017)
}

void Actions::updateTilt() throw()
{
    // React to the actual tilt
    switch ( m_currentGestureEvent.m_tiltEvent.m_currentState )
    {
    case Imu::Motion::Cube::Tilt::eTILT_NORTH:
        if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_NORTH )
        {
            m_tiltCount++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_SOUTH )
        {
            m_tiltAction = eROCKER;
        }
        m_currentTilt = m_currentGestureEvent.m_tiltEvent.m_currentState;
        break;

    case Imu::Motion::Cube::Tilt::eTILT_SOUTH:
        if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_SOUTH )
        {
            m_tiltCount++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_NORTH )
        {
            m_tiltAction = eROCKER;
        }
        m_currentTilt = m_currentGestureEvent.m_tiltEvent.m_currentState;
        break;

    case Imu::Motion::Cube::Tilt::eTILT_WEST:
        if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_WEST )
        {
            m_tiltCount++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_EAST )
        {
            m_tiltAction = eROCKER;
        }
        m_currentTilt = m_currentGestureEvent.m_tiltEvent.m_currentState;
        break;

    case Imu::Motion::Cube::Tilt::eTILT_EAST:
        if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_EAST )
        {
            m_tiltCount++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_WEST )
        {
            m_tiltAction = eROCKER;
        }
        m_currentTilt = m_currentGestureEvent.m_tiltEvent.m_currentState;
        break;

    default:
        break;
    }
}


///////////////////////////////
void Actions::setAllLEDs( uint32_t wrgb )
{
    for ( uint16_t i=0; i < m_ledDriver.numPixels(); i++ )
    {
        m_ledDriver.setPixelColor( i, wrgb );
    }
}
