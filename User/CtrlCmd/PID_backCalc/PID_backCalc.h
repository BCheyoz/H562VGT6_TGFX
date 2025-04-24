//
// File: PID_backCalc.h
//
// Code generated for Simulink model 'PID_backCalc'.
//
// Model version                  : 5.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:41 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef PID_backCalc_h_
#define PID_backCalc_h_
#include <stdbool.h>
#include <stdint.h>
#include "Derivator.h"
#include "Integrator.h"
#include "cntrlSaturation.h"
#include "firstOrderTF.h"
#include <cstring>

// Class declaration for model PID_backCalc
class PID_backCalc final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'PID_backCalc'
  struct DW_PID_backCalc_T {
    float ovrFlw_DSTATE;               // '<Root>/ovrFlw'
  };

  // Initial conditions function
  void init(float rtp_InitVal);

  // Copy Constructor
  PID_backCalc(PID_backCalc const&) = delete;

  // Assignment Operator
  PID_backCalc& operator= (PID_backCalc const&) & = delete;

  // Move Constructor
  PID_backCalc(PID_backCalc &&) = delete;

  // Move Assignment Operator
  PID_backCalc& operator= (PID_backCalc &&) = delete;

  // Reset function
  void reset(float rtp_InitVal);

  // model step function
  void step(const float *rtu_setPoint, const float *rtu_measure, const float *rtu_cmd_max, const float *rtu_cmd_min, const bool *rtu_rst, float *rty_cmd, float *
            rty_ovrFlwVal, float rtp_InitVal, float rtp_Kawu, float rtp_Kd, float rtp_Ki, float rtp_Kp, float rtp_SampleTime, float rtp_Tau_f);

  // Constructor
  PID_backCalc();

  // Destructor
  ~PID_backCalc();

  // private data and function members
 private:
  // Block states
  DW_PID_backCalc_T PID_backCalc_DW;

  // model instance variable for '<Root>/DerivativeFilter'
  firstOrderTF DerivativeFilterMDLOBJ1;

  // model instance variable for '<Root>/Derivator'
  Derivator DerivatorMDLOBJ2;

  // model instance variable for '<Root>/Integrator'
  Integrator IntegratorMDLOBJ3;

  // model instance variable for '<Root>/cmdLimits'
  cntrlSaturation cmdLimitsMDLOBJ4;
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
//  '<Root>' : 'PID_backCalc'

#endif                                 // PID_backCalc_h_

//
// File trailer for generated code.
//
// [EOF]
//
