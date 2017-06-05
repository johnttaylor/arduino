#ifndef Gestures_h_
#define Gestures_h_
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

#include "Imu/Motion/Cube/Tilt.h"

/// Configuration option...
#ifndef OPTION_GESTURES_TILT_THRESHOLD
#define OPTION_GESTURES_TILT_THRESHOLD                  200
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_TILT_VERTICAL_THRESHOLD
#define OPTION_GESTURES_TILT_VERTICAL_THRESHOLD         10
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_SURFACE_SWITCH_DURATION_MSEC
#define OPTION_GESTURES_SURFACE_SWITCH_DURATION_MSEC    2000
#endif


/// Configuration option...
#ifndef OPTION_GESTURES_SPINNER_FILTER_CONSTANT
#define OPTION_GESTURES_SPINNER_FILTER_CONSTANT         (0.17F)
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_SPINNER_SLOW_WEIGHT
#define OPTION_GESTURES_SPINNER_SLOW_WEIGHT             1
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_SPINNER_NORMAL_WEIGHT
#define OPTION_GESTURES_SPINNER_NORMAL_WEIGHT           5
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_SPINNER_FAST_WEIGHT
#define OPTION_GESTURES_SPINNER_FAST_WEIGHT             50
#endif


/// Configuration option...
#ifndef OPTION_GESTURES_SAMPLING_DELAY_MSEC
#define OPTION_GESTURES_SAMPLING_DELAY_MSEC             50
#endif


/// Configuration option...
#ifndef OPTION_GESTURES_EVENT_BUFFER_SIZE
#define OPTION_GESTURES_EVENT_BUFFER_SIZE               3
#endif


/// Configuration option...
#ifndef OPTION_GESTURES_STACK_SIZE
#define OPTION_GESTURES_STACK_SIZE                      (1024*2)
#endif
/// Configuration option...
#ifndef OPTION_GESTURES_THREAD_PRIORITY
#define OPTION_GESTURES_THREAD_PRIORITY                 CPL_SYSTEM_THREAD_PRIORITY_NORMAL
#endif

/////////////////////////////////////////////////

/// Gesture Events
typedef struct
{
    uint32_t                            m_spinnerCount;     //!< A non-zero value indicates spinner activity
    Imu::Motion::Cube::Tilt::Event_T    m_tiltEvent;        //!< Contains information about a tilt event.  
    bool                                m_tiltEventOccurred;//!< Is set to true if a tilt event occurred.
} GestureEvent_t;


/////////////////////////////////////////////////

/// Initializes the Gesture sub-system.  Should be called before the scheduler is started
void setupGestures( void );

/** Retrieves that oldest buffered Gesture Event.  If there are no remaining
    Gesture Events, the method returns false
 */
bool getNextGestureEvent( GestureEvent_t& nextevent );


#endif  // end Header latch