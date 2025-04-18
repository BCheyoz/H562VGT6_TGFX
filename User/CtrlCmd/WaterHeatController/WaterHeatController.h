//
// File: WaterHeatController.h
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.570
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:52:49 2025
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
#ifndef WaterHeatController_h_
#define WaterHeatController_h_
#include <stdbool.h>
#include <stdint.h>
#include "WaterHeatController_types.h"
#include "Derivator.h"
#include "Hysteresis.h"
#include "PID_backCalc.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "RisingEdgeDetect.h"
#include "Timer.h"
#include "VariableRisingEdgeDelay.h"
#include "cntrlSaturation.h"
#include "firstOrderTF.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_WaterHeatCtrl_tick_C;// Variable: WaterHeatCtrl_tick_C
                                           //  Referenced by:
                                           //    '<S1>/TevapRegulator'
                                           //    '<S2>/TdownminReg'
                                           //    '<S3>/AntiShortCycle'
                                           //    '<S5>/TevapRegulator'
                                           //    '<S14>/Constant2'
                                           //    '<S17>/Timer'

extern const bool WaterHeatController_BGND;// bool ground

// Class declaration for model WaterHeatController
class WaterHeatController final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'WaterHeatController'
  struct DW_WaterHeatController_T {
    float Cs_ctrl_temp_evap_spd_cmd;   // '<S5>/Divide'
    float Derivator_h;                 // '<S18>/Derivator'
    float Cs_ctrl_tref_max_spd_cmd;    // '<S1>/Divide'
    float UnitDelay1_DSTATE;           // '<S3>/Unit Delay1'
    ta_temp UnitDelay1_DSTATE_e;       // '<S14>/Unit Delay1'
    ta_rot_spd UnitDelay_DSTATE;       // '<S4>/Unit Delay'
    uint16_t UnitDelay_DSTATE_a;       // '<S3>/Unit Delay'
    uint8_t is_active_c2_WaterHeatControlle;// '<S4>/TankLevelStates'
    uint8_t is_c2_WaterHeatController; // '<S4>/TankLevelStates'
    uint8_t is_active_c3_WaterHeatControlle;// '<S8>/Chart'
    uint8_t temporalCounter_i1;        // '<S8>/Chart'
    bool exhstTempProt_DSTATE;         // '<S3>/exhstTempProt'
    bool antiShort_DSTATE;             // '<S3>/antiShort'
    bool prevTimerVal_DSTATE;          // '<S14>/prevTimerVal'
  };

  // Invariant block signals for model 'WaterHeatController'
  struct ConstB_WaterHeatController_h_T {
    te_on_off TmpSignalConversionAtSs_ele;
    te_on_off TmpSignalConversionAtSs_hea;
    te_on_off TmpSignalConversionAtSs_e_m;
  };

  // Parameters (default storage)
  struct P_WaterHeatController_T {
    float firstOrderTF2_K;             // Mask Parameter: firstOrderTF2_K
                                          //  Referenced by: '<S23>/firstOrderTF'

    float firstOrderTF1_K;             // Mask Parameter: firstOrderTF1_K
                                          //  Referenced by: '<S22>/firstOrderTF'

    float pumpFreqFilt_K;              // Mask Parameter: pumpFreqFilt_K
                                          //  Referenced by: '<S19>/firstOrderTF'

    float pumpFreqFilt1_K;             // Mask Parameter: pumpFreqFilt1_K
                                          //  Referenced by: '<S20>/firstOrderTF'

    float firstOrderTF2_Tau;           // Mask Parameter: firstOrderTF2_Tau
                                          //  Referenced by: '<S23>/firstOrderTF'

    float firstOrderTF1_Tau;           // Mask Parameter: firstOrderTF1_Tau
                                          //  Referenced by: '<S22>/firstOrderTF'

    float pumpFreqFilt_Tau;            // Mask Parameter: pumpFreqFilt_Tau
                                          //  Referenced by: '<S19>/firstOrderTF'

    float pumpFreqFilt1_Tau;           // Mask Parameter: pumpFreqFilt1_Tau
                                          //  Referenced by: '<S20>/firstOrderTF'

    float Derivator_initVal;           // Mask Parameter: Derivator_initVal
                                          //  Referenced by: '<S18>/Derivator'

    float firstOrderTF2_initVal;       // Mask Parameter: firstOrderTF2_initVal
                                          //  Referenced by: '<S23>/firstOrderTF'

    float firstOrderTF1_initVal;       // Mask Parameter: firstOrderTF1_initVal
                                          //  Referenced by: '<S22>/firstOrderTF'

    float pumpFreqFilt_initVal;        // Mask Parameter: pumpFreqFilt_initVal
                                          //  Referenced by: '<S19>/firstOrderTF'

    float pumpFreqFilt1_initVal;       // Mask Parameter: pumpFreqFilt1_initVal
                                          //  Referenced by: '<S20>/firstOrderTF'

    float HysteresisProtectTankDefrost_of;// Mask Parameter: HysteresisProtectTankDefrost_of
                                             //  Referenced by: '<S6>/Hysteresis'

    float HysteresisProtectTankDefrost_on;// Mask Parameter: HysteresisProtectTankDefrost_on
                                             //  Referenced by: '<S6>/Hysteresis'

    float Derivator_sampleTime;        // Mask Parameter: Derivator_sampleTime
                                          //  Referenced by: '<S18>/Derivator'

    float firstOrderTF2_sampleTime;    // Mask Parameter: firstOrderTF2_sampleTime
                                          //  Referenced by: '<S23>/firstOrderTF'

    float firstOrderTF1_sampleTime;    // Mask Parameter: firstOrderTF1_sampleTime
                                          //  Referenced by: '<S22>/firstOrderTF'

    float pumpFreqFilt_sampleTime;     // Mask Parameter: pumpFreqFilt_sampleTime
                                          //  Referenced by: '<S19>/firstOrderTF'

    float pumpFreqFilt1_sampleTime;    // Mask Parameter: pumpFreqFilt1_sampleTime
                                          //  Referenced by: '<S20>/firstOrderTF'

    ta_temp HysteresisProtectTankDefrost_sw;// Mask Parameter: HysteresisProtectTankDefrost_sw
                                               //  Referenced by: '<S6>/Hysteresis'

    ta_temp HysteresisProtectTankDefrost__g;// Mask Parameter: HysteresisProtectTankDefrost__g
                                               //  Referenced by: '<S6>/Hysteresis'

    double Constant5_Value;            // Expression: 1
                                          //  Referenced by: '<S3>/Constant5'

    double Constant_Value;             // Expression: -100/120
                                          //  Referenced by: '<S8>/Constant'

    double Constant2_Value;            // Expression: -12/120
                                          //  Referenced by: '<S8>/Constant2'

    double Constant1_Value;            // Expression: 0
                                          //  Referenced by: '<S8>/Constant1'

    float Cs_ctrl_tref_max_spd_cmd_Y0; // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
                                          //  Referenced by: '<S1>/Cs_ctrl_tref_max_spd_cmd'

    float TevapRegulator_InitVal;      // Computed Parameter: TevapRegulator_InitVal
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TevapRegulator_Kawu;         // Computed Parameter: TevapRegulator_Kawu
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TevapRegulator_Kd;           // Computed Parameter: TevapRegulator_Kd
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TevapRegulator_Ki;           // Computed Parameter: TevapRegulator_Ki
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TevapRegulator_Kp;           // Computed Parameter: TevapRegulator_Kp
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TevapRegulator_Tau_f;        // Computed Parameter: TevapRegulator_Tau_f
                                          //  Referenced by: '<S1>/TevapRegulator'

    float TdownminReg_InitVal;         // Computed Parameter: TdownminReg_InitVal
                                          //  Referenced by: '<S2>/TdownminReg'

    float TdownminReg_Kawu;            // Computed Parameter: TdownminReg_Kawu
                                          //  Referenced by: '<S2>/TdownminReg'

    float TdownminReg_Kd;              // Computed Parameter: TdownminReg_Kd
                                          //  Referenced by: '<S2>/TdownminReg'

    float TdownminReg_Ki;              // Computed Parameter: TdownminReg_Ki
                                          //  Referenced by: '<S2>/TdownminReg'

    float TdownminReg_Kp;              // Computed Parameter: TdownminReg_Kp
                                          //  Referenced by: '<S2>/TdownminReg'

    float TdownminReg_Tau_f;           // Computed Parameter: TdownminReg_Tau_f
                                          //  Referenced by: '<S2>/TdownminReg'

    float Cs_tank_down_temp_der_Y0;    // Computed Parameter: Cs_tank_down_temp_der_Y0
                                          //  Referenced by: '<S15>/Cs_tank_down_temp_der'

    float Saturation_UpperSat;         // Computed Parameter: Saturation_UpperSat
                                          //  Referenced by: '<S9>/Saturation'

    float Saturation_LowerSat;         // Computed Parameter: Saturation_LowerSat
                                          //  Referenced by: '<S9>/Saturation'

    float Saturation1_UpperSat;        // Computed Parameter: Saturation1_UpperSat
                                          //  Referenced by: '<S9>/Saturation1'

    float Saturation1_LowerSat;        // Computed Parameter: Saturation1_LowerSat
                                          //  Referenced by: '<S9>/Saturation1'

    float Cs_ctrl_temp_evap_spd_cmd_Y0;// Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
                                          //  Referenced by: '<S5>/Cs_ctrl_temp_evap_spd_cmd'

    float TevapRegulator_InitVal_l;    // Computed Parameter: TevapRegulator_InitVal_l
                                          //  Referenced by: '<S5>/TevapRegulator'

    float TevapRegulator_Kawu_e;       // Computed Parameter: TevapRegulator_Kawu_e
                                          //  Referenced by: '<S5>/TevapRegulator'

    float TevapRegulator_Kd_l;         // Computed Parameter: TevapRegulator_Kd_l
                                          //  Referenced by: '<S5>/TevapRegulator'

    float TevapRegulator_Ki_c;         // Computed Parameter: TevapRegulator_Ki_c
                                          //  Referenced by: '<S5>/TevapRegulator'

    float TevapRegulator_Kp_h;         // Computed Parameter: TevapRegulator_Kp_h
                                          //  Referenced by: '<S5>/TevapRegulator'

    float TevapRegulator_Tau_f_j;      // Computed Parameter: TevapRegulator_Tau_f_j
                                          //  Referenced by: '<S5>/TevapRegulator'

    float evapTempProtDelay_Delay;     // Computed Parameter: evapTempProtDelay_Delay
                                          //  Referenced by: '<S3>/evapTempProtDelay'

    float evapTempProtDelay_TimeStep;  // Computed Parameter: evapTempProtDelay_TimeStep
                                          //  Referenced by: '<S3>/evapTempProtDelay'

    float UnitDelay1_InitialCondition; // Computed Parameter: UnitDelay1_InitialCondition
                                          //  Referenced by: '<S3>/Unit Delay1'

    float evapTempProtDisable_Delay_Delay;// Computed Parameter: evapTempProtDisable_Delay_Delay
                                             //  Referenced by: '<S3>/evapTempProtDisable_Delay'

    float evapTempProtDisable_Delay_TimeS;// Computed Parameter: evapTempProtDisable_Delay_TimeS
                                             //  Referenced by: '<S3>/evapTempProtDisable_Delay'

    te_pump_mode TrefReg_Value;        // Expression: te_pump_mode.Tref
                                          //  Referenced by: '<S9>/TrefReg'

    te_pump_mode TevapReg_Value;       // Expression: te_pump_mode.Tevap
                                          //  Referenced by: '<S9>/TevapReg'

    te_pump_mode TankLevelReg_Value;   // Expression: te_pump_mode.Tank_lvl
                                          //  Referenced by: '<S9>/TankLevelReg'

    ta_temp Constant2_Value_c;         // Expression: Cs_temp_ref_tol_C
                                          //  Referenced by: '<S1>/Constant2'

    ta_temp Temp_10C_SetPointPID_Value;// Computed Parameter: Temp_10C_SetPointPID_Value
                                          //  Referenced by: '<S2>/Temp_10°C_SetPointPID'

    ta_temp TimeToPumpStartup_bp01Data[6];// Expression: Ct_tank_down_temp_pump_off_bp_C
                                             //  Referenced by: '<S3>/TimeToPumpStartup'

    ta_temp Constant8_Value;           // Expression: Cs_temp_tank_high_C
                                          //  Referenced by: '<S12>/Constant8'

    ta_temp Constant6_Value;           // Expression: Cs_temp_tank_full_C
                                          //  Referenced by: '<S12>/Constant6'

    ta_temp Constant3_Value;           // Expression: Cs_temp_tank_low_C
                                          //  Referenced by: '<S12>/Constant3'

    ta_temp Temp_2C_SetPointPID_Value; // Computed Parameter: Temp_2C_SetPointPID_Value
                                          //  Referenced by: '<S5>/Temp_-2°C_SetPointPID'

    ta_temp Tref_max_tableData[7];     // Expression: Ct_temp_ref_max_data_C
                                          //  Referenced by: '<S3>/Tref_max'

    ta_temp Tref_max_bp01Data[7];      // Expression: Ct_temp_evap_bp_C
                                          //  Referenced by: '<S3>/Tref_max'

    ta_temp Constant4_Value;           // Expression: Cs_hpc_dfr_prot_temp_l_C
                                          //  Referenced by: '<S3>/Constant4'

    ta_temp Constant6_Value_h;         // Expression: Cs_hpc_dfr_prot_temp_l_C
                                          //  Referenced by: '<S3>/Constant6'

    ta_temp Constant4_Value_p;         // Expression: Cs_temp_tank_empty_C
                                          //  Referenced by: '<S12>/Constant4'

    ta_temp Constant5_Value_n;         // Expression: Cs_temp_up_tank_low_C
                                          //  Referenced by: '<S12>/Constant5'

    ta_temp Constant2_Value_i;         // Expression: Cs_temp_tank_full_C
                                          //  Referenced by: '<S12>/Constant2'

    ta_temp CartoSetPointTankCold_bp01Data[8];// Expression: Ct_temp_tank_cold_sp_bp_C
                                                 //  Referenced by: '<S10>/CartoSetPointTankCold'

    ta_temp CartoSetPointTankCold_low_table[8];// Computed Parameter: CartoSetPointTankCold_low_table
                                                  //  Referenced by: '<S10>/CartoSetPointTankCold_low'

    ta_temp CartoSetPointTankCold_low_bp01D[8];// Expression: Ct_temp_tank_cold_sp_bp_C
                                                  //  Referenced by: '<S10>/CartoSetPointTankCold_low'

    int16_t Constant3_Value_a;         // Computed Parameter: Constant3_Value_a
                                          //  Referenced by: '<S14>/Constant3'

    int16_t UnitDelay1_InitialCondition_n;// Computed Parameter: UnitDelay1_InitialCondition_n
                                             //  Referenced by: '<S14>/Unit Delay1'

    ta_rot_spd max_Value;              // Computed Parameter: max_Value
                                          //  Referenced by: '<S1>/max'

    ta_rot_spd maxDerat_Value;         // Computed Parameter: maxDerat_Value
                                          //  Referenced by: '<S1>/maxDerat'

    ta_rot_spd minDerat_Value;         // Computed Parameter: minDerat_Value
                                          //  Referenced by: '<S1>/minDerat'

    ta_rot_spd Cs_ctrl_temp_tdwn_spd_cmd_Y0;// Computed Parameter: Cs_ctrl_temp_tdwn_spd_cmd_Y0
                                               //  Referenced by: '<S2>/Cs_ctrl_temp_tdwn_spd_cmd'

    ta_rot_spd SpdMinPIDTdownTank_Value;// Expression: Cs_rot_spd_pump_min_C
                                           //  Referenced by: '<S2>/SpdMinPIDTdownTank'

    ta_rot_spd SpdMaxPIDTdownTank_Value;// Expression: Cs_rot_spd_pump_max_C
                                           //  Referenced by: '<S2>/SpdMaxPIDTdownTank'

    uint16_t TimeToPumpStartup_tableData[6];// Expression: Ct_time_cntr_pump_off_data_C
                                               //  Referenced by: '<S3>/TimeToPumpStartup'

    ta_rot_spd SpdOFF_Value;           // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S9>/SpdOFF'

    ta_rot_spd SpdMax_Value;           // Expression: Cs_rot_spd_pump_max_C
                                          //  Referenced by: '<S10>/SpdMax'

    ta_wtr_vol Constant1_Value_c;      // Expression: Cs_v40_tank_low_C
                                          //  Referenced by: '<S12>/Constant1'

    ta_rot_spd max_Value_i;            // Computed Parameter: max_Value_i
                                          //  Referenced by: '<S5>/max'

    ta_rot_spd maxDerat_Value_f;       // Computed Parameter: maxDerat_Value_f
                                          //  Referenced by: '<S5>/maxDerat'

    ta_rot_spd minDerat_Value_h;       // Computed Parameter: minDerat_Value_h
                                          //  Referenced by: '<S5>/minDerat'

    ta_wtr_vol Constant7_Value;        // Computed Parameter: Constant7_Value
                                          //  Referenced by: '<S12>/Constant7'

    ta_wtr_vol isV40Treshold_Threshold;// Computed Parameter: isV40Treshold_Threshold
                                          //  Referenced by: '<S12>/isV40Treshold'

    uint16_t UnitDelay_InitialCondition;// Computed Parameter: UnitDelay_InitialCondition
                                           //  Referenced by: '<S4>/Unit Delay'

    ta_rot_spd Constant_Value_p;       // Expression: Cs_rot_spd_pump_max_C
                                          //  Referenced by: '<S4>/Constant'

    ta_rot_spd Constant1_Value_f;      // Expression: Cs_rot_spd_pump_min_C
                                          //  Referenced by: '<S4>/Constant1'

    ta_rot_spd CartoSetPointTankCold_tableData[8];// Expression: Ct_rpm_pump_sp_tank_cold_C
                                                     //  Referenced by: '<S10>/CartoSetPointTankCold'

    ta_rot_spd SpdOFF_Value_b;         // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S10>/SpdOFF'

    ta_rot_spd SpdOFF_Value_e;         // Computed Parameter: SpdOFF_Value_e
                                          //  Referenced by: '<S3>/SpdOFF'

    uint16_t UnitDelay_InitialCondition_m;// Computed Parameter: UnitDelay_InitialCondition_m
                                             //  Referenced by: '<S3>/Unit Delay'

    bool exhstTempProt_InitialCondition;// Expression: false
                                           //  Referenced by: '<S3>/exhstTempProt'

    bool Constant3_Value_g;            // Expression: false
                                          //  Referenced by: '<S3>/Constant3'

    bool antiShort_InitialCondition;   // Expression: false
                                          //  Referenced by: '<S3>/antiShort'

    bool prevTimerVal_InitialCondition;// Computed Parameter: prevTimerVal_InitialCondition
                                          //  Referenced by: '<S14>/prevTimerVal'

    uint8_t Zero_Value;                // Computed Parameter: Zero_Value
                                          //  Referenced by: '<S3>/Zero'

    uint8_t Constant4_Value_e;         // Computed Parameter: Constant4_Value_e
                                          //  Referenced by: '<S14>/Constant4'

  };

  // Initial conditions function
  void init();

  // Copy Constructor
  WaterHeatController(WaterHeatController const&) = delete;

  // Assignment Operator
  WaterHeatController& operator= (WaterHeatController const&) & = delete;

  // Move Constructor
  WaterHeatController(WaterHeatController &&) = delete;

  // Move Assignment Operator
  WaterHeatController& operator= (WaterHeatController &&) = delete;

  // model step function
  void step(const ta_wtr_vol *rtu_Cs_v40_min, const ta_temp *rtu_Cs_tank_down_temp, const ta_temp *rtu_Cs_tank_up_temp, const ta_wtr_vol *rtu_Cs_v40_sp, const
            ta_temp *rtu_Cs_pump_evap_temp, const ta_temp *rtu_Cs_pump_xhst_temp, ta_rot_spd *rty_Cs_heat_pump_rot_spd_sp, uint8_t *rty_Cs_v40_rat);

  // Reset function
  void reset();

  // Constructor
  WaterHeatController();

  // Destructor
  ~WaterHeatController();

  // private data and function members
 private:
  // Block states
  DW_WaterHeatController_T WaterHeatController_DW;

  // Tunable parameters
  static P_WaterHeatController_T WaterHeatController_rtP;

  // model instance variable for '<S1>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ1;

  // model instance variable for '<S2>/TdownminReg'
  PID_backCalc TdownminRegMDLOBJ2;

  // model instance variable for '<S3>/AntiShortCycle'
  VariableRisingEdgeDelay AntiShortCycleMDLOBJ3;

  // model instance variable for '<S6>/Hysteresis'
  Hysteresis HysteresisMDLOBJ4;

  // model instance variable for '<S3>/PumpOFFdetect'
  RisingEdgeDetect PumpOFFdetectMDLOBJ5;

  // model instance variable for '<S7>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ6;

  // model instance variable for '<S3>/evapTempProtDelay'
  RisingEdgeDelay evapTempProtDelayMDLOBJ7;

  // model instance variable for '<S3>/evapTempProtDisable_Delay'
  RisingEdgeDelay evapTempProtDisable_MDLOBJ8;

  // model instance variable for '<S17>/Timer'
  Timer TimerMDLOBJ9;

  // model instance variable for '<S8>/Model'
  RisingEdgeDetect ModelMDLOBJ10;

  // model instance variable for '<S18>/Derivator'
  Derivator DerivatorMDLOBJ11;

  // model instance variable for '<S16>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ12;

  // model instance variable for '<S19>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ13;

  // model instance variable for '<S20>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ14;

  // model instance variable for '<S21>/Saturation'
  cntrlSaturation SaturationMDLOBJ15;

  // model instance variable for '<S22>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ16;

  // model instance variable for '<S23>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ17;

  // model instance variable for '<S5>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ18;
};

extern WaterHeatController::P_WaterHeatController_T WaterHeatController_rtP;

// Invariant block signals (default storage)
extern const WaterHeatController::ConstB_WaterHeatController_h_T WaterHeatController_ConstB;

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'WaterHeatController'
//  '<S1>'   : 'WaterHeatController/CntrlProtectPump'
//  '<S2>'   : 'WaterHeatController/CntrlTankDefrostPID'
//  '<S3>'   : 'WaterHeatController/ConditionsPumpProtection'
//  '<S4>'   : 'WaterHeatController/Hyst_Temperature_Control'
//  '<S5>'   : 'WaterHeatController/Subsystem1'
//  '<S6>'   : 'WaterHeatController/ConditionsPumpProtection/HysteresisProtectTankDefrost'
//  '<S7>'   : 'WaterHeatController/ConditionsPumpProtection/TevapProtState'
//  '<S8>'   : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod'
//  '<S9>'   : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng'
//  '<S10>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement'
//  '<S11>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelStates'
//  '<S12>'  : 'WaterHeatController/Hyst_Temperature_Control/tank levels indicators'
//  '<S13>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/Chart'
//  '<S14>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/DownTempVar'
//  '<S15>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/TempVarWatch'
//  '<S16>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/UseRecStt'
//  '<S17>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/DownTempVar/nextChangeTimer'
//  '<S18>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecrod/TempVarWatch/Derivator'
//  '<S19>'  : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng/pumpFreqFilt'
//  '<S20>'  : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng/pumpFreqFilt1'
//  '<S21>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/Pump_rpm_sat'
//  '<S22>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/firstOrderTF1'
//  '<S23>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/firstOrderTF2'

#endif                                 // WaterHeatController_h_

//
// File trailer for generated code.
//
// [EOF]
//
