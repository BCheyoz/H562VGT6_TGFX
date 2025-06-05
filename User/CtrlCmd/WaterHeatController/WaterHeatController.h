//
// File: WaterHeatController.h
//
// Code generated for Simulink model 'WaterHeatController'.
//
// Model version                  : 1.598
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Wed Jun  4 11:34:29 2025
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
#include "VariableRisingEdgeDelay.h"
#include "cntrlSaturation.h"
#include "firstOrderTF.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_WaterHeatCtrl_tick_C;// Variable: WaterHeatCtrl_tick_C
                                           //  Referenced by:
                                           //    '<S1>/TevapRegulator'
                                           //    '<S2>/TevapRegulator'
                                           //    '<S3>/TdownminReg'
                                           //    '<S4>/AntiShortCycle'

extern const bool WaterHeatController_BGND;// bool ground

// Class declaration for model WaterHeatController
class WaterHeatController final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'WaterHeatController'
  struct DW_WaterHeatController_T {
    float Derivator_h;                 // '<S17>/Derivator'
    float Cs_ctrl_tref_max_spd_cmd;    // '<S2>/Divide'
    float Cs_ctrl_temp_evap_spd_cmd;   // '<S1>/Divide'
    float UnitDelay1_DSTATE;           // '<S4>/Unit Delay1'
    ta_temp TempThreshold[5];          // '<S10>/TempThreshold'
    ta_temp Ct_temp_thrs_dly_DSTATE[5];// '<S10>/Ct_temp_thrs_dly'
    ta_rot_spd UnitDelay_DSTATE;       // '<S5>/Unit Delay'
    uint16_t UnitDelay_DSTATE_a;       // '<S4>/Unit Delay'
    uint8_t is_active_c2_WaterHeatControlle;// '<S12>/TankLevelStates'
    uint8_t is_c2_WaterHeatController; // '<S12>/TankLevelStates'
    uint8_t is_active_c3_WaterHeatControlle;// '<S13>/Chart'
    uint8_t temporalCounter_i1;        // '<S13>/Chart'
    bool exhstTempProt_DSTATE;         // '<S4>/exhstTempProt'
    bool antiShort_DSTATE;             // '<S4>/antiShort'
  };

  // Invariant block signals for model 'WaterHeatController'
  struct ConstB_WaterHeatController_h_T {
    te_on_off TmpSignalConversionAtSs_ele;
    te_on_off TmpSignalConversionAtSs_hea;
    te_on_off TmpSignalConversionAtSs_e_m;
    uint8_t TmpSignalConversionAtCs_v40;
  };

  // Parameters (default storage)
  struct P_WaterHeatController_T {
    float filterSp1_K;                 // Mask Parameter: filterSp1_K
                                          //  Referenced by: '<S24>/firstOrderTF'

    float filterSp2_K;                 // Mask Parameter: filterSp2_K
                                          //  Referenced by: '<S25>/firstOrderTF'

    float pumpFreqFilt_K;              // Mask Parameter: pumpFreqFilt_K
                                          //  Referenced by: '<S18>/firstOrderTF'

    float pumpFreqFilt1_K;             // Mask Parameter: pumpFreqFilt1_K
                                          //  Referenced by: '<S19>/firstOrderTF'

    float filterSp1_Tau;               // Mask Parameter: filterSp1_Tau
                                          //  Referenced by: '<S24>/firstOrderTF'

    float filterSp2_Tau;               // Mask Parameter: filterSp2_Tau
                                          //  Referenced by: '<S25>/firstOrderTF'

    float pumpFreqFilt_Tau;            // Mask Parameter: pumpFreqFilt_Tau
                                          //  Referenced by: '<S18>/firstOrderTF'

    float pumpFreqFilt1_Tau;           // Mask Parameter: pumpFreqFilt1_Tau
                                          //  Referenced by: '<S19>/firstOrderTF'

    float Derivator_initVal;           // Mask Parameter: Derivator_initVal
                                          //  Referenced by: '<S17>/Derivator'

    float filterSp1_initVal;           // Mask Parameter: filterSp1_initVal
                                          //  Referenced by: '<S24>/firstOrderTF'

    float filterSp2_initVal;           // Mask Parameter: filterSp2_initVal
                                          //  Referenced by: '<S25>/firstOrderTF'

    float pumpFreqFilt_initVal;        // Mask Parameter: pumpFreqFilt_initVal
                                          //  Referenced by: '<S18>/firstOrderTF'

    float pumpFreqFilt1_initVal;       // Mask Parameter: pumpFreqFilt1_initVal
                                          //  Referenced by: '<S19>/firstOrderTF'

    float HysteresisProtectTankDefrost_of;// Mask Parameter: HysteresisProtectTankDefrost_of
                                             //  Referenced by: '<S6>/Hysteresis'

    float HysteresisProtectTankDefrost_on;// Mask Parameter: HysteresisProtectTankDefrost_on
                                             //  Referenced by: '<S6>/Hysteresis'

    float Derivator_sampleTime;        // Mask Parameter: Derivator_sampleTime
                                          //  Referenced by: '<S17>/Derivator'

    float filterSp1_sampleTime;        // Mask Parameter: filterSp1_sampleTime
                                          //  Referenced by: '<S24>/firstOrderTF'

    float filterSp2_sampleTime;        // Mask Parameter: filterSp2_sampleTime
                                          //  Referenced by: '<S25>/firstOrderTF'

    float pumpFreqFilt_sampleTime;     // Mask Parameter: pumpFreqFilt_sampleTime
                                          //  Referenced by: '<S18>/firstOrderTF'

    float pumpFreqFilt1_sampleTime;    // Mask Parameter: pumpFreqFilt1_sampleTime
                                          //  Referenced by: '<S19>/firstOrderTF'

    ta_temp HysteresisProtectTankDefrost_sw;// Mask Parameter: HysteresisProtectTankDefrost_sw
                                               //  Referenced by: '<S6>/Hysteresis'

    ta_temp HysteresisProtectTankDefrost__g;// Mask Parameter: HysteresisProtectTankDefrost__g
                                               //  Referenced by: '<S6>/Hysteresis'

    double Constant5_Value;            // Expression: 1
                                          //  Referenced by: '<S4>/Constant5'

    double Cs_hard_draw_thrs_Value;    // Expression: Cs_hard_draw_thrs_C
                                          //  Referenced by: '<S8>/Cs_hard_draw_thrs'

    double Cs_soft_draw_thrs_Value;    // Expression: Cs_soft_draw_thrs_C
                                          //  Referenced by: '<S8>/Cs_soft_draw_thrs'

    double Constant1_Value;            // Expression: 0
                                          //  Referenced by: '<S8>/Constant1'

    float Cs_ctrl_temp_evap_spd_cmd_Y0;// Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
                                          //  Referenced by: '<S1>/Cs_ctrl_temp_evap_spd_cmd'

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

    float Cs_ctrl_tref_max_spd_cmd_Y0; // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
                                          //  Referenced by: '<S2>/Cs_ctrl_tref_max_spd_cmd'

    float TevapRegulator_InitVal_a;    // Computed Parameter: TevapRegulator_InitVal_a
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TevapRegulator_Kawu_j;       // Computed Parameter: TevapRegulator_Kawu_j
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TevapRegulator_Kd_b;         // Computed Parameter: TevapRegulator_Kd_b
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TevapRegulator_Ki_i;         // Computed Parameter: TevapRegulator_Ki_i
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TevapRegulator_Kp_d;         // Computed Parameter: TevapRegulator_Kp_d
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TevapRegulator_Tau_f_n;      // Computed Parameter: TevapRegulator_Tau_f_n
                                          //  Referenced by: '<S2>/TevapRegulator'

    float TdownminReg_InitVal;         // Computed Parameter: TdownminReg_InitVal
                                          //  Referenced by: '<S3>/TdownminReg'

    float TdownminReg_Kawu;            // Computed Parameter: TdownminReg_Kawu
                                          //  Referenced by: '<S3>/TdownminReg'

    float TdownminReg_Kd;              // Computed Parameter: TdownminReg_Kd
                                          //  Referenced by: '<S3>/TdownminReg'

    float TdownminReg_Ki;              // Computed Parameter: TdownminReg_Ki
                                          //  Referenced by: '<S3>/TdownminReg'

    float TdownminReg_Kp;              // Computed Parameter: TdownminReg_Kp
                                          //  Referenced by: '<S3>/TdownminReg'

    float TdownminReg_Tau_f;           // Computed Parameter: TdownminReg_Tau_f
                                          //  Referenced by: '<S3>/TdownminReg'

    float Cs_tank_down_temp_der_Y0;    // Computed Parameter: Cs_tank_down_temp_der_Y0
                                          //  Referenced by: '<S16>/Cs_tank_down_temp_der'

    float Saturation_UpperSat;         // Computed Parameter: Saturation_UpperSat
                                          //  Referenced by: '<S9>/Saturation'

    float Saturation_LowerSat;         // Computed Parameter: Saturation_LowerSat
                                          //  Referenced by: '<S9>/Saturation'

    float Saturation1_UpperSat;        // Computed Parameter: Saturation1_UpperSat
                                          //  Referenced by: '<S9>/Saturation1'

    float Saturation1_LowerSat;        // Computed Parameter: Saturation1_LowerSat
                                          //  Referenced by: '<S9>/Saturation1'

    float evapTempProtDelay_Delay;     // Computed Parameter: evapTempProtDelay_Delay
                                          //  Referenced by: '<S4>/evapTempProtDelay'

    float evapTempProtDelay_TimeStep;  // Computed Parameter: evapTempProtDelay_TimeStep
                                          //  Referenced by: '<S4>/evapTempProtDelay'

    float UnitDelay1_InitialCondition; // Computed Parameter: UnitDelay1_InitialCondition
                                          //  Referenced by: '<S4>/Unit Delay1'

    float evapTempProtDisable_Delay_Delay;// Computed Parameter: evapTempProtDisable_Delay_Delay
                                             //  Referenced by: '<S4>/evapTempProtDisable_Delay'

    float evapTempProtDisable_Delay_TimeS;// Computed Parameter: evapTempProtDisable_Delay_TimeS
                                             //  Referenced by: '<S4>/evapTempProtDisable_Delay'

    te_pump_mode TrefReg_Value;        // Expression: te_pump_mode.Tref
                                          //  Referenced by: '<S9>/TrefReg'

    te_pump_mode TevapReg_Value;       // Expression: te_pump_mode.Tevap
                                          //  Referenced by: '<S9>/TevapReg'

    te_pump_mode TankLevelReg_Value;   // Expression: te_pump_mode.Tank_lvl
                                          //  Referenced by: '<S9>/TankLevelReg'

    te_tank_size L180_Value;           // Expression: te_tank_size.L180
                                          //  Referenced by: '<S21>/L180'

    te_tank_size L180_Value_c;         // Expression: te_tank_size.L180
                                          //  Referenced by: '<S22>/L180'

    ta_temp Temp_2C_SetPointPID_Value; // Computed Parameter: Temp_2C_SetPointPID_Value
                                          //  Referenced by: '<S1>/Temp_-2°C_SetPointPID'

    ta_temp Constant2_Value;           // Expression: Cs_temp_ref_tol_C
                                          //  Referenced by: '<S2>/Constant2'

    ta_temp Temp_10C_SetPointPID_Value;// Computed Parameter: Temp_10C_SetPointPID_Value
                                          //  Referenced by: '<S3>/Temp_10°C_SetPointPID'

    ta_temp TimeToPumpStartup_bp01Data[6];// Expression: Ct_tank_down_temp_pump_off_bp_C
                                             //  Referenced by: '<S4>/TimeToPumpStartup'

    ta_temp Ct_180L_eco_3ppl_Value[5]; // Computed Parameter: Ct_180L_eco_3ppl_Value
                                          //  Referenced by: '<S22>/Ct_180L_eco_3ppl'

    ta_temp Ct_180L_eco_4ppl_Value[5]; // Computed Parameter: Ct_180L_eco_4ppl_Value
                                          //  Referenced by: '<S22>/Ct_180L_eco_4ppl'

    ta_temp Ct_180L_eco_5ppl_Value[5]; // Computed Parameter: Ct_180L_eco_5ppl_Value
                                          //  Referenced by: '<S22>/Ct_180L_eco_5ppl'

    ta_temp Ct_180L_eco_6ppl_Value[5]; // Computed Parameter: Ct_180L_eco_6ppl_Value
                                          //  Referenced by: '<S22>/Ct_180L_eco_6ppl'

    ta_temp Ct_105L_eco_3ppl_Value[5]; // Computed Parameter: Ct_105L_eco_3ppl_Value
                                          //  Referenced by: '<S22>/Ct_105L_eco_3ppl'

    ta_temp Ct_105L_eco_4ppl_Value[5]; // Computed Parameter: Ct_105L_eco_4ppl_Value
                                          //  Referenced by: '<S22>/Ct_105L_eco_4ppl'

    ta_temp Ct_105L_eco_5ppl_Value[5]; // Computed Parameter: Ct_105L_eco_5ppl_Value
                                          //  Referenced by: '<S22>/Ct_105L_eco_5ppl'

    ta_temp Ct_105L_eco_6ppl_Value[5]; // Computed Parameter: Ct_105L_eco_6ppl_Value
                                          //  Referenced by: '<S22>/Ct_105L_eco_6ppl'

    ta_temp Ct_180L_auto_3ppl_Value[5];// Computed Parameter: Ct_180L_auto_3ppl_Value
                                          //  Referenced by: '<S21>/Ct_180L_auto_3ppl'

    ta_temp Ct_180L_auto_4ppl_Value[5];// Computed Parameter: Ct_180L_auto_4ppl_Value
                                          //  Referenced by: '<S21>/Ct_180L_auto_4ppl'

    ta_temp Ct_180L_auto_5ppl_Value[5];// Computed Parameter: Ct_180L_auto_5ppl_Value
                                          //  Referenced by: '<S21>/Ct_180L_auto_5ppl'

    ta_temp Ct_180L_auto_6ppl_Value[5];// Computed Parameter: Ct_180L_auto_6ppl_Value
                                          //  Referenced by: '<S21>/Ct_180L_auto_6ppl'

    ta_temp Ct_105L_auto_3ppl_Value[5];// Computed Parameter: Ct_105L_auto_3ppl_Value
                                          //  Referenced by: '<S21>/Ct_105L_auto_3ppl'

    ta_temp Ct_105L_auto_4ppl_Value[5];// Computed Parameter: Ct_105L_auto_4ppl_Value
                                          //  Referenced by: '<S21>/Ct_105L_auto_4ppl'

    ta_temp Ct_105L_auto_5ppl_Value[5];// Computed Parameter: Ct_105L_auto_5ppl_Value
                                          //  Referenced by: '<S21>/Ct_105L_auto_5ppl'

    ta_temp Ct_105L_auto_6ppl_Value[5];// Computed Parameter: Ct_105L_auto_6ppl_Value
                                          //  Referenced by: '<S21>/Ct_105L_auto_6ppl'

    int16_t Ct_temp_thrs_dly_InitialConditi;// Computed Parameter: Ct_temp_thrs_dly_InitialConditi
                                               //  Referenced by: '<S10>/Ct_temp_thrs_dly'

    ta_temp Ct_180L_auto_2ppl_Value[5];// Computed Parameter: Ct_180L_auto_2ppl_Value
                                          //  Referenced by: '<S21>/Ct_180L_auto_2ppl'

    ta_temp Ct_105L_auto_2ppl_Value[5];// Computed Parameter: Ct_105L_auto_2ppl_Value
                                          //  Referenced by: '<S21>/Ct_105L_auto_2ppl'

    ta_temp Ct_180L_eco_2ppl_Value[5]; // Computed Parameter: Ct_180L_eco_2ppl_Value
                                          //  Referenced by: '<S22>/Ct_180L_eco_2ppl'

    ta_temp Ct_105L_eco_2ppl_Value[5]; // Computed Parameter: Ct_105L_eco_2ppl_Value
                                          //  Referenced by: '<S22>/Ct_105L_eco_2ppl'

    ta_temp Constant4_Value;           // Expression: Cs_hpc_dfr_prot_temp_l_C
                                          //  Referenced by: '<S4>/Constant4'

    ta_temp Constant6_Value;           // Expression: Cs_hpc_dfr_prot_temp_l_C
                                          //  Referenced by: '<S4>/Constant6'

    ta_temp Tref_max_tableData[7];     // Expression: Ct_temp_ref_max_data_C
                                          //  Referenced by: '<S4>/Tref_max'

    ta_temp Tref_max_bp01Data[7];      // Expression: Ct_temp_evap_bp_C
                                          //  Referenced by: '<S4>/Tref_max'

    ta_temp CartoSetPointTankCold_bp01Data[8];// Expression: Ct_temp_tank_cold_sp_bp_C
                                                 //  Referenced by: '<S11>/CartoSetPointTankCold'

    ta_temp CartoSetPointTankCold_low_table[8];// Computed Parameter: CartoSetPointTankCold_low_table
                                                  //  Referenced by: '<S11>/CartoSetPointTankCold_low'

    ta_temp CartoSetPointTankCold_low_bp01D[8];// Expression: Ct_temp_tank_cold_sp_bp_C
                                                  //  Referenced by: '<S11>/CartoSetPointTankCold_low'

    ta_rot_spd max_Value;              // Computed Parameter: max_Value
                                          //  Referenced by: '<S1>/max'

    ta_rot_spd maxDerat_Value;         // Computed Parameter: maxDerat_Value
                                          //  Referenced by: '<S1>/maxDerat'

    ta_rot_spd minDerat_Value;         // Computed Parameter: minDerat_Value
                                          //  Referenced by: '<S1>/minDerat'

    ta_rot_spd max_Value_d;            // Computed Parameter: max_Value_d
                                          //  Referenced by: '<S2>/max'

    ta_rot_spd maxDerat_Value_e;       // Computed Parameter: maxDerat_Value_e
                                          //  Referenced by: '<S2>/maxDerat'

    ta_rot_spd minDerat_Value_m;       // Computed Parameter: minDerat_Value_m
                                          //  Referenced by: '<S2>/minDerat'

    ta_rot_spd Cs_ctrl_temp_tdwn_spd_cmd_Y0;// Computed Parameter: Cs_ctrl_temp_tdwn_spd_cmd_Y0
                                               //  Referenced by: '<S3>/Cs_ctrl_temp_tdwn_spd_cmd'

    ta_rot_spd SpdMinPIDTdownTank_Value;// Expression: Cs_rot_spd_pump_min_C
                                           //  Referenced by: '<S3>/SpdMinPIDTdownTank'

    ta_rot_spd SpdMaxPIDTdownTank_Value;// Expression: Cs_rot_spd_pump_max_C
                                           //  Referenced by: '<S3>/SpdMaxPIDTdownTank'

    uint16_t TimeToPumpStartup_tableData[6];// Expression: Ct_time_cntr_pump_off_data_C
                                               //  Referenced by: '<S4>/TimeToPumpStartup'

    ta_rot_spd SpdOFF_Value;           // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S9>/SpdOFF'

    ta_rot_spd SpdOFF1_Value;          // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S11>/SpdOFF1'

    ta_rot_spd SpdMax_Value;           // Expression: Cs_rot_spd_pump_max_C
                                          //  Referenced by: '<S11>/SpdMax'

    ta_rot_spd SpdOFF_Value_b;         // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S11>/SpdOFF'

    uint16_t UnitDelay_InitialCondition;// Computed Parameter: UnitDelay_InitialCondition
                                           //  Referenced by: '<S5>/Unit Delay'

    ta_rot_spd Cs_rot_spd_pump_max_C_Value;// Expression: Cs_rot_spd_pump_max_C
                                              //  Referenced by: '<S11>/Cs_rot_spd_pump_max_C'

    ta_rot_spd Cs_rot_spd_pump_min_C_Value;// Expression: Cs_rot_spd_pump_min_C
                                              //  Referenced by: '<S11>/Cs_rot_spd_pump_min_C'

    ta_rot_spd CartoSetPointTankCold_tableData[8];// Expression: Ct_rpm_pump_sp_tank_cold_C
                                                     //  Referenced by: '<S11>/CartoSetPointTankCold'

    ta_rot_spd SpdOFF_Value_e;         // Computed Parameter: SpdOFF_Value_e
                                          //  Referenced by: '<S4>/SpdOFF'

    uint16_t UnitDelay_InitialCondition_m;// Computed Parameter: UnitDelay_InitialCondition_m
                                             //  Referenced by: '<S4>/Unit Delay'

    bool exhstTempProt_InitialCondition;// Expression: false
                                           //  Referenced by: '<S4>/exhstTempProt'

    bool Constant3_Value;              // Expression: false
                                          //  Referenced by: '<S4>/Constant3'

    bool antiShort_InitialCondition;   // Expression: false
                                          //  Referenced by: '<S4>/antiShort'

  };

  // Initial conditions function
  void init();

  // model step function
  void step(const te_tank_size *rtu_Ss_tank_size, const ta_temp *rtu_Cs_tank_down_temp, const ta_temp *rtu_Cs_tank_up_temp, const ta_temp *rtu_Cs_pump_evap_temp,
            const ta_temp *rtu_Cs_pump_xhst_temp, const te_op_mode *rtu_Ss_op_mode, const ta_pers_nb *rtu_Ns_pers_nb, ta_rot_spd *rty_Cs_heat_pump_rot_spd_sp);

  // Copy Constructor
  WaterHeatController(WaterHeatController const&) = delete;

  // Assignment Operator
  WaterHeatController& operator= (WaterHeatController const&) & = delete;

  // Move Constructor
  WaterHeatController(WaterHeatController &&) = delete;

  // Move Assignment Operator
  WaterHeatController& operator= (WaterHeatController &&) = delete;

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

  // Declare private class scope variables for system: "model 'WaterHeatController'"
  const te_tank_size *WaterHeatContr_rtu_Ss_tank_size;// '<Root>/Ss_tank_size'
  const te_op_mode *WaterHeatControl_rtu_Ss_op_mode;// '<Root>/Ss_op_mode'
  const ta_pers_nb *WaterHeatControl_rtu_Ns_pers_nb;// '<Root>/Ns_pers_nb'

  // private member function(s) for subsystem '<S5>/ModeThresholdTempCalc'
  void Wate_ModeThresholdTempCalc_Init();
  void Wat_ModeThresholdTempCalc_Reset();
  void WaterHeat_ModeThresholdTempCalc();

  // model instance variable for '<S1>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ1;

  // model instance variable for '<S2>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ2;

  // model instance variable for '<S3>/TdownminReg'
  PID_backCalc TdownminRegMDLOBJ3;

  // model instance variable for '<S4>/AntiShortCycle'
  VariableRisingEdgeDelay AntiShortCycleMDLOBJ4;

  // model instance variable for '<S6>/Hysteresis'
  Hysteresis HysteresisMDLOBJ5;

  // model instance variable for '<S4>/PumpOFFdetect'
  RisingEdgeDetect PumpOFFdetectMDLOBJ6;

  // model instance variable for '<S7>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ7;

  // model instance variable for '<S4>/evapTempProtDelay'
  RisingEdgeDelay evapTempProtDelayMDLOBJ8;

  // model instance variable for '<S4>/evapTempProtDisable_Delay'
  RisingEdgeDelay evapTempProtDisable_MDLOBJ9;

  // model instance variable for '<S17>/Derivator'
  Derivator DerivatorMDLOBJ10;

  // model instance variable for '<S8>/Model'
  RisingEdgeDetect ModelMDLOBJ11;

  // model instance variable for '<S14>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ12;

  // model instance variable for '<S18>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ13;

  // model instance variable for '<S19>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ14;

  // model instance variable for '<S23>/Saturation'
  cntrlSaturation SaturationMDLOBJ15;

  // model instance variable for '<S24>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ16;

  // model instance variable for '<S25>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ17;
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
//  '<S1>'   : 'WaterHeatController/CntrlEvapPump'
//  '<S2>'   : 'WaterHeatController/CntrlProtectPump'
//  '<S3>'   : 'WaterHeatController/CntrlTankDefrostPID'
//  '<S4>'   : 'WaterHeatController/ConditionsPumpProtection'
//  '<S5>'   : 'WaterHeatController/Hyst_Temperature_Control'
//  '<S6>'   : 'WaterHeatController/ConditionsPumpProtection/HysteresisProtectTankDefrost'
//  '<S7>'   : 'WaterHeatController/ConditionsPumpProtection/TevapProtState'
//  '<S8>'   : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord'
//  '<S9>'   : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng'
//  '<S10>'  : 'WaterHeatController/Hyst_Temperature_Control/ModeThresholdTempCalc'
//  '<S11>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement'
//  '<S12>'  : 'WaterHeatController/Hyst_Temperature_Control/tank levels indicators'
//  '<S13>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord/DerivativeCalc'
//  '<S14>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord/UseRecStt'
//  '<S15>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord/DerivativeCalc/Chart'
//  '<S16>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord/DerivativeCalc/TempVarWatch'
//  '<S17>'  : 'WaterHeatController/Hyst_Temperature_Control/DrawRecord/DerivativeCalc/TempVarWatch/Derivator'
//  '<S18>'  : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng/pumpFreqFilt'
//  '<S19>'  : 'WaterHeatController/Hyst_Temperature_Control/HeatPumpMng/pumpFreqFilt1'
//  '<S20>'  : 'WaterHeatController/Hyst_Temperature_Control/ModeThresholdTempCalc/ThesholdCalc'
//  '<S21>'  : 'WaterHeatController/Hyst_Temperature_Control/ModeThresholdTempCalc/ThesholdCalc/autoTemperatureThreshold'
//  '<S22>'  : 'WaterHeatController/Hyst_Temperature_Control/ModeThresholdTempCalc/ThesholdCalc/ecoTemperatureThreshold'
//  '<S23>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/Pump_rpm_sat'
//  '<S24>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/filterSp1'
//  '<S25>'  : 'WaterHeatController/Hyst_Temperature_Control/TankLevelManagement/filterSp2'
//  '<S26>'  : 'WaterHeatController/Hyst_Temperature_Control/tank levels indicators/TankLevelStates'

#endif                                 // WaterHeatController_h_

//
// File trailer for generated code.
//
// [EOF]
//
