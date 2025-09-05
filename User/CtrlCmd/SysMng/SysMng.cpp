//
// File: SysMng.cpp
//
// Code generated for Simulink model 'SysMng'.
//
// Model version                  : 1.150
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Sep  4 18:37:05 2025
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
#include "SysMng.h"
#include "SysMng_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "RisingEdgeDetect.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "Timer.h"

// Named constants for Chart: '<Root>/modeSelection'
const uint8_t SysM_IN_AntiLegionella_fullElec{ 1U };

const uint8_t SysMng_IN_AntiLegionella{ 1U };

const uint8_t SysMng_IN_AntiLegionella_normal{ 2U };

const uint8_t SysMng_IN_Auto{ 1U };

const uint8_t SysMng_IN_Boost{ 1U };

const uint8_t SysMng_IN_Eco{ 2U };

const uint8_t SysMng_IN_FullElec{ 1U };

const uint8_t SysMng_IN_HeatPumpMode{ 2U };

const uint8_t SysMng_IN_HeatPumpUsed{ 2U };

const uint8_t SysMng_IN_Holidays{ 2U };

const uint8_t SysMng_IN_Hybrid{ 1U };

const uint8_t SysMng_IN_ModeSelection{ 3U };

const uint8_t SysMng_IN_NO_ACTIVE_CHILD{ 0U };

const uint8_t SysMng_IN_SmartGrid{ 4U };

const uint8_t SysMng_IN_SmartGrid_fullElec{ 1U };

const uint8_t SysMng_IN_SmartGrid_normal{ 2U };

const uint8_t SysMng_IN_SystemMode{ 1U };

const uint8_t SysMng_IN_TestMode{ 2U };

const uint8_t SysMng_IN_UsualMode{ 2U };

SysMng::P_SysMng_T SysMng::SysMng_rtP{
  // Mask Parameter: RisingEdgeDelay_delay
  //  Referenced by: '<S13>/RisingEdgeDelay'

  1800.0F,

  // Mask Parameter: secTimer_sampleTime
  //  Referenced by: '<S15>/Timer'

  1.15740741E-5F,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S5>/Constant4'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S5>/Constant6'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Waiting
  //  Referenced by: '<S5>/Constant7'

  te_heat_stt::Waiting,

  // Expression: te_heat_stt.Waiting
  //  Referenced by: '<S5>/Constant8'

  te_heat_stt::Waiting,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S6>/Constant3'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Waiting
  //  Referenced by: '<S6>/Constant5'

  te_heat_stt::Waiting,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S6>/Constant4'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Waiting
  //  Referenced by: '<S6>/Constant6'

  te_heat_stt::Waiting,

  // Expression: te_on_off.on
  //  Referenced by: '<S5>/Contant'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S6>/Constant'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S6>/Constant1'

  te_on_off::on,

  // Expression: te_on_off.off
  //  Referenced by: '<S6>/Constant2'

  te_on_off::off,

  // Expression: te_on_off.on
  //  Referenced by: '<S8>/Constant4'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S8>/Constant1'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S9>/Constant5'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S2>/on'

  te_on_off::on,

  // Expression: te_op_mode.Boost
  //  Referenced by: '<S5>/Constant2'

  te_op_mode::Boost,

  // Expression: te_op_mode.AntiLegionella
  //  Referenced by: '<S6>/Constant7'

  te_op_mode::AntiLegionella,

  // Expression: te_op_mode.HeatPumpTest
  //  Referenced by: '<S7>/HeatPumpTestCstt'

  te_op_mode::HeatPumpTest,

  // Expression: te_op_mode.Holidays
  //  Referenced by: '<S8>/Constant5'

  te_op_mode::Holidays,

  // Expression: te_op_mode.FullElec
  //  Referenced by: '<Root>/opModeDelay'

  te_op_mode::FullElec,

  // Expression: te_sys_ver.Collective
  //  Referenced by: '<S2>/Collective'

  te_sys_ver::Collective,

  // Expression: te_tech_mode.HeatPump
  //  Referenced by: '<S5>/Constant1'

  te_tech_mode::HeatPump,

  // Expression: te_tech_mode.FullElec
  //  Referenced by: '<S8>/Constant'

  te_tech_mode::FullElec,

  // Expression: te_vent_op_mode.Inoperative
  //  Referenced by: '<S2>/Inoperative'

  te_vent_op_mode::Inoperative,

  // Expression: te_vent_op_mode.Stopped
  //  Referenced by: '<S2>/Stopped'

  te_vent_op_mode::Stopped,

  // Expression: te_vent_op_mode.ConstantFlow
  //  Referenced by: '<S2>/ConstantFlow'

  te_vent_op_mode::ConstantFlow,

  // Expression: te_vent_op_mode.Normal
  //  Referenced by: '<S2>/Normal'

  te_vent_op_mode::Normal,

  // Computed Parameter: AntiLegionnellaModeActDelay_Ini
  //  Referenced by: '<Root>/AntiLegionnellaModeActDelay'

  false,

  // Computed Parameter: HolidaysModeActDelay_InitialCon
  //  Referenced by: '<Root>/HolidaysModeActDelay'

  false,

  // Computed Parameter: SmartGridActDelay_InitialCondit
  //  Referenced by: '<Root>/SmartGridActDelay'

  false,

  // Computed Parameter: bstModeActDelay_InitialConditio
  //  Referenced by: '<Root>/bstModeActDelay'

  false,

  // Computed Parameter: heatPumpTestModeActDelay_Initia
  //  Referenced by: '<Root>/heatPumpTestModeActDelay'

  false,

  // Computed Parameter: OneMoreDay_Bias
  //  Referenced by: '<S8>/OneMoreDay'

  1U
};

// System reset for atomic system: '<S1>/BoostModeActivation'
void SysMng::SysMn_BoostModeActivation_Reset()
{
  BoostRqstRisingEdgeMDLOBJ1.reset();
  R_S_FlipFlopMDLOBJ2.reset();
}

// Output and update for atomic system: '<S1>/BoostModeActivation'
void SysMng::SysMng_BoostModeActivation()
{
  // local block i/o variables
  bool rtb_Bs_bst_mode_rqst_uint;
  uint8_t rtb_Ss_bst_rqst_on_uint;
  bool rtb_Bs_bst_mode_rst_o;
  bool rtb_Bs_bst_mode_set;
  rtb_Ss_bst_rqst_on_uint = (SysMng_rtu_SysMng_In->Ss_bst_rqst == SysMng_rtP.Contant_Value);
  BoostRqstRisingEdgeMDLOBJ1.step(&rtb_Ss_bst_rqst_on_uint, &rtb_Bs_bst_mode_rqst_uint);
  rtb_Bs_bst_mode_set = (rtb_Bs_bst_mode_rqst_uint && (SysMng_rtu_SysMng_In->Ss_tech_mode == SysMng_rtP.Constant1_Value_a));
  rtb_Bs_bst_mode_rst_o = ((rtb_Bs_bst_mode_rqst_uint || (((SysMng_rtu_SysMng_In->Ss_heat_pump_stt == SysMng_rtP.Constant4_Value) ||
    (SysMng_rtu_SysMng_In->Ss_heat_pump_stt == SysMng_rtP.Constant7_Value)) && ((SysMng_rtu_SysMng_In->Ss_elec_htr_bstr_stt == SysMng_rtP.Constant6_Value) ||
    (SysMng_rtu_SysMng_In->Ss_elec_htr_bstr_stt == SysMng_rtP.Constant8_Value)))) && (SysMng_DW.opModeDelay == SysMng_rtP.Constant2_Value_i));
  R_S_FlipFlopMDLOBJ2.step(&rtb_Bs_bst_mode_set, &rtb_Bs_bst_mode_rst_o, &SysMng_DW.stt_d);
}

// System reset for atomic system: '<S1>/antiLegionellaModeActivation'
void SysMng::antiLegionellaModeActivat_Reset()
{
  R_S_FlipFlopMDLOBJ3.reset();
}

// Output and update for atomic system: '<S1>/antiLegionellaModeActivation'
void SysMng::Sy_antiLegionellaModeActivation()
{
  bool rtb_Bs_anti_lgn_rst;
  bool rtb_Bs_anti_lgn_set;
  rtb_Bs_anti_lgn_set = ((SysMng_rtu_SysMng_In->Ss_anti_lgn_ena == SysMng_rtP.Constant_Value) && (SysMng_DW.conv2Days >= SysMng_rtu_SysMng_In->Ns_anti_lgn_day) &&
    ((SysMng_rtu_SysMng_In->Ss_oph_stt == SysMng_rtP.Constant1_Value) || (SysMng_rtu_SysMng_In->Ss_oph_mode_ena == SysMng_rtP.Constant2_Value)));
  rtb_Bs_anti_lgn_rst = (((SysMng_rtu_SysMng_In->Ss_heat_pump_stt == SysMng_rtP.Constant3_Value) || (SysMng_rtu_SysMng_In->Ss_heat_pump_stt ==
    SysMng_rtP.Constant5_Value)) && ((SysMng_rtu_SysMng_In->Ss_elec_htr_bstr_stt == SysMng_rtP.Constant4_Value_j) || (SysMng_rtu_SysMng_In->Ss_elec_htr_bstr_stt
    == SysMng_rtP.Constant6_Value_j)) && (SysMng_DW.opModeDelay == SysMng_rtP.Constant7_Value_h));
  R_S_FlipFlopMDLOBJ3.step(&rtb_Bs_anti_lgn_set, &rtb_Bs_anti_lgn_rst, &SysMng_DW.stt_a);
}

// System reset for atomic system: '<S1>/heatPumpTestModeActivation'
void SysMng::heatPumpTestModeActivatio_Reset()
{
  RisingEdgeDelayMDLOBJ7.reset();
  HeatPumpTestRqstRstRMDLOBJ5.reset();
  HeatPumpTestRqstRisiMDLOBJ4.reset();
  R_S_FlipFlopMDLOBJ6.reset();
}

// Output and update for atomic system: '<S1>/heatPumpTestModeActivation'
void SysMng::SysM_heatPumpTestModeActivation()
{
  // local block i/o variables
  uint8_t rtb_Out;
  bool rtb_Bs_heat_pump_test_rqst_rst_;
  bool rtb_Bs_heat_pump_test_set_uint;
  uint8_t rtb_Bs_heat_pump_test_rqst_ui_0;
  uint8_t rtb_Ss_op_mode_prev_uint;
  bool rtb_AND_HeatMode_rst;
  bool rtb_AND_HeatMode_set;
  bool rtb_Bs_test_mode_stt;
  rtb_Bs_test_mode_stt = (SysMng_DW.opModeDelay == SysMng_rtP.HeatPumpTestCstt_Value);
  rtb_Ss_op_mode_prev_uint = rtb_Bs_test_mode_stt;
  RisingEdgeDelayMDLOBJ7.step(&rtb_Ss_op_mode_prev_uint, &rtb_Out, SysMng_rtP.RisingEdgeDelay_delay, rtP_Control_tick_C);
  rtb_Bs_heat_pump_test_rqst_ui_0 = static_cast<uint8_t>(SysMng_rtu_SysMng_In->Ss_heat_pump_test_rqst);
  HeatPumpTestRqstRstRMDLOBJ5.step(&rtb_Bs_heat_pump_test_rqst_ui_0, &rtb_Bs_heat_pump_test_rqst_rst_);
  rtb_AND_HeatMode_rst = (((rtb_Out != 0) || rtb_Bs_heat_pump_test_rqst_rst_) && rtb_Bs_test_mode_stt);
  HeatPumpTestRqstRisiMDLOBJ4.step(&rtb_Bs_heat_pump_test_rqst_ui_0, &rtb_Bs_heat_pump_test_set_uint);
  rtb_AND_HeatMode_set = (rtb_Bs_heat_pump_test_set_uint && (!rtb_Bs_test_mode_stt));
  R_S_FlipFlopMDLOBJ6.step(&rtb_AND_HeatMode_set, &rtb_AND_HeatMode_rst, &SysMng_DW.stt_i);
}

// System reset for atomic system: '<S1>/holidaysModeActivation'
void SysMng::Sy_holidaysModeActivation_Reset()
{
  clkRchRisingEdgeMDLOBJ9.reset();
  hldyRqstRisingEdgeMDLOBJ10.reset();
  R_S_FlipFlopMDLOBJ8.reset();
}

// Output and update for atomic system: '<S1>/holidaysModeActivation'
void SysMng::SysMng_holidaysModeActivation()
{
  // local block i/o variables
  bool rtb_Bs_hldy_mode_rst_uint;
  bool rtb_Bs_hldy_mode_set_uint;
  uint8_t rtb_Bs_clk_rch_uint;
  uint8_t rtb_Bs_mode_chk_hldy_rqst_on_ui;
  bool rtb_Bs_hldy_mode;
  bool rtb_Bs_mode_chk_hldy_rqst_on_e;
  bool rtb_OR_Holidays_rst;
  rtb_Bs_hldy_mode = (SysMng_DW.opModeDelay == SysMng_rtP.Constant5_Value_p);
  rtb_Bs_clk_rch_uint = (SysMng_rtu_SysMng_In->Ss_hldy_rqst == SysMng_rtP.Constant4_Value_i);
  clkRchRisingEdgeMDLOBJ9.step(&rtb_Bs_clk_rch_uint, &rtb_Bs_hldy_mode_rst_uint);
  rtb_Bs_mode_chk_hldy_rqst_on_ui = (SysMng_rtu_SysMng_In->Ss_hldy_rqst == SysMng_rtP.Constant1_Value_k);
  hldyRqstRisingEdgeMDLOBJ10.step(&rtb_Bs_mode_chk_hldy_rqst_on_ui, &rtb_Bs_hldy_mode_set_uint);
  rtb_Bs_mode_chk_hldy_rqst_on_e = ((SysMng_rtu_SysMng_In->Ss_tech_mode != SysMng_rtP.Constant_Value_i) && (!rtb_Bs_hldy_mode) && rtb_Bs_hldy_mode_set_uint);
  rtb_OR_Holidays_rst = ((static_cast<uint8_t>(SysMng_DW.conv2Days + SysMng_rtP.OneMoreDay_Bias) >= SysMng_rtu_SysMng_In->Ns_hldy_nb) || (rtb_Bs_hldy_mode &&
    rtb_Bs_hldy_mode_rst_uint));
  R_S_FlipFlopMDLOBJ8.step(&rtb_Bs_mode_chk_hldy_rqst_on_e, &rtb_OR_Holidays_rst, &SysMng_DW.stt);
}

// Output and update for atomic system: '<S1>/smartGridModeActivation'
void SysMng::SysMng_smartGridModeActivation()
{
  SysMng_DW.isSgOn = (SysMng_rtu_SysMng_In->Ss_sg_stt == SysMng_rtP.Constant5_Value_a);
}

// System reset for atomic system: '<Root>/ConditionForModeSelection'
void SysMng::ConditionForModeSelection_Reset()
{
  SysMn_BoostModeActivation_Reset();
  antiLegionellaModeActivat_Reset();
  heatPumpTestModeActivatio_Reset();
  Sy_holidaysModeActivation_Reset();
}

// Output and update for atomic system: '<Root>/ConditionForModeSelection'
void SysMng::SysMn_ConditionForModeSelection()
{
  SysMng_BoostModeActivation();
  Sy_antiLegionellaModeActivation();
  SysM_heatPumpTestModeActivation();
  SysMng_holidaysModeActivation();
  SysMng_smartGridModeActivation();
}

// Output and update for atomic system: '<Root>/VentModeSelection'
void SysMng::SysMng_VentModeSelection() const
{
  if (SysMng_rtu_SysMng_In->Ss_sys_ver == SysMng_rtP.Collective_Value) {
    SysMng_rty_SysMng_Out->Ss_vent_op_mode = SysMng_rtP.Inoperative_Value;
  } else if (SysMng_rtu_SysMng_In->Ss_vent_sys_stop_stt == SysMng_rtP.on_Value) {
    SysMng_rty_SysMng_Out->Ss_vent_op_mode = SysMng_rtP.Stopped_Value;
  } else if (SysMng_rtu_SysMng_In->Bs_vent_pres_err) {
    SysMng_rty_SysMng_Out->Ss_vent_op_mode = SysMng_rtP.ConstantFlow_Value;
  } else {
    SysMng_rty_SysMng_Out->Ss_vent_op_mode = SysMng_rtP.Normal_Value;
  }
}

// System reset for atomic system: '<Root>/dayCounter'
void SysMng::SysMng_dayCounter_Reset()
{
  TimerMDLOBJ11.reset();
}

// Output and update for atomic system: '<Root>/dayCounter'
void SysMng::SysMng_dayCounter()
{
  // local block i/o variables
  float rtb_Timer_o1;
  bool rtb_TimerState;
  uint8_t rtb_bool2uint[2];
  rtb_bool2uint[0] = SysMng_DW.Bs_clk_on;
  rtb_bool2uint[1] = static_cast<uint8_t>(!SysMng_DW.Bs_clk_on);
  TimerMDLOBJ11.step(&rtb_bool2uint[0], &rtb_bool2uint[1], &rtb_bool2uint[1], &rtb_Timer_o1, &rtb_TimerState, SysMng_rtP.secTimer_sampleTime);
  SysMng_DW.conv2Days = static_cast<ta_time_day>(rtb_Timer_o1);
}

// Function for Chart: '<Root>/modeSelection'
void SysMng::Sys_enter_internal_HeatPumpUsed(void)
{
  if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::HeatPump) {
    SysMng_DW.is_HeatPumpUsed = SysMng_IN_UsualMode;
    SysMng_DW.is_UsualMode = SysMng_IN_HeatPumpMode;
    if (SysMng_rtu_SysMng_In->Ss_user_mode == te_user_mode::Auto) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_Auto;
      SysMng_DW.Ss_op_mode = te_op_mode::Auto;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_Eco;
      SysMng_DW.Ss_op_mode = te_op_mode::Eco;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    }
  } else {
    SysMng_DW.is_HeatPumpUsed = SysMng_IN_Hybrid;
    SysMng_DW.Ss_op_mode = te_op_mode::Hybrid;
    SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
    SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
  }
}

// Function for Chart: '<Root>/modeSelection'
void SysMng::SysMng_ModeSelection(void)
{
  if (SysMng_DW.AntiLegionnellaModeActDelay) {
    SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_ModeSelection = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_SystemMode = SysMng_IN_AntiLegionella;
    SysMng_DW.Ss_op_mode = te_op_mode::AntiLegionella;
    SysMng_DW.Bs_clk_on = false;
    if (SysMng_rtu_SysMng_In->Ss_tech_mode != te_tech_mode::FullElec) {
      SysMng_DW.is_AntiLegionella = SysMng_IN_AntiLegionella_normal;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else {
      SysMng_DW.is_AntiLegionella = SysM_IN_AntiLegionella_fullElec;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    }
  } else if (SysMng_DW.SmartGridActDelay) {
    SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_ModeSelection = SysMng_IN_NO_ACTIVE_CHILD;
    SysMng_DW.is_SystemMode = SysMng_IN_SmartGrid;
    SysMng_DW.Ss_op_mode = te_op_mode::SmartGrid;
    if (SysMng_rtu_SysMng_In->Ss_tech_mode != te_tech_mode::FullElec) {
      SysMng_DW.is_SmartGrid = SysMng_IN_SmartGrid_normal;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      SysMng_DW.is_SmartGrid = SysMng_IN_SmartGrid_fullElec;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    }
  } else {
    SysMng_DW.Bs_clk_on = ((SysMng_rtu_SysMng_In->Ss_anti_lgn_ena == te_on_off::on) && (SysMng_DW.Ss_op_mode != te_op_mode::Holidays));
    if (SysMng_DW.is_ModeSelection == SysMng_IN_FullElec) {
      if (SysMng_rtu_SysMng_In->Ss_tech_mode != te_tech_mode::FullElec) {
        SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
        Sys_enter_internal_HeatPumpUsed();
      } else {
        SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
        SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
        SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
      }

      // case IN_HeatPumpUsed:
    } else if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
      SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else if (SysMng_DW.HolidaysModeActDelay) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_ModeSelection = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_SystemMode = SysMng_IN_Holidays;
      SysMng_DW.Bs_clk_on = false;
      SysMng_DW.Ss_op_mode = te_op_mode::Holidays;
    } else if (SysMng_DW.is_HeatPumpUsed == SysMng_IN_Hybrid) {
      if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::HeatPump) {
        SysMng_DW.is_HeatPumpUsed = SysMng_IN_UsualMode;
        SysMng_DW.is_UsualMode = SysMng_IN_HeatPumpMode;
        if (SysMng_rtu_SysMng_In->Ss_user_mode == te_user_mode::Auto) {
          SysMng_DW.is_HeatPumpMode = SysMng_IN_Auto;
          SysMng_DW.Ss_op_mode = te_op_mode::Auto;
          SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
        } else {
          SysMng_DW.is_HeatPumpMode = SysMng_IN_Eco;
          SysMng_DW.Ss_op_mode = te_op_mode::Eco;
          SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
      } else {
        SysMng_DW.Ss_op_mode = te_op_mode::Hybrid;
        SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
        SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
      }

      // case IN_UsualMode:
    } else if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::Hybrid) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_HeatPumpUsed = SysMng_IN_Hybrid;
      SysMng_DW.Ss_op_mode = te_op_mode::Hybrid;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    } else if (SysMng_DW.is_UsualMode == SysMng_IN_Boost) {
      if (!SysMng_DW.bstModeActDelay) {
        SysMng_DW.is_UsualMode = SysMng_IN_HeatPumpMode;
        if (SysMng_rtu_SysMng_In->Ss_user_mode == te_user_mode::Auto) {
          SysMng_DW.is_HeatPumpMode = SysMng_IN_Auto;
          SysMng_DW.Ss_op_mode = te_op_mode::Auto;
          SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
        } else {
          SysMng_DW.is_HeatPumpMode = SysMng_IN_Eco;
          SysMng_DW.Ss_op_mode = te_op_mode::Eco;
          SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
      } else {
        SysMng_DW.Ss_op_mode = te_op_mode::Boost;
        SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
        SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
      }

      // case IN_HeatPumpMode:
    } else if (SysMng_DW.bstModeActDelay) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
      SysMng_DW.is_UsualMode = SysMng_IN_Boost;
      SysMng_DW.Ss_op_mode = te_op_mode::Boost;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else if (SysMng_DW.is_HeatPumpMode == SysMng_IN_Auto) {
      if (SysMng_rtu_SysMng_In->Ss_user_mode == te_user_mode::Eco) {
        SysMng_DW.is_HeatPumpMode = SysMng_IN_Eco;
        SysMng_DW.Ss_op_mode = te_op_mode::Eco;
        SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
        SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
      } else {
        SysMng_DW.Ss_op_mode = te_op_mode::Auto;
        SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
        SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
      }

      // case IN_Eco:
    } else if (SysMng_rtu_SysMng_In->Ss_user_mode == te_user_mode::Auto) {
      SysMng_DW.is_HeatPumpMode = SysMng_IN_Auto;
      SysMng_DW.Ss_op_mode = te_op_mode::Auto;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      SysMng_DW.Ss_op_mode = te_op_mode::Eco;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    }
  }
}

// System initialize for atomic system: '<Root>/modeSelection'
void SysMng::SysMng_modeSelection_Init()
{
  SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
}

// System reset for atomic system: '<Root>/modeSelection'
void SysMng::SysMng_modeSelection_Reset()
{
  SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
  SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
  SysMng_DW.Bs_clk_on = false;
  SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
  SysMng_DW.is_active_c3_SysMng = 0U;
  SysMng_DW.is_c3_SysMng = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_SystemMode = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_AntiLegionella = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_ModeSelection = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
  SysMng_DW.is_SmartGrid = SysMng_IN_NO_ACTIVE_CHILD;
}

// Output and update for atomic system: '<Root>/modeSelection'
void SysMng::SysMng_modeSelection()
{
  if (SysMng_DW.is_active_c3_SysMng == 0) {
    SysMng_DW.is_active_c3_SysMng = 1U;
    SysMng_DW.is_c3_SysMng = SysMng_IN_SystemMode;
    SysMng_DW.is_SystemMode = SysMng_IN_ModeSelection;
    if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
      SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
      SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
      Sys_enter_internal_HeatPumpUsed();
    }
  } else if (SysMng_DW.is_c3_SysMng == SysMng_IN_SystemMode) {
    if (SysMng_DW.heatPumpTestModeActDelay) {
      if (SysMng_DW.is_SystemMode == SysMng_IN_Holidays) {
        SysMng_DW.Bs_clk_on = false;
        SysMng_DW.is_SystemMode = SysMng_IN_NO_ACTIVE_CHILD;
      } else {
        SysMng_DW.is_AntiLegionella = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_HeatPumpMode = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_UsualMode = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_HeatPumpUsed = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_ModeSelection = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_SmartGrid = SysMng_IN_NO_ACTIVE_CHILD;
        SysMng_DW.is_SystemMode = SysMng_IN_NO_ACTIVE_CHILD;
      }

      SysMng_DW.is_c3_SysMng = SysMng_IN_TestMode;
      SysMng_DW.Ss_op_mode = te_op_mode::HeatPumpTest;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else {
      switch (SysMng_DW.is_SystemMode) {
       case SysMng_IN_AntiLegionella:
        if (!SysMng_DW.AntiLegionnellaModeActDelay) {
          SysMng_DW.is_AntiLegionella = SysMng_IN_NO_ACTIVE_CHILD;
          SysMng_DW.is_SystemMode = SysMng_IN_ModeSelection;
          if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
            SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
            SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
            SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
            SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
            Sys_enter_internal_HeatPumpUsed();
          }
        } else if (SysMng_DW.is_AntiLegionella == SysM_IN_AntiLegionella_fullElec) {
          SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
        } else {
          // case IN_AntiLegionella_normal:
          SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
        }
        break;

       case SysMng_IN_Holidays:
        if (!SysMng_DW.HolidaysModeActDelay) {
          SysMng_DW.Bs_clk_on = false;
          SysMng_DW.is_SystemMode = SysMng_IN_ModeSelection;
          if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
            SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
            SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
            SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
            SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
            Sys_enter_internal_HeatPumpUsed();
          }
        } else {
          SysMng_DW.Bs_clk_on = true;
          SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
          SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
        break;

       case SysMng_IN_ModeSelection:
        SysMng_ModeSelection();
        break;

       default:
        // case IN_SmartGrid:
        if (!SysMng_DW.SmartGridActDelay) {
          SysMng_DW.is_SmartGrid = SysMng_IN_NO_ACTIVE_CHILD;
          SysMng_DW.is_SystemMode = SysMng_IN_ModeSelection;
          if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
            SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
            SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
            SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
            SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
            Sys_enter_internal_HeatPumpUsed();
          }
        } else if (SysMng_DW.HolidaysModeActDelay) {
          SysMng_DW.is_SmartGrid = SysMng_IN_NO_ACTIVE_CHILD;
          SysMng_DW.is_SystemMode = SysMng_IN_Holidays;
          SysMng_DW.Bs_clk_on = false;
          SysMng_DW.Ss_op_mode = te_op_mode::Holidays;
        } else {
          SysMng_DW.Ss_op_mode = te_op_mode::SmartGrid;
          if (SysMng_DW.is_SmartGrid == SysMng_IN_SmartGrid_fullElec) {
            SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
            SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            // case IN_SmartGrid_normal:
            SysMng_DW.Ss_heat_pump_ena = te_on_off::on;
            SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          }
        }
        break;
      }
    }

    // case IN_TestMode:
  } else if (!SysMng_DW.heatPumpTestModeActDelay) {
    SysMng_DW.is_c3_SysMng = SysMng_IN_SystemMode;
    SysMng_DW.is_SystemMode = SysMng_IN_ModeSelection;
    if (SysMng_rtu_SysMng_In->Ss_tech_mode == te_tech_mode::FullElec) {
      SysMng_DW.is_ModeSelection = SysMng_IN_FullElec;
      SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
      SysMng_DW.Ss_heat_pump_ena = te_on_off::off;
      SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      SysMng_DW.is_ModeSelection = SysMng_IN_HeatPumpUsed;
      Sys_enter_internal_HeatPumpUsed();
    }
  } else {
    SysMng_DW.Ss_op_mode = te_op_mode::HeatPumpTest;
    SysMng_DW.Ss_heat_pump_ena = te_on_off::forced;
    SysMng_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
  }
}

// System initialize for referenced model: 'SysMng'
void SysMng::init(void)
{
  SysMng_DW.AntiLegionnellaModeActDelay_DST = SysMng_rtP.AntiLegionnellaModeActDelay_Ini;
  SysMng_DW.HolidaysModeActDelay_DSTATE = SysMng_rtP.HolidaysModeActDelay_InitialCon;
  SysMng_DW.SmartGridActDelay_DSTATE = SysMng_rtP.SmartGridActDelay_InitialCondit;
  SysMng_DW.bstModeActDelay_DSTATE = SysMng_rtP.bstModeActDelay_InitialConditio;
  SysMng_DW.heatPumpTestModeActDelay_DSTATE = SysMng_rtP.heatPumpTestModeActDelay_Initia;
  SysMng_DW.opModeDelay_DSTATE = SysMng_rtP.opModeDelay_InitialCondition;
  SysMng_modeSelection_Init();
}

// System reset for referenced model: 'SysMng'
void SysMng::reset(void)
{
  SysMng_DW.AntiLegionnellaModeActDelay_DST = SysMng_rtP.AntiLegionnellaModeActDelay_Ini;
  SysMng_DW.HolidaysModeActDelay_DSTATE = SysMng_rtP.HolidaysModeActDelay_InitialCon;
  SysMng_DW.SmartGridActDelay_DSTATE = SysMng_rtP.SmartGridActDelay_InitialCondit;
  SysMng_DW.bstModeActDelay_DSTATE = SysMng_rtP.bstModeActDelay_InitialConditio;
  SysMng_DW.heatPumpTestModeActDelay_DSTATE = SysMng_rtP.heatPumpTestModeActDelay_Initia;
  SysMng_DW.opModeDelay_DSTATE = SysMng_rtP.opModeDelay_InitialCondition;
  SysMng_modeSelection_Reset();
  SysMng_dayCounter_Reset();
  ConditionForModeSelection_Reset();
}

// Output and update for referenced model: 'SysMng'
void SysMng::step(const tb_SysMng_In *rtu_SysMng_In, tb_SysMng_Out *rty_SysMng_Out)
{
  SysMng_rtu_SysMng_In = rtu_SysMng_In;
  SysMng_rty_SysMng_Out = rty_SysMng_Out;
  SysMng_DW.AntiLegionnellaModeActDelay = SysMng_DW.AntiLegionnellaModeActDelay_DST;
  SysMng_DW.HolidaysModeActDelay = SysMng_DW.HolidaysModeActDelay_DSTATE;
  SysMng_DW.SmartGridActDelay = SysMng_DW.SmartGridActDelay_DSTATE;
  SysMng_DW.bstModeActDelay = SysMng_DW.bstModeActDelay_DSTATE;
  SysMng_DW.heatPumpTestModeActDelay = SysMng_DW.heatPumpTestModeActDelay_DSTATE;
  SysMng_modeSelection();
  SysMng_dayCounter();
  SysMng_DW.opModeDelay = SysMng_DW.opModeDelay_DSTATE;
  SysMn_ConditionForModeSelection();
  SysMng_VentModeSelection();
  rty_SysMng_Out->Ss_op_mode = SysMng_DW.Ss_op_mode;
  rty_SysMng_Out->Ss_heat_pump_ena = SysMng_DW.Ss_heat_pump_ena;
  rty_SysMng_Out->Ss_elec_bstr_htr_ena = SysMng_DW.Ss_elec_bstr_htr_ena;
  SysMng_DW.AntiLegionnellaModeActDelay_DST = SysMng_DW.stt_a;
  SysMng_DW.HolidaysModeActDelay_DSTATE = SysMng_DW.stt;
  SysMng_DW.SmartGridActDelay_DSTATE = SysMng_DW.isSgOn;
  SysMng_DW.bstModeActDelay_DSTATE = SysMng_DW.stt_d;
  SysMng_DW.heatPumpTestModeActDelay_DSTATE = SysMng_DW.stt_i;
  SysMng_DW.opModeDelay_DSTATE = SysMng_DW.Ss_op_mode;
}

// Model initialize function
void SysMng::initialize()
{
  // Registration code

  // states (dwork)
  {
    SysMng_DW.opModeDelay = te_op_mode::FullElec;
    SysMng_DW.Ss_op_mode = te_op_mode::FullElec;
  }
}

// Constructor
SysMng::SysMng():
  SysMng_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
SysMng::~SysMng() = default;

//
// File trailer for generated code.
//
// [EOF]
//
