#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/Container/Map.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Mutex.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Dac/Maker.h"
#include "Cpl/TShell/Dac/Cmd/Help.h"
#include "Cpl/TShell/Dac/Cmd/Bye.h"
#include "Cpl/TShell/Dac/Cmd/Trace.h"
#include "Cpl/TShell/Dac/Cmd/Arduino/Dbg.h"
#include "Cpl/TShell/Dac/Cmd/FreeRTOS/Threads.h"
#include "Golem/Main.h"
#include "Golem/StreamAddress.h"
#include "Golem/OutputNeoPixel.h"
#include "Golem/FrameSimple.h"
#include "Golem/ColorSingle.h"
#include "Golem/RampNone.h"
#include "gestures.h"
#include "Arduino.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////

/// The output pin for controlling the NeoPixel LED strip
#ifndef OPTION_NEOPIXEL_CFG_PIN
#define OPTION_NEOPIXEL_CFG_PIN         30
#endif

/// Number of LEDs in the LED strip
#ifndef OPTION_NEOPIXEL_CFG_NUM_PIXELS
#define OPTION_NEOPIXEL_CFG_NUM_PIXELS  16
#endif

/// Type of NexoPixel strip
#ifndef OPTION_NEOPIXEL_CFG_NEO_TYPE
#define OPTION_NEOPIXEL_CFG_NEO_TYPE    NEO_GRBW
#endif

/// Boolean flag to indicate 3 color (RGB) or 4 color (RGBW) LEDs
#ifndef OPTION_NEOPIXEL_CFG_IS_RGBW
#define OPTION_NEOPIXEL_CFG_IS_RGBW     true
#endif

/// Thread priority for the debug shell
#ifndef OPTION_DAC_SHELL_THREAD_PRIORITY
#define OPTION_DAC_SHELL_THREAD_PRIORITY    CPL_SYSTEM_THREAD_PRIORITY_NORMAL 
#endif


// Cpl::System::Trace section identifier
#define SECT_ "sketch"


////////////////////////////////////////////////////////////

// Get access to the serial port
extern Cpl::Io::InputOutput& Bsp_Serial( void );

//extern uint32_t setLoopStacksize( void );
//uint32_t setLoopStacksize( void )
//{
//    return 512 * 6;
//}


////////////////////////////////////////////////////////////
static Cpl::System::Mutex policyLock;
static Golem::Main golem( policyLock );
extern uint32_t __etext[];

// Shell Processor and Shell commands
static Cpl::Container::Map<Cpl::TShell::Dac::Command>   cmdlist_;
static Cpl::TShell::Dac::Maker                          cmdProcessor_( cmdlist_ );
static Cpl::TShell::Dac::Cmd::Help                      helpCmd_( cmdlist_ );
static Cpl::TShell::Dac::Cmd::Trace                     traceCmd_( cmdlist_ );
static Cpl::TShell::Dac::Cmd::Arduino::Dbg              debugCmd_( cmdlist_ );
static Cpl::TShell::Dac::Cmd::FreeRTOS::Threads         threads_( cmdlist_ );
static Cpl::TShell::Stdio                               shell_( cmdProcessor_, "DAC-Shell", OPTION_DAC_SHELL_THREAD_PRIORITY );


/**************************************************************************
    Arduino setup function (automatically called at startup)
**************************************************************************/
void setup( void )
{
    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    // Initialize CPL
    Cpl::System::Api::initialize();

    // Initialize the Gesture Sub-system
    setupGestures();

    // Create/Launch the Command shell
    shell_.launch( Bsp_Serial(), Bsp_Serial() );

    // Set initial Golem Policies
    Golem::StreamAddress*   streamP = new Golem::StreamAddress( (void*) 0x1c000, (void*) __etext );
    Golem::FrameSimple*     frameP  = new Golem::FrameSimple( 500, 8, 0, Golem::Frame::eODD );
    Golem::ColorSingle*     colorP  = new Golem::ColorSingle( Golem::FrameBitColor::eGREEN );
    Golem::RampNone*        rampP   = new Golem::RampNone();
    Golem::OutputNeoPixel*  outputP = new Golem::OutputNeoPixel( OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
    golem.setPolicies( frameP, streamP, colorP, rampP, outputP );
}

/**************************************************************************
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
**************************************************************************/
void loop( void )
{
    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    // Run the Golem app at 20Hz
    golem.process();
    Cpl::System::Api::sleep( 50 );
}

