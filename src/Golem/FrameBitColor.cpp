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

#include "Golem/FrameBitColor.h"

/// Namespaces
using namespace Golem;


const char* FrameBitColor::toString( Color_T color )
{
    switch ( color )
    {
    case eRED:      return "red";
    case eGREEN:    return "green";
    case eBLUE:     return "blue";
    case eWHITE:    return "white";
    case eOFF:      return "off";
    }

    return "unknown";
}
