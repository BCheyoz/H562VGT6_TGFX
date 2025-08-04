//
// File: VentCtrl.cpp
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.146
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:04:01 2025
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
  // Mask Parameter: pressSpfilt_K
  //  Referenced by: '<S11>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOpressSpFilt_K
  //  Referenced by: '<S10>/firstOrderTF'

  1.0F,

  // Mask Parameter: presMesfilt_K
  //  Referenced by: '<S9>/firstOrderTF'

  1.0F,

  // Mask Parameter: presFilter_K
  //  Referenced by: '<S12>/firstOrderTF'

  1.0F,

  // Mask Parameter: pressSpfilt_Tau
  //  Referenced by: '<S11>/firstOrderTF'

  11.0F,

  // Mask Parameter: firstOpressSpFilt_Tau
  //  Referenced by: '<S10>/firstOrderTF'

  8.5F,

  // Mask Parameter: presMesfilt_Tau
  //  Referenced by: '<S9>/firstOrderTF'

  0.1F,

  // Mask Parameter: presFilter_Tau
  //  Referenced by: '<S12>/firstOrderTF'

  0.1F,

  // Mask Parameter: pressSpfilt_initVal
  //  Referenced by: '<S11>/firstOrderTF'

  0.0F,

  // Mask Parameter: firstOpressSpFilt_initVal
  //  Referenced by: '<S10>/firstOrderTF'

  0.0F,

  // Mask Parameter: presMesfilt_initVal
  //  Referenced by: '<S9>/firstOrderTF'

  0.0F,

  // Mask Parameter: presFilter_initVal
  //  Referenced by: '<S12>/firstOrderTF'

  0.0F,

  // Mask Parameter: pressSpfilt_sampleTime
  //  Referenced by: '<S11>/firstOrderTF'

  1.0F,

  // Mask Parameter: firstOpressSpFilt_sampleTime
  //  Referenced by: '<S10>/firstOrderTF'

  1.0F,

  // Mask Parameter: presMesfilt_sampleTime
  //  Referenced by: '<S9>/firstOrderTF'

  1.0F,

  // Mask Parameter: presFilter_sampleTime
  //  Referenced by: '<S12>/firstOrderTF'

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

  0.0,

  // Expression: Cs_press_reg_init_C
  //  Referenced by: '<S5>/PressureRegulator'

  100.0F,

  // Expression: Cs_reg_press_awu_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.5F,

  // Expression: Cs_reg_press_tau_1_C*Cs_reg_press_tau_2_C*Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  1.87F,

  // Expression: Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.02F,

  // Expression: (Cs_reg_press_tau_1_C+Cs_reg_press_tau_2_C)*Cs_reg_press_gain_C
  //  Referenced by: '<S5>/PressureRegulator'

  0.39F,

  // Expression: Cs_PressCtrl_TimeSample_C
  //  Referenced by: '<S5>/PressureRegulator'

  1.0F,

  // Expression: Cs_reg_press_d_tau_filt_C
  //  Referenced by: '<S5>/PressureRegulator'

  5.0F,

  // Computed Parameter: Cs_flow_fan_sp_C_Value
  //  Referenced by: '<S4>/Cs_flow_fan_sp_C'

  2000U,

  // Computed Parameter: NullFlowConstant_Value
  //  Referenced by: '<S1>/NullFlowConstant'

  0U,

  // Expression: Ct_mtr_volt_data_C
  //  Referenced by: '<S2>/flow2volt'

  { 0U, 2000U, 3000U, 4000U, 5000U, 6000U, 7000U, 8000U, 9000U, 9500U, 10000U },

  // Expression: Ct_mtr_flow_bkpt_C
  //  Referenced by: '<S2>/flow2volt'

  { 0U, 1U, 400U, 800U, 1200U, 1600U, 2000U, 2400U, 2800U, 3000U, 3200U },

  // Expression: Ct_rise_curv_data_C
  //  Referenced by: '<S6>/Rise_Curve'

  { 0U, 0U, 0U, 0U, 0U, 2U, 9U, 18U, 32U, 49U, 70U, 95U, 122U, 153U, 185U, 220U, 255U, 291U, 327U, 362U, 397U, 429U, 460U, 488U, 514U, 537U, 558U, 576U, 592U,
    607U, 622U, 637U, 654U },

  // Expression: Ct_rise_curv_bp_C
  //  Referenced by: '<S6>/Rise_Curve'

  { 0U, 100U, 200U, 300U, 400U, 500U, 600U, 700U, 800U, 900U, 1000U, 1100U, 1200U, 1300U, 1400U, 1500U, 1600U, 1700U, 1800U, 1900U, 2000U, 2100U, 2200U, 2300U,
    2400U, 2500U, 2600U, 2700U, 2800U, 2900U, 3000U, 3100U, 3200U },

  // Computed Parameter: flowEsti_InitialCondition
  //  Referenced by: '<S3>/flowEsti'

  0U
};

// Output and update for atomic system: '<Root>/ConsFlow'
void VentCtrl::VentCtrl_ConsFlow()
{
  if (VentCtrl_rtu_VentCtrl_In->Bs_vent_flow_sp_simu_ena) {
    VentCtrl_DW.VentFlowSwitch = VentCtrl_rtu_VentCtrl_In->Cs_vent_flow_sp_simu;
  } else {
    switch (VentCtrl_rtu_VentCtrl_In->Ss_vent_op_mode) {
     case te_vent_op_mode::ConstantFlow:
      VentCtrl_DW.VentFlowSwitch = VentCtrl_rtP.Cs_flow_fan_sp_C_Value;
      break;

     case te_vent_op_mode::Normal:
      VentCtrl_DW.VentFlowSwitch = VentCtrl_DW.Cs_reg_fan_flow;
      break;

     case te_vent_op_mode::Stopped:
      VentCtrl_DW.VentFlowSwitch = VentCtrl_rtP.NullFlowConstant_Value;
      break;

     default:
      VentCtrl_DW.VentFlowSwitch = VentCtrl_rtP.NullFlowConstant_Value;
      break;
    }
  }
}

// Output and update for atomic system: '<Root>/ConsTens'
void VentCtrl::VentCtrl_ConsTens() const
{
  if (VentCtrl_rtu_VentCtrl_In->Bs_vent_vltg_sp_simu_ena) {
    VentCtrl_rty_VentCtrl_Out->Cs_vent_vltg_sp = VentCtrl_rtu_VentCtrl_In->Cs_vent_vltg_sp_simu;
  } else {
    VentCtrl_rty_VentCtrl_Out->Cs_vent_vltg_sp = look1_iu16lu32n16_binlcse(VentCtrl_DW.VentFlowSwitch, VentCtrl_rtP.flow2volt_bp01Data,
      VentCtrl_rtP.flow2volt_tableData, 10U);
  }
}

// System initialize for atomic system: '<S3>/ConsPres'
void VentCtrl::VentCtrl_ConsPres_Init()
{
  firstOrderTFMDLOBJ4.init(VentCtrl_rtP.pressSpfilt_initVal);
  firstOrderTFMDLOBJ3.init(VentCtrl_rtP.firstOpressSpFilt_initVal);
  firstOrderTFMDLOBJ5.init(VentCtrl_rtP.presMesfilt_initVal);
  PressureRegulatorMDLOBJ2.init(VentCtrl_rtP.PressureRegulator_InitVal);
}

// System reset for atomic system: '<S3>/ConsPres'
void VentCtrl::VentCtrl_ConsPres_Reset()
{
  firstOrderTFMDLOBJ4.reset(VentCtrl_rtP.pressSpfilt_initVal);
  firstOrderTFMDLOBJ3.reset(VentCtrl_rtP.firstOpressSpFilt_initVal);
  firstOrderTFMDLOBJ5.reset(VentCtrl_rtP.presMesfilt_initVal);
  PressureRegulatorMDLOBJ2.reset(VentCtrl_rtP.PressureRegulator_InitVal);
}

// Output and update for atomic system: '<S3>/ConsPres'
void VentCtrl::VentCtrl_ConsPres()
{
  // local block i/o variables
  float rtb_output;
  float rtb_output_n;
  float rtb_Saturation_o1;
  float rtb_Saturation_o2;
  float rtb_output_d;
  float rtb_cmd;
  float rtb_ovrFlwVal;
  float rtb_Cs_press_max_C_sngl;
  float rtb_Cs_press_min_C_sngl;
  float rtb_Cs_press_rise_curv_sp_sngl;
  float rtb_Cs_vent_pres_sngl;
  float rtb_Pres_setPoint;
  float rtb_cmd_max;
  float rtb_cmd_min;
  rtb_Cs_press_max_C_sngl = static_cast<float>(VentCtrl_rtP.press_max_Value);
  rtb_Cs_press_min_C_sngl = static_cast<float>(VentCtrl_rtP.press_min_Value);
  rtb_Cs_press_rise_curv_sp_sngl = VentCtrl_DW.VentPresSwitch;
  firstOrderTFMDLOBJ4.step(&rtb_Cs_press_rise_curv_sp_sngl, &rtb_output, VentCtrl_rtP.pressSpfilt_K, VentCtrl_rtP.pressSpfilt_Tau,
    VentCtrl_rtP.pressSpfilt_sampleTime);
  firstOrderTFMDLOBJ3.step(&rtb_output, &rtb_output_n, VentCtrl_rtP.firstOpressSpFilt_K, VentCtrl_rtP.firstOpressSpFilt_Tau,
    VentCtrl_rtP.firstOpressSpFilt_sampleTime);
  SaturationMDLOBJ1.step(&rtb_Cs_press_max_C_sngl, &rtb_Cs_press_min_C_sngl, &rtb_output_n, &rtb_Saturation_o1, &rtb_Saturation_o2);
  rtb_Cs_vent_pres_sngl = VentCtrl_rtu_VentCtrl_In->Cs_vent_pres;
  firstOrderTFMDLOBJ5.step(&rtb_Cs_vent_pres_sngl, &rtb_output_d, VentCtrl_rtP.presMesfilt_K, VentCtrl_rtP.presMesfilt_Tau, VentCtrl_rtP.presMesfilt_sampleTime);
  if (VentCtrl_rtu_VentCtrl_In->Ss_vent_op_mode == te_vent_op_mode::Normal) {
    rtb_Pres_setPoint = rtb_Saturation_o1;
  } else {
    rtb_Pres_setPoint = rtb_output_d;
  }

  rtb_cmd_max = static_cast<float>(VentCtrl_rtP.flow_max_Value);
  rtb_cmd_min = static_cast<float>(VentCtrl_rtP.flow_min_Value);
  PressureRegulatorMDLOBJ2.step(&rtb_Pres_setPoint, &rtb_output_d, &rtb_cmd_max, &rtb_cmd_min, (const_cast<bool*>(&VentCtrl_BGND)), &rtb_cmd, &rtb_ovrFlwVal,
    VentCtrl_rtP.PressureRegulator_InitVal, VentCtrl_rtP.PressureRegulator_Kawu, VentCtrl_rtP.PressureRegulator_Kd, VentCtrl_rtP.PressureRegulator_Ki,
    VentCtrl_rtP.PressureRegulator_Kp, VentCtrl_rtP.PressureRegulator_SampleTime, VentCtrl_rtP.PressureRegulator_Tau_f);
  VentCtrl_rty_VentCtrl_Out->Cs_pres_mes_filt = static_cast<ta_air_pres>(rt_roundf(rtb_output_d));
  VentCtrl_DW.Cs_reg_fan_flow = static_cast<ta_flow>(rt_roundf(rtb_cmd));
}

// System initialize for atomic system: '<S3>/CourbMont'
void VentCtrl::VentCtrl_CourbMont_Init()
{
  firstOrderTFMDLOBJ6.init(VentCtrl_rtP.presFilter_initVal);
}

// System reset for atomic system: '<S3>/CourbMont'
void VentCtrl::VentCtrl_CourbMont_Reset()
{
  firstOrderTFMDLOBJ6.reset(VentCtrl_rtP.presFilter_initVal);
}

// Output and update for atomic system: '<S3>/CourbMont'
void VentCtrl::VentCtrl_CourbMont()
{
  // local block i/o variables
  float rtb_output_m;
  float rtb_Cs_pres_sp_sngl;
  ta_air_pres tmp;
  if (VentCtrl_rtu_VentCtrl_In->Cs_vent_pres_min >= VentCtrl_rtu_VentCtrl_In->Cs_vent_pres_sys) {
    tmp = VentCtrl_rtu_VentCtrl_In->Cs_vent_pres_min;
  } else {
    tmp = VentCtrl_rtu_VentCtrl_In->Cs_vent_pres_sys;
  }

  rtb_Cs_pres_sp_sngl = static_cast<uint16_t>(look1_iu16lu32n16_binlcse(VentCtrl_DW.flowEsti, VentCtrl_rtP.Rise_Curve_bp01Data,
    VentCtrl_rtP.Rise_Curve_tableData, 32U) + tmp);
  firstOrderTFMDLOBJ6.step(&rtb_Cs_pres_sp_sngl, &rtb_output_m, VentCtrl_rtP.presFilter_K, VentCtrl_rtP.presFilter_Tau, VentCtrl_rtP.presFilter_sampleTime);
  VentCtrl_DW.Cs_press_rise_curv_sp = static_cast<ta_air_pres>(rt_roundf(rtb_output_m));
}

// System initialize for atomic system: '<Root>/PresMng'
void VentCtrl::VentCtrl_PresMng_Init()
{
  VentCtrl_DW.flowEsti_DSTATE = VentCtrl_rtP.flowEsti_InitialCondition;
  VentCtrl_CourbMont_Init();
  VentCtrl_ConsPres_Init();
}

// System reset for atomic system: '<Root>/PresMng'
void VentCtrl::VentCtrl_PresMng_Reset()
{
  VentCtrl_DW.flowEsti_DSTATE = VentCtrl_rtP.flowEsti_InitialCondition;
  VentCtrl_CourbMont_Reset();
  VentCtrl_ConsPres_Reset();
}

// Output and update for atomic system: '<Root>/PresMng'
void VentCtrl::VentCtrl_PresMng()
{
  VentCtrl_DW.flowEsti = VentCtrl_DW.flowEsti_DSTATE;
  VentCtrl_CourbMont();
  if (VentCtrl_rtu_VentCtrl_In->Bs_vent_pres_sp_simu_ena) {
    VentCtrl_DW.VentPresSwitch = VentCtrl_rtu_VentCtrl_In->Cs_vent_pres_sp_simu;
  } else {
    VentCtrl_DW.VentPresSwitch = VentCtrl_DW.Cs_press_rise_curv_sp;
  }

  VentCtrl_ConsPres();
  VentCtrl_DW.flowEsti_DSTATE = VentCtrl_DW.Cs_reg_fan_flow;
}

// System initialize for referenced model: 'VentCtrl'
void VentCtrl::init(void)
{
  VentCtrl_PresMng_Init();
}

// System reset for referenced model: 'VentCtrl'
void VentCtrl::reset(void)
{
  VentCtrl_PresMng_Reset();
}

// Output and update for referenced model: 'VentCtrl'
void VentCtrl::step(const tb_VentCtrl_In *rtu_VentCtrl_In, tb_VentCtrl_Out *rty_VentCtrl_Out)
{
  VentCtrl_rtu_VentCtrl_In = rtu_VentCtrl_In;
  VentCtrl_rty_VentCtrl_Out = rty_VentCtrl_Out;
  VentCtrl_PresMng();
  VentCtrl_ConsFlow();
  VentCtrl_ConsTens();
  rty_VentCtrl_Out->Cs_vent_pres_sp = VentCtrl_DW.VentPresSwitch;
  rty_VentCtrl_Out->Cs_vent_flow_sp = VentCtrl_DW.VentFlowSwitch;
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
