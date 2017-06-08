#ifndef Golem_Main_h_
#define Golem_Main_h_
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

#include "Golem/Frame.h"
#include "Golem/FrameBitColor.h"
#include "Golem/Output.h"
#include "Golem/DataStream.h"
#include "Golem/IntensityRamp.h"
#include "Cpl/System/Mutex.h"


/// Namespaces
namespace Golem {


/** TLDR; Golem is a way complicated (but not random) LED flasher :).

    This concrete class implements the Golem LED Lighting application. Conceptually
    the Golem application outputs a stream of 'words' to an 'LED Uart' where
    each word in the stream has a traditional UART Framing (Start, data, parity,
    stop bits).  For each bit in each Frame the 'LED Uart' outputs a specific
    LED pattern/color on a strip of colored LEDs.
    
    Golem is designed to be part of multi-threaded application where other
    applications can change/request new Policies (that determine how the LEDs
    are controlled).
 */
class Main
{
protected:
    /// Policy Instance in use
    Frame*              m_frameP;

    /// Policy Instance in use
    FrameBitColor*      m_colorP;

    /// Policy Instance in use
    Output*             m_outputP;

    /// Policy Instance in use
    DataStream*         m_streamP;

    /// Policy Instance in use
    IntensityRamp*      m_rampP;

    /// Policy Instance in to use Next
    Frame*              m_newFrameP;

    /// Policy Instance in use
    FrameBitColor*      m_newColorP;

    /// Policy Instance in use Next
    Output*             m_newOutputP;

    /// Policy Instance in use Next
    DataStream*         m_newStreamP;

    /// Policy Instance in use Next
    IntensityRamp*      m_newRampP;

    /// Mutex use to safely request new policies
    Cpl::System::Mutex& m_lock;

    /// Timer marker of last bit
    unsigned long       m_timeMarker;

    /// The current bit being processed
    Frame::Bit_T        m_currentBit;

    /// The current bit's value
    bool                m_currentValue;

    /// The current stop bit processed
    uint8_t             m_stopBit;

    /// The current data bit processed
    uint8_t             m_dataBit;

    /// Used to calculate the parity bit
    uint8_t             m_paritySum;

    /// At least one new Policy is available
    bool                m_dirty;

public:
    /// Constructor
    Main( Cpl::System::Mutex& lock );

public:
    /** This method provides the 'execution time slice' for the Golem
        application.  This method should be called from a thread's main/forever
        loop.  The frequency at which it is called determines the resolution
        for policy feature, e.g. resolution of the intensity ramp rate.
     */
    void process( void );


public:
    /** This method allows the application to update/change one or more
        polices of the Golem application.  The application MUST call this
        function at least to set ALL policies (i.e. Golem instances are
        constructed with NO policies).  Do NOT update/change a policy - specify
        zero as the policy argument.  This method is thread safe, i.e. can
        be called by other thread (than the thread that calls the process()
        method).

        FIXME: This is broken -->who creates new policies and mostly IMPORTANTLY who and when do the 'old' policies get DELETED?!?!?!
     */
    void setPolicies( Frame*            framePolicyP,
                      DataStream*       streamPolicyP,
                      FrameBitColor*    colorPolicyP,
                      IntensityRamp*    rampPolicyP,
                      Output*           outputPolicyP
                      );


};


};      // end namespace
#endif  // end header latch
