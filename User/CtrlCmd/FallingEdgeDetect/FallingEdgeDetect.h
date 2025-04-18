//
// File: FallingEdgeDetect.h
//
// Code generated for Simulink model 'FallingEdgeDetect'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:50:38 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef FallingEdgeDetect_h_
#define FallingEdgeDetect_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model FallingEdgeDetect
class FallingEdgeDetect final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'FallingEdgeDetect'
  struct DW_FallingEdgeDetect_T {
    uint8_t preivousVal_DSTATE;        // '<Root>/preivousVal'
  };

  // Copy Constructor
  FallingEdgeDetect(FallingEdgeDetect const&) = delete;

  // Assignment Operator
  FallingEdgeDetect& operator= (FallingEdgeDetect const&) & = delete;

  // Move Constructor
  FallingEdgeDetect(FallingEdgeDetect &&) = delete;

  // Move Assignment Operator
  FallingEdgeDetect& operator= (FallingEdgeDetect &&) = delete;

  // model step function
  void step(const uint8_t *rtu_In, bool *rty_Out);

  // Reset function
  void reset();

  // Constructor
  FallingEdgeDetect();

  // Destructor
  ~FallingEdgeDetect();

  // private data and function members
 private:
  // Block states
  DW_FallingEdgeDetect_T FallingEdgeDetect_DW;
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
//  '<Root>' : 'FallingEdgeDetect'

#endif                                 // FallingEdgeDetect_h_

//
// File trailer for generated code.
//
// [EOF]
//
