/*-----------------------------------------------------------------------------
* Blah..
*----------------------------------------------------------------------------*/
/** @file */

/* Command line options: -p CADIFRA -doxygen -o ActionsFsm -l cppx -Trace ActionsFsm.cdd   */
/* This file is generated from ActionsFsm.cdd - do not edit manually  */
/* Generated on:  version 3.7 */


/* Include default headers (no header definition in UML state-chart provided) */
/* Define a header comment in your statediagram.model to define own includes here. */
#include "Golem/ActionsFsm_ext_.h"
#include "Golem/ActionsFsm_.h"
#include "Golem/ActionsFsm_trace_.h"

namespace Golem  {

    ActionsFsm::ActionsFsm(void)
    {
        m_initialized=0U;
    }



    /* State names */
    const char states[]=
        "AckExit\0WaitingForHomed\0Homed\0Idle\0Titled\0AckTilt\0ControllingLEDOutputs\0Spinning\0TimedOut\0";

    const unsigned short state_idx[]={
        0,8,24,30,35,42,50,72,81,90};

    /* Event names */
    const char events[]=
        "NO_MSG\0";

    const unsigned short evt_idx[]={
        0};

    const char* ActionsFsm::getNameByState(unsigned short state) const {
        return states+state_idx[state];
    }

    const char* ActionsFsm::getNameByEvent(ACTIONSFSM_EVENT_T evt) const {
        return events+evt_idx[evt];
    }


    // Helper(s) to reset history
    void ActionsFsm::resetHistoryControllingLEDOutputs(void){
        stateVars.stateVarControllingLEDOutputs=Titled;
    }


    // Helper(s) to find out if the machine is in a certain state

    bool ActionsFsm::isInAckExit(void) const {return(((stateVars.stateVarControllingLEDOutputs== AckExit)&&(stateVars.stateVar== ControllingLEDOutputs)) ? (true) : (false));}
    bool ActionsFsm::isInWaitingForHomed(void) const {return(((stateVars.stateVar== WaitingForHomed)) ? (true) : (false));}
    bool ActionsFsm::isInHomed(void) const {return(((stateVars.stateVarControllingLEDOutputs== Homed)&&(stateVars.stateVar== ControllingLEDOutputs)) ? (true) : (false));}
    bool ActionsFsm::isInIdle(void) const {return(((stateVars.stateVar== Idle)) ? (true) : (false));}
    bool ActionsFsm::isInTitled(void) const {return(((stateVars.stateVarControllingLEDOutputs== Titled)&&(stateVars.stateVar== ControllingLEDOutputs)) ? (true) : (false));}
    bool ActionsFsm::isInAckTilt(void) const {return(((stateVars.stateVarControllingLEDOutputs== AckTilt)&&(stateVars.stateVar== ControllingLEDOutputs)) ? (true) : (false));}
    bool ActionsFsm::isInControllingLEDOutputs(void) const {return(((stateVars.stateVar== ControllingLEDOutputs)) ? (true) : (false));}
    bool ActionsFsm::isInSpinning(void) const {return(((stateVars.stateVar== Spinning)) ? (true) : (false));}
    bool ActionsFsm::isInTimedOut(void) const {return(((stateVars.stateVar== TimedOut)) ? (true) : (false));}

    // Helper to get id of innermost active state

    unsigned short ActionsFsm::getInnermostActiveState(void) const {
        if(isInAckTilt()){
            return AckTilt;
        }else if(isInTitled()){
            return Titled;
        }else if(isInHomed()){
            return Homed;
        }else if(isInAckExit()){
            return AckExit;
        }else if(isInTimedOut()){
            return TimedOut;
        }else if(isInSpinning()){
            return Spinning;
        }else if(isInIdle()){
            return Idle;
        }else if(isInWaitingForHomed()){
            return WaitingForHomed;
        }else{
            return NUM_STATES;
        }
    }
    // Initialize method. Must be called once to init the machine
    void ActionsFsm::initialize(){
        //call on entry code of default states
        if(m_initialized==0U){
            m_initialized=1U;

            //Create copy of statevar
            stateVarsCopy = stateVars;

            // Set state vars to default states

            stateVarsCopy.stateVar = Idle; /* set init state of top state */
            stateVarsCopy.stateVarControllingLEDOutputs = Titled; /* set init state of ControllingLEDOutputs */

        // Copy state variables back
        stateVars = stateVarsCopy;

        }



    }

    // State machine event handler
    int ActionsFsm::processEvent(ACTIONSFSM_EVENT_T msg){

        int evConsumed = 0U;

    

        if(m_initialized==0U) return 0;

        //Create copy of statevar
        stateVarsCopy = stateVars;


        switch (stateVars.stateVar) {

            case Idle:
                if(isTilt()){
                    /* Transition from Idle to ControllingLEDOutputs */
                    evConsumed=16;

                    beginTilt();
                    setTiltVisualCue();
                    startTimeoutTimer();
                    startMultiTimer();
                    stateVarsCopy.stateVar = ControllingLEDOutputs;/* Default in entry chain  */
                    stateVarsCopy.stateVarControllingLEDOutputs = Titled;/* Default in entry chain  */

                    ActionsFsmTraceEvent(3);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case Idle  */

            case ControllingLEDOutputs:

                switch (stateVars.stateVarControllingLEDOutputs) {

                    case Titled:
                        if(isHomed()){
                            /* Transition from Titled to Homed */
                            evConsumed=16;

                            /* Action code for transition  */
                            startTimeoutTimer();
                            startMultiTimer();
                            setHomedVisualCue();
                            updateHomed();


                            /* adjust state variables  */
                            stateVarsCopy.stateVarControllingLEDOutputs = Homed;
                            ActionsFsmTraceEvent(4);
                        }else if(isMultiTimerExpired()){
                            /* Transition from Titled to TimedOut */
                            evConsumed=16;


                            /* Action code for transition  */
                            stopTimeoutTimer();


                            /* adjust state variables  */
                            stateVarsCopy.stateVar = TimedOut;
                            stateVarsCopy.stateVarControllingLEDOutputs = Titled;
                            ActionsFsmTraceEvent(7);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case Titled  */

                    case AckTilt:
                        if(isAckTimerExpired()){
                            /* Transition from AckTilt to Spinning */
                            evConsumed=16;


                            /* Action code for transition  */
                            beginAction();
                            startSpinnerTimer();


                            /* adjust state variables  */
                            stateVarsCopy.stateVar = Spinning;
                            stateVarsCopy.stateVarControllingLEDOutputs = Titled;
                            ActionsFsmTraceEvent(8);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case AckTilt  */

                    case Homed:
                        if(isMultiTimerExpired()){
                            if(isValidMultiAction()){
                                /* Transition from Homed to AckTilt */
                                evConsumed=16;

                                /* OnEntry code of state AckTilt */
                                stopTimeoutTimer();
                                stopMultiTimer();
                                startAckTimer();
                                setAckTiltVisualCue();

                                /* adjust state variables  */
                                stateVarsCopy.stateVarControllingLEDOutputs = AckTilt;
                                ActionsFsmTraceEvent(1);
                            }else{
                                /* Transition from Homed to Idle */
                                evConsumed=16;



                                /* adjust state variables  */
                                stateVarsCopy.stateVar = Idle;
                                stateVarsCopy.stateVarControllingLEDOutputs = Titled;
                                ActionsFsmTraceEvent(2);
                            } /*end of event selection */
                        }else if(isTilt()){
                            /* Transition from Homed to Titled */
                            evConsumed=16;

                            /* Action code for transition  */
                            startTimeoutTimer();
                            startMultiTimer();
                            setTiltVisualCue();
                            updateTilt();


                            /* adjust state variables  */
                            stateVarsCopy.stateVarControllingLEDOutputs = Titled;
                            ActionsFsmTraceEvent(3);
                        }else if(isValidAction()){
                            /* Transition from Homed to AckTilt */
                            evConsumed=16;

                            /* OnEntry code of state AckTilt */
                            stopTimeoutTimer();
                            stopMultiTimer();
                            startAckTimer();
                            setAckTiltVisualCue();

                            /* adjust state variables  */
                            stateVarsCopy.stateVarControllingLEDOutputs = AckTilt;
                            ActionsFsmTraceEvent(5);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case Homed  */

                    case AckExit:
                        if(isAckTimerExpired()){
                            /* Transition from AckExit to WaitingForHomed */
                            evConsumed=16;



                            /* adjust state variables  */
                            stateVarsCopy.stateVar = WaitingForHomed;
                            stateVarsCopy.stateVarControllingLEDOutputs = Titled;
                            ActionsFsmTraceEvent(8);
                        }else{
                            /* Intentionally left blank */
                        } /*end of event selection */
                    break; /* end of case AckExit  */

                    default:
                        /* Intentionally left blank */
                    break;
                } /* end switch ControllingLEDOutputs */

                /* Check if event was already processed  */
                if(evConsumed==0){

                    if(isTimeoutExpired()){
                        /* Transition from ControllingLEDOutputs to TimedOut */
                        evConsumed=16;
                        

                        /* adjust state variables  */
                        stateVarsCopy.stateVar = TimedOut;
                        ActionsFsmTraceEvent(6);
                    }else{
                        /* Intentionally left blank */
                    } /*end of event selection */
                }
            break; /* end of case ControllingLEDOutputs  */

            case Spinning:
                if(isSpinnerChange()){
                    /* Transition from Spinning to Spinning */
                    evConsumed=16;

                    /* Action code for transition  */
                    updateAction();
                    startSpinnerTimer();


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = Spinning;
                    ActionsFsmTraceEvent(9);
                }else if(isSpinnerTimerExpired()){
                    /* Transition from Spinning to AckExit */
                    evConsumed=16;


                    /* OnEntry code of state AckExit */
                    stopSpinnerTimer();
                    startAckTimer();
                    setAckExitVisualCue();

                    /* adjust state variables  */
                    stateVarsCopy.stateVar = ControllingLEDOutputs;
                    stateVarsCopy.stateVarControllingLEDOutputs = AckExit;
                    ActionsFsmTraceEvent(10);
                }else if(isTilt()){
                    /* Transition from Spinning to AckExit */
                    evConsumed=16;


                    /* OnEntry code of state AckExit */
                    stopSpinnerTimer();
                    startAckTimer();
                    setAckExitVisualCue();

                    /* adjust state variables  */
                    stateVarsCopy.stateVar = ControllingLEDOutputs;
                    stateVarsCopy.stateVarControllingLEDOutputs = AckExit;
                    ActionsFsmTraceEvent(3);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case Spinning  */

            case TimedOut:
                if(isHomed()){
                    /* Transition from TimedOut to Idle */
                    evConsumed=16;


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = Idle;
                    ActionsFsmTraceEvent(4);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case TimedOut  */

            case WaitingForHomed:
                if(isHomed()){
                    /* Transition from WaitingForHomed to Idle */
                    evConsumed=16;


                    /* adjust state variables  */
                    stateVarsCopy.stateVar = Idle;
                    ActionsFsmTraceEvent(4);
                }else{
                    /* Intentionally left blank */
                } /*end of event selection */
            break; /* end of case WaitingForHomed  */

            default:
                /* Intentionally left blank */
            break;
        } /* end switch stateVar_root */
    // Copy state variables back
    stateVars = stateVarsCopy;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "New State=%s", getNameByState(getInnermostActiveState()) ));

        return evConsumed;
    } // end processEvent







}; //namespace Golem 

