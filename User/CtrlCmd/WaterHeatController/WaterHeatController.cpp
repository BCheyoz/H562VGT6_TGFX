//
// File: WaterHeatController.cpp
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.570
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:49 2025
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
#include "WaterHeatController.h"
#include "WaterHeatController_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "WaterHeatController_private.h"
#include "look1_is16lu32n16Ds32_binlcs.h"
#include "plook_u32s16_bincka.h"
#include "look1_is16lu32n16tu16_binlcse.h"
#include "PID_backCalc.h"
#include "Derivator.h"
#include "firstOrderTF.h"
#include "Hysteresis.h"
#include "RisingEdgeDelay.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDetect.h"
#include "VariableRisingEdgeDelay.h"
#include "Timer.h"
#include "cntrlSaturation.h"

// Named constants for Chart: '<S4>/TankLevelStates'
const uint8_t WaterHeatControll_IN_Tank_empty{ 2U };

const uint8_t WaterHeatControlle_IN_Tank_Full{ 1U };

const uint8_t WaterHeatControlle_IN_Tank_high{ 3U };

const uint8_t WaterHeatController_IN_Tank_low{ 4U };

const bool WaterHeatController_BGND{ false };// bool ground

WaterHeatController::P_WaterHeatController_T WaterHeatController::WaterHeatController_rtP{
  // Mask Parameter: firstOrderTF2_K
  //  Referenced by: '<S23>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOrderTF1_K
  //  Referenced by: '<S22>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt_K
  //  Referenced by: '<S19>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt1_K
  //  Referenced by: '<S20>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOrderTF2_Tau
  //  Referenced by: '<S23>/firstOrderTF'

  10.0F,

  // Mask Parameter: firstOrderTF1_Tau
  //  Referenced by: '<S22>/firstOrderTF'

  10.0F,

  // Mask Parameter: pumpFreqFilt_Tau
  //  Referenced by: '<S19>/firstOrderTF'

  20.0F,

  // Mask Parameter: pumpFreqFilt1_Tau
  //  Referenced by: '<S20>/firstOrderTF'

  20.0F,

  // Mask Parameter: Derivator_initVal
  //  Referenced by: '<S18>/Derivator'

  0.0F,

  // Mask Parameter: firstOrderTF2_initVal
  //  Referenced by: '<S23>/firstOrderTF'

  0.0F,

  // Mask Parameter: firstOrderTF1_initVal
  //  Referenced by: '<S22>/firstOrderTF'

  0.0F,

  // Mask Parameter: pumpFreqFilt_initVal
  //  Referenced by: '<S19>/firstOrderTF'

  0.0F,

  // Mask Parameter: pumpFreqFilt1_initVal
  //  Referenced by: '<S20>/firstOrderTF'

  0.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_of
  //  Referenced by: '<S6>/Hysteresis'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_on
  //  Referenced by: '<S6>/Hysteresis'

  0.0F,

  // Mask Parameter: Derivator_sampleTime
  //  Referenced by: '<S18>/Derivator'

  120.0F,

  // Mask Parameter: firstOrderTF2_sampleTime
  //  Referenced by: '<S23>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOrderTF1_sampleTime
  //  Referenced by: '<S22>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt_sampleTime
  //  Referenced by: '<S19>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt1_sampleTime
  //  Referenced by: '<S20>/firstOrderTF'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_sw
  //  Referenced by: '<S6>/Hysteresis'

  80,

  // Mask Parameter: HysteresisProtectTankDefrost__g
  //  Referenced by: '<S6>/Hysteresis'

  120,

  // Expression: 1
  //  Referenced by: '<S3>/Constant5'

  1.0,

  // Expression: -100/120
  //  Referenced by: '<S8>/Constant'

  -0.83333333333333337,

  // Expression: -12/120
  //  Referenced by: '<S8>/Constant2'

  -0.1,

  // Expression: 0
  //  Referenced by: '<S8>/Constant1'

  0.0,

  // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
  //  Referenced by: '<S1>/Cs_ctrl_tref_max_spd_cmd'

  0.0F,

  // Computed Parameter: TevapRegulator_InitVal
  //  Referenced by: '<S1>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Kawu
  //  Referenced by: '<S1>/TevapRegulator'

  1.0F,

  // Computed Parameter: TevapRegulator_Kd
  //  Referenced by: '<S1>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Ki
  //  Referenced by: '<S1>/TevapRegulator'

  0.02F,

  // Computed Parameter: TevapRegulator_Kp
  //  Referenced by: '<S1>/TevapRegulator'

  0.5F,

  // Computed Parameter: TevapRegulator_Tau_f
  //  Referenced by: '<S1>/TevapRegulator'

  5.0F,

  // Computed Parameter: TdownminReg_InitVal
  //  Referenced by: '<S2>/TdownminReg'

  0.0F,

  // Computed Parameter: TdownminReg_Kawu
  //  Referenced by: '<S2>/TdownminReg'

  1.0F,

  // Computed Parameter: TdownminReg_Kd
  //  Referenced by: '<S2>/TdownminReg'

  0.0F,

  // Computed Parameter: TdownminReg_Ki
  //  Referenced by: '<S2>/TdownminReg'

  0.1F,

  // Computed Parameter: TdownminReg_Kp
  //  Referenced by: '<S2>/TdownminReg'

  1.0F,

  // Computed Parameter: TdownminReg_Tau_f
  //  Referenced by: '<S2>/TdownminReg'

  5.0F,

  // Computed Parameter: Cs_tank_down_temp_der_Y0
  //  Referenced by: '<S15>/Cs_tank_down_temp_der'

  0.0F,

  // Computed Parameter: Saturation_UpperSat
  //  Referenced by: '<S9>/Saturation'

  3900.0F,

  // Computed Parameter: Saturation_LowerSat
  //  Referenced by: '<S9>/Saturation'

  1200.0F,

  // Computed Parameter: Saturation1_UpperSat
  //  Referenced by: '<S9>/Saturation1'

  3900.0F,

  // Computed Parameter: Saturation1_LowerSat
  //  Referenced by: '<S9>/Saturation1'

  1200.0F,

  // Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
  //  Referenced by: '<S5>/Cs_ctrl_temp_evap_spd_cmd'

  0.0F,

  // Computed Parameter: TevapRegulator_InitVal_l
  //  Referenced by: '<S5>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Kawu_e
  //  Referenced by: '<S5>/TevapRegulator'

  1.0F,

  // Computed Parameter: TevapRegulator_Kd_l
  //  Referenced by: '<S5>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Ki_c
  //  Referenced by: '<S5>/TevapRegulator'

  0.003F,

  // Computed Parameter: TevapRegulator_Kp_h
  //  Referenced by: '<S5>/TevapRegulator'

  0.01F,

  // Computed Parameter: TevapRegulator_Tau_f_j
  //  Referenced by: '<S5>/TevapRegulator'

  5.0F,

  // Computed Parameter: evapTempProtDelay_Delay
  //  Referenced by: '<S3>/evapTempProtDelay'

  7200.0F,

  // Computed Parameter: evapTempProtDelay_TimeStep
  //  Referenced by: '<S3>/evapTempProtDelay'

  1.0F,

  // Computed Parameter: UnitDelay1_InitialCondition
  //  Referenced by: '<S3>/Unit Delay1'

  0.0F,

  // Computed Parameter: evapTempProtDisable_Delay_Delay
  //  Referenced by: '<S3>/evapTempProtDisable_Delay'

  600.0F,

  // Computed Parameter: evapTempProtDisable_Delay_TimeS
  //  Referenced by: '<S3>/evapTempProtDisable_Delay'

  1.0F,

  // Expression: te_pump_mode.Tref
  //  Referenced by: '<S9>/TrefReg'

  te_pump_mode::Tref,

  // Expression: te_pump_mode.Tevap
  //  Referenced by: '<S9>/TevapReg'

  te_pump_mode::Tevap,

  // Expression: te_pump_mode.Tank_lvl
  //  Referenced by: '<S9>/TankLevelReg'

  te_pump_mode::Tank_lvl,

  // Expression: Cs_temp_ref_tol_C
  //  Referenced by: '<S1>/Constant2'

  30,

  // Computed Parameter: Temp_10C_SetPointPID_Value
  //  Referenced by: '<S2>/Temp_10°C_SetPointPID'

  100,

  // Expression: Ct_tank_down_temp_pump_off_bp_C
  //  Referenced by: '<S3>/TimeToPumpStartup'

  { 100, 250, 260, 490, 500, 700 },

  // Expression: Cs_temp_tank_high_C
  //  Referenced by: '<S12>/Constant8'

  540,

  // Expression: Cs_temp_tank_full_C
  //  Referenced by: '<S12>/Constant6'

  540,

  // Expression: Cs_temp_tank_low_C
  //  Referenced by: '<S12>/Constant3'

  380,

  // Computed Parameter: Temp_2C_SetPointPID_Value
  //  Referenced by: '<S5>/Temp_-2°C_SetPointPID'

  -20,

  // Expression: Ct_temp_ref_max_data_C
  //  Referenced by: '<S3>/Tref_max'

  { 750, 750, 750, 750, 750, 750, 750 },

  // Expression: Ct_temp_evap_bp_C
  //  Referenced by: '<S3>/Tref_max'

  { 200, 300, 400, 500, 600, 650, 700 },

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S3>/Constant4'

  -20,

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S3>/Constant6'

  -20,

  // Expression: Cs_temp_tank_empty_C
  //  Referenced by: '<S12>/Constant4'

  300,

  // Expression: Cs_temp_up_tank_low_C
  //  Referenced by: '<S12>/Constant5'

  470,

  // Expression: Cs_temp_tank_full_C
  //  Referenced by: '<S12>/Constant2'

  540,

  // Expression: Ct_temp_tank_cold_sp_bp_C
  //  Referenced by: '<S10>/CartoSetPointTankCold'

  { 150, 200, 250, 300, 350, 360, 450, 500 },

  // Computed Parameter: CartoSetPointTankCold_low_table
  //  Referenced by: '<S10>/CartoSetPointTankCold_low'

  { 3900, 3600, 1200, 1200, 1200, 1200, 1200, 1200 },

  // Expression: Ct_temp_tank_cold_sp_bp_C
  //  Referenced by: '<S10>/CartoSetPointTankCold_low'

  { 150, 200, 250, 300, 350, 360, 450, 500 },

  // Computed Parameter: Constant3_Value_a
  //  Referenced by: '<S14>/Constant3'

  0,

  // Computed Parameter: UnitDelay1_InitialCondition_n
  //  Referenced by: '<S14>/Unit Delay1'

  0,

  // Computed Parameter: max_Value
  //  Referenced by: '<S1>/max'

  100U,

  // Computed Parameter: maxDerat_Value
  //  Referenced by: '<S1>/maxDerat'

  1U,

  // Computed Parameter: minDerat_Value
  //  Referenced by: '<S1>/minDerat'

  0U,

  // Computed Parameter: Cs_ctrl_temp_tdwn_spd_cmd_Y0
  //  Referenced by: '<S2>/Cs_ctrl_temp_tdwn_spd_cmd'

  0U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S2>/SpdMinPIDTdownTank'

  1200U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S2>/SpdMaxPIDTdownTank'

  3900U,

  // Expression: Ct_time_cntr_pump_off_data_C
  //  Referenced by: '<S3>/TimeToPumpStartup'

  { 300U, 300U, 360U, 1740U, 1800U, 1800U },

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S9>/SpdOFF'

  0U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S10>/SpdMax'

  3900U,

  // Expression: Cs_v40_tank_low_C
  //  Referenced by: '<S12>/Constant1'

  800U,

  // Computed Parameter: max_Value_i
  //  Referenced by: '<S5>/max'

  100U,

  // Computed Parameter: maxDerat_Value_f
  //  Referenced by: '<S5>/maxDerat'

  100U,

  // Computed Parameter: minDerat_Value_h
  //  Referenced by: '<S5>/minDerat'

  0U,

  // Computed Parameter: Constant7_Value
  //  Referenced by: '<S12>/Constant7'

  0U,

  // Computed Parameter: isV40Treshold_Threshold
  //  Referenced by: '<S12>/isV40Treshold'

  0U,

  // Computed Parameter: UnitDelay_InitialCondition
  //  Referenced by: '<S4>/Unit Delay'

  0U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S4>/Constant'

  3900U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S4>/Constant1'

  1200U,

  // Expression: Ct_rpm_pump_sp_tank_cold_C
  //  Referenced by: '<S10>/CartoSetPointTankCold'

  { 3900U, 3600U, 1200U, 1200U, 1200U, 1200U, 1200U, 1200U },

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S10>/SpdOFF'

  0U,

  // Computed Parameter: SpdOFF_Value_e
  //  Referenced by: '<S3>/SpdOFF'

  0U,

  // Computed Parameter: UnitDelay_InitialCondition_m
  //  Referenced by: '<S3>/Unit Delay'

  300U,

  // Expression: false
  //  Referenced by: '<S3>/exhstTempProt'

  false,

  // Expression: false
  //  Referenced by: '<S3>/Constant3'

  false,

  // Expression: false
  //  Referenced by: '<S3>/antiShort'

  false,

  // Computed Parameter: prevTimerVal_InitialCondition
  //  Referenced by: '<S14>/prevTimerVal'

  false,

  // Computed Parameter: Zero_Value
  //  Referenced by: '<S3>/Zero'

  0U,

  // Computed Parameter: Constant4_Value_e
  //  Referenced by: '<S14>/Constant4'

  0U
};

// System initialize for referenced model: 'WaterHeatController'
void WaterHeatController::init(void)
{
  WaterHeatController_DW.exhstTempProt_DSTATE = WaterHeatController_rtP.exhstTempProt_InitialCondition;
  WaterHeatController_DW.antiShort_DSTATE = WaterHeatController_rtP.antiShort_InitialCondition;
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_rtP.UnitDelay1_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE = WaterHeatController_rtP.UnitDelay_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE_a = WaterHeatController_rtP.UnitDelay_InitialCondition_m;
  WaterHeatController_DW.prevTimerVal_DSTATE = WaterHeatController_rtP.prevTimerVal_InitialCondition;
  WaterHeatController_DW.UnitDelay1_DSTATE_e = WaterHeatController_rtP.UnitDelay1_InitialCondition_n;
  TevapRegulatorMDLOBJ1.init(WaterHeatController_rtP.TevapRegulator_InitVal);
  WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd = WaterHeatController_rtP.Cs_ctrl_tref_max_spd_cmd_Y0;
  TdownminRegMDLOBJ2.init(WaterHeatController_rtP.TdownminReg_InitVal);
  TevapRegulatorMDLOBJ18.init(WaterHeatController_rtP.TevapRegulator_InitVal_l);
  WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd = WaterHeatController_rtP.Cs_ctrl_temp_evap_spd_cmd_Y0;
  DerivatorMDLOBJ11.init(WaterHeatController_rtP.Derivator_initVal);
  WaterHeatController_DW.Derivator_h = WaterHeatController_rtP.Cs_tank_down_temp_der_Y0;
  firstOrderTFMDLOBJ17.init(WaterHeatController_rtP.firstOrderTF2_initVal);
  firstOrderTFMDLOBJ16.init(WaterHeatController_rtP.firstOrderTF1_initVal);
  firstOrderTFMDLOBJ13.init(WaterHeatController_rtP.pumpFreqFilt_initVal);
  firstOrderTFMDLOBJ14.init(WaterHeatController_rtP.pumpFreqFilt1_initVal);
  HysteresisMDLOBJ4.init(WaterHeatController_rtP.HysteresisProtectTankDefrost_of);
}

// System reset for referenced model: 'WaterHeatController'
void WaterHeatController::reset(void)
{
  WaterHeatController_DW.exhstTempProt_DSTATE = WaterHeatController_rtP.exhstTempProt_InitialCondition;
  WaterHeatController_DW.antiShort_DSTATE = WaterHeatController_rtP.antiShort_InitialCondition;
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_rtP.UnitDelay1_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE = WaterHeatController_rtP.UnitDelay_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE_a = WaterHeatController_rtP.UnitDelay_InitialCondition_m;
  WaterHeatController_DW.prevTimerVal_DSTATE = WaterHeatController_rtP.prevTimerVal_InitialCondition;
  WaterHeatController_DW.UnitDelay1_DSTATE_e = WaterHeatController_rtP.UnitDelay1_InitialCondition_n;
  evapTempProtDelayMDLOBJ7.reset();
  evapTempProtDisable_MDLOBJ8.reset();
  R_S_FlipFlopMDLOBJ6.reset();
  WaterHeatController_DW.temporalCounter_i1 = 0U;
  WaterHeatController_DW.is_active_c3_WaterHeatControlle = 0U;
  ModelMDLOBJ10.reset();
  R_S_FlipFlopMDLOBJ12.reset();
  WaterHeatController_DW.is_active_c2_WaterHeatControlle = 0U;
  WaterHeatController_DW.is_c2_WaterHeatController = 0;
  firstOrderTFMDLOBJ17.reset(WaterHeatController_rtP.firstOrderTF2_initVal);
  firstOrderTFMDLOBJ16.reset(WaterHeatController_rtP.firstOrderTF1_initVal);
  firstOrderTFMDLOBJ13.reset(WaterHeatController_rtP.pumpFreqFilt_initVal);
  firstOrderTFMDLOBJ14.reset(WaterHeatController_rtP.pumpFreqFilt1_initVal);
  PumpOFFdetectMDLOBJ5.reset();
  AntiShortCycleMDLOBJ3.reset();
  HysteresisMDLOBJ4.reset(WaterHeatController_rtP.HysteresisProtectTankDefrost_of);
  TimerMDLOBJ9.reset();
}

// Output and update for referenced model: 'WaterHeatController'
void WaterHeatController::step(const ta_wtr_vol *rtu_Cs_v40_min, const ta_temp *rtu_Cs_tank_down_temp, const ta_temp *rtu_Cs_tank_up_temp, const ta_wtr_vol
  *rtu_Cs_v40_sp, const ta_temp *rtu_Cs_pump_evap_temp, const ta_temp *rtu_Cs_pump_xhst_temp, ta_rot_spd *rty_Cs_heat_pump_rot_spd_sp, uint8_t *rty_Cs_v40_rat)
{
  // local block i/o variables
  float rtb_output;
  float rtb_output_k;
  float rtb_Saturation_o1;
  float rtb_Saturation_o2;
  float rtb_output_a;
  float rtb_output_e;
  float rtb_Out;
  float rtb_Timer_o1;
  float rtb_final_cmd;
  float rtb_ovrFlwVal;
  float rtb_final_cmd_p;
  float rtb_ovrFlwVal_m;
  float rtb_final_cmd_pm;
  float rtb_ovrFlwVal_l;
  uint8_t rtb_Out_a;
  uint8_t rtb_Out_c;
  uint8_t rtb_Out_h;
  bool rtb_stt;
  bool rtb_Out_cb;
  bool rtb_stt_l;
  bool rtb_Out_c1;
  bool rtb_TimerState;
  float rtb_DataTypeConversion;
  float rtb_DataTypeConversion1;
  float rtb_DataTypeConversion2;
  float rtb_DataTypeConversion3;
  float rtb_DataTypeConversion5;
  float rtb_Product;
  float rtb_Product1;
  float rtb_SpdMaxPID;
  float rtb_SpdMinPID;
  float rtb_Switch4;
  float rtb_TdwnToSingle;
  float rtb_delay;
  float rtb_maxDerat;
  float rtb_maxDerat_f;
  float rtb_mesTdownTank;
  float rtb_mesTevap;
  float rtb_mesTpumpXhst;
  float rtb_minDerat;
  float rtb_minDerat_b;
  float rtb_setPointTankDownTemp;
  float rtb_setPointTevap;
  float rtb_setPointTrefMax;
  int32_t tmp;
  uint32_t bpIdx;
  ta_rot_spd rtb_Cs_tank_lvl_spd_pump_sp;
  ta_temp rtb_Switch_d;
  ta_temp rtb_Tref_max;
  ta_temp rtb_derivUpDate1;
  ta_temp rtb_diff;
  uint16_t rtb_Divide_0;
  uint16_t rtb_isAntiShortCycleDelay;
  uint8_t rtb_DataTypeConversion1_c;
  uint8_t rtb_DataTypeConversion2_m;
  uint8_t rtb_DataTypeConversion3_i;
  uint8_t rtb_DataTypeConversion4;
  uint8_t rtb_DataTypeConversion_c;
  uint8_t rtb_DataTypeConversion_o;
  bool rtb_Bs_soft_draw;
  bool rtb_Bs_tank_empty;
  bool rtb_Bs_tank_empty_f;
  bool rtb_Bs_tank_full;
  bool rtb_Bs_tank_low;
  bool rtb_DataTypeConversion5_k;
  bool rtb_DataTypeConversion6;
  bool rtb_Equal;
  bool rtb_ProtectPump;
  bool rtb_RelationalOperator;
  bool rtb_RelationalOperator1;
  bool rtb_RelationalOperator1_i;
  bool rtb_RelationalOperator2_j;
  bool rtb_RelationalOperator3;
  bool rtb_RelationalOperator4;
  bool rtb_RelationalOperator5;
  bool rtb_RelationalOperator6;
  bool rtb_isNoDiff;
  bool rtb_isV40Treshold;
  te_pump_mode tmp_0;
  te_tank_lvl rtb_TankLevel;
  rtb_Tref_max = look1_is16lu32n16Ds32_binlcs(*rtu_Cs_pump_evap_temp, WaterHeatController_rtP.Tref_max_bp01Data, WaterHeatController_rtP.Tref_max_tableData, 6U);
  if (!WaterHeatController_DW.exhstTempProt_DSTATE) {
    rtb_setPointTrefMax = static_cast<int16_t>(rtb_Tref_max - WaterHeatController_rtP.Constant2_Value_c);
    rtb_mesTpumpXhst = *rtu_Cs_pump_xhst_temp;
    rtb_maxDerat_f = WaterHeatController_rtP.maxDerat_Value;
    rtb_minDerat_b = WaterHeatController_rtP.minDerat_Value;
    TevapRegulatorMDLOBJ1.step(&rtb_setPointTrefMax, &rtb_mesTpumpXhst, &rtb_maxDerat_f, &rtb_minDerat_b, (const_cast<bool*>(&WaterHeatController_BGND)),
      &rtb_final_cmd_pm, &rtb_ovrFlwVal_l, WaterHeatController_rtP.TevapRegulator_InitVal, WaterHeatController_rtP.TevapRegulator_Kawu,
      WaterHeatController_rtP.TevapRegulator_Kd, WaterHeatController_rtP.TevapRegulator_Ki, WaterHeatController_rtP.TevapRegulator_Kp, rtP_WaterHeatCtrl_tick_C,
      WaterHeatController_rtP.TevapRegulator_Tau_f);
    WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd = (static_cast<float>(WaterHeatController_rtP.max_Value) - rtb_final_cmd_pm) / static_cast<float>
      (WaterHeatController_rtP.max_Value);
  }

  if (WaterHeatController_rtP.Constant3_Value_g) {
    rtb_setPointTankDownTemp = WaterHeatController_rtP.Temp_10C_SetPointPID_Value;
    rtb_mesTdownTank = *rtu_Cs_tank_down_temp;
    rtb_SpdMinPID = WaterHeatController_rtP.SpdMinPIDTdownTank_Value;
    rtb_SpdMaxPID = WaterHeatController_rtP.SpdMaxPIDTdownTank_Value;
    TdownminRegMDLOBJ2.step(&rtb_setPointTankDownTemp, &rtb_mesTdownTank, &rtb_SpdMaxPID, &rtb_SpdMinPID, (const_cast<bool*>(&WaterHeatController_BGND)),
      &rtb_final_cmd_p, &rtb_ovrFlwVal_m, WaterHeatController_rtP.TdownminReg_InitVal, WaterHeatController_rtP.TdownminReg_Kawu,
      WaterHeatController_rtP.TdownminReg_Kd, WaterHeatController_rtP.TdownminReg_Ki, WaterHeatController_rtP.TdownminReg_Kp, rtP_WaterHeatCtrl_tick_C,
      WaterHeatController_rtP.TdownminReg_Tau_f);
  }

  rtb_ProtectPump = (*rtu_Cs_pump_xhst_temp >= rtb_Tref_max);
  rtb_RelationalOperator = (*rtu_Cs_pump_evap_temp < WaterHeatController_rtP.Constant4_Value);
  rtb_DataTypeConversion3_i = rtb_RelationalOperator;
  evapTempProtDelayMDLOBJ7.step(&rtb_DataTypeConversion3_i, &rtb_Out_a, WaterHeatController_rtP.evapTempProtDelay_Delay,
    WaterHeatController_rtP.evapTempProtDelay_TimeStep);
  rtb_DataTypeConversion5_k = (rtb_Out_a != 0);
  rtb_RelationalOperator3 = (*rtu_Cs_pump_evap_temp >= WaterHeatController_rtP.Constant6_Value_h);
  rtb_DataTypeConversion4 = ((WaterHeatController_DW.UnitDelay1_DSTATE >= WaterHeatController_rtP.Constant5_Value) && rtb_RelationalOperator3);
  evapTempProtDisable_MDLOBJ8.step(&rtb_DataTypeConversion4, &rtb_Out_c, WaterHeatController_rtP.evapTempProtDisable_Delay_Delay,
    WaterHeatController_rtP.evapTempProtDisable_Delay_TimeS);
  rtb_DataTypeConversion6 = (rtb_Out_c != 0);
  R_S_FlipFlopMDLOBJ6.step(&rtb_DataTypeConversion5_k, &rtb_DataTypeConversion6, &rtb_stt);
  if (rtb_stt) {
    rtb_setPointTevap = WaterHeatController_rtP.Temp_2C_SetPointPID_Value;
    rtb_mesTevap = *rtu_Cs_pump_evap_temp;
    rtb_maxDerat = WaterHeatController_rtP.maxDerat_Value_f;
    rtb_minDerat = WaterHeatController_rtP.minDerat_Value_h;
    TevapRegulatorMDLOBJ18.step(&rtb_setPointTevap, &rtb_mesTevap, &rtb_maxDerat, &rtb_minDerat, (const_cast<bool*>(&WaterHeatController_BGND)), &rtb_final_cmd,
      &rtb_ovrFlwVal, WaterHeatController_rtP.TevapRegulator_InitVal_l, WaterHeatController_rtP.TevapRegulator_Kawu_e,
      WaterHeatController_rtP.TevapRegulator_Kd_l, WaterHeatController_rtP.TevapRegulator_Ki_c, WaterHeatController_rtP.TevapRegulator_Kp_h,
      rtP_WaterHeatCtrl_tick_C, WaterHeatController_rtP.TevapRegulator_Tau_f_j);
    WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd = (static_cast<float>(WaterHeatController_rtP.max_Value_i) - rtb_final_cmd) / static_cast<float>
      (WaterHeatController_rtP.max_Value_i);
  }

  rtb_Bs_tank_empty = (*rtu_Cs_tank_up_temp < WaterHeatController_rtP.Constant4_Value_p);
  if (WaterHeatController_DW.temporalCounter_i1 < 120) {
    WaterHeatController_DW.temporalCounter_i1 = static_cast<uint8_t>(WaterHeatController_DW.temporalCounter_i1 + 1);
  }

  if (WaterHeatController_DW.is_active_c3_WaterHeatControlle == 0) {
    WaterHeatController_DW.is_active_c3_WaterHeatControlle = 1U;
    WaterHeatController_DW.temporalCounter_i1 = 0U;
  } else if (WaterHeatController_DW.temporalCounter_i1 == 120) {
    rtb_DataTypeConversion = *rtu_Cs_tank_down_temp;
    DerivatorMDLOBJ11.step(&rtb_DataTypeConversion, &WaterHeatController_DW.Derivator_h, WaterHeatController_rtP.Derivator_sampleTime);
  }

  if (WaterHeatController_DW.temporalCounter_i1 == 120) {
    WaterHeatController_DW.temporalCounter_i1 = 0U;
  }

  rtb_Bs_tank_empty_f = (rtb_Bs_tank_empty || (WaterHeatController_DW.Derivator_h <= WaterHeatController_rtP.Constant_Value));
  if (WaterHeatController_rtP.Constant7_Value > WaterHeatController_rtP.isV40Treshold_Threshold) {
    rtb_isV40Treshold = (*rtu_Cs_v40_min < WaterHeatController_rtP.Constant1_Value_c);
  } else {
    rtb_isV40Treshold = (*rtu_Cs_tank_down_temp < WaterHeatController_rtP.Constant3_Value);
  }

  rtb_RelationalOperator4 = (*rtu_Cs_tank_up_temp < WaterHeatController_rtP.Constant5_Value_n);
  rtb_Bs_tank_low = (rtb_isV40Treshold || rtb_RelationalOperator4);
  rtb_RelationalOperator5 = (*rtu_Cs_tank_up_temp >= WaterHeatController_rtP.Constant2_Value_i);
  rtb_RelationalOperator2_j = (*rtu_Cs_tank_down_temp >= WaterHeatController_rtP.Constant2_Value_i);
  rtb_Bs_tank_full = (rtb_RelationalOperator5 && rtb_RelationalOperator2_j);
  rtb_Bs_soft_draw = (WaterHeatController_DW.Derivator_h <= WaterHeatController_rtP.Constant2_Value);
  rtb_DataTypeConversion_c = (WaterHeatController_DW.UnitDelay_DSTATE == WaterHeatController_rtP.Constant1_Value);
  ModelMDLOBJ10.step(&rtb_DataTypeConversion_c, &rtb_Out_cb);
  R_S_FlipFlopMDLOBJ12.step(&rtb_Bs_soft_draw, &rtb_Out_cb, &rtb_stt_l);
  if (rtb_stt_l) {
    rtb_Switch_d = WaterHeatController_rtP.Constant8_Value;
  } else {
    rtb_Switch_d = WaterHeatController_rtP.Constant6_Value;
  }

  rtb_RelationalOperator6 = (*rtu_Cs_tank_up_temp >= rtb_Switch_d);
  rtb_RelationalOperator1 = (*rtu_Cs_tank_down_temp >= rtb_Switch_d);
  if (WaterHeatController_DW.is_active_c2_WaterHeatControlle == 0) {
    WaterHeatController_DW.is_active_c2_WaterHeatControlle = 1U;
    WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControll_IN_Tank_empty;
    rtb_TankLevel = te_tank_lvl::Empty;
  } else {
    switch (WaterHeatController_DW.is_c2_WaterHeatController) {
     case WaterHeatControlle_IN_Tank_Full:
      rtb_TankLevel = te_tank_lvl::Ready;
      if (rtb_Bs_tank_low) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatController_IN_Tank_low;
        rtb_TankLevel = te_tank_lvl::Cold;
      } else if (rtb_Bs_tank_empty_f) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControll_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      }
      break;

     case WaterHeatControll_IN_Tank_empty:
      rtb_TankLevel = te_tank_lvl::Empty;
      if (rtb_Bs_tank_full) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControlle_IN_Tank_Full;
        rtb_TankLevel = te_tank_lvl::Ready;
      }
      break;

     case WaterHeatControlle_IN_Tank_high:
      rtb_TankLevel = te_tank_lvl::Hot;
      if (rtb_Bs_tank_full) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControlle_IN_Tank_Full;
        rtb_TankLevel = te_tank_lvl::Ready;
      } else if (rtb_Bs_tank_empty_f) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControll_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      } else if (rtb_Bs_tank_low) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatController_IN_Tank_low;
        rtb_TankLevel = te_tank_lvl::Cold;
      }
      break;

     default:
      // case IN_Tank_low:
      rtb_TankLevel = te_tank_lvl::Cold;
      if (rtb_RelationalOperator6 && rtb_RelationalOperator1) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControlle_IN_Tank_high;
        rtb_TankLevel = te_tank_lvl::Hot;
      } else if (rtb_Bs_tank_empty_f) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControll_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      }
      break;
    }
  }

  rtb_DataTypeConversion3 = WaterHeatController_rtP.Constant_Value_p;
  rtb_DataTypeConversion5 = WaterHeatController_rtP.Constant1_Value_f;
  bpIdx = plook_u32s16_bincka(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.CartoSetPointTankCold_bp01Data, 7U);
  rtb_DataTypeConversion1 = WaterHeatController_rtP.CartoSetPointTankCold_tableData[bpIdx];
  firstOrderTFMDLOBJ17.step(&rtb_DataTypeConversion1, &rtb_output, WaterHeatController_rtP.firstOrderTF2_K, WaterHeatController_rtP.firstOrderTF2_Tau,
    WaterHeatController_rtP.firstOrderTF2_sampleTime);
  bpIdx = plook_u32s16_bincka(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.CartoSetPointTankCold_low_bp01D, 7U);
  rtb_DataTypeConversion2 = WaterHeatController_rtP.CartoSetPointTankCold_low_table[bpIdx];
  firstOrderTFMDLOBJ16.step(&rtb_DataTypeConversion2, &rtb_output_k, WaterHeatController_rtP.firstOrderTF1_K, WaterHeatController_rtP.firstOrderTF1_Tau,
    WaterHeatController_rtP.firstOrderTF1_sampleTime);
  if (rtb_stt_l) {
    rtb_Switch4 = rtb_output;
  } else {
    rtb_Switch4 = rtb_output_k;
  }

  SaturationMDLOBJ15.step(&rtb_DataTypeConversion3, &rtb_DataTypeConversion5, &rtb_Switch4, &rtb_Saturation_o1, &rtb_Saturation_o2);
  switch (rtb_TankLevel) {
   case te_tank_lvl::Cold:
    rtb_Cs_tank_lvl_spd_pump_sp = static_cast<ta_rot_spd>(rtb_Saturation_o1);
    break;

   case te_tank_lvl::Hot:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatController_rtP.SpdOFF_Value_b;
    break;

   case te_tank_lvl::Empty:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatController_rtP.SpdMax_Value;
    break;

   default:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatController_rtP.SpdOFF_Value_b;
    break;
  }

  rtb_Product = WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd * static_cast<float>(rtb_Cs_tank_lvl_spd_pump_sp);
  firstOrderTFMDLOBJ13.step(&rtb_Product, &rtb_output_a, WaterHeatController_rtP.pumpFreqFilt_K, WaterHeatController_rtP.pumpFreqFilt_Tau,
    WaterHeatController_rtP.pumpFreqFilt_sampleTime);
  rtb_Product1 = WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd * static_cast<float>(rtb_Cs_tank_lvl_spd_pump_sp);
  firstOrderTFMDLOBJ14.step(&rtb_Product1, &rtb_output_e, WaterHeatController_rtP.pumpFreqFilt1_K, WaterHeatController_rtP.pumpFreqFilt1_Tau,
    WaterHeatController_rtP.pumpFreqFilt1_sampleTime);
  if (WaterHeatController_DW.antiShort_DSTATE) {
    *rty_Cs_heat_pump_rot_spd_sp = WaterHeatController_rtP.SpdOFF_Value;
  } else {
    if ((WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd <= WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd) && WaterHeatController_DW.exhstTempProt_DSTATE) {
      tmp_0 = WaterHeatController_rtP.TrefReg_Value;
    } else if (rtb_stt) {
      tmp_0 = WaterHeatController_rtP.TevapReg_Value;
    } else {
      tmp_0 = WaterHeatController_rtP.TankLevelReg_Value;
    }

    switch (tmp_0) {
     case te_pump_mode::Tref:
      if (rtb_output_a > WaterHeatController_rtP.Saturation_UpperSat) {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(WaterHeatController_rtP.Saturation_UpperSat);
      } else if (rtb_output_a < WaterHeatController_rtP.Saturation_LowerSat) {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(WaterHeatController_rtP.Saturation_LowerSat);
      } else {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(rtb_output_a);
      }
      break;

     case te_pump_mode::Tevap:
      if (rtb_output_e > WaterHeatController_rtP.Saturation1_UpperSat) {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(WaterHeatController_rtP.Saturation1_UpperSat);
      } else if (rtb_output_e < WaterHeatController_rtP.Saturation1_LowerSat) {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(WaterHeatController_rtP.Saturation1_LowerSat);
      } else {
        *rty_Cs_heat_pump_rot_spd_sp = static_cast<ta_rot_spd>(rtb_output_e);
      }
      break;

     default:
      *rty_Cs_heat_pump_rot_spd_sp = rtb_Cs_tank_lvl_spd_pump_sp;
      break;
    }
  }

  rtb_RelationalOperator1_i = (*rty_Cs_heat_pump_rot_spd_sp == WaterHeatController_rtP.SpdOFF_Value_e);
  rtb_DataTypeConversion_o = rtb_RelationalOperator1_i;
  PumpOFFdetectMDLOBJ5.step(&rtb_DataTypeConversion_o, &rtb_Out_c1);
  if (rtb_Out_c1) {
    rtb_isAntiShortCycleDelay = look1_is16lu32n16tu16_binlcse(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.TimeToPumpStartup_bp01Data,
      WaterHeatController_rtP.TimeToPumpStartup_tableData, 5U);
  } else {
    rtb_isAntiShortCycleDelay = WaterHeatController_DW.UnitDelay_DSTATE_a;
  }

  rtb_delay = rtb_isAntiShortCycleDelay;
  AntiShortCycleMDLOBJ3.step(&rtb_DataTypeConversion_o, &rtb_delay, &rtb_Out_h, rtP_WaterHeatCtrl_tick_C);
  rtb_TdwnToSingle = *rtu_Cs_tank_down_temp;
  HysteresisMDLOBJ4.step(&rtb_TdwnToSingle, &rtb_Out, WaterHeatController_rtP.HysteresisProtectTankDefrost_of,
    WaterHeatController_rtP.HysteresisProtectTankDefrost_on, WaterHeatController_rtP.HysteresisProtectTankDefrost_sw,
    WaterHeatController_rtP.HysteresisProtectTankDefrost__g);
  rtb_Equal = (*rtu_Cs_v40_sp != WaterHeatController_rtP.Zero_Value);
  if (rtb_Equal) {
    tmp = *rtu_Cs_v40_sp;
    if (tmp == 0) {
      rtb_Divide_0 = UINT16_MAX;

      // Divide by zero handler
    } else {
      rtb_Divide_0 = static_cast<uint16_t>(*rtu_Cs_v40_min / static_cast<uint32_t>(tmp));
    }

    *rty_Cs_v40_rat = static_cast<uint8_t>(rtb_Divide_0);
  } else {
    *rty_Cs_v40_rat = WaterHeatController_rtP.Zero_Value;
  }

  if (WaterHeatController_DW.prevTimerVal_DSTATE) {
    rtb_derivUpDate1 = *rtu_Cs_tank_down_temp;
  } else {
    rtb_derivUpDate1 = WaterHeatController_DW.UnitDelay1_DSTATE_e;
  }

  rtb_diff = static_cast<ta_temp>(*rtu_Cs_tank_down_temp - rtb_derivUpDate1);
  rtb_isNoDiff = (rtb_diff == WaterHeatController_rtP.Constant3_Value_a);
  rtb_DataTypeConversion1_c = rtb_isNoDiff;
  rtb_DataTypeConversion2_m = WaterHeatController_DW.prevTimerVal_DSTATE;
  TimerMDLOBJ9.step(&rtb_DataTypeConversion1_c, &rtb_DataTypeConversion2_m, &WaterHeatController_rtP.Constant4_Value_e, &rtb_Timer_o1, &rtb_TimerState,
                    rtP_WaterHeatCtrl_tick_C);
  WaterHeatController_DW.exhstTempProt_DSTATE = (rtb_ProtectPump && (!rtb_RelationalOperator1_i));
  WaterHeatController_DW.antiShort_DSTATE = ((rtb_Out_h == 0) && rtb_RelationalOperator1_i);
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd;
  WaterHeatController_DW.UnitDelay_DSTATE = *rty_Cs_heat_pump_rot_spd_sp;
  WaterHeatController_DW.UnitDelay_DSTATE_a = rtb_isAntiShortCycleDelay;
  WaterHeatController_DW.prevTimerVal_DSTATE = ((!rtb_isNoDiff) || (rtP_WaterHeatCtrl_tick_C <= rtb_Timer_o1));
  WaterHeatController_DW.UnitDelay1_DSTATE_e = rtb_derivUpDate1;
}

// Constructor
WaterHeatController::WaterHeatController():
  WaterHeatController_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
WaterHeatController::~WaterHeatController() = default;

//
// File trailer for generated code.
//
// [EOF]
//
