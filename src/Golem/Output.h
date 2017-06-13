#ifndef Golem_Output_h_
#define Golem_Output_h_
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

#include "Cpl/Text/String.h"
#include "Golem/FrameBitColor.h"
#include <stdint.h>

/// Namespaces
namespace Golem {


/** The class defines the interfaces used to actual 'light-up' the physically
    LED(s).  The concrete implementations of this class provides the policy 
    and/or pattern for which LED(s) get turned on/are for the individual frame 
    bits.

    NOTES:
    LED Pattern
    ------------
    All
    Half
    Quarter
    Spin Clockwise
    Spin Counter Clockwise
    Bounce
 */
class Output
{
public:
    /** Outputs the specified bit color.  The method assumes that a eSTART_BIT 
        bit indicates the start of a new 'word'.
     */
    virtual void write( bool newBit, FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType ) = 0;

    /// Force all of LED(s) to the off state (i.e. out of data)
    virtual void stop(void) = 0;

    /** This method returns a brief description of the current policy. The
        call is responsible for provide the memory/string that returns
        the description.
     */
    virtual void getDescription( Cpl::Text::String& brief ) = 0;

public:
    /// Virtual destructor
    virtual ~Output(){}

};


};      // end namespace
#endif  // end header latch
