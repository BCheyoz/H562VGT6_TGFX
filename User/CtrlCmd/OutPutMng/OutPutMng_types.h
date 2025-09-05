//
// File: OutPutMng_types.h
//
// Code generated for Simulink model 'OutPutMng'.
//
// Model version                  : 1.24
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Sep  4 18:36:22 2025
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
#ifndef OutPutMng_types_h_
#define OutPutMng_types_h_
#include <stdint.h>
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

#ifndef DEFINED_TYPEDEF_FOR_te_on_off_
#define DEFINED_TYPEDEF_FOR_te_on_off_

enum class te_on_off
  : int32_t {
  off = 0,                             // Default value
  on,
  forced
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

#ifndef DEFINED_TYPEDEF_FOR_ta_egy_
#define DEFINED_TYPEDEF_FOR_ta_egy_

using ta_egy = uint32_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_wtr_vol_
#define DEFINED_TYPEDEF_FOR_ta_wtr_vol_

using ta_wtr_vol = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_air_pres_
#define DEFINED_TYPEDEF_FOR_ta_air_pres_

using ta_air_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_flow_
#define DEFINED_TYPEDEF_FOR_ta_flow_

using ta_flow = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_vltg_
#define DEFINED_TYPEDEF_FOR_ta_vltg_

using ta_vltg = uint16_t;

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
#endif                                 // OutPutMng_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
