/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014, 2015  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Output.h"
#include "Cpl/Text/String.h"
#include "Golem/OutputDebug.h"
#include "Golem/OutputNeoPixel.h"


///
using namespace Golem::TShell::Cmd;



///////////////////////////
Output::Output( Golem::Main& application, const char* initialPolicyName, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "out" )
    , m_golem( application )
    , m_policyName( initialPolicyName )
{
}

Output::Output( Golem::Main& application, const char* initialPolicyName, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "out", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
    , m_policyName( initialPolicyName )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Output::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw()
{
    //Cpl::Text::String&  token      = context.getTokenBuffer();
    Cpl::Text::String&  outtext    = context.getOutputBuffer();
    bool                io         = true;
    unsigned            numParms   = tokens.numParameters();
    const char*         newName    = 0;
    Golem::Output*      newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_policyName );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }


    // Policy: OutputDebug
    if ( numParms == 2 && strcmp( tokens.getParameter( 1 ), "debug" ) == 0 )
    {
        newPolicyP = new Golem::OutputDebug();
        newName    = "OutputDebug";
    }

    // Policy: OutputNeoPixel
    else if ( numParms == 3 && strcmp( tokens.getParameter( 1 ), "neo" ) == 0 )
    {
        // ALL
        if ( strcmp( tokens.getParameter( 2 ), "all" ) == 0 )
        {
            newName    = "OutputNeoPixel(eALL)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eALL, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // eQUARTER
        else if ( strcmp( tokens.getParameter( 2 ), "4" ) == 0 )
        {
            newName    = "OutputNeoPixel(eQUARTER)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // ePAIRS_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "2spin" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS_SPIN_C)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_C, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // ePAIRS_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "2spincc" ) == 0 )
        {
            newName    = "OutputNeoPixel(ePAIRS_SPIN_C)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_CC, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // eQUARTER_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "4spin" ) == 0 )
        {
            newName    = "OutputNeoPixel(eQUARTER_SPIN_C)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_C, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // eQUARTER_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "4spincc" ) == 0 )
        {
            newName    = "OutputNeoPixel(eQUARTER_SPIN_CC)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_CC, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // Unrecognized/unsupported option
        else
        {
            return eERROR_INVALID_ARGS;
        }
    }

    // Update the application with the new policy
    if ( newPolicyP != 0 )
    {
        if ( !m_golem.setPolicies( 0, 0, 0, 0, newPolicyP ) )
        {
            return Command::eERROR_FAILED;
        }

        m_policyName = newName;
        outtext.format( "new output policy:= %s", m_policyName );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}

