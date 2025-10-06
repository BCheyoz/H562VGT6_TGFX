//
// File: SysMng_types.h
//
// Code generated for Simulink model 'SysMng'.
//
// Model version                  : 1.154
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Fri Sep  5 16:51:39 2025
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
#ifndef SysMng_types_h_
#define SysMng_types_h_
#include <stdint.h>
#include <stdbool.h>
#ifndef DEFINED_TYPEDEF_FOR_ta_time_min_
#define DEFINED_TYPEDEF_FOR_ta_time_min_

using ta_time_min = uint8_t;

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

#ifndef DEFINED_TYPEDEF_FOR_te_tech_mode_
#define DEFINED_TYPEDEF_FOR_te_tech_mode_

enum class te_tech_mode
  : int32_t {
  FullElec = 0,
  Hybrid,
  HeatPump                             // Default value
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
#endif                                 // SysMng_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
