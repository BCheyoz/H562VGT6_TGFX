//
// File: VariableRisingEdgeDelay.h
//
// Code generated for Simulink model 'VariableRisingEdgeDelay'.
//
// Model version                  : 5.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:06 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef VariableRisingEdgeDelay_h_
#define VariableRisingEdgeDelay_h_
#include <stdbool.h>
#include <stdint.h>
#include "FallingEdgeDetect.h"
#include "RisingEdgeDetect.h"
#include "Timer.h"
#include <cstring>

// Class declaration for model VariableRisingEdgeDelay
class VariableRisingEdgeDelay final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'VariableRisingEdgeDelay'
  struct DW_VariableRisingEdgeDelay_T {
    uint8_t previousVal_DSTATE;        // '<Root>/previousVal'
    bool isTimeOut_DSTATE;             // '<Root>/isTimeOut'
  };

  // Copy Constructor
  VariableRisingEdgeDelay(VariableRisingEdgeDelay const&) = delete;

  // Assignment Operator
  VariableRisingEdgeDelay& operator= (VariableRisingEdgeDelay const&) & = delete;

  // Move Constructor
  VariableRisingEdgeDelay(VariableRisingEdgeDelay &&) = delete;

  // Move Assignment Operator
  VariableRisingEdgeDelay& operator= (VariableRisingEdgeDelay &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const uint8_t *rtu_In, const float *rtu_delay, uint8_t *rty_Out, float rtp_TimeStep);

  // Constructor
  VariableRisingEdgeDelay();

  // Destructor
  ~VariableRisingEdgeDelay();

  // private data and function members
 private:
  // Block states
  DW_VariableRisingEdgeDelay_T VariableRisingEdgeDelay_DW;

  // model instance variable for '<Root>/DelayTimer'
  Timer DelayTimerMDLOBJ1;

  // model instance variable for '<Root>/EdgeOff'
  FallingEdgeDetect EdgeOffMDLOBJ2;

  // model instance variable for '<Root>/EdgeOn'
  RisingEdgeDetect EdgeOnMDLOBJ3;
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
//  '<Root>' : 'VariableRisingEdgeDelay'

#endif                                 // VariableRisingEdgeDelay_h_

//
// File trailer for generated code.
//
// [EOF]
//
