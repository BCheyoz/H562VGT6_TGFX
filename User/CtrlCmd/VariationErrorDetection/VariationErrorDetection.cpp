//
// File: VariationErrorDetection.cpp
//
// Code generated for Simulink model 'VariationErrorDetection'.
//
// Model version                  : 1.28
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:04:33 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "VariationErrorDetection.h"
#include <stdbool.h>
#include <cmath>
#include <stdint.h>
#include "RisingEdgeDelay.h"
#include "R_S_FlipFlop.h"

// System reset for referenced model: 'VariationErrorDetection'
void VariationErrorDetection::reset(void)
{
  VariationErrorDetection_DW.prevSignalMesRaw_DSTATE = 0.0F;
  edgeDelaySetMDLOBJ3.reset();
  edgeDelayResetMDLOBJ2.reset();
  DefectSwitchMDLOBJ1.reset();
}

// Output and update for referenced model: 'VariationErrorDetection'
void VariationErrorDetection::step(const float *rtu_signalRaw, float *rty_signal,
  bool *rty_isDefect, float rtp_Cs_dfct_rst_dlay_time, float
  rtp_Cs_dfct_set_dlay_time, float rtp_Cs_dflt_val, float rtp_Cs_smpl_time,
  float rtp_Cs_var_max)
{
  // local block i/o variables
  uint8_t rtb_setUint8;
  uint8_t rtb_rstUint8;
  float rtb_step;
  uint8_t rtb_errUint8;
  bool rtb_isErr;
  bool rtb_rst;
  bool rtb_set;
  rtb_step = *rtu_signalRaw - VariationErrorDetection_DW.prevSignalMesRaw_DSTATE;
  rtb_step /= rtp_Cs_smpl_time;
  rtb_isErr = (std::abs(rtb_step) > rtp_Cs_var_max);
  rtb_errUint8 = rtb_isErr;
  edgeDelaySetMDLOBJ3.step(&rtb_errUint8, &rtb_setUint8,
    rtp_Cs_dfct_set_dlay_time, rtp_Cs_smpl_time);
  rtb_set = (rtb_setUint8 != 0);
  rtb_errUint8 = static_cast<uint8_t>(!rtb_isErr);
  edgeDelayResetMDLOBJ2.step(&rtb_errUint8, &rtb_rstUint8,
    rtp_Cs_dfct_rst_dlay_time, rtp_Cs_smpl_time);
  rtb_rst = (rtb_rstUint8 != 0);
  DefectSwitchMDLOBJ1.step(&rtb_set, &rtb_rst, rty_isDefect);
  if (*rty_isDefect) {
    *rty_signal = rtp_Cs_dflt_val;
  } else {
    if (rtb_isErr) {
      rtb_step = VariationErrorDetection_DW.prevSignalMesRaw_DSTATE;
    } else {
      rtb_step = *rtu_signalRaw;
    }

    *rty_signal = rtb_step;
  }

  VariationErrorDetection_DW.prevSignalMesRaw_DSTATE = *rtu_signalRaw;
}

// Constructor
VariationErrorDetection::VariationErrorDetection():
  VariationErrorDetection_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
VariationErrorDetection::~VariationErrorDetection() = default;

//
// File trailer for generated code.
//
// [EOF]
//
