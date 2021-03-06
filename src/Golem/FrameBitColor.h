#ifndef Golem_FrameBitColor_h_
#define Golem_FrameBitColor_h_
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

#include "Golem/Frame.h"

/// Namespaces
namespace Golem {


/** This class defines the interface for mapping a LED color to a given
    bit in a Golem Data Stream.  Different concrete child classes provide
    different color options.
 */
class FrameBitColor
{
public:
    /// Possible colors
    typedef enum
    {
        eRED,
        eGREEN,
        eBLUE,
        eWHITE,
        eOFF
    } Color_T;



public:
    /** Returns the color for the specified bit.  The method assumes that
        a eSTART_BIT bit is the start of a new 'word'.
     */
    virtual Color_T getColor( Frame::Bit_T bitType, bool bitValue, bool newBit ) = 0;

public:
    /** This method returns a brief description of the current policy. The
        call is responsible for provide the memory/string that returns
        the description.
     */
    virtual const char* getDescription( Cpl::Text::String& brief ) = 0;

public:
    // Converts an enum value to text
    static const char* toString( Color_T color );

public:
    /// Virtual destructor
    virtual ~FrameBitColor() {}
};


};      // end namespace
#endif  // end header latch
