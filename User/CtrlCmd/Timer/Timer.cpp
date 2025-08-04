//
// File: Timer.cpp
//
// Code generated for Simulink model 'Timer'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:03:25 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "Timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "RisingEdgeDetect.h"
#include "FallingEdgeDetect.h"
#include "R_S_FlipFlop.h"

// System reset for referenced model: 'Timer'
void Timer::reset(void)
{
  Timer_DW.prevCounterValue_DSTATE = 0U;
  HoldOnMDLOBJ2.reset();
  HoldOffMDLOBJ1.reset();
  StopTimerMDLOBJ5.reset();
  HoldStateMDLOBJ3.reset();
  StartTimerMDLOBJ4.reset();
  TimerStateMDLOBJ6.reset();
}

// Output and update for referenced model: 'Timer'
void Timer::step(const uint8_t *rtu_On, const uint8_t *rtu_Reset, const uint8_t *
                 rtu_Hold, float *rty_TimerValue, bool *rty_state, float
                 rtp_TimeStep)
{
  // local block i/o variables
  bool rtb_HoldOn;
  bool rtb_HoldOff;
  bool rtb_StopTimer;
  bool rtb_HoldState;
  bool rtb_StartTimer;
  uint32_t rtb_CounterValue;
  bool rtb_ResetHoldOff;
  HoldOnMDLOBJ2.step(rtu_Hold, &rtb_HoldOn);
  HoldOffMDLOBJ1.step(rtu_Hold, &rtb_HoldOff);
  StopTimerMDLOBJ5.step(rtu_Reset, &rtb_StopTimer);
  rtb_ResetHoldOff = (rtb_HoldOff || rtb_StopTimer);
  HoldStateMDLOBJ3.step(&rtb_HoldOn, &rtb_ResetHoldOff, &rtb_HoldState);
  StartTimerMDLOBJ4.step(rtu_On, &rtb_StartTimer);
  TimerStateMDLOBJ6.step(&rtb_StartTimer, &rtb_StopTimer, rty_state);
  if (rtb_HoldState) {
    rtb_CounterValue = Timer_DW.prevCounterValue_DSTATE;
  } else if (*rty_state) {
    rtb_CounterValue = Timer_DW.prevCounterValue_DSTATE + 1U;
  } else {
    rtb_CounterValue = 0U;
  }

  *rty_TimerValue = static_cast<float>(rtb_CounterValue) * rtp_TimeStep;
  Timer_DW.prevCounterValue_DSTATE = rtb_CounterValue;
}

// Constructor
Timer::Timer():
  Timer_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Timer::~Timer() = default;

//
// File trailer for generated code.
//
// [EOF]
//
