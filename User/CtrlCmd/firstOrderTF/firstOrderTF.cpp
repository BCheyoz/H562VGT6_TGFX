//
// File: firstOrderTF.cpp
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
#include "firstOrderTF.h"

// System initialize for referenced model: 'firstOrderTF'
void firstOrderTF::init(float rtp_initVal)
{
  firstOrderTF_DW.previous_DSTATE = rtp_initVal;
}

// System reset for referenced model: 'firstOrderTF'
void firstOrderTF::reset(float rtp_initVal)
{
  firstOrderTF_DW.previous_DSTATE = rtp_initVal;
}

// Output and update for referenced model: 'firstOrderTF'
void firstOrderTF::step(const float *rtu_In, float *rty_Out, float rtp_K, float
  rtp_Tau, float rtp_Ts)
{
  float rtb_Ts_Tau;
  float rtb_TspTau;
  rtb_TspTau = rtp_Ts + rtp_Tau;
  rtb_Ts_Tau = rtp_Tau / rtb_TspTau;
  rtb_TspTau = *rtu_In * rtp_K * rtp_Ts / rtb_TspTau;
  *rty_Out = rtb_Ts_Tau * firstOrderTF_DW.previous_DSTATE + rtb_TspTau;
  firstOrderTF_DW.previous_DSTATE = *rty_Out;
}

// Constructor
firstOrderTF::firstOrderTF():
  firstOrderTF_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
firstOrderTF::~firstOrderTF() = default;

//
// File trailer for generated code.
//
// [EOF]
//
