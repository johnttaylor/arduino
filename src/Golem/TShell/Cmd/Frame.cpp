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
#include "Cpl/Text/atob.h"
#include "Golem/FrameSimple.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"

#define SECT_   "cmd::frame"

/// Namespaces
using namespace Golem::TShell::Cmd;



///////////////////////////
Frame::Frame( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Command>& commandList ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, "frame" )
    , m_golem( application )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Command::Result_T Frame::execute( Cpl::TShell::Context_& context, char* rawCmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( rawCmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              newName    = context.getTokenBuffer();
    Cpl::Text::String&              outtext    = context.getOutputBuffer();
    bool                            io         = true;
    unsigned                        numParms   = tokens.numParameters();
    Golem::Frame::FrameConfig_T     config     = m_golem.getFrameConfig();
    bool                            changed    = false;

    // Print current Policy
    if ( numParms == 1 )
    {
        io &= context.writeFrame( m_golem.getFramePolicyDescription( newName ) );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    // Policy: FrameSimple
    if ( numParms >= 2 && numParms <= 5 )
    {
    CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). [%s] [%s] [%s] [%s]", tokens.getParameter( 1 ), tokens.getParameter( 2 ), tokens.getParameter( 3 ), tokens.getParameter( 4 ) ));
 
        // Housekeeping
        numParms--;
        unsigned parmIdx = 1;

        // Loop through argument(s)
        while ( numParms-- )
        {
            const char* param = tokens.getParameter( parmIdx++ );
            CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). parmIdx=%d, numParms=%d, parm=[%s]", parmIdx, numParms, tokens.getParameter( parmIdx) ));

            // Bit Time
            if ( strncmp( param, "b=", 2 ) == 0 )
            {
                uint32_t msec;
                if ( !Cpl::Text::a2ul( msec, param + 2 ) )
                {
                    CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). Failed to parse bittime. param+3=[%s]", param + 2 ));
                    return eERROR_INVALID_ARGS;
                }
                config.m_bitTime = msec;
                changed          = true;
            }

            // Number of data bits
            if ( strncmp( param, "d=", 2 ) == 0 )
            {
                unsigned numBits;
                if ( !Cpl::Text::a2ui( numBits, param + 2 ) )
                {
                    CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). Failed to parse numBits. param+3=[%s]", param + 2 ));
                    return eERROR_INVALID_ARGS;
                }
                config.m_numDataBits = numBits;
                changed              = true;
            }

            // Number of stop bits
            if ( strncmp( param, "s=", 2 ) == 0 )
            {
                unsigned stopBits;
                if ( !Cpl::Text::a2ui( stopBits, param + 2 ) )
                {
                    CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). Failed to parse stopBits. param+3=[%s]", param + 2 ));
                    return eERROR_INVALID_ARGS;
                }
                config.m_stopBits = stopBits;
                changed           = true;
            }

            // Parity bits
            if ( strncmp( param, "p=", 2 ) == 0 )
            {
                switch ( param[2] )
                {
                case 'n':
                    config.m_parity = Golem::Frame::eNONE;
                    changed         = true;
                    break;
                case 'e':
                    config.m_parity = Golem::Frame::eEVEN;
                    changed         = true;
                    break;
                case 'o':
                    config.m_parity = Golem::Frame::eODD;
                    changed         = true;
                    break;
                default:
                    CPL_SYSTEM_TRACE_MSG( SECT_, ("frame(). Failed to parse parity. param+3=[%s]", param + 2 ));
                    return eERROR_INVALID_ARGS;
                }
            }
        }
    }
    
    // Unrecognized/unsupported option
    else
    {
        return eERROR_INVALID_ARGS;
    }

    // Update the application with the new policy
    if ( changed )
    {
        Golem::Frame* newPolicyP = new Golem::FrameSimple( config.m_bitTime, config.m_numDataBits, config.m_stopBits, config.m_parity );
        if ( !m_golem.setPolicies( newPolicyP, 0, 0, 0, 0 ) )
        {
            return Command::eERROR_FAILED;
        }

        outtext.format( "new frame policy:= %s", newPolicyP->getDescription( newName ) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }

    return eERROR_INVALID_ARGS;
}


