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
    virtual void beginAction() noexcept = 0;

    /// Action
    virtual void beginTilt() noexcept = 0;

    /// Action
    virtual void setAckExitVisualCue() noexcept = 0;

    /// Action
    virtual void setAckTiltVisualCue() noexcept = 0;

    /// Action
    virtual void setHomedVisualCue() noexcept = 0;

    /// Action
    virtual void setTiltVisualCue() noexcept = 0;

    /// Action
    virtual void startAckTimer() noexcept = 0;

    /// Action
    virtual void startMultiTimer() noexcept = 0;

    /// Action
    virtual void startSpinnerTimer() noexcept = 0;

    /// Action
    virtual void startTimeoutTimer() noexcept = 0;

    /// Action
    virtual void stopMultiTimer() noexcept = 0;

    /// Action
    virtual void stopSpinnerTimer() noexcept = 0;

    /// Action
    virtual void stopTimeoutTimer() noexcept = 0;

    /// Action
    virtual void updateAction() noexcept = 0;

    /// Action
    virtual void updateHomed() noexcept = 0;

    /// Action
    virtual void updateTilt() noexcept = 0;


public:
    /// Guard
    virtual bool isAckTimerExpired() noexcept = 0;

    /// Guard
    virtual bool isHomed() noexcept = 0;

    /// Guard
    virtual bool isMultiTimerExpired() noexcept = 0;

    /// Guard
    virtual bool isSpinnerChange() noexcept = 0;

    /// Guard
    virtual bool isSpinnerTimerExpired() noexcept = 0;

    /// Guard
    virtual bool isTilt() noexcept = 0;

    /// Guard
    virtual bool isTimeoutExpired() noexcept = 0;

    /// Guard
    virtual bool isValidAction() noexcept = 0;

    /// Guard
    virtual bool isValidMultiAction() noexcept = 0;


public:
    /// Virtual Destructor
    virtual ~ActionsFsmContext_(){}

};

};  /// end namespace(s)
#endif /// end header latch
