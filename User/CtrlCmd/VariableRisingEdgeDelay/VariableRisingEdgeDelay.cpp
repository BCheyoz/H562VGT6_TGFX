//
// File: VariableRisingEdgeDelay.cpp
//
// Code generated for Simulink model 'VariableRisingEdgeDelay'.
//
// Model version                  : 5.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:53 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "VariableRisingEdgeDelay.h"
#include <stdint.h>
#include <stdbool.h>
#include "RisingEdgeDetect.h"
#include "FallingEdgeDetect.h"
#include "Timer.h"

// System reset for referenced model: 'VariableRisingEdgeDelay'
void VariableRisingEdgeDelay::reset(void)
{
  VariableRisingEdgeDelay_DW.isTimeOut_DSTATE = false;
  VariableRisingEdgeDelay_DW.previousVal_DSTATE = 0U;
  EdgeOnMDLOBJ3.reset();
  EdgeOffMDLOBJ2.reset();
  DelayTimerMDLOBJ1.reset();
}

// Output and update for referenced model: 'VariableRisingEdgeDelay'
void VariableRisingEdgeDelay::step(const uint8_t *rtu_In, const float *rtu_delay,
  uint8_t *rty_Out, float rtp_TimeStep)
{
  // local block i/o variables
  float rtb_timerValue;
  bool rtb_edgeOn;
  bool rtb_edgeOff;
  bool rtb_timerState;
  uint8_t rtb_timerHold;
  uint8_t rtb_timerLaunch;
  uint8_t rtb_timerReset;
  rtb_timerHold = 0U;
  EdgeOnMDLOBJ3.step(rtu_In, &rtb_edgeOn);
  rtb_timerLaunch = rtb_edgeOn;
  EdgeOffMDLOBJ2.step(rtu_In, &rtb_edgeOff);
  rtb_timerReset = (rtb_edgeOff || VariableRisingEdgeDelay_DW.isTimeOut_DSTATE);
  DelayTimerMDLOBJ1.step(&rtb_timerLaunch, &rtb_timerReset, &rtb_timerHold,
    &rtb_timerValue, &rtb_timerState, rtp_TimeStep);
  if (rtb_timerState) {
    *rty_Out = VariableRisingEdgeDelay_DW.previousVal_DSTATE;
  } else {
    *rty_Out = *rtu_In;
  }

  VariableRisingEdgeDelay_DW.isTimeOut_DSTATE = (rtb_timerValue >= *rtu_delay);
  VariableRisingEdgeDelay_DW.previousVal_DSTATE = *rty_Out;
}

// Constructor
VariableRisingEdgeDelay::VariableRisingEdgeDelay():
  VariableRisingEdgeDelay_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
VariableRisingEdgeDelay::~VariableRisingEdgeDelay() = default;

//
// File trailer for generated code.
//
// [EOF]
//
