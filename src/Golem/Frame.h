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

    /// Stop bit options
    typedef enum
    {
        eZERO,
        eONE,
        eTWO
    } StopBits_T;

    /// Parity bit options
    typedef enum
    {
        eNONE,
        eEVEN,
        eODD
    } ParityBit_T;


public:
    /// Minimum data bit size for a Frame
    const uint8_t MINIMUM_DATA_BITS = 4;

    /// Maximum data bit size for a Frame
    const uint8_t MAXIMUM_DATA_BITS = 16;


public:
    //// Returns the number of data bits in the frame
    virtual uint8_t getNumberOfDataBits( void ) const = 0;

    /// Returns the number of stop bits in the frame
    virtual StopBits_T getStopBits( void ) const = 0;

    /// Returns the parity bit type for the frame
    virtual ParityBit_T getParity( void ) const = 0;
};


};      // end namespace
#endif  // end header latch