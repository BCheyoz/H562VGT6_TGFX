//
// File: SensErrorDetection.cpp
//
// Code generated for Simulink model 'SensErrorDetection'.
//
// Model version                  : 1.30
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:05:15 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "SensErrorDetection.h"
#include <stdbool.h>
#include "OpScaleDetection.h"
#include "VariationErrorDetection.h"
#include "StaticErrorDetection.h"

// System reset for referenced model: 'SensErrorDetection'
void SensErrorDetection::reset(void)
{
  OpScaleDetectionMDLOBJ1.reset();
  VariationErrorDetectMDLOBJ3.reset();
  StaticErrorDetectionMDLOBJ2.reset();
}

// Output and update for referenced model: 'SensErrorDetection'
void SensErrorDetection::step(const float *rtu_signalRaw, float *rty_signal,
  bool *rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_op_scl_err_rst_time,
  float rtp_Cs_op_scl_err_set_time, float rtp_Cs_smpl_time, float
  rtp_Cs_sttc_err_rst_time, float rtp_Cs_sttc_err_rst_tol, float
  rtp_Cs_sttc_err_set_time, float rtp_Cs_val_max, float rtp_Cs_val_min, float
  rtp_Cs_var_err_rst_time, float rtp_Cs_var_err_set_time, float rtp_Cs_var_max)
{
  // local block i/o variables
  float rtb_OpScaleDetection_o1;
  float rtb_signal;
  bool rtb_OpScaleError;
  bool rtb_VariationError;
  bool rtb_StaticError;
  OpScaleDetectionMDLOBJ1.step(rtu_signalRaw, &rtb_OpScaleDetection_o1,
    &rtb_OpScaleError, rtp_Cs_dflt_val, rtp_Cs_op_scl_err_rst_time,
    rtp_Cs_op_scl_err_set_time, rtp_Cs_smpl_time, rtp_Cs_val_max, rtp_Cs_val_min);
  VariationErrorDetectMDLOBJ3.step(&rtb_OpScaleDetection_o1, &rtb_signal,
    &rtb_VariationError, rtp_Cs_var_err_rst_time, rtp_Cs_var_err_set_time,
    rtp_Cs_dflt_val, rtp_Cs_smpl_time, rtp_Cs_var_max);
  StaticErrorDetectionMDLOBJ2.step(&rtb_signal, rty_signal, &rtb_StaticError,
    rtp_Cs_dflt_val, rtp_Cs_sttc_err_rst_time, rtp_Cs_sttc_err_rst_tol,
    rtp_Cs_sttc_err_set_time, rtp_Cs_smpl_time);
  *rty_isDefect = (rtb_StaticError || rtb_VariationError || rtb_OpScaleError);
}

// Constructor
SensErrorDetection::SensErrorDetection()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
SensErrorDetection::~SensErrorDetection() = default;

//
// File trailer for generated code.
//
// [EOF]
//
