//
// File: VentCtrl.cpp
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.136
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:24:26 2025
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
#include "VentCtrl.h"
#include "VentCtrl_types.h"
#include "rt_roundf.h"
#include <stdbool.h>
#include <stdint.h>
#include "VentCtrl_private.h"
#include "look1_iu16lu32n16_binlcse.h"
#include "firstOrderTF.h"
#include "PID_backCalc.h"
#include "cntrlSaturation.h"

const bool VentCtrl_BGND{ false };     // bool ground

VentCtrl::P_VentCtrl_T VentCtrl::VentCtrl_rtP{
  // Mask Parameter: presMesfilt_K
  //  Referenced by: '<S9>/firstOrderTF'

  1.0F,

  // Mask Parameter: presFilter_K
  //  Referenced by: '<S12>/firstOrderTF'

  1.0F,

  // Mask Parameter: pressSpfilt_K
  //  Referenced by: '<S11>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOpressSpFilt_K
  //  Referenced by: '<S10>/firstOrderTF'

  1.0F,

  // Mask Parameter: presMesfilt_Tau
  //  Referenced by: '<S9>/firstOrderTF'

  0.1F,

  // Mask Parameter: presFilter_Tau
  //  Referenced by: '<S12>/firstOrderTF'

  0.1F,

  // Mask Parameter: pressSpfilt_Tau
  //  Referenced by: '<S11>/firstOrderTF'

  20.0F,

  // Mask Parameter: firstOpressSpFilt_Tau
  //  Referenced by: '<S10>/firstOrderTF'

  9.0F,

  // Mask Parameter: presMesfilt_initVal
  //  Referenced by: '<S9>/firstOrderTF'

  0.0F,

  // Mask Parameter: presFilter_initVal
  //  Referenced by: '<S12>/firstOrderTF'

  0.0F,

  // Mask Parameter: pressSpfilt_initVal
  //  Referenced by: '<S11>/firstOrderTF'

  0.0F,

  // Mask Parameter: firstOpressSpFilt_initVal
  //  Referenced by: '<S10>/firstOrderTF'

  0.0F,

  // Mask Parameter: presMesfilt_sampleTime
  //  Referenced by: '<S9>/firstOrderTF'

  1.0F,

  // Mask Parameter: presFilter_sampleTime
  //  Referenced by: '<S12>/firstOrderTF'

  1.0F,

  // Mask Parameter: pressSpfilt_sampleTime
  //  Referenced by: '<S11>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOpressSpFilt_sampleTime
  //  Referenced by: '<S10>/firstOrderTF'

  1.0F,

  // Computed Parameter: press_max_Value
  //  Referenced by: '<S5>/press_max'

  3000.0,

  // Computed Parameter: press_min_Value
  //  Referenced by: '<S5>/press_min'

  200.0,

  // Computed Parameter: flow_max_Value
  //  Referenced by: '<S5>/flow_max'

  3500.0,

  // Computed Parameter: flow_min_Value
  //  Referenced by: '<S5>/flow_min'

  400.0,

  // Expression: Cs_press_reg_init_C
  //  Referenced by: '<S5>/PressureRegulator'

  100.0F,

  // Expression: Cs_reg_press_awu_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.5F,

  // Expression: Cs_reg_press_tau_1_C*Cs_reg_press_tau_2_C*Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  1.8F,

  // Expression: Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.01F,

  // Expression: (Cs_reg_press_tau_1_C+Cs_reg_press_tau_2_C)*Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.29F,

  // Expression: Cs_PressCtrl_TimeSample_C
  //  Referenced by: '<S5>/PressureRegulator'

  1.0F,

  // Expression: Cs_reg_press_d_tau_filt_C
  //  Referenced by: '<S5>/PressureRegulator'

  5.0F,

  // Computed Parameter: Cs_flow_fan_sp_C_Value
  //  Referenced by: '<S4>/Cs_flow_fan_sp_C'

  2000U,

  // Expression: Ct_mtr_volt_data_C
  //  Referenced by: '<S2>/1-D Lookup Table'

  { 0U, 3000U, 4000U, 5000U, 6000U, 7000U, 8000U, 9000U, 9500U, 10000U },

  // Expression: Ct_mtr_flow_bkpt_C
  //  Referenced by: '<S2>/1-D Lookup Table'

  { 0U, 400U, 800U, 1200U, 1600U, 2000U, 2400U, 2800U, 3000U, 3200U },

  // Computed Parameter: flowEsti_InitialCondition
  //  Referenced by: '<S3>/flowEsti'

  0U,

  // Expression: Ct_rise_curv_data_C
  //  Referenced by: '<S6>/Rise_Curve'

  { 0U, 0U, 0U, 0U, 0U, 2U, 9U, 18U, 32U, 49U, 70U, 95U, 122U, 153U, 185U, 220U,
    255U, 291U, 327U, 362U, 397U, 429U, 460U, 488U, 514U, 537U, 558U, 576U, 592U,
    607U, 622U, 637U, 654U },

  // Expression: Ct_rise_curv_bp_C
  //  Referenced by: '<S6>/Rise_Curve'

  { 0U, 100U, 200U, 300U, 400U, 500U, 600U, 700U, 800U, 900U, 1000U, 1100U,
    1200U, 1300U, 1400U, 1500U, 1600U, 1700U, 1800U, 1900U, 2000U, 2100U, 2200U,
    2300U, 2400U, 2500U, 2600U, 2700U, 2800U, 2900U, 3000U, 3100U, 3200U },

  // Computed Parameter: NullFlowConstant_Value
  //  Referenced by: '<S1>/NullFlowConstant'

  0U
};

// System initialize for referenced model: 'VentCtrl'
void VentCtrl::init(void)
{
  VentCtrl_DW.flowEsti_DSTATE = VentCtrl_rtP.flowEsti_InitialCondition;
  firstOrderTFMDLOBJ5.init(VentCtrl_rtP.presMesfilt_initVal);
  firstOrderTFMDLOBJ6.init(VentCtrl_rtP.presFilter_initVal);
  firstOrderTFMDLOBJ4.init(VentCtrl_rtP.pressSpfilt_initVal);
  firstOrderTFMDLOBJ3.init(VentCtrl_rtP.firstOpressSpFilt_initVal);
  PressureRegulatorMDLOBJ2.init(VentCtrl_rtP.PressureRegulator_InitVal);
}

// System reset for referenced model: 'VentCtrl'
void VentCtrl::reset(void)
{
  VentCtrl_DW.flowEsti_DSTATE = VentCtrl_rtP.flowEsti_InitialCondition;
  firstOrderTFMDLOBJ5.reset(VentCtrl_rtP.presMesfilt_initVal);
  firstOrderTFMDLOBJ6.reset(VentCtrl_rtP.presFilter_initVal);
  firstOrderTFMDLOBJ4.reset(VentCtrl_rtP.pressSpfilt_initVal);
  firstOrderTFMDLOBJ3.reset(VentCtrl_rtP.firstOpressSpFilt_initVal);
  PressureRegulatorMDLOBJ2.reset(VentCtrl_rtP.PressureRegulator_InitVal);
}

// Output and update for referenced model: 'VentCtrl'
void VentCtrl::step(const tb_VentCtrl_In *rtu_VentCtrl_In, tb_VentCtrl_Out
                    *rty_VentCtrl_Out)
{
  // local block i/o variables
  float rtb_output;
  float rtb_output_m;
  float rtb_output_g;
  float rtb_output_n;
  float rtb_Saturation_o1;
  float rtb_Saturation_o2;
  float rtb_cmd;
  float rtb_ovrFlwVal;
  float rtb_Cs_pres_sp_sngl;
  float rtb_Cs_press_max_C_sngl;
  float rtb_Cs_press_min_C_sngl;
  float rtb_Cs_press_rise_curv_sp_sngl;
  float rtb_Cs_vent_pres_sngl;
  float rtb_Pres_setPoint;
  float rtb_cmd_max;
  float rtb_cmd_min;
  ta_air_pres rtb_Cs_press_rise_curv_sp;
  ta_air_pres tmp;
  ta_flow rtb_Cs_reg_fan_flow;
  ta_flow rtb_Cs_reg_fan_flow_cs;
  rtb_Cs_vent_pres_sngl = rtu_VentCtrl_In->Cs_vent_pres;
  firstOrderTFMDLOBJ5.step(&rtb_Cs_vent_pres_sngl, &rtb_output,
    VentCtrl_rtP.presMesfilt_K, VentCtrl_rtP.presMesfilt_Tau,
    VentCtrl_rtP.presMesfilt_sampleTime);
  if (rtu_VentCtrl_In->Bs_vent_pres_sp_simu_ena) {
    rty_VentCtrl_Out->Cs_vent_pres_sp = rtu_VentCtrl_In->Cs_vent_pres_sp_simu;
  } else {
    rty_VentCtrl_Out->Cs_vent_pres_sp = static_cast<ta_air_pres>(rt_roundf
      (rtb_output));
  }

  rtb_Cs_press_max_C_sngl = static_cast<float>(VentCtrl_rtP.press_max_Value);
  rtb_Cs_press_min_C_sngl = static_cast<float>(VentCtrl_rtP.press_min_Value);
  if (rtu_VentCtrl_In->Cs_vent_pres_min >= rtu_VentCtrl_In->Cs_vent_pres_sys) {
    tmp = rtu_VentCtrl_In->Cs_vent_pres_min;
  } else {
    tmp = rtu_VentCtrl_In->Cs_vent_pres_sys;
  }

  rtb_Cs_pres_sp_sngl = static_cast<uint16_t>(look1_iu16lu32n16_binlcse
    (VentCtrl_DW.flowEsti_DSTATE, VentCtrl_rtP.Rise_Curve_bp01Data,
     VentCtrl_rtP.Rise_Curve_tableData, 32U) + tmp);
  firstOrderTFMDLOBJ6.step(&rtb_Cs_pres_sp_sngl, &rtb_output_m,
    VentCtrl_rtP.presFilter_K, VentCtrl_rtP.presFilter_Tau,
    VentCtrl_rtP.presFilter_sampleTime);
  rtb_Cs_press_rise_curv_sp = static_cast<ta_air_pres>(rt_roundf(rtb_output_m));
  rtb_Cs_press_rise_curv_sp_sngl = rtb_Cs_press_rise_curv_sp;
  firstOrderTFMDLOBJ4.step(&rtb_Cs_press_rise_curv_sp_sngl, &rtb_output_g,
    VentCtrl_rtP.pressSpfilt_K, VentCtrl_rtP.pressSpfilt_Tau,
    VentCtrl_rtP.pressSpfilt_sampleTime);
  firstOrderTFMDLOBJ3.step(&rtb_output_g, &rtb_output_n,
    VentCtrl_rtP.firstOpressSpFilt_K, VentCtrl_rtP.firstOpressSpFilt_Tau,
    VentCtrl_rtP.firstOpressSpFilt_sampleTime);
  SaturationMDLOBJ1.step(&rtb_Cs_press_max_C_sngl, &rtb_Cs_press_min_C_sngl,
    &rtb_output_n, &rtb_Saturation_o1, &rtb_Saturation_o2);
  if (rtu_VentCtrl_In->Ss_vent_op_mode == te_vent_op_mode::Normal) {
    rtb_Pres_setPoint = rtb_Saturation_o1;
  } else {
    rtb_Pres_setPoint = rtb_output;
  }

  rtb_cmd_max = static_cast<float>(VentCtrl_rtP.flow_max_Value);
  rtb_cmd_min = static_cast<float>(VentCtrl_rtP.flow_min_Value);
  PressureRegulatorMDLOBJ2.step(&rtb_Pres_setPoint, &rtb_output, &rtb_cmd_max,
    &rtb_cmd_min, (const_cast<bool*>(&VentCtrl_BGND)), &rtb_cmd, &rtb_ovrFlwVal,
    VentCtrl_rtP.PressureRegulator_InitVal, VentCtrl_rtP.PressureRegulator_Kawu,
    VentCtrl_rtP.PressureRegulator_Kd, VentCtrl_rtP.PressureRegulator_Ki,
    VentCtrl_rtP.PressureRegulator_Kp, VentCtrl_rtP.PressureRegulator_SampleTime,
    VentCtrl_rtP.PressureRegulator_Tau_f);
  rtb_Cs_reg_fan_flow = static_cast<ta_flow>(rt_roundf(rtb_cmd));
  switch (rtu_VentCtrl_In->Ss_vent_op_mode) {
   case te_vent_op_mode::ConstantFlow:
    rtb_Cs_reg_fan_flow_cs = VentCtrl_rtP.Cs_flow_fan_sp_C_Value;
    break;

   case te_vent_op_mode::Normal:
    rtb_Cs_reg_fan_flow_cs = rtb_Cs_reg_fan_flow;
    break;

   case te_vent_op_mode::SafetyOff:
    rtb_Cs_reg_fan_flow_cs = VentCtrl_rtP.NullFlowConstant_Value;
    break;

   default:
    rtb_Cs_reg_fan_flow_cs = VentCtrl_rtP.NullFlowConstant_Value;
    break;
  }

  if (rtu_VentCtrl_In->Bs_vent_flow_sp_simu_ena) {
    rty_VentCtrl_Out->Cs_vent_flow_sp = rtu_VentCtrl_In->Cs_vent_flow_sp_simu;
  } else {
    rty_VentCtrl_Out->Cs_vent_flow_sp = rtb_Cs_reg_fan_flow_cs;
  }

  if (rtu_VentCtrl_In->Bs_vent_vltg_sp_simu_ena) {
    rty_VentCtrl_Out->Cs_vent_vltg_sp = rtu_VentCtrl_In->Cs_vent_vltg_sp_simu;
  } else {
    rty_VentCtrl_Out->Cs_vent_vltg_sp = look1_iu16lu32n16_binlcse
      (rtb_Cs_reg_fan_flow_cs, VentCtrl_rtP.uDLookupTable_bp01Data,
       VentCtrl_rtP.uDLookupTable_tableData, 9U);
  }

  rty_VentCtrl_Out->Cs_pres_rise_curv_sp = rtb_Cs_press_rise_curv_sp;
  VentCtrl_DW.flowEsti_DSTATE = rtb_Cs_reg_fan_flow;
}

// Constructor
VentCtrl::VentCtrl():
  VentCtrl_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
VentCtrl::~VentCtrl() = default;

//
// File trailer for generated code.
//
// [EOF]
//
