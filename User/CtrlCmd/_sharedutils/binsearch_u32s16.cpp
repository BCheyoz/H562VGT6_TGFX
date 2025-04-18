//
// File: binsearch_u32s16.cpp
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.570
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 15:39:34 2025
//
#include "binsearch_u32s16.h"
#include <stdint.h>

uint32_t binsearch_u32s16(int16_t u, const int16_t bp[], uint32_t startIndex,
  uint32_t maxIndex)
{
  uint32_t bpIdx;
  uint32_t bpIndex;
  uint32_t iRght;

  // Binary Search
  bpIdx = startIndex;
  bpIndex = 0U;
  iRght = maxIndex;
  while (iRght - bpIndex > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1U;
  }

  return bpIndex;
}

//
// File trailer for generated code.
//
// [EOF]
//
