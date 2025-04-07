//
// File: FallingEdgeDetect.cpp
//
// Code generated for Simulink model 'FallingEdgeDetect'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:09:16 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "FallingEdgeDetect.h"
#include <stdint.h>
#include <stdbool.h>

// System reset for referenced model: 'FallingEdgeDetect'
void FallingEdgeDetect::reset(void)
{
  FallingEdgeDetect_DW.preivousVal_DSTATE = 0U;
}

// Output and update for referenced model: 'FallingEdgeDetect'
void FallingEdgeDetect::step(const uint8_t *rtu_In, bool *rty_Out)
{
  *rty_Out = (*rtu_In < FallingEdgeDetect_DW.preivousVal_DSTATE);
  FallingEdgeDetect_DW.preivousVal_DSTATE = *rtu_In;
}

// Constructor
FallingEdgeDetect::FallingEdgeDetect():
  FallingEdgeDetect_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
FallingEdgeDetect::~FallingEdgeDetect() = default;

//
// File trailer for generated code.
//
// [EOF]
//
