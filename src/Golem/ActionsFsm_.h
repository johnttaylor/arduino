/*-----------------------------------------------------------------------------
* Blah..
*----------------------------------------------------------------------------*/
/** @file */

/* Command line options: -p CADIFRA -doxygen -o ActionsFsm -l cppx -Trace ActionsFsm.cdd   */
/* This file is generated from ActionsFsm.cdd - do not edit manually  */
/* Generated on:  version 3.7 */


#ifndef __ACTIONSFSM_GOLEM_H__
#define __ACTIONSFSM_GOLEM_H__

/** \class Golem::ActionsFsm
Here is the graph that shows the state machine this class implements
\dot
digraph G {
rankdir=TB;
bgcolor=transparent;
node[fontsize=8 shape=box];
edge[fontsize=8, arrowsize=0.5];
compound=true;
remincross=true;
nodesep=0.3;
ranksep=.4;
"Idle"->"defaultControllingLEDOutputs"[labeldistance = 2.0, taillabel=<isTilt()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black lhead=clusterControllingLEDOutputs];//Idle ControllingLEDOutputs
"defaultControllingLEDOutputs"->"Titled"[label=< <br ALIGN="LEFT"/>beginTilt();<br ALIGN="LEFT"/>setTiltVisualCue();...<br ALIGN="LEFT"/> > style=dotted];
"Titled"->"Homed"[label=<isHomed() / <br ALIGN="LEFT"/>startTimeoutTimer();<br ALIGN="LEFT"/>startMultiT...<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Titled Homed
"Homed"->"AckTilt"[label=<isValidAction()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Homed AckTilt
"defaultControllingLEDOutputs"->"TimedOut"[labeldistance=2.0, headlabel=<isTimeoutExpired()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black ltail=clusterControllingLEDOutputs];//ControllingLEDOutputs TimedOut
"TimedOut"->"Idle"[label=<isHomed()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //TimedOut Idle
"Homed"->"Titled"[label=<isTilt() / <br ALIGN="LEFT"/>startTimeoutTimer();<br ALIGN="LEFT"/>startMultiT...<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Homed Titled
"Titled"->"TimedOut"[label=<isMultiTimerExpired() / <br ALIGN="LEFT"/>stopTimeoutTimer();<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Titled TimedOut
"Homed"->"__C0"[label=<isMultiTimerExpired()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Homed __C0
"__C0"->"AckTilt"[label=<[isValidMultiAction()]>  color=black, fontname=arial, fontcolor=black]; //__C0 AckTilt
"__C0"->"Idle"[label=<[else]>  color=black, fontname=arial, fontcolor=black]; //__C0 Idle
"AckTilt"->"Spinning"[label=<isAckTimerExpired() / <br ALIGN="LEFT"/>beginAction();<br ALIGN="LEFT"/>startSpinnerTimer...<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //AckTilt Spinning
"Spinning"->"Spinning"[label=<isSpinnerChange() / <br ALIGN="LEFT"/>updateAction();<br ALIGN="LEFT"/>startSpinnerTime...<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Spinning Spinning
"Spinning"->"AckExit"[label=<isTilt()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Spinning AckExit
"Spinning"->"AckExit"[label=<isSpinnerTimerExpired()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //Spinning AckExit
"AckExit"->"WaitingForHomed"[label=<isAckTimerExpired()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //AckExit WaitingForHomed
"defaultroot"->"Idle"[label=<  > style=dotted];
"WaitingForHomed"->"Idle"[label=<isHomed()<br ALIGN="LEFT"/>>  color=black, fontname=arial, fontcolor=black]; //WaitingForHomed Idle
"Idle"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>Idle</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];subgraph "clusterControllingLEDOutputs"{fontname=arial; fontsize=8
color=black; style="rounded";
label=<ControllingLEDOutputs<br ALIGN="LEFT"/><br ALIGN="LEFT"/>>;
"Titled"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>Titled</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];"AckTilt"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>AckTilt</B><br ALIGN="LEFT"/>| <br ALIGN="LEFT"/><u>Entry</u>:<br ALIGN="LEFT"/>stopTimeoutTimer();<br ALIGN="LEFT"/>stopMultiTim...<br ALIGN="LEFT"/><br ALIGN="LEFT"/>}>];"defaultControllingLEDOutputs"[label=< >,shape=circle, fontsize=8, fixedsize=true, height=0.2, width=0.2, fillcolor=black, style=filled];
"Homed"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>Homed</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];"AckExit"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>AckExit</B><br ALIGN="LEFT"/>| <br ALIGN="LEFT"/><u>Entry</u>:<br ALIGN="LEFT"/>stopSpinnerTimer();<br ALIGN="LEFT"/>startAckTime...<br ALIGN="LEFT"/><br ALIGN="LEFT"/>}>];};
"Spinning"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>Spinning</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];"TimedOut"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>TimedOut</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];"defaultroot"[label=< >,shape=circle, fontsize=8, fixedsize=true, height=0.2, width=0.2, fillcolor=black, style=filled];
"WaitingForHomed"[shape=record, color=black, fontname=arial, style=rounded, label=<{<B>WaitingForHomed</B><br ALIGN="LEFT"/>|<br ALIGN="LEFT"/>}>];"__C0" [label=<  > shape=diamond, fixedsize=true, height=0.2, width=0.2, fontname=arial color=black];
}
\enddot
*/

#include "Golem/ActionsFsm_ext_.h"
#include "Golem/ActionsFsmContext_.h"

namespace Golem  {

    class ActionsFsm: public ActionsFsmContext_
    {
        public:
            ActionsFsm(void);

            int processEvent(ACTIONSFSM_EVENT_T msg);

            void initialize();

            // Helper(s) to reset history
            void resetHistoryControllingLEDOutputs(void);

            // Helper(s) to find out if the machine is in a certain state
            bool isInAckExit(void) const;
            bool isInWaitingForHomed(void) const;
            bool isInHomed(void) const;
            bool isInIdle(void) const;
            bool isInTitled(void) const;
            bool isInAckTilt(void) const;
            bool isInControllingLEDOutputs(void) const;
            bool isInSpinning(void) const;
            bool isInTimedOut(void) const;


            // Helper returning the innermost active state id.
            unsigned short getInnermostActiveState(void) const;


            // Returns the state name as string
            const char* getNameByState(unsigned short state) const;

            // Returns the event as string
            const char* getNameByEvent(ACTIONSFSM_EVENT_T evt) const;

            enum States{
                AckExit,
                WaitingForHomed,
                Homed,
                Idle,
                Titled,
                AckTilt,
                ControllingLEDOutputs,
                Spinning,
                TimedOut,
                NUM_STATES  // number of states in the machine
            };

        protected:
            int m_initialized;



        protected:
            // State variables
            struct stateVarsT{
                States stateVar;
                States stateVarControllingLEDOutputs;
            };
            stateVarsT stateVars;
            stateVarsT stateVarsCopy;



            /* Region handler prototypes  */
    };

}; //namespace Golem 


#endif // __ACTIONSFSM_GOLEM_H__
