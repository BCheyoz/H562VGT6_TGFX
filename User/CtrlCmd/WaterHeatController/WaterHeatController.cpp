//
// File: WaterHeatController.cpp
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.598
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Wed Jun  4 11:34:29 2025
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
#include "cntrlSaturation.h"

// Named constants for Chart: '<S12>/TankLevelStates'
const uint8_t WaterHeatControll_IN_Tank_empty{ 2U };

const uint8_t WaterHeatControlle_IN_Tank_Full{ 1U };

const uint8_t WaterHeatControlle_IN_Tank_high{ 3U };

const uint8_t WaterHeatController_IN_Tank_low{ 4U };

const bool WaterHeatController_BGND{ false };// bool ground

WaterHeatController::P_WaterHeatController_T WaterHeatController::WaterHeatController_rtP{
  // Mask Parameter: filterSp1_K
  //  Referenced by: '<S24>/firstOrderTF'

  1.0F,

  // Mask Parameter: filterSp2_K
  //  Referenced by: '<S25>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt_K
  //  Referenced by: '<S18>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt1_K
  //  Referenced by: '<S19>/firstOrderTF'

  1.0F,

  // Mask Parameter: filterSp1_Tau
  //  Referenced by: '<S24>/firstOrderTF'

  10.0F,

  // Mask Parameter: filterSp2_Tau
  //  Referenced by: '<S25>/firstOrderTF'

  10.0F,

  // Mask Parameter: pumpFreqFilt_Tau
  //  Referenced by: '<S18>/firstOrderTF'

  20.0F,

  // Mask Parameter: pumpFreqFilt1_Tau
  //  Referenced by: '<S19>/firstOrderTF'

  20.0F,

  // Mask Parameter: Derivator_initVal
  //  Referenced by: '<S17>/Derivator'

  0.0F,

  // Mask Parameter: filterSp1_initVal
  //  Referenced by: '<S24>/firstOrderTF'

  0.0F,

  // Mask Parameter: filterSp2_initVal
  //  Referenced by: '<S25>/firstOrderTF'

  0.0F,

  // Mask Parameter: pumpFreqFilt_initVal
  //  Referenced by: '<S18>/firstOrderTF'

  0.0F,

  // Mask Parameter: pumpFreqFilt1_initVal
  //  Referenced by: '<S19>/firstOrderTF'

  0.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_of
  //  Referenced by: '<S6>/Hysteresis'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_on
  //  Referenced by: '<S6>/Hysteresis'

  0.0F,

  // Mask Parameter: Derivator_sampleTime
  //  Referenced by: '<S17>/Derivator'

  120.0F,

  // Mask Parameter: filterSp1_sampleTime
  //  Referenced by: '<S24>/firstOrderTF'

  1.0F,

  // Mask Parameter: filterSp2_sampleTime
  //  Referenced by: '<S25>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt_sampleTime
  //  Referenced by: '<S18>/firstOrderTF'

  1.0F,

  // Mask Parameter: pumpFreqFilt1_sampleTime
  //  Referenced by: '<S19>/firstOrderTF'

  1.0F,

  // Mask Parameter: HysteresisProtectTankDefrost_sw
  //  Referenced by: '<S6>/Hysteresis'

  80,

  // Mask Parameter: HysteresisProtectTankDefrost__g
  //  Referenced by: '<S6>/Hysteresis'

  120,

  // Expression: 1
  //  Referenced by: '<S4>/Constant5'

  1.0,

  // Expression: Cs_hard_draw_thrs_C
  //  Referenced by: '<S8>/Cs_hard_draw_thrs'

  -1.35,

  // Expression: Cs_soft_draw_thrs_C
  //  Referenced by: '<S8>/Cs_soft_draw_thrs'

  -0.1,

  // Expression: 0
  //  Referenced by: '<S8>/Constant1'

  0.0,

  // Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
  //  Referenced by: '<S1>/Cs_ctrl_temp_evap_spd_cmd'

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

  0.003F,

  // Computed Parameter: TevapRegulator_Kp
  //  Referenced by: '<S1>/TevapRegulator'

  0.01F,

  // Computed Parameter: TevapRegulator_Tau_f
  //  Referenced by: '<S1>/TevapRegulator'

  5.0F,

  // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
  //  Referenced by: '<S2>/Cs_ctrl_tref_max_spd_cmd'

  0.0F,

  // Computed Parameter: TevapRegulator_InitVal_a
  //  Referenced by: '<S2>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Kawu_j
  //  Referenced by: '<S2>/TevapRegulator'

  1.0F,

  // Computed Parameter: TevapRegulator_Kd_b
  //  Referenced by: '<S2>/TevapRegulator'

  0.0F,

  // Computed Parameter: TevapRegulator_Ki_i
  //  Referenced by: '<S2>/TevapRegulator'

  0.02F,

  // Computed Parameter: TevapRegulator_Kp_d
  //  Referenced by: '<S2>/TevapRegulator'

  0.5F,

  // Computed Parameter: TevapRegulator_Tau_f_n
  //  Referenced by: '<S2>/TevapRegulator'

  5.0F,

  // Computed Parameter: TdownminReg_InitVal
  //  Referenced by: '<S3>/TdownminReg'

  0.0F,

  // Computed Parameter: TdownminReg_Kawu
  //  Referenced by: '<S3>/TdownminReg'

  1.0F,

  // Computed Parameter: TdownminReg_Kd
  //  Referenced by: '<S3>/TdownminReg'

  0.0F,

  // Computed Parameter: TdownminReg_Ki
  //  Referenced by: '<S3>/TdownminReg'

  0.1F,

  // Computed Parameter: TdownminReg_Kp
  //  Referenced by: '<S3>/TdownminReg'

  1.0F,

  // Computed Parameter: TdownminReg_Tau_f
  //  Referenced by: '<S3>/TdownminReg'

  5.0F,

  // Computed Parameter: Cs_tank_down_temp_der_Y0
  //  Referenced by: '<S16>/Cs_tank_down_temp_der'

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

  // Computed Parameter: evapTempProtDelay_Delay
  //  Referenced by: '<S4>/evapTempProtDelay'

  7200.0F,

  // Computed Parameter: evapTempProtDelay_TimeStep
  //  Referenced by: '<S4>/evapTempProtDelay'

  1.0F,

  // Computed Parameter: UnitDelay1_InitialCondition
  //  Referenced by: '<S4>/Unit Delay1'

  0.0F,

  // Computed Parameter: evapTempProtDisable_Delay_Delay
  //  Referenced by: '<S4>/evapTempProtDisable_Delay'

  600.0F,

  // Computed Parameter: evapTempProtDisable_Delay_TimeS
  //  Referenced by: '<S4>/evapTempProtDisable_Delay'

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

  // Expression: te_tank_size.L180
  //  Referenced by: '<S21>/L180'

  te_tank_size::L180,

  // Expression: te_tank_size.L180
  //  Referenced by: '<S22>/L180'

  te_tank_size::L180,

  // Computed Parameter: Temp_2C_SetPointPID_Value
  //  Referenced by: '<S1>/Temp_-2°C_SetPointPID'

  -20,

  // Expression: Cs_temp_ref_tol_C
  //  Referenced by: '<S2>/Constant2'

  30,

  // Computed Parameter: Temp_10C_SetPointPID_Value
  //  Referenced by: '<S3>/Temp_10°C_SetPointPID'

  100,

  // Expression: Ct_tank_down_temp_pump_off_bp_C
  //  Referenced by: '<S4>/TimeToPumpStartup'

  { 100, 250, 260, 490, 500, 700 },

  // Computed Parameter: Ct_180L_eco_3ppl_Value
  //  Referenced by: '<S22>/Ct_180L_eco_3ppl'

  { 540, 540, 450, 380, 300 },

  // Computed Parameter: Ct_180L_eco_4ppl_Value
  //  Referenced by: '<S22>/Ct_180L_eco_4ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_eco_5ppl_Value
  //  Referenced by: '<S22>/Ct_180L_eco_5ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_eco_6ppl_Value
  //  Referenced by: '<S22>/Ct_180L_eco_6ppl'

  { 540, 540, 450, 380, 300 },

  // Computed Parameter: Ct_105L_eco_3ppl_Value
  //  Referenced by: '<S22>/Ct_105L_eco_3ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_eco_4ppl_Value
  //  Referenced by: '<S22>/Ct_105L_eco_4ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_eco_5ppl_Value
  //  Referenced by: '<S22>/Ct_105L_eco_5ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_eco_6ppl_Value
  //  Referenced by: '<S22>/Ct_105L_eco_6ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_3ppl_Value
  //  Referenced by: '<S21>/Ct_180L_auto_3ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_4ppl_Value
  //  Referenced by: '<S21>/Ct_180L_auto_4ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_5ppl_Value
  //  Referenced by: '<S21>/Ct_180L_auto_5ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_180L_auto_6ppl_Value
  //  Referenced by: '<S21>/Ct_180L_auto_6ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_3ppl_Value
  //  Referenced by: '<S21>/Ct_105L_auto_3ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_4ppl_Value
  //  Referenced by: '<S21>/Ct_105L_auto_4ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_5ppl_Value
  //  Referenced by: '<S21>/Ct_105L_auto_5ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_auto_6ppl_Value
  //  Referenced by: '<S21>/Ct_105L_auto_6ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_temp_thrs_dly_InitialConditi
  //  Referenced by: '<S10>/Ct_temp_thrs_dly'

  0,

  // Computed Parameter: Ct_180L_auto_2ppl_Value
  //  Referenced by: '<S21>/Ct_180L_auto_2ppl'

  { 540, 540, 470, 380, 300 },

  // Computed Parameter: Ct_105L_auto_2ppl_Value
  //  Referenced by: '<S21>/Ct_105L_auto_2ppl'

  { 540, 540, 470, 380, 300 },

  // Computed Parameter: Ct_180L_eco_2ppl_Value
  //  Referenced by: '<S22>/Ct_180L_eco_2ppl'

  { -1, -1, -1, -1, -1 },

  // Computed Parameter: Ct_105L_eco_2ppl_Value
  //  Referenced by: '<S22>/Ct_105L_eco_2ppl'

  { -1, -1, -1, -1, -1 },

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S4>/Constant4'

  -20,

  // Expression: Cs_hpc_dfr_prot_temp_l_C
  //  Referenced by: '<S4>/Constant6'

  -20,

  // Expression: Ct_temp_ref_max_data_C
  //  Referenced by: '<S4>/Tref_max'

  { 750, 750, 750, 750, 750, 750, 750 },

  // Expression: Ct_temp_evap_bp_C
  //  Referenced by: '<S4>/Tref_max'

  { 200, 300, 400, 500, 600, 650, 700 },

  // Expression: Ct_temp_tank_cold_sp_bp_C
  //  Referenced by: '<S11>/CartoSetPointTankCold'

  { 150, 200, 250, 300, 350, 360, 450, 500 },

  // Computed Parameter: CartoSetPointTankCold_low_table
  //  Referenced by: '<S11>/CartoSetPointTankCold_low'

  { 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200 },

  // Expression: Ct_temp_tank_cold_sp_bp_C
  //  Referenced by: '<S11>/CartoSetPointTankCold_low'

  { 150, 200, 250, 300, 350, 360, 450, 500 },

  // Computed Parameter: max_Value
  //  Referenced by: '<S1>/max'

  100U,

  // Computed Parameter: maxDerat_Value
  //  Referenced by: '<S1>/maxDerat'

  100U,

  // Computed Parameter: minDerat_Value
  //  Referenced by: '<S1>/minDerat'

  0U,

  // Computed Parameter: max_Value_d
  //  Referenced by: '<S2>/max'

  100U,

  // Computed Parameter: maxDerat_Value_e
  //  Referenced by: '<S2>/maxDerat'

  1U,

  // Computed Parameter: minDerat_Value_m
  //  Referenced by: '<S2>/minDerat'

  0U,

  // Computed Parameter: Cs_ctrl_temp_tdwn_spd_cmd_Y0
  //  Referenced by: '<S3>/Cs_ctrl_temp_tdwn_spd_cmd'

  0U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S3>/SpdMinPIDTdownTank'

  1200U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S3>/SpdMaxPIDTdownTank'

  3900U,

  // Expression: Ct_time_cntr_pump_off_data_C
  //  Referenced by: '<S4>/TimeToPumpStartup'

  { 300U, 300U, 360U, 1740U, 1800U, 1800U },

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S9>/SpdOFF'

  0U,

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S11>/SpdOFF1'

  0U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S11>/SpdMax'

  3900U,

  // Expression: Cs_rot_spd_pump_off_C
  //  Referenced by: '<S11>/SpdOFF'

  0U,

  // Computed Parameter: UnitDelay_InitialCondition
  //  Referenced by: '<S5>/Unit Delay'

  0U,

  // Expression: Cs_rot_spd_pump_max_C
  //  Referenced by: '<S11>/Cs_rot_spd_pump_max_C'

  3900U,

  // Expression: Cs_rot_spd_pump_min_C
  //  Referenced by: '<S11>/Cs_rot_spd_pump_min_C'

  1200U,

  // Expression: Ct_rpm_pump_sp_tank_cold_C
  //  Referenced by: '<S11>/CartoSetPointTankCold'

  { 1200U, 1200U, 1200U, 1200U, 1200U, 1200U, 1200U, 1200U },

  // Computed Parameter: SpdOFF_Value_e
  //  Referenced by: '<S4>/SpdOFF'

  0U,

  // Computed Parameter: UnitDelay_InitialCondition_m
  //  Referenced by: '<S4>/Unit Delay'

  300U,

  // Expression: false
  //  Referenced by: '<S4>/exhstTempProt'

  false,

  // Expression: false
  //  Referenced by: '<S4>/Constant3'

  false,

  // Expression: false
  //  Referenced by: '<S4>/antiShort'

  false
};

// System initialize for atomic system: '<S5>/ModeThresholdTempCalc'
void WaterHeatController::Wate_ModeThresholdTempCalc_Init()
{
  for (int32_t i{0}; i < 5; i++) {
    WaterHeatController_DW.Ct_temp_thrs_dly_DSTATE[i] = WaterHeatController_rtP.Ct_temp_thrs_dly_InitialConditi;
  }
}

// System reset for atomic system: '<S5>/ModeThresholdTempCalc'
void WaterHeatController::Wat_ModeThresholdTempCalc_Reset()
{
  for (int32_t i{0}; i < 5; i++) {
    WaterHeatController_DW.Ct_temp_thrs_dly_DSTATE[i] = WaterHeatController_rtP.Ct_temp_thrs_dly_InitialConditi;
  }
}

// Output and update for atomic system: '<S5>/ModeThresholdTempCalc'
void WaterHeatController::WaterHeat_ModeThresholdTempCalc()
{
  ta_temp rtb_Ct_eco_temp_thrs[5];
  if (*WaterHeatContr_rtu_Ss_tank_size == WaterHeatController_rtP.L180_Value_c) {
    switch (*WaterHeatControl_rtu_Ns_pers_nb) {
     case 0:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_2ppl_Value[i];
      }
      break;

     case 1:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_2ppl_Value[i];
      }
      break;

     case 2:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_2ppl_Value[i];
      }
      break;

     case 3:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_3ppl_Value[i];
      }
      break;

     case 4:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_4ppl_Value[i];
      }
      break;

     case 5:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_5ppl_Value[i];
      }
      break;

     default:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_180L_eco_6ppl_Value[i];
      }
      break;
    }
  } else {
    switch (*WaterHeatControl_rtu_Ns_pers_nb) {
     case 0:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_2ppl_Value[i];
      }
      break;

     case 1:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_2ppl_Value[i];
      }
      break;

     case 2:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_2ppl_Value[i];
      }
      break;

     case 3:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_3ppl_Value[i];
      }
      break;

     case 4:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_4ppl_Value[i];
      }
      break;

     case 5:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_5ppl_Value[i];
      }
      break;

     default:
      for (int32_t i{0}; i < 5; i++) {
        rtb_Ct_eco_temp_thrs[i] = WaterHeatController_rtP.Ct_105L_eco_6ppl_Value[i];
      }
      break;
    }
  }

  switch (*WaterHeatControl_rtu_Ss_op_mode) {
   case te_op_mode::Auto:
    if (*WaterHeatContr_rtu_Ss_tank_size == WaterHeatController_rtP.L180_Value) {
      switch (*WaterHeatControl_rtu_Ns_pers_nb) {
       case 0:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_2ppl_Value[i];
        }
        break;

       case 1:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_2ppl_Value[i];
        }
        break;

       case 2:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_2ppl_Value[i];
        }
        break;

       case 3:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_3ppl_Value[i];
        }
        break;

       case 4:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_4ppl_Value[i];
        }
        break;

       case 5:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_5ppl_Value[i];
        }
        break;

       default:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_180L_auto_6ppl_Value[i];
        }
        break;
      }
    } else {
      switch (*WaterHeatControl_rtu_Ns_pers_nb) {
       case 0:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_2ppl_Value[i];
        }
        break;

       case 1:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_2ppl_Value[i];
        }
        break;

       case 2:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_2ppl_Value[i];
        }
        break;

       case 3:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_3ppl_Value[i];
        }
        break;

       case 4:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_4ppl_Value[i];
        }
        break;

       case 5:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_5ppl_Value[i];
        }
        break;

       default:
        for (int32_t i{0}; i < 5; i++) {
          WaterHeatController_DW.TempThreshold[i] = WaterHeatController_rtP.Ct_105L_auto_6ppl_Value[i];
        }
        break;
      }
    }
    break;

   case te_op_mode::FullElec:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;

   case te_op_mode::Eco:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;

   case te_op_mode::Boost:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = WaterHeatController_DW.Ct_temp_thrs_dly_DSTATE[i];
    }
    break;

   case te_op_mode::Hybrid:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;

   case te_op_mode::AntiLegionella:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;

   case te_op_mode::Holidays:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;

   default:
    for (int32_t i{0}; i < 5; i++) {
      WaterHeatController_DW.TempThreshold[i] = rtb_Ct_eco_temp_thrs[i];
    }
    break;
  }

  for (int32_t i{0}; i < 5; i++) {
    WaterHeatController_DW.Ct_temp_thrs_dly_DSTATE[i] = WaterHeatController_DW.TempThreshold[i];
  }
}

// System initialize for referenced model: 'WaterHeatController'
void WaterHeatController::init(void)
{
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_rtP.UnitDelay1_InitialCondition;
  WaterHeatController_DW.exhstTempProt_DSTATE = WaterHeatController_rtP.exhstTempProt_InitialCondition;
  WaterHeatController_DW.antiShort_DSTATE = WaterHeatController_rtP.antiShort_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE = WaterHeatController_rtP.UnitDelay_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE_a = WaterHeatController_rtP.UnitDelay_InitialCondition_m;
  TevapRegulatorMDLOBJ1.init(WaterHeatController_rtP.TevapRegulator_InitVal);
  WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd = WaterHeatController_rtP.Cs_ctrl_temp_evap_spd_cmd_Y0;
  TevapRegulatorMDLOBJ2.init(WaterHeatController_rtP.TevapRegulator_InitVal_a);
  WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd = WaterHeatController_rtP.Cs_ctrl_tref_max_spd_cmd_Y0;
  TdownminRegMDLOBJ3.init(WaterHeatController_rtP.TdownminReg_InitVal);
  Wate_ModeThresholdTempCalc_Init();
  DerivatorMDLOBJ10.init(WaterHeatController_rtP.Derivator_initVal);
  WaterHeatController_DW.Derivator_h = WaterHeatController_rtP.Cs_tank_down_temp_der_Y0;
  firstOrderTFMDLOBJ16.init(WaterHeatController_rtP.filterSp1_initVal);
  firstOrderTFMDLOBJ17.init(WaterHeatController_rtP.filterSp2_initVal);
  firstOrderTFMDLOBJ13.init(WaterHeatController_rtP.pumpFreqFilt_initVal);
  firstOrderTFMDLOBJ14.init(WaterHeatController_rtP.pumpFreqFilt1_initVal);
  HysteresisMDLOBJ5.init(WaterHeatController_rtP.HysteresisProtectTankDefrost_of);
}

// System reset for referenced model: 'WaterHeatController'
void WaterHeatController::reset(void)
{
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_rtP.UnitDelay1_InitialCondition;
  WaterHeatController_DW.exhstTempProt_DSTATE = WaterHeatController_rtP.exhstTempProt_InitialCondition;
  WaterHeatController_DW.antiShort_DSTATE = WaterHeatController_rtP.antiShort_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE = WaterHeatController_rtP.UnitDelay_InitialCondition;
  WaterHeatController_DW.UnitDelay_DSTATE_a = WaterHeatController_rtP.UnitDelay_InitialCondition_m;
  evapTempProtDelayMDLOBJ8.reset();
  evapTempProtDisable_MDLOBJ9.reset();
  R_S_FlipFlopMDLOBJ7.reset();
  Wat_ModeThresholdTempCalc_Reset();
  WaterHeatController_DW.temporalCounter_i1 = 0U;
  WaterHeatController_DW.is_active_c3_WaterHeatControlle = 0U;
  ModelMDLOBJ11.reset();
  R_S_FlipFlopMDLOBJ12.reset();
  WaterHeatController_DW.is_active_c2_WaterHeatControlle = 0U;
  WaterHeatController_DW.is_c2_WaterHeatController = 0;
  firstOrderTFMDLOBJ16.reset(WaterHeatController_rtP.filterSp1_initVal);
  firstOrderTFMDLOBJ17.reset(WaterHeatController_rtP.filterSp2_initVal);
  firstOrderTFMDLOBJ13.reset(WaterHeatController_rtP.pumpFreqFilt_initVal);
  firstOrderTFMDLOBJ14.reset(WaterHeatController_rtP.pumpFreqFilt1_initVal);
  PumpOFFdetectMDLOBJ6.reset();
  AntiShortCycleMDLOBJ4.reset();
  HysteresisMDLOBJ5.reset(WaterHeatController_rtP.HysteresisProtectTankDefrost_of);
}

// Output and update for referenced model: 'WaterHeatController'
void WaterHeatController::step(const te_tank_size *rtu_Ss_tank_size, const ta_temp *rtu_Cs_tank_down_temp, const ta_temp *rtu_Cs_tank_up_temp, const ta_temp
  *rtu_Cs_pump_evap_temp, const ta_temp *rtu_Cs_pump_xhst_temp, const te_op_mode *rtu_Ss_op_mode, const ta_pers_nb *rtu_Ns_pers_nb, ta_rot_spd
  *rty_Cs_heat_pump_rot_spd_sp)
{
  // local block i/o variables
  float rtb_output;
  float rtb_output_k;
  float rtb_Saturation_o1;
  float rtb_Saturation_o2;
  float rtb_output_a;
  float rtb_output_e;
  float rtb_Out;
  float rtb_final_cmd;
  float rtb_ovrFlwVal;
  float rtb_final_cmd_p;
  float rtb_ovrFlwVal_l;
  float rtb_final_cmd_k;
  float rtb_ovrFlwVal_k;
  uint8_t rtb_Out_a;
  uint8_t rtb_Out_c;
  uint8_t rtb_Out_h;
  bool rtb_stt;
  bool rtb_Out_cb;
  bool rtb_stt_l;
  bool rtb_Out_c1;
  float rtb_DataTypeConversion;
  float rtb_DataTypeConversion1;
  float rtb_DataTypeConversion2;
  float rtb_DataTypeConversion3;
  float rtb_DataTypeConversion5;
  float rtb_Product;
  float rtb_Product1;
  float rtb_SpdMaxPID;
  float rtb_SpdMinPID;
  float rtb_TdwnToSingle;
  float rtb_delay;
  float rtb_isUseRecSwitch;
  float rtb_maxDerat;
  float rtb_maxDerat_e;
  float rtb_mesTdownTank;
  float rtb_mesTevap;
  float rtb_mesTpumpXhst;
  float rtb_minDerat;
  float rtb_minDerat_g;
  float rtb_setPointTankDownTemp;
  float rtb_setPointTevap;
  float rtb_setPointTrefMax;
  uint32_t bpIdx;
  ta_rot_spd rtb_Cs_tank_lvl_spd_pump_sp;
  ta_temp rtb_Switch;
  ta_temp rtb_Tref_max;
  uint16_t rtb_isAntiShortCycleDelay;
  uint8_t rtb_DataTypeConversion3_i;
  uint8_t rtb_DataTypeConversion4;
  uint8_t rtb_DataTypeConversion_n;
  uint8_t rtb_uuint8;
  bool rtb_Bs_soft_draw;
  bool rtb_Bs_tank_empt;
  bool rtb_Bs_tank_empty;
  bool rtb_Bs_tank_full;
  bool rtb_Bs_tank_low;
  bool rtb_DataTypeConversion5_k;
  bool rtb_DataTypeConversion6;
  bool rtb_ProtectPump;
  bool rtb_RelationalOperator;
  bool rtb_RelationalOperator1;
  bool rtb_RelationalOperator1_i;
  bool rtb_RelationalOperator2_j;
  bool rtb_RelationalOperator3;
  bool rtb_RelationalOperator4;
  bool rtb_RelationalOperator5;
  bool rtb_RelationalOperator6;
  bool rtb_RelationalOperator8;
  te_pump_mode tmp;
  te_tank_lvl rtb_TankLevel;
  WaterHeatContr_rtu_Ss_tank_size = rtu_Ss_tank_size;
  WaterHeatControl_rtu_Ss_op_mode = rtu_Ss_op_mode;
  WaterHeatControl_rtu_Ns_pers_nb = rtu_Ns_pers_nb;
  rtb_RelationalOperator = (*rtu_Cs_pump_evap_temp < WaterHeatController_rtP.Constant4_Value);
  rtb_DataTypeConversion3_i = rtb_RelationalOperator;
  evapTempProtDelayMDLOBJ8.step(&rtb_DataTypeConversion3_i, &rtb_Out_a, WaterHeatController_rtP.evapTempProtDelay_Delay,
    WaterHeatController_rtP.evapTempProtDelay_TimeStep);
  rtb_DataTypeConversion5_k = (rtb_Out_a != 0);
  rtb_RelationalOperator3 = (*rtu_Cs_pump_evap_temp >= WaterHeatController_rtP.Constant6_Value);
  rtb_DataTypeConversion4 = ((WaterHeatController_DW.UnitDelay1_DSTATE >= WaterHeatController_rtP.Constant5_Value) && rtb_RelationalOperator3);
  evapTempProtDisable_MDLOBJ9.step(&rtb_DataTypeConversion4, &rtb_Out_c, WaterHeatController_rtP.evapTempProtDisable_Delay_Delay,
    WaterHeatController_rtP.evapTempProtDisable_Delay_TimeS);
  rtb_DataTypeConversion6 = (rtb_Out_c != 0);
  R_S_FlipFlopMDLOBJ7.step(&rtb_DataTypeConversion5_k, &rtb_DataTypeConversion6, &rtb_stt);
  if (rtb_stt) {
    rtb_setPointTevap = WaterHeatController_rtP.Temp_2C_SetPointPID_Value;
    rtb_mesTevap = *rtu_Cs_pump_evap_temp;
    rtb_maxDerat_e = WaterHeatController_rtP.maxDerat_Value;
    rtb_minDerat_g = WaterHeatController_rtP.minDerat_Value;
    TevapRegulatorMDLOBJ1.step(&rtb_setPointTevap, &rtb_mesTevap, &rtb_maxDerat_e, &rtb_minDerat_g, (const_cast<bool*>(&WaterHeatController_BGND)),
      &rtb_final_cmd_k, &rtb_ovrFlwVal_k, WaterHeatController_rtP.TevapRegulator_InitVal, WaterHeatController_rtP.TevapRegulator_Kawu,
      WaterHeatController_rtP.TevapRegulator_Kd, WaterHeatController_rtP.TevapRegulator_Ki, WaterHeatController_rtP.TevapRegulator_Kp, rtP_WaterHeatCtrl_tick_C,
      WaterHeatController_rtP.TevapRegulator_Tau_f);
    WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd = (static_cast<float>(WaterHeatController_rtP.max_Value) - rtb_final_cmd_k) / static_cast<float>
      (WaterHeatController_rtP.max_Value);
  }

  rtb_Tref_max = look1_is16lu32n16Ds32_binlcs(*rtu_Cs_pump_evap_temp, WaterHeatController_rtP.Tref_max_bp01Data, WaterHeatController_rtP.Tref_max_tableData, 6U);
  if (!WaterHeatController_DW.exhstTempProt_DSTATE) {
    rtb_setPointTrefMax = static_cast<int16_t>(rtb_Tref_max - WaterHeatController_rtP.Constant2_Value);
    rtb_mesTpumpXhst = *rtu_Cs_pump_xhst_temp;
    rtb_maxDerat = WaterHeatController_rtP.maxDerat_Value_e;
    rtb_minDerat = WaterHeatController_rtP.minDerat_Value_m;
    TevapRegulatorMDLOBJ2.step(&rtb_setPointTrefMax, &rtb_mesTpumpXhst, &rtb_maxDerat, &rtb_minDerat, (const_cast<bool*>(&WaterHeatController_BGND)),
      &rtb_final_cmd_p, &rtb_ovrFlwVal_l, WaterHeatController_rtP.TevapRegulator_InitVal_a, WaterHeatController_rtP.TevapRegulator_Kawu_j,
      WaterHeatController_rtP.TevapRegulator_Kd_b, WaterHeatController_rtP.TevapRegulator_Ki_i, WaterHeatController_rtP.TevapRegulator_Kp_d,
      rtP_WaterHeatCtrl_tick_C, WaterHeatController_rtP.TevapRegulator_Tau_f_n);
    WaterHeatController_DW.Cs_ctrl_tref_max_spd_cmd = (static_cast<float>(WaterHeatController_rtP.max_Value_d) - rtb_final_cmd_p) / static_cast<float>
      (WaterHeatController_rtP.max_Value_d);
  }

  if (WaterHeatController_rtP.Constant3_Value) {
    rtb_setPointTankDownTemp = WaterHeatController_rtP.Temp_10C_SetPointPID_Value;
    rtb_mesTdownTank = *rtu_Cs_tank_down_temp;
    rtb_SpdMinPID = WaterHeatController_rtP.SpdMinPIDTdownTank_Value;
    rtb_SpdMaxPID = WaterHeatController_rtP.SpdMaxPIDTdownTank_Value;
    TdownminRegMDLOBJ3.step(&rtb_setPointTankDownTemp, &rtb_mesTdownTank, &rtb_SpdMaxPID, &rtb_SpdMinPID, (const_cast<bool*>(&WaterHeatController_BGND)),
      &rtb_final_cmd, &rtb_ovrFlwVal, WaterHeatController_rtP.TdownminReg_InitVal, WaterHeatController_rtP.TdownminReg_Kawu,
      WaterHeatController_rtP.TdownminReg_Kd, WaterHeatController_rtP.TdownminReg_Ki, WaterHeatController_rtP.TdownminReg_Kp, rtP_WaterHeatCtrl_tick_C,
      WaterHeatController_rtP.TdownminReg_Tau_f);
  }

  rtb_ProtectPump = (*rtu_Cs_pump_xhst_temp >= rtb_Tref_max);
  WaterHeat_ModeThresholdTempCalc();
  rtb_Bs_tank_empty = (*rtu_Cs_tank_up_temp < WaterHeatController_DW.TempThreshold[4]);
  if (WaterHeatController_DW.temporalCounter_i1 < 120) {
    WaterHeatController_DW.temporalCounter_i1 = static_cast<uint8_t>(WaterHeatController_DW.temporalCounter_i1 + 1);
  }

  if (WaterHeatController_DW.is_active_c3_WaterHeatControlle == 0) {
    WaterHeatController_DW.is_active_c3_WaterHeatControlle = 1U;
    WaterHeatController_DW.temporalCounter_i1 = 0U;
  } else if (WaterHeatController_DW.temporalCounter_i1 == 120) {
    rtb_DataTypeConversion = *rtu_Cs_tank_down_temp;
    DerivatorMDLOBJ10.step(&rtb_DataTypeConversion, &WaterHeatController_DW.Derivator_h, WaterHeatController_rtP.Derivator_sampleTime);
  }

  if (WaterHeatController_DW.temporalCounter_i1 == 120) {
    WaterHeatController_DW.temporalCounter_i1 = 0U;
  }

  rtb_Bs_tank_empt = (rtb_Bs_tank_empty || (WaterHeatController_DW.Derivator_h <= WaterHeatController_rtP.Cs_hard_draw_thrs_Value));
  rtb_RelationalOperator8 = (*rtu_Cs_tank_down_temp < WaterHeatController_DW.TempThreshold[3]);
  rtb_RelationalOperator4 = (*rtu_Cs_tank_up_temp < WaterHeatController_DW.TempThreshold[2]);
  rtb_Bs_tank_low = (rtb_RelationalOperator8 || rtb_RelationalOperator4);
  rtb_RelationalOperator5 = (*rtu_Cs_tank_up_temp >= WaterHeatController_DW.TempThreshold[0]);
  rtb_RelationalOperator2_j = (WaterHeatController_DW.TempThreshold[0] < *rtu_Cs_tank_down_temp);
  rtb_Bs_tank_full = (rtb_RelationalOperator5 && rtb_RelationalOperator2_j);
  rtb_Bs_soft_draw = (WaterHeatController_DW.Derivator_h <= WaterHeatController_rtP.Cs_soft_draw_thrs_Value);
  rtb_uuint8 = (WaterHeatController_DW.UnitDelay_DSTATE == WaterHeatController_rtP.Constant1_Value);
  ModelMDLOBJ11.step(&rtb_uuint8, &rtb_Out_cb);
  R_S_FlipFlopMDLOBJ12.step(&rtb_Bs_soft_draw, &rtb_Out_cb, &rtb_stt_l);
  if (rtb_stt_l) {
    rtb_Switch = WaterHeatController_DW.TempThreshold[1];
  } else {
    rtb_Switch = WaterHeatController_DW.TempThreshold[0];
  }

  rtb_RelationalOperator6 = (*rtu_Cs_tank_up_temp >= rtb_Switch);
  rtb_RelationalOperator1 = (rtb_Switch < *rtu_Cs_tank_down_temp);
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
      } else if (rtb_Bs_tank_empt) {
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
      } else if (rtb_Bs_tank_empt) {
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
      } else if (rtb_Bs_tank_empt) {
        WaterHeatController_DW.is_c2_WaterHeatController = WaterHeatControll_IN_Tank_empty;
        rtb_TankLevel = te_tank_lvl::Empty;
      }
      break;
    }
  }

  rtb_DataTypeConversion3 = WaterHeatController_rtP.Cs_rot_spd_pump_max_C_Value;
  rtb_DataTypeConversion5 = WaterHeatController_rtP.Cs_rot_spd_pump_min_C_Value;
  bpIdx = plook_u32s16_bincka(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.CartoSetPointTankCold_bp01Data, 7U);
  rtb_DataTypeConversion1 = WaterHeatController_rtP.CartoSetPointTankCold_tableData[bpIdx];
  firstOrderTFMDLOBJ16.step(&rtb_DataTypeConversion1, &rtb_output, WaterHeatController_rtP.filterSp1_K, WaterHeatController_rtP.filterSp1_Tau,
    WaterHeatController_rtP.filterSp1_sampleTime);
  bpIdx = plook_u32s16_bincka(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.CartoSetPointTankCold_low_bp01D, 7U);
  rtb_DataTypeConversion2 = WaterHeatController_rtP.CartoSetPointTankCold_low_table[bpIdx];
  firstOrderTFMDLOBJ17.step(&rtb_DataTypeConversion2, &rtb_output_k, WaterHeatController_rtP.filterSp2_K, WaterHeatController_rtP.filterSp2_Tau,
    WaterHeatController_rtP.filterSp2_sampleTime);
  if (rtb_stt_l) {
    rtb_isUseRecSwitch = rtb_output;
  } else {
    rtb_isUseRecSwitch = rtb_output_k;
  }

  SaturationMDLOBJ15.step(&rtb_DataTypeConversion3, &rtb_DataTypeConversion5, &rtb_isUseRecSwitch, &rtb_Saturation_o1, &rtb_Saturation_o2);
  switch (rtb_TankLevel) {
   case te_tank_lvl::Cold:
    rtb_Cs_tank_lvl_spd_pump_sp = static_cast<ta_rot_spd>(rtb_Saturation_o1);
    break;

   case te_tank_lvl::Hot:
    rtb_Cs_tank_lvl_spd_pump_sp = WaterHeatController_rtP.SpdOFF1_Value;
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
      tmp = WaterHeatController_rtP.TrefReg_Value;
    } else if (rtb_stt) {
      tmp = WaterHeatController_rtP.TevapReg_Value;
    } else {
      tmp = WaterHeatController_rtP.TankLevelReg_Value;
    }

    switch (tmp) {
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
  rtb_DataTypeConversion_n = rtb_RelationalOperator1_i;
  PumpOFFdetectMDLOBJ6.step(&rtb_DataTypeConversion_n, &rtb_Out_c1);
  if (rtb_Out_c1) {
    rtb_isAntiShortCycleDelay = look1_is16lu32n16tu16_binlcse(*rtu_Cs_tank_down_temp, WaterHeatController_rtP.TimeToPumpStartup_bp01Data,
      WaterHeatController_rtP.TimeToPumpStartup_tableData, 5U);
  } else {
    rtb_isAntiShortCycleDelay = WaterHeatController_DW.UnitDelay_DSTATE_a;
  }

  rtb_delay = rtb_isAntiShortCycleDelay;
  AntiShortCycleMDLOBJ4.step(&rtb_DataTypeConversion_n, &rtb_delay, &rtb_Out_h, rtP_WaterHeatCtrl_tick_C);
  rtb_TdwnToSingle = *rtu_Cs_tank_down_temp;
  HysteresisMDLOBJ5.step(&rtb_TdwnToSingle, &rtb_Out, WaterHeatController_rtP.HysteresisProtectTankDefrost_of,
    WaterHeatController_rtP.HysteresisProtectTankDefrost_on, WaterHeatController_rtP.HysteresisProtectTankDefrost_sw,
    WaterHeatController_rtP.HysteresisProtectTankDefrost__g);
  WaterHeatController_DW.UnitDelay1_DSTATE = WaterHeatController_DW.Cs_ctrl_temp_evap_spd_cmd;
  WaterHeatController_DW.exhstTempProt_DSTATE = (rtb_ProtectPump && (!rtb_RelationalOperator1_i));
  WaterHeatController_DW.antiShort_DSTATE = ((rtb_Out_h == 0) && rtb_RelationalOperator1_i);
  WaterHeatController_DW.UnitDelay_DSTATE = *rty_Cs_heat_pump_rot_spd_sp;
  WaterHeatController_DW.UnitDelay_DSTATE_a = rtb_isAntiShortCycleDelay;
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
