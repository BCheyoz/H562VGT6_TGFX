//
// File: FallingEdgeDelay.cpp
//
// Code generated for Simulink model 'FallingEdgeDelay'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:52 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "FallingEdgeDelay.h"
#include <stdint.h>
#include <stdbool.h>
#include "FallingEdgeDetect.h"
#include "RisingEdgeDetect.h"
#include "Timer.h"

// System reset for referenced model: 'FallingEdgeDelay'
void FallingEdgeDelay::reset(void)
{
  FallingEdgeDelay_DW.isTimeOut_DSTATE = false;
  FallingEdgeDelay_DW.previousVal_DSTATE = 0U;
  EdgeOnMDLOBJ3.reset();
  EdgeOffMDLOBJ2.reset();
  DelayTimerMDLOBJ1.reset();
}

// Output and update for referenced model: 'FallingEdgeDelay'
void FallingEdgeDelay::step(const uint8_t *rtu_In, uint8_t *rty_Out, float rtp_Delay, float rtp_TimeStep)
{
  // local block i/o variables
  float rtb_timerValue;
  bool rtb_edgeOn;
  bool rtb_edgeOff;
  bool rtb_timerState;
  uint8_t rtb_timerHold;
  uint8_t rtb_timerLaunch;
  uint8_t rtb_timerReset;
  EdgeOnMDLOBJ3.step(rtu_In, &rtb_edgeOn);
  rtb_timerLaunch = rtb_edgeOn;
  EdgeOffMDLOBJ2.step(rtu_In, &rtb_edgeOff);
  rtb_timerReset = (rtb_edgeOff || FallingEdgeDelay_DW.isTimeOut_DSTATE);
  rtb_timerHold = 0U;
  DelayTimerMDLOBJ1.step(&rtb_timerLaunch, &rtb_timerReset, &rtb_timerHold, &rtb_timerValue, &rtb_timerState, rtp_TimeStep);
  if (rtb_timerState) {
    *rty_Out = FallingEdgeDelay_DW.previousVal_DSTATE;
  } else {
    *rty_Out = *rtu_In;
  }

  FallingEdgeDelay_DW.isTimeOut_DSTATE = (rtb_timerValue >= rtp_Delay);
  FallingEdgeDelay_DW.previousVal_DSTATE = *rty_Out;
}

// Constructor
FallingEdgeDelay::FallingEdgeDelay():
  FallingEdgeDelay_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
FallingEdgeDelay::~FallingEdgeDelay() = default;

//
// File trailer for generated code.
//
// [EOF]
//
