#include "colony_config.h"
#include "Bsp/Api.h"
#include "Cpl/Container/Map.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Mutex.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/Arduino/Dbg.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"
#include "Golem/Main.h"
#include "Golem/StreamAddress.h"
#include "Golem/OutputNeoPixel.h"
#include "Golem/FrameSimple.h"
#include "Golem/ColorStream.h"
#include "Golem/RampPercent.h"
#include "Golem/TShell/Cmd/Output.h"
#include "Golem/TShell/Cmd/Frame.h"
#include "Golem/TShell/Cmd/Ramp.h"
#include "Golem/TShell/Cmd/Color.h"
#include "Golem/TShell/Cmd/Stream.h"
#include "Golem/TShell/Cmd/Battery.h"
#include "Golem/gestures.h"
#include "Cpl/Io/Serial/Adafruit/Nrf5/BLE/InputOutput.h"
#include "Arduino.h"
#include "WMath.h"
#include <bluefruit.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////


/// Thread priority for the debug shell
#ifndef OPTION_DAC_SHELL_THREAD_PRIORITY
#define OPTION_DAC_SHELL_THREAD_PRIORITY        CPL_SYSTEM_THREAD_PRIORITY_NORMAL 
#endif

/// Since of the buffer to use for the BLE UART Received data
#ifndef OPTION_GOLEM_BLE_UART_RX_BUFFER
#define OPTION_GOLEM_BLE_UART_RX_BUFFER         BLE_UART_DEFAULT_FIFO_DEPTH   // default is 256 bytes
#endif

// Cpl::System::Trace section identifier
#define SECT_ "sketch"


////////////////////////////////////////////////////////////

// Get access to the serial port
extern Cpl::Io::InputOutput& Bsp_Serial( void );

extern uint32_t setLoopStacksize( void );
uint32_t setLoopStacksize( void )
{
    return 512;
}

////////////////////////////////////////////////////////////
// BLE Service
static BLEDis  bledis;
static BLEUart bleuart( OPTION_GOLEM_BLE_UART_RX_BUFFER );
static void setupBLE( void );
static void ble_connect_callback( void );
static void ble_disconnect_callback( uint8_t reason );
static Cpl::Io::Serial::Adafruit::Nrf5::BLE::InputOutput bleStdio( bleuart );


////////////////////////////////////////////////////////////
static Cpl::System::Mutex policyLock_;
static Adafruit_NeoPixel ledDriver_( OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
static Golem::Main golem_( policyLock_, ledDriver_ );
extern uint32_t __etext[];

// Shell Processor and Shell commands
static Cpl::Container::Map<Cpl::TShell::Command>        cmdlist_;
static Cpl::TShell::Maker                               cmdProcessor_( cmdlist_ );
static Cpl::TShell::Cmd::Help                           helpCmd_( cmdlist_ );
static Cpl::TShell::Cmd::Trace                          traceCmd_( cmdlist_ );
static Cpl::TShell::Cmd::Arduino::Dbg                   debugCmd_( cmdlist_ );
static Cpl::TShell::Cmd::FreeRTOS::Threads              threads_( cmdlist_ );
static Golem::TShell::Cmd::Output                       outputPolicy( golem_, ledDriver_, cmdlist_ );
static Golem::TShell::Cmd::Frame                        framePolicy( golem_, cmdlist_ );
static Golem::TShell::Cmd::Ramp                         rampPolicy( golem_, cmdlist_ );
static Golem::TShell::Cmd::Color                        colorPolicy( golem_, cmdlist_ );
static Golem::TShell::Cmd::Stream                       streamPolicy( golem_, cmdlist_ );
static Golem::TShell::Cmd::Battery                      batteryStatus( cmdlist_ );
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

    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( SECT_ );

    // Initialize the Gesture Sub-system
    setupGestures();

    // Initialize the random number generator (which is built on top of stdlib's rand() and srand())
    randomSeed( analogRead( 0 ) );

    // Create/Launch the Command shell
#ifndef USE_BLEUART_TSHELL
    shell_.launch( Bsp_Serial(), Bsp_Serial() ); 
#else
    bleStdio.start();
    shell_.launch( bleStdio, bleStdio );
#endif

    // Set initial Golem Policies
    Golem::DataStream*      streamP = new Golem::StreamAddress( (void*) 0x1c000, (void*) __etext );
    Golem::Frame*           frameP  = new Golem::FrameSimple( 500, 8, 1, Golem::Frame::eNONE );
    Golem::FrameBitColor*   colorP  = new Golem::ColorStream( Golem::ColorStream::eALL_BITS );
    Golem::IntensityRamp*   rampP   = new Golem::RampPercent( 0.0 );
    Golem::Output*          outputP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_C, ledDriver_, OPTION_NEOPIXEL_CFG_IS_RGBW );
    golem_.setPolicies( frameP, streamP, colorP, rampP, outputP );

    // Setup Bluetooth comms/services
    setupBLE();
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

///////////
void setupBLE( void )
{
    // Setup the BLE LED to be enabled on CONNECT
    // Note: This is actually the default behavior, but provided
    // here in case you want to control this LED manually via PIN 19
    Bluefruit.autoConnLed( true );

    Bluefruit.begin();
    Bluefruit.setName( "Golem" );
    Bluefruit.setConnectCallback( ble_connect_callback );
    Bluefruit.setDisconnectCallback( ble_disconnect_callback );

    // Configure and Start Device Information Service
    bledis.setManufacturer( "Integer Fox" );
    bledis.setModel( "Prototype I" );
    bledis.begin();

    // Configure and Start BLE Uart Service
    bleuart.begin();

    // 
    // Set up Advertising Packet
    //
    Bluefruit.Advertising.addFlags( BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE );
    Bluefruit.Advertising.addTxPower();

    // Include bleuart 128-bit uuid
    Bluefruit.Advertising.addService( bleuart );

    // There is no room for Name in Advertising packet
    // Use Scan response for Name
    Bluefruit.ScanResponse.addName();


    // 
    // Start Advertising
    //
    Bluefruit.Advertising.start();
}

void ble_connect_callback( void )
{
    Bsp_Serial().write( "BLE Connected.\r\n" );
}

void ble_disconnect_callback( uint8_t reason )
{
    Bsp_Serial().write( "BLE Disconnected. Auto starting advertising.\r\n" );
}

void rtos_idle_callback( void )
{
    // Don't call any other FreeRTOS blocking API()
    // Perform background task(s) here

    // Request CPU to enter low-power mode until an event/interrupt occurs
    waitForEvent();
}