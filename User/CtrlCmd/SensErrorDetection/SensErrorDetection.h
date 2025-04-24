//
// File: SensErrorDetection.h
//
// Code generated for Simulink model 'SensErrorDetection'.
//
// Model version                  : 1.30
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:56 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef SensErrorDetection_h_
#define SensErrorDetection_h_
#include <stdbool.h>
#include <stdint.h>
#include "OpScaleDetection.h"
#include "StaticErrorDetection.h"
#include "VariationErrorDetection.h"
#include <cstring>

// Class declaration for model SensErrorDetection
class SensErrorDetection final
{
  // public data and function members
 public:
  // Copy Constructor
  SensErrorDetection(SensErrorDetection const&) = delete;

  // Assignment Operator
  SensErrorDetection& operator= (SensErrorDetection const&) & = delete;

  // Move Constructor
  SensErrorDetection(SensErrorDetection &&) = delete;

  // Move Assignment Operator
  SensErrorDetection& operator= (SensErrorDetection &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const float *rtu_signalRaw, float *rty_signal, bool *rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_op_scl_err_rst_time, float
            rtp_Cs_op_scl_err_set_time, float rtp_Cs_smpl_time, float rtp_Cs_sttc_err_rst_time, float rtp_Cs_sttc_err_rst_tol, float rtp_Cs_sttc_err_set_time,
            float rtp_Cs_val_max, float rtp_Cs_val_min, float rtp_Cs_var_err_rst_time, float rtp_Cs_var_err_set_time, float rtp_Cs_var_max);

  // Constructor
  SensErrorDetection();

  // Destructor
  ~SensErrorDetection();

  // private data and function members
 private:
  // model instance variable for '<Root>/OpScaleDetection'
  OpScaleDetection OpScaleDetectionMDLOBJ1;

  // model instance variable for '<Root>/StaticErrorDetection'
  StaticErrorDetection StaticErrorDetectionMDLOBJ2;

  // model instance variable for '<Root>/VariationErrorDetection'
  VariationErrorDetection VariationErrorDetectMDLOBJ3;
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
//  '<Root>' : 'SensErrorDetection'

#endif                                 // SensErrorDetection_h_

//
// File trailer for generated code.
//
// [EOF]
//
