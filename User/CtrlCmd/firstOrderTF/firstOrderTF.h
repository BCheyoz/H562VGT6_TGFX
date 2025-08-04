//
// File: firstOrderTF.h
//
// Code generated for Simulink model 'firstOrderTF'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:02:49 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef firstOrderTF_h_
#define firstOrderTF_h_
#include <stdbool.h>
#include <stdint.h>
#include <cstring>

// Class declaration for model firstOrderTF
class firstOrderTF final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'firstOrderTF'
  struct DW_firstOrderTF_T {
    float previous_DSTATE;             // '<Root>/previous'
  };

  // Initial conditions function
  void init(float rtp_initVal);

  // Copy Constructor
  firstOrderTF(firstOrderTF const&) = delete;

  // Assignment Operator
  firstOrderTF& operator= (firstOrderTF const&) & = delete;

  // Move Constructor
  firstOrderTF(firstOrderTF &&) = delete;

  // Move Assignment Operator
  firstOrderTF& operator= (firstOrderTF &&) = delete;

  // Reset function
  void reset(float rtp_initVal);

  // model step function
  void step(const float *rtu_In, float *rty_Out, float rtp_K, float rtp_Tau,
            float rtp_Ts);

  // Constructor
  firstOrderTF();

  // Destructor
  ~firstOrderTF();

  // private data and function members
 private:
  // Block states
  DW_firstOrderTF_T firstOrderTF_DW;
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
//  '<Root>' : 'firstOrderTF'

#endif                                 // firstOrderTF_h_

//
// File trailer for generated code.
//
// [EOF]
//
