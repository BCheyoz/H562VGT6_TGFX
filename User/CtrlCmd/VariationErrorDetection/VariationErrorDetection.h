//
// File: VariationErrorDetection.h
//
// Code generated for Simulink model 'VariationErrorDetection'.
//
// Model version                  : 1.28
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:22 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef VariationErrorDetection_h_
#define VariationErrorDetection_h_
#include <stdbool.h>
#include <stdint.h>
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include <cstring>

// Class declaration for model VariationErrorDetection
class VariationErrorDetection final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'VariationErrorDetection'
  struct DW_VariationErrorDetection_T {
    float prevSignalMesRaw_DSTATE;     // '<Root>/prevSignalMesRaw'
  };

  // Copy Constructor
  VariationErrorDetection(VariationErrorDetection const&) = delete;

  // Assignment Operator
  VariationErrorDetection& operator= (VariationErrorDetection const&) & = delete;

  // Move Constructor
  VariationErrorDetection(VariationErrorDetection &&) = delete;

  // Move Assignment Operator
  VariationErrorDetection& operator= (VariationErrorDetection &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const float *rtu_signalRaw, float *rty_signal, bool *rty_isDefect, float rtp_Cs_dfct_rst_dlay_time, float rtp_Cs_dfct_set_dlay_time, float
            rtp_Cs_dflt_val, float rtp_Cs_smpl_time, float rtp_Cs_var_max);

  // Constructor
  VariationErrorDetection();

  // Destructor
  ~VariationErrorDetection();

  // private data and function members
 private:
  // Block states
  DW_VariationErrorDetection_T VariationErrorDetection_DW;

  // model instance variable for '<Root>/DefectSwitch'
  R_S_FlipFlop DefectSwitchMDLOBJ1;

  // model instance variable for '<Root>/edgeDelayReset'
  RisingEdgeDelay edgeDelayResetMDLOBJ2;

  // model instance variable for '<Root>/edgeDelaySet'
  RisingEdgeDelay edgeDelaySetMDLOBJ3;
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
//  '<Root>' : 'VariationErrorDetection'

#endif                                 // VariationErrorDetection_h_

//
// File trailer for generated code.
//
// [EOF]
//
