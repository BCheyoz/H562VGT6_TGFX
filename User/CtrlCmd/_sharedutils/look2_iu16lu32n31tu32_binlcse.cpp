//
// File: look2_iu16lu32n31tu32_binlcse.cpp
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.335
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:05:30 2025
//
#include "look2_iu16lu32n31tu32_binlcse.h"
#include "div_nzp_repeat_u32.h"
#include "mul_u32_loSR.h"
#include <stdint.h>

uint32_t look2_iu16lu32n31tu32_binlcse(uint16_t u0, uint16_t u1, const uint16_t bp0[], const uint16_t bp1[], const uint32_t table[], const uint32_t maxIndex[],
  uint32_t stride)
{
  uint32_t bpIndices[2];
  uint32_t fractions[2];
  uint32_t bpIdx;
  uint32_t frac;
  uint32_t iLeft;
  uint32_t iRght;
  uint32_t y;
  uint32_t yL_1d;
  uint16_t bpLeftVar;

  // Column-major Lookup 2-D
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
  } else if (u0 < bp0[maxIndex[0U]]) {
    // Binary Search
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    bpLeftVar = bp0[iLeft];
    frac = div_nzp_repeat_u32((static_cast<uint32_t>(u0) - bpLeftVar) << 16, static_cast<uint32_t>(bp0[iLeft + 1U]) - bpLeftVar, 15U);
  } else {
    iLeft = maxIndex[0U] - 1U;
    frac = 2147483648U;
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Extrapolation method: 'Clip'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'
  // Rounding mode: 'simplest'

  if (u1 <= bp1[0U]) {
    iLeft = 0U;
    frac = 0U;
  } else if (u1 < bp1[maxIndex[1U]]) {
    // Binary Search
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    bpLeftVar = bp1[iLeft];
    frac = div_nzp_repeat_u32((static_cast<uint32_t>(u1) - bpLeftVar) << 16, static_cast<uint32_t>(bp1[iLeft + 1U]) - bpLeftVar, 15U);
  } else {
    iLeft = maxIndex[1U] - 1U;
    frac = 2147483648U;
  }

  // Column-major Interpolation 2-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Rounding mode: 'simplest'
  // Overflow mode: 'wrapping'

  iLeft = iLeft * stride + bpIndices[0U];
  bpIdx = table[iLeft + 1U];
  iRght = table[iLeft];
  if (bpIdx >= iRght) {
    yL_1d = mul_u32_loSR(fractions[0U], bpIdx - iRght, 31U) + iRght;
  } else {
    yL_1d = iRght - mul_u32_loSR(fractions[0U], iRght - bpIdx, 31U);
  }

  bpIdx = iLeft + stride;
  iRght = table[bpIdx + 1U];
  bpIdx = table[bpIdx];
  if (iRght >= bpIdx) {
    bpIdx += mul_u32_loSR(fractions[0U], iRght - bpIdx, 31U);
  } else {
    bpIdx -= mul_u32_loSR(fractions[0U], bpIdx - iRght, 31U);
  }

  if (bpIdx >= yL_1d) {
    y = mul_u32_loSR(frac, bpIdx - yL_1d, 31U) + yL_1d;
  } else {
    y = yL_1d - mul_u32_loSR(frac, yL_1d - bpIdx, 31U);
  }

  return y;
}

//
// File trailer for generated code.
//
// [EOF]
//
