#ifndef Golem_Actions_h_
#define Golem_Actions_h_
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
/** @file */

#include "Golem/ActionsFsm_.h"
#include "Cpl/System/Mutex.h"
#include <stdint.h>
#include "gestures.h"
#include "Adafruit_NeoPixel.h"


/// Timeout period, in msec, for the feedback interval
#ifndef OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC              3000
#endif

/// Timeout period, in msec, for 'multiple tilts'
#ifndef OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC  (OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC/2)
#endif

/// Timeout period, in msec, for acknowledging a tilt action
#ifndef OPTION_GOLEM_FEEDBACK_ACK_TILTS_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_ACK_TILTS_TIMEOUT_MSEC    (OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC/2)
#endif

/// Timeout period, in msec, for updating polices using the 'spinner'
#ifndef OPTION_GOLEM_FEEDBACK_SPINNER_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_SPINNER_TIMEOUT_MSEC      (1000*60)   // 1 minute
#endif

/// Feedback ON Color 
#ifndef OPTION_GOLEM_FEEDBACK_ON_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_ON_WRGB_COLOR             0xFF000000  // white
#endif

/// Feedback 'Spinner' Color 
#ifndef OPTION_GOLEM_FEEDBACK_ACK_TILT_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_ACK_TILT_WRGB_COLOR       0x0000FF00  // Green
#endif

/// Feedback OFF Color 
#ifndef OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR            0x00000000  // off
#endif

/// Feedback Exiting Color 
#ifndef OPTION_GOLEM_FEEDBACK_EXIT_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_EXIT_WRGB_COLOR           0x00FF0000  // red
#endif

/// Namespaces
namespace Golem {


/** Converts tilt/spin gestures into actions/commands.  This class is not
    thread safe, i.e. it needs to run/execute in the same thread as the
    Golem::Main class.

    Tilt (any axis)				bit time, spin +/-
    Tilt x2 (any axis)			output -->cycle through all|2|....
    Tilt x3 (any axis)			ramp -->cycle through 0 to 50 to 0
    Tilt + opposite Tilt			colors -->cycle through on, off, all, all+fast
    Tilt + opposite | 90 tilt + opposite
    Tilt each axis clockwise/cc		on|off
    Tilt each axis clockwise/cc x2		BLE on|off

    Enable setting .... timeout or Tilt cancels


 */
class Actions : public ActionsFsm
{
protected:
    /// Tilt sequences
    typedef enum
    {
        eSINGLE=0,                          //!< Single tilt on any axis
        eDOUBLE,                            //!< Double tilt on the same axis
        eTRIPLE,                            //!< Triple tilt on the same axis
        eROCKER,                            //!< Two tilts on opposite axises
        eCORNER_CLOCKWISE,                  //!< Two tilts on two axis 90' apart - with a 'clockwise turn' (with respect to the top) 
        eCORNER_COUNTER_CLOCKWISE,          //!< Two tilts on two axis 90' apart - with a 'counter clockwise turn' (with respect to the top) 
        eCIRCLE_CLOCKWISE,                  //!< Four tilts on all axises in clockwise turn sequence
        eDOUBLE_CIRCLE_CLOCKWISE,           //!< Eight tilts on all axises in clockwise turn sequence
        eCIRCLE_COUNTER_CLOCKWISE,          //!< Four tilts on all axises in counter clockwise turn sequence
        eDOUBLE_CIRCLE_COUNTER_CLOCKWISE,   //!< Four tilts on all axises in counter clockwise turn sequence
        eINVALID                            //!< An unsupported/unrecognized tilt sequence
    } Tilt_T;

    /// States
    typedef enum
    {
        eIDLE=0,                            //!< No actions in progress
        eTILTED,                            //!< Currently Tilted
        eACKNOWLEDGE_TILT,                  //!< Pause to visually acknowledge that tilt action was recognized/accepted
        eSPINNER_IN_PROGRESS,               //!< Accepted a Tilt Action, now processing Spinner events
        eTIMEOUT                            //!< No valid Tilt action was found before timing out
    } State_T;


protected:
    /// LED Driver
    Adafruit_NeoPixel&      m_ledDriver;

    ///
    GestureEvent_t          m_currentGestureEvent;

    /// Time marker for timing out Feedback mode
    unsigned long           m_timeoutTimerMarker;

    /// Time marker for timing out multi-tilt sequences
    unsigned long           m_multiTimerMarker;

    /// Time marker for timing out ack'ing the detection of tilt action
    unsigned long           m_ackTimerMarker;

    /// Time marker for timing out spinner mode 
    unsigned long           m_spinnerTimerMarker;

    /// Current time of when event/fsm processing
    unsigned long           m_now;

    /// Number of tilt actions in the current multi-tilt sequence
    uint16_t                m_tiltCount;

    /// Action detected
    Tilt_T                  m_tiltAction;

    /// Current Tilt 
    Imu::Motion::Cube::Tilt::AspectState_T m_currentTilt;

    /// Timer state
    bool                    m_timeoutTimerActive;

    /// Timer state
    bool                    m_multiTimerActive;

    /// Timer state
    bool                    m_ackTimerActive;

    /// Timer state
    bool                    m_spinnerTimerActive;




public:
    /// Constructor
    Actions( Adafruit_NeoPixel& ledDriver );

public:
    /** This method provides the 'execution time slice' for the action handler.
        This method should be called every time the Golem::process() method
        is called.
     */
    void process( void );


public:
    /// Returns the true if the feedback mode is enabled. 
    bool isActionDetectionInProgress( void );



protected:
    /// See Golem::ActionFsmContext_
    void beginAction() throw();

    /// See Golem::ActionFsmContext_
    void beginTilt() throw();

    /// See Golem::ActionFsmContext_
    void setAckExitVisualCue() throw();

    /// See Golem::ActionFsmContext_
    void setAckTiltVisualCue() throw();

    /// See Golem::ActionFsmContext_
    void setHomedVisualCue() throw();

    /// See Golem::ActionFsmContext_
    void setTiltVisualCue() throw();

    /// See Golem::ActionFsmContext_
    void startAckTimer() throw();

    /// See Golem::ActionFsmContext_
    void startSpinnerTimer() throw();

    /// See Golem::ActionFsmContext_
    void startMultiTimer() throw();

    /// See Golem::ActionFsmContext_
    void startTimeoutTimer() throw();

    /// See Golem::ActionFsmContext_
    void stopMultiTimer() throw();

    /// See Golem::ActionFsmContext_
    void stopSpinnerTimer() throw();

    /// See Golem::ActionFsmContext_
    void stopTimeoutTimer() throw();

    /// See Golem::ActionFsmContext_
    void updateAction() throw();

    /// See Golem::ActionFsmContext_
    void updateHomed() throw();

    /// See Golem::ActionFsmContext_
    void updateTilt() throw();


protected:
    /// See Golem::ActionFsmContext_
    bool isAckTimerExpired() throw();

    /// See Golem::ActionFsmContext_
    bool isHomed() throw();

    /// See Golem::ActionFsmContext_
    bool isMultiTimerExpired() throw();

    /// See Golem::ActionFsmContext_
    bool isSpinnerChange() throw();

    /// See Golem::ActionFsmContext_
    bool isSpinnerTimerExpired() throw();

    /// See Golem::ActionFsmContext_
    bool isTilt() throw();

    /// See Golem::ActionFsmContext_
    bool isTimeoutExpired() throw();

    /// See Golem::ActionFsmContext_
    bool isValidAction() throw();

    /// See Golem::ActionFsmContext_
    bool isValidMultiAction() throw();

protected:
    /// Helper
    void doSingleAction( void );

    /// Helper
    void doDoubleAction( void );

    /// Helper
    void doTripleAction( void );

    /// Helper
    void doRockerAction( void );

protected:
    /// Helper method that converts individual color intensity into a single RGBW value
    static inline uint32_t convertToRGBW_( uint8_t red, uint8_t green, uint8_t blue, uint8_t white ) { return Adafruit_NeoPixel::Color( red, green, blue, white ); }

    /// Helper
    void setAllLEDs( uint32_t wrgb );
};


};      // end namespace
#endif  // end header latch
