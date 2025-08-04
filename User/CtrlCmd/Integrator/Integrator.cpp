//
// File: Integrator.cpp
//
// Code generated for Simulink model 'Integrator'.
//
// Model version                  : 5.1
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:02:25 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "Integrator.h"
#include <stdbool.h>

// System initialize for referenced model: 'Integrator'
void Integrator::init(float rtp_initVal)
{
  Integrator_DW.recIntegral = rtp_initVal;
}

// System reset for referenced model: 'Integrator'
void Integrator::reset(float rtp_initVal)
{
  Integrator_DW.recIntegral = rtp_initVal;
}

// Output and update for referenced model: 'Integrator'
void Integrator::step(const float *rtu_In, const bool *rtu_rst, float *rty_out,
                      float rtp_Ts, float rtp_initVal)
{
  if (*rtu_rst) {
    *rty_out = rtp_initVal;
  } else {
    float rtb_locIntegral;

    //
    //  Block description for '<Root>/Ts':
    //   Sample Time of the integrator

    rtb_locIntegral = rtp_Ts * *rtu_In;
    *rty_out = rtb_locIntegral + Integrator_DW.recIntegral;
  }

  Integrator_DW.recIntegral = *rty_out;
}

// Constructor
Integrator::Integrator():
  Integrator_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
Integrator::~Integrator() = default;

//
// File trailer for generated code.
//
// [EOF]
//
