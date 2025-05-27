//
// File: mul_u32_loSR.cpp
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.329
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon May 12 09:59:06 2025
//
#include "mul_u32_loSR.h"
#include "mul_wide_u32.h"
#include <stdint.h>

uint32_t mul_u32_loSR(uint32_t a, uint32_t b, uint32_t aShift)
{
  uint32_t result;
  uint32_t u32_chi;
  mul_wide_u32(a, b, &u32_chi, &result);
  return u32_chi << /*MW:OvBitwiseOk*/ (32U - aShift) | result >> aShift;
}

//
// File trailer for generated code.
//
// [EOF]
//
