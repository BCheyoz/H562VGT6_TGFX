//
// File: Derivator.cpp
//
// Code generated for Simulink model 'Derivator'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:09 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "Derivator.h"

// System initialize for referenced model: 'Derivator'
void Derivator::init(float rtp_InitVal)
{
  Derivator_DW.Previous_DSTATE = rtp_InitVal;
}

// System reset for referenced model: 'Derivator'
void Derivator::reset(float rtp_InitVal)
{
  Derivator_DW.Previous_DSTATE = rtp_InitVal;
}

// Output and update for referenced model: 'Derivator'
void Derivator::step(const float *rtu_In, float *rty_Out, float rtp_TimeStep)
{
  float rtb_Diff;
  rtb_Diff = *rtu_In - Derivator_DW.Previous_DSTATE;
  *rty_Out = rtb_Diff / rtp_TimeStep;
  Derivator_DW.Previous_DSTATE = *rtu_In;
}

// Constructor
Derivator::Derivator():
  Derivator_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Derivator::~Derivator() = default;

//
// File trailer for generated code.
//
// [EOF]
//
