#ifndef Golem_TShell_Cmd_Stream_h_
#define Golem_TShell_Cmd_Stream_h_
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
#include "Cpl/TShell/Cmd/Command.h"

/** Usage
                                       "         1         2         3         4         5         6         7         8"
                                       "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
*/
#define GOLEMSHELLCMD_USAGE_STREAM_     "stream\n" \
                                        "stream <saddr> <eaddr> [[<loop>] <shuffle>]\n"

/// Detailed Help text
#ifndef GOLEMSHELLCMD_DETAIL_STREAM_
#define GOLEMSHELLCMD_DETAIL_STREAM_   "  Shell command sets the Golem DataStream policy.  The <saddr> and <eaddr> are\n" \
                                       "  memory start and stop addresses.  The <loop> and <shuffle> arguments are\n"\
                                       "  boolean (1|0) values.If no argument is provide then the current policy is\n" \
                                       "  displayed."

#endif // ifndef allows detailed help to be compacted down to a single character if FLASH/code space is an issue


/// Namespaces
namespace Golem {
namespace TShell {
namespace Cmd {


/** This class implements a DAC Shell command
 */
class Stream: public Cpl::TShell::Cmd::Command
{
protected:
    /// Reference to my application
    Golem::Main&                m_golem;


public:
    /// See Cpl::TShell::Command
    const char* getUsage() const noexcept { return GOLEMSHELLCMD_USAGE_STREAM_; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return GOLEMSHELLCMD_DETAIL_STREAM_; }


public:
    /// Constructor
    Stream( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Command>& commandList ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* rawCmdString, Cpl::Io::Output& outfd ) noexcept;


};


};      // end namespace
};      // end namespace
};      // end namespace
#endif  // end header latch
