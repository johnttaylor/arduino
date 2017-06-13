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


#include "Frame.h"


/// Namespaces
using namespace Golem::TShell::Cmd;



///////////////////////////
Frame::Frame( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "f" )
    , m_golem( application )
{
}

Frame::Frame( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "f", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Frame::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Frame& outfd ) throw()
{
    Cpl::Text::String&  newName    = context.getTokenBuffer();
    Cpl::Text::String&  outtext    = context.getOutputBuffer();
    bool                io         = true;
    unsigned            numParms   = tokens.numParameters();
    Golem::Frame*       newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getFramePolicyDescription( policyName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Policy: FrameSimple
    if ( numParms >= 2 && numParms <= 5 )
    {
        // Housekeeping
        numParms--;
        unsigned parmIdx = 2;

        // Loop through argument(s)
        while( numParms-- )
        {
            const char* param = tokens.getParameter( parmIdx );
            // Bit Time
            if ( strncmp( param, "b=", 3 ) == 0 )
            {
                uint32_t msec;
                if ( Cpl::Text::a2ul(msec,param+3) )
                {
                    

                newName +    = "OutputNeoPixel(eALL)";
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eALL, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // PAIRS
        else if ( strcmp( tokens.getParameter( 2 ), "2" ) == 0 )
        {
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
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // ePAIRS_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "2c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_C, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // ePAIRS_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "2cc" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::ePAIRS_SPIN_CC, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // eQUARTER_SPIN_C
        else if ( strcmp( tokens.getParameter( 2 ), "4c" ) == 0 )
        {
            newPolicyP = new Golem::OutputNeoPixel( Golem::OutputNeoPixel::eQUARTER_SPIN_C, OPTION_NEOPIXEL_CFG_NUM_PIXELS, OPTION_NEOPIXEL_CFG_PIN, OPTION_NEOPIXEL_CFG_IS_RGBW, OPTION_NEOPIXEL_CFG_NEO_TYPE + NEO_KHZ800 );
        }

        // eQUARTER_SPIN_CC
        else if ( strcmp( tokens.getParameter( 2 ), "4cc" ) == 0 )
        {
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

        outtext.format( "new frame policy:= %s", m_golem.getFramePolicyDescription( policyName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}


