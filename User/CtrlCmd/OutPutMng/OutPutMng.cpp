//
// File: OutPutMng.cpp
//
// Code generated for Simulink model 'OutPutMng'.
//
// Model version                  : 1.23
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:02:55 2025
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
#include "OutPutMng.h"
#include "OutPutMng_types.h"

// Output and update for referenced model: 'OutPutMng'
void OutPutMng::step(const tb_OutPutMng_In *rtu_OutPutMng_In, tb_Control_Out *rty_Control_Out)
{
  rty_Control_Out->Cs_v40_lvl = rtu_OutPutMng_In->Cs_v40_rat;
  rty_Control_Out->Ss_op_mode = rtu_OutPutMng_In->Ss_op_mode;
  rty_Control_Out->Ss_sg_stt = rtu_OutPutMng_In->Ss_sg_stt;
  rty_Control_Out->Ss_oph_stt = rtu_OutPutMng_In->Ss_oph_stt;
  rty_Control_Out->Cs_tank_down_temp = rtu_OutPutMng_In->Cs_tank_down_temp;
  rty_Control_Out->Cs_tank_up_temp = rtu_OutPutMng_In->Cs_tank_up_temp;
  rty_Control_Out->Cs_pump_xhst_temp = rtu_OutPutMng_In->Cs_pump_xhst_temp;
  rty_Control_Out->Cs_pump_evap_temp = rtu_OutPutMng_In->Cs_pump_evap_temp;
  rty_Control_Out->Cs_vent_temp = rtu_OutPutMng_In->Cs_vent_temp;
  rty_Control_Out->Ss_elec_bstr_htr_sp = rtu_OutPutMng_In->Ss_elec_bstr_htr_sp;
  rty_Control_Out->Cs_heat_pump_rot_spd_sp = rtu_OutPutMng_In->Cs_heat_pump_rot_spd_sp;
  rty_Control_Out->Cs_vent_rot_spd = rtu_OutPutMng_In->Cs_vent_rot_spd;
  rty_Control_Out->Cs_vent_cnsp = rtu_OutPutMng_In->Cs_vent_cnsp;
  rty_Control_Out->Cs_heat_wtr_cnsp = rtu_OutPutMng_In->Cs_heat_wtr_cnsp;
  rty_Control_Out->Cs_tot_cnsp = rtu_OutPutMng_In->Cs_tot_cnsp;
  rty_Control_Out->Cs_v40_sp = rtu_OutPutMng_In->Cs_v40_sp;
  rty_Control_Out->Cs_temp_sp = rtu_OutPutMng_In->Cs_temp_sp;
  rty_Control_Out->Cs_vent_pres_sp = rtu_OutPutMng_In->Cs_vent_pres_sp;
  rty_Control_Out->Cs_vent_flow_sp = rtu_OutPutMng_In->Cs_vent_flow_sp;
  rty_Control_Out->Cs_vent_vltg_sp = rtu_OutPutMng_In->Cs_vent_vltg_sp;
  rty_Control_Out->Cs_vent_pres = rtu_OutPutMng_In->Cs_vent_pres;
}

// Constructor
OutPutMng::OutPutMng()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
OutPutMng::~OutPutMng() = default;

//
// File trailer for generated code.
//
// [EOF]
//
