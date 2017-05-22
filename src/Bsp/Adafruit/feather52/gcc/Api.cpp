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

#include "Bsp/Api.h"
#include "Cpl/Io/Serial/Adafruit/Nrf5/InputOutput.h"
#include "Uart.h"
#include <ios>

static Cpl::Io::Serial::Adafruit::Nrf5::InputOutput fd_( Serial );    // Note: 'Serial' is the Arduino primary uart object!
extern Cpl::Io::Output& Bsp_Serial(void);



///////////////////////////////////////////
void Bsp_Api_initialize( void )
{
    // Configure the LEDs as output pins 
    pinMode( PIN_LED1, OUTPUT );
    pinMode( PIN_LED2, OUTPUT );
}

void Bsp_beginArduinoSerialObject( unsigned long baudrate, uint16_t frameConfig )
{
    fd_.start( baudrate, frameConfig );
    while ( fd_.isReady() == false )
        ;
}

Cpl::Io::Output& Bsp_Serial( void )
{
    return fd_;
}


////////////////////////////////////////////////////
/*
** Method stubs to satisfy the linker -->NOTE: C++ Streams are NOT supported by this BSP
*/
//void std::ios_base::Init::_S_ios_destroy()
//{
//}
//
//void std::ios_base::Init::_S_ios_create( bool  )
//{
//}

std::ios_base::Init::Init()
{
}

std::ios_base::Init::~Init()
{
}

