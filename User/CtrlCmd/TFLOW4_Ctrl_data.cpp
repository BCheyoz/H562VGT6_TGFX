//
// File: TFLOW4_Ctrl_data.cpp
//
// Code generated for Simulink model 'TFLOW4_Ctrl'.
//
// Model version                  : 1.359
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:05:50 2025
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
#include "TFLOW4_Ctrl.h"

// Model block global parameters (default storage)
uint8_t rtP_Control_tick_C{ 1U };      // Variable: Control_tick_C
                                          //  Referenced by: '<S4>/SysMng'


uint8_t rtP_WaterHeatCtrl_tick_C{ 1U };// Variable: WaterHeatCtrl_tick_C
                                          //  Referenced by: '<S7>/WaterHeatCtrl'


uint8_t rtP_input_mng_tick{ 1U };      // Variable: input_mng_tick
                                          //  Referenced by: '<S1>/InPutMng'


// Block parameters (default storage)
TFLOW4_Ctrl::P_TFLOW4_Ctrl_T TFLOW4_Ctrl::TFLOW4_Ctrl_P{
  // Computed Parameter: InPutSecu_Out_Y0
  //  Referenced by: '<S1>/InPutSecu_Out'

  {
    0U,                                // Cs_pump_xhst_pres
    0U,                                // Cs_pump_evap_pres
    0,                                 // Cs_tank_down_temp
    0,                                 // Cs_tank_up_temp
    0,                                 // Cs_pump_xhst_temp
    0,                                 // Cs_pump_evap_temp
    0,                                 // Cs_vent_temp
    0U,                                // Cs_vent_pres
    0U,                                // Cs_v40_min
    0U,                                // Cs_v40_mean
    0U,                                // Cs_v40_max
    0U,                                // Cs_vent_rot_spd
    te_on_off::off,                    // Ss_sg_stt
    te_on_off::off,                    // Ss_oph_stt
    te_on_off::off,                    // Ss_vent_sys_stop_stt
    0U,                                // Cs_vent_cnsp
    0U,                                // Cs_heat_wtr_cnsp
    0U,                                // Cs_tot_cnsp
    0U,                                // Cs_vent_pwr
    0U,                                // Cs_heat_wtr_pwr
    0U,                                // Cs_tot_pwr
    false,                             // Bs_tank_down_temp_err
    false,                             // Bs_tank_up_temp_err
    false,                             // Bs_pump_xhst_temp_err
    false,                             // Bs_pump_evap_temp_err
    false,                             // Bs_vent_temp_err
    false,                             // Bs_vent_pres_err
    false,                             // Bs_vent_fan_err
    false                              // Bs_tank_anod_err
  },

  // Computed Parameter: SysMng_Out_Y0
  //  Referenced by: '<S4>/SysMng_Out'

  {
    te_op_mode::FullElec,              // Ss_op_mode
    te_vent_op_mode::Normal,           // Ss_vent_op_mode
    te_on_off::off,                    // Ss_heat_pump_ena
    te_on_off::off                     // Ss_elec_bstr_htr_ena
  },

  // Computed Parameter: WaterHeatDrv_Out_Y0
  //  Referenced by: '<S7>/WaterHeatDrv_Out'

  {
    0U,                                // Cs_heat_pump_rot_spd_sp
    te_on_off::off,                    // Ss_elec_bstr_htr_sp
    te_heat_stt::Stopped,              // Ss_heat_pump_stt
    te_heat_stt::Stopped               // Ss_elec_htr_bstr_stt
  },

  // Computed Parameter: SysConfigCalib_Out_Y0
  //  Referenced by: '<S3>/SysConfigCalib_Out'

  {
    0U,                                // Cs_tank_vol_up
    0U,                                // Cs_tank_vol_btwn
    0U,                                // Cs_tank_vol_down
    0U                                 // Cs_tank_vol_tot
  },

  // Computed Parameter: VentCtrl_Out_Y0
  //  Referenced by: '<S6>/VentCtrl_Out'

  {
    0U,                                // Cs_vent_pres_sp
    0U,                                // Cs_vent_flow_sp
    0U,                                // Cs_vent_vltg_sp
    0U                                 // Cs_pres_mes_filt
  }
};

//
// File trailer for generated code.
//
// [EOF]
//
