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
#define OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC      2000
#endif

/// Feedback color
#ifndef OPTION_GOLEM_FEEDBACK_INITIAL_COLOR
#define OPTION_GOLEM_FEEDBACK_INITIAL_COLOR     0xFF000000  // white
#endif

/// Namespaces
namespace Golem {


/** Converts tilt/spin gestures into actions/commands
 */
class Actions
{
public:
    /// The possible types of visual feedback cues
    typedef enum
    {
        eSOLID_ON,              //!< Turn on all of the LEDs and stay on till command to FLASH
        eNO_CHANGE              //!< Retain previous feedback option.  If 'eNO_CHANGE' is used as the initial feedback state, internally the feeddback state is set to eSOLID_ON
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
    bool isFeedbackModeEnabled(void);

public:
    /** This method temporarily puts the Golem into feedback mode.  Golem
        will automatically timeout of feedback mode (and revert to policy mode)
        if the application does not call disableFeedbackMode().  This method
        is thread safe.
     */
    void enableFeedbackMode( FeedbackOption_T initialOption, uint32_t initialWrgbColor, unsigned long  timeoutInMsec=OPTION_GOLEM_FEEDBACK_TIMEOUT_MSEC );

    /** This method updates the current feedback output.  f 'timeoutInMsec' is
        zero then no change is made the timeout period (that was specified on
        the enableFeedbackMode() call). This method is thread safe.
     */
    void updateFeedback( uint32_t wrbgColor, FeedbackOption_T option=eNO_CHANGE, unsigned long  timeoutInMsec=0 );

    /** This method cancels feedback mode.  If Golem is not in feedback mode
        this method does nothing. This method is thread safe.
     */
    void disableFeedbackMode( void );

    /// Helper method that converts individual color intensity into a single RGBW value
    static inline uint32_t convertToRGBW( uint8_t red, uint8_t green, uint8_t blue, uint8_t white ) { return Adafruit_NeoPixel::Color( red, green, blue, white ); }

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
