//
// File: look1_iu16lu32n16_binlcse.cpp
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.136
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:10:43 2025
//
#include "look1_iu16lu32n16_binlcse.h"
#include <stdint.h>

uint16_t look1_iu16lu32n16_binlcse(uint16_t u0, const uint16_t bp0[], const
  uint16_t table[], uint32_t maxIndex)
{
  uint32_t frac;
  uint32_t iLeft;
  uint16_t bpLeftVar;
  uint16_t y;
  uint16_t yL_0d0;

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
    frac = ((static_cast<uint32_t>(u0) - bpLeftVar) << 16) /
      (static_cast<uint32_t>(bp0[iLeft + 1U]) - bpLeftVar);
  } else {
    iLeft = maxIndex - 1U;
    frac = 65536U;
  }

  // Column-major Interpolation 1-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Rounding mode: 'simplest'
  // Overflow mode: 'wrapping'

  bpLeftVar = table[iLeft + 1U];
  yL_0d0 = table[iLeft];
  if (bpLeftVar >= yL_0d0) {
    y = static_cast<uint16_t>(static_cast<uint32_t>(static_cast<uint16_t>((
      static_cast<uint16_t>(static_cast<uint32_t>(bpLeftVar) - yL_0d0) * frac) >>
      16)) + yL_0d0);
  } else {
    y = static_cast<uint16_t>(static_cast<uint32_t>(yL_0d0) -
      static_cast<uint16_t>((static_cast<uint16_t>(static_cast<uint32_t>(yL_0d0)
      - bpLeftVar) * frac) >> 16));
  }

  return y;
}

//
// File trailer for generated code.
//
// [EOF]
//
