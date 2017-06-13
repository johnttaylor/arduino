#ifndef Golem_OutputDebug_h_
#define Golem_OutputDebug_h_
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

#include "Golem/Output.h"

/// Namespaces
namespace Golem {


/** The concrete class define implements the Golem::Output interface using the
    one of the Bsp::Api debug LEDs.  The intent of this class is to facilitate
    debugging of Golem application without having a external 'LED stripped'
    wired to the board.
 */
class OutputDebug: public Output
{
public:
    /// Constructor
    OutputDebug();
    
    /// Destructor
    ~OutputDebug();
    
public:
    /// See Golem::Output
    void write( bool newBit, FrameBitColor::Color_T bitColor, uint8_t colorIntensity, Frame::Bit_T bitType );

    /// See Golem::Output
    void stop(void);

    /// See Golem::Output
    void getDescription( Cpl::Text::String& brief );
};


};      // end namespace
#endif  // end header latch
