//
// File: TFLOW4_Ctrl.h
//
// Code generated for Simulink model 'TFLOW4_Ctrl'.
//
// Model version                  : 1.362
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Fri Sep  5 16:52:29 2025
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
#ifndef TFLOW4_Ctrl_h_
#define TFLOW4_Ctrl_h_
#include <stdbool.h>
#include <stdint.h>
#include "TFLOW4_Ctrl_types.h"
#include "InPutMng.h"
#include "OutPutMng.h"
#include "SysConfigCalib.h"
#include "SysMng.h"
#include "VentCtrl.h"
#include "WaterHeatCtrl.h"

// External data declarations for dependent source files
extern const tb_Control_In TFLOW4_Ctrl_rtZtb_Control_In;// tb_Control_In ground
extern const tb_InPutMng_In TFLOW4_Ctrl_rtZtb_InPutMng_In;// tb_InPutMng_In ground
extern const tb_SysMng_In TFLOW4_Ctrl_rtZtb_SysMng_In;// tb_SysMng_In ground
extern const tb_VentCtrl_In TFLOW4_Ctrl_rtZtb_VentCtrl_In;// tb_VentCtrl_In ground
extern const tb_WaterHeatCtrl_In TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_In;// tb_WaterHeatCtrl_In ground
extern const tb_OutPutMng_In TFLOW4_Ctrl_rtZtb_OutPutMng_In;// tb_OutPutMng_In ground
extern const tb_WaterHeatCtrl_Out TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_Out;// tb_WaterHeatCtrl_Out ground
extern const tb_Control_Out TFLOW4_Ctrl_rtZtb_Control_Out;// tb_Control_Out ground
extern const tb_SysMng_Out TFLOW4_Ctrl_rtZtb_SysMng_Out;// tb_SysMng_Out ground

// Model block global parameters (default storage)
extern uint8_t rtP_Control_tick_C;     // Variable: Control_tick_C
                                          //  Referenced by: '<S4>/SysMng'

extern uint8_t rtP_WaterHeatCtrl_tick_C;// Variable: WaterHeatCtrl_tick_C
                                           //  Referenced by: '<S7>/WaterHeatCtrl'

extern uint8_t rtP_input_mng_tick;     // Variable: input_mng_tick
                                          //  Referenced by: '<S1>/InPutMng'


// Class declaration for model TFLOW4_Ctrl
class TFLOW4_Ctrl final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW_TFLOW4_Ctrl_T {
    tb_InPutMng_In In_InPutMng;        // '<S8>/In_InPutMng'
    tb_InPutMng_Out InPutSecu_Out;     // '<S1>/InPutMng'
    tb_SysMng_In SysMng_In;            // '<S9>/In_SysMng'
    tb_OutPutMng_In OutPutMng_In;      // '<S2>/In_OutPutMng'
    tb_WaterHeatCtrl_In In_WaterHeatCtrl;// '<S11>/In_WaterHeatCtrl'
    tb_WaterHeatCtrl_Out WaterHeatDrv_Out;// '<S7>/WaterHeatCtrl'
    tb_VentCtrl_In In_VentCtrl;        // '<S10>/In_VentCtrl'
    tb_SysMng_Out SysMng_Out;          // '<S4>/SysMng'
    tb_SysConfigCalib_Out SysConfigCalib_Out;// '<S3>/SysConfigCalib'
    tb_VentCtrl_Out VentCtrl_Out;      // '<S6>/VentCtrl'
    uint8_t is_active_c2_TFLOW4_Ctrl;  // '<Root>/SysScheduler'
    uint8_t is_c2_TFLOW4_Ctrl;         // '<Root>/SysScheduler'
    uint8_t temporalCounter_i1;        // '<Root>/SysScheduler'
    uint8_t temporalCounter_i2;        // '<Root>/SysScheduler'
    uint8_t temporalCounter_i3;        // '<Root>/SysScheduler'
    uint8_t temporalCounter_i4;        // '<Root>/SysScheduler'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_TFLOW4_Ctrl_T {
    tb_Control_In Control_In;          // '<Root>/Control_In'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_TFLOW4_Ctrl_T {
    tb_Control_Out Control_Out;        // '<Root>/Control_Out'
  };

  // Parameters (default storage)
  struct P_TFLOW4_Ctrl_T {
    tb_InPutMng_Out InPutSecu_Out_Y0;  // Computed Parameter: InPutSecu_Out_Y0
                                          //  Referenced by: '<S1>/InPutSecu_Out'

    tb_WaterHeatCtrl_Out WaterHeatDrv_Out_Y0;// Computed Parameter: WaterHeatDrv_Out_Y0
                                                //  Referenced by: '<S7>/WaterHeatDrv_Out'

    tb_SysMng_Out SysMng_Out_Y0;       // Computed Parameter: SysMng_Out_Y0
                                          //  Referenced by: '<S4>/SysMng_Out'

    tb_SysConfigCalib_Out SysConfigCalib_Out_Y0;// Computed Parameter: SysConfigCalib_Out_Y0
                                                   //  Referenced by: '<S3>/SysConfigCalib_Out'

    tb_VentCtrl_Out VentCtrl_Out_Y0;   // Computed Parameter: VentCtrl_Out_Y0
                                          //  Referenced by: '<S6>/VentCtrl_Out'

  };

  // Copy Constructor
  TFLOW4_Ctrl(TFLOW4_Ctrl const&) = delete;

  // Assignment Operator
  TFLOW4_Ctrl& operator= (TFLOW4_Ctrl const&) & = delete;

  // Move Constructor
  TFLOW4_Ctrl(TFLOW4_Ctrl &&) = delete;

  // Move Assignment Operator
  TFLOW4_Ctrl& operator= (TFLOW4_Ctrl &&) = delete;

  // model instance variable for '<S1>/InPutMng'
  InPutMng InPutMngMDLOBJ1;

  // model instance variable for '<Root>/OutPutMng'
  OutPutMng OutPutMngMDLOBJ2;

  // model instance variable for '<S4>/SysMng'
  SysMng SysMngMDLOBJ3;

  // model instance variable for '<S6>/VentCtrl'
  VentCtrl VentCtrlMDLOBJ4;

  // model instance variable for '<S7>/WaterHeatCtrl'
  WaterHeatCtrl WaterHeatCtrlMDLOBJ5;

  // model instance variable for '<S3>/SysConfigCalib'
  SysConfigCalib SysConfigCalibMDLOBJ6;

  // External inputs
  ExtU_TFLOW4_Ctrl_T TFLOW4_Ctrl_U;

  // External outputs
  ExtY_TFLOW4_Ctrl_T TFLOW4_Ctrl_Y;

  // Block states
  DW_TFLOW4_Ctrl_T TFLOW4_Ctrl_DW;

  // Tunable parameters
  static P_TFLOW4_Ctrl_T TFLOW4_Ctrl_P;

  // Root inports set method
  void setExternalInputs(const ExtU_TFLOW4_Ctrl_T *pExtU_TFLOW4_Ctrl_T)
  {
    TFLOW4_Ctrl_U = *pExtU_TFLOW4_Ctrl_T;
  }

  // Root outports get method
  const ExtY_TFLOW4_Ctrl_T &getExternalOutputs() const
  {
    return TFLOW4_Ctrl_Y;
  }

  // Block states get method
  const DW_TFLOW4_Ctrl_T &getDWork() const
  {
    return TFLOW4_Ctrl_DW;
  }

  // Block states set method
  void setDWork(const DW_TFLOW4_Ctrl_T *pDW_TFLOW4_Ctrl_T)
  {
    TFLOW4_Ctrl_DW = *pDW_TFLOW4_Ctrl_T;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  TFLOW4_Ctrl();

  // Destructor
  ~TFLOW4_Ctrl();

  // private data and function members
 private:
  // private member function(s) for subsystem '<Root>/OutPutMng_Interfaces'
  void TFLOW4_Ctr_OutPutMng_Interfaces();

  // private member function(s) for subsystem '<S7>/WaterHeatCtrl_Interfaces'
  void TFLOW4_WaterHeatCtrl_Interfaces();

  // private member function(s) for subsystem '<S6>/VentCtrl_Interfaces'
  void TFLOW4_Ctrl_VentCtrl_Interfaces();

  // private member function(s) for subsystem '<S1>/InPutMng_Interfaces'
  void TFLOW4_Ctrl_InPutMng_Interfaces();

  // private member function(s) for subsystem '<S4>/SysMng_Interfaces'
  void TFLOW4_Ctrl_SysMng_Interfaces();
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
//  '<Root>' : 'TFLOW4_Ctrl'
//  '<S1>'   : 'TFLOW4_Ctrl/InPutMng'
//  '<S2>'   : 'TFLOW4_Ctrl/OutPutMng_Interfaces'
//  '<S3>'   : 'TFLOW4_Ctrl/SysConfigCalib'
//  '<S4>'   : 'TFLOW4_Ctrl/SysMng'
//  '<S5>'   : 'TFLOW4_Ctrl/SysScheduler'
//  '<S6>'   : 'TFLOW4_Ctrl/VentCtrl'
//  '<S7>'   : 'TFLOW4_Ctrl/WaterHeatCtrl'
//  '<S8>'   : 'TFLOW4_Ctrl/InPutMng/InPutMng_Interfaces'
//  '<S9>'   : 'TFLOW4_Ctrl/SysMng/SysMng_Interfaces'
//  '<S10>'  : 'TFLOW4_Ctrl/VentCtrl/VentCtrl_Interfaces'
//  '<S11>'  : 'TFLOW4_Ctrl/WaterHeatCtrl/WaterHeatCtrl_Interfaces'

#endif                                 // TFLOW4_Ctrl_h_

//
// File trailer for generated code.
//
// [EOF]
//
