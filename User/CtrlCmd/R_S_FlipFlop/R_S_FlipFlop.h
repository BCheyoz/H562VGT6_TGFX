//
// File: R_S_FlipFlop.h
//
// Code generated for Simulink model 'R_S_FlipFlop'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:50:27 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef R_S_FlipFlop_h_
#define R_S_FlipFlop_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model R_S_FlipFlop
class R_S_FlipFlop final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'R_S_FlipFlop'
  struct DW_R_S_FlipFlop_T {
    bool prevVal_DSTATE;               // '<Root>/prevVal'
  };

  // Copy Constructor
  R_S_FlipFlop(R_S_FlipFlop const&) = delete;

  // Assignment Operator
  R_S_FlipFlop& operator= (R_S_FlipFlop const&) & = delete;

  // Move Constructor
  R_S_FlipFlop(R_S_FlipFlop &&) = delete;

  // Move Assignment Operator
  R_S_FlipFlop& operator= (R_S_FlipFlop &&) = delete;

  // model step function
  void step(const bool *rtu_set, const bool *rtu_reset, bool *rty_stt);

  // Reset function
  void reset();

  // Constructor
  R_S_FlipFlop();

  // Destructor
  ~R_S_FlipFlop();

  // private data and function members
 private:
  // Block states
  DW_R_S_FlipFlop_T R_S_FlipFlop_DW;
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
//  '<Root>' : 'R_S_FlipFlop'

#endif                                 // R_S_FlipFlop_h_

//
// File trailer for generated code.
//
// [EOF]
//
