//
// File: WaterHeatCtrl.h
//
// Code generated for Simulink model 'WaterHeatCtrl'.
//
// Model version                  : 1.73
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 12:15:46 2025
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
#ifndef WaterHeatCtrl_h_
#define WaterHeatCtrl_h_
#include <stdbool.h>
#include <stdint.h>
#include "WaterHeatCtrl_types.h"
#include "WaterHeatController.h"
#include "WaterHeatSpCalc.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_WaterHeatCtrl_tick_C;// Variable: WaterHeatCtrl_tick_C
                                           //  Referenced by: '<Root>/WaterHeatController'


// Class declaration for model WaterHeatCtrl
class WaterHeatCtrl final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'WaterHeatCtrl'
  struct DW_WaterHeatCtrl_T {
    te_on_off WaterHeatController_o2;  // '<Root>/WaterHeatController'
    te_on_off WaterHeatController_o4;  // '<Root>/WaterHeatController'
    te_on_off WaterHeatController_o5;  // '<Root>/WaterHeatController'
  };

  // Initial conditions function
  void init();

  // Copy Constructor
  WaterHeatCtrl(WaterHeatCtrl const&) = delete;

  // Assignment Operator
  WaterHeatCtrl& operator= (WaterHeatCtrl const&) & = delete;

  // Move Constructor
  WaterHeatCtrl(WaterHeatCtrl &&) = delete;

  // Move Assignment Operator
  WaterHeatCtrl& operator= (WaterHeatCtrl &&) = delete;

  // model step function
  void step(const tb_WaterHeatCtrl_In *rtu_WaterHeatCtrl_In, tb_WaterHeatCtrl_Out *rty_WaterHeatCtrl_Out);

  // Reset function
  void reset();

  // Constructor
  WaterHeatCtrl();

  // Destructor
  ~WaterHeatCtrl();

  // private data and function members
 private:
  // Block states
  DW_WaterHeatCtrl_T WaterHeatCtrl_DW;

  // model instance variable for '<Root>/WaterHeatController'
  WaterHeatController WaterHeatControllerMDLOBJ1;

  // model instance variable for '<Root>/WaterHeatSpCalc'
  WaterHeatSpCalc WaterHeatSpCalcMDLOBJ2;
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
//  '<Root>' : 'WaterHeatCtrl'

#endif                                 // WaterHeatCtrl_h_

//
// File trailer for generated code.
//
// [EOF]
//
