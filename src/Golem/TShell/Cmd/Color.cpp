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


#include "Color.h"
#include "Cpl/Text/atob.h"
#include "Golem/ColorSingle.h"
#include "Golem/ColorRainbow.h"
#include "Cpl/System/Trace.h"

#define SECT_   "cmd::ramp"

/// Namespaces
using namespace Golem::TShell::Cmd;

static Golem::FrameBitColor::Color_T convertToEnum_( const char* string );

///////////////////////////
Color::Color( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "color" )
    , m_golem( application )
{
}

Color::Color( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw()
    : Cpl::TShell::Dac::Cmd::Command( commandList, "color", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance )
    , m_golem( application )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Color::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw()
{
    Cpl::Text::String&          newName    = context.getTokenBuffer();
    Cpl::Text::String&          outtext    = context.getOutputBuffer();
    bool                        io         = true;
    unsigned                    numParms   = tokens.numParameters();
    Golem::FrameBitColor*       newPolicyP = 0;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getFrameBitColorPolicyDescription( newName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Policy: Single Color
    if ( numParms == 4 && strcmp( tokens.getParameter( 1 ), "bin" ) == 0 )
    {
         newPolicyP = new Golem::ColorSingle( convertToEnum_(tokens.getParameter( 2 )), convertToEnum_(tokens.getParameter( 3 )) );
    }


    // Policy: Rainbow Colors
    if ( numParms == 7 && strcmp( tokens.getParameter( 1 ), "bits" ) == 0 )
    {
         newPolicyP = new Golem::ColorRainbow( convertToEnum_(tokens.getParameter( 2 )), convertToEnum_(tokens.getParameter( 3 )), convertToEnum_(tokens.getParameter( 4 )), convertToEnum_(tokens.getParameter( 5 )), convertToEnum_(tokens.getParameter( 6 )) );
    }

    // Update the application with the new policy
    if ( newPolicyP )
    {
        if ( !m_golem.setPolicies( 0, 0, newPolicyP, 0, 0 ) )
        {
            return Command::eERROR_FAILED;
        }

        outtext.format( "new color policy:= %s", newPolicyP->getDescription( newName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}

Golem::FrameBitColor::Color_T convertToEnum_( const char* string )
{
    if ( *string == 'r' )
    {
        return Golem::FrameBitColor::eRED;
    }
    else if ( *string == 'g' )
    {
        return Golem::FrameBitColor::eGREEN;
    }
    else if ( *string == 'b' )
    {
        return Golem::FrameBitColor::eBLUE;
    }
    else if ( *string == 'w' )
    {
        return Golem::FrameBitColor::eWHITE;
    }

    return Golem::FrameBitColor::eOFF;
}