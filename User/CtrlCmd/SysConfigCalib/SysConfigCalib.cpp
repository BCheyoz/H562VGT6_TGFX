//
// File: SysConfigCalib.cpp
//
// Code generated for Simulink model 'SysConfigCalib'.
//
// Model version                  : 1.24
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Sep  4 18:36:33 2025
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
#include "SysConfigCalib.h"
#include "SysConfigCalib_types.h"

SysConfigCalib::P_SysConfigCalib_T SysConfigCalib::SysConfigCalib_rtP{
  // Expression: Cs_180L_tank_vol_up_C
  //  Referenced by: '<Root>/Cs_180L_tank_vol_up_C'

  580U,

  // Expression: Cs_180L_tank_vol_btwn_C
  //  Referenced by: '<Root>/Cs_180L_tank_vol_btwn_C'

  440U,

  // Expression: Cs_180L_tank_vol_down_C
  //  Referenced by: '<Root>/Cs_180L_tank_vol_down_C'

  830U,

  // Expression: Cs_180L_tank_vol_tot_C
  //  Referenced by: '<Root>/Cs_180L_tank_vol_tot_C'

  1850U,

  // Expression: Cs_105L_tank_vol_up_C
  //  Referenced by: '<Root>/Cs_105L_tank_vol_up_C'

  350U,

  // Expression: Cs_105L_tank_vol_btwn_C
  //  Referenced by: '<Root>/Cs_105L_tank_vol_btwn_C'

  210U,

  // Expression: Cs_105L_tank_vol_down_C
  //  Referenced by: '<Root>/Cs_105L_tank_vol_down_C'

  550U,

  // Expression: Cs_105L_tank_vol_tot_C
  //  Referenced by: '<Root>/Cs_105L_tank_vol_tot_C'

  1070U
};

// Output and update for referenced model: 'SysConfigCalib'
void SysConfigCalib::step(const te_tank_size *rtu_Control_In_HMI_TECH_Ss_tank, tb_SysConfigCalib_Out *rty_SysConfigCalib_Out) const
{
  ta_wtr_vol rtb_Ct_tank_vol_idx_1;
  ta_wtr_vol rtb_Ct_tank_vol_idx_2;
  ta_wtr_vol rtb_Ct_tank_vol_idx_3;
  if (*rtu_Control_In_HMI_TECH_Ss_tank == te_tank_size::L180) {
    rty_SysConfigCalib_Out->Cs_tank_vol_up = SysConfigCalib_rtP.Cs_180L_tank_vol_up_C_Value;
    rtb_Ct_tank_vol_idx_1 = SysConfigCalib_rtP.Cs_180L_tank_vol_btwn_C_Value;
    rtb_Ct_tank_vol_idx_2 = SysConfigCalib_rtP.Cs_180L_tank_vol_down_C_Value;
    rtb_Ct_tank_vol_idx_3 = SysConfigCalib_rtP.Cs_180L_tank_vol_tot_C_Value;
  } else {
    rty_SysConfigCalib_Out->Cs_tank_vol_up = SysConfigCalib_rtP.Cs_105L_tank_vol_up_C_Value;
    rtb_Ct_tank_vol_idx_1 = SysConfigCalib_rtP.Cs_105L_tank_vol_btwn_C_Value;
    rtb_Ct_tank_vol_idx_2 = SysConfigCalib_rtP.Cs_105L_tank_vol_down_C_Value;
    rtb_Ct_tank_vol_idx_3 = SysConfigCalib_rtP.Cs_105L_tank_vol_tot_C_Value;
  }

  rty_SysConfigCalib_Out->Cs_tank_vol_btwn = rtb_Ct_tank_vol_idx_1;
  rty_SysConfigCalib_Out->Cs_tank_vol_down = rtb_Ct_tank_vol_idx_2;
  rty_SysConfigCalib_Out->Cs_tank_vol_tot = rtb_Ct_tank_vol_idx_3;
}

// Constructor
SysConfigCalib::SysConfigCalib()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
SysConfigCalib::~SysConfigCalib() = default;

//
// File trailer for generated code.
//
// [EOF]
//
