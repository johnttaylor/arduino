#ifndef Golem_Frame_h_
#define Golem_Frame_h_
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

#include "colony_config.h"
#include "Cpl/Text/String.h"
#include <stdint.h>

/// Defines the minimum allowed Bit time for the Application
#ifndef OPTION_GOLEM_FRAME_MIN_BIT_TIME 
#define OPTION_GOLEM_FRAME_MIN_BIT_TIME     200
#endif

/// Defines the maximum allowed Bit time for the Application
#ifndef OPTION_GOLEM_FRAME_MAX_BIT_TIME 
#define OPTION_GOLEM_FRAME_MAX_BIT_TIME     3000
#endif

/// Namespaces
namespace Golem {


/** This class defines the interface for retrieving the current Data Frame
    configuration.
 */
class Frame
{
public:
    /// Different types of frame bits
    typedef enum
    {
        eSTART_BIT,
        eDATA_BIT,
        ePARITY_BIT,
        eSTOP_BIT
    } Bit_T;

    /// Parity bit options
    typedef enum
    {
        eNONE,
        eEVEN,
        eODD
    } ParityBit_T;

    /// All frame options (in a single struct)
    struct FrameConfig_T
    {
        uint32_t    m_bitTime;
        uint8_t     m_numDataBits;
        uint8_t     m_stopBits;
        ParityBit_T m_parity;

        /// Constructor
        FrameConfig_T():m_bitTime(MINIMUM_BIT_TIME),m_numDataBits(MINIMUM_DATA_BITS),m_stopBits(MINIMUM_STOP_BITS),m_parity(eNONE){}
    };

public:
    /// Minimum data bit size for a Frame
    static const uint8_t MINIMUM_DATA_BITS = 4;

    /// Maximum data bit size for a Frame
    static const uint8_t MAXIMUM_DATA_BITS = 16;

    /// Minimum stop bit size for a Frame
    static const uint8_t MINIMUM_STOP_BITS = 0;

    /// Maximum stop bit size for a Frame
    static const uint8_t MAXIMUM_STOP_BITS = 2;

    /// Minimum bit time for a Frame
    static const uint32_t MINIMUM_BIT_TIME = OPTION_GOLEM_FRAME_MIN_BIT_TIME;

    /// Maximum bit time for a Frame
    static const uint32_t MAXIMUM_BIT_TIME = OPTION_GOLEM_FRAME_MAX_BIT_TIME;


public:
    //// Returns the number of data bits in the frame
    virtual uint8_t getNumberOfDataBits( void ) const = 0;

    /// Returns the number of stop bits in the frame
    virtual uint8_t getStopBits( void ) const = 0;

    /// Returns the parity bit type for the frame
    virtual ParityBit_T getParity( void ) const = 0;

    /** Returns the 'baud rate' - well actual it returns the duration,
        in milliseconds, of a single bit.
     */
    virtual uint32_t getBitTime( void ) const = 0;

    /// Returns all configuration settings as a single struct
    virtual FrameConfig_T getConfig( void ) const = 0;


public:
    /** This method returns a brief description of the current policy. The
     call is responsible for provide the memory/string that returns
     the description.
     */
    virtual const char* getDescription( Cpl::Text::String& brief ) = 0;


public:
    /// Virtual destructor
    virtual ~Frame() {}
};


};      // end namespace
#endif  // end header latch
