#ifndef Golem_TShell_Cmd_Output_h
#define Golem_TShell_Cmd_Output_h
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
/** @file */


#include "colony_config.h"
#include "Golem/Main.h"
#include "Cpl/TShell/Dac/Cmd/Command.h"
#include "Adafruit_NeoPixel.h"


/** Usage
                                       "         1         2         3         4         5         6         7         8"
                                       "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
*/
#define GOLEMSHELLCMD_USAGE_OUTPUT_      "output\n" \
                                         "output debug\n"  \
                                         "output neo (all|2|4|2c|2cc|4c|4cc|f2c|f2cc|f4c|f4cc)\n"

/// Detailed Help text
#ifndef GOLEMSHELLCMD_DETAIL_OUTPUT_
#define GOLEMSHELLCMD_DETAIL_OUTPUT_   "  Shell command set the Golem output policy.  If no arguments are specified\n" \
                                       "  the current policy is displayed"

#endif // ifndef allows detailed help to be compacted down to a single character if FLASH/code space is an issue


///
namespace Golem {
namespace TShell {
namespace Cmd {



/** This class implements a DAC Shell command
 */
class Output : public Cpl::TShell::Dac::Cmd::Command
{
protected:
    /// Reference to my application
    Golem::Main&        m_golem;

    /// Reference to the NeoPixel driver
    Adafruit_NeoPixel&  m_ledDriver;


public:
    /// See Cpl::TShell::Dac::Command
    const char* getUsage() const throw() { return GOLEMSHELLCMD_USAGE_OUTPUT_; }

    /// See Cpl::TShell::Dac::Command
    const char* getHelp() const throw() { return GOLEMSHELLCMD_DETAIL_OUTPUT_; }


public:
    /// Constructor
    Output( Golem::Main& application, Adafruit_NeoPixel& ledDriver, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw();

    /// Constructor.  Used to create a static instance of the command
    Output( Golem::Main& application, Adafruit_NeoPixel& ledDriver, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw();


public:
    /// See Cpl::TShell::Dac::Command
    Cpl::TShell::Dac::Command::Result_T execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw();

};

};      // end namespaces
};
};
#endif  // end header latch
