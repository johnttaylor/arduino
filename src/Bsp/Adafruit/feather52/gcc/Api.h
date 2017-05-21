#ifndef Bsp_feather52_Api_h_
#define Bsp_feather52_Api_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2017  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file


    This BSP is developed/build with a 'just-in-time' approach.  This means
    as functional is added to the BSP incrementally as there is 'client'
    need for.  This BSP does not claim to provide full/complete functional
    and/or APIs for everything the board supports.


    DO NOT include this file directly! Instead include the generic BSP
    interface - src/Bsp/Api.h - and then configure your project's
    'colony_map.h' to include THIS file.

*----------------------------------------------------------------------------*/


#include "colony_config.h"
#include "variants/feather52/variant.h"
#include "cores/nRF5/wiring_digital.h"
#include "cores/nRF5/wiring_constants.h"
#include "cores/nRF5/freertos/source/include/FreeRTOS.h"



//////////////////////////////////////////////////////////
/// Board Specific APIs
//////////////////////////////////////////////////////////

/** Initialize the primary serial port AND it waits for
    a Host to connect to the serial port. 
*/
void Bsp_beginArduinoSerialObject( unsigned long baudrate, uint16_t frameConfig );

/** Returns a reference to the Primary Serial Port Object. This is implicitly 
    exposed (can't directly expose it due to racing conditions on how the colony_map.h 
    header files work).  This means that the application must cut-n-paste the following
    extern statement to get access to this method.

    extern Cpl::Io::Output& Bsp_Serial(void);
*/



//////////////////////////////////////////////////////////
/// Generic APIs
//////////////////////////////////////////////////////////

/// Generic API
#define Bsp_Api_nop_MAP()                       __asm("nop")                

/// Generic API
#define Bsp_Api_disableIrqs_MAP()               taskDISABLE_INTERRUPTS()

/// Generic API
#define Bsp_Api_enableIrqs_MAP()                taskENABLE_INTERRUPTS()

/// Generic API
#define Bsp_Api_pushAndDisableIrqs_MAP()        taskDISABLE_INTERRUPTS()    // FIXME: This really needs to PUSH the IRQ state!!!

/// Generic API
#define Bsp_Api_popIrqs_MAP()                   taskENABLE_INTERRUPTS()     // FIXME: This really needs to POP the IRQ state!!!!




/// Default Mapping for the "debug LED 1"
#ifndef OPTION_BSP_DEBUG_LED1_INDEX
#define OPTION_BSP_DEBUG_LED1_INDEX             PIN_LED1
#endif

/// Default Mapping for the "debug LED 2"
#ifndef OPTION_BSP_DEBUG_LED2_INDEX
#define OPTION_BSP_DEBUG_LED2_INDEX             PIN_LED2
#endif


/// Generic API
#define Bsp_Api_turnOn_debug1_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, HIGH )

/// Generic API
#define Bsp_Api_turnOff_debug1_MAP()            digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, LOW )

/// Generic API
#define Bsp_Api_toggle_debug1_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED1_INDEX, !digitalRead(OPTION_BSP_DEBUG_LED1_INDEX) )


/// Generic API
#define Bsp_Api_turnOn_debug2_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, HIGH )

/// Generic API
#define Bsp_Api_turnOff_debug2_MAP()            digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, LOW )

/// Generic API
#define Bsp_Api_toggle_debug2_MAP()             digitalWrite( OPTION_BSP_DEBUG_LED2_INDEX, !digitalRead(OPTION_BSP_DEBUG_LED2_INDEX) )



#endif  // end header latch
