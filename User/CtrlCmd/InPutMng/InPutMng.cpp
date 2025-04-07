//
// File: InPutMng.cpp
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.319
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:25:17 2025
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
#include "InPutMng.h"
#include "InPutMng_types.h"
#include <stdint.h>
#include <cmath>
#include <stdbool.h>
#include "InPutMng_private.h"
#include "look1_is16lu16n16Ds32_binlcas.h"
#include "firstOrderTF.h"
#include "SensErrorDetection.h"

InPutMng::P_InPutMng_T InPutMng::InPutMng_rtP{
  // Mask Parameter: FTempEvap_K
  //  Referenced by: '<S25>/firstOrderTF'

  1.0F,

  // Mask Parameter: FTempDown_K
  //  Referenced by: '<S24>/firstOrderTF'

  1.0F,

  // Mask Parameter: FTempUp_K
  //  Referenced by: '<S26>/firstOrderTF'

  1.0F,

  // Mask Parameter: FTempXhst_K
  //  Referenced by: '<S28>/firstOrderTF'

  1.0F,

  // Mask Parameter: FTempVent_K
  //  Referenced by: '<S27>/firstOrderTF'

  1.0F,

  // Mask Parameter: FPresVent_K
  //  Referenced by: '<S22>/firstOrderTF'

  1.0F,

  // Mask Parameter: FRotSpd_K
  //  Referenced by: '<S23>/firstOrderTF'

  1.0F,

  // Mask Parameter: FTempEvap_Tau
  //  Referenced by: '<S25>/firstOrderTF'

  2.0F,

  // Mask Parameter: FTempDown_Tau
  //  Referenced by: '<S24>/firstOrderTF'

  5.0F,

  // Mask Parameter: FTempUp_Tau
  //  Referenced by: '<S26>/firstOrderTF'

  5.0F,

  // Mask Parameter: FTempXhst_Tau
  //  Referenced by: '<S28>/firstOrderTF'

  5.0F,

  // Mask Parameter: FTempVent_Tau
  //  Referenced by: '<S27>/firstOrderTF'

  5.0F,

  // Mask Parameter: FPresVent_Tau
  //  Referenced by: '<S22>/firstOrderTF'

  5.0F,

  // Mask Parameter: FRotSpd_Tau
  //  Referenced by: '<S23>/firstOrderTF'

  5.0F,

  // Mask Parameter: ErrTempDown_defVal
  //  Referenced by: '<S34>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempUp_defVal
  //  Referenced by: '<S36>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempXhst_defVal
  //  Referenced by: '<S38>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempEvap_defVal
  //  Referenced by: '<S35>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempVent_defVal
  //  Referenced by: '<S37>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrPresVent_defVal
  //  Referenced by: '<S32>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrRotSpd_defVal
  //  Referenced by: '<S33>/SensErrorDetection'

  0.0F,

  // Mask Parameter: FTempEvap_initVal
  //  Referenced by: '<S25>/firstOrderTF'

  0.0F,

  // Mask Parameter: FTempDown_initVal
  //  Referenced by: '<S24>/firstOrderTF'

  0.0F,

  // Mask Parameter: FTempUp_initVal
  //  Referenced by: '<S26>/firstOrderTF'

  0.0F,

  // Mask Parameter: FTempXhst_initVal
  //  Referenced by: '<S28>/firstOrderTF'

  0.0F,

  // Mask Parameter: FTempVent_initVal
  //  Referenced by: '<S27>/firstOrderTF'

  0.0F,

  // Mask Parameter: FPresVent_initVal
  //  Referenced by: '<S22>/firstOrderTF'

  0.0F,

  // Mask Parameter: FRotSpd_initVal
  //  Referenced by: '<S23>/firstOrderTF'

  0.0F,

  // Mask Parameter: ErrTempDown_staticRstTol
  //  Referenced by: '<S34>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempUp_staticRstTol
  //  Referenced by: '<S36>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempXhst_staticRstTol
  //  Referenced by: '<S38>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempEvap_staticRstTol
  //  Referenced by: '<S35>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempVent_staticRstTol
  //  Referenced by: '<S37>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrPresVent_staticRstTol
  //  Referenced by: '<S32>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrRotSpd_staticRstTol
  //  Referenced by: '<S33>/SensErrorDetection'

  0.0F,

  // Mask Parameter: ErrTempDown_maxThrs
  //  Referenced by: '<S34>/SensErrorDetection'

  1050,

  // Mask Parameter: ErrTempUp_maxThrs
  //  Referenced by: '<S36>/SensErrorDetection'

  1050,

  // Mask Parameter: ErrTempXhst_maxThrs
  //  Referenced by: '<S38>/SensErrorDetection'

  1050,

  // Mask Parameter: ErrTempEvap_maxThrs
  //  Referenced by: '<S35>/SensErrorDetection'

  1050,

  // Mask Parameter: ErrTempVent_maxThrs
  //  Referenced by: '<S37>/SensErrorDetection'

  1050,

  // Mask Parameter: ErrTempDown_maxVar
  //  Referenced by: '<S34>/SensErrorDetection'

  300,

  // Mask Parameter: ErrTempUp_maxVar
  //  Referenced by: '<S36>/SensErrorDetection'

  300,

  // Mask Parameter: ErrTempXhst_maxVar
  //  Referenced by: '<S38>/SensErrorDetection'

  300,

  // Mask Parameter: ErrTempEvap_maxVar
  //  Referenced by: '<S35>/SensErrorDetection'

  300,

  // Mask Parameter: ErrTempVent_maxVar
  //  Referenced by: '<S37>/SensErrorDetection'

  300,

  // Mask Parameter: ErrTempDown_minThrs
  //  Referenced by: '<S34>/SensErrorDetection'

  -200,

  // Mask Parameter: ErrTempUp_minThrs
  //  Referenced by: '<S36>/SensErrorDetection'

  -200,

  // Mask Parameter: ErrTempXhst_minThrs
  //  Referenced by: '<S38>/SensErrorDetection'

  -200,

  // Mask Parameter: ErrTempEvap_minThrs
  //  Referenced by: '<S35>/SensErrorDetection'

  -200,

  // Mask Parameter: ErrTempVent_minThrs
  //  Referenced by: '<S37>/SensErrorDetection'

  -200,

  // Mask Parameter: ErrPresVent_maxThrs
  //  Referenced by: '<S32>/SensErrorDetection'

  3000U,

  // Mask Parameter: ErrRotSpd_maxThrs
  //  Referenced by: '<S33>/SensErrorDetection'

  4000U,

  // Mask Parameter: ErrPresVent_maxVar
  //  Referenced by: '<S32>/SensErrorDetection'

  500U,

  // Mask Parameter: ErrRotSpd_maxVar
  //  Referenced by: '<S33>/SensErrorDetection'

  500U,

  // Mask Parameter: ErrPresVent_minThrs
  //  Referenced by: '<S32>/SensErrorDetection'

  0U,

  // Mask Parameter: ErrRotSpd_minThrs
  //  Referenced by: '<S33>/SensErrorDetection'

  0U,

  // Mask Parameter: ErrTempDown_opScaleRstTime
  //  Referenced by: '<S34>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempUp_opScaleRstTime
  //  Referenced by: '<S36>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempXhst_opScaleRstTime
  //  Referenced by: '<S38>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempEvap_opScaleRstTime
  //  Referenced by: '<S35>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempVent_opScaleRstTime
  //  Referenced by: '<S37>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrPresVent_opScaleRstTime
  //  Referenced by: '<S32>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrRotSpd_opScaleRstTime
  //  Referenced by: '<S33>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempDown_opScaleSetTime
  //  Referenced by: '<S34>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempUp_opScaleSetTime
  //  Referenced by: '<S36>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempXhst_opScaleSetTime
  //  Referenced by: '<S38>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempEvap_opScaleSetTime
  //  Referenced by: '<S35>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempVent_opScaleSetTime
  //  Referenced by: '<S37>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrPresVent_opScaleSetTime
  //  Referenced by: '<S32>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrRotSpd_opScaleSetTime
  //  Referenced by: '<S33>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempDown_staticRstTime
  //  Referenced by: '<S34>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrTempUp_staticRstTime
  //  Referenced by: '<S36>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrTempXhst_staticRstTime
  //  Referenced by: '<S38>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrTempEvap_staticRstTime
  //  Referenced by: '<S35>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrTempVent_staticRstTime
  //  Referenced by: '<S37>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrPresVent_staticRstTime
  //  Referenced by: '<S32>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrRotSpd_staticRstTime
  //  Referenced by: '<S33>/SensErrorDetection'

  20U,

  // Mask Parameter: ErrTempDown_staticSetTime
  //  Referenced by: '<S34>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrTempUp_staticSetTime
  //  Referenced by: '<S36>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrTempXhst_staticSetTime
  //  Referenced by: '<S38>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrTempEvap_staticSetTime
  //  Referenced by: '<S35>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrTempVent_staticSetTime
  //  Referenced by: '<S37>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrPresVent_staticSetTime
  //  Referenced by: '<S32>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrRotSpd_staticSetTime
  //  Referenced by: '<S33>/SensErrorDetection'

  200U,

  // Mask Parameter: ErrTempDown_varRstTime
  //  Referenced by: '<S34>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempUp_varRstTime
  //  Referenced by: '<S36>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempXhst_varRstTime
  //  Referenced by: '<S38>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempEvap_varRstTime
  //  Referenced by: '<S35>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempVent_varRstTime
  //  Referenced by: '<S37>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrPresVent_varRstTime
  //  Referenced by: '<S32>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrRotSpd_varRstTime
  //  Referenced by: '<S33>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempDown_varSetTime
  //  Referenced by: '<S34>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempUp_varSetTime
  //  Referenced by: '<S36>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempXhst_varSetTime
  //  Referenced by: '<S38>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempEvap_varSetTime
  //  Referenced by: '<S35>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrTempVent_varSetTime
  //  Referenced by: '<S37>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrPresVent_varSetTime
  //  Referenced by: '<S32>/SensErrorDetection'

  30U,

  // Mask Parameter: ErrRotSpd_varSetTime
  //  Referenced by: '<S33>/SensErrorDetection'

  30U,

  // Computed Parameter: const1_Value
  //  Referenced by: '<S16>/const1'

  0.0F,

  // Computed Parameter: const2_Value
  //  Referenced by: '<S16>/const2'

  0.0F,

  // Computed Parameter: const3_Value
  //  Referenced by: '<S14>/const3'

  0.0F,

  // Computed Parameter: const1_Value_f
  //  Referenced by: '<S14>/const1'

  0.0F,

  // Computed Parameter: const2_Value_f
  //  Referenced by: '<S14>/const2'

  0.0F,

  // Computed Parameter: Tnul_Value
  //  Referenced by: '<S17>/Tnul'

  0.0F,

  // Computed Parameter: Vnull1_Value
  //  Referenced by: '<S17>/Vnull1'

  0.0F,

  // Computed Parameter: Tnul_Value_k
  //  Referenced by: '<S18>/Tnul'

  0.0F,

  // Computed Parameter: Vnull1_Value_l
  //  Referenced by: '<S18>/Vnull1'

  0.0F,

  // Computed Parameter: Tnul_Value_a
  //  Referenced by: '<S19>/Tnul'

  0.0F,

  // Computed Parameter: Vnull1_Value_i
  //  Referenced by: '<S19>/Vnull1'

  0.0F,

  // Computed Parameter: TmaxPrev_InitialCondition
  //  Referenced by: '<S13>/TmaxPrev'

  0.0F,

  // Computed Parameter: AddConstant1_Bias
  //  Referenced by: '<S13>/Add Constant1'

  -0.0009F,

  // Computed Parameter: getTcap_Gain
  //  Referenced by: '<S19>/getTcap'

  0.5F,

  // Computed Parameter: Vnull2_Value
  //  Referenced by: '<S19>/Vnull2'

  0.0F,

  // Computed Parameter: getTcap_Gain_k
  //  Referenced by: '<S18>/getTcap'

  0.5F,

  // Computed Parameter: Vnull2_Value_e
  //  Referenced by: '<S18>/Vnull2'

  0.0F,

  // Computed Parameter: getTcap_Gain_o
  //  Referenced by: '<S17>/getTcap'

  0.5F,

  // Computed Parameter: Vnull2_Value_f
  //  Referenced by: '<S17>/Vnull2'

  0.0F,

  // Expression: te_ctry.Germany
  //  Referenced by: '<S3>/inVentStopSys1'

  te_ctry::Germany,

  // Expression: te_heat_stt.Heat_Heating
  //  Referenced by: '<S8>/Constant1'

  te_heat_stt::Heat_Heating,

  // Expression: te_heat_stt.Heat_Heating
  //  Referenced by: '<S13>/Constant1'

  te_heat_stt::Heat_Heating,

  // Expression: te_heat_stt.Heat_Heating
  //  Referenced by: '<S13>/Constant3'

  te_heat_stt::Heat_Heating,

  // Expression: te_on_off.on
  //  Referenced by: '<S3>/outOnOph'

  te_on_off::on,

  // Expression: te_on_off.off
  //  Referenced by: '<S3>/outOffOph'

  te_on_off::off,

  // Expression: te_on_off.on
  //  Referenced by: '<S3>/outOnVst'

  te_on_off::on,

  // Expression: te_on_off.off
  //  Referenced by: '<S3>/outOffVst'

  te_on_off::off,

  // Expression: te_on_off.on
  //  Referenced by: '<S3>/outOnSg'

  te_on_off::on,

  // Expression: te_on_off.off
  //  Referenced by: '<S3>/outOffSg'

  te_on_off::off,

  // Expression: te_on_off.on
  //  Referenced by: '<S3>/inOnSg1'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S6>/On'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S8>/On'

  te_on_off::on,

  // Expression: te_on_off.on
  //  Referenced by: '<S9>/On'

  te_on_off::on,

  // Expression: te_tor_mode.tor_SmartGrid
  //  Referenced by: '<S3>/inSmartGrid'

  te_tor_mode::tor_SmartGrid,

  // Expression: te_tor_mode.tor_OffPeakHour
  //  Referenced by: '<S3>/inOffPeakHour'

  te_tor_mode::tor_OffPeakHour,

  // Expression: te_tor_mode.tor_VentSysStop
  //  Referenced by: '<S3>/inVentStopSys'

  te_tor_mode::tor_VentSysStop,

  // Computed Parameter: ResetValue_Value
  //  Referenced by: '<S6>/ResetValue'

  0U,

  // Computed Parameter: ResetValue_Value_i
  //  Referenced by: '<S8>/ResetValue'

  0U,

  // Computed Parameter: ElecBoosterHeaterPower_Value
  //  Referenced by: '<S8>/ElecBoosterHeaterPower'

  15000U,

  // Computed Parameter: NulPower_Value
  //  Referenced by: '<S8>/NulPower'

  0U,

  // Computed Parameter: ResetValue_Value_d
  //  Referenced by: '<S9>/ResetValue'

  0U,

  // Computed Parameter: Cs_elec_htr_bstr_cnsp_prev_Init
  //  Referenced by: '<S6>/Cs_elec_htr_bstr_cnsp_prev'

  0U,

  // Computed Parameter: Cs_elec_htr_bstr_cnsp_prev_In_a
  //  Referenced by: '<S8>/Cs_elec_htr_bstr_cnsp_prev'

  0U,

  // Computed Parameter: Cs_vent_cnsp_prev_InitialCondit
  //  Referenced by: '<S9>/Cs_vent_cnsp_prev'

  0U,

  // Computed Parameter: BpEsti_tableData
  //  Referenced by: '<S2>/BpEsti'

  { 7, 11, 16, 24, 34, 47, 63, 83, 107, 136, 171, 211, 258, 313, 376 },

  // Expression: Ct_evap_temp_C
  //  Referenced by: '<S2>/BpEsti'

  { -500, -400, -300, -200, -100, 0, 100, 200, 300, 400, 500, 600, 700, 800, 900
  },

  // Expression: Cs_in_temp_C
  //  Referenced by: '<S13>/Constant2'

  100,

  // Expression: Cs_ref_temp_C
  //  Referenced by: '<S13>/Constant'

  400,

  // Computed Parameter: const2_Value_a
  //  Referenced by: '<S13>/const2'

  0U,

  // Computed Parameter: const1_Value_a
  //  Referenced by: '<S13>/const1'

  0U,

  // Computed Parameter: Constant_Value_o
  //  Referenced by: '<S29>/Constant'

  false,

  // Computed Parameter: mergeErr_InitialOutput
  //  Referenced by: '<S31>/mergeErr'

  false,

  // Computed Parameter: Constant_Value_i
  //  Referenced by: '<S10>/Constant'

  0U,

  // Computed Parameter: Constant_Value_m
  //  Referenced by: '<S11>/Constant'

  0U,

  // Computed Parameter: Constant_Value_o2
  //  Referenced by: '<S12>/Constant'

  0U
};

// System initialize for referenced model: 'InPutMng'
void InPutMng::init(void)
{
  int32_t i;
  InPutMng_DW.TmaxPrev_DSTATE = InPutMng_rtP.TmaxPrev_InitialCondition;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DSTA =
    InPutMng_rtP.Cs_elec_htr_bstr_cnsp_prev_Init;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DS_g =
    InPutMng_rtP.Cs_elec_htr_bstr_cnsp_prev_In_a;
  InPutMng_DW.Cs_vent_cnsp_prev_DSTATE =
    InPutMng_rtP.Cs_vent_cnsp_prev_InitialCondit;
  firstOrderTFMDLOBJ4.init(InPutMng_rtP.FTempEvap_initVal);
  firstOrderTFMDLOBJ3.init(InPutMng_rtP.FTempDown_initVal);
  firstOrderTFMDLOBJ5.init(InPutMng_rtP.FTempUp_initVal);
  firstOrderTFMDLOBJ7.init(InPutMng_rtP.FTempXhst_initVal);
  firstOrderTFMDLOBJ6.init(InPutMng_rtP.FTempVent_initVal);
  firstOrderTFMDLOBJ1.init(InPutMng_rtP.FPresVent_initVal);
  for (i = 0; i < 7; i++) {
    InPutMng_DW.mergeErr[i] = InPutMng_rtP.mergeErr_InitialOutput;
  }

  firstOrderTFMDLOBJ2.init(InPutMng_rtP.FRotSpd_initVal);
}

// System reset for referenced model: 'InPutMng'
void InPutMng::reset(void)
{
  InPutMng_DW.TmaxPrev_DSTATE = InPutMng_rtP.TmaxPrev_InitialCondition;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DSTA =
    InPutMng_rtP.Cs_elec_htr_bstr_cnsp_prev_Init;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DS_g =
    InPutMng_rtP.Cs_elec_htr_bstr_cnsp_prev_In_a;
  InPutMng_DW.Cs_vent_cnsp_prev_DSTATE =
    InPutMng_rtP.Cs_vent_cnsp_prev_InitialCondit;
  firstOrderTFMDLOBJ4.reset(InPutMng_rtP.FTempEvap_initVal);
  firstOrderTFMDLOBJ3.reset(InPutMng_rtP.FTempDown_initVal);
  firstOrderTFMDLOBJ5.reset(InPutMng_rtP.FTempUp_initVal);
  firstOrderTFMDLOBJ7.reset(InPutMng_rtP.FTempXhst_initVal);
  firstOrderTFMDLOBJ6.reset(InPutMng_rtP.FTempVent_initVal);
  firstOrderTFMDLOBJ1.reset(InPutMng_rtP.FPresVent_initVal);
  firstOrderTFMDLOBJ2.reset(InPutMng_rtP.FRotSpd_initVal);
}

// Output and update for referenced model: 'InPutMng'
void InPutMng::step(const tb_InPutMng_In *rtu_InPutMng_In, tb_InPutMng_Out
                    *rty_InPutMng_Out)
{
  // local block i/o variables
  float rtb_output;
  float rtb_output_m;
  float rtb_output_i;
  float rtb_output_d;
  float rtb_output_b;
  float rtb_output_dm;
  float rtb_output_p;
  float rtb_SensErrorDetection_o1;
  float rtb_SensErrorDetection_o1_i;
  float rtb_SensErrorDetection_o1_b;
  float rtb_SensErrorDetection_o1_ig;
  float rtb_SensErrorDetection_o1_io;
  float rtb_SensErrorDetection_o1_n;
  float rtb_SensErrorDetection_o1_h;
  float rtb_PresToSing;
  float rtb_PresToSing_e;
  float rtb_RotSpdToSing;
  float rtb_RotSpdToSing_m;
  float rtb_Tcap;
  float rtb_Tcap_c;
  float rtb_Tcap_g;
  float rtb_TempToSing;
  float rtb_TempToSing1_f;
  float rtb_TempToSing2_c;
  float rtb_TempToSing3;
  float rtb_TempToSing3_a;
  float rtb_TempToSing4;
  float rtb_TempToSing4_a;
  float rtb_TempToSing5;
  float rtb_TempToSing_m;
  float rtb_TenpToSing1;
  float rtb_Tmax;
  float rtb_Vb_Switch;
  float rtb_Vol;
  float rtb_Vol_e;
  float rtb_Vol_m;
  float rtb_selectTh40Val;
  float rtb_selectTh40Val1;
  float tmp;
  float tmp_0;
  float tmp_1;
  float tmp_3;
  int32_t rtb_Tcap_p_tmp;
  int32_t rtb_Tcap_tmp;
  int32_t rtb_Vh_Switch;
  ta_pwr rtb_AddCnsp;
  ta_pwr rtb_resetSwitch;
  ta_pwr rtb_resetSwitch_d;
  ta_pwr rtb_resetSwitch_h;
  ta_pwr tmp_2;
  ta_air_pres rtb_Cs_pres_vent_cs;
  ta_rot_spd rtb_Cs_vent_rot_spd_cs;
  ta_temp rtb_Cs_pump_evap_temp;
  ta_temp rtb_Cs_pump_evap_temp_cs;
  ta_temp rtb_Cs_pump_xhst_temp_cs;
  ta_temp rtb_Cs_tank_down_temp;
  ta_temp rtb_Cs_tank_down_temp_cs;
  ta_temp rtb_Cs_tank_up_temp;
  ta_temp rtb_Cs_tank_up_temp_cs_c;
  ta_temp rtb_Cs_vent_temp_cs;
  bool rtb_Bs_tank_down_temp_err;
  bool rtb_Bs_tank_up_temp_err;
  bool rtb_isGermany;
  bool rtb_isOn_0;
  bool rtb_isOn_idx_0;
  bool rtb_isSmartGridAndOn_idx_0;
  if (rtu_InPutMng_In->Bs_pump_xhst_pres_simu_ena) {
    rty_InPutMng_Out->Cs_pump_xhst_pres =
      rtu_InPutMng_In->Cs_pump_xhst_pres_simu;
  } else {
    rty_InPutMng_Out->Cs_pump_xhst_pres = 0U;
  }

  if (rtu_InPutMng_In->Bs_err_dtct_shnt_ena) {
    rtb_Cs_tank_down_temp_cs = rtu_InPutMng_In->Cs_tank_down_temp_raw;
    rtb_Cs_tank_up_temp_cs_c = rtu_InPutMng_In->Cs_tank_up_temp_raw;
    rtb_Cs_pump_xhst_temp_cs = rtu_InPutMng_In->Cs_pump_xhst_temp_raw;
    rtb_Cs_pump_evap_temp_cs = rtu_InPutMng_In->Cs_pump_evap_temp_raw;
    rtb_Cs_vent_temp_cs = rtu_InPutMng_In->Cs_vent_temp_raw;
    rtb_Cs_pres_vent_cs = rtu_InPutMng_In->Cs_vent_pres_raw;
    rtb_Cs_vent_rot_spd_cs = rtu_InPutMng_In->Cs_vent_rot_spd_raw;
    InPutMng_DW.mergeErr[5] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[2] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[0] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[1] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[6] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[3] = InPutMng_rtP.Constant_Value_o;
    InPutMng_DW.mergeErr[4] = InPutMng_rtP.Constant_Value_o;
  } else {
    rtb_TempToSing_m = rtu_InPutMng_In->Cs_tank_down_temp_raw;
    SensErrorDetectionMDLOBJ10.step(&rtb_TempToSing_m,
      &rtb_SensErrorDetection_o1, &InPutMng_DW.mergeErr[0],
      InPutMng_rtP.ErrTempDown_defVal, InPutMng_rtP.ErrTempDown_opScaleRstTime,
      InPutMng_rtP.ErrTempDown_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrTempDown_staticRstTime,
      InPutMng_rtP.ErrTempDown_staticRstTol,
      InPutMng_rtP.ErrTempDown_staticSetTime, InPutMng_rtP.ErrTempDown_maxThrs,
      InPutMng_rtP.ErrTempDown_minThrs, InPutMng_rtP.ErrTempDown_varRstTime,
      InPutMng_rtP.ErrTempDown_varSetTime, InPutMng_rtP.ErrTempDown_maxVar);
    rtb_Cs_tank_down_temp_cs = static_cast<ta_temp>(std::floor
      (rtb_SensErrorDetection_o1));
    rtb_TempToSing1_f = rtu_InPutMng_In->Cs_tank_up_temp_raw;
    SensErrorDetectionMDLOBJ12.step(&rtb_TempToSing1_f,
      &rtb_SensErrorDetection_o1_i, &InPutMng_DW.mergeErr[1],
      InPutMng_rtP.ErrTempUp_defVal, InPutMng_rtP.ErrTempUp_opScaleRstTime,
      InPutMng_rtP.ErrTempUp_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrTempUp_staticRstTime, InPutMng_rtP.ErrTempUp_staticRstTol,
      InPutMng_rtP.ErrTempUp_staticSetTime, InPutMng_rtP.ErrTempUp_maxThrs,
      InPutMng_rtP.ErrTempUp_minThrs, InPutMng_rtP.ErrTempUp_varRstTime,
      InPutMng_rtP.ErrTempUp_varSetTime, InPutMng_rtP.ErrTempUp_maxVar);
    rtb_Cs_tank_up_temp_cs_c = static_cast<ta_temp>(std::floor
      (rtb_SensErrorDetection_o1_i));
    rtb_TempToSing2_c = rtu_InPutMng_In->Cs_pump_xhst_temp_raw;
    SensErrorDetectionMDLOBJ14.step(&rtb_TempToSing2_c,
      &rtb_SensErrorDetection_o1_b, &InPutMng_DW.mergeErr[2],
      InPutMng_rtP.ErrTempXhst_defVal, InPutMng_rtP.ErrTempXhst_opScaleRstTime,
      InPutMng_rtP.ErrTempXhst_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrTempXhst_staticRstTime,
      InPutMng_rtP.ErrTempXhst_staticRstTol,
      InPutMng_rtP.ErrTempXhst_staticSetTime, InPutMng_rtP.ErrTempXhst_maxThrs,
      InPutMng_rtP.ErrTempXhst_minThrs, InPutMng_rtP.ErrTempXhst_varRstTime,
      InPutMng_rtP.ErrTempXhst_varSetTime, InPutMng_rtP.ErrTempXhst_maxVar);
    rtb_Cs_pump_xhst_temp_cs = static_cast<ta_temp>(std::floor
      (rtb_SensErrorDetection_o1_b));
    rtb_TempToSing3_a = rtu_InPutMng_In->Cs_pump_evap_temp_raw;
    SensErrorDetectionMDLOBJ11.step(&rtb_TempToSing3_a,
      &rtb_SensErrorDetection_o1_ig, &InPutMng_DW.mergeErr[3],
      InPutMng_rtP.ErrTempEvap_defVal, InPutMng_rtP.ErrTempEvap_opScaleRstTime,
      InPutMng_rtP.ErrTempEvap_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrTempEvap_staticRstTime,
      InPutMng_rtP.ErrTempEvap_staticRstTol,
      InPutMng_rtP.ErrTempEvap_staticSetTime, InPutMng_rtP.ErrTempEvap_maxThrs,
      InPutMng_rtP.ErrTempEvap_minThrs, InPutMng_rtP.ErrTempEvap_varRstTime,
      InPutMng_rtP.ErrTempEvap_varSetTime, InPutMng_rtP.ErrTempEvap_maxVar);
    rtb_Cs_pump_evap_temp_cs = static_cast<ta_temp>(std::floor
      (rtb_SensErrorDetection_o1_ig));
    rtb_TempToSing4_a = rtu_InPutMng_In->Cs_vent_temp_raw;
    SensErrorDetectionMDLOBJ13.step(&rtb_TempToSing4_a,
      &rtb_SensErrorDetection_o1_io, &InPutMng_DW.mergeErr[4],
      InPutMng_rtP.ErrTempVent_defVal, InPutMng_rtP.ErrTempVent_opScaleRstTime,
      InPutMng_rtP.ErrTempVent_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrTempVent_staticRstTime,
      InPutMng_rtP.ErrTempVent_staticRstTol,
      InPutMng_rtP.ErrTempVent_staticSetTime, InPutMng_rtP.ErrTempVent_maxThrs,
      InPutMng_rtP.ErrTempVent_minThrs, InPutMng_rtP.ErrTempVent_varRstTime,
      InPutMng_rtP.ErrTempVent_varSetTime, InPutMng_rtP.ErrTempVent_maxVar);
    rtb_Cs_vent_temp_cs = static_cast<ta_temp>(std::floor
      (rtb_SensErrorDetection_o1_io));
    rtb_PresToSing_e = rtu_InPutMng_In->Cs_vent_pres_raw;
    SensErrorDetectionMDLOBJ8.step(&rtb_PresToSing_e,
      &rtb_SensErrorDetection_o1_n, &InPutMng_DW.mergeErr[5],
      InPutMng_rtP.ErrPresVent_defVal, InPutMng_rtP.ErrPresVent_opScaleRstTime,
      InPutMng_rtP.ErrPresVent_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrPresVent_staticRstTime,
      InPutMng_rtP.ErrPresVent_staticRstTol,
      InPutMng_rtP.ErrPresVent_staticSetTime, InPutMng_rtP.ErrPresVent_maxThrs,
      InPutMng_rtP.ErrPresVent_minThrs, InPutMng_rtP.ErrPresVent_varRstTime,
      InPutMng_rtP.ErrPresVent_varSetTime, InPutMng_rtP.ErrPresVent_maxVar);
    rtb_Cs_pres_vent_cs = static_cast<ta_air_pres>(rtb_SensErrorDetection_o1_n);
    rtb_RotSpdToSing_m = rtu_InPutMng_In->Cs_vent_rot_spd_raw;
    SensErrorDetectionMDLOBJ9.step(&rtb_RotSpdToSing_m,
      &rtb_SensErrorDetection_o1_h, &InPutMng_DW.mergeErr[6],
      InPutMng_rtP.ErrRotSpd_defVal, InPutMng_rtP.ErrRotSpd_opScaleRstTime,
      InPutMng_rtP.ErrRotSpd_opScaleSetTime, rtP_input_mng_tick,
      InPutMng_rtP.ErrRotSpd_staticRstTime, InPutMng_rtP.ErrRotSpd_staticRstTol,
      InPutMng_rtP.ErrRotSpd_staticSetTime, InPutMng_rtP.ErrRotSpd_maxThrs,
      InPutMng_rtP.ErrRotSpd_minThrs, InPutMng_rtP.ErrRotSpd_varRstTime,
      InPutMng_rtP.ErrRotSpd_varSetTime, InPutMng_rtP.ErrRotSpd_maxVar);
    rtb_Cs_vent_rot_spd_cs = static_cast<ta_rot_spd>(rtb_SensErrorDetection_o1_h);
  }

  rtb_TempToSing4 = rtb_Cs_pump_evap_temp_cs;
  firstOrderTFMDLOBJ4.step(&rtb_TempToSing4, &rtb_output,
    InPutMng_rtP.FTempEvap_K, InPutMng_rtP.FTempEvap_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_pump_evap_temp_simu_ena) {
    rtb_Cs_pump_evap_temp = rtu_InPutMng_In->Cs_pump_evap_temp_simu;
  } else {
    rtb_Cs_pump_evap_temp = static_cast<ta_temp>(std::floor(rtb_output));
  }

  if (rtu_InPutMng_In->Bs_pump_evap_pres_simu_ena) {
    rty_InPutMng_Out->Cs_pump_evap_pres =
      rtu_InPutMng_In->Cs_pump_evap_pres_simu;
  } else {
    rty_InPutMng_Out->Cs_pump_evap_pres = static_cast<ta_rfrg_pres>
      (look1_is16lu16n16Ds32_binlcas(rtb_Cs_pump_evap_temp,
        InPutMng_rtP.BpEsti_bp01Data, InPutMng_rtP.BpEsti_tableData, 14U));
  }

  rtb_TempToSing = rtb_Cs_tank_down_temp_cs;
  firstOrderTFMDLOBJ3.step(&rtb_TempToSing, &rtb_output_m,
    InPutMng_rtP.FTempDown_K, InPutMng_rtP.FTempDown_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_tank_down_temp_simu_ena) {
    rtb_Cs_tank_down_temp = rtu_InPutMng_In->Cs_tank_down_temp_simu;
  } else {
    rtb_Cs_tank_down_temp = static_cast<ta_temp>(std::floor(rtb_output_m));
  }

  rtb_TenpToSing1 = rtb_Cs_tank_up_temp_cs_c;
  firstOrderTFMDLOBJ5.step(&rtb_TenpToSing1, &rtb_output_i,
    InPutMng_rtP.FTempUp_K, InPutMng_rtP.FTempUp_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_tank_up_temp_simu_ena) {
    rtb_Cs_tank_up_temp = rtu_InPutMng_In->Cs_tank_up_temp_simu;
  } else {
    rtb_Cs_tank_up_temp = static_cast<ta_temp>(std::floor(rtb_output_i));
  }

  rtb_TempToSing3 = rtb_Cs_pump_xhst_temp_cs;
  firstOrderTFMDLOBJ7.step(&rtb_TempToSing3, &rtb_output_d,
    InPutMng_rtP.FTempXhst_K, InPutMng_rtP.FTempXhst_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_pump_xhst_temp_simu_ena) {
    rty_InPutMng_Out->Cs_pump_xhst_temp =
      rtu_InPutMng_In->Cs_pump_xhst_temp_simu;
  } else {
    rty_InPutMng_Out->Cs_pump_xhst_temp = static_cast<ta_temp>(std::floor
      (rtb_output_d));
  }

  rtb_TempToSing5 = rtb_Cs_vent_temp_cs;
  firstOrderTFMDLOBJ6.step(&rtb_TempToSing5, &rtb_output_b,
    InPutMng_rtP.FTempVent_K, InPutMng_rtP.FTempVent_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_vent_temp_simu_ena) {
    rty_InPutMng_Out->Cs_vent_temp = rtu_InPutMng_In->Cs_vent_temp_simu;
  } else {
    rty_InPutMng_Out->Cs_vent_temp = static_cast<ta_temp>(std::floor
      (rtb_output_b));
  }

  rtb_PresToSing = rtb_Cs_pres_vent_cs;
  firstOrderTFMDLOBJ1.step(&rtb_PresToSing, &rtb_output_dm,
    InPutMng_rtP.FPresVent_K, InPutMng_rtP.FPresVent_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_vent_pres_simu_ena) {
    rty_InPutMng_Out->Cs_vent_pres = rtu_InPutMng_In->Cs_vent_pres_simu;
  } else {
    rty_InPutMng_Out->Cs_vent_pres = static_cast<ta_air_pres>(rtb_output_dm);
  }

  if (rtu_InPutMng_In->Bs_tank_down_temp_err_simu_ena) {
    rtb_Bs_tank_down_temp_err = rtu_InPutMng_In->Bs_tank_down_temp_err_simu;
  } else {
    rtb_Bs_tank_down_temp_err = InPutMng_DW.mergeErr[0];
  }

  if (rtb_Bs_tank_down_temp_err) {
    rtb_Vh_Switch = InPutMng_rtP.const1_Value_a;
  } else {
    rtb_Vh_Switch = rtu_InPutMng_In->Cs_tank_vol_up;
  }

  if (rtu_InPutMng_In->Bs_tank_up_temp_err_simu_ena) {
    rtb_Bs_tank_up_temp_err = rtu_InPutMng_In->Bs_tank_up_temp_err_simu;
  } else {
    rtb_Bs_tank_up_temp_err = InPutMng_DW.mergeErr[1];
  }

  if (rtb_Bs_tank_up_temp_err) {
    rtb_Vb_Switch = InPutMng_rtP.const2_Value_a;
  } else if (rtb_Bs_tank_down_temp_err) {
    rtb_Vb_Switch = static_cast<float>(rtu_InPutMng_In->Cs_tank_vol_up +
      rtu_InPutMng_In->Cs_tank_vol_btwn);
  } else {
    rtb_Vb_Switch = rtu_InPutMng_In->Cs_tank_vol_btwn;
  }

  if (rtu_InPutMng_In->Bs_v40_min_simu_ena) {
    rty_InPutMng_Out->Cs_v40_min = rtu_InPutMng_In->Cs_v40_min_simu;
  } else {
    if (rtb_Cs_tank_up_temp >= InPutMng_rtP.Constant_Value) {
      tmp = static_cast<float>(rtb_Cs_tank_up_temp -
        InPutMng_rtP.Constant2_Value) * static_cast<float>(rtb_Vh_Switch);
    } else {
      tmp = InPutMng_rtP.const1_Value;
    }

    if (rtb_Cs_tank_down_temp >= InPutMng_rtP.Constant_Value) {
      tmp_0 = static_cast<float>(rtb_Cs_tank_down_temp -
        InPutMng_rtP.Constant2_Value) * rtb_Vb_Switch;
    } else {
      tmp_0 = InPutMng_rtP.const2_Value;
    }

    rty_InPutMng_Out->Cs_v40_min = static_cast<ta_wtr_vol>((tmp + tmp_0) /
      static_cast<float>(InPutMng_rtP.Constant_Value -
                         InPutMng_rtP.Constant2_Value));
  }

  if ((rtu_InPutMng_In->Ss_heat_pump_stt == InPutMng_rtP.Constant1_Value_p) ||
      (rtu_InPutMng_In->Ss_elec_htr_bstr_stt == InPutMng_rtP.Constant3_Value)) {
    rtb_Tmax = InPutMng_DW.TmaxPrev_DSTATE + InPutMng_rtP.AddConstant1_Bias;
  } else {
    rtb_Tmax = std::fmax(static_cast<float>(rtb_Cs_tank_up_temp),
                         InPutMng_DW.TmaxPrev_DSTATE +
                         InPutMng_rtP.AddConstant1_Bias);
  }

  rtb_Tcap_tmp = static_cast<int32_t>(std::fmax(static_cast<float>
    (rtb_Cs_tank_up_temp), static_cast<float>(InPutMng_rtP.Constant_Value)));
  rtb_Tcap = (std::fmax(rtb_Tmax, static_cast<float>(InPutMng_rtP.Constant_Value))
              + static_cast<float>(rtb_Tcap_tmp)) * InPutMng_rtP.getTcap_Gain;
  if (rtb_Tcap >= InPutMng_rtP.Constant_Value) {
    rtb_Vol = (rtb_Tmax - static_cast<float>(InPutMng_rtP.Constant_Value)) /
      (rtb_Tmax - std::fmin(static_cast<float>(rtb_Cs_tank_up_temp),
        static_cast<float>(InPutMng_rtP.Constant_Value))) * static_cast<float>
      (rtb_Vh_Switch);
  } else {
    rtb_Vol = InPutMng_rtP.Vnull1_Value_i;
  }

  rtb_Tcap_p_tmp = static_cast<int32_t>(std::fmax(static_cast<float>
    (rtb_Cs_tank_down_temp), static_cast<float>(InPutMng_rtP.Constant_Value)));
  rtb_Tcap_g = static_cast<float>(rtb_Tcap_tmp + rtb_Tcap_p_tmp) *
    InPutMng_rtP.getTcap_Gain_k;
  if (rtb_Tcap_g >= InPutMng_rtP.Constant_Value) {
    rtb_Vol_e = static_cast<float>(rtb_Cs_tank_up_temp -
      InPutMng_rtP.Constant_Value) / (static_cast<float>(rtb_Cs_tank_up_temp) -
      std::fmin(static_cast<float>(rtb_Cs_tank_down_temp), static_cast<float>
                (InPutMng_rtP.Constant_Value))) * rtb_Vb_Switch;
  } else {
    rtb_Vol_e = InPutMng_rtP.Vnull1_Value_l;
  }

  rtb_Tcap_c = (static_cast<float>(rtb_Tcap_p_tmp) + std::fmax(static_cast<float>
    (InPutMng_rtP.Constant2_Value), static_cast<float>
    (InPutMng_rtP.Constant_Value))) * InPutMng_rtP.getTcap_Gain_o;
  if (rtb_Tcap_c >= InPutMng_rtP.Constant_Value) {
    rtb_Vol_m = static_cast<float>(rtb_Cs_tank_down_temp -
      InPutMng_rtP.Constant_Value) / (static_cast<float>(rtb_Cs_tank_down_temp)
      - std::fmin(static_cast<float>(InPutMng_rtP.Constant2_Value), static_cast<
                  float>(InPutMng_rtP.Constant_Value))) * static_cast<float>
      (rtu_InPutMng_In->Cs_tank_vol_down);
  } else {
    rtb_Vol_m = InPutMng_rtP.Vnull1_Value;
  }

  if (rtb_Vol > InPutMng_rtP.Vnull2_Value) {
    tmp = rtb_Tcap;
  } else {
    tmp = InPutMng_rtP.Tnul_Value_a;
  }

  if (rtb_Vol_e > InPutMng_rtP.Vnull2_Value_e) {
    tmp_0 = rtb_Tcap_g;
  } else {
    tmp_0 = InPutMng_rtP.Tnul_Value_k;
  }

  if (rtb_Vol_m > InPutMng_rtP.Vnull2_Value_f) {
    tmp_1 = rtb_Tcap_c;
  } else {
    tmp_1 = InPutMng_rtP.Tnul_Value;
  }

  tmp_3 = static_cast<float>(InPutMng_rtP.Constant_Value -
    InPutMng_rtP.Constant2_Value);
  rty_InPutMng_Out->Cs_v40_mean = static_cast<ta_wtr_vol>((((tmp - static_cast<
    float>(InPutMng_rtP.Constant2_Value)) * rtb_Vol + (tmp_0 - static_cast<float>
    (InPutMng_rtP.Constant2_Value)) * rtb_Vol_e) + (tmp_1 - static_cast<float>
    (InPutMng_rtP.Constant2_Value)) * rtb_Vol_m) / tmp_3);
  if (rtb_Cs_tank_up_temp >= InPutMng_rtP.Constant_Value) {
    rtb_selectTh40Val1 = (rtb_Tmax - static_cast<float>
                          (InPutMng_rtP.Constant2_Value)) * static_cast<float>
      (rtb_Vh_Switch);
    rtb_selectTh40Val = static_cast<float>(rtb_Cs_tank_up_temp -
      InPutMng_rtP.Constant2_Value) * rtb_Vb_Switch;
  } else {
    rtb_selectTh40Val1 = InPutMng_rtP.const1_Value_f;
    rtb_selectTh40Val = InPutMng_rtP.const2_Value_f;
  }

  if (rtb_Cs_tank_down_temp >= InPutMng_rtP.Constant_Value) {
    tmp = static_cast<float>(rtb_Cs_tank_down_temp -
      InPutMng_rtP.Constant2_Value) * static_cast<float>
      (rtu_InPutMng_In->Cs_tank_vol_down);
  } else {
    tmp = InPutMng_rtP.const3_Value;
  }

  rty_InPutMng_Out->Cs_v40_max = static_cast<ta_wtr_vol>(((rtb_selectTh40Val1 +
    rtb_selectTh40Val) + tmp) / tmp_3);
  rtb_RotSpdToSing = rtb_Cs_vent_rot_spd_cs;
  firstOrderTFMDLOBJ2.step(&rtb_RotSpdToSing, &rtb_output_p,
    InPutMng_rtP.FRotSpd_K, InPutMng_rtP.FRotSpd_Tau, rtP_input_mng_tick);
  if (rtu_InPutMng_In->Bs_vent_rot_spd_simu_ena) {
    rty_InPutMng_Out->Cs_vent_rot_spd = rtu_InPutMng_In->Cs_vent_rot_spd_simu;
  } else {
    rty_InPutMng_Out->Cs_vent_rot_spd = static_cast<ta_rot_spd>(rtb_output_p);
  }

  rtb_isOn_0 = (rtu_InPutMng_In->St_tor_stt_raw[0] == InPutMng_rtP.inOnSg1_Value);
  rtb_isOn_idx_0 = rtb_isOn_0;
  rtb_isSmartGridAndOn_idx_0 = ((rtu_InPutMng_In->St_tor_mode[0] ==
    InPutMng_rtP.inSmartGrid_Value) && rtb_isOn_0);
  rtb_isOn_0 = (rtu_InPutMng_In->St_tor_stt_raw[1] == InPutMng_rtP.inOnSg1_Value);
  if (static_cast<int32_t>(static_cast<uint32_t>((rtu_InPutMng_In->St_tor_mode[1]
         == InPutMng_rtP.inSmartGrid_Value) && rtb_isOn_0) +
       rtb_isSmartGridAndOn_idx_0) > InPutMng_rtP.Constant_Value_i) {
    rty_InPutMng_Out->Ss_sg_stt = InPutMng_rtP.outOnSg_Value;
  } else {
    rty_InPutMng_Out->Ss_sg_stt = InPutMng_rtP.outOffSg_Value;
  }

  rtb_isGermany = (rtu_InPutMng_In->Ss_ctry == InPutMng_rtP.inVentStopSys1_Value);
  if (static_cast<int32_t>(static_cast<uint32_t>((rtu_InPutMng_In->St_tor_mode[0]
         == InPutMng_rtP.inOffPeakHour_Value) && rtb_isOn_idx_0) +
       ((rtu_InPutMng_In->St_tor_mode[1] == InPutMng_rtP.inOffPeakHour_Value) &&
        rtb_isOn_0)) > InPutMng_rtP.Constant_Value_m) {
    rty_InPutMng_Out->Ss_oph_stt = InPutMng_rtP.outOnOph_Value;
  } else {
    rty_InPutMng_Out->Ss_oph_stt = InPutMng_rtP.outOffOph_Value;
  }

  if (static_cast<int32_t>(static_cast<uint32_t>((rtu_InPutMng_In->St_tor_mode[0]
         == InPutMng_rtP.inVentStopSys_Value) && rtb_isOn_idx_0 && rtb_isGermany)
       + ((rtu_InPutMng_In->St_tor_mode[1] == InPutMng_rtP.inVentStopSys_Value) &&
          rtb_isOn_0 && rtb_isGermany)) > InPutMng_rtP.Constant_Value_o2) {
    rty_InPutMng_Out->Ss_vent_sys_stop_stt = InPutMng_rtP.outOnVst_Value;
  } else {
    rty_InPutMng_Out->Ss_vent_sys_stop_stt = InPutMng_rtP.outOffVst_Value;
  }

  if ((rtu_InPutMng_In->Ss_vent_cnsp_rst == InPutMng_rtP.On_Value) ||
      (InPutMng_rtP.On_Value == rtu_InPutMng_In->Ss_tot_cnsp_rst)) {
    rtb_resetSwitch = InPutMng_rtP.ResetValue_Value;
  } else {
    rtb_resetSwitch = InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DSTA;
  }

  if ((rtu_InPutMng_In->Ss_heat_wtr_cnsp_rst == InPutMng_rtP.On_Value_d) ||
      (InPutMng_rtP.On_Value_d == rtu_InPutMng_In->Ss_tot_cnsp_rst)) {
    rtb_resetSwitch_d = InPutMng_rtP.ResetValue_Value_i;
  } else {
    if (rtu_InPutMng_In->Ss_elec_htr_bstr_stt == InPutMng_rtP.Constant1_Value) {
      tmp_2 = InPutMng_rtP.ElecBoosterHeaterPower_Value;
    } else {
      tmp_2 = InPutMng_rtP.NulPower_Value;
    }

    rtb_resetSwitch_d = tmp_2 + InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DS_g;
  }

  if ((rtu_InPutMng_In->Ss_heat_wtr_cnsp_rst == InPutMng_rtP.On_Value_c) ||
      (InPutMng_rtP.On_Value_c == rtu_InPutMng_In->Ss_tot_cnsp_rst)) {
    rtb_resetSwitch_h = InPutMng_rtP.ResetValue_Value_d;
  } else {
    rtb_resetSwitch_h = rtu_InPutMng_In->Cs_heat_pump_pwr +
      InPutMng_DW.Cs_vent_cnsp_prev_DSTATE;
  }

  rtb_AddCnsp = rtb_resetSwitch_d + rtb_resetSwitch_h;
  rty_InPutMng_Out->Cs_tot_cnsp = rtb_resetSwitch + rtb_AddCnsp;
  if (rtu_InPutMng_In->Bs_pump_xhst_temp_err_simu_ena) {
    rty_InPutMng_Out->Bs_pump_xhst_temp_err =
      rtu_InPutMng_In->Bs_pump_xhst_temp_err_simu;
  } else {
    rty_InPutMng_Out->Bs_pump_xhst_temp_err = InPutMng_DW.mergeErr[2];
  }

  if (rtu_InPutMng_In->Bs_pump_evap_temp_err_simu_ena) {
    rty_InPutMng_Out->Bs_pump_evap_temp_err =
      rtu_InPutMng_In->Bs_pump_evap_temp_err_simu;
  } else {
    rty_InPutMng_Out->Bs_pump_evap_temp_err = InPutMng_DW.mergeErr[3];
  }

  if (rtu_InPutMng_In->Bs_vent_temp_err_simu_ena) {
    rty_InPutMng_Out->Bs_vent_temp_err = rtu_InPutMng_In->Bs_vent_temp_err_simu;
  } else {
    rty_InPutMng_Out->Bs_vent_temp_err = InPutMng_DW.mergeErr[4];
  }

  if (rtu_InPutMng_In->Bs_vent_pres_err_simu_ena) {
    rty_InPutMng_Out->Bs_vent_pres_err = rtu_InPutMng_In->Bs_vent_pres_err_simu;
  } else {
    rty_InPutMng_Out->Bs_vent_pres_err = InPutMng_DW.mergeErr[5];
  }

  rty_InPutMng_Out->Cs_tank_down_temp = rtb_Cs_tank_down_temp;
  rty_InPutMng_Out->Cs_tank_up_temp = rtb_Cs_tank_up_temp;
  rty_InPutMng_Out->Cs_pump_evap_temp = rtb_Cs_pump_evap_temp;
  rty_InPutMng_Out->Cs_vent_cnsp = rtb_resetSwitch;
  rty_InPutMng_Out->Cs_heat_wtr_cnsp = rtb_AddCnsp;
  rty_InPutMng_Out->Bs_tank_down_temp_err = rtb_Bs_tank_down_temp_err;
  rty_InPutMng_Out->Bs_tank_up_temp_err = rtb_Bs_tank_up_temp_err;
  rty_InPutMng_Out->Bs_vent_fan_err = rtu_InPutMng_In->Bs_hw_fan_err;
  rty_InPutMng_Out->Bs_tank_anod_err = rtu_InPutMng_In->Bs_hw_anod_err;
  InPutMng_DW.TmaxPrev_DSTATE = rtb_Tmax;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DSTA = rtb_resetSwitch;
  InPutMng_DW.Cs_elec_htr_bstr_cnsp_prev_DS_g = rtb_resetSwitch_d;
  InPutMng_DW.Cs_vent_cnsp_prev_DSTATE = rtb_resetSwitch_h;
}

// Constructor
InPutMng::InPutMng():
  InPutMng_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
InPutMng::~InPutMng() = default;

//
// File trailer for generated code.
//
// [EOF]
//
