//
// File: WaterHeatCtrl.cpp
//
// Code generated for Simulink model 'WaterHeatCtrl'.
//
// Model version                  : 1.159
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Fri Sep  5 16:51:55 2025
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
#include "WaterHeatCtrl.h"
#include "WaterHeatCtrl_types.h"
#include <stdint.h>
#include <cmath>
#include <stdbool.h>
#include "WaterHeatCtrl_private.h"
#include "look1_is16lu32n16tu16_binlcse.h"
#include "look1_is16lu32n16Ds32_binlcs.h"
#include "plook_u32s16_bincka.h"
#include "cntrlSaturation.h"
#include "PID_backCalc.h"
#include "Derivator.h"
#include "firstOrderTF.h"
#include "Hysteresis.h"
#include "RisingEdgeDetect.h"
#include "VariableRisingEdgeDelay.h"
#include "RisingEdgeDelay.h"
#include "R_S_FlipFlop.h"

// Named constants for Chart: '<S16>/TankLevelStates'
const uint8_t WaterHeatCtrl_IN_Tank_empty{ 1U };

const uint8_t WaterHeatCtrl_IN_Tank_high{ 2U };

const uint8_t WaterHeatCtrl_IN_Tank_low{ 3U };

const bool WaterHeatCtrl_BGND{ false };// bool ground

WaterHeatCtrl::P_WaterHeatCtrl_T WaterHeatCtrl::WaterHeatCtrl_rtP{
  // Mask Parameter: coldTank_rpmSp_filter_K
  //  Referenced by: '<S28>/coldTank_rpmSp_filter'

  1.0F,

  // Mask Parameter: coldTank_rpmSp_filter_Tau
  //  Referenced by: '<S28>/coldTank_rpmSp_filter'

  10.0F,

  // Mask Parameter: downTemp_changeRate_initVal
  //  Referenced by: '<S10>/downTemp_changeRate'

  0.0F,

  // Mask Parameter: coldTank_rpmSp_filter_initVal
  //  Referenced by: '<S28>/coldTank_rpmSp_filter'

  0.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_of
  //  Referenced by: '<S67>/HysteresisProtectTankDefrost'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_on
  //  Referenced by: '<S67>/HysteresisProtectTankDefrost'

  0.0F,

  // Mask Parameter: downTemp_changeRate_sampleTime
  //  Referenced by: '<S10>/downTemp_changeRate'

  120.0F,

  // Mask Parameter: coldTank_rpmSp_filter_sampleTim
  //  Referenced by: '<S28>/coldTank_rpmSp_filter'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_sw
  //  Referenced by: '<S67>/HysteresisProtectTankDefrost'

  60,

  // Mask Parameter: HysteresisProtectTankDefrost__c
  //  Referenced by: '<S67>/HysteresisProtectTankDefrost'

  100,

  // Expression: 120/5
  //  Referenced by: '<S17>/posRateLimCst'

  24.0,

  // Expression: -120/5
  //  Referenced by: '<S17>/negRateLimCst'

  -24.0,

  // Expression: 1
  //  Referenced by: '<S25>/max_Tevap_StopDerating'

  1.0,

  // Expression: Cs_hard_draw_thrs_C
  //  Referenced by: '<S2>/Cs_hard_draw_thrs'

  -0.83333333333333337,

  // Expression: 290
  //  Referenced by: '<S20>/airTempLim'

  290.0,

  // Expression: 500
  //  Referenced by: '<S21>/waterTempLim'

  500.0,

  // Computed Parameter: Cs_tank_down_temp_der_Y0
  //  Referenced by: '<S9>/Cs_tank_down_temp_der'

  0.0F,

  // Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
  //  Referenced by: '<S11>/Cs_ctrl_temp_evap_spd_cmd'

  0.0F,

  // Computed Parameter: max_Tevap_Derating_Value
  //  Referenced by: '<S11>/max_Tevap_Derating'

  100.0F,

  // Computed Parameter: TevapRegulator_InitVal
  //  Referenced by: '<S11>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Kawu
  //  Referenced by: '<S11>/TevapRegulator'

  1.0F,

  // Computed Parameter: TevapRegulator_Kd
  //  Referenced by: '<S11>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Ki
  //  Referenced by: '<S11>/TevapRegulator'

  0.003F,

  // Computed Parameter: TevapRegulator_Kp
  //  Referenced by: '<S11>/TevapRegulator'

  0.01F,

  // Computed Parameter: TevapRegulator_Tau_f
  //  Referenced by: '<S11>/TevapRegulator'

  5.0F,

  // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
  //  Referenced by: '<S12>/Cs_ctrl_tref_max_spd_cmd'

  0.0F,

  // Computed Parameter: TevapRegulator_InitVal_d
  //  Referenced by: '<S12>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Kawu_d
  //  Referenced by: '<S12>/TevapRegulator'

  1.0F,

  // Computed Parameter: TevapRegulator_Kd_c
  //  Referenced by: '<S12>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Ki_d
  //  Referenced by: '<S12>/TevapRegulator'

  0.02F,

  // Computed Parameter: TevapRegulator_Kp_p
  //  Referenced by: '<S12>/TevapRegulator'

  0.5F,

  // Computed Parameter: TevapRegulator_Tau_f_m
  //  Referenced by: '<S12>/TevapRegulator'

  5.0F,

  // Computed Parameter: heatPumpSat_trefMax_UpperSat
  //  Referenced by: '<S13>/heatPumpSat_trefMax'

  3900.0F,

  // Computed Parameter: heatPumpSat_trefMax_LowerSat
  //  Referenced by: '<S13>/heatPumpSat_trefMax'

  1200.0F,

  // Computed Parameter: heatPumpSat_tevap_UpperSat
  //  Referenced by: '<S13>/heatPumpSat_tevap'

  3900.0F,

  // Computed Parameter: heatPumpSat_tevap_LowerSat
  //  Referenced by: '<S13>/heatPumpSat_tevap'

  1200.0F,

  // Computed Parameter: rateLimPrevValue_InitialConditi
  //  Referenced by: '<S17>/rateLimPrevValue'

  0.0F,

  // Computed Parameter: rateLimitMaxVal_UpperSat
  //  Referenced by: '<S17>/rateLimitMaxVal'

  3900.0F,

  // Computed Parameter: rateLimitMaxVal_LowerSat
  //  Referenced by: '<S17>/rateLimitMaxVal'

  1200.0F,

  // Computed Parameter: rateLimitMinVal_UpperSat
  //  Referenced by: '<S17>/rateLimitMinVal'

  3900.0F,

  // Computed Parameter: rateLimitMinVal_LowerSat
  //  Referenced by: '<S17>/rateLimitMinVal'

  1200.0F,

  // Computed Parameter: lowCritDefrostProtection_Delay
  //  Referenced by: '<S24>/lowCritDefrostProtection'

  1200.0F,

  // Computed Parameter: lowCritDefrostProtection_TimeSt
  //  Referenced by: '<S24>/lowCritDefrostProtection'

  1.0F,

  // Computed Parameter: highCritDefrostProtection_Delay
  //  Referenced by: '<S24>/highCritDefrostProtection'

  120.0F,

  // Computed Parameter: highCritDefrostProtection_TimeS
  //  Referenced by: '<S24>/highCritDefrostProtection'

  1.0F,

  // Computed Parameter: ctrl_temp_evap_spd_cmd_prev_Ini
  //  Referenced by: '<S25>/ctrl_temp_evap_spd_cmd_prev'

  0.0F,

  // Computed Parameter: evapTempProtReset_Delay
  //  Referenced by: '<S25>/evapTempProtReset'

  600.0F,

  // Computed Parameter: evapTempProtReset_TimeStep
  //  Referenced by: '<S25>/evapTempProtReset'

  1.0F,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S1>/BstrStopped'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Heating
  //  Referenced by: '<S1>/BstrHeating'

  te_heat_stt::Heating,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S1>/HpStopped'

  te_heat_stt::Stopped,

  // Expression: te_heat_stt.Heating
  //  Referenced by: '<S1>/hpHeating'

  te_heat_stt::Heating,

  // Expression: te_heat_stt.Heating
  //  Referenced by: '<Root>/heat_pump_stt_prev'

  te_heat_stt::Heating,

  // Expression: te_heat_stt.Stopped
  //  Referenced by: '<S14>/heatPump_off'

  te_heat_stt::Stopped,

  // Expression: te_on_off.off
  //  Referenced by: '<S3>/heatPumpOff'

  te_on_off::off,

  // Expression: te_on_off.off
  //  Referenced by: '<S13>/te_on_off'

  te_on_off::off,

  // Expression: te_on_off.off
  //  Referenced by: '<S1>/boosterOff'

  te_on_off::off,

  // Expression: te_pump_mode.highPres
  //  Referenced by: '<S13>/highPres'

  te_pump_mode::highPres,

  // Expression: te_pump_mode.highLoad
  //  Referenced by: '<S13>/highLoad'

  te_pump_mode::highLoad,

  // Expression: te_pump_mode.pumpOFF
  //  Referenced by: '<S13>/pumpOFF'

  te_pump_mode::pumpOFF,

  // Expression: te_pump_mode.Tref
  //  Referenced by: '<S13>/TrefReg'

  te_pump_mode::Tref,

  // Expression: te_pump_mode.Tevap
  //  Referenced by: '<S13>/TevapReg'

  te_pump_mode::Tevap,

  // Expression: te_pump_mode.Tank_lvl
  //  Referenced by: '<S13>/TankLevelReg'

  te_pump_mode::Tank_lvl,

  // Expression: te_tank_size.L180
  //  Referenced by: '<S32>/L180'

  te_tank_size::L180,

  // Expression: te_tank_size.L180
  //  Referenced by: '<S33>/L180'

  te_tank_size::L180,

  // Computed Parameter: Temp_2C_SetPointPID_Value
  //  Referenced by: '<S11>/Temp_-2°C_SetPointPID'

  -20,

  // Expression: Cs_temp_ref_tol_C
  //  Referenced by: '<S12>/trefTol'

  30,

  // Expression: Ct_tank_down_temp_pump_off_bp_C
  //  Referenced by: '<S22>/TimeToPumpStartup'

  { 100, 250, 260, 490, 500, 700 },

  // Computed Parameter: Ct_180L_auto_3ppl_Value
  //  Referenced by: '<S32>/Ct_180L_auto_3ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_4ppl_Value
  //  Referenced by: '<S32>/Ct_180L_auto_4ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_5ppl_Value
  //  Referenced by: '<S32>/Ct_180L_auto_5ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_6ppl_Value
  //  Referenced by: '<S32>/Ct_180L_auto_6ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_3ppl_Value
  //  Referenced by: '<S32>/Ct_105L_auto_3ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_4ppl_Value
  //  Referenced by: '<S32>/Ct_105L_auto_4ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_5ppl_Value
  //  Referenced by: '<S32>/Ct_105L_auto_5ppl'

  { -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_6ppl_Value
  //  Referenced by: '<S32>/Ct_105L_auto_6ppl'

  { -1, -1, -1, -1 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S42>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S42>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_thrs_min_C
  //  Referenced by: '<S42>/Cs_temp_thrs_min'

  500,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S42>/Cs_temp_sys_min'

  60,

  // Computed Parameter: Ct_180L_auto_2ppl_Value
  //  Referenced by: '<S32>/Ct_180L_auto_2ppl'

  { 540, 470, 380, 300 },

  // Computed Parameter: Ct_105L_auto_2ppl_Value
  //  Referenced by: '<S32>/Ct_105L_auto_2ppl'

  { 540, 470, 380, 300 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S47>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S47>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_thrs_min_C
  //  Referenced by: '<S47>/Cs_temp_thrs_min'

  500,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S47>/Cs_temp_sys_min_C'

  60,

  // Expression: Ct_180L_eco_3ppl_C
  //  Referenced by: '<S33>/Ct_180L_eco_3ppl'

  { 540, 400, 350, 300 },

  // Expression: Ct_180L_eco_6ppl_C
  //  Referenced by: '<S33>/Ct_180L_eco_6ppl'

  { 540, 420, 380, 300 },

  // Expression: Ct_105L_eco_3ppl_C
  //  Referenced by: '<S33>/Ct_105L_eco_3ppl'

  { 540, 400, 350, 300 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S62>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S62>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S62>/Cs_temp_sys_min'

  60,

  // Computed Parameter: Ct_hyb_temp_thrs_Value
  //  Referenced by: '<S36>/Ct_hyb_temp_thrs'

  { 480, -1, 400, 300 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S37>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S37>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_thrs_min_C
  //  Referenced by: '<S37>/Cs_temp_thrs_min'

  500,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S37>/Cs_temp_sys_min'

  60,

  // Computed Parameter: Ct_anti_lgn_temp_thrs_Value
  //  Referenced by: '<S31>/Ct_anti_lgn_temp_thrs'

  { 540, 540, 540, 540 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S57>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S57>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S57>/Cs_temp_sys_min'

  60,

  // Computed Parameter: Ct_hldy_temp_thrs_Value
  //  Referenced by: '<S35>/Ct_hldy_temp_thrs'

  { 60, 60, 30, 0 },

  // Expression: Cs_temp_sys_max_C
  //  Referenced by: '<S52>/Cs_temp_sys_max'

  550,

  // Expression: Cs_temp_heat_pump_temp_max_C
  //  Referenced by: '<S52>/Cs_temp_heat_pump_temp_max'

  550,

  // Expression: Cs_temp_thrs_min_C
  //  Referenced by: '<S52>/Cs_temp_thrs_min'

  500,

  // Expression: Cs_temp_sys_min_C
  //  Referenced by: '<S52>/Cs_temp_sys_min'

  60,

  // Computed Parameter: Ct_heat_pump_test_temp_thrs_Val
  //  Referenced by: '<S34>/Ct_heat_pump_test_temp_thrs'

  { 540, 540, 540, 30 },

  // Expression: Cs_heat_pump_lim_func_l_C
  //  Referenced by: '<S18>/Cs_heat_pump_lim_func_l'

  100,

  // Expression: Cs_heat_pump_lim_func_h_C
  //  Referenced by: '<S18>/Cs_heat_pump_lim_func_h'

  400,

  // Expression: Cs_hpc_low_crit_dfr_prot_temp_C
  //  Referenced by: '<S24>/Cs_hpc_low_crit_dfr_prot_temp_C'

  -40,

  // Expression: Cs_hpc_high_crit_dfr_prot_temp_C
  //  Referenced by: '<S24>/Cs_hpc_high_crit_dfr_prot_temp_C'

  -100,

  // Expression: Ct_temp_ref_max_data_C
  //  Referenced by: '<S19>/Tref_max'

  { 750, 750, 750, 750, 750, 750, 750 },

  // Expression: Ct_temp_evap_bp_C
  //  Referenced by: '<S19>/Tref_max'

  { 200, 300, 400, 500, 600, 650, 700 },

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S25>/Cs_hpc_dfr_prot_temp'

  -20,

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S25>/Cs_hpc_dfr_prot_temp_l'

  -20,

  // Expression: Ct_temp_tank_cold_sp_bp_C
  //  Referenced by: '<S15>/CartoSetPointTankCold'

  { 150, 200, 250, 300, 350, 360, 450, 500 },

  // Computed Parameter: maxDerat_Value
  //  Referenced by: '<S11>/maxDerat'

  100U,

  // Computed Parameter: minDerat_Value
  //  Referenced by: '<S11>/minDerat'

  0U,

  // Computed Parameter: maxDerat_Value_a
  //  Referenced by: '<S12>/maxDerat'

  1U,

  // Computed Parameter: minDerat_Value_f
  //  Referenced by: '<S12>/minDerat'

  0U,

  // Computed Parameter: max_Value
  //  Referenced by: '<S12>/max'

  100U,

  // Expression: Cs_pump_high_load_prot_rot_spd_max_C
  //  Referenced by: '<S13>/highLoadRotSpeedMax'

  3000U,

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S13>/SpdOFF'

  0U,

  // Expression: Cs_pump_high_load_prot_rot_spd_max_C
  //  Referenced by: '<S13>/Cs_pump_high_load_prot_rot_spd_max_C'

  3000U,

  // Expression: Cs_pump_high_pres_prot_rot_spd_max_C
  //  Referenced by: '<S13>/highPresRotSpeedMax'

  1800U,

  // Expression: Ct_time_cntr_pump_off_data_C
  //  Referenced by: '<S22>/TimeToPumpStartup'

  { 300U, 300U, 360U, 1740U, 1800U, 1800U },

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S15>/SpdMax'

  3900U,

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S15>/SpdOFF1'

  0U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S13>/minRotSpdSp'

  1200U,

  // Computed Parameter: Cs_anti_shrt_cycl_dly_prev_Init
  //  Referenced by: '<S22>/Cs_anti_shrt_cycl_dly_prev'

  0U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S15>/Cs_rot_spd_pump_max_C'

  3900U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S15>/Cs_rot_spd_pump_min_C'

  1200U,

  // Expression: Ct_rpm_pump_sp_tank_cold_C
  //  Referenced by: '<S15>/CartoSetPointTankCold'

  { 3900U, 3600U, 1200U, 1200U, 1200U, 1200U, 1200U, 1200U },

  // Computed Parameter: nullSpeed_Value
  //  Referenced by: '<S1>/nullSpeed'

  0U,

  // Expression: Cs_pump_high_pres_prot_rot_spd_max_C
  //  Referenced by: '<S13>/Cs_pump_high_pres_prot_rot_spd_max_C'

  1800U,

  // Expression: false
  //  Referenced by: '<S19>/exhstTempProt'

  false
};

// Output and update for atomic system: '<Root>/ModeThresholdTempCalc'
void WaterHeatCtrl::WaterHeat_ModeThresholdTempCalc()
{
  // local block i/o variables
  float rtb_Saturation2_o1;
  float rtb_Saturation2_o2;
  float rtb_Saturation6_o1;
  float rtb_Saturation6_o2;
  float rtb_Saturation2_o1_l;
  float rtb_Saturation2_o2_m;
  float rtb_Saturation2_o1_g;
  float rtb_Saturation2_o2_k;
  float rtb_Saturation2_o1_c;
  float rtb_Saturation2_o2_h;
  float rtb_Saturation2_o1_b;
  float rtb_Saturation2_o2_i;
  float rtb_Saturation1_o1;
  float rtb_Saturation1_o2;
  float rtb_Saturation5_o1;
  float rtb_Saturation5_o2;
  float rtb_Saturation1_o1_n;
  float rtb_Saturation1_o2_e;
  float rtb_Saturation1_o1_c;
  float rtb_Saturation1_o2_a;
  float rtb_Saturation1_o1_p;
  float rtb_Saturation1_o2_c;
  float rtb_Saturation1_o1_g;
  float rtb_Saturation1_o2_f;
  float rtb_Saturation3_o1;
  float rtb_Saturation3_o2;
  float rtb_Saturation7_o1;
  float rtb_Saturation7_o2;
  float rtb_Saturation3_o1_d;
  float rtb_Saturation3_o2_m;
  float rtb_Saturation3_o1_e;
  float rtb_Saturation3_o2_m2;
  float rtb_Saturation3_o1_f;
  float rtb_Saturation3_o2_k;
  float rtb_Saturation3_o1_db;
  float rtb_Saturation3_o2_a;
  float rtb_Saturation4_o1;
  float rtb_Saturation4_o2;
  float rtb_Saturation8_o1;
  float rtb_Saturation8_o2;
  float rtb_Saturation4_o1_e;
  float rtb_Saturation4_o2_m;
  float rtb_Saturation4_o1_b;
  float rtb_Saturation4_o2_k;
  float rtb_Saturation4_o1_g;
  float rtb_Saturation4_o2_e;
  float rtb_Saturation4_o1_c;
  float rtb_Saturation4_o2_p;
  float rtb_DataTypeConversion12;
  float rtb_DataTypeConversion12_a;
  float rtb_DataTypeConversion12_e;
  float rtb_DataTypeConversion12_h;
  float rtb_DataTypeConversion12_j;
  float rtb_DataTypeConversion12_m;
  float rtb_DataTypeConversion1_a_tmp;
  float rtb_DataTypeConversion1_e_tmp;
  float rtb_DataTypeConversion1_g_tmp;
  float rtb_DataTypeConversion1_k_tmp;
  float rtb_DataTypeConversion1_l_tmp;
  float rtb_DataTypeConversion1_tmp;
  float rtb_DataTypeConversion2;
  float rtb_DataTypeConversion2_d;
  float rtb_DataTypeConversion2_e;
  float rtb_DataTypeConversion2_ey_tmp;
  float rtb_DataTypeConversion2_i;
  float rtb_DataTypeConversion2_j3_tmp;
  float rtb_DataTypeConversion3;
  float rtb_DataTypeConversion3_c4;
  float rtb_DataTypeConversion3_i;
  float rtb_DataTypeConversion3_j;
  float rtb_DataTypeConversion3_k;
  float rtb_DataTypeConversion3_p;
  float rtb_DataTypeConversion5_c_tmp;
  float rtb_DataTypeConversion5_g_tmp;
  float rtb_DataTypeConversion5_k_tmp;
  float rtb_DataTypeConversion5_tmp;
  float rtb_DataTypeConversion6;
  float rtb_DataTypeConversion6_c;
  float rtb_DataTypeConversion6_e;
  float rtb_DataTypeConversion6_i;
  float rtb_DataTypeConversion6_j;
  float rtb_DataTypeConversion6_o;
  float rtb_DataTypeConversion9;
  float rtb_DataTypeConversion9_f;
  float rtb_DataTypeConversion9_i;
  float rtb_DataTypeConversion9_mq;
  float rtb_DataTypeConversion9_n;
  float rtb_DataTypeConversion9_o;
  ta_temp rtb_Ct_auto_temp_thrs_idx_0;
  ta_temp rtb_Ct_auto_temp_thrs_idx_1;
  ta_temp rtb_Ct_auto_temp_thrs_idx_2;
  ta_temp rtb_Ct_auto_temp_thrs_idx_3;
  ta_temp rtb_Ct_eco_temp_thrstttt_idx_1;
  ta_temp rtb_Ct_eco_temp_thrstttt_idx_2;
  ta_temp rtb_Ct_eco_temp_thrstttt_idx_3;
  ta_temp rtb_minTemp;
  ta_temp rtb_minTemp_i;
  ta_temp rtb_minTemp_ii;
  ta_temp rtb_minTemp_j;
  ta_temp rtb_minTemp_l;
  ta_temp rtb_minTemp_o;
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Valu) {
    rtb_minTemp = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value;
  } else {
    rtb_minTemp = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Valu;
  }

  rtb_DataTypeConversion1_tmp = rtb_minTemp;
  if (WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value >= WaterHeatCtrl_rtP.Cs_temp_sys_min_Value) {
    rtb_DataTypeConversion2 = WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value;
  } else {
    rtb_DataTypeConversion2 = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value;
  }

  if (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_tank_size == WaterHeatCtrl_rtP.L180_Value) {
    switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ns_pers_nb) {
     case 0:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[3];
      break;

     case 1:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[3];
      break;

     case 2:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_2ppl_Value[3];
      break;

     case 3:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_3ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_3ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_3ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_3ppl_Value[3];
      break;

     case 4:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_4ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_4ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_4ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_4ppl_Value[3];
      break;

     case 5:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_5ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_5ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_5ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_5ppl_Value[3];
      break;

     default:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_auto_6ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_180L_auto_6ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_180L_auto_6ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_180L_auto_6ppl_Value[3];
      break;
    }
  } else {
    switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ns_pers_nb) {
     case 0:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[3];
      break;

     case 1:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[3];
      break;

     case 2:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_2ppl_Value[3];
      break;

     case 3:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_3ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_3ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_3ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_3ppl_Value[3];
      break;

     case 4:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_4ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_4ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_4ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_4ppl_Value[3];
      break;

     case 5:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_5ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_5ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_5ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_5ppl_Value[3];
      break;

     default:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_auto_6ppl_Value[0];
      rtb_Ct_auto_temp_thrs_idx_1 = WaterHeatCtrl_rtP.Ct_105L_auto_6ppl_Value[1];
      rtb_Ct_auto_temp_thrs_idx_2 = WaterHeatCtrl_rtP.Ct_105L_auto_6ppl_Value[2];
      rtb_Ct_auto_temp_thrs_idx_3 = WaterHeatCtrl_rtP.Ct_105L_auto_6ppl_Value[3];
      break;
    }
  }

  rtb_DataTypeConversion3 = rtb_Ct_auto_temp_thrs_idx_0;
  Saturation2MDLOBJ18.step(&rtb_DataTypeConversion1_tmp, &rtb_DataTypeConversion2, &rtb_DataTypeConversion3, &rtb_Saturation2_o1, &rtb_Saturation2_o2);
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_o <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_m) {
    rtb_minTemp_o = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_o;
  } else {
    rtb_minTemp_o = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_m;
  }

  rtb_DataTypeConversion1_a_tmp = rtb_minTemp_o;
  if (WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_h >= WaterHeatCtrl_rtP.Cs_temp_sys_min_C_Value) {
    rtb_DataTypeConversion2_d = WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_h;
  } else {
    rtb_DataTypeConversion2_d = WaterHeatCtrl_rtP.Cs_temp_sys_min_C_Value;
  }

  if (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_tank_size == WaterHeatCtrl_rtP.L180_Value_b) {
    switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ns_pers_nb) {
     case 0:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[3];
      break;

     case 1:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[3];
      break;

     case 2:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[3];
      break;

     case 3:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_3ppl_Value[3];
      break;

     case 4:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[3];
      break;

     case 5:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[3];
      break;

     default:
      rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[0];
      rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[1];
      rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[2];
      rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_180L_eco_6ppl_Value[3];
      break;
    }
  } else {
    rtb_Ct_auto_temp_thrs_idx_0 = WaterHeatCtrl_rtP.Ct_105L_eco_3ppl_Value[0];
    rtb_Ct_eco_temp_thrstttt_idx_1 = WaterHeatCtrl_rtP.Ct_105L_eco_3ppl_Value[1];
    rtb_Ct_eco_temp_thrstttt_idx_2 = WaterHeatCtrl_rtP.Ct_105L_eco_3ppl_Value[2];
    rtb_Ct_eco_temp_thrstttt_idx_3 = WaterHeatCtrl_rtP.Ct_105L_eco_3ppl_Value[3];
  }

  rtb_DataTypeConversion3_i = rtb_Ct_auto_temp_thrs_idx_0;
  Saturation6MDLOBJ22.step(&rtb_DataTypeConversion1_a_tmp, &rtb_DataTypeConversion2_d, &rtb_DataTypeConversion3_i, &rtb_Saturation6_o1, &rtb_Saturation6_o2);
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_ot <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_p) {
    rtb_minTemp_i = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_ot;
  } else {
    rtb_minTemp_i = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_p;
  }

  rtb_DataTypeConversion1_e_tmp = rtb_minTemp_i;
  rtb_DataTypeConversion2_ey_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_h;
  rtb_DataTypeConversion3_c4 = WaterHeatCtrl_rtP.Ct_hyb_temp_thrs_Value[0];
  Saturation2MDLOBJ34.step(&rtb_DataTypeConversion1_e_tmp, &rtb_DataTypeConversion2_ey_tmp, &rtb_DataTypeConversion3_c4, &rtb_Saturation2_o1_l,
    &rtb_Saturation2_o2_m);
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_c <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_g) {
    rtb_minTemp_ii = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_c;
  } else {
    rtb_minTemp_ii = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_g;
  }

  rtb_DataTypeConversion1_g_tmp = rtb_minTemp_ii;
  if (WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_c >= WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_i) {
    rtb_DataTypeConversion2_e = WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_c;
  } else {
    rtb_DataTypeConversion2_e = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_i;
  }

  rtb_DataTypeConversion3_k = WaterHeatCtrl_rtP.Ct_anti_lgn_temp_thrs_Value[0];
  Saturation2MDLOBJ14.step(&rtb_DataTypeConversion1_g_tmp, &rtb_DataTypeConversion2_e, &rtb_DataTypeConversion3_k, &rtb_Saturation2_o1_g, &rtb_Saturation2_o2_k);
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_ch <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_c) {
    rtb_minTemp_l = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_ch;
  } else {
    rtb_minTemp_l = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_c;
  }

  rtb_DataTypeConversion1_l_tmp = rtb_minTemp_l;
  rtb_DataTypeConversion2_j3_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_j;
  rtb_DataTypeConversion3_p = WaterHeatCtrl_rtP.Ct_hldy_temp_thrs_Value[0];
  Saturation2MDLOBJ30.step(&rtb_DataTypeConversion1_l_tmp, &rtb_DataTypeConversion2_j3_tmp, &rtb_DataTypeConversion3_p, &rtb_Saturation2_o1_c,
    &rtb_Saturation2_o2_h);
  if (WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_e <= WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_a) {
    rtb_minTemp_j = WaterHeatCtrl_rtP.Cs_temp_sys_max_Value_e;
  } else {
    rtb_minTemp_j = WaterHeatCtrl_rtP.Cs_temp_heat_pump_temp_max_Va_a;
  }

  rtb_DataTypeConversion1_k_tmp = rtb_minTemp_j;
  if (WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_l >= WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_it) {
    rtb_DataTypeConversion2_i = WaterHeatCtrl_rtP.Cs_temp_thrs_min_Value_l;
  } else {
    rtb_DataTypeConversion2_i = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_it;
  }

  rtb_DataTypeConversion3_j = WaterHeatCtrl_rtP.Ct_heat_pump_test_temp_thrs_Val[0];
  Saturation2MDLOBJ26.step(&rtb_DataTypeConversion1_k_tmp, &rtb_DataTypeConversion2_i, &rtb_DataTypeConversion3_j, &rtb_Saturation2_o1_b, &rtb_Saturation2_o2_i);
  switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_op_mode) {
   case te_op_mode::Auto:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1));
    break;

   case te_op_mode::FullElec:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1));
    break;

   case te_op_mode::Eco:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation6_o1));
    break;

   case te_op_mode::Boost:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation6_o1));
    break;

   case te_op_mode::Hybrid:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1_l));
    break;

   case te_op_mode::AntiLegionella:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1_g));
    break;

   case te_op_mode::Holidays:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1_c));
    break;

   default:
    WaterHeatCtrl_DW.Cs_temp_tank_high = static_cast<ta_temp>(std::floor(rtb_Saturation2_o1_b));
    break;
  }

  rtb_DataTypeConversion5_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value;
  rtb_DataTypeConversion6 = rtb_Ct_auto_temp_thrs_idx_1;
  Saturation1MDLOBJ17.step(&rtb_DataTypeConversion1_tmp, &rtb_DataTypeConversion5_tmp, &rtb_DataTypeConversion6, &rtb_Saturation1_o1, &rtb_Saturation1_o2);
  rtb_DataTypeConversion5_c_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_C_Value;
  rtb_DataTypeConversion6_i = rtb_Ct_eco_temp_thrstttt_idx_1;
  Saturation5MDLOBJ21.step(&rtb_DataTypeConversion1_a_tmp, &rtb_DataTypeConversion5_c_tmp, &rtb_DataTypeConversion6_i, &rtb_Saturation5_o1, &rtb_Saturation5_o2);
  rtb_DataTypeConversion6_o = WaterHeatCtrl_rtP.Ct_hyb_temp_thrs_Value[1];
  Saturation1MDLOBJ33.step(&rtb_DataTypeConversion1_e_tmp, &rtb_DataTypeConversion2_ey_tmp, &rtb_DataTypeConversion6_o, &rtb_Saturation1_o1_n,
    &rtb_Saturation1_o2_e);
  rtb_DataTypeConversion5_k_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_i;
  rtb_DataTypeConversion6_j = WaterHeatCtrl_rtP.Ct_anti_lgn_temp_thrs_Value[1];
  Saturation1MDLOBJ13.step(&rtb_DataTypeConversion1_g_tmp, &rtb_DataTypeConversion5_k_tmp, &rtb_DataTypeConversion6_j, &rtb_Saturation1_o1_c,
    &rtb_Saturation1_o2_a);
  rtb_DataTypeConversion6_c = WaterHeatCtrl_rtP.Ct_hldy_temp_thrs_Value[1];
  Saturation1MDLOBJ29.step(&rtb_DataTypeConversion1_l_tmp, &rtb_DataTypeConversion2_j3_tmp, &rtb_DataTypeConversion6_c, &rtb_Saturation1_o1_p,
    &rtb_Saturation1_o2_c);
  rtb_DataTypeConversion5_g_tmp = WaterHeatCtrl_rtP.Cs_temp_sys_min_Value_it;
  rtb_DataTypeConversion6_e = WaterHeatCtrl_rtP.Ct_heat_pump_test_temp_thrs_Val[1];
  Saturation1MDLOBJ25.step(&rtb_DataTypeConversion1_k_tmp, &rtb_DataTypeConversion5_g_tmp, &rtb_DataTypeConversion6_e, &rtb_Saturation1_o1_g,
    &rtb_Saturation1_o2_f);
  switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_op_mode) {
   case te_op_mode::Auto:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1));
    break;

   case te_op_mode::FullElec:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1));
    break;

   case te_op_mode::Eco:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation5_o1));
    break;

   case te_op_mode::Boost:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation5_o1));
    break;

   case te_op_mode::Hybrid:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1_n));
    break;

   case te_op_mode::AntiLegionella:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1_c));
    break;

   case te_op_mode::Holidays:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1_p));
    break;

   default:
    WaterHeatCtrl_DW.Cs_temp_tank_med = static_cast<ta_temp>(std::floor(rtb_Saturation1_o1_g));
    break;
  }

  rtb_DataTypeConversion9 = rtb_Ct_auto_temp_thrs_idx_2;
  Saturation3MDLOBJ19.step(&rtb_DataTypeConversion1_tmp, &rtb_DataTypeConversion5_tmp, &rtb_DataTypeConversion9, &rtb_Saturation3_o1, &rtb_Saturation3_o2);
  rtb_DataTypeConversion9_f = rtb_Ct_eco_temp_thrstttt_idx_2;
  Saturation7MDLOBJ23.step(&rtb_DataTypeConversion1_a_tmp, &rtb_DataTypeConversion5_c_tmp, &rtb_DataTypeConversion9_f, &rtb_Saturation7_o1, &rtb_Saturation7_o2);
  rtb_DataTypeConversion9_o = WaterHeatCtrl_rtP.Ct_hyb_temp_thrs_Value[2];
  Saturation3MDLOBJ35.step(&rtb_DataTypeConversion1_e_tmp, &rtb_DataTypeConversion2_ey_tmp, &rtb_DataTypeConversion9_o, &rtb_Saturation3_o1_d,
    &rtb_Saturation3_o2_m);
  rtb_DataTypeConversion9_n = WaterHeatCtrl_rtP.Ct_anti_lgn_temp_thrs_Value[2];
  Saturation3MDLOBJ15.step(&rtb_DataTypeConversion1_g_tmp, &rtb_DataTypeConversion5_k_tmp, &rtb_DataTypeConversion9_n, &rtb_Saturation3_o1_e,
    &rtb_Saturation3_o2_m2);
  rtb_DataTypeConversion9_i = WaterHeatCtrl_rtP.Ct_hldy_temp_thrs_Value[2];
  Saturation3MDLOBJ31.step(&rtb_DataTypeConversion1_l_tmp, &rtb_DataTypeConversion2_j3_tmp, &rtb_DataTypeConversion9_i, &rtb_Saturation3_o1_f,
    &rtb_Saturation3_o2_k);
  rtb_DataTypeConversion9_mq = WaterHeatCtrl_rtP.Ct_heat_pump_test_temp_thrs_Val[2];
  Saturation3MDLOBJ27.step(&rtb_DataTypeConversion1_k_tmp, &rtb_DataTypeConversion5_g_tmp, &rtb_DataTypeConversion9_mq, &rtb_Saturation3_o1_db,
    &rtb_Saturation3_o2_a);
  switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_op_mode) {
   case te_op_mode::Auto:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1));
    break;

   case te_op_mode::FullElec:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1));
    break;

   case te_op_mode::Eco:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation7_o1));
    break;

   case te_op_mode::Boost:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation7_o1));
    break;

   case te_op_mode::Hybrid:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1_d));
    break;

   case te_op_mode::AntiLegionella:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1_e));
    break;

   case te_op_mode::Holidays:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1_f));
    break;

   default:
    WaterHeatCtrl_DW.Cs_temp_tank_low = static_cast<ta_temp>(std::floor(rtb_Saturation3_o1_db));
    break;
  }

  rtb_DataTypeConversion12 = rtb_Ct_auto_temp_thrs_idx_3;
  Saturation4MDLOBJ20.step(&rtb_DataTypeConversion1_tmp, &rtb_DataTypeConversion5_tmp, &rtb_DataTypeConversion12, &rtb_Saturation4_o1, &rtb_Saturation4_o2);
  rtb_DataTypeConversion12_m = rtb_Ct_eco_temp_thrstttt_idx_3;
  Saturation8MDLOBJ24.step(&rtb_DataTypeConversion1_a_tmp, &rtb_DataTypeConversion5_c_tmp, &rtb_DataTypeConversion12_m, &rtb_Saturation8_o1, &rtb_Saturation8_o2);
  rtb_DataTypeConversion12_j = WaterHeatCtrl_rtP.Ct_hyb_temp_thrs_Value[3];
  Saturation4MDLOBJ36.step(&rtb_DataTypeConversion1_e_tmp, &rtb_DataTypeConversion2_ey_tmp, &rtb_DataTypeConversion12_j, &rtb_Saturation4_o1_e,
    &rtb_Saturation4_o2_m);
  rtb_DataTypeConversion12_a = WaterHeatCtrl_rtP.Ct_anti_lgn_temp_thrs_Value[3];
  Saturation4MDLOBJ16.step(&rtb_DataTypeConversion1_g_tmp, &rtb_DataTypeConversion5_k_tmp, &rtb_DataTypeConversion12_a, &rtb_Saturation4_o1_b,
    &rtb_Saturation4_o2_k);
  rtb_DataTypeConversion12_h = WaterHeatCtrl_rtP.Ct_hldy_temp_thrs_Value[3];
  Saturation4MDLOBJ32.step(&rtb_DataTypeConversion1_l_tmp, &rtb_DataTypeConversion2_j3_tmp, &rtb_DataTypeConversion12_h, &rtb_Saturation4_o1_g,
    &rtb_Saturation4_o2_e);
  rtb_DataTypeConversion12_e = WaterHeatCtrl_rtP.Ct_heat_pump_test_temp_thrs_Val[3];
  Saturation4MDLOBJ28.step(&rtb_DataTypeConversion1_k_tmp, &rtb_DataTypeConversion5_g_tmp, &rtb_DataTypeConversion12_e, &rtb_Saturation4_o1_c,
    &rtb_Saturation4_o2_p);
  switch (WaterHeatC_rtu_WaterHeatCtrl_In->Ss_op_mode) {
   case te_op_mode::Auto:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1));
    break;

   case te_op_mode::FullElec:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1));
    break;

   case te_op_mode::Eco:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation8_o1));
    break;

   case te_op_mode::Boost:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation8_o1));
    break;

   case te_op_mode::Hybrid:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1_e));
    break;

   case te_op_mode::AntiLegionella:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1_b));
    break;

   case te_op_mode::Holidays:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1_g));
    break;

   default:
    WaterHeatCtrl_DW.Cs_temp_tank_empt = static_cast<ta_temp>(std::floor(rtb_Saturation4_o1_c));
    break;
  }
}

// System initialize for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::init(void)
{
  WaterHeatCtrl_DW.rateLimPrevValue_DSTATE = WaterHeatCtrl_rtP.rateLimPrevValue_InitialConditi;
  WaterHeatCtrl_DW.heat_pump_stt_prev_DSTATE = WaterHeatCtrl_rtP.heat_pump_stt_prev_InitialCondi;
  WaterHeatCtrl_DW.Cs_anti_shrt_cycl_dly_prev_DSTA = WaterHeatCtrl_rtP.Cs_anti_shrt_cycl_dly_prev_Init;
  WaterHeatCtrl_DW.exhstTempProt_DSTATE = WaterHeatCtrl_rtP.exhstTempProt_InitialCondition;
  WaterHeatCtrl_DW.ctrl_temp_evap_spd_cmd_prev_DST = WaterHeatCtrl_rtP.ctrl_temp_evap_spd_cmd_prev_Ini;
  TevapRegulatorMDLOBJ3.init(WaterHeatCtrl_rtP.TevapRegulator_InitVal_d);
  WaterHeatCtrl_DW.Cs_ctrl_tref_max_spd_cmd = WaterHeatCtrl_rtP.Cs_ctrl_tref_max_spd_cmd_Y0;
  TevapRegulatorMDLOBJ2.init(WaterHeatCtrl_rtP.TevapRegulator_InitVal);
  WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd = WaterHeatCtrl_rtP.Cs_ctrl_temp_evap_spd_cmd_Y0;
  downTemp_changeRateMDLOBJ1.init(WaterHeatCtrl_rtP.downTemp_changeRate_initVal);
  WaterHeatCtrl_DW.downTemp_changeRate = WaterHeatCtrl_rtP.Cs_tank_down_temp_der_Y0;
  coldTank_rpmSp_filteMDLOBJ12.init(WaterHeatCtrl_rtP.coldTank_rpmSp_filter_initVal);
  HysteresisProtectTanMDLOBJ37.init(WaterHeatCtrl_rtP.HysteresisProtectTankDefrost_of);
}

// System reset for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::reset(void)
{
  WaterHeatCtrl_DW.rateLimPrevValue_DSTATE = WaterHeatCtrl_rtP.rateLimPrevValue_InitialConditi;
  WaterHeatCtrl_DW.heat_pump_stt_prev_DSTATE = WaterHeatCtrl_rtP.heat_pump_stt_prev_InitialCondi;
  WaterHeatCtrl_DW.Cs_anti_shrt_cycl_dly_prev_DSTA = WaterHeatCtrl_rtP.Cs_anti_shrt_cycl_dly_prev_Init;
  WaterHeatCtrl_DW.exhstTempProt_DSTATE = WaterHeatCtrl_rtP.exhstTempProt_InitialCondition;
  WaterHeatCtrl_DW.ctrl_temp_evap_spd_cmd_prev_DST = WaterHeatCtrl_rtP.ctrl_temp_evap_spd_cmd_prev_Ini;
  PumpOFFdetectMDLOBJ6.reset();
  AntiShortCycleMDLOBJ5.reset();
  lowCritDefrostProtecMDLOBJ8.reset();
  highCritDefrostProteMDLOBJ7.reset();
  evapTempProtResetMDLOBJ10.reset();
  TevapProtStateMDLOBJ9.reset();
  WaterHeatCtrl_DW.temporalCounter_i1 = 0U;
  WaterHeatCtrl_DW.is_active_c3_WaterHeatCtrl = 0U;
  WaterHeatCtrl_DW.is_active_c2_WaterHeatCtrl = 0U;
  WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = 0;
  coldTank_rpmSp_filteMDLOBJ12.reset(WaterHeatCtrl_rtP.coldTank_rpmSp_filter_initVal);
  HysteresisProtectTanMDLOBJ37.reset(WaterHeatCtrl_rtP.HysteresisProtectTankDefrost_of);
}

// Disable for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::disable(void)
{
  WaterHeatCtrl_DW.CntrlEvapPump_MODE = false;
}

// Output and update for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::output(const tb_WaterHeatCtrl_In *rtu_WaterHeatCtrl_In, tb_WaterHeatCtrl_Out *rty_WaterHeatCtrl_Out)
{
  // local block i/o variables
  float rtb_output;
  float rtb_Pump_rpm_sat_o1;
  float rtb_Pump_rpm_sat_o2;
  float rtb_Out;
  float rtb_ovrFlw;
  float rtb_Out_f;
  float rtb_final_cmd;
  float rtb_ovrFlwVal;
  float rtb_final_cmd_f;
  float rtb_ovrFlwVal_e;
  uint8_t rtb_Out_h;
  uint8_t rtb_Bs_low_crit_prot_ena_uint;
  uint8_t rtb_Bs_high_crit_prot_ena_uint;
  uint8_t rtb_Bs_hpc_dfr_prot_rst_uint;
  bool rtb_Bs_pump_off_dtct;
  bool rtb_stt;
  float rtb_Cs_heat_pump_rot_spd_sp_boo;
  float rtb_Cs_tank_down_temp_sngl;
  float rtb_DataTypeConversion;
  float rtb_DataTypeConversion1_n;
  float rtb_DataTypeConversion3;
  float rtb_DataTypeConversion5_d;
  float rtb_delay;
  float rtb_maxDerat;
  float rtb_maxDerat_a;
  float rtb_mesTevap;
  float rtb_mesTpumpXhst;
  float rtb_minDerat;
  float rtb_minDerat_a;
  float rtb_rateLimitMaxVal;
  float rtb_rateLimitMinVal;
  float rtb_setPointTevap;
  float rtb_setPointTrefMax;
  float u0;
  ta_rot_spd rtb_Cs_heat_pump_freq_sp_Switch;
  ta_rot_spd rtb_Cs_heat_pump_rot_spd_sp;
  ta_rot_spd rtb_Cs_heat_pump_rot_spd_sp_l_h;
  ta_rot_spd rtb_Cs_heat_pump_rot_spd_sp_l_i;
  ta_rot_spd rtb_Cs_tank_lvl_spd_pump_sp;
  ta_temp rtb_Tref_max;
  uint16_t rtb_isAntiShortCycleDelay;
  uint8_t rtb_Bs_high_crit_prot_uint;
  uint8_t rtb_Bs_hpc_dfr_prot_rst_cond_ui;
  uint8_t rtb_Bs_low_crit_prot_uint;
  uint8_t rtb_DataTypeConversion_d;
  bool rtb_Bs_hpc_crit_dfr_prot_ena;
  bool rtb_Bs_hpc_dfr_prot_rst;
  bool rtb_Bs_hpc_dfr_prot_set;
  bool rtb_Bs_pump_on_tmp;
  bool rtb_Bs_tank_empt;
  bool rtb_Bs_tank_high;
  bool rtb_GreaterThan;
  bool rtb_GreaterThan_m;
  bool rtb_LimInfTmpAirXhst;
  bool rtb_LimSupTmpAirXhst;
  bool rtb_andAntiShortCycleProt;
  bool rtb_isHeatPumpStopped;
  te_heat_stt rtb_heatPumpOffSwitch;
  te_on_off rtb_Bs_elec_bstr_htr_sp_Switch;
  te_pump_mode rtb_Ss_pump_ctrl_mod_prem;
  te_tank_lvl rtb_TankLevel;
  WaterHeatC_rtu_WaterHeatCtrl_In = rtu_WaterHeatCtrl_In;
  u0 = WaterHeatCtrl_DW.rateLimPrevValue_DSTATE + static_cast<float>(WaterHeatCtrl_rtP.posRateLimCst_Value);
  if (u0 > WaterHeatCtrl_rtP.rateLimitMaxVal_UpperSat) {
    rtb_rateLimitMaxVal = WaterHeatCtrl_rtP.rateLimitMaxVal_UpperSat;
  } else if (u0 < WaterHeatCtrl_rtP.rateLimitMaxVal_LowerSat) {
    rtb_rateLimitMaxVal = WaterHeatCtrl_rtP.rateLimitMaxVal_LowerSat;
  } else {
    rtb_rateLimitMaxVal = u0;
  }

  u0 = WaterHeatCtrl_DW.rateLimPrevValue_DSTATE + static_cast<float>(WaterHeatCtrl_rtP.negRateLimCst_Value);
  if (u0 > WaterHeatCtrl_rtP.rateLimitMinVal_UpperSat) {
    rtb_rateLimitMinVal = WaterHeatCtrl_rtP.rateLimitMinVal_UpperSat;
  } else if (u0 < WaterHeatCtrl_rtP.rateLimitMinVal_LowerSat) {
    rtb_rateLimitMinVal = WaterHeatCtrl_rtP.rateLimitMinVal_LowerSat;
  } else {
    rtb_rateLimitMinVal = u0;
  }

  rtb_LimInfTmpAirXhst = (rtu_WaterHeatCtrl_In->Cs_vent_temp < WaterHeatCtrl_rtP.Cs_heat_pump_lim_func_l_Value);
  rtb_LimSupTmpAirXhst = (rtu_WaterHeatCtrl_In->Cs_vent_temp > WaterHeatCtrl_rtP.Cs_heat_pump_lim_func_h_Value);
  rtb_isHeatPumpStopped = (WaterHeatCtrl_DW.heat_pump_stt_prev_DSTATE == WaterHeatCtrl_rtP.heatPump_off_Value);
  rtb_DataTypeConversion_d = rtb_isHeatPumpStopped;
  PumpOFFdetectMDLOBJ6.step(&rtb_DataTypeConversion_d, &rtb_Bs_pump_off_dtct);
  if (rtb_Bs_pump_off_dtct) {
    rtb_isAntiShortCycleDelay = look1_is16lu32n16tu16_binlcse(rtu_WaterHeatCtrl_In->Cs_tank_down_temp, WaterHeatCtrl_rtP.TimeToPumpStartup_bp01Data,
      WaterHeatCtrl_rtP.TimeToPumpStartup_tableData, 5U);
  } else {
    rtb_isAntiShortCycleDelay = WaterHeatCtrl_DW.Cs_anti_shrt_cycl_dly_prev_DSTA;
  }

  rtb_delay = rtb_isAntiShortCycleDelay;
  AntiShortCycleMDLOBJ5.step(&rtb_DataTypeConversion_d, &rtb_delay, &rtb_Out_h, rtP_WaterHeatCtrl_tick_C);
  rtb_andAntiShortCycleProt = ((rtb_Out_h == 0) && rtb_isHeatPumpStopped);
  rtb_Bs_pump_on_tmp = !rtb_isHeatPumpStopped;
  rtb_Bs_low_crit_prot_uint = ((rtu_WaterHeatCtrl_In->Cs_pump_evap_temp < WaterHeatCtrl_rtP.Cs_hpc_low_crit_dfr_prot_temp_C) && rtb_Bs_pump_on_tmp);
  lowCritDefrostProtecMDLOBJ8.step(&rtb_Bs_low_crit_prot_uint, &rtb_Bs_low_crit_prot_ena_uint, WaterHeatCtrl_rtP.lowCritDefrostProtection_Delay,
    WaterHeatCtrl_rtP.lowCritDefrostProtection_TimeSt);
  rtb_Bs_high_crit_prot_uint = (rtb_Bs_pump_on_tmp && (rtu_WaterHeatCtrl_In->Cs_pump_evap_temp < WaterHeatCtrl_rtP.Cs_hpc_high_crit_dfr_prot_temp_));
  highCritDefrostProteMDLOBJ7.step(&rtb_Bs_high_crit_prot_uint, &rtb_Bs_high_crit_prot_ena_uint, WaterHeatCtrl_rtP.highCritDefrostProtection_Delay,
    WaterHeatCtrl_rtP.highCritDefrostProtection_TimeS);
  rtb_Bs_hpc_crit_dfr_prot_ena = ((rtb_Bs_low_crit_prot_ena_uint != 0) || (rtb_Bs_high_crit_prot_ena_uint != 0));
  rtb_Tref_max = look1_is16lu32n16Ds32_binlcs(rtu_WaterHeatCtrl_In->Cs_pump_evap_temp, WaterHeatCtrl_rtP.Tref_max_bp01Data, WaterHeatCtrl_rtP.Tref_max_tableData,
    6U);
  if (WaterHeatCtrl_DW.exhstTempProt_DSTATE) {
    rtb_setPointTrefMax = static_cast<int16_t>(rtb_Tref_max - WaterHeatCtrl_rtP.trefTol_Value);
    rtb_mesTpumpXhst = rtu_WaterHeatCtrl_In->Cs_pump_xhst_temp;
    rtb_maxDerat = WaterHeatCtrl_rtP.maxDerat_Value_a;
    rtb_minDerat = WaterHeatCtrl_rtP.minDerat_Value_f;
    TevapRegulatorMDLOBJ3.step(&rtb_setPointTrefMax, &rtb_mesTpumpXhst, &rtb_maxDerat, &rtb_minDerat, (const_cast<bool*>(&WaterHeatCtrl_BGND)), &rtb_final_cmd,
      &rtb_ovrFlwVal, WaterHeatCtrl_rtP.TevapRegulator_InitVal_d, WaterHeatCtrl_rtP.TevapRegulator_Kawu_d, WaterHeatCtrl_rtP.TevapRegulator_Kd_c,
      WaterHeatCtrl_rtP.TevapRegulator_Ki_d, WaterHeatCtrl_rtP.TevapRegulator_Kp_p, rtP_WaterHeatCtrl_tick_C, WaterHeatCtrl_rtP.TevapRegulator_Tau_f_m);
    WaterHeatCtrl_DW.Cs_ctrl_tref_max_spd_cmd = (static_cast<float>(WaterHeatCtrl_rtP.max_Value) - rtb_final_cmd) / static_cast<float>
      (WaterHeatCtrl_rtP.max_Value);
  }

  rtb_Bs_hpc_dfr_prot_set = (rtb_Bs_pump_on_tmp && (rtu_WaterHeatCtrl_In->Cs_pump_evap_temp < WaterHeatCtrl_rtP.Cs_hpc_dfr_prot_temp_Value));
  rtb_Bs_hpc_dfr_prot_rst_cond_ui = (((rtu_WaterHeatCtrl_In->Cs_pump_evap_temp > WaterHeatCtrl_rtP.Cs_hpc_dfr_prot_temp_l_Value) &&
    (WaterHeatCtrl_DW.ctrl_temp_evap_spd_cmd_prev_DST >= WaterHeatCtrl_rtP.max_Tevap_StopDerating_Value)) || rtb_isHeatPumpStopped);
  evapTempProtResetMDLOBJ10.step(&rtb_Bs_hpc_dfr_prot_rst_cond_ui, &rtb_Bs_hpc_dfr_prot_rst_uint, WaterHeatCtrl_rtP.evapTempProtReset_Delay,
    WaterHeatCtrl_rtP.evapTempProtReset_TimeStep);
  rtb_Bs_hpc_dfr_prot_rst = (rtb_Bs_hpc_dfr_prot_rst_uint != 0);
  TevapProtStateMDLOBJ9.step(&rtb_Bs_hpc_dfr_prot_set, &rtb_Bs_hpc_dfr_prot_rst, &rtb_stt);
  if (rtb_stt) {
    if (!WaterHeatCtrl_DW.CntrlEvapPump_MODE) {
      TevapRegulatorMDLOBJ2.reset(WaterHeatCtrl_rtP.TevapRegulator_InitVal);
      WaterHeatCtrl_DW.CntrlEvapPump_MODE = true;
    }

    rtb_setPointTevap = WaterHeatCtrl_rtP.Temp_2C_SetPointPID_Value;
    rtb_mesTevap = rtu_WaterHeatCtrl_In->Cs_pump_evap_temp;
    rtb_maxDerat_a = WaterHeatCtrl_rtP.maxDerat_Value;
    rtb_minDerat_a = WaterHeatCtrl_rtP.minDerat_Value;
    TevapRegulatorMDLOBJ2.step(&rtb_setPointTevap, &rtb_mesTevap, &rtb_maxDerat_a, &rtb_minDerat_a, (const_cast<bool*>(&WaterHeatCtrl_BGND)), &rtb_final_cmd_f,
      &rtb_ovrFlwVal_e, WaterHeatCtrl_rtP.TevapRegulator_InitVal, WaterHeatCtrl_rtP.TevapRegulator_Kawu, WaterHeatCtrl_rtP.TevapRegulator_Kd,
      WaterHeatCtrl_rtP.TevapRegulator_Ki, WaterHeatCtrl_rtP.TevapRegulator_Kp, rtP_WaterHeatCtrl_tick_C, WaterHeatCtrl_rtP.TevapRegulator_Tau_f);
    WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd = (WaterHeatCtrl_rtP.max_Tevap_Derating_Value - rtb_final_cmd_f) / WaterHeatCtrl_rtP.max_Tevap_Derating_Value;
  } else {
    WaterHeatCtrl_DW.CntrlEvapPump_MODE = false;
  }

  if (rtb_LimInfTmpAirXhst || rtb_LimSupTmpAirXhst || rtb_andAntiShortCycleProt || rtb_Bs_hpc_crit_dfr_prot_ena || (rtu_WaterHeatCtrl_In->Ss_heat_pump_ena ==
       WaterHeatCtrl_rtP.te_on_off_Value)) {
    rtb_Ss_pump_ctrl_mod_prem = WaterHeatCtrl_rtP.pumpOFF_Value;
  } else if ((WaterHeatCtrl_DW.Cs_ctrl_tref_max_spd_cmd <= WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd) && WaterHeatCtrl_DW.exhstTempProt_DSTATE) {
    rtb_Ss_pump_ctrl_mod_prem = WaterHeatCtrl_rtP.TrefReg_Value;
  } else if (rtb_stt) {
    rtb_Ss_pump_ctrl_mod_prem = WaterHeatCtrl_rtP.TevapReg_Value;
  } else {
    rtb_Ss_pump_ctrl_mod_prem = WaterHeatCtrl_rtP.TankLevelReg_Value;
  }

  WaterHeat_ModeThresholdTempCalc();
  if (WaterHeatCtrl_DW.temporalCounter_i1 < 120) {
    WaterHeatCtrl_DW.temporalCounter_i1 = static_cast<uint8_t>(WaterHeatCtrl_DW.temporalCounter_i1 + 1);
  }

  if (WaterHeatCtrl_DW.is_active_c3_WaterHeatCtrl == 0) {
    WaterHeatCtrl_DW.is_active_c3_WaterHeatCtrl = 1U;
    WaterHeatCtrl_DW.temporalCounter_i1 = 0U;
  } else if (WaterHeatCtrl_DW.temporalCounter_i1 == 120) {
    rtb_DataTypeConversion = rtu_WaterHeatCtrl_In->Cs_tank_down_temp;
    downTemp_changeRateMDLOBJ1.step(&rtb_DataTypeConversion, &WaterHeatCtrl_DW.downTemp_changeRate, WaterHeatCtrl_rtP.downTemp_changeRate_sampleTime);
  }

  if (WaterHeatCtrl_DW.temporalCounter_i1 == 120) {
    WaterHeatCtrl_DW.temporalCounter_i1 = 0U;
  }

  rtb_Bs_tank_empt = ((rtu_WaterHeatCtrl_In->Cs_tank_up_temp < WaterHeatCtrl_DW.Cs_temp_tank_empt) || (WaterHeatCtrl_DW.downTemp_changeRate <=
    WaterHeatCtrl_rtP.Cs_hard_draw_thrs_Value));
  rtb_Bs_tank_high = ((rtu_WaterHeatCtrl_In->Cs_tank_down_temp >= WaterHeatCtrl_DW.Cs_temp_tank_high) && (rtu_WaterHeatCtrl_In->Cs_tank_up_temp >=
    WaterHeatCtrl_DW.Cs_temp_tank_high));
  if (WaterHeatCtrl_DW.is_active_c2_WaterHeatCtrl == 0) {
    WaterHeatCtrl_DW.is_active_c2_WaterHeatCtrl = 1U;
    WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_empty;
    rtb_TankLevel = te_tank_lvl::Empty;
  } else {
    switch (WaterHeatCtrl_DW.is_c2_WaterHeatCtrl) {
     case WaterHeatCtrl_IN_Tank_empty:
      rtb_TankLevel = te_tank_lvl::Empty;
      if (rtb_Bs_tank_high) {
        WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_high;
        rtb_TankLevel = te_tank_lvl::Hot;
      }
      break;

     case WaterHeatCtrl_IN_Tank_high:
      rtb_TankLevel = te_tank_lvl::Hot;
      if (rtb_Bs_tank_empt) {
        WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      } else if ((rtu_WaterHeatCtrl_In->Cs_tank_up_temp < WaterHeatCtrl_DW.Cs_temp_tank_med) || (rtu_WaterHeatCtrl_In->Cs_tank_down_temp <
                  WaterHeatCtrl_DW.Cs_temp_tank_low)) {
        WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_low;
        rtb_TankLevel = te_tank_lvl::Cold;
      }
      break;

     default:
      // case IN_Tank_low:
      rtb_TankLevel = te_tank_lvl::Cold;
      if (rtb_Bs_tank_empt) {
        WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      } else if (rtb_Bs_tank_high) {
        WaterHeatCtrl_DW.is_c2_WaterHeatCtrl = WaterHeatCtrl_IN_Tank_high;
        rtb_TankLevel = te_tank_lvl::Hot;
      }
      break;
    }
  }

  rtb_DataTypeConversion3 = WaterHeatCtrl_rtP.Cs_rot_spd_pump_max_C_Value;
  rtb_DataTypeConversion5_d = WaterHeatCtrl_rtP.Cs_rot_spd_pump_min_C_Value;
  rtb_DataTypeConversion1_n = WaterHeatCtrl_rtP.CartoSetPointTankCold_tableData[plook_u32s16_bincka(rtu_WaterHeatCtrl_In->Cs_tank_down_temp,
    WaterHeatCtrl_rtP.CartoSetPointTankCold_bp01Data, 7U)];
  coldTank_rpmSp_filteMDLOBJ12.step(&rtb_DataTypeConversion1_n, &rtb_output, WaterHeatCtrl_rtP.coldTank_rpmSp_filter_K,
    WaterHeatCtrl_rtP.coldTank_rpmSp_filter_Tau, WaterHeatCtrl_rtP.coldTank_rpmSp_filter_sampleTim);
  Pump_rpm_satMDLOBJ11.step(&rtb_DataTypeConversion3, &rtb_DataTypeConversion5_d, &rtb_output, &rtb_Pump_rpm_sat_o1, &rtb_Pump_rpm_sat_o2);
  switch (rtb_TankLevel) {
   case te_tank_lvl::Empty:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatCtrl_rtP.SpdMax_Value;
    break;

   case te_tank_lvl::Cold:
    rtb_Cs_tank_lvl_spd_pump_sp = static_cast<ta_rot_spd>(rtb_Pump_rpm_sat_o1);
    break;

   default:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatCtrl_rtP.SpdOFF1_Value;
    break;
  }

  switch (rtb_Ss_pump_ctrl_mod_prem) {
   case te_pump_mode::Tref:
    u0 = WaterHeatCtrl_DW.Cs_ctrl_tref_max_spd_cmd * static_cast<float>(rtb_Cs_tank_lvl_spd_pump_sp);
    if (u0 > WaterHeatCtrl_rtP.heatPumpSat_trefMax_UpperSat) {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(WaterHeatCtrl_rtP.heatPumpSat_trefMax_UpperSat);
    } else if (u0 < WaterHeatCtrl_rtP.heatPumpSat_trefMax_LowerSat) {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(WaterHeatCtrl_rtP.heatPumpSat_trefMax_LowerSat);
    } else {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(u0);
    }
    break;

   case te_pump_mode::Tevap:
    u0 = WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd * static_cast<float>(rtb_Cs_tank_lvl_spd_pump_sp);
    if (u0 > WaterHeatCtrl_rtP.heatPumpSat_tevap_UpperSat) {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(WaterHeatCtrl_rtP.heatPumpSat_tevap_UpperSat);
    } else if (u0 < WaterHeatCtrl_rtP.heatPumpSat_tevap_LowerSat) {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(WaterHeatCtrl_rtP.heatPumpSat_tevap_LowerSat);
    } else {
      rtb_Cs_heat_pump_rot_spd_sp_l_h = static_cast<ta_rot_spd>(u0);
    }
    break;

   case te_pump_mode::Tank_lvl:
    rtb_Cs_heat_pump_rot_spd_sp_l_h = rtb_Cs_tank_lvl_spd_pump_sp;
    break;

   default:
    rtb_Cs_heat_pump_rot_spd_sp_l_h = WaterHeatCtrl_rtP.SpdOFF_Value;
    break;
  }

  rtb_GreaterThan = (rtu_WaterHeatCtrl_In->Cs_vent_temp > WaterHeatCtrl_rtP.airTempLim_Value);
  if (rtb_GreaterThan) {
    if (WaterHeatCtrl_rtP.highLoadRotSpeedMax_Value <= rtb_Cs_heat_pump_rot_spd_sp_l_h) {
      rtb_Cs_heat_pump_rot_spd_sp_l_i = WaterHeatCtrl_rtP.highLoadRotSpeedMax_Value;
    } else {
      rtb_Cs_heat_pump_rot_spd_sp_l_i = rtb_Cs_heat_pump_rot_spd_sp_l_h;
    }
  } else {
    rtb_Cs_heat_pump_rot_spd_sp_l_i = rtb_Cs_heat_pump_rot_spd_sp_l_h;
  }

  rtb_GreaterThan_m = (rtu_WaterHeatCtrl_In->Cs_tank_down_temp > WaterHeatCtrl_rtP.waterTempLim_Value);
  if (rtb_GreaterThan_m) {
    if (WaterHeatCtrl_rtP.highPresRotSpeedMax_Value <= rtb_Cs_heat_pump_rot_spd_sp_l_i) {
      rtb_Cs_heat_pump_rot_spd_sp = WaterHeatCtrl_rtP.highPresRotSpeedMax_Value;
    } else {
      rtb_Cs_heat_pump_rot_spd_sp = rtb_Cs_heat_pump_rot_spd_sp_l_i;
    }
  } else {
    rtb_Cs_heat_pump_rot_spd_sp = rtb_Cs_heat_pump_rot_spd_sp_l_i;
  }

  rtb_Cs_heat_pump_rot_spd_sp_boo = rtb_Cs_heat_pump_rot_spd_sp;
  rateSaturationMDLOBJ4.step(&rtb_rateLimitMaxVal, &rtb_rateLimitMinVal, &rtb_Cs_heat_pump_rot_spd_sp_boo, &rtb_Out, &rtb_ovrFlw);
  if (rtu_WaterHeatCtrl_In->Bs_heat_pump_freq_sp_simu_ena) {
    rtb_Cs_heat_pump_freq_sp_Switch = rtu_WaterHeatCtrl_In->Cs_heat_pump_rot_spd_sp_simu;
  } else if (rtb_Cs_heat_pump_rot_spd_sp > WaterHeatCtrl_rtP.minRotSpdSp_Value) {
    rtb_Cs_heat_pump_freq_sp_Switch = static_cast<ta_rot_spd>(rtb_Out);
  } else {
    rtb_Cs_heat_pump_freq_sp_Switch = rtb_Cs_heat_pump_rot_spd_sp;
  }

  if (rtb_Cs_heat_pump_freq_sp_Switch == WaterHeatCtrl_rtP.nullSpeed_Value) {
    rtb_heatPumpOffSwitch = WaterHeatCtrl_rtP.HpStopped_Value;
  } else {
    rtb_heatPumpOffSwitch = WaterHeatCtrl_rtP.hpHeating_Value;
  }

  if (rtu_WaterHeatCtrl_In->Bs_elec_bstr_htr_sp_simu_ena) {
    rtb_Bs_elec_bstr_htr_sp_Switch = rtu_WaterHeatCtrl_In->Ss_elec_bstr_htr_sp_simu;
  } else {
    rtb_Bs_elec_bstr_htr_sp_Switch = WaterHeatCtrl_rtP.heatPumpOff_Value;
  }

  if (rtb_Bs_elec_bstr_htr_sp_Switch == WaterHeatCtrl_rtP.boosterOff_Value) {
    rty_WaterHeatCtrl_Out->Ss_elec_htr_bstr_stt = WaterHeatCtrl_rtP.BstrStopped_Value;
  } else {
    rty_WaterHeatCtrl_Out->Ss_elec_htr_bstr_stt = WaterHeatCtrl_rtP.BstrHeating_Value;
  }

  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_air_low_temp_prot_ena = rtb_LimInfTmpAirXhst;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_air_high_temp_prot_ena = rtb_LimSupTmpAirXhst;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_ant_shrt_cycl_lim_pump_ena = rtb_andAntiShortCycleProt;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_pump_high_pres_prot_ena = rtb_GreaterThan_m;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_pump_high_load_prot_ena = rtb_GreaterThan;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_hpc_dfr_prot_ena = rtb_stt;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_hpc_crit_dfr_prot_ena = rtb_Bs_hpc_crit_dfr_prot_ena;
  rty_WaterHeatCtrl_Out->Bt_heat_pump_prot.Bs_pump_prot_ena = WaterHeatCtrl_DW.exhstTempProt_DSTATE;
  if ((WaterHeatCtrl_rtP.Cs_pump_high_pres_prot_rot_spd_ == rtb_Cs_heat_pump_rot_spd_sp) && rtb_GreaterThan_m) {
    rty_WaterHeatCtrl_Out->Ss_pump_ctrl_mod = WaterHeatCtrl_rtP.highPres_Value;
  } else if ((WaterHeatCtrl_rtP.Cs_pump_high_load_prot_rot_spd_ == rtb_Cs_heat_pump_rot_spd_sp) && rtb_GreaterThan) {
    rty_WaterHeatCtrl_Out->Ss_pump_ctrl_mod = WaterHeatCtrl_rtP.highLoad_Value;
  } else {
    rty_WaterHeatCtrl_Out->Ss_pump_ctrl_mod = rtb_Ss_pump_ctrl_mod_prem;
  }

  rty_WaterHeatCtrl_Out->Cs_heat_pump_rot_spd_sp = rtb_Cs_heat_pump_freq_sp_Switch;
  rty_WaterHeatCtrl_Out->Ss_elec_bstr_htr_sp = rtb_Bs_elec_bstr_htr_sp_Switch;
  rty_WaterHeatCtrl_Out->Ss_heat_pump_stt = rtb_heatPumpOffSwitch;
  rty_WaterHeatCtrl_Out->Ss_tank_lvl_stt = rtb_TankLevel;
  rty_WaterHeatCtrl_Out->Cs_ctrl_temp_evap_spd_cmd = WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd;
  rtb_Cs_tank_down_temp_sngl = rtu_WaterHeatCtrl_In->Cs_tank_down_temp;
  HysteresisProtectTanMDLOBJ37.step(&rtb_Cs_tank_down_temp_sngl, &rtb_Out_f, WaterHeatCtrl_rtP.HysteresisProtectTankDefrost_of,
    WaterHeatCtrl_rtP.HysteresisProtectTankDefrost_on, WaterHeatCtrl_rtP.HysteresisProtectTankDefrost_sw, WaterHeatCtrl_rtP.HysteresisProtectTankDefrost__c);
  WaterHeatCtrl_DW.rateLimPrevValue_DSTATE = rtb_Out;
  WaterHeatCtrl_DW.heat_pump_stt_prev_DSTATE = rtb_heatPumpOffSwitch;
  WaterHeatCtrl_DW.Cs_anti_shrt_cycl_dly_prev_DSTA = rtb_isAntiShortCycleDelay;
  WaterHeatCtrl_DW.exhstTempProt_DSTATE = ((rtu_WaterHeatCtrl_In->Cs_pump_xhst_temp >= rtb_Tref_max) && rtb_Bs_pump_on_tmp);
  WaterHeatCtrl_DW.ctrl_temp_evap_spd_cmd_prev_DST = WaterHeatCtrl_DW.Cs_ctrl_temp_evap_spd_cmd;
}

// Constructor
WaterHeatCtrl::WaterHeatCtrl():
  WaterHeatCtrl_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
WaterHeatCtrl::~WaterHeatCtrl() = default;

//
// File trailer for generated code.
//
// [EOF]
//
