//
// File: WaterHeatCtrl.cpp
//
// Code generated for Simulink model 'WaterHeatCtrl'.
//
// Model version                  : 1.73
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:25:04 2025
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
#include "WaterHeatSpCalc.h"
#include "WaterHeatController.h"

// System initialize for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::init(void)
{
  WaterHeatSpCalcMDLOBJ2.init();
  WaterHeatControllerMDLOBJ1.init();
}

// System reset for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::reset(void)
{
  WaterHeatSpCalcMDLOBJ2.reset();
  WaterHeatControllerMDLOBJ1.reset();
}

// Output and update for referenced model: 'WaterHeatCtrl'
void WaterHeatCtrl::step(const tb_WaterHeatCtrl_In *rtu_WaterHeatCtrl_In,
  tb_WaterHeatCtrl_Out *rty_WaterHeatCtrl_Out)
{
  // local block i/o variables
  te_heat_mode rtb_Ss_heat_mode;
  ta_wtr_vol rtb_Cs_v40_sp;
  ta_rot_spd rtb_Cs_heat_pump_rot_spd_sp;
  ta_temp rtb_Cs_temp_sp;
  ta_wtr_vol rtb_Cs_v40_sp_Switch;
  WaterHeatSpCalcMDLOBJ2.step(&rtu_WaterHeatCtrl_In->Ss_op_mode,
    &rtu_WaterHeatCtrl_In->Ns_pers_nb, &rtb_Ss_heat_mode, &rtb_Cs_v40_sp,
    &rtb_Cs_temp_sp);
  if (rtu_WaterHeatCtrl_In->Bs_v40_sp_simu_ena) {
    rtb_Cs_v40_sp_Switch = rtu_WaterHeatCtrl_In->Cs_v40_sp_simu;
  } else {
    rtb_Cs_v40_sp_Switch = rtb_Cs_v40_sp;
  }

  WaterHeatControllerMDLOBJ1.step(&rtu_WaterHeatCtrl_In->Cs_v40_min,
    &rtu_WaterHeatCtrl_In->Cs_tank_down_temp,
    &rtu_WaterHeatCtrl_In->Cs_tank_up_temp, &rtb_Cs_v40_sp_Switch,
    &rtu_WaterHeatCtrl_In->Cs_pump_evap_temp,
    &rtu_WaterHeatCtrl_In->Cs_pump_xhst_temp, &rtb_Cs_heat_pump_rot_spd_sp,
    &rty_WaterHeatCtrl_Out->Cs_v40_rat);
  if (rtu_WaterHeatCtrl_In->Bs_elec_bstr_htr_sp_simu_ena) {
    rty_WaterHeatCtrl_Out->Ss_elec_bstr_htr_sp =
      rtu_WaterHeatCtrl_In->Ss_elec_bstr_htr_sp_simu;
  } else {
    rty_WaterHeatCtrl_Out->Ss_elec_bstr_htr_sp =
      WaterHeatCtrl_DW.WaterHeatController_o2;
  }

  if (rtu_WaterHeatCtrl_In->Bs_heat_pump_freq_sp_simu_ena) {
    rty_WaterHeatCtrl_Out->Cs_heat_pump_rot_spd_sp =
      rtu_WaterHeatCtrl_In->Cs_heat_pump_rot_spd_sp_simu;
  } else {
    rty_WaterHeatCtrl_Out->Cs_heat_pump_rot_spd_sp = rtb_Cs_heat_pump_rot_spd_sp;
  }

  rty_WaterHeatCtrl_Out->Ss_heat_pump_stt = static_cast<te_heat_stt>
    (WaterHeatCtrl_DW.WaterHeatController_o4);
  rty_WaterHeatCtrl_Out->Ss_elec_htr_bstr_stt = static_cast<te_heat_stt>
    (WaterHeatCtrl_DW.WaterHeatController_o5);
  if (rtu_WaterHeatCtrl_In->Bs_heat_mode_simu_ena) {
    rty_WaterHeatCtrl_Out->Ss_heat_mode =
      rtu_WaterHeatCtrl_In->Ss_heat_mode_simu;
  } else {
    rty_WaterHeatCtrl_Out->Ss_heat_mode = rtb_Ss_heat_mode;
  }

  rty_WaterHeatCtrl_Out->Cs_v40_sp = rtb_Cs_v40_sp_Switch;
  if (rtu_WaterHeatCtrl_In->Bs_temp_sp_simu_ena) {
    rty_WaterHeatCtrl_Out->Cs_temp_sp = rtu_WaterHeatCtrl_In->Cs_temp_sp_simu;
  } else {
    rty_WaterHeatCtrl_Out->Cs_temp_sp = rtb_Cs_temp_sp;
  }
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
