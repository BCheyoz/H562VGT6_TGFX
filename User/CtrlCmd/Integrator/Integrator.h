//
// File: Integrator.h
//
// Code generated for Simulink model 'Integrator'.
//
// Model version                  : 5.1
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:13 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef Integrator_h_
#define Integrator_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model Integrator
class Integrator final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'Integrator'
  struct DW_Integrator_T {
    float recIntegral;                 // '<Root>/totIntegral'
  };

  // Initial conditions function
  void init(float rtp_initVal);

  // Copy Constructor
  Integrator(Integrator const&) = delete;

  // Assignment Operator
  Integrator& operator= (Integrator const&) & = delete;

  // Move Constructor
  Integrator(Integrator &&) = delete;

  // Move Assignment Operator
  Integrator& operator= (Integrator &&) = delete;

  // Reset function
  void reset(float rtp_initVal);

  // model step function
  void step(const float *rtu_In, const bool *rtu_rst, float *rty_out, float rtp_Ts, float rtp_initVal);

  // Constructor
  Integrator();

  // Destructor
  ~Integrator();

  // private data and function members
 private:
  // Block states
  DW_Integrator_T Integrator_DW;
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
//  '<Root>' : 'Integrator'

#endif                                 // Integrator_h_

//
// File trailer for generated code.
//
// [EOF]
//
