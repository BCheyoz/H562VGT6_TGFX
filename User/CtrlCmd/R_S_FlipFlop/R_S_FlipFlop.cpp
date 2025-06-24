//
// File: R_S_FlipFlop.cpp
//
// Code generated for Simulink model 'R_S_FlipFlop'.
//
// Model version                  : 5.0
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:50:27 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#include "R_S_FlipFlop.h"
#include <stdbool.h>

// System reset for referenced model: 'R_S_FlipFlop'
void R_S_FlipFlop::reset(void)
{
  R_S_FlipFlop_DW.prevVal_DSTATE = false;
}

// Output and update for referenced model: 'R_S_FlipFlop'
void R_S_FlipFlop::step(const bool *rtu_set, const bool *rtu_reset, bool *rty_stt)
{
  bool rtb_no_S;
  rtb_no_S = ((!*rtu_set) && (!R_S_FlipFlop_DW.prevVal_DSTATE));
  *rty_stt = ((!rtb_no_S) && (!*rtu_reset));
  R_S_FlipFlop_DW.prevVal_DSTATE = *rty_stt;
}

// Constructor
R_S_FlipFlop::R_S_FlipFlop():
  R_S_FlipFlop_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
R_S_FlipFlop::~R_S_FlipFlop() = default;

//
// File trailer for generated code.
//
// [EOF]
//
