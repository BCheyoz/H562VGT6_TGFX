//
// File: InPutMng.h
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.320
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 12:15:58 2025
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
#ifndef InPutMng_h_
#define InPutMng_h_
#include <stdbool.h>
#include <stdint.h>
#include "InPutMng_types.h"
#include "SensErrorDetection.h"
#include "firstOrderTF.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_input_mng_tick;     // Variable: input_mng_tick
                                          //  Referenced by:
                                          //    '<S22>/firstOrderTF'
                                          //    '<S23>/firstOrderTF'
                                          //    '<S24>/firstOrderTF'
                                          //    '<S25>/firstOrderTF'
                                          //    '<S26>/firstOrderTF'
                                          //    '<S27>/firstOrderTF'
                                          //    '<S28>/firstOrderTF'
                                          //    '<S32>/SensErrorDetection'
                                          //    '<S33>/SensErrorDetection'
                                          //    '<S34>/SensErrorDetection'
                                          //    '<S35>/SensErrorDetection'
                                          //    '<S36>/SensErrorDetection'
                                          //    '<S37>/SensErrorDetection'
                                          //    '<S38>/SensErrorDetection'


// Class declaration for model InPutMng
class InPutMng final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'InPutMng'
  struct DW_InPutMng_T {
    float divideByTrefMinusTin;        // '<S16>/divideByTrefMinusTin'
    float Vref;                        // '<S15>/getVref'
    float Vol;                         // '<S19>/getVol'
    float getTemp;                     // '<S19>/getTemp'
    float Vol_i;                       // '<S18>/getVol'
    float getTemp_h;                   // '<S18>/getTemp'
    float Vol_o;                       // '<S17>/getVol'
    float getTemp_c;                   // '<S17>/getTemp'
    float V_ref_max;                   // '<S14>/divideByTrefMinusTin'
    float TempToSing3;                 // '<S13>/TempToSing3'
    float TempToSing4;                 // '<S13>/TempToSing4'
    float TempToSing2;                 // '<S13>/TempToSing2'
    float Tmax;                        // '<S13>/Switch'
    float TempToSing1;                 // '<S13>/TempToSing1'
    float Vb_Switch;                   // '<S13>/Vb_Switch'
    float Vh_Switch;                   // '<S13>/Vh_Switch'
    float VolToSing1;                  // '<S13>/VolToSing1'
    float TmaxPrev_DSTATE;             // '<S13>/TmaxPrev'
    ta_pwr AddCnsp_g;                  // '<S7>/AddCnsp'
    ta_pwr resetSwitch;                // '<S6>/resetSwitch'
    ta_pwr Cs_elec_htr_bstr_cnsp_prev_DSTA;// '<S8>/Cs_elec_htr_bstr_cnsp_prev'
    ta_pwr Cs_vent_cnsp_prev_DSTATE;   // '<S9>/Cs_vent_cnsp_prev'
    ta_pwr Cs_elec_htr_bstr_cnsp_prev_DS_g;// '<S6>/Cs_elec_htr_bstr_cnsp_prev'
    ta_temp Cs_tank_down_temp_raw;
    ta_temp Cs_tank_up_temp_raw;
    ta_temp Cs_pump_xhst_temp_raw;
    ta_temp Cs_pump_evap_temp_raw;
    ta_temp Cs_vent_temp_raw;
    ta_temp Cs_pump_evap_temp;         // '<Root>/SimuSwitch3'
    ta_temp Cs_tank_down_temp;         // '<Root>/SimuSwitch'
    ta_temp Cs_tank_up_temp;           // '<Root>/SimuSwitch1'
    ta_temp Cs_tank_down_temp_cs;      // '<S31>/mergeTempDown'
    ta_temp Cs_tank_up_temp_cs;        // '<S31>/mergeTempUp'
    ta_temp Cs_pump_xhst_temp_cs;      // '<S31>/mergeXhstTemp'
    ta_temp Cs_pump_evap_temp_cs;      // '<S31>/mergeEvapTemp'
    ta_temp Cs_vent_temp_cs;           // '<S31>/mergeVentTemp'
    ta_temp Cs_tank_down_temp_e;       // '<S20>/SingToTemp'
    ta_temp Cs_tank_up_temp_g;         // '<S20>/SingToTemp1'
    ta_temp Cs_pump_xhst_temp;         // '<S20>/SingToTemp2'
    ta_temp Cs_pump_evap_temp_l;       // '<S20>/SingToTemp3'
    ta_temp Cs_vent_temp;              // '<S20>/SingToTemp4'
    ta_temp BpEsti;                    // '<S2>/BpEsti'
    ta_air_pres Cs_vent_pres_raw;
    ta_rot_spd Cs_vent_rot_spd_raw;
    ta_air_pres Cs_pres_vent_cs;       // '<S31>/mergePresVent'
    ta_rot_spd Cs_vent_rot_spd_cs;     // '<S31>/mergeRotSpd'
    ta_air_pres Cs_vent_pres;          // '<S20>/SingToPres'
    ta_rot_spd Cs_vent_rot_spd;        // '<S20>/SingToRotSpd'
    ta_wtr_vol Cs_v40_min;             // '<S4>/SingToVol1'
    bool mergeErr[7];                  // '<S31>/mergeErr'
    bool Bs_tank_down_temp_err;        // '<Root>/SimuSwitch7'
    bool Bs_tank_up_temp_err;          // '<Root>/SimuSwitch8'
  };

  // Parameters (default storage)
  struct P_InPutMng_T {
    float FTempDown_K;                 // Mask Parameter: FTempDown_K
                                          //  Referenced by: '<S24>/firstOrderTF'

    float FTempUp_K;                   // Mask Parameter: FTempUp_K
                                          //  Referenced by: '<S26>/firstOrderTF'

    float FTempXhst_K;                 // Mask Parameter: FTempXhst_K
                                          //  Referenced by: '<S28>/firstOrderTF'

    float FTempEvap_K;                 // Mask Parameter: FTempEvap_K
                                          //  Referenced by: '<S25>/firstOrderTF'

    float FTempVent_K;                 // Mask Parameter: FTempVent_K
                                          //  Referenced by: '<S27>/firstOrderTF'

    float FPresVent_K;                 // Mask Parameter: FPresVent_K
                                          //  Referenced by: '<S22>/firstOrderTF'

    float FRotSpd_K;                   // Mask Parameter: FRotSpd_K
                                          //  Referenced by: '<S23>/firstOrderTF'

    float FTempDown_Tau;               // Mask Parameter: FTempDown_Tau
                                          //  Referenced by: '<S24>/firstOrderTF'

    float FTempUp_Tau;                 // Mask Parameter: FTempUp_Tau
                                          //  Referenced by: '<S26>/firstOrderTF'

    float FTempXhst_Tau;               // Mask Parameter: FTempXhst_Tau
                                          //  Referenced by: '<S28>/firstOrderTF'

    float FTempEvap_Tau;               // Mask Parameter: FTempEvap_Tau
                                          //  Referenced by: '<S25>/firstOrderTF'

    float FTempVent_Tau;               // Mask Parameter: FTempVent_Tau
                                          //  Referenced by: '<S27>/firstOrderTF'

    float FPresVent_Tau;               // Mask Parameter: FPresVent_Tau
                                          //  Referenced by: '<S22>/firstOrderTF'

    float FRotSpd_Tau;                 // Mask Parameter: FRotSpd_Tau
                                          //  Referenced by: '<S23>/firstOrderTF'

    float ErrTempDown_defVal;          // Mask Parameter: ErrTempDown_defVal
                                          //  Referenced by: '<S34>/SensErrorDetection'

    float ErrTempUp_defVal;            // Mask Parameter: ErrTempUp_defVal
                                          //  Referenced by: '<S36>/SensErrorDetection'

    float ErrTempXhst_defVal;          // Mask Parameter: ErrTempXhst_defVal
                                          //  Referenced by: '<S38>/SensErrorDetection'

    float ErrTempEvap_defVal;          // Mask Parameter: ErrTempEvap_defVal
                                          //  Referenced by: '<S35>/SensErrorDetection'

    float ErrTempVent_defVal;          // Mask Parameter: ErrTempVent_defVal
                                          //  Referenced by: '<S37>/SensErrorDetection'

    float ErrPresVent_defVal;          // Mask Parameter: ErrPresVent_defVal
                                          //  Referenced by: '<S32>/SensErrorDetection'

    float ErrRotSpd_defVal;            // Mask Parameter: ErrRotSpd_defVal
                                          //  Referenced by: '<S33>/SensErrorDetection'

    float FTempDown_initVal;           // Mask Parameter: FTempDown_initVal
                                          //  Referenced by: '<S24>/firstOrderTF'

    float FTempUp_initVal;             // Mask Parameter: FTempUp_initVal
                                          //  Referenced by: '<S26>/firstOrderTF'

    float FTempXhst_initVal;           // Mask Parameter: FTempXhst_initVal
                                          //  Referenced by: '<S28>/firstOrderTF'

    float FTempEvap_initVal;           // Mask Parameter: FTempEvap_initVal
                                          //  Referenced by: '<S25>/firstOrderTF'

    float FTempVent_initVal;           // Mask Parameter: FTempVent_initVal
                                          //  Referenced by: '<S27>/firstOrderTF'

    float FPresVent_initVal;           // Mask Parameter: FPresVent_initVal
                                          //  Referenced by: '<S22>/firstOrderTF'

    float FRotSpd_initVal;             // Mask Parameter: FRotSpd_initVal
                                          //  Referenced by: '<S23>/firstOrderTF'

    float ErrTempDown_staticRstTol;    // Mask Parameter: ErrTempDown_staticRstTol
                                          //  Referenced by: '<S34>/SensErrorDetection'

    float ErrTempUp_staticRstTol;      // Mask Parameter: ErrTempUp_staticRstTol
                                          //  Referenced by: '<S36>/SensErrorDetection'

    float ErrTempXhst_staticRstTol;    // Mask Parameter: ErrTempXhst_staticRstTol
                                          //  Referenced by: '<S38>/SensErrorDetection'

    float ErrTempEvap_staticRstTol;    // Mask Parameter: ErrTempEvap_staticRstTol
                                          //  Referenced by: '<S35>/SensErrorDetection'

    float ErrTempVent_staticRstTol;    // Mask Parameter: ErrTempVent_staticRstTol
                                          //  Referenced by: '<S37>/SensErrorDetection'

    float ErrPresVent_staticRstTol;    // Mask Parameter: ErrPresVent_staticRstTol
                                          //  Referenced by: '<S32>/SensErrorDetection'

    float ErrRotSpd_staticRstTol;      // Mask Parameter: ErrRotSpd_staticRstTol
                                          //  Referenced by: '<S33>/SensErrorDetection'

    ta_temp ErrTempDown_maxThrs;       // Mask Parameter: ErrTempDown_maxThrs
                                          //  Referenced by: '<S34>/SensErrorDetection'

    ta_temp ErrTempUp_maxThrs;         // Mask Parameter: ErrTempUp_maxThrs
                                          //  Referenced by: '<S36>/SensErrorDetection'

    ta_temp ErrTempXhst_maxThrs;       // Mask Parameter: ErrTempXhst_maxThrs
                                          //  Referenced by: '<S38>/SensErrorDetection'

    ta_temp ErrTempEvap_maxThrs;       // Mask Parameter: ErrTempEvap_maxThrs
                                          //  Referenced by: '<S35>/SensErrorDetection'

    ta_temp ErrTempVent_maxThrs;       // Mask Parameter: ErrTempVent_maxThrs
                                          //  Referenced by: '<S37>/SensErrorDetection'

    ta_temp ErrTempDown_maxVar;        // Mask Parameter: ErrTempDown_maxVar
                                          //  Referenced by: '<S34>/SensErrorDetection'

    ta_temp ErrTempUp_maxVar;          // Mask Parameter: ErrTempUp_maxVar
                                          //  Referenced by: '<S36>/SensErrorDetection'

    ta_temp ErrTempXhst_maxVar;        // Mask Parameter: ErrTempXhst_maxVar
                                          //  Referenced by: '<S38>/SensErrorDetection'

    ta_temp ErrTempEvap_maxVar;        // Mask Parameter: ErrTempEvap_maxVar
                                          //  Referenced by: '<S35>/SensErrorDetection'

    ta_temp ErrTempVent_maxVar;        // Mask Parameter: ErrTempVent_maxVar
                                          //  Referenced by: '<S37>/SensErrorDetection'

    ta_temp ErrTempDown_minThrs;       // Mask Parameter: ErrTempDown_minThrs
                                          //  Referenced by: '<S34>/SensErrorDetection'

    ta_temp ErrTempUp_minThrs;         // Mask Parameter: ErrTempUp_minThrs
                                          //  Referenced by: '<S36>/SensErrorDetection'

    ta_temp ErrTempXhst_minThrs;       // Mask Parameter: ErrTempXhst_minThrs
                                          //  Referenced by: '<S38>/SensErrorDetection'

    ta_temp ErrTempEvap_minThrs;       // Mask Parameter: ErrTempEvap_minThrs
                                          //  Referenced by: '<S35>/SensErrorDetection'

    ta_temp ErrTempVent_minThrs;       // Mask Parameter: ErrTempVent_minThrs
                                          //  Referenced by: '<S37>/SensErrorDetection'

    ta_air_pres ErrPresVent_maxThrs;   // Mask Parameter: ErrPresVent_maxThrs
                                          //  Referenced by: '<S32>/SensErrorDetection'

    ta_rot_spd ErrRotSpd_maxThrs;      // Mask Parameter: ErrRotSpd_maxThrs
                                          //  Referenced by: '<S33>/SensErrorDetection'

    ta_air_pres ErrPresVent_maxVar;    // Mask Parameter: ErrPresVent_maxVar
                                          //  Referenced by: '<S32>/SensErrorDetection'

    ta_rot_spd ErrRotSpd_maxVar;       // Mask Parameter: ErrRotSpd_maxVar
                                          //  Referenced by: '<S33>/SensErrorDetection'

    ta_air_pres ErrPresVent_minThrs;   // Mask Parameter: ErrPresVent_minThrs
                                          //  Referenced by: '<S32>/SensErrorDetection'

    ta_rot_spd ErrRotSpd_minThrs;      // Mask Parameter: ErrRotSpd_minThrs
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_opScaleRstTime;// Mask Parameter: ErrTempDown_opScaleRstTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_opScaleRstTime;  // Mask Parameter: ErrTempUp_opScaleRstTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_opScaleRstTime;// Mask Parameter: ErrTempXhst_opScaleRstTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_opScaleRstTime;// Mask Parameter: ErrTempEvap_opScaleRstTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_opScaleRstTime;// Mask Parameter: ErrTempVent_opScaleRstTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_opScaleRstTime;// Mask Parameter: ErrPresVent_opScaleRstTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_opScaleRstTime;  // Mask Parameter: ErrRotSpd_opScaleRstTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_opScaleSetTime;// Mask Parameter: ErrTempDown_opScaleSetTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_opScaleSetTime;  // Mask Parameter: ErrTempUp_opScaleSetTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_opScaleSetTime;// Mask Parameter: ErrTempXhst_opScaleSetTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_opScaleSetTime;// Mask Parameter: ErrTempEvap_opScaleSetTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_opScaleSetTime;// Mask Parameter: ErrTempVent_opScaleSetTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_opScaleSetTime;// Mask Parameter: ErrPresVent_opScaleSetTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_opScaleSetTime;  // Mask Parameter: ErrRotSpd_opScaleSetTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_staticRstTime; // Mask Parameter: ErrTempDown_staticRstTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_staticRstTime;   // Mask Parameter: ErrTempUp_staticRstTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_staticRstTime; // Mask Parameter: ErrTempXhst_staticRstTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_staticRstTime; // Mask Parameter: ErrTempEvap_staticRstTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_staticRstTime; // Mask Parameter: ErrTempVent_staticRstTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_staticRstTime; // Mask Parameter: ErrPresVent_staticRstTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_staticRstTime;   // Mask Parameter: ErrRotSpd_staticRstTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_staticSetTime; // Mask Parameter: ErrTempDown_staticSetTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_staticSetTime;   // Mask Parameter: ErrTempUp_staticSetTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_staticSetTime; // Mask Parameter: ErrTempXhst_staticSetTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_staticSetTime; // Mask Parameter: ErrTempEvap_staticSetTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_staticSetTime; // Mask Parameter: ErrTempVent_staticSetTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_staticSetTime; // Mask Parameter: ErrPresVent_staticSetTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_staticSetTime;   // Mask Parameter: ErrRotSpd_staticSetTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_varRstTime;    // Mask Parameter: ErrTempDown_varRstTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_varRstTime;      // Mask Parameter: ErrTempUp_varRstTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_varRstTime;    // Mask Parameter: ErrTempXhst_varRstTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_varRstTime;    // Mask Parameter: ErrTempEvap_varRstTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_varRstTime;    // Mask Parameter: ErrTempVent_varRstTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_varRstTime;    // Mask Parameter: ErrPresVent_varRstTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_varRstTime;      // Mask Parameter: ErrRotSpd_varRstTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    uint8_t ErrTempDown_varSetTime;    // Mask Parameter: ErrTempDown_varSetTime
                                          //  Referenced by: '<S34>/SensErrorDetection'

    uint8_t ErrTempUp_varSetTime;      // Mask Parameter: ErrTempUp_varSetTime
                                          //  Referenced by: '<S36>/SensErrorDetection'

    uint8_t ErrTempXhst_varSetTime;    // Mask Parameter: ErrTempXhst_varSetTime
                                          //  Referenced by: '<S38>/SensErrorDetection'

    uint8_t ErrTempEvap_varSetTime;    // Mask Parameter: ErrTempEvap_varSetTime
                                          //  Referenced by: '<S35>/SensErrorDetection'

    uint8_t ErrTempVent_varSetTime;    // Mask Parameter: ErrTempVent_varSetTime
                                          //  Referenced by: '<S37>/SensErrorDetection'

    uint8_t ErrPresVent_varSetTime;    // Mask Parameter: ErrPresVent_varSetTime
                                          //  Referenced by: '<S32>/SensErrorDetection'

    uint8_t ErrRotSpd_varSetTime;      // Mask Parameter: ErrRotSpd_varSetTime
                                          //  Referenced by: '<S33>/SensErrorDetection'

    float TmaxPrev_InitialCondition;   // Computed Parameter: TmaxPrev_InitialCondition
                                          //  Referenced by: '<S13>/TmaxPrev'

    float AddConstant1_Bias;           // Computed Parameter: AddConstant1_Bias
                                          //  Referenced by: '<S13>/Add Constant1'

    float const3_Value;                // Computed Parameter: const3_Value
                                          //  Referenced by: '<S14>/const3'

    float const1_Value;                // Computed Parameter: const1_Value
                                          //  Referenced by: '<S14>/const1'

    float const2_Value;                // Computed Parameter: const2_Value
                                          //  Referenced by: '<S14>/const2'

    float Tnul_Value;                  // Computed Parameter: Tnul_Value
                                          //  Referenced by: '<S17>/Tnul'

    float Vnull1_Value;                // Computed Parameter: Vnull1_Value
                                          //  Referenced by: '<S17>/Vnull1'

    float getTcap_Gain;                // Computed Parameter: getTcap_Gain
                                          //  Referenced by: '<S17>/getTcap'

    float Vnull2_Value;                // Computed Parameter: Vnull2_Value
                                          //  Referenced by: '<S17>/Vnull2'

    float Tnul_Value_k;                // Computed Parameter: Tnul_Value_k
                                          //  Referenced by: '<S18>/Tnul'

    float Vnull1_Value_l;              // Computed Parameter: Vnull1_Value_l
                                          //  Referenced by: '<S18>/Vnull1'

    float getTcap_Gain_k;              // Computed Parameter: getTcap_Gain_k
                                          //  Referenced by: '<S18>/getTcap'

    float Vnull2_Value_e;              // Computed Parameter: Vnull2_Value_e
                                          //  Referenced by: '<S18>/Vnull2'

    float Tnul_Value_a;                // Computed Parameter: Tnul_Value_a
                                          //  Referenced by: '<S19>/Tnul'

    float Vnull1_Value_i;              // Computed Parameter: Vnull1_Value_i
                                          //  Referenced by: '<S19>/Vnull1'

    float getTcap_Gain_b;              // Computed Parameter: getTcap_Gain_b
                                          //  Referenced by: '<S19>/getTcap'

    float Vnull2_Value_eh;             // Computed Parameter: Vnull2_Value_eh
                                          //  Referenced by: '<S19>/Vnull2'

    float const1_Value_i;              // Computed Parameter: const1_Value_i
                                          //  Referenced by: '<S16>/const1'

    float const2_Value_e;              // Computed Parameter: const2_Value_e
                                          //  Referenced by: '<S16>/const2'

    te_ctry inVentStopSys1_Value;      // Expression: te_ctry.Germany
                                          //  Referenced by: '<S3>/inVentStopSys1'

    te_heat_stt Constant1_Value;       // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S8>/Constant1'

    te_heat_stt Constant1_Value_p;     // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S13>/Constant1'

    te_heat_stt Constant3_Value;       // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S13>/Constant3'

    te_on_off On_Value;                // Expression: te_on_off.on
                                          //  Referenced by: '<S6>/On'

    te_on_off On_Value_d;              // Expression: te_on_off.on
                                          //  Referenced by: '<S8>/On'

    te_on_off On_Value_c;              // Expression: te_on_off.on
                                          //  Referenced by: '<S9>/On'

    te_on_off outOnOph_Value;          // Expression: te_on_off.on
                                          //  Referenced by: '<S3>/outOnOph'

    te_on_off outOffOph_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S3>/outOffOph'

    te_on_off outOnVst_Value;          // Expression: te_on_off.on
                                          //  Referenced by: '<S3>/outOnVst'

    te_on_off outOffVst_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S3>/outOffVst'

    te_on_off outOnSg_Value;           // Expression: te_on_off.on
                                          //  Referenced by: '<S3>/outOnSg'

    te_on_off outOffSg_Value;          // Expression: te_on_off.off
                                          //  Referenced by: '<S3>/outOffSg'

    te_on_off inOnSg1_Value;           // Expression: te_on_off.on
                                          //  Referenced by: '<S3>/inOnSg1'

    te_tor_mode inOffPeakHour_Value;   // Expression: te_tor_mode.OffPeakHour
                                          //  Referenced by: '<S3>/inOffPeakHour'

    te_tor_mode inVentStopSys_Value;   // Expression: te_tor_mode.VentSysStop
                                          //  Referenced by: '<S3>/inVentStopSys'

    te_tor_mode inSmartGrid_Value;     // Expression: te_tor_mode.SmartGrid
                                          //  Referenced by: '<S3>/inSmartGrid'

    ta_pwr ResetValue_Value;           // Computed Parameter: ResetValue_Value
                                          //  Referenced by: '<S6>/ResetValue'

    uint32_t Cs_elec_htr_bstr_cnsp_prev_Init;// Computed Parameter: Cs_elec_htr_bstr_cnsp_prev_Init
                                                //  Referenced by: '<S6>/Cs_elec_htr_bstr_cnsp_prev'

    ta_pwr ResetValue_Value_i;         // Computed Parameter: ResetValue_Value_i
                                          //  Referenced by: '<S8>/ResetValue'

    ta_pwr ElecBoosterHeaterPower_Value;// Computed Parameter: ElecBoosterHeaterPower_Value
                                           //  Referenced by: '<S8>/ElecBoosterHeaterPower'

    ta_pwr NulPower_Value;             // Computed Parameter: NulPower_Value
                                          //  Referenced by: '<S8>/NulPower'

    ta_pwr ResetValue_Value_d;         // Computed Parameter: ResetValue_Value_d
                                          //  Referenced by: '<S9>/ResetValue'

    uint32_t Cs_elec_htr_bstr_cnsp_prev_In_a;// Computed Parameter: Cs_elec_htr_bstr_cnsp_prev_In_a
                                                //  Referenced by: '<S8>/Cs_elec_htr_bstr_cnsp_prev'

    uint32_t Cs_vent_cnsp_prev_InitialCondit;// Computed Parameter: Cs_vent_cnsp_prev_InitialCondit
                                                //  Referenced by: '<S9>/Cs_vent_cnsp_prev'

    ta_temp BpEsti_tableData[15];      // Computed Parameter: BpEsti_tableData
                                          //  Referenced by: '<S2>/BpEsti'

    ta_temp BpEsti_bp01Data[15];       // Expression: Ct_evap_temp_C
                                          //  Referenced by: '<S2>/BpEsti'

    ta_temp Constant_Value;            // Expression: Cs_ref_temp_C
                                          //  Referenced by: '<S13>/Constant'

    ta_temp Constant2_Value;           // Expression: Cs_in_temp_C
                                          //  Referenced by: '<S13>/Constant2'

    ta_wtr_vol const2_Value_a;         // Computed Parameter: const2_Value_a
                                          //  Referenced by: '<S13>/const2'

    ta_wtr_vol const1_Value_a;         // Computed Parameter: const1_Value_a
                                          //  Referenced by: '<S13>/const1'

    bool Constant_Value_o;             // Computed Parameter: Constant_Value_o
                                          //  Referenced by: '<S29>/Constant'

    bool mergeErr_InitialOutput;       // Computed Parameter: mergeErr_InitialOutput
                                          //  Referenced by: '<S31>/mergeErr'

    uint8_t Constant_Value_i;          // Computed Parameter: Constant_Value_i
                                          //  Referenced by: '<S10>/Constant'

    uint8_t Constant_Value_m;          // Computed Parameter: Constant_Value_m
                                          //  Referenced by: '<S11>/Constant'

    uint8_t Constant_Value_o2;         // Computed Parameter: Constant_Value_o2
                                          //  Referenced by: '<S12>/Constant'

  };

  // Initial conditions function
  void init();

  // model step function
  void step(const tb_InPutMng_In *rtu_InPutMng_In, tb_InPutMng_Out *rty_InPutMng_Out);

  // Copy Constructor
  InPutMng(InPutMng const&) = delete;

  // Assignment Operator
  InPutMng& operator= (InPutMng const&) & = delete;

  // Move Constructor
  InPutMng(InPutMng &&) = delete;

  // Move Assignment Operator
  InPutMng& operator= (InPutMng &&) = delete;

  // Tunable parameters
  static P_InPutMng_T InPutMng_rtP;

  // Reset function
  void reset();

  // Constructor
  InPutMng();

  // Destructor
  ~InPutMng();

  // private data and function members
 private:
  // Block states
  DW_InPutMng_T InPutMng_DW;

  // Declare private class scope variables for system: "model 'InPutMng'"
  const tb_InPutMng_In *InPutMng_rtu_InPutMng_In;// '<Root>/InPutMng_In'
  tb_InPutMng_Out *InPutMng_rty_InPutMng_Out;// '<Root>/InPutMng_Out'

  // private member function(s) for subsystem '<S1>/VentConsumption'
  void InPutMng_VentConsumption_Init();
  void InPutMng_VentConsumption_Reset();
  void InPutMng_VentConsumption();

  // private member function(s) for subsystem '<S1>/WaterHeatingConsumption'
  void In_WaterHeatingConsumption_Init();
  void I_WaterHeatingConsumption_Reset();
  void InPutMn_WaterHeatingConsumption();

  // private member function(s) for subsystem '<Root>/ConsumptionCalc'
  void InPutMng_ConsumptionCalc_Init();
  void InPutMng_ConsumptionCalc_Reset();
  void InPutMng_ConsumptionCalc();

  // private member function(s) for subsystem '<Root>/Temp2PresConvert'
  void InPutMng_Temp2PresConvert();

  // private member function(s) for subsystem '<Root>/TorInputMng'
  void InPutMng_TorInputMng() const;

  // private member function(s) for subsystem '<S4>/Err_cons'
  void InPutMng_Err_cons_Init();
  void InPutMng_Err_cons_Reset();
  void InPutMng_Err_cons();

  // private member function(s) for subsystem '<S4>/V_ref_max_Calc'
  void InPutMng_V_ref_max_Calc();

  // private member function(s) for subsystem '<S15>/BottomCalc'
  void InPutMng_BottomCalc();

  // private member function(s) for subsystem '<S15>/MiddleCalc'
  void InPutMng_MiddleCalc();

  // private member function(s) for subsystem '<S15>/TopCalc'
  void InPutMng_TopCalc();

  // private member function(s) for subsystem '<S4>/V_ref_mean_Calc'
  void InPutMng_V_ref_mean_Calc();

  // private member function(s) for subsystem '<S4>/V_ref_min_Calc'
  void InPutMng_V_ref_min_Calc();

  // private member function(s) for subsystem '<Root>/V40Estimation'
  void InPutMng_V40Estimation_Init();
  void InPutMng_V40Estimation_Reset();
  void InPutMng_V40Estimation();

  // private member function(s) for subsystem '<S5>/Filtering'
  void InPutMng_Filtering_Init();
  void InPutMng_Filtering_Reset();
  void InPutMng_Filtering();

  // private member function(s) for subsystem '<S21>/errorBypass'
  void InPutMng_errorBypass();

  // private member function(s) for subsystem '<S21>/errorDetect'
  void InPutMng_errorDetect();

  // private member function(s) for subsystem '<S5>/errorDetection'
  void InPutMng_errorDetection_Init();
  void InPutMng_errorDetection();

  // private member function(s) for subsystem '<Root>/sensSignalMng'
  void InPutMng_sensSignalMng_Init();
  void InPutMng_sensSignalMng_Reset();
  void InPutMng_sensSignalMng();

  // model instance variable for '<S22>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ1;

  // model instance variable for '<S23>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ2;

  // model instance variable for '<S24>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ3;

  // model instance variable for '<S25>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ4;

  // model instance variable for '<S26>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ5;

  // model instance variable for '<S27>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ6;

  // model instance variable for '<S28>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ7;

  // model instance variable for '<S32>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ8;

  // model instance variable for '<S33>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ9;

  // model instance variable for '<S34>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ10;

  // model instance variable for '<S35>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ11;

  // model instance variable for '<S36>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ12;

  // model instance variable for '<S37>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ13;

  // model instance variable for '<S38>/SensErrorDetection'
  SensErrorDetection SensErrorDetectionMDLOBJ14;
};

extern InPutMng::P_InPutMng_T InPutMng_rtP;

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
//  '<Root>' : 'InPutMng'
//  '<S1>'   : 'InPutMng/ConsumptionCalc'
//  '<S2>'   : 'InPutMng/Temp2PresConvert'
//  '<S3>'   : 'InPutMng/TorInputMng'
//  '<S4>'   : 'InPutMng/V40Estimation'
//  '<S5>'   : 'InPutMng/sensSignalMng'
//  '<S6>'   : 'InPutMng/ConsumptionCalc/VentConsumption'
//  '<S7>'   : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption'
//  '<S8>'   : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/BoosterHeaterPower'
//  '<S9>'   : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/HeatPumpPower'
//  '<S10>'  : 'InPutMng/TorInputMng/isTrue'
//  '<S11>'  : 'InPutMng/TorInputMng/isTrue1'
//  '<S12>'  : 'InPutMng/TorInputMng/isTrue2'
//  '<S13>'  : 'InPutMng/V40Estimation/Err_cons'
//  '<S14>'  : 'InPutMng/V40Estimation/V_ref_max_Calc'
//  '<S15>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc'
//  '<S16>'  : 'InPutMng/V40Estimation/V_ref_min_Calc'
//  '<S17>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/BottomCalc'
//  '<S18>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/MiddleCalc'
//  '<S19>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/TopCalc'
//  '<S20>'  : 'InPutMng/sensSignalMng/Filtering'
//  '<S21>'  : 'InPutMng/sensSignalMng/errorDetection'
//  '<S22>'  : 'InPutMng/sensSignalMng/Filtering/FPresVent'
//  '<S23>'  : 'InPutMng/sensSignalMng/Filtering/FRotSpd'
//  '<S24>'  : 'InPutMng/sensSignalMng/Filtering/FTempDown'
//  '<S25>'  : 'InPutMng/sensSignalMng/Filtering/FTempEvap'
//  '<S26>'  : 'InPutMng/sensSignalMng/Filtering/FTempUp'
//  '<S27>'  : 'InPutMng/sensSignalMng/Filtering/FTempVent'
//  '<S28>'  : 'InPutMng/sensSignalMng/Filtering/FTempXhst'
//  '<S29>'  : 'InPutMng/sensSignalMng/errorDetection/errorBypass'
//  '<S30>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect'
//  '<S31>'  : 'InPutMng/sensSignalMng/errorDetection/mergeByp'
//  '<S32>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrPresVent'
//  '<S33>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrRotSpd'
//  '<S34>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempDown'
//  '<S35>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempEvap'
//  '<S36>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempUp'
//  '<S37>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempVent'
//  '<S38>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempXhst'

#endif                                 // InPutMng_h_

//
// File trailer for generated code.
//
// [EOF]
//
