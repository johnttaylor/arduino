#ifndef Golem_ColorStream_h_
#define Golem_ColorStream_h_
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

#include "Golem/FrameBitColor.h"

/// Namespaces
namespace Golem {


/** This concrete class implements the Golem::FrameBitColor by sequencing
    through the available colors.
 */
class ColorStream : public FrameBitColor
{
public:
    /// Options for the coloring sequencing
    typedef enum
    {
        eON_BITS,   //!< Only 'use' colors for 1 bits
        eOFF_BITS,  //!< Only 'use' colors for 0 bits
        eALL_BITS   //!< Sequence colors for ALL bits
    } Sequence_T;


protected:
    /// 
    Sequence_T  m_option;
    ///
    Color_T     m_currentColor;
    ///
    bool        m_fast;

public:
    /// Constructor.
    ColorStream( Sequence_T  option, bool fastSeq = false );

public:
    /// See Golem::FrameBitColor
    Color_T getColor( Frame::Bit_T bitType, bool bitValue, bool newBit );

    /// See Golem::FrameBitColor
    const char* getDescription( Cpl::Text::String& brief );

};


};      // end namespace
#endif  // end header latch
