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


#include "Stream.h"
#include "Cpl/Text/atob.h"
#include "Golem/StreamAddress.h"
#include "Cpl/System/Trace.h"

#define SECT_   "cmd::stream"

/// Namespaces
using namespace Golem::TShell::Cmd;



///////////////////////////
Stream::Stream( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "stream" )
    , m_golem( application )
{
}

Stream::Stream( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "stream", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Stream::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw()
{
    Cpl::Text::String&          newName    = context.getTokenBuffer();
    Cpl::Text::String&          outtext    = context.getOutputBuffer();
    bool                        io         = true;
    unsigned                    numParms   = tokens.numParameters();
    Golem::DataStream*          newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getDataStreamPolicyDescription( newName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Policy: StreamAddress
    bool            updatePolicy = false;
    unsigned long   saddr = 0;
    unsigned long   eaddr = 0;
    if ( numParms >= 3 )
    {
        if ( !Cpl::Text::a2ul( saddr, tokens.getParameter( 1 ), 0 ) )
        {
            return eERROR_INVALID_ARGS;
        }
        if ( !Cpl::Text::a2ul( eaddr, tokens.getParameter( 2 ), 0 ) )
        {
            return eERROR_INVALID_ARGS;
        }
        updatePolicy = true;
    }

    bool loop = false;
    if ( numParms >= 4 )
    {
        if ( *(tokens.getParameter( 3 )) == '1' || *(tokens.getParameter( 3 )) == 't' )
        {
            loop = true;
        }
    }
    bool shuffle = false;
    if ( numParms == 5 )
    {
        if ( *(tokens.getParameter( 4 )) == '1' || *(tokens.getParameter( 4 )) == 't' )
        {
            shuffle = true;
        }
    }

    // Create the Policy object
    if ( updatePolicy )
    {
        newPolicyP = new Golem::StreamAddress( (void*) saddr, (void*) eaddr, loop, shuffle );
    }


    // Unrecognized/unsupported option
    else
    {
        return eERROR_INVALID_ARGS;
    }

    // Update the application with the new policy
    if ( newPolicyP )
    {
        if ( !m_golem.setPolicies( 0, newPolicyP, 0, 0, 0 ) )
        {
            return Command::eERROR_FAILED;
        }

        outtext.format( "new ramp policy:= %s", newPolicyP->getDescription( newName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}


