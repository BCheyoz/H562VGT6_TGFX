//
// File: RisingEdgeDetect.cpp
//
// Code generated for Simulink model 'RisingEdgeDetect'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:50:45 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "RisingEdgeDetect.h"
#include <stdint.h>
#include <stdbool.h>

// System reset for referenced model: 'RisingEdgeDetect'
void RisingEdgeDetect::reset(void)
{
  RisingEdgeDetect_DW.preivousVal_DSTATE = 0U;
}

// Output and update for referenced model: 'RisingEdgeDetect'
void RisingEdgeDetect::step(const uint8_t *rtu_In, bool *rty_Out)
{
  *rty_Out = (*rtu_In > RisingEdgeDetect_DW.preivousVal_DSTATE);
  RisingEdgeDetect_DW.preivousVal_DSTATE = *rtu_In;
}

// Constructor
RisingEdgeDetect::RisingEdgeDetect():
  RisingEdgeDetect_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
RisingEdgeDetect::~RisingEdgeDetect() = default;

//
// File trailer for generated code.
//
// [EOF]
//
