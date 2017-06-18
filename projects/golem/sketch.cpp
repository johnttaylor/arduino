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
#include "Golem/RampPercent.h"
#include "Golem/TShell/Cmd/Output.h"
#include "Golem/TShell/Cmd/Frame.h"
#include "Golem/TShell/Cmd/Ramp.h"
#include "Golem/TShell/Cmd/Color.h"
#include "Golem/TShell/Cmd/Stream.h"
#include "Golem/gestures.h"
#include "Arduino.h"
#include "WMath.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////


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
static Cpl::System::Mutex policyLock_;
static Adafruit_NeoPixel ledDriver_( OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_NEO_TYPE+ NEO_KHZ800 );
static Golem::Main golem_( policyLock_, ledDriver_ );
extern uint32_t __etext[];

// Shell Processor and Shell commands
static Cpl::Container::Map<Cpl::TShell::Dac::Command>   cmdlist_;
static Cpl::TShell::Dac::Maker                          cmdProcessor_( cmdlist_ );
static Cpl::TShell::Dac::Cmd::Help                      helpCmd_( cmdlist_, "invoke_special_static_constructor"  );
static Cpl::TShell::Dac::Cmd::Trace                     traceCmd_( cmdlist_, "invoke_special_static_constructor"  );
static Cpl::TShell::Dac::Cmd::Arduino::Dbg              debugCmd_( cmdlist_, "invoke_special_static_constructor"  );
static Cpl::TShell::Dac::Cmd::FreeRTOS::Threads         threads_( cmdlist_, "invoke_special_static_constructor"  );
static Golem::TShell::Cmd::Output                       outputPolicy( golem_, ledDriver_, cmdlist_, "invoke_special_static_constructor"  );
static Golem::TShell::Cmd::Frame                        framePolicy( golem_, cmdlist_, "invoke_special_static_constructor"  );
static Golem::TShell::Cmd::Ramp                         rampPolicy( golem_, cmdlist_, "invoke_special_static_constructor"  );
static Golem::TShell::Cmd::Color                        colorPolicy( golem_, cmdlist_, "invoke_special_static_constructor"  );
static Golem::TShell::Cmd::Stream                       streamPolicy( golem_, cmdlist_, "invoke_special_static_constructor"  );
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

    // Initialize the random number generator (which is built on top of stdlib's rand() and srand())
    randomSeed(analogRead(0));

    // Create/Launch the Command shell
    shell_.launch( Bsp_Serial(), Bsp_Serial() );

    // Set initial Golem Policies
    Golem::DataStream*      streamP = new Golem::StreamAddress( (void*) 0x1c000, (void*) __etext );
    Golem::Frame*           frameP  = new Golem::FrameSimple( 500, 8, 1, Golem::Frame::eNONE );
    Golem::FrameBitColor*   colorP  = new Golem::ColorSingle( Golem::FrameBitColor::eGREEN );
    Golem::IntensityRamp*   rampP   = new Golem::RampPercent(0.0);
    Golem::Output*          outputP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eALL, ledDriver_, OPTION_NEOPIXEL_CFG_IS_RGBW );
    golem_.setPolicies( frameP, streamP, colorP, rampP, outputP );
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
    golem_.process();
    Cpl::System::Api::sleep( 50 );
}

