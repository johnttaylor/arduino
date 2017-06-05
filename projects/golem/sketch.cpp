#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/Container/Map.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Dac/Maker.h"
#include "Cpl/TShell/Dac/Cmd/Help.h"
#include "Cpl/TShell/Dac/Cmd/Bye.h"
#include "Cpl/TShell/Dac/Cmd/Trace.h"
#include "Cpl/TShell/Dac/Cmd/Arduino/Dbg.h"
#include "Cpl/TShell/Dac/Cmd/FreeRTOS/Threads.h"
#include "gestures.h"
#include "Arduino.h"
#include <stdlib.h>
//#include "debug.h"

////////////////////////////////////////////////////////////
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

}




/**************************************************************************
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
**************************************************************************/
void loop( void )
{
    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    //// TODO: Remove -->debug stuff for now
    //dbgMemInfo();
    Cpl::System::Api::sleep( 1000 );
}
