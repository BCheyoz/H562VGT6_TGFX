//
// File: StaticErrorDetection.h
//
// Code generated for Simulink model 'StaticErrorDetection'.
//
// Model version                  : 1.50
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:17 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Traceability
//    2. RAM efficiency
//    3. Execution efficiency
// Validation result: Not run
//
#ifndef StaticErrorDetection_h_
#define StaticErrorDetection_h_
#include <stdbool.h>
#include <stdint.h>
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "Timer.h"
#include <cstring>

// Class declaration for model StaticErrorDetection
class StaticErrorDetection final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'StaticErrorDetection'
  struct DW_StaticErrorDetection_T {
    float prevSignalRaw_DSTATE;        // '<Root>/prevSignalRaw'
    uint8_t UnitDelay3_DSTATE;         // '<Root>/Unit Delay3'
    bool UnitDelay1_DSTATE;            // '<Root>/Unit Delay1'
  };

  // Copy Constructor
  StaticErrorDetection(StaticErrorDetection const&) = delete;

  // Assignment Operator
  StaticErrorDetection& operator= (StaticErrorDetection const&) & = delete;

  // Move Constructor
  StaticErrorDetection(StaticErrorDetection &&) = delete;

  // Move Assignment Operator
  StaticErrorDetection& operator= (StaticErrorDetection &&) = delete;

  // Reset function
  void reset();

  // model step function
  void step(const float *rtu_signalRaw, float *rty_signal, bool *rty_isDefect, float rtp_Cs_dflt_val, float rtp_Cs_rst_dlay_time, float rtp_Cs_rst_tol, float
            rtp_Cs_set_dlay_time, float rtp_Cs_smpl_time);

  // Constructor
  StaticErrorDetection();

  // Destructor
  ~StaticErrorDetection();

  // private data and function members
 private:
  // Block states
  DW_StaticErrorDetection_T StaticErrorDetection_DW;

  // model instance variable for '<Root>/EdgeDelayRstTime'
  RisingEdgeDelay EdgeDelayRstTimeMDLOBJ1;

  // model instance variable for '<Root>/EdgeDelaySet'
  RisingEdgeDelay EdgeDelaySetMDLOBJ2;

  // model instance variable for '<Root>/StaticErrorDetection'
  Timer StaticErrorDetectionMDLOBJ3;

  // model instance variable for '<Root>/StaticErrorRS'
  R_S_FlipFlop StaticErrorRSMDLOBJ4;
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
//  '<Root>' : 'StaticErrorDetection'

#endif                                 // StaticErrorDetection_h_

//
// File trailer for generated code.
//
// [EOF]
//
