//
// File: plook_u32s16_bincka.cpp
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.570
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 15:39:34 2025
//
#include "plook_u32s16_bincka.h"
#include "binsearch_u32s16.h"
#include <stdint.h>

uint32_t plook_u32s16_bincka(int16_t u, const int16_t bp[], uint32_t maxIndex)
{
  uint32_t bpIndex;

  // Prelookup - Index only
  // Index Search method: 'binary'
  // Extrapolation method: 'Clip'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'on'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u <= bp[0U]) {
    bpIndex = 0U;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32s16(u, bp, maxIndex >> 1U, maxIndex);
  } else {
    bpIndex = maxIndex;
  }

  return bpIndex;
}

//
// File trailer for generated code.
//
// [EOF]
//
