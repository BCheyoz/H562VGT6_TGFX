//
// File: VentCtrl_types.h
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.136
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:24:26 2025
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
#ifndef VentCtrl_types_h_
#define VentCtrl_types_h_
#include <stdint.h>
#include <stdbool.h>
#ifndef DEFINED_TYPEDEF_FOR_te_vent_op_mode_
#define DEFINED_TYPEDEF_FOR_te_vent_op_mode_

enum class te_vent_op_mode
  : int32_t {
  Inoperative = 0,
  SafetyOff,
  ConstantFlow,
  Normal                               // Default value
};

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

#ifndef DEFINED_TYPEDEF_FOR_tb_VentCtrl_Out_
#define DEFINED_TYPEDEF_FOR_tb_VentCtrl_Out_

struct tb_VentCtrl_Out
{
  ta_air_pres Cs_vent_pres_sp;
  ta_flow Cs_vent_flow_sp;
  ta_vltg Cs_vent_vltg_sp;
  ta_air_pres Cs_pres_rise_curv_sp;
};

#endif
#endif                                 // VentCtrl_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
