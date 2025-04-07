//
// File: TFLOW4_Ctrl.cpp
//
// Code generated for Simulink model 'TFLOW4_Ctrl'.
//
// Model version                  : 1.309
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:25:49 2025
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
#include <stdbool.h>
#include <stdint.h>
#include "TFLOW4_Ctrl_types.h"
#include "WaterHeatCtrl.h"
#include "VentCtrl.h"
#include "RisingEdgeDetect.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "SysConfigCalib.h"
#include "Timer.h"
#include "InPutMng.h"
#include "OutPutMng.h"

// Named constants for Chart: '<S4>/SysScheduler'
const uint8_t TFLOW4_Ctr_WaterHeatCtrl_tick_C{ 1U };

const uint8_t TFLOW4_Ctrl_VentCtrl_tick_C{ 1U };

// Named constants for Chart: '<S4>/modeSelection'
const uint8_t TFLOW4_Ct_IN_SmartGrid_fullElec{ 1U };

const uint8_t TFLOW4_Ctrl_IN_AntiLegionella{ 1U };

const uint8_t TFLOW4_Ctrl_IN_Auto{ 1U };

const uint8_t TFLOW4_Ctrl_IN_Boost{ 1U };

const uint8_t TFLOW4_Ctrl_IN_Eco{ 2U };

const uint8_t TFLOW4_Ctrl_IN_FullElec{ 1U };

const uint8_t TFLOW4_Ctrl_IN_HeatPumpMode{ 2U };

const uint8_t TFLOW4_Ctrl_IN_HeatPumpUsed{ 2U };

const uint8_t TFLOW4_Ctrl_IN_Holidays{ 2U };

const uint8_t TFLOW4_Ctrl_IN_Hybrid{ 1U };

const uint8_t TFLOW4_Ctrl_IN_ModeSelection{ 3U };

const uint8_t TFLOW4_Ctrl_IN_SmartGrid{ 4U };

const uint8_t TFLOW4_Ctrl_IN_SmartGrid_normal{ 2U };

const uint8_t TFLOW4_Ctrl_IN_SystemMode{ 1U };

const uint8_t TFLOW4_Ctrl_IN_TestMode{ 2U };

const uint8_t TFLOW4_Ctrl_IN_UsualMode{ 2U };

const uint8_t TFLOW4_IN_AntiLegionella_normal{ 2U };

const uint8_t TFLO_IN_AntiLegionella_fullElec{ 1U };

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

const tb_Control_In TFLOW4_Ctrl_rtZtb_Control_In{ { { false// Bs_err_dtct_shnt_ena 
    },                                 // EXPERT

    { 0U,                              // Ns_hldy_nb
      0U,                              // Ns_pers_nb
      te_user_mode::user_Auto,         // Ss_user_mode
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
      te_tech_mode::tech_FullElec,     // Ss_tech_mode
      0U,                              // Ns_anti_lgn_day
      te_on_off::off,                  // Ss_heat_pump_test_rqst
      te_tank_size::L105,              // Ss_tank_size
      te_sys_ver::Collective,          // Ss_sys_ver
      0U,                              // Cs_vent_pres_min
      0U,                              // Cs_vent_pres_sys

      { te_tor_mode::tor_NotConnected, te_tor_mode::tor_NotConnected }// St_tor_mode 
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

// Function for Chart: '<S4>/modeSelection'
void TFLOW4_Ctrl::TFL_enter_internal_HeatPumpUsed(void)
{
  if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
      tech_HeatPump) {
    TFLOW4_Ctrl_DW.is_HeatPumpUsed = TFLOW4_Ctrl_IN_UsualMode;
    TFLOW4_Ctrl_DW.is_UsualMode = TFLOW4_Ctrl_IN_HeatPumpMode;
    if (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode == te_user_mode::
        user_Auto) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Auto;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Auto;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Eco;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Eco;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    }
  } else {
    TFLOW4_Ctrl_DW.is_HeatPumpUsed = TFLOW4_Ctrl_IN_Hybrid;
    TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Hybrid;
    TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
    TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
  }
}

// Function for Chart: '<S4>/modeSelection'
void TFLOW4_Ctrl::TFLOW4_Ctrl_ModeSelection(const bool *HolidaysModeActDelay,
  const bool *SmartGridActDelay, const bool *AntiLegionnellaModeActDelay)
{
  if (*AntiLegionnellaModeActDelay) {
    TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
    TFLOW4_Ctrl_DW.is_UsualMode = 0;
    TFLOW4_Ctrl_DW.is_HeatPumpUsed = 0;
    TFLOW4_Ctrl_DW.is_ModeSelection = 0;
    TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_AntiLegionella;
    TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::AntiLegionella;
    TFLOW4_Ctrl_DW.Bs_clk_on = false;
    if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode != te_tech_mode::
        tech_FullElec) {
      TFLOW4_Ctrl_DW.is_AntiLegionella = TFLOW4_IN_AntiLegionella_normal;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else {
      TFLOW4_Ctrl_DW.is_AntiLegionella = TFLO_IN_AntiLegionella_fullElec;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    }
  } else if (*SmartGridActDelay) {
    TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
    TFLOW4_Ctrl_DW.is_UsualMode = 0;
    TFLOW4_Ctrl_DW.is_HeatPumpUsed = 0;
    TFLOW4_Ctrl_DW.is_ModeSelection = 0;
    TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_SmartGrid;
    TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::SmartGrid;
    if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode != te_tech_mode::
        tech_FullElec) {
      TFLOW4_Ctrl_DW.is_SmartGrid = TFLOW4_Ctrl_IN_SmartGrid_normal;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      TFLOW4_Ctrl_DW.is_SmartGrid = TFLOW4_Ct_IN_SmartGrid_fullElec;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    }
  } else {
    TFLOW4_Ctrl_DW.Bs_clk_on =
      ((TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_anti_lgn_ena == te_on_off::on) &&
       (TFLOW4_Ctrl_DW.Ss_op_mode != te_op_mode::Holidays));
    if (TFLOW4_Ctrl_DW.is_ModeSelection == TFLOW4_Ctrl_IN_FullElec) {
      if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode != te_tech_mode::
          tech_FullElec) {
        TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
        TFL_enter_internal_HeatPumpUsed();
      } else {
        TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
      }

      // case IN_HeatPumpUsed:
    } else if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
               tech_FullElec) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
      TFLOW4_Ctrl_DW.is_UsualMode = 0;
      TFLOW4_Ctrl_DW.is_HeatPumpUsed = 0;
      TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else if (*HolidaysModeActDelay) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
      TFLOW4_Ctrl_DW.is_UsualMode = 0;
      TFLOW4_Ctrl_DW.is_HeatPumpUsed = 0;
      TFLOW4_Ctrl_DW.is_ModeSelection = 0;
      TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_Holidays;
      TFLOW4_Ctrl_DW.Bs_clk_on = false;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Holidays;
    } else if (TFLOW4_Ctrl_DW.is_HeatPumpUsed == TFLOW4_Ctrl_IN_Hybrid) {
      if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
          tech_HeatPump) {
        TFLOW4_Ctrl_DW.is_HeatPumpUsed = TFLOW4_Ctrl_IN_UsualMode;
        TFLOW4_Ctrl_DW.is_UsualMode = TFLOW4_Ctrl_IN_HeatPumpMode;
        if (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode == te_user_mode::
            user_Auto) {
          TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Auto;
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Auto;
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
        } else {
          TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Eco;
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Eco;
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
      } else {
        TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Hybrid;
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
      }

      // case IN_UsualMode:
    } else if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
               tech_Hybrid) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
      TFLOW4_Ctrl_DW.is_UsualMode = 0;
      TFLOW4_Ctrl_DW.is_HeatPumpUsed = TFLOW4_Ctrl_IN_Hybrid;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Hybrid;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    } else if (TFLOW4_Ctrl_DW.is_UsualMode == TFLOW4_Ctrl_IN_Boost) {
      if (!TFLOW4_Ctrl_DW.stt) {
        TFLOW4_Ctrl_DW.is_UsualMode = TFLOW4_Ctrl_IN_HeatPumpMode;
        if (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode == te_user_mode::
            user_Auto) {
          TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Auto;
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Auto;
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
        } else {
          TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Eco;
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Eco;
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
      } else {
        TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Boost;
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
      }

      // case IN_HeatPumpMode:
    } else if (TFLOW4_Ctrl_DW.stt) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
      TFLOW4_Ctrl_DW.is_UsualMode = TFLOW4_Ctrl_IN_Boost;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Boost;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else if (TFLOW4_Ctrl_DW.is_HeatPumpMode == TFLOW4_Ctrl_IN_Auto) {
      if (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode == te_user_mode::
          user_Eco) {
        TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Eco;
        TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Eco;
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
      } else {
        TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Auto;
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
      }

      // case IN_Eco:
    } else if (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_user_mode == te_user_mode::
               user_Auto) {
      TFLOW4_Ctrl_DW.is_HeatPumpMode = TFLOW4_Ctrl_IN_Auto;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Auto;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Eco;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
    }
  }
}

// System initialize for atomic system: '<Root>/SysMng'
void TFLOW4_Ctrl::TFLOW4_Ctrl_SysMng_Init()
{
  TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE =
    TFLOW4_Ctrl_P.HolidaysModeActDelay_InitialCon;
  TFLOW4_Ctrl_DW.SmartGridActDelay_DSTATE =
    TFLOW4_Ctrl_P.SmartGridActDelay_InitialCondit;
  TFLOW4_Ctrl_DW.opModeDelay_DSTATE = TFLOW4_Ctrl_P.opModeDelay_InitialCondition;
  TFLOW4_Ctrl_DW.AntiLegionnellaModeActDelay_DST =
    TFLOW4_Ctrl_P.AntiLegionnellaModeActDelay_Ini;
  TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
  WaterHeatCtrlMDLOBJ16.init();
  TFLOW4_Ctrl_DW.WaterHeatDrv_Out = TFLOW4_Ctrl_P.WaterHeatDrv_Out_Y0;
  VentCtrlMDLOBJ15.init();
  TFLOW4_Ctrl_DW.VentCtrl_Out = TFLOW4_Ctrl_P.VentCtrl_Out_Y0;
  TFLOW4_Ctrl_DW.SysConfigCalib_j = TFLOW4_Ctrl_P.SysConfigCalib_Out_Y0;
}

// Output and update for atomic system: '<Root>/SysMng'
void TFLOW4_Ctrl::TFLOW4_Ctrl_SysMng()
{
  // local block i/o variables
  float rtb_Timer_o1;
  uint8_t rtb_Out;
  bool rtb_TimerState;
  tb_VentCtrl_In rtb_BusConversion_InsertedFor_V;
  uint8_t rtb_bool2uint[2];
  uint8_t rtb_Bs_clk_rch_uint;
  uint8_t rtb_Bs_heat_pump_test_rqst_ui_0;
  uint8_t rtb_Bs_mode_chk_hldy_rqst_on_ui;
  uint8_t rtb_Ss_bst_rqst_on_uint;
  uint8_t rtb_Ss_op_mode_prev_uint;
  bool HolidaysModeActDelay;
  bool SmartGridActDelay;
  bool rtb_AND_HeatMode_rst;
  bool rtb_AND_HeatMode_set;
  bool rtb_Bs_anti_lgn_rst;
  bool rtb_Bs_anti_lgn_set;
  bool rtb_Bs_bst_mode_rst_n;
  bool rtb_Bs_bst_mode_set;
  bool rtb_Bs_hldy_mode;
  bool rtb_Bs_mode_chk_hldy_rqst_on_g;
  bool rtb_Bs_test_mode_stt;
  bool rtb_OR_Holidays_rst;
  HolidaysModeActDelay = TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE;
  SmartGridActDelay = TFLOW4_Ctrl_DW.SmartGridActDelay_DSTATE;
  rtb_Ss_bst_rqst_on_uint = (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_bst_rqst ==
    TFLOW4_Ctrl_P.Contant_Value);
  BoostRqstRisingEdgeMDLOBJ4.step(&rtb_Ss_bst_rqst_on_uint,
    &HolidaysModeActDelay);
  rtb_Bs_bst_mode_set = (HolidaysModeActDelay &&
    (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode ==
     TFLOW4_Ctrl_P.Constant1_Value_i));
  rtb_Bs_bst_mode_rst_n = ((HolidaysModeActDelay ||
    (((TFLOW4_Ctrl_DW.Ss_heat_pump_stt == TFLOW4_Ctrl_P.Constant4_Value) ||
      (TFLOW4_Ctrl_DW.Ss_heat_pump_stt == TFLOW4_Ctrl_P.Constant7_Value)) &&
     ((TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt == TFLOW4_Ctrl_P.Constant6_Value) ||
      (TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt == TFLOW4_Ctrl_P.Constant8_Value)))) &&
    (TFLOW4_Ctrl_DW.opModeDelay_DSTATE == TFLOW4_Ctrl_P.Constant2_Value_m));
  R_S_FlipFlopMDLOBJ5.step(&rtb_Bs_bst_mode_set, &rtb_Bs_bst_mode_rst_n,
    &TFLOW4_Ctrl_DW.stt);
  rtb_Bs_heat_pump_test_rqst_ui_0 = static_cast<uint8_t>
    (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_heat_pump_test_rqst);
  HeatPumpTestRqstRisiMDLOBJ7.step(&rtb_Bs_heat_pump_test_rqst_ui_0,
    &HolidaysModeActDelay);
  rtb_Bs_test_mode_stt = (TFLOW4_Ctrl_DW.opModeDelay_DSTATE ==
    TFLOW4_Ctrl_P.HeatPumpTestCstt_Value);
  rtb_AND_HeatMode_set = (HolidaysModeActDelay && (!rtb_Bs_test_mode_stt));
  rtb_Ss_op_mode_prev_uint = rtb_Bs_test_mode_stt;
  RisingEdgeDelayMDLOBJ10.step(&rtb_Ss_op_mode_prev_uint, &rtb_Out,
    TFLOW4_Ctrl_P.RisingEdgeDelay_delay, rtP_Control_tick_C);
  HeatPumpTestRqstRstRMDLOBJ8.step(&rtb_Bs_heat_pump_test_rqst_ui_0,
    &HolidaysModeActDelay);
  rtb_AND_HeatMode_rst = (((rtb_Out != 0) || HolidaysModeActDelay) &&
    rtb_Bs_test_mode_stt);
  R_S_FlipFlopMDLOBJ9.step(&rtb_AND_HeatMode_set, &rtb_AND_HeatMode_rst,
    &TFLOW4_Ctrl_DW.stt_k);
  if (TFLOW4_Ctrl_DW.is_active_c3_TFLOW4_Ctrl == 0) {
    TFLOW4_Ctrl_DW.is_active_c3_TFLOW4_Ctrl = 1U;
    TFLOW4_Ctrl_DW.is_c3_TFLOW4_Ctrl = TFLOW4_Ctrl_IN_SystemMode;
    TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_ModeSelection;
    if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
        tech_FullElec) {
      TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
      TFL_enter_internal_HeatPumpUsed();
    }
  } else if (TFLOW4_Ctrl_DW.is_c3_TFLOW4_Ctrl == TFLOW4_Ctrl_IN_SystemMode) {
    if (TFLOW4_Ctrl_DW.stt_k) {
      if (TFLOW4_Ctrl_DW.is_SystemMode == TFLOW4_Ctrl_IN_Holidays) {
        TFLOW4_Ctrl_DW.Bs_clk_on = false;
        TFLOW4_Ctrl_DW.is_SystemMode = 0;
      } else {
        TFLOW4_Ctrl_DW.is_AntiLegionella = 0;
        TFLOW4_Ctrl_DW.is_HeatPumpMode = 0;
        TFLOW4_Ctrl_DW.is_UsualMode = 0;
        TFLOW4_Ctrl_DW.is_HeatPumpUsed = 0;
        TFLOW4_Ctrl_DW.is_ModeSelection = 0;
        TFLOW4_Ctrl_DW.is_SmartGrid = 0;
        TFLOW4_Ctrl_DW.is_SystemMode = 0;
      }

      TFLOW4_Ctrl_DW.is_c3_TFLOW4_Ctrl = TFLOW4_Ctrl_IN_TestMode;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::HeatPumpTest;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
    } else {
      switch (TFLOW4_Ctrl_DW.is_SystemMode) {
       case TFLOW4_Ctrl_IN_AntiLegionella:
        if (!TFLOW4_Ctrl_DW.AntiLegionnellaModeActDelay_DST) {
          TFLOW4_Ctrl_DW.is_AntiLegionella = 0;
          TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_ModeSelection;
          if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
              tech_FullElec) {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
            TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
            TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
            TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
            TFL_enter_internal_HeatPumpUsed();
          }
        } else if (TFLOW4_Ctrl_DW.is_AntiLegionella ==
                   TFLO_IN_AntiLegionella_fullElec) {
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
        } else {
          // case IN_AntiLegionella_normal:
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
        }
        break;

       case TFLOW4_Ctrl_IN_Holidays:
        if (!TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE) {
          TFLOW4_Ctrl_DW.Bs_clk_on = false;
          TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_ModeSelection;
          if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
              tech_FullElec) {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
            TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
            TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
            TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
            TFL_enter_internal_HeatPumpUsed();
          }
        } else {
          TFLOW4_Ctrl_DW.Bs_clk_on = true;
          TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
          TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::off;
        }
        break;

       case TFLOW4_Ctrl_IN_ModeSelection:
        TFLOW4_Ctrl_ModeSelection(&TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE,
          &TFLOW4_Ctrl_DW.SmartGridActDelay_DSTATE,
          &TFLOW4_Ctrl_DW.AntiLegionnellaModeActDelay_DST);
        break;

       default:
        // case IN_SmartGrid:
        if (!TFLOW4_Ctrl_DW.SmartGridActDelay_DSTATE) {
          TFLOW4_Ctrl_DW.is_SmartGrid = 0;
          TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_ModeSelection;
          if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
              tech_FullElec) {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
            TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
            TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
            TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
            TFL_enter_internal_HeatPumpUsed();
          }
        } else if (TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE) {
          TFLOW4_Ctrl_DW.is_SmartGrid = 0;
          TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_Holidays;
          TFLOW4_Ctrl_DW.Bs_clk_on = false;
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::Holidays;
        } else {
          TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::SmartGrid;
          if (TFLOW4_Ctrl_DW.is_SmartGrid == TFLOW4_Ct_IN_SmartGrid_fullElec) {
            TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
            TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          } else {
            // case IN_SmartGrid_normal:
            TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::on;
            TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
          }
        }
        break;
      }
    }

    // case IN_TestMode:
  } else if (!TFLOW4_Ctrl_DW.stt_k) {
    TFLOW4_Ctrl_DW.is_c3_TFLOW4_Ctrl = TFLOW4_Ctrl_IN_SystemMode;
    TFLOW4_Ctrl_DW.is_SystemMode = TFLOW4_Ctrl_IN_ModeSelection;
    if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode == te_tech_mode::
        tech_FullElec) {
      TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_FullElec;
      TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
      TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::off;
      TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::on;
    } else {
      TFLOW4_Ctrl_DW.is_ModeSelection = TFLOW4_Ctrl_IN_HeatPumpUsed;
      TFL_enter_internal_HeatPumpUsed();
    }
  } else {
    TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::HeatPumpTest;
    TFLOW4_Ctrl_DW.Ss_heat_pump_ena = te_on_off::forced;
    TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena = te_on_off::forced;
  }

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
    TFLOW4_Ctrl_DW.temporalCounter_i4 = static_cast<uint8_t>
      (TFLOW4_Ctrl_DW.temporalCounter_i4 + 1);
  }

  if (TFLOW4_Ctrl_DW.is_active_c2_TFLOW4_Ctrl == 0) {
    TFLOW4_Ctrl_DW.is_active_c2_TFLOW4_Ctrl = 1U;
    TFLOW4_Ctrl_DW.temporalCounter_i1 = 0U;
    TFLOW4_Ctrl_DW.temporalCounter_i4 = 0U;
    TFLOW4_Ctrl_DW.SysSchedulerMode = SysSchedulerModeType::Initialisation;
  } else if (TFLOW4_Ctrl_DW.SysSchedulerMode == SysSchedulerModeType::
             Initialisation) {
    if (TFLOW4_Ctrl_DW.temporalCounter_i4 >= 5) {
      TFLOW4_Ctrl_DW.temporalCounter_i2 = 0U;
      TFLOW4_Ctrl_DW.temporalCounter_i3 = 0U;
      TFLOW4_Ctrl_DW.SysSchedulerMode = SysSchedulerModeType::Normal;
    } else if (TFLOW4_Ctrl_DW.temporalCounter_i1 == 1) {
      SysConfigCalibMDLOBJ3.step(&TFLOW4_Ctrl_U.Control_In,
        &TFLOW4_Ctrl_DW.SysConfigCalib_j);
    }
  } else {
    // State Normal
    if (TFLOW4_Ctrl_DW.temporalCounter_i2 == TFLOW4_Ctr_WaterHeatCtrl_tick_C) {
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_op_mode =
        TFLOW4_Ctrl_DW.Ss_op_mode;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_pump_xhst_pres =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_pres;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_pump_evap_pres =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_pres;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_tank_down_temp =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_down_temp;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_tank_up_temp =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_up_temp;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_pump_xhst_temp =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_temp;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_pump_evap_temp =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_temp;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_vent_temp =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_temp;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_heat_pump_ena =
        TFLOW4_Ctrl_DW.Ss_heat_pump_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_elec_bstr_htr_ena =
        TFLOW4_Ctrl_DW.Ss_elec_bstr_htr_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_v40_min =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_v40_min;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ns_pers_nb =
        TFLOW4_Ctrl_U.Control_In.HMI.USER.Ns_pers_nb;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_tank_size =
        TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tank_size;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_heat_mode_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Ss_heat_mode_simu;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_v40_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_v40_sp_simu;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_temp_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_temp_sp_simu;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Cs_heat_pump_rot_spd_sp_simu
        = TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_heat_pump_rot_spd_sp_simu;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Ss_elec_bstr_htr_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Ss_elec_bstr_htr_sp_simu;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Bs_heat_mode_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_heat_mode_simu_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Bs_v40_sp_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_v40_sp_simu_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Bs_temp_sp_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_temp_sp_simu_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Bs_heat_pump_freq_sp_simu_ena
        = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_heat_pump_freq_sp_simu_ena;
      TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water.Bs_elec_bstr_htr_sp_simu_ena
        = TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_elec_bstr_htr_sp_simu_ena;
      WaterHeatCtrlMDLOBJ16.step(&TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water,
        &TFLOW4_Ctrl_DW.WaterHeatDrv_Out);
    }

    if (TFLOW4_Ctrl_DW.temporalCounter_i3 == TFLOW4_Ctrl_VentCtrl_tick_C) {
      if (TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_sys_ver ==
          TFLOW4_Ctrl_P.Collective_Value) {
        rtb_BusConversion_InsertedFor_V.Ss_vent_op_mode =
          TFLOW4_Ctrl_P.Inoperative_Value;
      } else if (TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_vent_sys_stop_stt ==
                 TFLOW4_Ctrl_P.on_Value) {
        rtb_BusConversion_InsertedFor_V.Ss_vent_op_mode =
          TFLOW4_Ctrl_P.Stopped_Value;
      } else if (TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_pres_err) {
        rtb_BusConversion_InsertedFor_V.Ss_vent_op_mode =
          TFLOW4_Ctrl_P.ConstantFlow_Value;
      } else {
        rtb_BusConversion_InsertedFor_V.Ss_vent_op_mode =
          TFLOW4_Ctrl_P.Normal_Value;
      }

      rtb_BusConversion_InsertedFor_V.Cs_vent_pres =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_pres;
      rtb_BusConversion_InsertedFor_V.Cs_vent_pres_min =
        TFLOW4_Ctrl_U.Control_In.HMI.TECH.Cs_vent_pres_min;
      rtb_BusConversion_InsertedFor_V.Cs_vent_pres_sys =
        TFLOW4_Ctrl_U.Control_In.HMI.TECH.Cs_vent_pres_sys;
      rtb_BusConversion_InsertedFor_V.Bs_vent_pres_err =
        TFLOW4_Ctrl_DW.InPutSecu_Out.Bs_vent_pres_err;
      rtb_BusConversion_InsertedFor_V.Cs_vent_pres_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_pres_sp_simu;
      rtb_BusConversion_InsertedFor_V.Cs_vent_flow_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_flow_sp_simu;
      rtb_BusConversion_InsertedFor_V.Cs_vent_vltg_sp_simu =
        TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_vltg_sp_simu;
      rtb_BusConversion_InsertedFor_V.Bs_vent_pres_sp_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_vltg_sp_simu_ena;
      rtb_BusConversion_InsertedFor_V.Bs_vent_flow_sp_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_flow_sp_simu_ena;
      rtb_BusConversion_InsertedFor_V.Bs_vent_vltg_sp_simu_ena =
        TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_sp_simu_ena;
      VentCtrlMDLOBJ15.step(&rtb_BusConversion_InsertedFor_V,
                            &TFLOW4_Ctrl_DW.VentCtrl_Out);
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

  rtb_bool2uint[0] = TFLOW4_Ctrl_DW.Bs_clk_on;
  rtb_bool2uint[1] = static_cast<uint8_t>(!TFLOW4_Ctrl_DW.Bs_clk_on);
  TimerMDLOBJ14.step(&rtb_bool2uint[0], &rtb_bool2uint[1], &rtb_bool2uint[1],
                     &rtb_Timer_o1, &rtb_TimerState,
                     TFLOW4_Ctrl_P.secTimer_sampleTime);
  rtb_Bs_anti_lgn_set = ((TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_anti_lgn_ena ==
    TFLOW4_Ctrl_P.Constant_Value) && (static_cast<ta_time_day>(rtb_Timer_o1) >=
    TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ns_anti_lgn_day) &&
    ((TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_oph_stt == TFLOW4_Ctrl_P.Constant1_Value) ||
     (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_oph_mode_ena ==
      TFLOW4_Ctrl_P.Constant2_Value)));
  rtb_Bs_anti_lgn_rst = (((TFLOW4_Ctrl_DW.Ss_heat_pump_stt ==
    TFLOW4_Ctrl_P.Constant3_Value) || (TFLOW4_Ctrl_DW.Ss_heat_pump_stt ==
    TFLOW4_Ctrl_P.Constant5_Value)) && ((TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt ==
    TFLOW4_Ctrl_P.Constant4_Value_j) || (TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt ==
    TFLOW4_Ctrl_P.Constant6_Value_p)) && (TFLOW4_Ctrl_DW.opModeDelay_DSTATE ==
    TFLOW4_Ctrl_P.Constant7_Value_c));
  R_S_FlipFlopMDLOBJ6.step(&rtb_Bs_anti_lgn_set, &rtb_Bs_anti_lgn_rst,
    &TFLOW4_Ctrl_DW.AntiLegionnellaModeActDelay_DST);
  rtb_Bs_hldy_mode = (TFLOW4_Ctrl_DW.opModeDelay_DSTATE ==
                      TFLOW4_Ctrl_P.Constant5_Value_p);
  rtb_Bs_clk_rch_uint = (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_hldy_rqst ==
    TFLOW4_Ctrl_P.Constant4_Value_m);
  clkRchRisingEdgeMDLOBJ12.step(&rtb_Bs_clk_rch_uint, &HolidaysModeActDelay);
  rtb_Bs_mode_chk_hldy_rqst_on_ui =
    (TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_hldy_rqst ==
     TFLOW4_Ctrl_P.Constant1_Value_p);
  hldyRqstRisingEdgeMDLOBJ13.step(&rtb_Bs_mode_chk_hldy_rqst_on_ui,
    &SmartGridActDelay);
  rtb_Bs_mode_chk_hldy_rqst_on_g =
    ((TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_tech_mode !=
      TFLOW4_Ctrl_P.Constant_Value_d) && (!rtb_Bs_hldy_mode) &&
     SmartGridActDelay);
  rtb_OR_Holidays_rst = ((static_cast<uint8_t>(static_cast<ta_time_day>
    (rtb_Timer_o1) + TFLOW4_Ctrl_P.OneMoreDay_Bias) >=
    TFLOW4_Ctrl_U.Control_In.HMI.USER.Ns_hldy_nb) || (rtb_Bs_hldy_mode &&
    HolidaysModeActDelay));
  R_S_FlipFlopMDLOBJ11.step(&rtb_Bs_mode_chk_hldy_rqst_on_g,
    &rtb_OR_Holidays_rst, &TFLOW4_Ctrl_DW.HolidaysModeActDelay_DSTATE);
  TFLOW4_Ctrl_DW.SmartGridActDelay_DSTATE =
    (TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_sg_stt == TFLOW4_Ctrl_P.Constant5_Value_h);
  TFLOW4_Ctrl_DW.opModeDelay_DSTATE = TFLOW4_Ctrl_DW.Ss_op_mode;
}

// Model step function
void TFLOW4_Ctrl::step()
{
  tb_InPutMng_In rtb_BusConversion_InsertedFor_I;
  tb_OutPutMng_In rtb_BusConversion_InsertedFor_O;
  rtb_BusConversion_InsertedFor_I.Ss_ctry =
    TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_ctry;
  rtb_BusConversion_InsertedFor_I.Ss_sys_ver =
    TFLOW4_Ctrl_U.Control_In.HMI.TECH.Ss_sys_ver;
  rtb_BusConversion_InsertedFor_I.Cs_tank_vol_up =
    TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE.Cs_tank_vol_up;
  rtb_BusConversion_InsertedFor_I.Cs_tank_vol_btwn =
    TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE.Cs_tank_vol_btwn;
  rtb_BusConversion_InsertedFor_I.Cs_tank_vol_down =
    TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE.Cs_tank_vol_down;
  rtb_BusConversion_InsertedFor_I.Cs_tank_vol_tot =
    TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE.Cs_tank_vol_tot;
  rtb_BusConversion_InsertedFor_I.Cs_tank_down_temp_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_tank_down_temp_raw;
  rtb_BusConversion_InsertedFor_I.Cs_tank_up_temp_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_tank_up_temp_raw;
  rtb_BusConversion_InsertedFor_I.Cs_pump_xhst_temp_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_pump_xhst_temp_raw;
  rtb_BusConversion_InsertedFor_I.Cs_pump_evap_temp_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_pump_evap_temp_raw;
  rtb_BusConversion_InsertedFor_I.Cs_vent_temp_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_temp_raw;
  rtb_BusConversion_InsertedFor_I.Cs_vent_pres_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_pres_raw;
  rtb_BusConversion_InsertedFor_I.Cs_vent_rot_spd_raw =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_vent_rot_spd_raw;
  rtb_BusConversion_InsertedFor_I.Ss_heat_wtr_cnsp_rst =
    TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_heat_wtr_cnsp_rst;
  rtb_BusConversion_InsertedFor_I.Ss_vent_cnsp_rst =
    TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_vent_cnsp_rst;
  rtb_BusConversion_InsertedFor_I.Ss_tot_cnsp_rst =
    TFLOW4_Ctrl_U.Control_In.HMI.USER.Ss_tot_cnsp_rst;
  rtb_BusConversion_InsertedFor_I.St_tor_mode[0] =
    TFLOW4_Ctrl_U.Control_In.HMI.TECH.St_tor_mode[0];
  rtb_BusConversion_InsertedFor_I.St_tor_stt_raw[0] =
    TFLOW4_Ctrl_U.Control_In.HW.St_tor_stt_raw[0];
  rtb_BusConversion_InsertedFor_I.St_tor_mode[1] =
    TFLOW4_Ctrl_U.Control_In.HMI.TECH.St_tor_mode[1];
  rtb_BusConversion_InsertedFor_I.St_tor_stt_raw[1] =
    TFLOW4_Ctrl_U.Control_In.HW.St_tor_stt_raw[1];
  rtb_BusConversion_InsertedFor_I.Bs_err_dtct_shnt_ena =
    TFLOW4_Ctrl_U.Control_In.HMI.EXPERT.Bs_err_dtct_shnt_ena;
  rtb_BusConversion_InsertedFor_I.Bs_hw_fan_err =
    TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_fan_err;
  rtb_BusConversion_InsertedFor_I.Bs_hw_pres_err =
    TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_pres_err;
  rtb_BusConversion_InsertedFor_I.Bs_hw_anod_err =
    TFLOW4_Ctrl_U.Control_In.ERR.Bs_hw_anod_err;
  rtb_BusConversion_InsertedFor_I.Ss_heat_pump_stt =
    TFLOW4_Ctrl_DW.WaterHeatCtrl_Out_delay_DSTATE.Ss_heat_pump_stt;
  rtb_BusConversion_InsertedFor_I.Ss_elec_htr_bstr_stt =
    TFLOW4_Ctrl_DW.WaterHeatCtrl_Out_delay_DSTATE.Ss_elec_htr_bstr_stt;
  rtb_BusConversion_InsertedFor_I.Cs_heat_pump_pwr =
    TFLOW4_Ctrl_U.Control_In.HW.Cs_heat_pump_pwr;
  rtb_BusConversion_InsertedFor_I.Cs_tank_down_temp_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_tank_down_temp_simu;
  rtb_BusConversion_InsertedFor_I.Cs_tank_up_temp_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_tank_up_temp_simu;
  rtb_BusConversion_InsertedFor_I.Cs_pump_xhst_temp_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_xhst_temp_simu;
  rtb_BusConversion_InsertedFor_I.Cs_pump_evap_temp_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_evap_temp_simu;
  rtb_BusConversion_InsertedFor_I.Cs_vent_temp_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_temp_simu;
  rtb_BusConversion_InsertedFor_I.Cs_vent_pres_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_pres_simu;
  rtb_BusConversion_InsertedFor_I.Cs_pump_xhst_pres_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_xhst_pres_simu;
  rtb_BusConversion_InsertedFor_I.Cs_pump_evap_pres_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_pump_evap_pres_simu;
  rtb_BusConversion_InsertedFor_I.Cs_v40_min_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_v40_min_simu;
  rtb_BusConversion_InsertedFor_I.Cs_vent_rot_spd_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Cs_vent_rot_spd_simu;
  rtb_BusConversion_InsertedFor_I.Bs_tank_down_temp_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_tank_down_temp_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_tank_up_temp_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_tank_up_temp_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_pump_xhst_temp_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_pump_xhst_temp_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_pump_evap_temp_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_pump_evap_temp_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_vent_temp_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_vent_temp_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_vent_pres_err_simu =
    TFLOW4_Ctrl_U.Control_In.SIMU.VAR.Bs_vent_pres_err_simu;
  rtb_BusConversion_InsertedFor_I.Bs_tank_down_temp_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_down_temp_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_tank_up_temp_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_up_temp_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_xhst_temp_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_temp_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_evap_temp_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_temp_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_vent_temp_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_temp_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_vent_pres_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_tank_down_temp_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_down_temp_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_tank_up_temp_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_tank_up_temp_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_xhst_temp_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_temp_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_evap_temp_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_temp_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_vent_temp_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_temp_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_vent_pres_err_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_pres_err_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_xhst_pres_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_xhst_pres_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_pump_evap_pres_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_pump_evap_pres_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_v40_min_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_v40_min_simu_ena;
  rtb_BusConversion_InsertedFor_I.Bs_vent_rot_spd_simu_ena =
    TFLOW4_Ctrl_U.Control_In.SIMU.ENA.Bs_vent_rot_spd_simu_ena;
  InPutMngMDLOBJ1.step(&rtb_BusConversion_InsertedFor_I,
                       &TFLOW4_Ctrl_DW.InPutSecu_Out);
  TFLOW4_Ctrl_DW.Ss_heat_pump_stt = TFLOW4_Ctrl_DW.Ss_heat_pump_stt_DSTATE;
  TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt =
    TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt_DSTATE;
  TFLOW4_Ctrl_SysMng();
  rtb_BusConversion_InsertedFor_O.Cs_v40_rat =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_v40_rat;
  rtb_BusConversion_InsertedFor_O.Ss_op_mode = TFLOW4_Ctrl_DW.Ss_op_mode;
  rtb_BusConversion_InsertedFor_O.Ss_sg_stt =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_sg_stt;
  rtb_BusConversion_InsertedFor_O.Ss_oph_stt =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Ss_oph_stt;
  rtb_BusConversion_InsertedFor_O.Cs_tank_down_temp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_down_temp;
  rtb_BusConversion_InsertedFor_O.Cs_tank_up_temp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tank_up_temp;
  rtb_BusConversion_InsertedFor_O.Cs_pump_xhst_temp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_xhst_temp;
  rtb_BusConversion_InsertedFor_O.Cs_pump_evap_temp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_pump_evap_temp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_temp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_temp;
  rtb_BusConversion_InsertedFor_O.Ss_elec_bstr_htr_sp =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_elec_bstr_htr_sp;
  rtb_BusConversion_InsertedFor_O.Cs_heat_pump_rot_spd_sp =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_heat_pump_rot_spd_sp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_rot_spd =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_rot_spd;
  rtb_BusConversion_InsertedFor_O.Cs_vent_cnsp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_cnsp;
  rtb_BusConversion_InsertedFor_O.Cs_heat_wtr_cnsp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_heat_wtr_cnsp;
  rtb_BusConversion_InsertedFor_O.Cs_tot_cnsp =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_tot_cnsp;
  rtb_BusConversion_InsertedFor_O.Cs_v40_sp =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_v40_sp;
  rtb_BusConversion_InsertedFor_O.Cs_temp_sp =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Cs_temp_sp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_pres_sp =
    TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_pres_sp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_flow_sp =
    TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_flow_sp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_vltg_sp =
    TFLOW4_Ctrl_DW.VentCtrl_Out.Cs_vent_vltg_sp;
  rtb_BusConversion_InsertedFor_O.Cs_vent_pres =
    TFLOW4_Ctrl_DW.InPutSecu_Out.Cs_vent_pres;
  OutPutMngMDLOBJ2.step(&rtb_BusConversion_InsertedFor_O,
                        &TFLOW4_Ctrl_Y.Control_Out);
  TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE =
    TFLOW4_Ctrl_DW.SysConfigCalib_j;
  TFLOW4_Ctrl_DW.WaterHeatCtrl_Out_delay_DSTATE =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out;
  TFLOW4_Ctrl_DW.Ss_heat_pump_stt_DSTATE =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_heat_pump_stt;
  TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt_DSTATE =
    TFLOW4_Ctrl_DW.WaterHeatDrv_Out.Ss_elec_htr_bstr_stt;
}

// Model initialize function
void TFLOW4_Ctrl::initialize()
{
  // Registration code

  // states (dwork)
  {
    TFLOW4_Ctrl_DW.BusConversion_InsertedFor_Water =
      TFLOW4_Ctrl_rtZtb_WaterHeatCtrl_In;
    TFLOW4_Ctrl_DW.Ss_op_mode = te_op_mode::FullElec;
  }

  // external inputs
  TFLOW4_Ctrl_U.Control_In = TFLOW4_Ctrl_rtZtb_Control_In;

  // external outputs
  TFLOW4_Ctrl_Y.Control_Out = TFLOW4_Ctrl_rtZtb_Control_Out;
  TFLOW4_Ctrl_DW.SysConfigCalib_Out_delay_DSTATE =
    TFLOW4_Ctrl_P.SysConfigCalib_Out_delay_Initia;
  TFLOW4_Ctrl_DW.WaterHeatCtrl_Out_delay_DSTATE =
    TFLOW4_Ctrl_P.WaterHeatCtrl_Out_delay_Initial;
  TFLOW4_Ctrl_DW.Ss_heat_pump_stt_DSTATE =
    TFLOW4_Ctrl_P.Ss_heat_pump_stt_InitialConditi;
  TFLOW4_Ctrl_DW.Ss_elec_htr_bstr_stt_DSTATE =
    TFLOW4_Ctrl_P.Ss_elec_htr_bstr_stt_InitialCon;
  InPutMngMDLOBJ1.init();
  TFLOW4_Ctrl_SysMng_Init();
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
