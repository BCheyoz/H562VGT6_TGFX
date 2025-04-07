//
// File: PID_backCalc.cpp
//
// Code generated for Simulink model 'PID_backCalc'.
//
// Model version                  : 5.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:26 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "PID_backCalc.h"
#include <stdbool.h>
#include "Derivator.h"
#include "firstOrderTF.h"
#include "Integrator.h"
#include "cntrlSaturation.h"

// System initialize for referenced model: 'PID_backCalc'
void PID_backCalc::init(float rtp_InitVal)
{
  DerivatorMDLOBJ2.init(0.0F);
  DerivativeFilterMDLOBJ1.init(0.0F);
  IntegratorMDLOBJ3.init(rtp_InitVal);
}

// System reset for referenced model: 'PID_backCalc'
void PID_backCalc::reset(float rtp_InitVal)
{
  PID_backCalc_DW.ovrFlw_DSTATE = 0.0F;
  DerivatorMDLOBJ2.reset(0.0F);
  DerivativeFilterMDLOBJ1.reset(0.0F);
  IntegratorMDLOBJ3.reset(rtp_InitVal);
}

// Output and update for referenced model: 'PID_backCalc'
void PID_backCalc::step(const float *rtu_setPoint, const float *rtu_measure,
  const float *rtu_cmd_max, const float *rtu_cmd_min, const bool *rtu_rst, float
  *rty_cmd, float *rty_ovrFlwVal, float rtp_InitVal, float rtp_Kawu, float
  rtp_Kd, float rtp_Ki, float rtp_Kp, float rtp_SampleTime, float rtp_Tau_f)
{
  // local block i/o variables
  float rtb_Differential_cmd;
  float rtb_Differential_cmd_filt;
  float rtb_Integral_cmd;
  float rtb_Differential_error;
  float rtb_antiWindUpVal;
  float rtb_error;
  rtb_error = *rtu_setPoint - *rtu_measure;
  rtb_Differential_error = rtp_Kd * rtb_error;
  DerivatorMDLOBJ2.step(&rtb_Differential_error, &rtb_Differential_cmd,
                        rtp_SampleTime);
  DerivativeFilterMDLOBJ1.step(&rtb_Differential_cmd, &rtb_Differential_cmd_filt,
    1.0F, rtp_Tau_f, rtp_SampleTime);
  rtb_Differential_error = rtp_Ki * rtb_error;
  rtb_antiWindUpVal = rtp_Kawu * PID_backCalc_DW.ovrFlw_DSTATE;
  rtb_Differential_error -= rtb_antiWindUpVal;
  IntegratorMDLOBJ3.step(&rtb_Differential_error, rtu_rst, &rtb_Integral_cmd,
    rtp_SampleTime, rtp_InitVal);
  rtb_error *= rtp_Kp;
  rtb_error = (rtb_Differential_cmd_filt + rtb_error) + rtb_Integral_cmd;
  cmdLimitsMDLOBJ4.step(rtu_cmd_max, rtu_cmd_min, &rtb_error, rty_cmd,
                        rty_ovrFlwVal);
  PID_backCalc_DW.ovrFlw_DSTATE = *rty_ovrFlwVal;
}

// Constructor
PID_backCalc::PID_backCalc():
  PID_backCalc_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
PID_backCalc::~PID_backCalc() = default;

//
// File trailer for generated code.
//
// [EOF]
//
