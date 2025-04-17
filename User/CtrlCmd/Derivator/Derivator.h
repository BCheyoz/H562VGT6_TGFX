//
// File: Derivator.h
//
// Code generated for Simulink model 'Derivator'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:09 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef Derivator_h_
#define Derivator_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model Derivator
class Derivator final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'Derivator'
  struct DW_Derivator_T {
    float Previous_DSTATE;             // '<Root>/Previous'
  };

  // Initial conditions function
  void init(float rtp_InitVal);

  // Copy Constructor
  Derivator(Derivator const&) = delete;

  // Assignment Operator
  Derivator& operator= (Derivator const&) & = delete;

  // Move Constructor
  Derivator(Derivator &&) = delete;

  // Move Assignment Operator
  Derivator& operator= (Derivator &&) = delete;

  // Reset function
  void reset(float rtp_InitVal);

  // model step function
  void step(const float *rtu_In, float *rty_Out, float rtp_TimeStep);

  // Constructor
  Derivator();

  // Destructor
  ~Derivator();

  // private data and function members
 private:
  // Block states
  DW_Derivator_T Derivator_DW;
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
//  '<Root>' : 'Derivator'

#endif                                 // Derivator_h_

//
// File trailer for generated code.
//
// [EOF]
//
