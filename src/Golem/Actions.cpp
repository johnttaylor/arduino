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

#ifndef OPTION_GOLEM_NEXT_SPIN_STATE_THRESHOLD
#define OPTION_GOLEM_NEXT_SPIN_STATE_THRESHOLD          OPTION_GESTURES_SPINNER_FAST_WEIGHT
#endif

/// Namespaces
using namespace Golem;



///////////////////////////////
Actions::Actions( Adafruit_NeoPixel& ledDriver )
    : m_ledDriver( ledDriver )
    , m_tiltCount( 0 )
    , m_outputState( OutputNeoPixel::eQUARTER_SPIN_C )  // Needs to match the start up state defined by the sketch.
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
    Main* golemP   = Main::getApplicationPointer();
    bool  newState = false;
    bool  advance  = false;

    // Accumulate 'enough' spin to advance to the next state
    m_spinAccumulator += m_currentGestureEvent.m_spinnerCount;
    if ( m_spinAccumulator > OPTION_GOLEM_NEXT_SPIN_STATE_THRESHOLD )
    {
        advance  = true;
        newState = true;
    }
    else if ( m_spinAccumulator < -OPTION_GOLEM_NEXT_SPIN_STATE_THRESHOLD )
    {
        advance  = false;
        newState = true;
    }

    // Take action....
    if ( newState )
    {
        // Determine the next output state
        Golem::OutputNeoPixel::Options_T previous = m_outputState;
        switch ( m_outputState )
        {
        case Golem::OutputNeoPixel::eALL:
            m_outputState = advance ? Golem::OutputNeoPixel::ePAIRS : Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_CC;
            break;
        case Golem::OutputNeoPixel::ePAIRS:
            m_outputState = advance ? Golem::OutputNeoPixel::ePAIRS_SPIN_C : Golem::OutputNeoPixel::eALL;
            break;
        case Golem::OutputNeoPixel::ePAIRS_SPIN_C:
            m_outputState = advance ? Golem::OutputNeoPixel::ePAIRS_SPIN_CC : Golem::OutputNeoPixel::ePAIRS;
            break;
        case Golem::OutputNeoPixel::ePAIRS_SPIN_CC:
            m_outputState = advance ? Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_C : Golem::OutputNeoPixel::ePAIRS_SPIN_C;
            break;
        case Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_C:
            m_outputState = advance ? Golem::OutputNeoPixel::ePAIRS_SPIN_CC : Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_CC;
            break;
        case Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_CC:
            m_outputState = advance ? Golem::OutputNeoPixel::eQUARTER : Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_C;
            break;
        case Golem::OutputNeoPixel::eQUARTER:
            m_outputState = advance ? Golem::OutputNeoPixel::eQUARTER_SPIN_C : Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_CC;
            break;
        case Golem::OutputNeoPixel::eQUARTER_SPIN_C:
            m_outputState = advance ? Golem::OutputNeoPixel::eQUARTER_SPIN_CC : Golem::OutputNeoPixel::eQUARTER;
            break;
        case Golem::OutputNeoPixel::eQUARTER_SPIN_CC:
            m_outputState = advance ? Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_C : Golem::OutputNeoPixel::eQUARTER_SPIN_C;
            break;
        case Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_C:
            m_outputState = advance ? Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_CC : Golem::OutputNeoPixel::eQUARTER_SPIN_CC;
            break;
        case Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_CC:
            m_outputState = advance ? Golem::OutputNeoPixel::eALL : Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_C;
            break;
        }

        // Create the new output policy
        Golem::Output* newPolicyP = new Golem::OutputNeoPixel( m_outputState, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        golemP->setPolicies( 0, 0, 0, 0, newPolicyP );
        m_spinAccumulator = 0;
        CPL_SYSTEM_TRACE_MSG( SECT_DO, ("Action::Double.  new output policy=%s, previous=%s", OutputNeoPixel::toString( m_outputState ), OutputNeoPixel::toString( previous )) );
    }
}

void Actions::doTripleAction( void )
{
}

void Actions::doRockerAction( void )
{
}

void Actions::doCornerClockwiseAction( void )
{
}

void Actions::doCornerCounterClockwiseAction( void )
{
}

void Actions::doCircleClockwiseAction( void )
{
}

void Actions::doDoubleCircleClockwiseAction( void )
{
}

void Actions::doCircleCounterClockwiseAction( void )
{
}

void Actions::doDoubleCircleCounterClockwiseAction( void )
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
    if ( m_clockWise == 1 )
    {
        m_tiltAction = eCORNER_CLOCKWISE;
        return true;
    }
    if ( m_counterClockwise == 1 )
    {
        m_tiltAction = eCORNER_COUNTER_CLOCKWISE;
        return true;
    }
    if ( m_clockWise == 4 )
    {
        m_tiltAction = eCIRCLE_CLOCKWISE;
        return true;
    }
    if ( m_counterClockwise == 4 )
    {
        m_tiltAction = eCIRCLE_COUNTER_CLOCKWISE;
        return true;
    }
    if ( m_clockWise == 8 )
    {
        m_tiltAction = eDOUBLE_CIRCLE_CLOCKWISE;
        return true;
    }
    if ( m_counterClockwise == 8 )
    {
        m_tiltAction = eDOUBLE_CIRCLE_COUNTER_CLOCKWISE;
        return true;
    }
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
    m_spinAccumulator = 0;
}

void Actions::beginTilt() throw()
{
    m_tiltCount        = 1;
    m_clockWise        = 0;
    m_counterClockwise = 0;
    m_tiltAction       = eINVALID;
    m_currentTilt      = m_currentGestureEvent.m_tiltEvent.m_currentState;
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
    case eCORNER_CLOCKWISE:
        doCornerClockwiseAction();
        break;

    case eCORNER_COUNTER_CLOCKWISE:
        doCornerCounterClockwiseAction();
        break;

    case eCIRCLE_CLOCKWISE:
        doCircleClockwiseAction();
        break;

    case eDOUBLE_CIRCLE_CLOCKWISE:
        doDoubleCircleClockwiseAction();
        break;

    case eCIRCLE_COUNTER_CLOCKWISE:
        doCircleCounterClockwiseAction();
        break;

    case eDOUBLE_CIRCLE_COUNTER_CLOCKWISE:
        doDoubleCircleCounterClockwiseAction();
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
            m_counterClockwise = 0;
            m_clockWise        = 0;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_SOUTH )
        {
            m_tiltAction = eROCKER;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_WEST )
        {
            m_counterClockwise = 0;
            m_tiltCount        = 0;
            m_clockWise++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_EAST )
        {
            m_counterClockwise++;
            m_clockWise = 0;
            m_tiltCount = 0;
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
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_EAST )
        {
            m_counterClockwise = 0;
            m_tiltCount        = 0;
            m_clockWise++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_WEST )
        {
            m_counterClockwise++;
            m_clockWise = 0;
            m_tiltCount = 0;
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
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_SOUTH )
        {
            m_counterClockwise = 0;
            m_tiltCount        = 0;
            m_clockWise++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_NORTH )
        {
            m_counterClockwise++;
            m_clockWise = 0;
            m_tiltCount = 0;
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
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_NORTH )
        {
            m_counterClockwise = 0;
            m_tiltCount        = 0;
            m_clockWise++;
        }
        else if ( m_currentTilt == Imu::Motion::Cube::Tilt::eTILT_SOUTH )
        {
            m_counterClockwise++;
            m_clockWise = 0;
            m_tiltCount = 0;
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
