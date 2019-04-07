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


#include "Ramp.h"
#include "Cpl/Text/atob.h"
#include "Golem/RampPercent.h"
#include "Cpl/System/Trace.h"

#define SECT_   "cmd::ramp"

/// Namespaces
using namespace Golem::TShell::Cmd;



///////////////////////////
Ramp::Ramp( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) noexcept
    : Cpl::TShell::Dac::Cmd::Command( commandList, "ramp" )
    , m_golem( application )
{
}

Ramp::Ramp( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) noexcept
    : Cpl::TShell::Dac::Cmd::Command( commandList, "ramp", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Ramp::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::String&          newName    = context.getTokenBuffer();
    Cpl::Text::String&          outtext    = context.getOutputBuffer();
    bool                        io         = true;
    unsigned                    numParms   = tokens.numParameters();
    Golem::IntensityRamp*      newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getIntensityRampPolicyDescription( newName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Policy: RampPercent
    if ( numParms == 2 )
    {
        double rampTime;
        if ( !Cpl::Text::a2d( rampTime, tokens.getParameter( 1 ) ) )
        {
            return eERROR_INVALID_ARGS;
        }
        newPolicyP = new Golem::RampPercent( (float) rampTime );
    }

    // Unrecognized/unsupported option
    else
    {
        return eERROR_INVALID_ARGS;
    }

    // Update the application with the new policy
    if ( newPolicyP )
    {
        if ( !m_golem.setPolicies( 0, 0, 0, newPolicyP, 0 ) )
        {
            return Command::eERROR_FAILED;
        }

        outtext.format( "new ramp policy:= %s", newPolicyP->getDescription( newName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}


