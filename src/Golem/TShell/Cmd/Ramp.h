#ifndef Golem_TShell_Cmd_Ramp_h_
#define Golem_TShell_Cmd_Ramp_h_
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
#define GOLEMSHELLCMD_USAGE_RAMP_      "ramp\n" \
                                       "ramp <perctange>\n"
                                       
/// Detailed Help text
#ifndef GOLEMSHELLCMD_DETAIL_RAMP_
#define GOLEMSHELLCMD_DETAIL_RAMP_     "  Shell command sets the Golem SetIntensity policy.  The <perctange> argument is\n"\
                                       "  value between 0 and 0.5 and set the ramp time for both the on/off edges. If no\n" \
                                       "  argument is provide then the current policy is displayed."

#endif // ifndef allows detailed help to be compacted down to a single character if FLASH/code space is an issue


/// Namespaces
namespace Golem {
namespace TShell {
namespace Cmd {


/** This class implements a DAC Shell command
 */
class Ramp: public Cpl::TShell::Cmd::Command
{
protected:
    /// Reference to my application
    Golem::Main&                m_golem;


public:
    /// See Cpl::TShell::Command
    const char* getUsage() const noexcept { return GOLEMSHELLCMD_USAGE_RAMP_; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return GOLEMSHELLCMD_DETAIL_RAMP_; }


public:
    /// Constructor
    Ramp( Golem::Main& application, Cpl::Container::Map<Cpl::TShell::Command>& commandList ) noexcept;


public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* rawCmdString, Cpl::Io::Output& outfd ) noexcept;


};


};      // end namespace
};      // end namespace
};      // end namespace
#endif  // end header latch
