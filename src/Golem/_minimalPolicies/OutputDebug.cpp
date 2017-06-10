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

#include "Golem/OutputDebug.h"
#include "Golem/IntensityRamp.h"
#include "Bsp/Api.h"


/// Namespaces
using namespace Golem;


/////////////////////////////////////
OutputDebug::OutputDebug() {}


/////////////////////////////////////
void OutputDebug::write( FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType )
{
    if ( bitColor != FrameBitColor::eOFF && colorIntensity > OPTION_GOLEM_INTENSITY_RAMP_MID_INTENSITY )
    {
        Bsp_Api_turnOn_debug1();
    }
    else
    {
        Bsp_Api_turnOff_debug1();
    }
}

void OutputDebug::stop( void )
{
    Bsp_Api_turnOff_debug1();
}
