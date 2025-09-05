//
// File: InPutMng_types.h
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.337
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Sep  4 18:37:32 2025
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
#ifndef InPutMng_types_h_
#define InPutMng_types_h_
#include <stdint.h>
#include <stdbool.h>
#ifndef DEFINED_TYPEDEF_FOR_ta_air_pres_
#define DEFINED_TYPEDEF_FOR_ta_air_pres_

using ta_air_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rot_spd_
#define DEFINED_TYPEDEF_FOR_ta_rot_spd_

using ta_rot_spd = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_temp_
#define DEFINED_TYPEDEF_FOR_ta_temp_

using ta_temp = int16_t;

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

#ifndef DEFINED_TYPEDEF_FOR_te_sys_ver_
#define DEFINED_TYPEDEF_FOR_te_sys_ver_

enum class te_sys_ver
  : int32_t {
  Collective = 0,                      // Default value
  Individual
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_wtr_vol_
#define DEFINED_TYPEDEF_FOR_ta_wtr_vol_

using ta_wtr_vol = uint16_t;

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

#ifndef DEFINED_TYPEDEF_FOR_ta_pwr_
#define DEFINED_TYPEDEF_FOR_ta_pwr_

using ta_pwr = uint32_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rfrg_pres_
#define DEFINED_TYPEDEF_FOR_ta_rfrg_pres_

using ta_rfrg_pres = uint16_t;

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

#ifndef DEFINED_TYPEDEF_FOR_ta_egy_
#define DEFINED_TYPEDEF_FOR_ta_egy_

using ta_egy = uint32_t;

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
#endif                                 // InPutMng_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
