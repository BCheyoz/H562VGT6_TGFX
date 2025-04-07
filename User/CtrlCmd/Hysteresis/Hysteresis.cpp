//
// File: Hysteresis.cpp
//
// Code generated for Simulink model 'Hysteresis'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:06 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "Hysteresis.h"
#include <cmath>
#include <stdbool.h>

// System initialize for referenced model: 'Hysteresis'
void Hysteresis::init(float rtp_offVal)
{
  Hysteresis_DW.UnitDelay_DSTATE = rtp_offVal;
}

// System reset for referenced model: 'Hysteresis'
void Hysteresis::reset(float rtp_offVal)
{
  Hysteresis_DW.UnitDelay_DSTATE = rtp_offVal;
}

// Output and update for referenced model: 'Hysteresis'
void Hysteresis::step(const float *rtu_In, float *rty_Out, float rtp_offVal,
                      float rtp_onVal, float rtp_swOffVal, float rtp_swOnVal)
{
  float rtb_noLowVal;
  bool rtb_isDirect;
  bool rtb_isLowIn;
  rtb_isDirect = (rtp_swOnVal < rtp_swOffVal);
  rtb_noLowVal = std::fmin(rtp_swOnVal, rtp_swOffVal);
  rtb_isLowIn = (*rtu_In <= rtb_noLowVal);
  if (rtb_isLowIn) {
    if (rtb_isDirect) {
      rtb_noLowVal = rtp_onVal;
    } else {
      rtb_noLowVal = rtp_offVal;
    }

    *rty_Out = rtb_noLowVal;
  } else {
    rtb_noLowVal = std::fmax(rtp_swOnVal, rtp_swOffVal);
    rtb_isLowIn = (*rtu_In >= rtb_noLowVal);
    if (rtb_isLowIn) {
      if (rtb_isDirect) {
        rtb_noLowVal = rtp_offVal;
      } else {
        rtb_noLowVal = rtp_onVal;
      }
    } else {
      rtb_noLowVal = Hysteresis_DW.UnitDelay_DSTATE;
    }

    *rty_Out = rtb_noLowVal;
  }

  Hysteresis_DW.UnitDelay_DSTATE = *rty_Out;
}

// Constructor
Hysteresis::Hysteresis():
  Hysteresis_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Hysteresis::~Hysteresis() = default;

//
// File trailer for generated code.
//
// [EOF]
//
