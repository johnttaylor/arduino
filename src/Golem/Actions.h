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

#include "Cpl/System/Mutex.h"
#include <stdint.h>
#include "Adafruit_NeoPixel.h"


/// Timeout period, in msec, for the feedback interval
#ifndef OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC              2000
#endif

/// Timeout period, in msec, for 'multiple tilts'
#ifndef OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC
#define OPTION_GOLEM_FEEDBACK_MULTI_TILTS_TIMEOUT_MSEC  (OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC/2)
#endif

/// Feedback ON Ccolor 
#ifndef OPTION_GOLEM_FEEDBACK_INITIAL_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_INITIAL_WRGB_COLOR        0xFF000000  // white
#endif

/// Feedback OFF Color 
#ifndef OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR
#define OPTION_GOLEM_FEEDBACK_OFF_WRGB_COLOR            0x00000000  // off
#endif

/// Namespaces
namespace Golem {


/** Converts tilt/spin gestures into actions/commands

    Tilt (any axis)				bit time, spin +/-
    Tilt x2 (any axis)			output -->cycle through all|2|....
    Tilt x3 (any axis)			ramp -->cycle through 0 to 50 to 0
    Tilt + opposite Tilt			colors -->cycle through on, off, all, all+fast
    Tilt + opposite | 90 tilt + opposite
    Tilt each axis clockwise/cc		on|off
    Tilt each axis clockwise/cc x2		BLE on|off

    Enable setting .... timeout or Tilt cancels


 */
class Actions
{
public:
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

public:
    /// The possible types of visual feedback cues
    typedef enum
    {
        eSOLID_ON,              //!< Turn on all of the LEDs and stay on till command to FLASH
        eNO_CHANGE              //!< Retain previous feedback option.  If 'eNO_CHANGE' is used as the initial feedback state, internally the feedback state is set to eSOLID_ON
    } FeedbackOption_T;

protected:
    /// LED Driver
    Adafruit_NeoPixel&      m_ledDriver;

    /// Mutex for thread safety
    Cpl::System::Mutex&     m_lock;

    /// Flag that keeps track of executing policies vs. feedback mode
    bool                    m_feedbackMode;

    /// Current feedback option
    FeedbackOption_T        m_feedbackOption;

    /// Current feedback color (rgbw format)
    uint32_t                m_feedbackWrgbColor;

    /// Time marker for timing out Feedback mode
    unsigned long           m_feedbackTimeMarker;

    /// Timeout period, in milliseconds, for feedback mode
    unsigned long           m_feedbackTimeout;

    /// Time marker for timing out multi-tilt sequences
    unsigned long           m_multiTimeMarker;

    /// Timeout period, in milliseconds, for multi-tilt sequences
    unsigned long           m_multiTimeout;

    /// Number of tilt actions in the current multi-tilt sequence
    uint16_t                m_tiltCount;
    
    /// Current Tilt action
    Tilt_T                  m_currentAction;

    /// Tracks i-am-tilted
    bool                    m_tilted;



public:
    /// Constructor
    Actions( Cpl::System::Mutex& lock, Adafruit_NeoPixel& ledDriver );

public:
    /** This method provides the 'execution time slice' for the action handler.
        This method should be called every time the Golem::process() method
        is called.
     */
    void process( void );


public:
    /** Returns the true if the feedback mode is enabled. This method is
        thread safe.
     */
    bool isFeedbackModeEnabled( void );


public:
    /** This method temporarily puts the Golem into feedback mode.  Golem
        will automatically timeout of feedback mode (and revert to policy mode)
        if the application does not call disableFeedbackMode_().  This method
        is thread safe.
     */
    void enableFeedbackMode_( FeedbackOption_T initialOption, uint32_t initialWrgbColor, unsigned long  timeoutInMsec=OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC );

    /** This method updates the current feedback output.  f 'timeoutInMsec' is
        zero then no change is made the timeout period (that was specified on
        the enableFeedbackMode() call). This method is thread safe.
     */
    void updateFeedback_( uint32_t wrbgColor, FeedbackOption_T option=eNO_CHANGE, unsigned long  timeoutInMsec=0 );

    /** This method cancels feedback mode.  If Golem is not in feedback mode
        this method does nothing. This method is thread safe.
     */
    void disableFeedbackMode_( void );

    /// Helper method that converts individual color intensity into a single RGBW value
    static inline uint32_t convertToRGBW_( uint8_t red, uint8_t green, uint8_t blue, uint8_t white ) { return Adafruit_NeoPixel::Color( red, green, blue, white ); }

public:
    /// This method carries on the detected action
    void startAction_( Tilt_T action );

    /// This method updates the action base on current rotation
    void updateAction( uint32_t spinnerAdjustment );

    /// This method terminates the detected action
    void stopAction( void );

protected:
    /// Helper 
    void runFeedbackMode( void );

    /// Helper 
    void runGestures( void );

    /// Helper
    void setAllLEDs( uint32_t wrgb );

};


};      // end namespace
#endif  // end header latch
