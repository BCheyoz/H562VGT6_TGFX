//
// File: SysConfigCalib_types.h
//
// Code generated for Simulink model 'SysConfigCalib'.
//
// Model version                  : 1.23
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:03:01 2025
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
#ifndef SysConfigCalib_types_h_
#define SysConfigCalib_types_h_
#include <stdint.h>
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
#endif                                 // SysConfigCalib_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
