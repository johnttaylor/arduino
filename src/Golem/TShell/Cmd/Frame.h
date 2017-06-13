#ifndef Golem_TShell_Cmd_Frame_h_
#define Golem_TShell_Cmd_Frame_h_
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

#include "colony_config.h"
#include "Golem/Main.h"
#include "Cpl/TShell/Dac/Cmd/Command.h"

/** Usage
                                       "         1         2         3         4         5         6         7         8"
                                       "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
*/
#define GOLEMSHELLCMD_USAGE_FRAME_       "frame\n" \
                                         "frame [b=<ms>][d=<4-16>][s=<0-2>][p=<n,e,o>]\n"

/// Detailed Help text
#ifndef GOLEMSHELLCMD_DETAIL_FRAME_
#define GOLEMSHELLCMD_DETAIL_FRAME_    "  Shell command sets the Golem Frame policy.  'b' set the bit time in msec. 'd'\n"\
                                       "  sets the number of data bits. 's' sets the number of stop bit. 'p' sets the\n"\
                                       "  the parity bit. If no arguments are specified the current policy is displayed"

#endif // ifndef allows detailed help to be compacted down to a single character if FLASH/code space is an issue


/// Namespaces
namespace Golem {
namespace TShell {
namespace Cmd {


/** This class implements a DAC Shell command
 */
class Frame: public Cpl::TShell::Dac::Cmd::Command
{
protected:
    /// Reference to my application
    Golem::Main&                m_golem;


public:
    /// See Cpl::TShell::Dac::Command
    const char* getUsage() const throw() { return GOLEMSHELLCMD_USAGE_FRAME_; }

    /// See Cpl::TShell::Dac::Command
    const char* getHelp() const throw() { return GOLEMSHELLCMD_DETAIL_FRAME_; }


public:
    /// Constructor
    Frame( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList ) throw();

    /// Constructor.  Used to create a static instance of the command
    Frame( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Dac::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) throw();


public:
    /// See Cpl::TShell::Dac::Command
    Cpl::TShell::Dac::Command::Result_T execute( Cpl::TShell::Dac::Context_& context, Cpl::Text::Tokenizer::TextBlock& tokens, const char* rawInputString, Cpl::Io::Output& outfd ) throw();


};


};      // end namespace
};      // end namespace
};      // end namespace
#endif  // end header latch
