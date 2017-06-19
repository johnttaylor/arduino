#ifndef Golem_ActionsFsmContext_x_h_
#define Golem_ActionsFsmContext_x_h_
/*-----------------------------------------------------------------------------
* Blah..
*----------------------------------------------------------------------------*/
/** @file */



/* This file is auto-generated DO NOT MANUALLY EDIT this file! */


/// Namespace(s)
namespace Golem { 


/// Context (aka actions/guards) for my Finite State Machine
class ActionsFsmContext_
{
public:
    /// Action
    virtual void beginAction() throw() = 0;

    /// Action
    virtual void beginTilt() throw() = 0;

    /// Action
    virtual void setAckExitVisualCue() throw() = 0;

    /// Action
    virtual void setAckTiltVisualCue() throw() = 0;

    /// Action
    virtual void setHomedVisualCue() throw() = 0;

    /// Action
    virtual void setTiltVisualCue() throw() = 0;

    /// Action
    virtual void startAckTimer() throw() = 0;

    /// Action
    virtual void startMultiTimer() throw() = 0;

    /// Action
    virtual void startSpinnerTimer() throw() = 0;

    /// Action
    virtual void startTimeoutTimer() throw() = 0;

    /// Action
    virtual void stopMultiTimer() throw() = 0;

    /// Action
    virtual void stopSpinnerTimer() throw() = 0;

    /// Action
    virtual void stopTimeoutTimer() throw() = 0;

    /// Action
    virtual void updateAction() throw() = 0;

    /// Action
    virtual void updateHomed() throw() = 0;

    /// Action
    virtual void updateTilt() throw() = 0;


public:
    /// Guard
    virtual bool isAckTimerExpired() throw() = 0;

    /// Guard
    virtual bool isHomed() throw() = 0;

    /// Guard
    virtual bool isMultiTimerExpired() throw() = 0;

    /// Guard
    virtual bool isSpinnerChange() throw() = 0;

    /// Guard
    virtual bool isSpinnerTimerExpired() throw() = 0;

    /// Guard
    virtual bool isTilt() throw() = 0;

    /// Guard
    virtual bool isTimeoutExpired() throw() = 0;

    /// Guard
    virtual bool isValidAction() throw() = 0;

    /// Guard
    virtual bool isValidMultiAction() throw() = 0;


public:
    /// Virtual Destructor
    virtual ~ActionsFsmContext_(){}

};

};  /// end namespace(s)
#endif /// end header latch
