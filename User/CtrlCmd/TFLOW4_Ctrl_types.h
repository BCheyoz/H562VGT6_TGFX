//
// File: TFLOW4_Ctrl_types.h
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
#ifndef TFLOW4_Ctrl_types_h_
#define TFLOW4_Ctrl_types_h_
#include <stdbool.h>
#include <stdint.h>
#ifndef DEFINED_TYPEDEF_FOR_tb_hmi_expert_
#define DEFINED_TYPEDEF_FOR_tb_hmi_expert_

struct tb_hmi_expert
{
  bool Bs_err_dtct_shnt_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_time_day_
#define DEFINED_TYPEDEF_FOR_ta_time_day_

using ta_time_day = uint8_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_pers_nb_
#define DEFINED_TYPEDEF_FOR_ta_pers_nb_

using ta_pers_nb = uint8_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_user_mode_
#define DEFINED_TYPEDEF_FOR_te_user_mode_

enum class te_user_mode
  : int32_t {
  Auto = 10,                           // Default value
  Eco = 20,
  Boost = 30,
  Holidays = 40
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_on_off_
#define DEFINED_TYPEDEF_FOR_te_on_off_

enum class te_on_off
  : int32_t {
  off = 0,                             // Default value
  on,
  forced
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_hmi_user_
#define DEFINED_TYPEDEF_FOR_tb_hmi_user_

struct tb_hmi_user
{
  ta_time_day Ns_hldy_nb;
  ta_pers_nb Ns_pers_nb;
  te_user_mode Ss_user_mode;
  te_on_off Ss_anti_lgn_ena;
  te_on_off Ss_heat_wtr_cnsp_rst;
  te_on_off Ss_vent_cnsp_rst;
  te_on_off Ss_tot_cnsp_rst;
  te_on_off Ss_sg_mode_ena;
  te_on_off Ss_oph_mode_ena;
  te_on_off Ss_hldy_rqst;
  te_on_off Ss_bst_rqst;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_ctry_
#define DEFINED_TYPEDEF_FOR_te_ctry_

enum class te_ctry
  : int32_t {
  France = 33,                         // Default value
  Germany = 49,
  Spain = 34,
  Marocco = 212,
  WesternSahara = 213
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_tech_mode_
#define DEFINED_TYPEDEF_FOR_te_tech_mode_

enum class te_tech_mode
  : int32_t {
  FullElec = 0,
  Hybrid,
  HeatPump                             // Default value
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_tank_size_
#define DEFINED_TYPEDEF_FOR_te_tank_size_

enum class te_tank_size
  : int32_t {
  L105 = 105,                          // Default value
  L180 = 180
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_sys_ver_
#define DEFINED_TYPEDEF_FOR_te_sys_ver_

enum class te_sys_ver
  : int32_t {
  Collective = 0,                      // Default value
  Individual
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_air_pres_
#define DEFINED_TYPEDEF_FOR_ta_air_pres_

using ta_air_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_tor_mode_
#define DEFINED_TYPEDEF_FOR_te_tor_mode_

enum class te_tor_mode
  : int32_t {
  NotConnected = 0,                    // Default value
  SmartGrid,
  OffPeakHour,
  VentSysStop
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_hmi_tech_
#define DEFINED_TYPEDEF_FOR_tb_hmi_tech_

struct tb_hmi_tech
{
  te_ctry Ss_ctry;
  te_tech_mode Ss_tech_mode;
  ta_time_day Ns_anti_lgn_day;
  te_on_off Ss_heat_pump_test_rqst;
  te_tank_size Ss_tank_size;
  te_sys_ver Ss_sys_ver;
  ta_air_pres Cs_vent_pres_min;
  ta_air_pres Cs_vent_pres_sys;
  te_tor_mode St_tor_mode[2];
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_hmi_
#define DEFINED_TYPEDEF_FOR_tb_hmi_

struct tb_hmi
{
  tb_hmi_expert EXPERT;
  tb_hmi_user USER;
  tb_hmi_tech TECH;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_temp_
#define DEFINED_TYPEDEF_FOR_ta_temp_

using ta_temp = int16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rot_spd_
#define DEFINED_TYPEDEF_FOR_ta_rot_spd_

using ta_rot_spd = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_pwr_
#define DEFINED_TYPEDEF_FOR_ta_pwr_

using ta_pwr = uint32_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_hw_
#define DEFINED_TYPEDEF_FOR_tb_hw_

struct tb_hw
{
  ta_temp Cs_tank_down_temp_raw;
  ta_temp Cs_tank_up_temp_raw;
  ta_temp Cs_pump_xhst_temp_raw;
  ta_temp Cs_pump_evap_temp_raw;
  ta_temp Cs_vent_temp_raw;
  ta_air_pres Cs_vent_pres_raw;
  ta_rot_spd Cs_vent_rot_spd_raw;
  te_on_off St_tor_stt_raw[2];
  ta_pwr Cs_heat_pump_pwr;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_err_
#define DEFINED_TYPEDEF_FOR_tb_err_

struct tb_err
{
  bool Bs_hw_fan_err;
  bool Bs_hw_pres_err;
  bool Bs_hw_anod_err;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_heat_mode_
#define DEFINED_TYPEDEF_FOR_te_heat_mode_

enum class te_heat_mode
  : int32_t {
  v40_ctrl = 0,                        // Default value
  temp_ctrl
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_wtr_vol_
#define DEFINED_TYPEDEF_FOR_ta_wtr_vol_

using ta_wtr_vol = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_flow_
#define DEFINED_TYPEDEF_FOR_ta_flow_

using ta_flow = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_vltg_
#define DEFINED_TYPEDEF_FOR_ta_vltg_

using ta_vltg = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rfrg_pres_
#define DEFINED_TYPEDEF_FOR_ta_rfrg_pres_

using ta_rfrg_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_simu_var_
#define DEFINED_TYPEDEF_FOR_tb_simu_var_

struct tb_simu_var
{
  te_heat_mode Ss_heat_mode_simu;
  ta_wtr_vol Cs_v40_sp_simu;
  ta_temp Cs_temp_sp_simu;
  ta_rot_spd Cs_heat_pump_rot_spd_sp_simu;
  te_on_off Ss_elec_bstr_htr_sp_simu;
  ta_air_pres Cs_vent_pres_sp_simu;
  ta_flow Cs_vent_flow_sp_simu;
  ta_vltg Cs_vent_vltg_sp_simu;
  ta_temp Cs_tank_down_temp_simu;
  ta_temp Cs_tank_up_temp_simu;
  ta_temp Cs_pump_xhst_temp_simu;
  ta_temp Cs_pump_evap_temp_simu;
  ta_temp Cs_vent_temp_simu;
  ta_air_pres Cs_vent_pres_simu;
  bool Bs_tank_down_temp_err_simu;
  bool Bs_tank_up_temp_err_simu;
  bool Bs_pump_xhst_temp_err_simu;
  bool Bs_pump_evap_temp_err_simu;
  bool Bs_vent_temp_err_simu;
  bool Bs_vent_pres_err_simu;
  ta_rfrg_pres Cs_pump_xhst_pres_simu;
  ta_rfrg_pres Cs_pump_evap_pres_simu;
  ta_wtr_vol Cs_v40_min_simu;
  ta_rot_spd Cs_vent_rot_spd_simu;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_simu_ena_
#define DEFINED_TYPEDEF_FOR_tb_simu_ena_

struct tb_simu_ena
{
  bool Bs_heat_mode_simu_ena;
  bool Bs_v40_sp_simu_ena;
  bool Bs_temp_sp_simu_ena;
  bool Bs_heat_pump_freq_sp_simu_ena;
  bool Bs_elec_bstr_htr_sp_simu_ena;
  bool Bs_vent_pres_sp_simu_ena;
  bool Bs_vent_flow_sp_simu_ena;
  bool Bs_vent_vltg_sp_simu_ena;
  bool Bs_tank_down_temp_simu_ena;
  bool Bs_tank_up_temp_simu_ena;
  bool Bs_pump_xhst_temp_simu_ena;
  bool Bs_pump_evap_temp_simu_ena;
  bool Bs_vent_temp_simu_ena;
  bool Bs_vent_pres_simu_ena;
  bool Bs_tank_down_temp_err_simu_ena;
  bool Bs_tank_up_temp_err_simu_ena;
  bool Bs_pump_xhst_temp_err_simu_ena;
  bool Bs_pump_evap_temp_err_simu_ena;
  bool Bs_vent_temp_err_simu_ena;
  bool Bs_vent_pres_err_simu_ena;
  bool Bs_pump_xhst_pres_simu_ena;
  bool Bs_pump_evap_pres_simu_ena;
  bool Bs_v40_min_simu_ena;
  bool Bs_vent_rot_spd_simu_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_simu_
#define DEFINED_TYPEDEF_FOR_tb_simu_

struct tb_simu
{
  tb_simu_var VAR;
  tb_simu_ena ENA;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_Control_In_
#define DEFINED_TYPEDEF_FOR_tb_Control_In_

struct tb_Control_In
{
  tb_hmi HMI;
  tb_hw HW;
  tb_err ERR;
  tb_simu SIMU;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_heat_stt_
#define DEFINED_TYPEDEF_FOR_te_heat_stt_

enum class te_heat_stt
  : int32_t {
  Stopped = 0,                         // Default value
  Waiting,
  WillHeat,
  Heating
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_InPutMng_In_
#define DEFINED_TYPEDEF_FOR_tb_InPutMng_In_

struct tb_InPutMng_In
{
  te_ctry Ss_ctry;
  te_sys_ver Ss_sys_ver;
  ta_wtr_vol Cs_tank_vol_up;
  ta_wtr_vol Cs_tank_vol_btwn;
  ta_wtr_vol Cs_tank_vol_down;
  ta_wtr_vol Cs_tank_vol_tot;
  ta_temp Cs_tank_down_temp_raw;
  ta_temp Cs_tank_up_temp_raw;
  ta_temp Cs_pump_xhst_temp_raw;
  ta_temp Cs_pump_evap_temp_raw;
  ta_temp Cs_vent_temp_raw;
  ta_air_pres Cs_vent_pres_raw;
  ta_rot_spd Cs_vent_rot_spd_raw;
  te_on_off Ss_heat_wtr_cnsp_rst;
  te_on_off Ss_vent_cnsp_rst;
  te_on_off Ss_tot_cnsp_rst;
  te_tor_mode St_tor_mode[2];
  te_on_off St_tor_stt_raw[2];
  bool Bs_err_dtct_shnt_ena;
  bool Bs_hw_fan_err;
  bool Bs_hw_pres_err;
  bool Bs_hw_anod_err;
  te_heat_stt Ss_heat_pump_stt;
  te_heat_stt Ss_elec_htr_bstr_stt;
  ta_pwr Cs_heat_pump_pwr;
  ta_temp Cs_tank_down_temp_simu;
  ta_temp Cs_tank_up_temp_simu;
  ta_temp Cs_pump_xhst_temp_simu;
  ta_temp Cs_pump_evap_temp_simu;
  ta_temp Cs_vent_temp_simu;
  ta_air_pres Cs_vent_pres_simu;
  ta_rfrg_pres Cs_pump_xhst_pres_simu;
  ta_rfrg_pres Cs_pump_evap_pres_simu;
  ta_wtr_vol Cs_v40_min_simu;
  ta_rot_spd Cs_vent_rot_spd_simu;
  bool Bs_tank_down_temp_err_simu;
  bool Bs_tank_up_temp_err_simu;
  bool Bs_pump_xhst_temp_err_simu;
  bool Bs_pump_evap_temp_err_simu;
  bool Bs_vent_temp_err_simu;
  bool Bs_vent_pres_err_simu;
  bool Bs_tank_down_temp_simu_ena;
  bool Bs_tank_up_temp_simu_ena;
  bool Bs_pump_xhst_temp_simu_ena;
  bool Bs_pump_evap_temp_simu_ena;
  bool Bs_vent_temp_simu_ena;
  bool Bs_vent_pres_simu_ena;
  bool Bs_tank_down_temp_err_simu_ena;
  bool Bs_tank_up_temp_err_simu_ena;
  bool Bs_pump_xhst_temp_err_simu_ena;
  bool Bs_pump_evap_temp_err_simu_ena;
  bool Bs_vent_temp_err_simu_ena;
  bool Bs_vent_pres_err_simu_ena;
  bool Bs_pump_xhst_pres_simu_ena;
  bool Bs_pump_evap_pres_simu_ena;
  bool Bs_v40_min_simu_ena;
  bool Bs_vent_rot_spd_simu_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_SysMng_In_
#define DEFINED_TYPEDEF_FOR_tb_SysMng_In_

struct tb_SysMng_In
{
  ta_time_day Ns_hldy_nb;
  ta_pers_nb Ns_pers_nb;
  te_user_mode Ss_user_mode;
  te_on_off Ss_anti_lgn_ena;
  te_ctry Ss_ctry;
  te_sys_ver Ss_sys_ver;
  te_tech_mode Ss_tech_mode;
  ta_time_day Ns_anti_lgn_day;
  te_on_off Ss_heat_pump_test_rqst;
  te_on_off Ss_hldy_rqst;
  te_on_off Ss_bst_rqst;
  te_on_off Ss_sg_stt;
  te_on_off Ss_oph_stt;
  te_on_off Ss_oph_mode_ena;
  te_on_off Ss_vent_sys_stop_stt;
  te_heat_stt Ss_heat_pump_stt;
  te_heat_stt Ss_elec_htr_bstr_stt;
  bool Bs_tank_down_temp_err;
  bool Bs_tank_up_temp_err;
  bool Bs_pump_xhst_temp_err;
  bool Bs_pump_evap_temp_err;
  bool Bs_vent_temp_err;
  bool Bs_vent_pres_err;
  bool Bs_vent_fan_err;
  bool Bs_tank_anod_err;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_vent_op_mode_
#define DEFINED_TYPEDEF_FOR_te_vent_op_mode_

enum class te_vent_op_mode
  : int32_t {
  Inoperative = 0,
  Stopped,
  ConstantFlow,
  Normal                               // Default value
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_VentCtrl_In_
#define DEFINED_TYPEDEF_FOR_tb_VentCtrl_In_

struct tb_VentCtrl_In
{
  te_vent_op_mode Ss_vent_op_mode;
  ta_air_pres Cs_vent_pres;
  ta_air_pres Cs_vent_pres_min;
  ta_air_pres Cs_vent_pres_sys;
  bool Bs_vent_pres_err;
  ta_air_pres Cs_vent_pres_sp_simu;
  ta_flow Cs_vent_flow_sp_simu;
  ta_vltg Cs_vent_vltg_sp_simu;
  bool Bs_vent_pres_sp_simu_ena;
  bool Bs_vent_flow_sp_simu_ena;
  bool Bs_vent_vltg_sp_simu_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_op_mode_
#define DEFINED_TYPEDEF_FOR_te_op_mode_

enum class te_op_mode
  : int32_t {
  HeatPumpTest = 0,
  AntiLegionella = 1,
  FullElec = 5,                        // Default value
  SmartGrid = 6,
  Hybrid = 9,
  Auto = 10,
  Eco = 20,
  Boost = 30,
  Holidays = 40
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_In_
#define DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_In_

struct tb_WaterHeatCtrl_In
{
  te_op_mode Ss_op_mode;
  ta_rfrg_pres Cs_pump_xhst_pres;
  ta_rfrg_pres Cs_pump_evap_pres;
  ta_temp Cs_tank_down_temp;
  ta_temp Cs_tank_up_temp;
  ta_temp Cs_pump_xhst_temp;
  ta_temp Cs_pump_evap_temp;
  ta_temp Cs_vent_temp;
  te_on_off Ss_heat_pump_ena;
  te_on_off Ss_elec_bstr_htr_ena;
  ta_pers_nb Ns_pers_nb;
  te_tank_size Ss_tank_size;
  ta_rot_spd Cs_heat_pump_rot_spd_sp_simu;
  te_on_off Ss_elec_bstr_htr_sp_simu;
  bool Bs_heat_pump_freq_sp_simu_ena;
  bool Bs_elec_bstr_htr_sp_simu_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_egy_
#define DEFINED_TYPEDEF_FOR_ta_egy_

using ta_egy = uint32_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_OutPutMng_In_
#define DEFINED_TYPEDEF_FOR_tb_OutPutMng_In_

struct tb_OutPutMng_In
{
  uint8_t Cs_v40_rat;
  te_op_mode Ss_op_mode;
  te_on_off Ss_sg_stt;
  te_on_off Ss_oph_stt;
  ta_temp Cs_tank_down_temp;
  ta_temp Cs_tank_up_temp;
  ta_temp Cs_pump_xhst_temp;
  ta_temp Cs_pump_evap_temp;
  ta_temp Cs_vent_temp;
  te_on_off Ss_elec_bstr_htr_sp;
  ta_rot_spd Cs_heat_pump_rot_spd_sp;
  ta_rot_spd Cs_vent_rot_spd;
  ta_egy Cs_vent_cnsp;
  ta_egy Cs_heat_wtr_cnsp;
  ta_egy Cs_tot_cnsp;
  ta_wtr_vol Cs_v40_sp;
  ta_temp Cs_temp_sp;
  ta_air_pres Cs_vent_pres_sp;
  ta_flow Cs_vent_flow_sp;
  ta_vltg Cs_vent_vltg_sp;
  ta_air_pres Cs_vent_pres;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_InPutMng_Out_
#define DEFINED_TYPEDEF_FOR_tb_InPutMng_Out_

struct tb_InPutMng_Out
{
  ta_rfrg_pres Cs_pump_xhst_pres;
  ta_rfrg_pres Cs_pump_evap_pres;
  ta_temp Cs_tank_down_temp;
  ta_temp Cs_tank_up_temp;
  ta_temp Cs_pump_xhst_temp;
  ta_temp Cs_pump_evap_temp;
  ta_temp Cs_vent_temp;
  ta_air_pres Cs_vent_pres;
  ta_wtr_vol Cs_v40_min;
  ta_wtr_vol Cs_v40_mean;
  ta_wtr_vol Cs_v40_max;
  ta_rot_spd Cs_vent_rot_spd;
  te_on_off Ss_sg_stt;
  te_on_off Ss_oph_stt;
  te_on_off Ss_vent_sys_stop_stt;
  ta_egy Cs_vent_cnsp;
  ta_egy Cs_heat_wtr_cnsp;
  ta_egy Cs_tot_cnsp;
  ta_pwr Cs_vent_pwr;
  ta_pwr Cs_heat_wtr_pwr;
  ta_pwr Cs_tot_pwr;
  bool Bs_tank_down_temp_err;
  bool Bs_tank_up_temp_err;
  bool Bs_pump_xhst_temp_err;
  bool Bs_pump_evap_temp_err;
  bool Bs_vent_temp_err;
  bool Bs_vent_pres_err;
  bool Bs_vent_fan_err;
  bool Bs_tank_anod_err;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_SysConfigCalib_Out_
#define DEFINED_TYPEDEF_FOR_tb_SysConfigCalib_Out_

struct tb_SysConfigCalib_Out
{
  ta_wtr_vol Cs_tank_vol_up;
  ta_wtr_vol Cs_tank_vol_btwn;
  ta_wtr_vol Cs_tank_vol_down;
  ta_wtr_vol Cs_tank_vol_tot;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_Out_
#define DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_Out_

struct tb_WaterHeatCtrl_Out
{
  ta_rot_spd Cs_heat_pump_rot_spd_sp;
  te_on_off Ss_elec_bstr_htr_sp;
  te_heat_stt Ss_heat_pump_stt;
  te_heat_stt Ss_elec_htr_bstr_stt;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_Control_Out_
#define DEFINED_TYPEDEF_FOR_tb_Control_Out_

struct tb_Control_Out
{
  uint8_t Cs_v40_lvl;
  te_op_mode Ss_op_mode;
  te_on_off Ss_sg_stt;
  te_on_off Ss_oph_stt;
  ta_temp Cs_tank_down_temp;
  ta_temp Cs_tank_up_temp;
  ta_temp Cs_pump_xhst_temp;
  ta_temp Cs_pump_evap_temp;
  ta_temp Cs_vent_temp;
  te_on_off Ss_elec_bstr_htr_sp;
  ta_rot_spd Cs_heat_pump_rot_spd_sp;
  ta_rot_spd Cs_vent_rot_spd;
  ta_egy Cs_vent_cnsp;
  ta_egy Cs_heat_wtr_cnsp;
  ta_egy Cs_tot_cnsp;
  ta_wtr_vol Cs_v40_sp;
  ta_temp Cs_temp_sp;
  ta_air_pres Cs_vent_pres_sp;
  ta_flow Cs_vent_flow_sp;
  ta_vltg Cs_vent_vltg_sp;
  ta_air_pres Cs_vent_pres;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_SysMng_Out_
#define DEFINED_TYPEDEF_FOR_tb_SysMng_Out_

struct tb_SysMng_Out
{
  te_op_mode Ss_op_mode;
  te_vent_op_mode Ss_vent_op_mode;
  te_on_off Ss_heat_pump_ena;
  te_on_off Ss_elec_bstr_htr_ena;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_tb_VentCtrl_Out_
#define DEFINED_TYPEDEF_FOR_tb_VentCtrl_Out_

struct tb_VentCtrl_Out
{
  ta_air_pres Cs_vent_pres_sp;
  ta_flow Cs_vent_flow_sp;
  ta_vltg Cs_vent_vltg_sp;
  ta_air_pres Cs_pres_mes_filt;
};

#endif
#endif                                 // TFLOW4_Ctrl_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
