//
// File: OutPutMng.h
//
// Code generated for Simulink model 'OutPutMng'.
//
// Model version                  : 1.21
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:23:53 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. ROM efficiency
//    3. RAM efficiency
//    4. Execution efficiency
// Validation result: Not run
//
#ifndef OutPutMng_h_
#define OutPutMng_h_
#include <stdbool.h>
#include <stdint.h>
#include "OutPutMng_types.h"
#include <cstring>

// Class declaration for model OutPutMng
class OutPutMng final
{
  // public data and function members
 public:
  // Copy Constructor
  OutPutMng(OutPutMng const&) = delete;

  // Assignment Operator
  OutPutMng& operator= (OutPutMng const&) & = delete;

  // Move Constructor
  OutPutMng(OutPutMng &&) = delete;

  // Move Assignment Operator
  OutPutMng& operator= (OutPutMng &&) = delete;

  // model step function
  void step(const tb_OutPutMng_In *rtu_OutPutMng_In, tb_Control_Out
            *rty_Control_Out);

  // Constructor
  OutPutMng();

  // Destructor
  ~OutPutMng();
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'OutPutMng'

#endif                                 // OutPutMng_h_

//
// File trailer for generated code.
//
// [EOF]
//
