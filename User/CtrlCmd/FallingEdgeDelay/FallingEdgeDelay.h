//
// File: FallingEdgeDelay.h
//
// Code generated for Simulink model 'FallingEdgeDelay'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:31 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef FallingEdgeDelay_h_
#define FallingEdgeDelay_h_
#include <stdbool.h>
#include <stdint.h>
#include "FallingEdgeDetect.h"
#include "RisingEdgeDetect.h"
#include "Timer.h"
#include <cstring>

// Class declaration for model FallingEdgeDelay
class FallingEdgeDelay final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'FallingEdgeDelay'
  struct DW_FallingEdgeDelay_T {
    uint8_t previousVal_DSTATE;        // '<Root>/previousVal'
    bool isTimeOut_DSTATE;             // '<Root>/isTimeOut'
  };

  // Copy Constructor
  FallingEdgeDelay(FallingEdgeDelay const&) = delete;

  // Assignment Operator
  FallingEdgeDelay& operator= (FallingEdgeDelay const&) & = delete;

  // Move Constructor
  FallingEdgeDelay(FallingEdgeDelay &&) = delete;

  // Move Assignment Operator
  FallingEdgeDelay& operator= (FallingEdgeDelay &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const uint8_t *rtu_In, uint8_t *rty_Out, float rtp_Delay, float
            rtp_TimeStep);

  // Constructor
  FallingEdgeDelay();

  // Destructor
  ~FallingEdgeDelay();

  // private data and function members
 private:
  // Block states
  DW_FallingEdgeDelay_T FallingEdgeDelay_DW;

  // model instance variable for '<Root>/DelayTimer'
  Timer DelayTimerMDLOBJ1;

  // model instance variable for '<Root>/EdgeOff'
  RisingEdgeDetect EdgeOffMDLOBJ2;

  // model instance variable for '<Root>/EdgeOn'
  FallingEdgeDetect EdgeOnMDLOBJ3;
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
//  '<Root>' : 'FallingEdgeDelay'

#endif                                 // FallingEdgeDelay_h_

//
// File trailer for generated code.
//
// [EOF]
//
