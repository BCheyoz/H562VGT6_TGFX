//
// File: rt_roundf.cpp
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.136
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 14:54:26 2025
//
#include "rt_roundf.h"
#include <cmath>

float rt_roundf(float u)
{
  float y;
  if (std::abs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = std::floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = 0.0F;
    } else {
      y = std::ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

//
// File trailer for generated code.
//
// [EOF]
//
