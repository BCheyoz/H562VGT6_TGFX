//
// File: WaterHeatSpCalc.cpp
//
// Code generated for Simulink model 'WaterHeatSpCalc'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 24 11:22:03 2025
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
#include "WaterHeatSpCalc.h"
#include "WaterHeatSpCalc_types.h"
#include "plook_u32u8_binckan.h"

WaterHeatSpCalc::P_WaterHeatSpCalc_T WaterHeatSpCalc::WaterHeatSpCalc_rtP{
  // Expression: te_heat_mode.v40_ctrl
  //  Referenced by: '<S1>/v40_ctrl'

  te_heat_mode::v40_ctrl,

  // Expression: te_heat_mode.temp_ctrl
  //  Referenced by: '<S1>/temp_ctrl'

  te_heat_mode::temp_ctrl,

  // Expression: Cs_cons_hyb_C
  //  Referenced by: '<S3>/Cs_cons_hyb_C'

  480,

  // Expression: Cs_cons_AL_C
  //  Referenced by: '<S3>/Cs_cons_AL_C'

  650,

  // Expression: Cs_cons_hol_C
  //  Referenced by: '<S3>/Cs_cons_hol_C'

  60,

  // Expression: Cs_cons_test_pac_C
  //  Referenced by: '<S3>/Cs_cons_test_pac_C'

  650,

  // Computed Parameter: NullConstant_Value
  //  Referenced by: '<S3>/NullConstant'

  0,

  // Expression: Cs_bain_eco_C
  //  Referenced by: '<S2>/Cs_bain_eco_C'

  1300U,

  // Expression: Ct_V40j_eco_C
  //  Referenced by: '<S2>/ConsEco'

  { 1350U, 2000U, 2600U, 3150U, 3600U },

  // Expression: Ct_V40j_auto_C
  //  Referenced by: '<S2>/ConsAuto'

  { 1700U, 2100U, 2600U, 3250U, 3900U },

  // Expression: Cs_bain_auto_elec_C
  //  Referenced by: '<S2>/Cs_bain_auto_elec_C'

  1500U,

  // Computed Parameter: Cs_Boost_v40_sp_InitialConditio
  //  Referenced by: '<S2>/Cs_Boost_v40_sp'

  0U,

  // Computed Parameter: v40NullSpConstant_Value
  //  Referenced by: '<S2>/v40NullSpConstant'

  0U,

  // Expression: Ct_nb_pres_C
  //  Referenced by: '<S2>/ConsEco'

  { 2U, 3U, 4U, 5U, 6U },

  // Expression: Ct_nb_pres_C
  //  Referenced by: '<S2>/ConsAuto'

  { 2U, 3U, 4U, 5U, 6U }
};

// Output and update for atomic system: '<Root>/ConsModeChauf'
void WaterHeatSpCalc::WaterHeatSpCalc_ConsModeChauf() const
{
  switch (*WaterHeatSpCalc_rtu_Ss_op_mode) {
   case te_op_mode::Auto:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.v40_ctrl_Value;
    break;

   case te_op_mode::FullElec:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.v40_ctrl_Value;
    break;

   case te_op_mode::Eco:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.v40_ctrl_Value;
    break;

   case te_op_mode::Boost:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.v40_ctrl_Value;
    break;

   case te_op_mode::Hybrid:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.temp_ctrl_Value;
    break;

   case te_op_mode::AntiLegionella:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.temp_ctrl_Value;
    break;

   case te_op_mode::Holidays:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.temp_ctrl_Value;
    break;

   default:
    *WaterHeatSpCal_rty_Ss_heat_mode = WaterHeatSpCalc_rtP.temp_ctrl_Value;
    break;
  }
}

// System initialize for atomic system: '<Root>/consigne_V40'
void WaterHeatSpCalc::WaterHeatSpCa_consigne_V40_Init()
{
  WaterHeatSpCalc_DW.Cs_Boost_v40_sp_DSTATE = WaterHeatSpCalc_rtP.Cs_Boost_v40_sp_InitialConditio;
}

// System reset for atomic system: '<Root>/consigne_V40'
void WaterHeatSpCalc::WaterHeatSpC_consigne_V40_Reset()
{
  WaterHeatSpCalc_DW.Cs_Boost_v40_sp_DSTATE = WaterHeatSpCalc_rtP.Cs_Boost_v40_sp_InitialConditio;
}

// Output and update for atomic system: '<Root>/consigne_V40'
void WaterHeatSpCalc::WaterHeatSpCalc_consigne_V40()
{
  switch (*WaterHeatSpCalc_rtu_Ss_op_mode) {
   case te_op_mode::Auto:
    *WaterHeatSpCalc_rty_Cs_v40_sp = static_cast<ta_wtr_vol>(WaterHeatSpCalc_rtP.ConsAuto_tableData[plook_u32u8_binckan(*WaterHeatSpCalc_rtu_Ns_pers_nb,
      WaterHeatSpCalc_rtP.ConsAuto_bp01Data, 4U)] + WaterHeatSpCalc_rtP.Cs_bain_auto_elec_C_Value);
    break;

   case te_op_mode::FullElec:
    *WaterHeatSpCalc_rty_Cs_v40_sp = static_cast<ta_wtr_vol>(WaterHeatSpCalc_rtP.ConsAuto_tableData[plook_u32u8_binckan(*WaterHeatSpCalc_rtu_Ns_pers_nb,
      WaterHeatSpCalc_rtP.ConsAuto_bp01Data, 4U)] + WaterHeatSpCalc_rtP.Cs_bain_auto_elec_C_Value);
    break;

   case te_op_mode::Eco:
    *WaterHeatSpCalc_rty_Cs_v40_sp = static_cast<ta_wtr_vol>(WaterHeatSpCalc_rtP.ConsEco_tableData[plook_u32u8_binckan(*WaterHeatSpCalc_rtu_Ns_pers_nb,
      WaterHeatSpCalc_rtP.ConsEco_bp01Data, 4U)] + WaterHeatSpCalc_rtP.Cs_bain_eco_C_Value);
    break;

   case te_op_mode::Boost:
    *WaterHeatSpCalc_rty_Cs_v40_sp = WaterHeatSpCalc_DW.Cs_Boost_v40_sp_DSTATE;
    break;

   case te_op_mode::Hybrid:
    *WaterHeatSpCalc_rty_Cs_v40_sp = WaterHeatSpCalc_rtP.v40NullSpConstant_Value;
    break;

   case te_op_mode::AntiLegionella:
    *WaterHeatSpCalc_rty_Cs_v40_sp = WaterHeatSpCalc_rtP.v40NullSpConstant_Value;
    break;

   case te_op_mode::Holidays:
    *WaterHeatSpCalc_rty_Cs_v40_sp = WaterHeatSpCalc_rtP.v40NullSpConstant_Value;
    break;

   default:
    *WaterHeatSpCalc_rty_Cs_v40_sp = WaterHeatSpCalc_rtP.v40NullSpConstant_Value;
    break;
  }

  WaterHeatSpCalc_DW.Cs_Boost_v40_sp_DSTATE = *WaterHeatSpCalc_rty_Cs_v40_sp;
}

// Output and update for atomic system: '<Root>/consigne_temp'
void WaterHeatSpCalc::WaterHeatSpCalc_consigne_temp() const
{
  switch (*WaterHeatSpCalc_rtu_Ss_op_mode) {
   case te_op_mode::Auto:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.NullConstant_Value;
    break;

   case te_op_mode::FullElec:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.NullConstant_Value;
    break;

   case te_op_mode::Eco:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.NullConstant_Value;
    break;

   case te_op_mode::Boost:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.NullConstant_Value;
    break;

   case te_op_mode::Hybrid:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.Cs_cons_hyb_C_Value;
    break;

   case te_op_mode::AntiLegionella:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.Cs_cons_AL_C_Value;
    break;

   case te_op_mode::Holidays:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.Cs_cons_hol_C_Value;
    break;

   default:
    *WaterHeatSpCalc_rty_Cs_temp_sp = WaterHeatSpCalc_rtP.Cs_cons_test_pac_C_Value;
    break;
  }
}

// System initialize for referenced model: 'WaterHeatSpCalc'
void WaterHeatSpCalc::init(void)
{
  WaterHeatSpCa_consigne_V40_Init();
}

// System reset for referenced model: 'WaterHeatSpCalc'
void WaterHeatSpCalc::reset(void)
{
  WaterHeatSpC_consigne_V40_Reset();
}

// Output and update for referenced model: 'WaterHeatSpCalc'
void WaterHeatSpCalc::step(const te_op_mode *rtu_Ss_op_mode, const ta_pers_nb *rtu_Ns_pers_nb, te_heat_mode *rty_Ss_heat_mode, ta_wtr_vol *rty_Cs_v40_sp,
  ta_temp *rty_Cs_temp_sp)
{
  WaterHeatSpCalc_rtu_Ss_op_mode = rtu_Ss_op_mode;
  WaterHeatSpCalc_rtu_Ns_pers_nb = rtu_Ns_pers_nb;
  WaterHeatSpCal_rty_Ss_heat_mode = rty_Ss_heat_mode;
  WaterHeatSpCalc_rty_Cs_v40_sp = rty_Cs_v40_sp;
  WaterHeatSpCalc_rty_Cs_temp_sp = rty_Cs_temp_sp;
  WaterHeatSpCalc_ConsModeChauf();
  WaterHeatSpCalc_consigne_V40();
  WaterHeatSpCalc_consigne_temp();
}

// Constructor
WaterHeatSpCalc::WaterHeatSpCalc():
  WaterHeatSpCalc_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
WaterHeatSpCalc::~WaterHeatSpCalc() = default;

//
// File trailer for generated code.
//
// [EOF]
//
