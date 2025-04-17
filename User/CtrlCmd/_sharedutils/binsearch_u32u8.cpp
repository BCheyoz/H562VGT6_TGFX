//
// File: binsearch_u32u8.cpp
//
// Code generated for Simulink model 'WaterHeatSpCalc'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 15:38:34 2025
//
#include "binsearch_u32u8.h"
#include <stdint.h>

uint32_t binsearch_u32u8(uint8_t u, const uint8_t bp[], uint32_t startIndex,
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
