//
// File: cntrlSaturation.cpp
//
// Code generated for Simulink model 'cntrlSaturation'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:18 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "cntrlSaturation.h"
#include <cmath>

// Output and update for referenced model: 'cntrlSaturation'
void cntrlSaturation::step(const float *rtu_maxVal, const float *rtu_minVal, const float *rtu_val, float *rty_satVal, float *rty_ovrFlw)
{
  float maxV;
  maxV = std::fmax(*rtu_val, *rtu_minVal);
  maxV = std::fmin(*rtu_maxVal, maxV);
  *rty_satVal = maxV;
  *rty_ovrFlw = *rtu_val - *rty_satVal;
}

// Constructor
cntrlSaturation::cntrlSaturation()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
cntrlSaturation::~cntrlSaturation() = default;

//
// File trailer for generated code.
//
// [EOF]
//
