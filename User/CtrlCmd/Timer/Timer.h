//
// File: Timer.h
//
// Code generated for Simulink model 'Timer'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:20 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef Timer_h_
#define Timer_h_
#include <stdbool.h>
#include <stdint.h>
#include "FallingEdgeDetect.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDetect.h"
#include <cstring>

// Class declaration for model Timer
class Timer final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'Timer'
  struct DW_Timer_T {
    uint32_t prevCounterValue_DSTATE;  // '<Root>/prevCounterValue'
  };

  // Copy Constructor
  Timer(Timer const&) = delete;

  // Assignment Operator
  Timer& operator= (Timer const&) & = delete;

  // Move Constructor
  Timer(Timer &&) = delete;

  // Move Assignment Operator
  Timer& operator= (Timer &&) = delete;

  // model step function
  void step(const uint8_t *rtu_On, const uint8_t *rtu_Reset, const uint8_t
            *rtu_Hold, float *rty_TimerValue, bool *rty_state, float
            rtp_TimeStep);

  // Reset function
  void reset();

  // Constructor
  Timer();

  // Destructor
  ~Timer();

  // private data and function members
 private:
  // Block states
  DW_Timer_T Timer_DW;

  // model instance variable for '<Root>/HoldOff'
  FallingEdgeDetect HoldOffMDLOBJ1;

  // model instance variable for '<Root>/HoldOn'
  RisingEdgeDetect HoldOnMDLOBJ2;

  // model instance variable for '<Root>/HoldState'
  R_S_FlipFlop HoldStateMDLOBJ3;

  // model instance variable for '<Root>/StartTimer'
  RisingEdgeDetect StartTimerMDLOBJ4;

  // model instance variable for '<Root>/StopTimer'
  RisingEdgeDetect StopTimerMDLOBJ5;

  // model instance variable for '<Root>/TimerState'
  R_S_FlipFlop TimerStateMDLOBJ6;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Timer'

#endif                                 // Timer_h_

//
// File trailer for generated code.
//
// [EOF]
//
