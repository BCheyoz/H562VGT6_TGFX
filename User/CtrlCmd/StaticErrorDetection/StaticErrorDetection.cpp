//
// File: StaticErrorDetection.cpp
//
// Code generated for Simulink model 'StaticErrorDetection'.
//
// Model version                  : 1.49
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:04:24 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "StaticErrorDetection.h"
#include <stdbool.h>
#include <stdint.h>
#include "Timer.h"
#include "RisingEdgeDelay.h"
#include "R_S_FlipFlop.h"

// System reset for referenced model: 'StaticErrorDetection'
void StaticErrorDetection::reset(void)
{
  StaticErrorDetection_DW.prevSignalRaw_DSTATE = 0.0F;
  StaticErrorDetection_DW.UnitDelay3_DSTATE = 0U;
  StaticErrorDetection_DW.UnitDelay1_DSTATE = false;
  NumberNotEqualMDLOBJ3.reset();
  EdgeDelaySetMDLOBJ2.reset();
  EdgeDelayRstTimeMDLOBJ1.reset();
  StaticErrorRSMDLOBJ4.reset();
}

// Output and update for referenced model: 'StaticErrorDetection'
void StaticErrorDetection::step(const float *rtu_signalRaw, float *rty_signal,
  bool *rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_rst_dlay_time, float
  rtp_Cs_rst_tol, float rtp_Cs_set_dlay_time, float rtp_Cs_smpl_time)
{
  // local block i/o variables
  float rtb_numberNotEqual;
  uint8_t rtb_setUint8;
  uint8_t rtb_rstTimeUint8;
  bool rtb_TimerState;
  float rtb_calcTol;
  uint8_t rtb_isEqualUint8;
  uint8_t rtb_isEqualUint8_b_tmp;
  uint8_t rtb_isEqualUint8_c;
  bool rtb_isEqual;
  bool rtb_isTolOk;
  rtb_isEqual = (*rtu_signalRaw == StaticErrorDetection_DW.prevSignalRaw_DSTATE);
  rtb_isEqualUint8 = ((static_cast<bool>
                       ((StaticErrorDetection_DW.UnitDelay3_DSTATE != 0) ^
                        !rtb_isEqual)) &&
                      StaticErrorDetection_DW.UnitDelay1_DSTATE);
  rtb_isEqualUint8_c = ((!StaticErrorDetection_DW.UnitDelay1_DSTATE) ||
                        (StaticErrorDetection_DW.UnitDelay3_DSTATE != 0));
  rtb_isEqualUint8_b_tmp = rtb_isEqual;
  NumberNotEqualMDLOBJ3.step(&rtb_isEqualUint8, &rtb_isEqualUint8_c,
    &rtb_isEqualUint8_b_tmp, &rtb_numberNotEqual, &rtb_TimerState,
    rtp_Cs_smpl_time);
  rtb_calcTol = rtb_numberNotEqual * 100.0F / rtp_Cs_rst_dlay_time;
  rtb_isTolOk = (rtb_calcTol >= rtp_Cs_rst_tol);
  rtb_isEqualUint8 = rtb_TimerState;
  EdgeDelaySetMDLOBJ2.step(&rtb_isEqualUint8_b_tmp, &rtb_setUint8,
    rtp_Cs_set_dlay_time, rtp_Cs_smpl_time);
  EdgeDelayRstTimeMDLOBJ1.step(&rtb_isEqualUint8, &rtb_rstTimeUint8,
    rtp_Cs_rst_dlay_time, rtp_Cs_smpl_time);
  rtb_isEqual = (rtb_isTolOk && (rtb_rstTimeUint8 != 0));
  rtb_isTolOk = (rtb_setUint8 != 0);
  StaticErrorRSMDLOBJ4.step(&rtb_isTolOk, &rtb_isEqual, rty_isDefect);
  if (*rty_isDefect) {
    *rty_signal = rtp_Cs_dflt_val;
  } else {
    *rty_signal = *rtu_signalRaw;
  }

  StaticErrorDetection_DW.prevSignalRaw_DSTATE = *rtu_signalRaw;
  StaticErrorDetection_DW.UnitDelay3_DSTATE = rtb_rstTimeUint8;
  StaticErrorDetection_DW.UnitDelay1_DSTATE = *rty_isDefect;
}

// Constructor
StaticErrorDetection::StaticErrorDetection():
  StaticErrorDetection_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
StaticErrorDetection::~StaticErrorDetection() = default;

//
// File trailer for generated code.
//
// [EOF]
//
