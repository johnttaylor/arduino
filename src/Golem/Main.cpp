/*-----------------------------------------------------------------------------
* This file is part of the Arduino Project.  The Arduino Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/arduino/license.txt
*
* Copyright (c) 2017 John T. Taylor Elasped
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/


#include "Main.h"
#include "Cpl/System/ElapsedTime.h"
#include "Cpl/System/Trace.h"

#define SECT_           "main"
#define SECTVERBOSE_    "main-v"


/// Namespaces
using namespace Golem;



///////////////////////////////
Main::Main( Cpl::System::Mutex& lock )
    : m_frameP( 0 )
    , m_colorP( 0 )
    , m_outputP( 0 )
    , m_streamP( 0 )
    , m_rampP( 0 )
    , m_newFrameP( 0 )
    , m_newColorP( 0 )
    , m_newOutputP( 0 )
    , m_newStreamP( 0 )
    , m_newRampP( 0 )
    , m_lock( lock )
    , m_timeMarker( 0 )
    , m_currentBit( Frame::eSTOP_BIT )
    , m_currentValue( false )
    , m_stopBit( Frame::MAXIMUM_STOP_BITS )
    , m_dataBit( Frame::MAXIMUM_DATA_BITS )
    , m_paritySum( 0 )
    , m_dirty( false )
{
}


///////////////////////////////
void Main::process( void )
{
    CPL_SYSTEM_TRACE_FUNC( SECTVERBOSE_ );
    CPL_SYSTEM_TRACE_MSG( SECTVERBOSE_, ("process().  current policies: frameP=%p, streamP=%, colorP=%p, rampP=%p, outputP=%p",
                          m_frameP, m_streamP, m_colorP, m_rampP, m_outputP) );

    // I must have ALL policies to do anything...
    if ( m_frameP && m_colorP && m_outputP && m_streamP && m_rampP )
    {
        CPL_SYSTEM_TRACE_MSG( SECTVERBOSE_, ("process(). dataAvailable=%d", m_streamP->bitsAreAvailable()) );

        // Do nothing if I am out of data
        if ( m_streamP->bitsAreAvailable() == false )
        {
            m_outputP->stop();  // Ensure all LEDs get turned off!
        }

        // Got Data! Let's flash it
        else
        {
            // Check for a start of a new bit
            unsigned long now     = Cpl::System::ElapsedTime::milliseconds();
            Frame::Bit_T  nextBit = m_currentBit;
            bool          newBit  = false;
            if ( Cpl::System::ElapsedTime::expiredMilliseconds( m_timeMarker, m_frameP->getBitTime(), now ) )
            {
                CPL_SYSTEM_TRACE_MSG( SECT_, ("Start of new bit. prevBit=%d, timeMarker=%ld, now=%ld, delta=%ld, bitTime=%ld", m_currentBit, m_timeMarker, now, Cpl::System::ElapsedTime::deltaMilliseconds( m_timeMarker, now ), m_frameP->getBitTime()) );
                m_timeMarker = now;
                newBit       = true;

                switch ( m_currentBit )
                {
                    // Note: The Start bit is actually 'sent' after the last stop bit -->so I am actually sending the first data bit here
                case Frame::eSTART_BIT:
                    m_dataBit      = 1;
                    m_currentBit   = nextBit = Frame::eDATA_BIT;
                    m_currentValue = m_streamP->getNextBit();
                    m_paritySum    = m_currentValue ? 1 : 0;
                    break;

                case Frame::eDATA_BIT:
                    m_currentValue = m_streamP->getNextBit();
                    m_paritySum   += m_currentValue ? 1 : 0;

                    if ( ++m_dataBit >= m_frameP->getNumberOfDataBits() )
                    {
                        nextBit = Frame::ePARITY_BIT;
                    }
                    break;

                case Frame::ePARITY_BIT:
                    m_stopBit = 0;
                    nextBit   = Frame::eSTOP_BIT;

                    if ( m_frameP->getParity() == Frame::eEVEN )
                    {
                        m_currentValue = m_paritySum & 0x1 ? true : false;
                        break;
                    }
                    else if ( m_frameP->getParity() == Frame::eODD )
                    {
                        m_currentValue = m_paritySum & 0x1 ? false : true;
                        break;
                    }

                    // No Parity -->fall through to the stop bit logic
                    else
                    {
                    }
                    // Intentional fall through!

                case Frame::eSTOP_BIT:
                    m_currentValue = true;         // Note: Stop and Start bits are both logical '1's

                    // Set my current bit to a Start bit ONCE all of my stop bits are done
                    if ( ++m_stopBit > m_frameP->getStopBits() )
                    {
                        m_currentBit = nextBit = Frame::eSTART_BIT;
                    }
                    break;
                }
            }

            // Process the current bit
            FrameBitColor::Color_T bitColor  = m_colorP->getColor( m_currentBit, m_currentValue );
            uint8_t                intensity = m_rampP->getIntensity( Cpl::System::ElapsedTime::deltaMilliseconds( m_timeMarker ), m_frameP->getBitTime() );
            m_outputP->write( newBit, bitColor, intensity, m_currentBit );
            if ( newBit )
            {
                CPL_SYSTEM_TRACE_MSG( SECT_, ("  currentBit=%d, nextBit=%d, bitValue=%d, intensity=%d, bitColor=%d (m_dataBit=%d, m_stopBit=%d)", m_currentBit, nextBit, m_currentValue, intensity, bitColor, m_dataBit, m_stopBit) );
            }
            m_currentBit = nextBit;
        }
    }

    // Check (safely) for any updates to my policies
    bool isDirty;
    m_lock.lock();
    isDirty = m_dirty;
    m_lock.unlock();
    if ( isDirty )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ("Updating Policies.  new policies: frameP=%p, streamP=%, colorP=%p, rampP=%p, outputP=%p",
                              m_newFrameP, m_newStreamP, m_newColorP, m_newRampP, m_newOutputP) );

        // Cache policy pointers for ONLY the policies that are being updated
        m_lock.lock();
        Frame*          tempFrameP  = m_newFrameP ? m_frameP : 0;
        DataStream*     tempStreamP = m_newStreamP ? m_streamP : 0;
        FrameBitColor*  tempColorP  = m_newColorP ? m_colorP : 0;
        IntensityRamp*  tempRampP   = m_newRampP ? m_rampP : 0;
        Output*         tempOutputP = m_newOutputP ? m_outputP : 0;

        // Update policies
        m_dirty   = false;
        m_frameP  = m_newFrameP ? m_newFrameP : m_frameP;
        m_colorP  = m_newColorP ? m_newColorP : m_colorP;
        m_outputP = m_newOutputP ? m_newOutputP : m_outputP;
        m_streamP = m_newStreamP ? m_newStreamP : m_streamP;
        m_rampP   = m_newRampP ? m_newRampP : m_rampP;
        m_lock.unlock();

        // Delete the policies that got change
        delete tempFrameP;  // Note: The C++ standard say its okay (i.e. its a NOP) to delete a null/zero pointer
        delete tempStreamP;
        delete tempColorP;
        delete tempRampP;
        delete tempOutputP;
    }
}


///////////////////////////////
bool Main::setPolicies( Frame*            framePolicyP,
                        DataStream*       streamPolicyP,
                        FrameBitColor*    colorPolicyP,
                        IntensityRamp*    rampPolicyP,
                        Output*           outputPolicyP
                        )
{
    // Do NOT accept a new change if we haven't accepted the last request
    bool isDirty;
    m_lock.lock();
    isDirty = m_dirty;
    m_lock.unlock();
    if ( isDirty )
    {
        return false;
    }

    // Accept the request (safely)
    m_lock.lock();
    m_dirty      = true;
    m_newFrameP  = framePolicyP;
    m_newStreamP = streamPolicyP;
    m_newColorP  = colorPolicyP;
    m_newRampP   = rampPolicyP;
    m_newOutputP = outputPolicyP;
    m_lock.unlock();
    return true;
}

/////////////////////////////////////////////    
const char* Main::getFramePolicyDescription( Cpl::Text::String& brief )
{
    brief = "none";
    if ( m_frameP )
    {
        Cpl::System::Mutex::ScopeBlock lock( m_lock );
        m_frameP->getDescription( brief );
    }

    return brief;}

const char* Main::getDataStreamPolicyDescription( Cpl::Text::String& brief )
{
    brief = "none";
    if ( m_streamP )
    {
        Cpl::System::Mutex::ScopeBlock lock( m_lock );
        m_streamP->getDescription( brief );
    }

    return brief;}

const char* Main::getFrameBitColorPolicyDescription( Cpl::Text::String& brief )
{
    brief = "none";
    if ( m_colorP )
    {
        Cpl::System::Mutex::ScopeBlock lock( m_lock );
        m_colorP->getDescription( brief );
    }

    return brief;}

const char* Main::getIntensityRampPolicyDescription( Cpl::Text::String& brief )
{
    brief = "none";
    if ( m_rampP )
    {
        Cpl::System::Mutex::ScopeBlock lock( m_lock );
        m_rampP->getDescription( brief );
    }

    return brief;}

const char* Main::getOutputPolicyDescription( Cpl::Text::String& brief )
{
    brief = "none";
    if ( m_outputP )
    {
        Cpl::System::Mutex::ScopeBlock lock( m_lock );
        m_outputP->getDescription( brief );
    }

    return brief;
}

