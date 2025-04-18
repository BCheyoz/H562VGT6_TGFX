//
// File: plook_u32u8_binckan.cpp
//
// Code generated for Simulink model 'WaterHeatSpCalc'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 15:38:34 2025
//
#include "plook_u32u8_binckan.h"
#include "binsearch_u32u8.h"
#include <stdint.h>

uint32_t plook_u32u8_binckan(uint8_t u, const uint8_t bp[], uint32_t maxIndex)
{
  uint32_t bpIndex;

  // Prelookup - Index only
  // Index Search method: 'binary'
  // Interpolation method: 'Use nearest'
  // Extrapolation method: 'Clip'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'on'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u <= bp[0U]) {
    bpIndex = 0U;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32u8(u, bp, maxIndex >> 1U, maxIndex);
    if ((bpIndex < maxIndex) && (static_cast<uint8_t>(static_cast<uint32_t>
          (bp[bpIndex + 1U]) - u) <= static_cast<uint8_t>(static_cast<uint32_t>
          (u) - bp[bpIndex]))) {
      bpIndex++;
    }
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
