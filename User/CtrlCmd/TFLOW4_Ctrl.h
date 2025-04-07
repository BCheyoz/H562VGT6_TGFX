//
// File: TFLOW4_Ctrl.h
//
// Code generated for Simulink model 'TFLOW4_Ctrl'.
//
// Model version                  : 1.309
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:25:49 2025
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
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "RisingEdgeDetect.h"
#include "SysConfigCalib.h"
#include "Timer.h"
#include "VentCtrl.h"
#include "WaterHeatCtrl.h"

// External data declarations for dependent source files
extern const tb_Control_In TFLOW4_Ctrl_rtZtb_Control_In;// tb_Control_In ground
extern const tb_WaterHeatCtrl_In TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_In;// tb_WaterHeatCtrl_In ground 
extern const tb_Control_Out TFLOW4_Ctrl_rtZtb_Control_Out;// tb_Control_Out ground 

// Model block global parameters (default storage)
extern uint8_t rtP_Control_tick_C;     // Variable: Control_tick_C
                                          //  Referenced by: '<S21>/RisingEdgeDelay'

extern uint8_t rtP_WaterHeatCtrl_tick_C;// Variable: WaterHeatCtrl_tick_C
                                           //  Referenced by: '<S7>/WaterHeatCtrl'

extern uint8_t rtP_input_mng_tick;     // Variable: input_mng_tick
                                          //  Referenced by: '<Root>/InPutMng'


// Class declaration for model TFLOW4_Ctrl
class TFLOW4_Ctrl final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW_TFLOW4_Ctrl_T {
    tb_WaterHeatCtrl_In BusConversion_InsertedFor_Water;
    tb_InPutMng_Out InPutSecu_Out;     // '<Root>/InPutMng'
    tb_WaterHeatCtrl_Out WaterHeatDrv_Out;// '<S7>/WaterHeatCtrl'
    tb_WaterHeatCtrl_Out WaterHeatCtrl_Out_delay_DSTATE;// '<S1>/WaterHeatCtrl_Out_delay' 
    tb_SysConfigCalib_Out SysConfigCalib_j;// '<S3>/SysConfigCalib'
    tb_SysConfigCalib_Out SysConfigCalib_Out_delay_DSTATE;// '<S1>/SysConfigCalib_Out_delay' 
    tb_VentCtrl_Out VentCtrl_Out;      // '<S6>/VentCtrl'
    te_on_off Ss_heat_pump_ena;        // '<S4>/modeSelection'
    te_on_off Ss_elec_bstr_htr_ena;    // '<S4>/modeSelection'
    te_heat_stt Ss_heat_pump_stt;      // '<S5>/Ss_heat_pump_stt'
    te_heat_stt Ss_elec_htr_bstr_stt;  // '<S5>/Ss_elec_htr_bstr_stt'
    te_heat_stt Ss_heat_pump_stt_DSTATE;// '<S5>/Ss_heat_pump_stt'
    te_heat_stt Ss_elec_htr_bstr_stt_DSTATE;// '<S5>/Ss_elec_htr_bstr_stt'
    te_op_mode Ss_op_mode;             // '<S4>/modeSelection'
    te_op_mode opModeDelay_DSTATE;     // '<S4>/opModeDelay'
    SysSchedulerModeType SysSchedulerMode;// '<S4>/SysScheduler'
    uint8_t is_active_c3_TFLOW4_Ctrl;  // '<S4>/modeSelection'
    uint8_t is_c3_TFLOW4_Ctrl;         // '<S4>/modeSelection'
    uint8_t is_SystemMode;             // '<S4>/modeSelection'
    uint8_t is_AntiLegionella;         // '<S4>/modeSelection'
    uint8_t is_ModeSelection;          // '<S4>/modeSelection'
    uint8_t is_HeatPumpUsed;           // '<S4>/modeSelection'
    uint8_t is_UsualMode;              // '<S4>/modeSelection'
    uint8_t is_HeatPumpMode;           // '<S4>/modeSelection'
    uint8_t is_SmartGrid;              // '<S4>/modeSelection'
    uint8_t is_active_c2_TFLOW4_Ctrl;  // '<S4>/SysScheduler'
    uint8_t temporalCounter_i1;        // '<S4>/SysScheduler'
    uint8_t temporalCounter_i2;        // '<S4>/SysScheduler'
    uint8_t temporalCounter_i3;        // '<S4>/SysScheduler'
    uint8_t temporalCounter_i4;        // '<S4>/SysScheduler'
    bool stt;                          // '<S18>/R_S_FlipFlop'
    bool stt_k;                        // '<S20>/R_S_FlipFlop'
    bool Bs_clk_on;                    // '<S4>/modeSelection'
    bool HolidaysModeActDelay_DSTATE;  // '<S4>/HolidaysModeActDelay'
    bool SmartGridActDelay_DSTATE;     // '<S4>/SmartGridActDelay'
    bool AntiLegionnellaModeActDelay_DST;// '<S4>/AntiLegionnellaModeActDelay'
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
    float RisingEdgeDelay_delay;       // Mask Parameter: RisingEdgeDelay_delay
                                          //  Referenced by: '<S21>/RisingEdgeDelay'

    float secTimer_sampleTime;         // Mask Parameter: secTimer_sampleTime
                                          //  Referenced by: '<S23>/Timer'

    tb_WaterHeatCtrl_Out WaterHeatDrv_Out_Y0;
                                      // Computed Parameter: WaterHeatDrv_Out_Y0
                                         //  Referenced by: '<S7>/WaterHeatDrv_Out'

    tb_WaterHeatCtrl_Out WaterHeatCtrl_Out_delay_Initial;
                          // Computed Parameter: WaterHeatCtrl_Out_delay_Initial
                             //  Referenced by: '<S1>/WaterHeatCtrl_Out_delay'

    tb_SysConfigCalib_Out SysConfigCalib_Out_Y0;
                                    // Computed Parameter: SysConfigCalib_Out_Y0
                                       //  Referenced by: '<S3>/SysConfigCalib_Out'

    tb_SysConfigCalib_Out SysConfigCalib_Out_delay_Initia;
                          // Computed Parameter: SysConfigCalib_Out_delay_Initia
                             //  Referenced by: '<S1>/SysConfigCalib_Out_delay'

    tb_VentCtrl_Out VentCtrl_Out_Y0;   // Computed Parameter: VentCtrl_Out_Y0
                                          //  Referenced by: '<S6>/VentCtrl_Out'

    te_heat_stt Constant4_Value;       // Expression: te_heat_stt.Heat_Stopped
                                          //  Referenced by: '<S13>/Constant4'

    te_heat_stt Constant7_Value;       // Expression: te_heat_stt.Heat_Waiting
                                          //  Referenced by: '<S13>/Constant7'

    te_heat_stt Constant6_Value;       // Expression: te_heat_stt.Heat_Stopped
                                          //  Referenced by: '<S13>/Constant6'

    te_heat_stt Constant8_Value;       // Expression: te_heat_stt.Heat_Waiting
                                          //  Referenced by: '<S13>/Constant8'

    te_heat_stt Constant3_Value;       // Expression: te_heat_stt.Heat_Stopped
                                          //  Referenced by: '<S14>/Constant3'

    te_heat_stt Constant5_Value;       // Expression: te_heat_stt.Heat_Waiting
                                          //  Referenced by: '<S14>/Constant5'

    te_heat_stt Constant4_Value_j;     // Expression: te_heat_stt.Heat_Stopped
                                          //  Referenced by: '<S14>/Constant4'

    te_heat_stt Constant6_Value_p;     // Expression: te_heat_stt.Heat_Waiting
                                          //  Referenced by: '<S14>/Constant6'

    te_heat_stt Ss_heat_pump_stt_InitialConditi;// Expression: te_heat_stt.Heat_Stopped
                                                   //  Referenced by: '<S5>/Ss_heat_pump_stt'

    te_heat_stt Ss_elec_htr_bstr_stt_InitialCon;// Expression: te_heat_stt.Heat_Stopped
                                                   //  Referenced by: '<S5>/Ss_elec_htr_bstr_stt'

    te_on_off on_Value;                // Expression: te_on_off.on
                                          //  Referenced by: '<S10>/on'

    te_on_off Contant_Value;           // Expression: te_on_off.on
                                          //  Referenced by: '<S13>/Contant'

    te_on_off Constant_Value;          // Expression: te_on_off.on
                                          //  Referenced by: '<S14>/Constant'

    te_on_off Constant1_Value;         // Expression: te_on_off.on
                                          //  Referenced by: '<S14>/Constant1'

    te_on_off Constant2_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S14>/Constant2'

    te_on_off Constant4_Value_m;       // Expression: te_on_off.on
                                          //  Referenced by: '<S16>/Constant4'

    te_on_off Constant1_Value_p;       // Expression: te_on_off.on
                                          //  Referenced by: '<S16>/Constant1'

    te_on_off Constant5_Value_h;       // Expression: te_on_off.on
                                          //  Referenced by: '<S17>/Constant5'

    te_op_mode opModeDelay_InitialCondition;// Expression: te_op_mode.FullElec
                                               //  Referenced by: '<S4>/opModeDelay'

    te_op_mode Constant2_Value_m;      // Expression: te_op_mode.Boost
                                          //  Referenced by: '<S13>/Constant2'

    te_op_mode HeatPumpTestCstt_Value; // Expression: te_op_mode.HeatPumpTest
                                          //  Referenced by: '<S15>/HeatPumpTestCstt'

    te_op_mode Constant7_Value_c;      // Expression: te_op_mode.AntiLegionella
                                          //  Referenced by: '<S14>/Constant7'

    te_op_mode Constant5_Value_p;      // Expression: te_op_mode.Holidays
                                          //  Referenced by: '<S16>/Constant5'

    te_sys_ver Collective_Value;       // Expression: te_sys_ver.Collective
                                          //  Referenced by: '<S10>/Collective'

    te_tech_mode Constant1_Value_i;    // Expression: te_tech_mode.tech_HeatPump
                                          //  Referenced by: '<S13>/Constant1'

    te_tech_mode Constant_Value_d;     // Expression: te_tech_mode.tech_FullElec
                                          //  Referenced by: '<S16>/Constant'

    te_vent_op_mode Inoperative_Value;// Expression: te_vent_op_mode.Inoperative
                                         //  Referenced by: '<S10>/Inoperative'

    te_vent_op_mode Stopped_Value;     // Expression: te_vent_op_mode.SafetyOff
                                          //  Referenced by: '<S10>/Stopped'

    te_vent_op_mode ConstantFlow_Value;
                                     // Expression: te_vent_op_mode.ConstantFlow
                                        //  Referenced by: '<S10>/ConstantFlow'

    te_vent_op_mode Normal_Value;      // Expression: te_vent_op_mode.Normal
                                          //  Referenced by: '<S10>/Normal'

    bool HolidaysModeActDelay_InitialCon;
                          // Computed Parameter: HolidaysModeActDelay_InitialCon
                             //  Referenced by: '<S4>/HolidaysModeActDelay'

    bool SmartGridActDelay_InitialCondit;
                          // Computed Parameter: SmartGridActDelay_InitialCondit
                             //  Referenced by: '<S4>/SmartGridActDelay'

    bool AntiLegionnellaModeActDelay_Ini;
                          // Computed Parameter: AntiLegionnellaModeActDelay_Ini
                             //  Referenced by: '<S4>/AntiLegionnellaModeActDelay'

    uint8_t OneMoreDay_Bias;           // Computed Parameter: OneMoreDay_Bias
                                          //  Referenced by: '<S16>/OneMoreDay'

  };

  // Copy Constructor
  TFLOW4_Ctrl(TFLOW4_Ctrl const&) = delete;

  // Assignment Operator
  TFLOW4_Ctrl& operator= (TFLOW4_Ctrl const&) & = delete;

  // Move Constructor
  TFLOW4_Ctrl(TFLOW4_Ctrl &&) = delete;

  // Move Assignment Operator
  TFLOW4_Ctrl& operator= (TFLOW4_Ctrl &&) = delete;

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
  // External inputs
  ExtU_TFLOW4_Ctrl_T TFLOW4_Ctrl_U;

  // External outputs
  ExtY_TFLOW4_Ctrl_T TFLOW4_Ctrl_Y;

  // Block states
  DW_TFLOW4_Ctrl_T TFLOW4_Ctrl_DW;

  // Tunable parameters
  static P_TFLOW4_Ctrl_T TFLOW4_Ctrl_P;

  // private member function(s) for subsystem '<Root>/SysMng'
  void TFLOW4_Ctrl_SysMng_Init();
  void TFLOW4_Ctrl_SysMng();
  void TFL_enter_internal_HeatPumpUsed(void);
  void TFLOW4_Ctrl_ModeSelection(const bool *HolidaysModeActDelay, const bool
    *SmartGridActDelay, const bool *AntiLegionnellaModeActDelay);

  // model instance variable for '<Root>/InPutMng'
  InPutMng InPutMngMDLOBJ1;

  // model instance variable for '<Root>/OutPutMng'
  OutPutMng OutPutMngMDLOBJ2;

  // model instance variable for '<S3>/SysConfigCalib'
  SysConfigCalib SysConfigCalibMDLOBJ3;

  // model instance variable for '<S13>/BoostRqstRisingEdge'
  RisingEdgeDetect BoostRqstRisingEdgeMDLOBJ4;

  // model instance variable for '<S18>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ5;

  // model instance variable for '<S19>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ6;

  // model instance variable for '<S15>/HeatPumpTestRqstRisingEdge'
  RisingEdgeDetect HeatPumpTestRqstRisiMDLOBJ7;

  // model instance variable for '<S15>/HeatPumpTestRqstRstRisingEdge'
  RisingEdgeDetect HeatPumpTestRqstRstRMDLOBJ8;

  // model instance variable for '<S20>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ9;

  // model instance variable for '<S21>/RisingEdgeDelay'
  RisingEdgeDelay RisingEdgeDelayMDLOBJ10;

  // model instance variable for '<S22>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ11;

  // model instance variable for '<S16>/clkRchRisingEdge'
  RisingEdgeDetect clkRchRisingEdgeMDLOBJ12;

  // model instance variable for '<S16>/hldyRqstRisingEdge'
  RisingEdgeDetect hldyRqstRisingEdgeMDLOBJ13;

  // model instance variable for '<S23>/Timer'
  Timer TimerMDLOBJ14;

  // model instance variable for '<S6>/VentCtrl'
  VentCtrl VentCtrlMDLOBJ15;

  // model instance variable for '<S7>/WaterHeatCtrl'
  WaterHeatCtrl WaterHeatCtrlMDLOBJ16;
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
//  '<S1>'   : 'TFLOW4_Ctrl/InPutMng_Interfaces'
//  '<S2>'   : 'TFLOW4_Ctrl/OutPutMng_Interfaces'
//  '<S3>'   : 'TFLOW4_Ctrl/SysConfigCalib'
//  '<S4>'   : 'TFLOW4_Ctrl/SysMng'
//  '<S5>'   : 'TFLOW4_Ctrl/SysMng_Interfaces'
//  '<S6>'   : 'TFLOW4_Ctrl/VentCtrl'
//  '<S7>'   : 'TFLOW4_Ctrl/WaterHeatCtrl'
//  '<S8>'   : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection'
//  '<S9>'   : 'TFLOW4_Ctrl/SysMng/SysScheduler'
//  '<S10>'  : 'TFLOW4_Ctrl/SysMng/VentModeSelection'
//  '<S11>'  : 'TFLOW4_Ctrl/SysMng/dayCounter'
//  '<S12>'  : 'TFLOW4_Ctrl/SysMng/modeSelection'
//  '<S13>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/BoostModeActivation'
//  '<S14>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/antiLegionellaModeActivation'
//  '<S15>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/heatPumpTestModeActivation'
//  '<S16>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/holidaysModeActivation'
//  '<S17>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/smartGridModeActivation'
//  '<S18>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/BoostModeActivation/R_S_Boost'
//  '<S19>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/antiLegionellaModeActivation/R_S_anti_Lgn'
//  '<S20>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/heatPumpTestModeActivation/R_S_anti_Lgn'
//  '<S21>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/heatPumpTestModeActivation/RisingEdgeDelay'
//  '<S22>'  : 'TFLOW4_Ctrl/SysMng/ConditionForModeSelection/holidaysModeActivation/R_S_Holidays'
//  '<S23>'  : 'TFLOW4_Ctrl/SysMng/dayCounter/secTimer'
//  '<S24>'  : 'TFLOW4_Ctrl/VentCtrl/VentCtrl_Interfaces'
//  '<S25>'  : 'TFLOW4_Ctrl/WaterHeatCtrl/WaterHeatCtrl_Interfaces'

#endif                                 // TFLOW4_Ctrl_h_

//
// File trailer for generated code.
//
// [EOF]
//
