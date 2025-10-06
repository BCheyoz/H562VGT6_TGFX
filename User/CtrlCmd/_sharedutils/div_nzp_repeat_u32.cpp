//
// File: div_nzp_repeat_u32.cpp
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.335
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:05:30 2025
//
#include "div_nzp_repeat_u32.h"
#include <stdint.h>
#include <stdbool.h>

uint32_t div_nzp_repeat_u32(uint32_t numerator, uint32_t denominator, uint32_t nRepeatSub)
{
  uint32_t localNumerator;
  uint32_t quotient;
  quotient = numerator / denominator;
  localNumerator = numerator % denominator;
  for (uint32_t iRepeatSub{0U}; iRepeatSub < nRepeatSub; iRepeatSub++) {
    bool numeratorExtraBit;
    numeratorExtraBit = (localNumerator >= 2147483648U);
    localNumerator <<= 1U;
    quotient <<= 1U;
    if (numeratorExtraBit || (localNumerator >= denominator)) {
      quotient++;
      localNumerator -= denominator;
    }
  }

  return quotient;
}

//
// File trailer for generated code.
//
// [EOF]
//
