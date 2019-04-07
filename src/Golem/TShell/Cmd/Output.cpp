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
Output::Output( Golem::Main& application, Adafruit_NeoPixel&  ledDriver, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) noexcept
    : Cpl::TShell::Dac::Cmd::Command( commandList, "output" )
    , m_golem( application )
    , m_ledDriver( ledDriver )
{
}

Output::Output( Golem::Main& application, Adafruit_NeoPixel&  ledDriver, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) noexcept
    : Cpl::TShell::Dac::Cmd::Command( commandList, "output", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
    , m_ledDriver( ledDriver )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Output::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::String&  policyName = context.getTokenBuffer();
    Cpl::Text::String&  outtext    = context.getOutputBuffer();
    bool                io         = true;
    unsigned            numParms   = tokens.numParameters();
    Golem::Output*      newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getOutputPolicyDescription( policyName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }


    // Policy: OutputDebug
    if ( numParms == 2 && strcmp( tokens.getParameter( 1 ), "debug" ) == 0 )
    {
        newPolicyP = new Golem::OutputDebug();
    }

    // Policy: OutputNeoPixel
    else if ( numParms == 3 && strcmp( tokens.getParameter( 1 ), "neo" ) == 0 )
    {
        // ALL
        if ( strcmp( tokens.getParameter( 2 ), "all" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eALL, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // eQUARTER
        else if ( strcmp( tokens.getParameter( 2 ), "4" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // ePAIRS_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "2c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_C, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // ePAIRS_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "2cc" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_CC, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // eQUARTER_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "4c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_C, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // eQUARTER_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "4cc" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_CC, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

         // ePAIRS_FAST_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "f2c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_C, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // ePAIRS_FAST_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "f2cc" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_FAST_SPIN_CC, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // eQUARTER_FAST_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "f4c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_C, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
        }

        // eQUARTER_FAST_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "f4cc" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_FAST_SPIN_CC, m_ledDriver, OPTION_NEOPIXEL_CFG_IS_RGBW );
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

        outtext.format( "new output policy:= %s", newPolicyP->getDescription( policyName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}


