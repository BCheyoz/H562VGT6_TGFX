//
// File: cntrlSaturation.h
//
// Code generated for Simulink model 'cntrlSaturation'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:01 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef cntrlSaturation_h_
#define cntrlSaturation_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model cntrlSaturation
class cntrlSaturation final
{
  // public data and function members
 public:
  // Copy Constructor
  cntrlSaturation(cntrlSaturation const&) = delete;

  // Assignment Operator
  cntrlSaturation& operator= (cntrlSaturation const&) & = delete;

  // Move Constructor
  cntrlSaturation(cntrlSaturation &&) = delete;

  // Move Assignment Operator
  cntrlSaturation& operator= (cntrlSaturation &&) = delete;

  // model step function
  void step(const float *rtu_maxVal, const float *rtu_minVal, const float
            *rtu_val, float *rty_satVal, float *rty_ovrFlw);

  // Constructor
  cntrlSaturation();

  // Destructor
  ~cntrlSaturation();
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
//  '<Root>' : 'cntrlSaturation'

#endif                                 // cntrlSaturation_h_

//
// File trailer for generated code.
//
// [EOF]
//
