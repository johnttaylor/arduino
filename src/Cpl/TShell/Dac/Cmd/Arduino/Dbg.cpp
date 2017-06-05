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

#include "Dbg.h"
#include "debug.h"
#include "Cpl/Text/String.h"


///
using namespace Cpl::TShell::Dac::Cmd::Arduino;



///////////////////////////
Dbg::Dbg( Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw()
:Cpl::TShell::Dac::Cmd::Command(commandList, "dbg")
    {
    }

Dbg::Dbg( Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw()
:Cpl::TShell::Dac::Cmd::Command(commandList, "dbg", ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance)
    {
    }


/////////////////////////////////////////////////////////
Cpl::TShell::Dac::Command::Result_T Dbg::execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw()
    {
    //Cpl::Text::String&  token    = context.getTokenBuffer();
    //Cpl::Text::String&  outtext  = context.getOutputBuffer();
    unsigned            numParms = tokens.numParameters();

    // Error checking
    if ( numParms > 2 )
        {
        return Command::eERROR_EXTRA_ARGS;
        }

    // Global enable/disable
    if ( numParms == 2 && strcmp(tokens.getParameter(1), "mem") == 0 )
        {
        dbgMemInfo();   // Outputs over the 'native' arduino serial port which in theory is the same serial port being used for the DAC shell.
        return Command::eSUCCESS;
        }

    return eERROR_INVALID_ARGS;
    }
