//
// File: TFLOW4_Ctrl_data.cpp
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
#include "TFLOW4_Ctrl.h"

// Model block global parameters (default storage)
uint8_t rtP_Control_tick_C{ 1U };      // Variable: Control_tick_C
                                          //  Referenced by: '<S21>/RisingEdgeDelay'


uint8_t rtP_WaterHeatCtrl_tick_C{ 1U };// Variable: WaterHeatCtrl_tick_C
                                          //  Referenced by: '<S7>/WaterHeatCtrl'


uint8_t rtP_input_mng_tick{ 1U };      // Variable: input_mng_tick
                                          //  Referenced by: '<Root>/InPutMng'


// Block parameters (default storage)
TFLOW4_Ctrl::P_TFLOW4_Ctrl_T TFLOW4_Ctrl::TFLOW4_Ctrl_P{
  // Mask Parameter: RisingEdgeDelay_delay
  //  Referenced by: '<S21>/RisingEdgeDelay'

  1800.0F,

  // Mask Parameter: secTimer_sampleTime
  //  Referenced by: '<S23>/Timer'

  1.15740741E-5F,

  // Computed Parameter: WaterHeatDrv_Out_Y0
  //  Referenced by: '<S7>/WaterHeatDrv_Out'

  {
    te_heat_mode::v40_ctrl,            // Ss_heat_mode
    0U,                                // Cs_v40_sp
    0,                                 // Cs_temp_sp
    0U,                                // Cs_heat_pump_rot_spd_sp
    te_on_off::off,                    // Ss_elec_bstr_htr_sp
    0U,                                // Cs_v40_rat
    te_heat_stt::Heat_Stopped,         // Ss_heat_pump_stt
    te_heat_stt::Heat_Stopped          // Ss_elec_htr_bstr_stt
  },

  // Computed Parameter: WaterHeatCtrl_Out_delay_Initial
  //  Referenced by: '<S1>/WaterHeatCtrl_Out_delay'

  {
    te_heat_mode::v40_ctrl,            // Ss_heat_mode
    0U,                                // Cs_v40_sp
    0,                                 // Cs_temp_sp
    0U,                                // Cs_heat_pump_rot_spd_sp
    te_on_off::off,                    // Ss_elec_bstr_htr_sp
    0U,                                // Cs_v40_rat
    te_heat_stt::Heat_Stopped,         // Ss_heat_pump_stt
    te_heat_stt::Heat_Stopped          // Ss_elec_htr_bstr_stt
  },

  // Computed Parameter: SysConfigCalib_Out_Y0
  //  Referenced by: '<S3>/SysConfigCalib_Out'

  {
    0U,                                // Cs_tank_vol_up
    0U,                                // Cs_tank_vol_btwn
    0U,                                // Cs_tank_vol_down
    0U                                 // Cs_tank_vol_tot
  },

  // Computed Parameter: SysConfigCalib_Out_delay_Initia
  //  Referenced by: '<S1>/SysConfigCalib_Out_delay'

  {
    0U,                                // Cs_tank_vol_up
    0U,                                // Cs_tank_vol_btwn
    0U,                                // Cs_tank_vol_down
    0U                                 // Cs_tank_vol_tot
  },

  // Computed Parameter: VentCtrl_Out_Y0
  //  Referenced by: '<S6>/VentCtrl_Out'

  {
    0U,                                // Cs_vent_pres_sp
    0U,                                // Cs_vent_flow_sp
    0U,                                // Cs_vent_vltg_sp
    0U                                 // Cs_pres_rise_curv_sp
  },

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S13>/Constant4'

  te_heat_stt::Heat_Stopped,

  // Expression: te_heat_stt.Heat_Waiting
  //  Referenced by: '<S13>/Constant7'

  te_heat_stt::Heat_Waiting,

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S13>/Constant6'

  te_heat_stt::Heat_Stopped,

  // Expression: te_heat_stt.Heat_Waiting
  //  Referenced by: '<S13>/Constant8'

  te_heat_stt::Heat_Waiting,

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S14>/Constant3'

  te_heat_stt::Heat_Stopped,

  // Expression: te_heat_stt.Heat_Waiting
  //  Referenced by: '<S14>/Constant5'

  te_heat_stt::Heat_Waiting,

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S14>/Constant4'

  te_heat_stt::Heat_Stopped,

  // Expression: te_heat_stt.Heat_Waiting
  //  Referenced by: '<S14>/Constant6'

  te_heat_stt::Heat_Waiting,

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S5>/Ss_heat_pump_stt'

  te_heat_stt::Heat_Stopped,

  // Expression: te_heat_stt.Heat_Stopped
  //  Referenced by: '<S5>/Ss_elec_htr_bstr_stt'

  te_heat_stt::Heat_Stopped,

  // Expression: te_on_off.on
  //  Referenced by: '<S10>/on'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S13>/Contant'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S14>/Constant'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S14>/Constant1'

  te_on_off::on,

  // Expression: te_on_off.off
  //  Referenced by: '<S14>/Constant2'

  te_on_off::off,

  // Expression: te_on_off.on
  //  Referenced by: '<S16>/Constant4'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S16>/Constant1'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S17>/Constant5'

  te_on_off::on,

  // Expression: te_op_mode.FullElec
  //  Referenced by: '<S4>/opModeDelay'

  te_op_mode::FullElec,

  // Expression: te_op_mode.Boost
  //  Referenced by: '<S13>/Constant2'

  te_op_mode::Boost,

  // Expression: te_op_mode.HeatPumpTest
  //  Referenced by: '<S15>/HeatPumpTestCstt'

  te_op_mode::HeatPumpTest,

  // Expression: te_op_mode.AntiLegionella
  //  Referenced by: '<S14>/Constant7'

  te_op_mode::AntiLegionella,

  // Expression: te_op_mode.Holidays
  //  Referenced by: '<S16>/Constant5'

  te_op_mode::Holidays,

  // Expression: te_sys_ver.Collective
  //  Referenced by: '<S10>/Collective'

  te_sys_ver::Collective,

  // Expression: te_tech_mode.tech_HeatPump
  //  Referenced by: '<S13>/Constant1'

  te_tech_mode::tech_HeatPump,

  // Expression: te_tech_mode.tech_FullElec
  //  Referenced by: '<S16>/Constant'

  te_tech_mode::tech_FullElec,

  // Expression: te_vent_op_mode.Inoperative
  //  Referenced by: '<S10>/Inoperative'

  te_vent_op_mode::Inoperative,

  // Expression: te_vent_op_mode.SafetyOff
  //  Referenced by: '<S10>/Stopped'

  te_vent_op_mode::SafetyOff,

  // Expression: te_vent_op_mode.ConstantFlow
  //  Referenced by: '<S10>/ConstantFlow'

  te_vent_op_mode::ConstantFlow,

  // Expression: te_vent_op_mode.Normal
  //  Referenced by: '<S10>/Normal'

  te_vent_op_mode::Normal,

  // Computed Parameter: HolidaysModeActDelay_InitialCon
  //  Referenced by: '<S4>/HolidaysModeActDelay'

  false,

  // Computed Parameter: SmartGridActDelay_InitialCondit
  //  Referenced by: '<S4>/SmartGridActDelay'

  false,

  // Computed Parameter: AntiLegionnellaModeActDelay_Ini
  //  Referenced by: '<S4>/AntiLegionnellaModeActDelay'

  false,

  // Computed Parameter: OneMoreDay_Bias
  //  Referenced by: '<S16>/OneMoreDay'

  1U
};

//
// File trailer for generated code.
//
// [EOF]
//
