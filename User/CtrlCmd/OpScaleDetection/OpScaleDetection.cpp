//
// File: OpScaleDetection.cpp
//
// Code generated for Simulink model 'OpScaleDetection'.
//
// Model version                  : 1.32
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:59 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "OpScaleDetection.h"
#include <stdbool.h>
#include <stdint.h>
#include "RisingEdgeDelay.h"
#include "R_S_FlipFlop.h"

// System reset for referenced model: 'OpScaleDetection'
void OpScaleDetection::reset(void)
{
  OpScaleDetection_DW.UnitDelay_DSTATE = 0.0F;
  risingEdgeSetMDLOBJ3.reset();
  risingEdgeResetMDLOBJ2.reset();
  isDefectRSMDLOBJ1.reset();
}

// Output and update for referenced model: 'OpScaleDetection'
void OpScaleDetection::step(const float *rtu_signalRaw, float *rty_Signal, bool *
  rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_rst_time, float
  rtp_Cs_set_time, float rtp_Cs_smpl_time, float rtp_Cs_val_max, float
  rtp_Cs_val_min)
{
  // local block i/o variables
  uint8_t rtb_setUint8;
  uint8_t rtb_rstUint8;
  float rtb_signalSat;
  uint8_t rtb_insErrUint8;
  bool rtb_isTooHigh;
  bool rtb_isTooLow;
  bool rtb_rst;
  rtb_isTooHigh = (rtp_Cs_val_max < *rtu_signalRaw);
  rtb_isTooLow = (*rtu_signalRaw < rtp_Cs_val_min);
  rtb_isTooHigh = (rtb_isTooHigh || rtb_isTooLow);
  rtb_insErrUint8 = rtb_isTooHigh;
  risingEdgeSetMDLOBJ3.step(&rtb_insErrUint8, &rtb_setUint8, rtp_Cs_set_time,
    rtp_Cs_smpl_time);
  rtb_isTooLow = (rtb_setUint8 != 0);
  rtb_insErrUint8 = static_cast<uint8_t>(!rtb_isTooHigh);
  risingEdgeResetMDLOBJ2.step(&rtb_insErrUint8, &rtb_rstUint8, rtp_Cs_rst_time,
    rtp_Cs_smpl_time);
  rtb_rst = (rtb_rstUint8 != 0);
  isDefectRSMDLOBJ1.step(&rtb_isTooLow, &rtb_rst, rty_isDefect);
  if (*rty_isDefect) {
    *rty_Signal = rtp_Cs_dflt_val;
  } else {
    if (rtb_isTooHigh) {
      rtb_signalSat = OpScaleDetection_DW.UnitDelay_DSTATE;
    } else {
      rtb_signalSat = *rtu_signalRaw;
    }

    *rty_Signal = rtb_signalSat;
  }

  OpScaleDetection_DW.UnitDelay_DSTATE = *rty_Signal;
}

// Constructor
OpScaleDetection::OpScaleDetection():
  OpScaleDetection_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
OpScaleDetection::~OpScaleDetection() = default;

//
// File trailer for generated code.
//
// [EOF]
//
