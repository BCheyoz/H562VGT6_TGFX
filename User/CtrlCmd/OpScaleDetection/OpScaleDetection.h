//
// File: OpScaleDetection.h
//
// Code generated for Simulink model 'OpScaleDetection'.
//
// Model version                  : 1.32
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:11 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef OpScaleDetection_h_
#define OpScaleDetection_h_
#include <stdbool.h>
#include <stdint.h>
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include <cstring>

// Class declaration for model OpScaleDetection
class OpScaleDetection final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'OpScaleDetection'
  struct DW_OpScaleDetection_T {
    float UnitDelay_DSTATE;            // '<Root>/Unit Delay'
  };

  // Copy Constructor
  OpScaleDetection(OpScaleDetection const&) = delete;

  // Assignment Operator
  OpScaleDetection& operator= (OpScaleDetection const&) & = delete;

  // Move Constructor
  OpScaleDetection(OpScaleDetection &&) = delete;

  // Move Assignment Operator
  OpScaleDetection& operator= (OpScaleDetection &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const float *rtu_signalRaw, float *rty_Signal, bool *rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_rst_time, float rtp_Cs_set_time, float
            rtp_Cs_smpl_time, float rtp_Cs_val_max, float rtp_Cs_val_min);

  // Constructor
  OpScaleDetection();

  // Destructor
  ~OpScaleDetection();

  // private data and function members
 private:
  // Block states
  DW_OpScaleDetection_T OpScaleDetection_DW;

  // model instance variable for '<Root>/isDefectRS'
  R_S_FlipFlop isDefectRSMDLOBJ1;

  // model instance variable for '<Root>/risingEdgeReset'
  RisingEdgeDelay risingEdgeResetMDLOBJ2;

  // model instance variable for '<Root>/risingEdgeSet'
  RisingEdgeDelay risingEdgeSetMDLOBJ3;
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
//  '<Root>' : 'OpScaleDetection'

#endif                                 // OpScaleDetection_h_

//
// File trailer for generated code.
//
// [EOF]
//
