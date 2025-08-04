//
// File: Hysteresis.h
//
// Code generated for Simulink model 'Hysteresis'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:03:18 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef Hysteresis_h_
#define Hysteresis_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model Hysteresis
class Hysteresis final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'Hysteresis'
  struct DW_Hysteresis_T {
    float UnitDelay_DSTATE;            // '<Root>/Unit Delay'
  };

  // Initial conditions function
  void init(float rtp_offVal);

  // Copy Constructor
  Hysteresis(Hysteresis const&) = delete;

  // Assignment Operator
  Hysteresis& operator= (Hysteresis const&) & = delete;

  // Move Constructor
  Hysteresis(Hysteresis &&) = delete;

  // Move Assignment Operator
  Hysteresis& operator= (Hysteresis &&) = delete;

  // Reset function
  void reset(float rtp_offVal);

  // model step function
  void step(const float *rtu_In, float *rty_Out, float rtp_offVal, float
            rtp_onVal, float rtp_swOffVal, float rtp_swOnVal);

  // Constructor
  Hysteresis();

  // Destructor
  ~Hysteresis();

  // private data and function members
 private:
  // Block states
  DW_Hysteresis_T Hysteresis_DW;
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
//  '<Root>' : 'Hysteresis'

#endif                                 // Hysteresis_h_

//
// File trailer for generated code.
//
// [EOF]
//
