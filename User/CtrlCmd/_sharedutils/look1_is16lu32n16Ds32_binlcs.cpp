//
// File: look1_is16lu32n16Ds32_binlcs.cpp
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.569
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:11:22 2025
//
#include "look1_is16lu32n16Ds32_binlcs.h"
#include <stdint.h>

int16_t look1_is16lu32n16Ds32_binlcs(int16_t u0, const int16_t bp0[], const
  int16_t table[], uint32_t maxIndex)
{
  uint32_t frac;
  uint32_t iLeft;
  int16_t bpLeftVar;

  // Column-major Lookup 1-D
  // Search method: 'binary'
  // Use previous index: 'off'
  // Interpolation method: 'Linear point-slope'
  // Extrapolation method: 'Clip'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'
  // Rounding mode: 'simplest'

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Extrapolation method: 'Clip'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'
  // Rounding mode: 'simplest'

  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = 0U;
  } else if (u0 < bp0[maxIndex]) {
    uint32_t iRght;

    // Binary Search
    frac = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[frac]) {
        iRght = frac;
      } else {
        iLeft = frac;
      }

      frac = (iRght + iLeft) >> 1U;
    }

    bpLeftVar = bp0[iLeft];
    frac = (static_cast<uint32_t>(u0 - bpLeftVar) << 16) / static_cast<uint32_t>
      (bp0[iLeft + 1U] - bpLeftVar);
  } else {
    iLeft = maxIndex - 1U;
    frac = 65536U;
  }

  // Column-major Interpolation 1-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Rounding mode: 'simplest'
  // Overflow mode: 'wrapping'

  bpLeftVar = table[iLeft];
  return static_cast<int16_t>(static_cast<int16_t>(((table[iLeft + 1U] -
    bpLeftVar) * static_cast<int32_t>(frac)) >> 16) + bpLeftVar);
}

//
// File trailer for generated code.
//
// [EOF]
//
