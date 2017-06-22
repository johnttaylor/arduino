/*-----------------------------------------------------------------------------
* Blah..
*----------------------------------------------------------------------------*/
/** @file */

/* Command line options: -p CADIFRA -doxygen -o ActionsFsm -l cppx -Trace ActionsFsm.cdd   */
/* This file is generated from ActionsFsm_trace - do not edit manually  */
/* Generated on: version 3.7 */ 


#ifndef __ACTIONSFSM_TRACE_GOLEM_H__
#define __ACTIONSFSM_TRACE_GOLEM_H__

#include "Cpl/System/Trace.h"

#define SECT_ "Golem::ActionsFsm"


namespace Golem  {


/*Simulation which can be sent to the state-machine statediagram.simulator  */

#define ActionsFsmTraceEvent(a) CPL_SYSTEM_TRACE_MSG( SECT_, ( "Old State=%s, Event=%s", getNameByState(getInnermostActiveState()), ActionsFsmTraceEvents[a] ));

const char* const ActionsFsmTraceEvents[] = {
    "",
    "isMultiTimerExpired()[isValidMultiAction()]",
    "isMultiTimerExpired()[else]",
    "isTilt()",
    "isHomed()",
    "isValidAction()",
    "isTimeoutExpired()",
    "isAckTimerExpired()",
    "isMultiTimerExpired()",
    "isSpinnerChange()",
    "isSpinnerTimerExpired()"
};


}; //namespace Golem 


#endif
