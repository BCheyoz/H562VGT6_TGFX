//
// File: WaterHeatCtrl_types.h
//
// Code generated for Simulink model 'WaterHeatCtrl'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Wed Jun  4 11:34:40 2025
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
#ifndef WaterHeatCtrl_types_h_
#define WaterHeatCtrl_types_h_
#include <stdint.h>
#include <stdbool.h>
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

#ifndef DEFINED_TYPEDEF_FOR_ta_rfrg_pres_
#define DEFINED_TYPEDEF_FOR_ta_rfrg_pres_

using ta_rfrg_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_temp_
#define DEFINED_TYPEDEF_FOR_ta_temp_

using ta_temp = int16_t;

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

#ifndef DEFINED_TYPEDEF_FOR_ta_wtr_vol_
#define DEFINED_TYPEDEF_FOR_ta_wtr_vol_

using ta_wtr_vol = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_pers_nb_
#define DEFINED_TYPEDEF_FOR_ta_pers_nb_

using ta_pers_nb = uint8_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_tank_size_
#define DEFINED_TYPEDEF_FOR_te_tank_size_

enum class te_tank_size
  : int32_t {
  L105 = 105,                          // Default value
  L180 = 180
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

#ifndef DEFINED_TYPEDEF_FOR_ta_rot_spd_
#define DEFINED_TYPEDEF_FOR_ta_rot_spd_

using ta_rot_spd = uint16_t;

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
  ta_wtr_vol Cs_v40_min;
  ta_pers_nb Ns_pers_nb;
  te_tank_size Ss_tank_size;
  te_heat_mode Ss_heat_mode_simu;
  ta_wtr_vol Cs_v40_sp_simu;
  ta_temp Cs_temp_sp_simu;
  ta_rot_spd Cs_heat_pump_rot_spd_sp_simu;
  te_on_off Ss_elec_bstr_htr_sp_simu;
  bool Bs_heat_mode_simu_ena;
  bool Bs_v40_sp_simu_ena;
  bool Bs_temp_sp_simu_ena;
  bool Bs_heat_pump_freq_sp_simu_ena;
  bool Bs_elec_bstr_htr_sp_simu_ena;
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

#ifndef DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_Out_
#define DEFINED_TYPEDEF_FOR_tb_WaterHeatCtrl_Out_

struct tb_WaterHeatCtrl_Out
{
  te_heat_mode Ss_heat_mode;
  ta_wtr_vol Cs_v40_sp;
  ta_temp Cs_temp_sp;
  ta_rot_spd Cs_heat_pump_rot_spd_sp;
  te_on_off Ss_elec_bstr_htr_sp;
  uint8_t Cs_v40_rat;
  te_heat_stt Ss_heat_pump_stt;
  te_heat_stt Ss_elec_htr_bstr_stt;
};

#endif
#endif                                 // WaterHeatCtrl_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
