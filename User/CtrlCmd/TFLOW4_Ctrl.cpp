//
// File: TFLOW4_Ctrl.cpp
//
// Code generated for Simulink model 'TFLOW4_Ctrl'.
//
// Model version                  : 1.349
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon May 12 09:59:29 2025
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
#include <stdint.h>
#include "TFLOW4_Ctrl_types.h"
#include "SysMng.h"
#include "InPutMng.h"
#include "WaterHeatCtrl.h"
#include "VentCtrl.h"
#include "SysConfigCalib.h"
#include "OutPutMng.h"

// Named constants for Chart: '<Root>/SysScheduler'
const uint8_t TFLOW4_Ctr_WaterHeatCtrl_tick_C{ 1U };

const uint8_t TFLOW4_Ctrl_IN_Initialisation{ 1U };

const uint8_t TFLOW4_Ctrl_IN_Normal{ 2U };

const uint8_t TFLOW4_Ctrl_VentCtrl_tick_C{ 1U };

const tb_InPutMng_In TFLOW4_Ctrl_rtZtb_InPutMng_In{
  te_ctry::France,                     // Ss_ctry
  te_sys_ver::Collective,              // Ss_sys_ver
  0U,                                  // Cs_tank_vol_up
  0U,                                  // Cs_tank_vol_btwn
  0U,                                  // Cs_tank_vol_down
  0U,                                  // Cs_tank_vol_tot
  0,                                   // Cs_tank_down_temp_raw
  0,                                   // Cs_tank_up_temp_raw
  0,                                   // Cs_pump_xhst_temp_raw
  0,                                   // Cs_pump_evap_temp_raw
  0,                                   // Cs_vent_temp_raw
  0U,                                  // Cs_vent_pres_raw
  0U,                                  // Cs_vent_rot_spd_raw
  te_on_off::off,                      // Ss_heat_wtr_cnsp_rst
  te_on_off::off,                      // Ss_vent_cnsp_rst
  te_on_off::off,                      // Ss_tot_cnsp_rst

  {
    te_tor_mode::NotConnected, te_tor_mode::NotConnected }
  ,                                    // St_tor_mode

  {
    te_on_off::off, te_on_off::off }
  ,                                    // St_tor_stt_raw
  false,                               // Bs_err_dtct_shnt_ena
  false,                               // Bs_hw_fan_err
  false,                               // Bs_hw_pres_err
  false,                               // Bs_hw_anod_err
  te_heat_stt::Stopped,                // Ss_heat_pump_stt
  te_heat_stt::Stopped,                // Ss_elec_htr_bstr_stt
  0U,                                  // Cs_heat_pump_pwr
  0,                                   // Cs_tank_down_temp_simu
  0,                                   // Cs_tank_up_temp_simu
  0,                                   // Cs_pump_xhst_temp_simu
  0,                                   // Cs_pump_evap_temp_simu
  0,                                   // Cs_vent_temp_simu
  0U,                                  // Cs_vent_pres_simu
  0U,                                  // Cs_pump_xhst_pres_simu
  0U,                                  // Cs_pump_evap_pres_simu
  0U,                                  // Cs_v40_min_simu
  0U,                                  // Cs_vent_rot_spd_simu
  false,                               // Bs_tank_down_temp_err_simu
  false,                               // Bs_tank_up_temp_err_simu
  false,                               // Bs_pump_xhst_temp_err_simu
  false,                               // Bs_pump_evap_temp_err_simu
  false,                               // Bs_vent_temp_err_simu
  false,                               // Bs_vent_pres_err_simu
  false,                               // Bs_tank_down_temp_simu_ena
  false,                               // Bs_tank_up_temp_simu_ena
  false,                               // Bs_pump_xhst_temp_simu_ena
  false,                               // Bs_pump_evap_temp_simu_ena
  false,                               // Bs_vent_temp_simu_ena
  false,                               // Bs_vent_pres_simu_ena
  false,                               // Bs_tank_down_temp_err_simu_ena
  false,                               // Bs_tank_up_temp_err_simu_ena
  false,                               // Bs_pump_xhst_temp_err_simu_ena
  false,                               // Bs_pump_evap_temp_err_simu_ena
  false,                               // Bs_vent_temp_err_simu_ena
  false,                               // Bs_vent_pres_err_simu_ena
  false,                               // Bs_pump_xhst_pres_simu_ena
  false,                               // Bs_pump_evap_pres_simu_ena
  false,                               // Bs_v40_min_simu_ena
  false                                // Bs_vent_rot_spd_simu_ena
};                                     // tb_InPutMng_In ground

const tb_SysMng_In TFLOW4_Ctrl_rtZtb_SysMng_In{
  0U,                                  // Ns_hldy_nb
  0U,                                  // Ns_pers_nb
  te_user_mode::Auto,                  // Ss_user_mode
  te_on_off::off,                      // Ss_anti_lgn_ena
  te_ctry::France,                     // Ss_ctry
  te_sys_ver::Collective,              // Ss_sys_ver
  te_tech_mode::HeatPump,              // Ss_tech_mode
  0U,                                  // Ns_anti_lgn_day
  te_on_off::off,                      // Ss_heat_pump_test_rqst
  te_on_off::off,                      // Ss_hldy_rqst
  te_on_off::off,                      // Ss_bst_rqst
  te_on_off::off,                      // Ss_sg_stt
  te_on_off::off,                      // Ss_oph_stt
  te_on_off::off,                      // Ss_oph_mode_ena
  te_on_off::off,                      // Ss_vent_sys_stop_stt
  te_heat_stt::Stopped,                // Ss_heat_pump_stt
  te_heat_stt::Stopped,                // Ss_elec_htr_bstr_stt
  false,                               // Bs_tank_down_temp_err
  false,                               // Bs_tank_up_temp_err
  false,                               // Bs_pump_xhst_temp_err
  false,                               // Bs_pump_evap_temp_err
  false,                               // Bs_vent_temp_err
  false,                               // Bs_vent_pres_err
  false,                               // Bs_vent_fan_err
  false                                // Bs_tank_anod_err
};                                     // tb_SysMng_In ground

const tb_VentCtrl_In TFLOW4_Ctrl_rtZtb_VentCtrl_In{
  te_vent_op_mode::Normal,             // Ss_vent_op_mode
  0U,                                  // Cs_vent_pres
  0U,                                  // Cs_vent_pres_min
  0U,                                  // Cs_vent_pres_sys
  false,                               // Bs_vent_pres_err
  0U,                                  // Cs_vent_pres_sp_simu
  0U,                                  // Cs_vent_flow_sp_simu
  0U,                                  // Cs_vent_vltg_sp_simu
  false,                               // Bs_vent_pres_sp_simu_ena
  false,                               // Bs_vent_flow_sp_simu_ena
  false                                // Bs_vent_vltg_sp_simu_ena
};                                     // tb_VentCtrl_In ground

const tb_WaterHeatCtrl_In TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_In{
  te_op_mode::FullElec,                // Ss_op_mode
  0U,                                  // Cs_pump_xhst_pres
  0U,                                  // Cs_pump_evap_pres
  0,                                   // Cs_tank_down_temp
  0,                                   // Cs_tank_up_temp
  0,                                   // Cs_pump_xhst_temp
  0,                                   // Cs_pump_evap_temp
  0,                                   // Cs_vent_temp
  te_on_off::off,                      // Ss_heat_pump_ena
  te_on_off::off,                      // Ss_elec_bstr_htr_ena
  0U,                                  // Cs_v40_min
  0U,                                  // Ns_pers_nb
  te_tank_size::L105,                  // Ss_tank_size
  te_heat_mode::v40_ctrl,              // Ss_heat_mode_simu
  0U,                                  // Cs_v40_sp_simu
  0,                                   // Cs_temp_sp_simu
  0U,                                  // Cs_heat_pump_rot_spd_sp_simu
  te_on_off::off,                      // Ss_elec_bstr_htr_sp_simu
  false,                               // Bs_heat_mode_simu_ena
  false,                               // Bs_v40_sp_simu_ena
  false,                               // Bs_temp_sp_simu_ena
  false,                               // Bs_heat_pump_freq_sp_simu_ena
  false                                // Bs_elec_bstr_htr_sp_simu_ena
};                                     // tb_WaterHeatCtrl_In ground

const tb_OutPutMng_In TFLOW4_Ctrl_rtZtb_OutPutMng_In{
  0U,                                  // Cs_v40_rat
  te_op_mode::FullElec,                // Ss_op_mode
  te_on_off::off,                      // Ss_sg_stt
  te_on_off::off,                      // Ss_oph_stt
  0,                                   // Cs_tank_down_temp
  0,                                   // Cs_tank_up_temp
  0,                                   // Cs_pump_xhst_temp
  0,                                   // Cs_pump_evap_temp
  0,                                   // Cs_vent_temp
  te_on_off::off,                      // Ss_elec_bstr_htr_sp
  0U,                                  // Cs_heat_pump_rot_spd_sp
  0U,                                  // Cs_vent_rot_spd
  0U,                                  // Cs_vent_cnsp
  0U,                                  // Cs_heat_wtr_cnsp
  0U,                                  // Cs_tot_cnsp
  0U,                                  // Cs_v40_sp
  0,                                   // Cs_temp_sp
  0U,                                  // Cs_vent_pres_sp
  0U,                                  // Cs_vent_flow_sp
  0U,                                  // Cs_vent_vltg_sp
  0U                                   // Cs_vent_pres
};                                     // tb_OutPutMng_In ground

const tb_SysMng_Out TFLOW4_Ctrl_rtZtb_SysMng_Out{
  te_op_mode::FullElec,                // Ss_op_mode
  te_vent_op_mode::Normal,             // Ss_vent_op_mode
  te_on_off::off,                      // Ss_heat_pump_ena
  te_on_off::off                       // Ss_elec_bstr_htr_ena
};                                     // tb_SysMng_Out ground

const tb_Control_In TFLOW4_Ctrl_rtZtb_Control_In{ { { false// Bs_err_dtct_shnt_ena
    },                                 // EXPERT

    { 0U,                              // Ns_hldy_nb
      0U,                              // Ns_pers_nb
      te_user_mode::Auto,              // Ss_user_mode
      te_on_off::off,                  // Ss_anti_lgn_ena
      te_on_off::off,                  // Ss_heat_wtr_cnsp_rst
      te_on_off::off,                  // Ss_vent_cnsp_rst
      te_on_off::off,                  // Ss_tot_cnsp_rst
      te_on_off::off,                  // Ss_sg_mode_ena
      te_on_off::off,                  // Ss_oph_mode_ena
      te_on_off::off,                  // Ss_hldy_rqst
      te_on_off::off                   // Ss_bst_rqst
    },                                 // USER

    { te_ctry::France,                 // Ss_ctry
      te_tech_mode::HeatPump,          // Ss_tech_mode
      0U,                              // Ns_anti_lgn_day
      te_on_off::off,                  // Ss_heat_pump_test_rqst
      te_tank_size::L105,              // Ss_tank_size
      te_sys_ver::Collective,          // Ss_sys_ver
      1050U,                           // Cs_vent_pres_min
      1050U,                           // Cs_vent_pres_sys

      { te_tor_mode::NotConnected, te_tor_mode::NotConnected }// St_tor_mode
    }                                  // TECH
  },                                   // HMI

  { 0,                                 // Cs_tank_down_temp_raw
    0,                                 // Cs_tank_up_temp_raw
    0,                                 // Cs_pump_xhst_temp_raw
    0,                                 // Cs_pump_evap_temp_raw
    0,                                 // Cs_vent_temp_raw
    0U,                                // Cs_vent_pres_raw
    0U,                                // Cs_vent_rot_spd_raw

    { te_on_off::off, te_on_off::off },// St_tor_stt_raw
    0U                                 // Cs_heat_pump_pwr
  },                                   // HW

  { false,                             // Bs_hw_fan_err
    false,                             // Bs_hw_pres_err
    false                              // Bs_hw_anod_err
  },                                   // ERR

  { { te_heat_mode::v40_ctrl,          // Ss_heat_mode_simu
      0U,                              // Cs_v40_sp_simu
      0,                               // Cs_temp_sp_simu
      0U,                              // Cs_heat_pump_rot_spd_sp_simu
      te_on_off::off,                  // Ss_elec_bstr_htr_sp_simu
      0U,                              // Cs_vent_pres_sp_simu
      0U,                              // Cs_vent_flow_sp_simu
      0U,                              // Cs_vent_vltg_sp_simu
      0,                               // Cs_tank_down_temp_simu
      0,                               // Cs_tank_up_temp_simu
      0,                               // Cs_pump_xhst_temp_simu
      0,                               // Cs_pump_evap_temp_simu
      0,                               // Cs_vent_temp_simu
      0U,                              // Cs_vent_pres_simu
      false,                           // Bs_tank_down_temp_err_simu
      false,                           // Bs_tank_up_temp_err_simu
      false,                           // Bs_pump_xhst_temp_err_simu
      false,                           // Bs_pump_evap_temp_err_simu
      false,                           // Bs_vent_temp_err_simu
      false,                           // Bs_vent_pres_err_simu
      0U,                              // Cs_pump_xhst_pres_simu
      0U,                              // Cs_pump_evap_pres_simu
      0U,                              // Cs_v40_min_simu
      0U                               // Cs_vent_rot_spd_simu
    },                                 // VAR

    { false,                           // Bs_heat_mode_simu_ena
      false,                           // Bs_v40_sp_simu_ena
      false,                           // Bs_temp_sp_simu_ena
      false,                           // Bs_heat_pump_freq_sp_simu_ena
      false,                           // Bs_elec_bstr_htr_sp_simu_ena
      false,                           // Bs_vent_pres_sp_simu_ena
      false,                           // Bs_vent_flow_sp_simu_ena
      false,                           // Bs_vent_vltg_sp_simu_ena
      false,                           // Bs_tank_down_temp_simu_ena
      false,                           // Bs_tank_up_temp_simu_ena
      false,                           // Bs_pump_xhst_temp_simu_ena
      false,                           // Bs_pump_evap_temp_simu_ena
      false,                           // Bs_vent_temp_simu_ena
      false,                           // Bs_vent_pres_simu_ena
      false,                           // Bs_tank_down_temp_err_simu_ena
      false,                           // Bs_tank_up_temp_err_simu_ena
      false,                           // Bs_pump_xhst_temp_err_simu_ena
      false,                           // Bs_pump_evap_temp_err_simu_ena
      false,                           // Bs_vent_temp_err_simu_ena
      false,                           // Bs_vent_pres_err_simu_ena
      false,                           // Bs_pump_xhst_pres_simu_ena
      false,                           // Bs_pump_evap_pres_simu_ena
      false,                           // Bs_v40_min_simu_ena
      false                            // Bs_vent_rot_spd_simu_ena
    }                                  // ENA
  }                                    // SIMU
};

const tb_Control_Out TFLOW4_Ctrl_rtZtb_Control_Out{ 0U,// Cs_v40_lvl
  te_op_mode::FullElec,                // Ss_op_mode
  te_on_off::off,                      // Ss_sg_stt
  te_on_off::off,                      // Ss_oph_stt
  0,                                   // Cs_tank_down_temp
  0,                                   // Cs_tank_up_temp
  0,                                   // Cs_pump_xhst_temp
  0,                                   // Cs_pump_evap_temp
  0,                                   // Cs_vent_temp
  te_on_off::off,                      // Ss_elec_bstr_htr_sp
  0U,                                  // Cs_heat_pump_rot_spd_sp
  0U,                                  // Cs_vent_rot_spd
  0U,                                  // Cs_vent_cnsp
  0U,                                  // Cs_heat_wtr_cnsp
  0U,                                  // Cs_tot_cnsp
  0U,                                  // Cs_v40_sp
  0,                                   // Cs_temp_sp
  0U,                                  // Cs_vent_pres_sp
  0U,                                  // Cs_vent_flow_sp
  0U,                                  // Cs_vent_vltg_sp
  0U                                   // Cs_vent_pres
};

// Output and update for atomic system: '<Root>/OutPutMng_Interfaces'
void TFLOW4_Ctrl::TFLOW4_Ctr_OutPutMng_Interfaces()
{
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_v40_rat = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_v40_rat;
  TFLOW4_Ctrl_DW.OutPutMng_In.Ss_op_mode = TFLOW4_Ctrl_DW.SysMng_Out.Ss_op_mode;
  TFLOW4_Ctrl_DW.OutPutMng_In.Ss_sg_stt = TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_sg_stt;
  TFLOW4_Ctrl_DW.OutPutMng_In.Ss_oph_stt = TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_oph_stt;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_tank_down_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_down_temp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_tank_up_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_up_temp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_pump_xhst_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_temp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_pump_evap_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_temp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_temp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Ss_elec_bstr_htr_sp = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_elec_bstr_htr_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_heat_pump_rot_spd_sp = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_heat_pump_rot_spd_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_rot_spd = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_rot_spd;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_cnsp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_cnsp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_heat_wtr_cnsp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_heat_wtr_cnsp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_tot_cnsp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tot_cnsp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_v40_sp = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_v40_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_temp_sp = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_temp_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_pres_sp = TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_pres_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_flow_sp = TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_flow_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_vltg_sp = TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_vltg_sp;
  TFLOW4_Ctrl_DW.OutPutMng_In.Cs_vent_pres = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_pres;
}

// Output and update for atomic system: '<S7>/WaterHeatCtrl_Interfaces'
void TFLOW4_Ctrl::TFLOW4_WaterHeatCtrl_Interfaces()
{
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_op_mode = TFLOW4_Ctrl_DW.SysMng_Out.Ss_op_mode;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_pump_xhst_pres = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_pres;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_pump_evap_pres = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_pres;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_tank_down_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_down_temp;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_tank_up_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_up_temp;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_pump_xhst_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_temp;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_pump_evap_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_temp;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_vent_temp = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_temp;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_heat_pump_ena = TFLOW4_Ctrl_DW.SysMng_Out.Ss_heat_pump_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_elec_bstr_htr_ena = TFLOW4_Ctrl_DW.SysMng_Out.Ss_elec_bstr_htr_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_v40_min = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_v40_min;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ns_pers_nb = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ns_pers_nb;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_tank_size = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tank_size;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_heat_mode_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Ss_heat_mode_simu;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_v40_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_v40_sp_simu;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_temp_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_temp_sp_simu;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Cs_heat_pump_rot_spd_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_heat_pump_rot_spd_sp_simu;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Ss_elec_bstr_htr_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Ss_elec_bstr_htr_sp_simu;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Bs_heat_mode_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_heat_mode_simu_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Bs_v40_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_v40_sp_simu_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Bs_temp_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_temp_sp_simu_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Bs_heat_pump_freq_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_heat_pump_freq_sp_simu_ena;
  TFLOW4_Ctrl_DW.In_WaterHeatCtrl.Bs_elec_bstr_htr_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_elec_bstr_htr_sp_simu_ena;
}

// Output and update for atomic system: '<S6>/VentCtrl_Interfaces'
void TFLOW4_Ctrl::TFLOW4_Ctrl_VentCtrl_Interfaces()
{
  TFLOW4_Ctrl_DW.In_VentCtrl.Ss_vent_op_mode = TFLOW4_Ctrl_DW.SysMng_Out.Ss_vent_op_mode;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_pres = TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_pres;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_pres_min = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Cs_vent_pres_min;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_pres_sys = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Cs_vent_pres_sys;
  TFLOW4_Ctrl_DW.In_VentCtrl.Bs_vent_pres_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_pres_err;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_pres_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_pres_sp_simu;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_flow_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_flow_sp_simu;
  TFLOW4_Ctrl_DW.In_VentCtrl.Cs_vent_vltg_sp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_vltg_sp_simu;
  TFLOW4_Ctrl_DW.In_VentCtrl.Bs_vent_pres_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_sp_simu_ena;
  TFLOW4_Ctrl_DW.In_VentCtrl.Bs_vent_flow_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_flow_sp_simu_ena;
  TFLOW4_Ctrl_DW.In_VentCtrl.Bs_vent_vltg_sp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_vltg_sp_simu_ena;
}

// Output and update for atomic system: '<S1>/InPutMng_Interfaces'
void TFLOW4_Ctrl::TFLOW4_Ctrl_InPutMng_Interfaces()
{
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_ctry = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_ctry;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_sys_ver = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_sys_ver;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_vol_up = TFLOW4_Ctrl_DW.SysConfigCalib_Out.Cs_tank_vol_up;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_vol_btwn = TFLOW4_Ctrl_DW.SysConfigCalib_Out.Cs_tank_vol_btwn;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_vol_down = TFLOW4_Ctrl_DW.SysConfigCalib_Out.Cs_tank_vol_down;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_vol_tot = TFLOW4_Ctrl_DW.SysConfigCalib_Out.Cs_tank_vol_tot;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_down_temp_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_tank_down_temp_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_up_temp_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_tank_up_temp_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_xhst_temp_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_pump_xhst_temp_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_evap_temp_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_pump_evap_temp_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_temp_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_temp_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_pres_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_pres_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_rot_spd_raw = TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_rot_spd_raw;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_heat_wtr_cnsp_rst = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_heat_wtr_cnsp_rst;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_vent_cnsp_rst = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_vent_cnsp_rst;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_tot_cnsp_rst = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_tot_cnsp_rst;
  TFLOW4_Ctrl_DW.In_InPutMng.St_tor_mode[0] = TFLOW4_Ctrl_U.Control_In.HMI.TECH.St_tor_mode[0];
  TFLOW4_Ctrl_DW.In_InPutMng.St_tor_stt_raw[0] = TFLOW4_Ctrl_U.Control_In.HW.St_tor_stt_raw[0];
  TFLOW4_Ctrl_DW.In_InPutMng.St_tor_mode[1] = TFLOW4_Ctrl_U.Control_In.HMI.TECH.St_tor_mode[1];
  TFLOW4_Ctrl_DW.In_InPutMng.St_tor_stt_raw[1] = TFLOW4_Ctrl_U.Control_In.HW.St_tor_stt_raw[1];
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_err_dtct_shnt_ena = TFLOW4_Ctrl_U.Control_In.HMI.EXPERT.Bs_err_dtct_shnt_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_hw_fan_err = TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_fan_err;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_hw_pres_err = TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_pres_err;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_hw_anod_err = TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_anod_err;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_heat_pump_stt = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_heat_pump_stt;
  TFLOW4_Ctrl_DW.In_InPutMng.Ss_elec_htr_bstr_stt = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_elec_htr_bstr_stt;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_heat_pump_pwr = TFLOW4_Ctrl_U.Control_In.HW.Cs_heat_pump_pwr;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_down_temp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_tank_down_temp_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_tank_up_temp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_tank_up_temp_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_xhst_temp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_xhst_temp_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_evap_temp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_evap_temp_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_temp_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_temp_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_pres_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_pres_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_xhst_pres_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_xhst_pres_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_pump_evap_pres_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_evap_pres_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_v40_min_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_v40_min_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Cs_vent_rot_spd_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_rot_spd_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_down_temp_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_tank_down_temp_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_up_temp_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_tank_up_temp_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_xhst_temp_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_pump_xhst_temp_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_evap_temp_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_pump_evap_temp_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_temp_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_vent_temp_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_pres_err_simu = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_vent_pres_err_simu;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_down_temp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_down_temp_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_up_temp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_up_temp_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_xhst_temp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_temp_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_evap_temp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_temp_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_temp_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_temp_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_pres_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_down_temp_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_down_temp_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_tank_up_temp_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_up_temp_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_xhst_temp_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_temp_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_evap_temp_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_temp_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_temp_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_temp_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_pres_err_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_err_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_xhst_pres_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_pres_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_pump_evap_pres_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_pres_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_v40_min_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_v40_min_simu_ena;
  TFLOW4_Ctrl_DW.In_InPutMng.Bs_vent_rot_spd_simu_ena = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_rot_spd_simu_ena;
}

// Output and update for atomic system: '<S4>/SysMng_Interfaces'
void TFLOW4_Ctrl::TFLOW4_Ctrl_SysMng_Interfaces()
{
  TFLOW4_Ctrl_DW.SysMng_In.Ns_hldy_nb = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ns_hldy_nb;
  TFLOW4_Ctrl_DW.SysMng_In.Ns_pers_nb = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ns_pers_nb;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_user_mode = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_anti_lgn_ena = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_anti_lgn_ena;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_ctry = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_ctry;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_sys_ver = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_sys_ver;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_tech_mode = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode;
  TFLOW4_Ctrl_DW.SysMng_In.Ns_anti_lgn_day = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ns_anti_lgn_day;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_heat_pump_test_rqst = TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_heat_pump_test_rqst;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_hldy_rqst = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_hldy_rqst;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_bst_rqst = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_bst_rqst;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_sg_stt = TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_sg_stt;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_oph_stt = TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_oph_stt;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_oph_mode_ena = TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_oph_mode_ena;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_vent_sys_stop_stt = TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_vent_sys_stop_stt;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_heat_pump_stt = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_heat_pump_stt;
  TFLOW4_Ctrl_DW.SysMng_In.Ss_elec_htr_bstr_stt = TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_elec_htr_bstr_stt;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_tank_down_temp_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_tank_down_temp_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_tank_up_temp_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_tank_up_temp_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_pump_xhst_temp_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_pump_xhst_temp_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_pump_evap_temp_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_pump_evap_temp_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_vent_temp_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_temp_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_vent_pres_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_pres_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_vent_fan_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_fan_err;
  TFLOW4_Ctrl_DW.SysMng_In.Bs_tank_anod_err = TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_tank_anod_err;
}

// Model step function
void TFLOW4_Ctrl::step()
{
  if (TFLOW4_Ctrl_DW.temporalCounter_i1 < 1) {
    TFLOW4_Ctrl_DW.temporalCounter_i1 = 1U;
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i2 < TFLOW4_Ctr_WaterHeatCtrl_tick_C) {
    TFLOW4_Ctrl_DW.temporalCounter_i2 = 1U;
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i3 < TFLOW4_Ctrl_VentCtrl_tick_C) {
    TFLOW4_Ctrl_DW.temporalCounter_i3 = 1U;
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i4 < 7) {
    TFLOW4_Ctrl_DW.temporalCounter_i4 = static_cast<uint8_t>(TFLOW4_Ctrl_DW.temporalCounter_i4 + 1);
  }

  if (TFLOW4_Ctrl_DW.is_active_c2_TFLOW4_Ctrl == 0) {
    TFLOW4_Ctrl_DW.is_active_c2_TFLOW4_Ctrl = 1U;
    TFLOW4_Ctrl_DW.temporalCounter_i1 = 0U;
    TFLOW4_Ctrl_DW.temporalCounter_i4 = 0U;
    TFLOW4_Ctrl_DW.is_c2_TFLOW4_Ctrl = TFLOW4_Ctrl_IN_Initialisation;
  } else if (TFLOW4_Ctrl_DW.is_c2_TFLOW4_Ctrl == TFLOW4_Ctrl_IN_Initialisation) {
    if (TFLOW4_Ctrl_DW.temporalCounter_i4 >= 5) {
      TFLOW4_Ctrl_DW.temporalCounter_i1 = 0U;
      TFLOW4_Ctrl_DW.temporalCounter_i2 = 0U;
      TFLOW4_Ctrl_DW.temporalCounter_i3 = 0U;
      TFLOW4_Ctrl_DW.is_c2_TFLOW4_Ctrl = TFLOW4_Ctrl_IN_Normal;
    } else if (TFLOW4_Ctrl_DW.temporalCounter_i1 == 1) {
      SysConfigCalibMDLOBJ6.step(&TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tank_size, &TFLOW4_Ctrl_DW.SysConfigCalib_Out);
    }
  } else {
    // case IN_Normal:
    if (TFLOW4_Ctrl_DW.temporalCounter_i1 == 1) {
      TFLOW4_Ctrl_InPutMng_Interfaces();
      InPutMngMDLOBJ1.step(&TFLOW4_Ctrl_DW.In_InPutMng, &TFLOW4_Ctrl_DW.InPutSecu_Out);
      TFLOW4_Ctrl_SysMng_Interfaces();
      SysMngMDLOBJ3.step(&TFLOW4_Ctrl_DW.SysMng_In, &TFLOW4_Ctrl_DW.SysMng_Out);
    }

    if (TFLOW4_Ctrl_DW.temporalCounter_i2 == TFLOW4_Ctr_WaterHeatCtrl_tick_C) {
      TFLOW4_WaterHeatCtrl_Interfaces();
      WaterHeatCtrlMDLOBJ5.step(&TFLOW4_Ctrl_DW.In_WaterHeatCtrl, &TFLOW4_Ctrl_DW.WaterHeatDrv_Out);
    }

    if (TFLOW4_Ctrl_DW.temporalCounter_i3 == TFLOW4_Ctrl_VentCtrl_tick_C) {
      TFLOW4_Ctrl_VentCtrl_Interfaces();
      VentCtrlMDLOBJ4.step(&TFLOW4_Ctrl_DW.In_VentCtrl, &TFLOW4_Ctrl_DW.VentCtrl_Out);
    }
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i1 == 1) {
    TFLOW4_Ctrl_DW.temporalCounter_i1 = 0U;
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i2 == TFLOW4_Ctr_WaterHeatCtrl_tick_C) {
    TFLOW4_Ctrl_DW.temporalCounter_i2 = 0U;
  }

  if (TFLOW4_Ctrl_DW.temporalCounter_i3 == TFLOW4_Ctrl_VentCtrl_tick_C) {
    TFLOW4_Ctrl_DW.temporalCounter_i3 = 0U;
  }

  TFLOW4_Ctr_OutPutMng_Interfaces();
  OutPutMngMDLOBJ2.step(&TFLOW4_Ctrl_DW.OutPutMng_In, &TFLOW4_Ctrl_Y.Control_Out);
}

// Model initialize function
void TFLOW4_Ctrl::initialize()
{
  // Registration code

  // states (dwork)
  {
    TFLOW4_Ctrl_DW.In_InPutMng = TFLOW4_Ctrl_rtZtb_InPutMng_In;
    TFLOW4_Ctrl_DW.SysMng_In = TFLOW4_Ctrl_rtZtb_SysMng_In;
    TFLOW4_Ctrl_DW.In_WaterHeatCtrl = TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_In;
    TFLOW4_Ctrl_DW.OutPutMng_In = TFLOW4_Ctrl_rtZtb_OutPutMng_In;
    TFLOW4_Ctrl_DW.In_VentCtrl = TFLOW4_Ctrl_rtZtb_VentCtrl_In;
    TFLOW4_Ctrl_DW.SysMng_Out = TFLOW4_Ctrl_rtZtb_SysMng_Out;
  }

  // external inputs
  TFLOW4_Ctrl_U.Control_In = TFLOW4_Ctrl_rtZtb_Control_In;

  // external outputs
  TFLOW4_Ctrl_Y.Control_Out = TFLOW4_Ctrl_rtZtb_Control_Out;

  // Model Initialize function for ModelReference Block: '<S4>/SysMng'
  SysMngMDLOBJ3.initialize();
  WaterHeatCtrlMDLOBJ5.init();
  TFLOW4_Ctrl_DW.WaterHeatDrv_Out = TFLOW4_Ctrl_P.WaterHeatDrv_Out_Y0;
  VentCtrlMDLOBJ4.init();
  TFLOW4_Ctrl_DW.VentCtrl_Out = TFLOW4_Ctrl_P.VentCtrl_Out_Y0;
  TFLOW4_Ctrl_DW.SysConfigCalib_Out = TFLOW4_Ctrl_P.SysConfigCalib_Out_Y0;
  InPutMngMDLOBJ1.init();
  TFLOW4_Ctrl_DW.InPutSecu_Out = TFLOW4_Ctrl_P.InPutSecu_Out_Y0;
  SysMngMDLOBJ3.init();
  TFLOW4_Ctrl_DW.SysMng_Out = TFLOW4_Ctrl_P.SysMng_Out_Y0;
}

// Model terminate function
void TFLOW4_Ctrl::terminate()
{
  // (no terminate code required)
}

// Constructor
TFLOW4_Ctrl::TFLOW4_Ctrl():
  TFLOW4_Ctrl_U(),
  TFLOW4_Ctrl_Y(),
  TFLOW4_Ctrl_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
TFLOW4_Ctrl::~TFLOW4_Ctrl() = default;

//
// File trailer for generated code.
//
// [EOF]
//
