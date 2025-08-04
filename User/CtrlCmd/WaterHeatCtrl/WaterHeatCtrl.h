//
// File: WaterHeatCtrl.h
//
// Code generated for Simulink model 'WaterHeatCtrl'.
//
// Model version                  : 1.150
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jul 31 11:05:04 2025
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
#ifndef WaterHeatCtrl_h_
#define WaterHeatCtrl_h_
#include <stdbool.h>
#include <stdint.h>
#include "WaterHeatCtrl_types.h"
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
                                           //    '<S11>/TevapRegulator'
                                           //    '<S12>/TevapRegulator'
                                           //    '<S22>/AntiShortCycle'

extern const bool WaterHeatCtrl_BGND;  // bool ground

// Class declaration for model WaterHeatCtrl
class WaterHeatCtrl final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'WaterHeatCtrl'
  struct DW_WaterHeatCtrl_T {
    float Cs_ctrl_tref_max_spd_cmd;    // '<S12>/Divide'
    float Cs_ctrl_temp_evap_spd_cmd;   // '<S11>/Divide'
    float Derivator_p;                 // '<S10>/Derivator'
    float prevValue_DSTATE;            // '<S17>/prevValue'
    te_heat_stt heat_pump_stt_prev_DSTATE;// '<Root>/heat_pump_stt_prev'
    ta_temp Cs_temp_tank_high;         // '<S5>/TempThresholdSelector'
    ta_temp Cs_temp_tank_med;          // '<S5>/TempThresholdSelector'
    ta_temp Cs_temp_tank_low;          // '<S5>/TempThresholdSelector'
    ta_temp Cs_temp_tank_empt;         // '<S5>/TempThresholdSelector'
    uint16_t Cs_anti_shrt_cycl_dly_prev_DSTA;// '<S22>/Cs_anti_shrt_cycl_dly_prev'
    uint8_t is_active_c2_WaterHeatCtrl;// '<S16>/TankLevelStates'
    uint8_t is_c2_WaterHeatCtrl;       // '<S16>/TankLevelStates'
    uint8_t is_active_c3_WaterHeatCtrl;// '<S7>/Chart'
    uint8_t temporalCounter_i1;        // '<S7>/Chart'
    bool exhstTempProt_DSTATE;         // '<S19>/exhstTempProt'
  };

  // Parameters (default storage)
  struct P_WaterHeatCtrl_T {
    float filterSp1_K;                 // Mask Parameter: filterSp1_K
                                          //  Referenced by: '<S28>/firstOrderTF'

    float filterSp1_Tau;               // Mask Parameter: filterSp1_Tau
                                          //  Referenced by: '<S28>/firstOrderTF'

    float Derivator_initVal;           // Mask Parameter: Derivator_initVal
                                          //  Referenced by: '<S10>/Derivator'

    float filterSp1_initVal;           // Mask Parameter: filterSp1_initVal
                                          //  Referenced by: '<S28>/firstOrderTF'

    float HysteresisProtectTankDefrost_of;// Mask Parameter: HysteresisProtectTankDefrost_of
                                             //  Referenced by: '<S67>/Hysteresis'

    float HysteresisProtectTankDefrost_on;// Mask Parameter: HysteresisProtectTankDefrost_on
                                             //  Referenced by: '<S67>/Hysteresis'

    float Derivator_sampleTime;        // Mask Parameter: Derivator_sampleTime
                                          //  Referenced by: '<S10>/Derivator'

    float filterSp1_sampleTime;        // Mask Parameter: filterSp1_sampleTime
                                          //  Referenced by: '<S28>/firstOrderTF'

    ta_temp HysteresisProtectTankDefrost_sw;// Mask Parameter: HysteresisProtectTankDefrost_sw
                                               //  Referenced by: '<S67>/Hysteresis'

    ta_temp HysteresisProtectTankDefrost__c;// Mask Parameter: HysteresisProtectTankDefrost__c
                                               //  Referenced by: '<S67>/Hysteresis'

    double posRateLimCst_Value;        // Expression: 120/5
                                          //  Referenced by: '<S17>/posRateLimCst'

    double negRateLimCst_Value;        // Expression: -120/5
                                          //  Referenced by: '<S17>/negRateLimCst'

    double Cs_hard_draw_thrs_Value;    // Expression: Cs_hard_draw_thrs_C
                                          //  Referenced by: '<S2>/Cs_hard_draw_thrs'

    double airTempLim_Value;           // Expression: 290
                                          //  Referenced by: '<S20>/airTempLim'

    double waterTempLim_Value;         // Expression: 500
                                          //  Referenced by: '<S21>/waterTempLim'

    float Cs_tank_down_temp_der_Y0;    // Computed Parameter: Cs_tank_down_temp_der_Y0
                                          //  Referenced by: '<S9>/Cs_tank_down_temp_der'

    float Cs_ctrl_temp_evap_spd_cmd_Y0;// Computed Parameter: Cs_ctrl_temp_evap_spd_cmd_Y0
                                          //  Referenced by: '<S11>/Cs_ctrl_temp_evap_spd_cmd'

    float TevapRegulator_InitVal;      // Computed Parameter: TevapRegulator_InitVal
                                          //  Referenced by: '<S11>/TevapRegulator'

    float TevapRegulator_Kawu;         // Computed Parameter: TevapRegulator_Kawu
                                          //  Referenced by: '<S11>/TevapRegulator'

    float TevapRegulator_Kd;           // Computed Parameter: TevapRegulator_Kd
                                          //  Referenced by: '<S11>/TevapRegulator'

    float TevapRegulator_Ki;           // Computed Parameter: TevapRegulator_Ki
                                          //  Referenced by: '<S11>/TevapRegulator'

    float TevapRegulator_Kp;           // Computed Parameter: TevapRegulator_Kp
                                          //  Referenced by: '<S11>/TevapRegulator'

    float TevapRegulator_Tau_f;        // Computed Parameter: TevapRegulator_Tau_f
                                          //  Referenced by: '<S11>/TevapRegulator'

    float Cs_ctrl_tref_max_spd_cmd_Y0; // Computed Parameter: Cs_ctrl_tref_max_spd_cmd_Y0
                                          //  Referenced by: '<S12>/Cs_ctrl_tref_max_spd_cmd'

    float TevapRegulator_InitVal_d;    // Computed Parameter: TevapRegulator_InitVal_d
                                          //  Referenced by: '<S12>/TevapRegulator'

    float TevapRegulator_Kawu_d;       // Computed Parameter: TevapRegulator_Kawu_d
                                          //  Referenced by: '<S12>/TevapRegulator'

    float TevapRegulator_Kd_c;         // Computed Parameter: TevapRegulator_Kd_c
                                          //  Referenced by: '<S12>/TevapRegulator'

    float TevapRegulator_Ki_d;         // Computed Parameter: TevapRegulator_Ki_d
                                          //  Referenced by: '<S12>/TevapRegulator'

    float TevapRegulator_Kp_p;         // Computed Parameter: TevapRegulator_Kp_p
                                          //  Referenced by: '<S12>/TevapRegulator'

    float TevapRegulator_Tau_f_m;      // Computed Parameter: TevapRegulator_Tau_f_m
                                          //  Referenced by: '<S12>/TevapRegulator'

    float Saturation_UpperSat;         // Computed Parameter: Saturation_UpperSat
                                          //  Referenced by: '<S13>/Saturation'

    float Saturation_LowerSat;         // Computed Parameter: Saturation_LowerSat
                                          //  Referenced by: '<S13>/Saturation'

    float Saturation1_UpperSat;        // Computed Parameter: Saturation1_UpperSat
                                          //  Referenced by: '<S13>/Saturation1'

    float Saturation1_LowerSat;        // Computed Parameter: Saturation1_LowerSat
                                          //  Referenced by: '<S13>/Saturation1'

    float prevValue_InitialCondition;  // Computed Parameter: prevValue_InitialCondition
                                          //  Referenced by: '<S17>/prevValue'

    float Saturation1_UpperSat_b;      // Computed Parameter: Saturation1_UpperSat_b
                                          //  Referenced by: '<S17>/Saturation1'

    float Saturation1_LowerSat_h;      // Computed Parameter: Saturation1_LowerSat_h
                                          //  Referenced by: '<S17>/Saturation1'

    float Saturation2_UpperSat;        // Computed Parameter: Saturation2_UpperSat
                                          //  Referenced by: '<S17>/Saturation2'

    float Saturation2_LowerSat;        // Computed Parameter: Saturation2_LowerSat
                                          //  Referenced by: '<S17>/Saturation2'

    float lowCritDefrostProtection_Delay;// Computed Parameter: lowCritDefrostProtection_Delay
                                            //  Referenced by: '<S24>/lowCritDefrostProtection'

    float lowCritDefrostProtection_TimeSt;// Computed Parameter: lowCritDefrostProtection_TimeSt
                                             //  Referenced by: '<S24>/lowCritDefrostProtection'

    float highCritDefrostProtection_Delay;// Computed Parameter: highCritDefrostProtection_Delay
                                             //  Referenced by: '<S24>/highCritDefrostProtection'

    float highCritDefrostProtection_TimeS;// Computed Parameter: highCritDefrostProtection_TimeS
                                             //  Referenced by: '<S24>/highCritDefrostProtection'

    float evapTempProtReset_Delay;     // Computed Parameter: evapTempProtReset_Delay
                                          //  Referenced by: '<S25>/evapTempProtReset'

    float evapTempProtReset_TimeStep;  // Computed Parameter: evapTempProtReset_TimeStep
                                          //  Referenced by: '<S25>/evapTempProtReset'

    te_heat_stt BstrStopped_Value;     // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S1>/BstrStopped'

    te_heat_stt BstrHeating_Value;     // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S1>/BstrHeating'

    te_heat_stt HpStopped_Value;       // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S1>/HpStopped'

    te_heat_stt hpHeating_Value;       // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S1>/hpHeating'

    te_heat_stt heat_pump_stt_prev_InitialCondi;// Expression: te_heat_stt.Stopped
                                                   //  Referenced by: '<Root>/heat_pump_stt_prev'

    te_heat_stt Constant_Value;        // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S14>/Constant'

    te_on_off Constant3_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S3>/Constant3'

    te_on_off te_on_off_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S13>/te_on_off'

    te_on_off boosterOff_Value;        // Expression: te_on_off.off
                                          //  Referenced by: '<S1>/boosterOff'

    te_pump_mode pumpOFF_Value;        // Expression: te_pump_mode.pumpOFF
                                          //  Referenced by: '<S13>/pumpOFF'

    te_pump_mode TrefReg_Value;        // Expression: te_pump_mode.Tref
                                          //  Referenced by: '<S13>/TrefReg'

    te_pump_mode TevapReg_Value;       // Expression: te_pump_mode.Tevap
                                          //  Referenced by: '<S13>/TevapReg'

    te_pump_mode TankLevelReg_Value;   // Expression: te_pump_mode.Tank_lvl
                                          //  Referenced by: '<S13>/TankLevelReg'

    te_tank_size L180_Value;           // Expression: te_tank_size.L180
                                          //  Referenced by: '<S32>/L180'

    te_tank_size L180_Value_b;         // Expression: te_tank_size.L180
                                          //  Referenced by: '<S33>/L180'

    ta_temp Temp_2C_SetPointPID_Value; // Computed Parameter: Temp_2C_SetPointPID_Value
                                          //  Referenced by: '<S11>/Temp_-2°C_SetPointPID'

    ta_temp Constant2_Value;           // Expression: Cs_temp_ref_tol_C
                                          //  Referenced by: '<S12>/Constant2'

    ta_temp TimeToPumpStartup_bp01Data[6];// Expression: Ct_tank_down_temp_pump_off_bp_C
                                             //  Referenced by: '<S22>/TimeToPumpStartup'

    ta_temp Ct_180L_auto_3ppl_Value[4];// Computed Parameter: Ct_180L_auto_3ppl_Value
                                          //  Referenced by: '<S32>/Ct_180L_auto_3ppl'

    ta_temp Ct_180L_auto_4ppl_Value[4];// Computed Parameter: Ct_180L_auto_4ppl_Value
                                          //  Referenced by: '<S32>/Ct_180L_auto_4ppl'

    ta_temp Ct_180L_auto_5ppl_Value[4];// Computed Parameter: Ct_180L_auto_5ppl_Value
                                          //  Referenced by: '<S32>/Ct_180L_auto_5ppl'

    ta_temp Ct_180L_auto_6ppl_Value[4];// Computed Parameter: Ct_180L_auto_6ppl_Value
                                          //  Referenced by: '<S32>/Ct_180L_auto_6ppl'

    ta_temp Ct_105L_auto_3ppl_Value[4];// Computed Parameter: Ct_105L_auto_3ppl_Value
                                          //  Referenced by: '<S32>/Ct_105L_auto_3ppl'

    ta_temp Ct_105L_auto_4ppl_Value[4];// Computed Parameter: Ct_105L_auto_4ppl_Value
                                          //  Referenced by: '<S32>/Ct_105L_auto_4ppl'

    ta_temp Ct_105L_auto_5ppl_Value[4];// Computed Parameter: Ct_105L_auto_5ppl_Value
                                          //  Referenced by: '<S32>/Ct_105L_auto_5ppl'

    ta_temp Ct_105L_auto_6ppl_Value[4];// Computed Parameter: Ct_105L_auto_6ppl_Value
                                          //  Referenced by: '<S32>/Ct_105L_auto_6ppl'

    ta_temp Cs_temp_sys_max_Value;     // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S42>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Valu;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S42>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_thrs_min_Value;    // Expression: Cs_temp_thrs_min_C
                                          //  Referenced by: '<S42>/Cs_temp_thrs_min'

    ta_temp Cs_temp_sys_min_Value;     // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S42>/Cs_temp_sys_min'

    ta_temp Ct_180L_auto_2ppl_Value[4];// Computed Parameter: Ct_180L_auto_2ppl_Value
                                          //  Referenced by: '<S32>/Ct_180L_auto_2ppl'

    ta_temp Ct_105L_auto_2ppl_Value[4];// Computed Parameter: Ct_105L_auto_2ppl_Value
                                          //  Referenced by: '<S32>/Ct_105L_auto_2ppl'

    ta_temp Cs_temp_sys_max_Value_o;   // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S47>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Va_m;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S47>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_thrs_min_Value_h;  // Expression: Cs_temp_thrs_min_C
                                          //  Referenced by: '<S47>/Cs_temp_thrs_min'

    ta_temp Cs_temp_sys_min_C_Value;   // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S47>/Cs_temp_sys_min_C'

    ta_temp Ct_180L_eco_3ppl_Value[4]; // Expression: Ct_180L_eco_3ppl_C
                                          //  Referenced by: '<S33>/Ct_180L_eco_3ppl'

    ta_temp Ct_180L_eco_6ppl_Value[4]; // Expression: Ct_180L_eco_6ppl_C
                                          //  Referenced by: '<S33>/Ct_180L_eco_6ppl'

    ta_temp Ct_105L_eco_3ppl_Value[4]; // Expression: Ct_105L_eco_3ppl_C
                                          //  Referenced by: '<S33>/Ct_105L_eco_3ppl'

    ta_temp Cs_temp_sys_max_Value_ot;  // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S62>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Va_p;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S62>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_sys_min_Value_h;   // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S62>/Cs_temp_sys_min'

    ta_temp Ct_hyb_temp_thrs_Value[4]; // Computed Parameter: Ct_hyb_temp_thrs_Value
                                          //  Referenced by: '<S36>/Ct_hyb_temp_thrs'

    ta_temp Cs_temp_sys_max_Value_c;   // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S37>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Va_g;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S37>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_thrs_min_Value_c;  // Expression: Cs_temp_thrs_min_C
                                          //  Referenced by: '<S37>/Cs_temp_thrs_min'

    ta_temp Cs_temp_sys_min_Value_i;   // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S37>/Cs_temp_sys_min'

    ta_temp Ct_anti_lgn_temp_thrs_Value[4];// Computed Parameter: Ct_anti_lgn_temp_thrs_Value
                                              //  Referenced by: '<S31>/Ct_anti_lgn_temp_thrs'

    ta_temp Cs_temp_sys_max_Value_ch;  // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S57>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Va_c;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S57>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_sys_min_Value_j;   // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S57>/Cs_temp_sys_min'

    ta_temp Ct_hldy_temp_thrs_Value[4];// Computed Parameter: Ct_hldy_temp_thrs_Value
                                          //  Referenced by: '<S35>/Ct_hldy_temp_thrs'

    ta_temp Cs_temp_sys_max_Value_e;   // Expression: Cs_temp_sys_max_C
                                          //  Referenced by: '<S52>/Cs_temp_sys_max'

    ta_temp Cs_temp_heat_pump_temp_max_Va_a;// Expression: Cs_temp_heat_pump_temp_max_C
                                               //  Referenced by: '<S52>/Cs_temp_heat_pump_temp_max'

    ta_temp Cs_temp_thrs_min_Value_l;  // Expression: Cs_temp_thrs_min_C
                                          //  Referenced by: '<S52>/Cs_temp_thrs_min'

    ta_temp Cs_temp_sys_min_Value_it;  // Expression: Cs_temp_sys_min_C
                                          //  Referenced by: '<S52>/Cs_temp_sys_min'

    ta_temp Ct_heat_pump_test_temp_thrs_Val[4];// Computed Parameter: Ct_heat_pump_test_temp_thrs_Val
                                                  //  Referenced by: '<S34>/Ct_heat_pump_test_temp_thrs'

    ta_temp Cs_heat_pump_lim_func_l_Value;// Expression: Cs_heat_pump_lim_func_l_C
                                             //  Referenced by: '<S18>/Cs_heat_pump_lim_func_l'

    ta_temp Cs_heat_pump_lim_func_h_Value;// Expression: Cs_heat_pump_lim_func_h_C
                                             //  Referenced by: '<S18>/Cs_heat_pump_lim_func_h'

    ta_temp Constant4_Value;           // Expression: Cs_hpc_low_crit_dfr_prot_temp_C
                                          //  Referenced by: '<S24>/Constant4'

    ta_temp Constant1_Value;           // Expression: Cs_hpc_high_crit_dfr_prot_temp_C
                                          //  Referenced by: '<S24>/Constant1'

    ta_temp Tref_max_tableData[7];     // Expression: Ct_temp_ref_max_data_C
                                          //  Referenced by: '<S19>/Tref_max'

    ta_temp Tref_max_bp01Data[7];      // Expression: Ct_temp_evap_bp_C
                                          //  Referenced by: '<S19>/Tref_max'

    ta_temp Cs_hpc_dfr_prot_temp_Value;// Expression: Cs_hpc_dfr_prot_temp_l_C
                                          //  Referenced by: '<S25>/Cs_hpc_dfr_prot_temp'

    int16_t Cs_hpc_dfr_prot_temp_PLUS_1_Val;// Expression: Cs_hpc_dfr_prot_temp_l_C + 1
                                               //  Referenced by: '<S25>/Cs_hpc_dfr_prot_temp_PLUS_1'

    ta_temp CartoSetPointTankCold_bp01Data[8];// Expression: Ct_temp_tank_cold_sp_bp_C
                                                 //  Referenced by: '<S15>/CartoSetPointTankCold'

    ta_rot_spd max_Value;              // Computed Parameter: max_Value
                                          //  Referenced by: '<S11>/max'

    ta_rot_spd maxDerat_Value;         // Computed Parameter: maxDerat_Value
                                          //  Referenced by: '<S11>/maxDerat'

    ta_rot_spd minDerat_Value;         // Computed Parameter: minDerat_Value
                                          //  Referenced by: '<S11>/minDerat'

    ta_rot_spd maxDerat_Value_a;       // Computed Parameter: maxDerat_Value_a
                                          //  Referenced by: '<S12>/maxDerat'

    ta_rot_spd minDerat_Value_f;       // Computed Parameter: minDerat_Value_f
                                          //  Referenced by: '<S12>/minDerat'

    ta_rot_spd max_Value_m;            // Computed Parameter: max_Value_m
                                          //  Referenced by: '<S12>/max'

    ta_rot_spd highLoadRotSpeedMax_Value;// Expression: Cs_pump_high_load_prot_rot_spd_max_C
                                            //  Referenced by: '<S13>/highLoadRotSpeedMax'

    ta_rot_spd SpdOFF_Value;           // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S13>/SpdOFF'

    ta_rot_spd highPresRotSpeedMax_Value;// Expression: Cs_pump_high_pres_prot_rot_spd_max_C
                                            //  Referenced by: '<S13>/highPresRotSpeedMax'

    uint16_t TimeToPumpStartup_tableData[6];// Expression: Ct_time_cntr_pump_off_data_C
                                               //  Referenced by: '<S22>/TimeToPumpStartup'

    ta_rot_spd SpdMax_Value;           // Expression: Cs_rot_spd_pump_max_C
                                          //  Referenced by: '<S15>/SpdMax'

    ta_rot_spd SpdOFF1_Value;          // Expression: Cs_rot_spd_pump_off_C
                                          //  Referenced by: '<S15>/SpdOFF1'

    ta_rot_spd minRotSpdSp_Value;      // Expression: Cs_rot_spd_pump_min_C
                                          //  Referenced by: '<S13>/minRotSpdSp'

    uint16_t Cs_anti_shrt_cycl_dly_prev_Init;// Computed Parameter: Cs_anti_shrt_cycl_dly_prev_Init
                                                //  Referenced by: '<S22>/Cs_anti_shrt_cycl_dly_prev'

    ta_rot_spd Cs_rot_spd_pump_max_C_Value;// Expression: Cs_rot_spd_pump_max_C
                                              //  Referenced by: '<S15>/Cs_rot_spd_pump_max_C'

    ta_rot_spd Cs_rot_spd_pump_min_C_Value;// Expression: Cs_rot_spd_pump_min_C
                                              //  Referenced by: '<S15>/Cs_rot_spd_pump_min_C'

    ta_rot_spd CartoSetPointTankCold_tableData[8];// Expression: Ct_rpm_pump_sp_tank_cold_C
                                                     //  Referenced by: '<S15>/CartoSetPointTankCold'

    ta_rot_spd nullSpeed_Value;        // Computed Parameter: nullSpeed_Value
                                          //  Referenced by: '<S1>/nullSpeed'

    bool exhstTempProt_InitialCondition;// Expression: false
                                           //  Referenced by: '<S19>/exhstTempProt'

  };

  // Initial conditions function
  void init();

  // model step function
  void output(const tb_WaterHeatCtrl_In *rtu_WaterHeatCtrl_In, tb_WaterHeatCtrl_Out *rty_WaterHeatCtrl_Out);

  // Copy Constructor
  WaterHeatCtrl(WaterHeatCtrl const&) = delete;

  // Assignment Operator
  WaterHeatCtrl& operator= (WaterHeatCtrl const&) & = delete;

  // Move Constructor
  WaterHeatCtrl(WaterHeatCtrl &&) = delete;

  // Move Assignment Operator
  WaterHeatCtrl& operator= (WaterHeatCtrl &&) = delete;

  // Reset function
  void reset();

  // Constructor
  WaterHeatCtrl();

  // Destructor
  ~WaterHeatCtrl();

  // private data and function members
 private:
  // Block states
  DW_WaterHeatCtrl_T WaterHeatCtrl_DW;

  // Tunable parameters
  static P_WaterHeatCtrl_T WaterHeatCtrl_rtP;

  // Declare private class scope variables for system: "model 'WaterHeatCtrl'"
  const tb_WaterHeatCtrl_In *WaterHeatC_rtu_WaterHeatCtrl_In;// '<Root>/WaterHeatCtrl_In'

  // private member function(s) for subsystem '<Root>/ModeThresholdTempCalc'
  void WaterHeat_ModeThresholdTempCalc();

  // model instance variable for '<S10>/Derivator'
  Derivator DerivatorMDLOBJ1;

  // model instance variable for '<S11>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ2;

  // model instance variable for '<S12>/TevapRegulator'
  PID_backCalc TevapRegulatorMDLOBJ3;

  // model instance variable for '<S17>/rateSaturation'
  cntrlSaturation rateSaturationMDLOBJ4;

  // model instance variable for '<S22>/AntiShortCycle'
  VariableRisingEdgeDelay AntiShortCycleMDLOBJ5;

  // model instance variable for '<S22>/PumpOFFdetect'
  RisingEdgeDetect PumpOFFdetectMDLOBJ6;

  // model instance variable for '<S24>/highCritDefrostProtection'
  RisingEdgeDelay highCritDefrostProteMDLOBJ7;

  // model instance variable for '<S24>/lowCritDefrostProtection'
  RisingEdgeDelay lowCritDefrostProtecMDLOBJ8;

  // model instance variable for '<S26>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ9;

  // model instance variable for '<S25>/evapTempProtReset'
  RisingEdgeDelay evapTempProtResetMDLOBJ10;

  // model instance variable for '<S27>/Saturation'
  cntrlSaturation SaturationMDLOBJ11;

  // model instance variable for '<S28>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ12;

  // model instance variable for '<S38>/Saturation'
  cntrlSaturation SaturationMDLOBJ13;

  // model instance variable for '<S39>/Saturation'
  cntrlSaturation SaturationMDLOBJ14;

  // model instance variable for '<S40>/Saturation'
  cntrlSaturation SaturationMDLOBJ15;

  // model instance variable for '<S41>/Saturation'
  cntrlSaturation SaturationMDLOBJ16;

  // model instance variable for '<S43>/Saturation'
  cntrlSaturation SaturationMDLOBJ17;

  // model instance variable for '<S44>/Saturation'
  cntrlSaturation SaturationMDLOBJ18;

  // model instance variable for '<S45>/Saturation'
  cntrlSaturation SaturationMDLOBJ19;

  // model instance variable for '<S46>/Saturation'
  cntrlSaturation SaturationMDLOBJ20;

  // model instance variable for '<S48>/Saturation'
  cntrlSaturation SaturationMDLOBJ21;

  // model instance variable for '<S49>/Saturation'
  cntrlSaturation SaturationMDLOBJ22;

  // model instance variable for '<S50>/Saturation'
  cntrlSaturation SaturationMDLOBJ23;

  // model instance variable for '<S51>/Saturation'
  cntrlSaturation SaturationMDLOBJ24;

  // model instance variable for '<S53>/Saturation'
  cntrlSaturation SaturationMDLOBJ25;

  // model instance variable for '<S54>/Saturation'
  cntrlSaturation SaturationMDLOBJ26;

  // model instance variable for '<S55>/Saturation'
  cntrlSaturation SaturationMDLOBJ27;

  // model instance variable for '<S56>/Saturation'
  cntrlSaturation SaturationMDLOBJ28;

  // model instance variable for '<S58>/Saturation'
  cntrlSaturation SaturationMDLOBJ29;

  // model instance variable for '<S59>/Saturation'
  cntrlSaturation SaturationMDLOBJ30;

  // model instance variable for '<S60>/Saturation'
  cntrlSaturation SaturationMDLOBJ31;

  // model instance variable for '<S61>/Saturation'
  cntrlSaturation SaturationMDLOBJ32;

  // model instance variable for '<S63>/Saturation'
  cntrlSaturation SaturationMDLOBJ33;

  // model instance variable for '<S64>/Saturation'
  cntrlSaturation SaturationMDLOBJ34;

  // model instance variable for '<S65>/Saturation'
  cntrlSaturation SaturationMDLOBJ35;

  // model instance variable for '<S66>/Saturation'
  cntrlSaturation SaturationMDLOBJ36;

  // model instance variable for '<S67>/Hysteresis'
  Hysteresis HysteresisMDLOBJ37;
};

extern WaterHeatCtrl::P_WaterHeatCtrl_T WaterHeatCtrl_rtP;

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
//  '<Root>' : 'WaterHeatCtrl'
//  '<S1>'   : 'WaterHeatCtrl/ActuatorStateMng'
//  '<S2>'   : 'WaterHeatCtrl/DrawDetection'
//  '<S3>'   : 'WaterHeatCtrl/ElectricHeaterBoosterMng'
//  '<S4>'   : 'WaterHeatCtrl/HeatPumpMng'
//  '<S5>'   : 'WaterHeatCtrl/ModeThresholdTempCalc'
//  '<S6>'   : 'WaterHeatCtrl/tankIcingProtectionDetect'
//  '<S7>'   : 'WaterHeatCtrl/DrawDetection/DerivativeCalc'
//  '<S8>'   : 'WaterHeatCtrl/DrawDetection/DerivativeCalc/Chart'
//  '<S9>'   : 'WaterHeatCtrl/DrawDetection/DerivativeCalc/TempVarWatch'
//  '<S10>'  : 'WaterHeatCtrl/DrawDetection/DerivativeCalc/TempVarWatch/Derivator'
//  '<S11>'  : 'WaterHeatCtrl/HeatPumpMng/CntrlEvapPump'
//  '<S12>'  : 'WaterHeatCtrl/HeatPumpMng/CntrlProtectPump'
//  '<S13>'  : 'WaterHeatCtrl/HeatPumpMng/HeatPumpCore'
//  '<S14>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection'
//  '<S15>'  : 'WaterHeatCtrl/HeatPumpMng/TankLevelManagement'
//  '<S16>'  : 'WaterHeatCtrl/HeatPumpMng/tank levels indicators'
//  '<S17>'  : 'WaterHeatCtrl/HeatPumpMng/HeatPumpCore/specificRateLimiter'
//  '<S18>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/AirTempProtectionDetect'
//  '<S19>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/CompressorProtectionDetect'
//  '<S20>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/HighCompressorLoadProtectionDetect'
//  '<S21>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/HighCompressorPressureProtectionDetect'
//  '<S22>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/antiShortCycleProtectionDetect'
//  '<S23>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/fluidIcingProtectionDetect'
//  '<S24>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/fluidIcingProtectionDetect/criticalIcingProtection'
//  '<S25>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/fluidIcingProtectionDetect/icingProtection'
//  '<S26>'  : 'WaterHeatCtrl/HeatPumpMng/ProtectionDetection/fluidIcingProtectionDetect/icingProtection/TevapProtState'
//  '<S27>'  : 'WaterHeatCtrl/HeatPumpMng/TankLevelManagement/Pump_rpm_sat'
//  '<S28>'  : 'WaterHeatCtrl/HeatPumpMng/TankLevelManagement/filterSp1'
//  '<S29>'  : 'WaterHeatCtrl/HeatPumpMng/tank levels indicators/TankLevelStates'
//  '<S30>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc'
//  '<S31>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold'
//  '<S32>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold'
//  '<S33>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold'
//  '<S34>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold'
//  '<S35>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold'
//  '<S36>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold'
//  '<S37>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold/TemperatureThresholdSaturation'
//  '<S38>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold/TemperatureThresholdSaturation/Saturation1'
//  '<S39>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold/TemperatureThresholdSaturation/Saturation2'
//  '<S40>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold/TemperatureThresholdSaturation/Saturation3'
//  '<S41>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/antiLegionnellaTemperatureThreshold/TemperatureThresholdSaturation/Saturation4'
//  '<S42>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold/AutoTemperatureThresholdSaturation'
//  '<S43>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold/AutoTemperatureThresholdSaturation/Saturation1'
//  '<S44>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold/AutoTemperatureThresholdSaturation/Saturation2'
//  '<S45>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold/AutoTemperatureThresholdSaturation/Saturation3'
//  '<S46>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/autoTemperatureThreshold/AutoTemperatureThresholdSaturation/Saturation4'
//  '<S47>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold/TemperatureThresholdSaturation'
//  '<S48>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold/TemperatureThresholdSaturation/Saturation5'
//  '<S49>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold/TemperatureThresholdSaturation/Saturation6'
//  '<S50>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold/TemperatureThresholdSaturation/Saturation7'
//  '<S51>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/ecoTemperatureThreshold/TemperatureThresholdSaturation/Saturation8'
//  '<S52>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold/heatPumpTemperatureThresholdSaturation'
//  '<S53>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold/heatPumpTemperatureThresholdSaturation/Saturation1'
//  '<S54>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold/heatPumpTemperatureThresholdSaturation/Saturation2'
//  '<S55>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold/heatPumpTemperatureThresholdSaturation/Saturation3'
//  '<S56>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/heatPumpTemperatureThreshold/heatPumpTemperatureThresholdSaturation/Saturation4'
//  '<S57>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold/HolidaysTemperatureThresholdSaturation'
//  '<S58>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold/HolidaysTemperatureThresholdSaturation/Saturation1'
//  '<S59>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold/HolidaysTemperatureThresholdSaturation/Saturation2'
//  '<S60>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold/HolidaysTemperatureThresholdSaturation/Saturation3'
//  '<S61>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/holidaysTemperatureThreshold/HolidaysTemperatureThresholdSaturation/Saturation4'
//  '<S62>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold/HybridTemperatureThresholdSaturation'
//  '<S63>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold/HybridTemperatureThresholdSaturation/Saturation1'
//  '<S64>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold/HybridTemperatureThresholdSaturation/Saturation2'
//  '<S65>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold/HybridTemperatureThresholdSaturation/Saturation3'
//  '<S66>'  : 'WaterHeatCtrl/ModeThresholdTempCalc/ThresholdCalc/hybridTemperatureThreshold/HybridTemperatureThresholdSaturation/Saturation4'
//  '<S67>'  : 'WaterHeatCtrl/tankIcingProtectionDetect/HysteresisProtectTankDefrost'

#endif                                 // WaterHeatCtrl_h_

//
// File trailer for generated code.
//
// [EOF]
//
