//
// File: InPutMng.h
//
// Code generated for Simulink model 'InPutMng'.
//
// Model version                  : 1.340
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Sep  8 11:00:51 2025
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
#include "Integrator.h"
#include "SensErrorDetection.h"
#include "firstOrderTF.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_input_mng_tick;     // Variable: input_mng_tick
                                          //  Referenced by:
                                          //    '<S28>/FPresVent'
                                          //    '<S29>/FRotSpd'
                                          //    '<S30>/FTempDown'
                                          //    '<S31>/FTempEvap'
                                          //    '<S32>/FTempUp'
                                          //    '<S33>/FTempVent'
                                          //    '<S34>/FTempXhst'
                                          //    '<S9>/egyCalc'
                                          //    '<S38>/ErrPresVent'
                                          //    '<S39>/ErrRotSpd'
                                          //    '<S40>/ErrTempDown'
                                          //    '<S41>/ErrTempEvap'
                                          //    '<S42>/ErrTempUp'
                                          //    '<S43>/ErrTempVent'
                                          //    '<S44>/ErrTempXhst'
                                          //    '<S13>/egyCalc'
                                          //    '<S15>/egyCalc'


// Class declaration for model InPutMng
class InPutMng final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'InPutMng'
  struct DW_InPutMng_T {
    float divideByTrefMinusTin;        // '<S22>/divideByTrefMinusTin'
    float Vref;                        // '<S21>/getVref'
    float Vol;                         // '<S25>/getVol'
    float getTemp;                     // '<S25>/getTemp'
    float Vol_i;                       // '<S24>/getVol'
    float getTemp_h;                   // '<S24>/getTemp'
    float Vol_o;                       // '<S23>/getVol'
    float getTemp_c;                   // '<S23>/getTemp'
    float V_ref_max;                   // '<S20>/divideByTrefMinusTin'
    float TempToSing3;                 // '<S19>/TempToSing3'
    float TempToSing4;                 // '<S19>/TempToSing4'
    float TempToSing2;                 // '<S19>/TempToSing2'
    float Tmax;                        // '<S19>/Switch'
    float TempToSing1;                 // '<S19>/TempToSing1'
    float Vb_Switch;                   // '<S19>/Vb_Switch'
    float Vh_Switch;                   // '<S19>/Vh_Switch'
    float VolToSing1;                  // '<S19>/VolToSing1'
    float TmaxPrev_DSTATE;             // '<S19>/TmaxPrev'
    ta_egy AddCnsp_g;                  // '<S7>/AddCnsp'
    ta_pwr AddPwr_g;                   // '<S7>/AddPwr'
    ta_pwr Cs_vent_pwr;                // '<S6>/ventPwrMap'
    ta_egy egyWh;                      // '<S6>/egyWh'
    ta_temp Cs_tank_down_temp_raw;
    ta_temp Cs_tank_up_temp_raw;
    ta_temp Cs_pump_xhst_temp_raw;
    ta_temp Cs_pump_evap_temp_raw;
    ta_temp Cs_vent_temp_raw;
    ta_temp Cs_pump_evap_temp;         // '<Root>/SimuSwitch3'
    ta_temp Cs_tank_down_temp;         // '<Root>/SimuSwitch'
    ta_temp Cs_tank_up_temp;           // '<Root>/SimuSwitch1'
    ta_temp Cs_tank_down_temp_cs;      // '<S37>/mergeTempDown'
    ta_temp Cs_tank_up_temp_cs;        // '<S37>/mergeTempUp'
    ta_temp Cs_pump_xhst_temp_cs;      // '<S37>/mergeXhstTemp'
    ta_temp Cs_pump_evap_temp_cs;      // '<S37>/mergeEvapTemp'
    ta_temp Cs_vent_temp_cs;           // '<S37>/mergeVentTemp'
    ta_temp Cs_tank_down_temp_e;       // '<S26>/SingToTemp'
    ta_temp Cs_tank_up_temp_g;         // '<S26>/SingToTemp1'
    ta_temp Cs_pump_xhst_temp;         // '<S26>/SingToTemp2'
    ta_temp Cs_pump_evap_temp_l;       // '<S26>/SingToTemp3'
    ta_temp Cs_vent_temp;              // '<S26>/SingToTemp4'
    ta_temp BpEsti;                    // '<S2>/BpEsti'
    ta_air_pres Cs_vent_pres_raw;
    ta_rot_spd Cs_vent_rot_spd_raw;
    ta_air_pres Cs_vent_pres;          // '<Root>/SimuSwitch6'
    ta_rot_spd Cs_vent_rot_spd;        // '<Root>/SimuSwitch15'
    ta_air_pres Cs_pres_vent_cs;       // '<S37>/mergePresVent'
    ta_rot_spd Cs_vent_rot_spd_cs;     // '<S37>/mergeRotSpd'
    ta_air_pres Cs_vent_pres_g;        // '<S26>/SingToPres'
    ta_rot_spd Cs_vent_rot_spd_k;      // '<S26>/SingToRotSpd'
    ta_wtr_vol Cs_v40_min;             // '<S4>/SingToVol1'
    bool mergeErr[7];                  // '<S37>/mergeErr'
    bool Bs_tank_down_temp_err;        // '<Root>/SimuSwitch7'
    bool Bs_tank_up_temp_err;          // '<Root>/SimuSwitch8'
  };

  // Parameters (default storage)
  struct P_InPutMng_T {
    float FTempDown_K;                 // Mask Parameter: FTempDown_K
                                          //  Referenced by: '<S30>/FTempDown'

    float FTempUp_K;                   // Mask Parameter: FTempUp_K
                                          //  Referenced by: '<S32>/FTempUp'

    float FTempXhst_K;                 // Mask Parameter: FTempXhst_K
                                          //  Referenced by: '<S34>/FTempXhst'

    float FTempEvap_K;                 // Mask Parameter: FTempEvap_K
                                          //  Referenced by: '<S31>/FTempEvap'

    float FTempVent_K;                 // Mask Parameter: FTempVent_K
                                          //  Referenced by: '<S33>/FTempVent'

    float FPresVent_K;                 // Mask Parameter: FPresVent_K
                                          //  Referenced by: '<S28>/FPresVent'

    float FRotSpd_K;                   // Mask Parameter: FRotSpd_K
                                          //  Referenced by: '<S29>/FRotSpd'

    float FTempDown_Tau;               // Mask Parameter: FTempDown_Tau
                                          //  Referenced by: '<S30>/FTempDown'

    float FTempUp_Tau;                 // Mask Parameter: FTempUp_Tau
                                          //  Referenced by: '<S32>/FTempUp'

    float FTempXhst_Tau;               // Mask Parameter: FTempXhst_Tau
                                          //  Referenced by: '<S34>/FTempXhst'

    float FTempEvap_Tau;               // Mask Parameter: FTempEvap_Tau
                                          //  Referenced by: '<S31>/FTempEvap'

    float FTempVent_Tau;               // Mask Parameter: FTempVent_Tau
                                          //  Referenced by: '<S33>/FTempVent'

    float FPresVent_Tau;               // Mask Parameter: FPresVent_Tau
                                          //  Referenced by: '<S28>/FPresVent'

    float FRotSpd_Tau;                 // Mask Parameter: FRotSpd_Tau
                                          //  Referenced by: '<S29>/FRotSpd'

    float ErrTempDown_defVal;          // Mask Parameter: ErrTempDown_defVal
                                          //  Referenced by: '<S40>/ErrTempDown'

    float ErrTempUp_defVal;            // Mask Parameter: ErrTempUp_defVal
                                          //  Referenced by: '<S42>/ErrTempUp'

    float ErrTempXhst_defVal;          // Mask Parameter: ErrTempXhst_defVal
                                          //  Referenced by: '<S44>/ErrTempXhst'

    float ErrTempEvap_defVal;          // Mask Parameter: ErrTempEvap_defVal
                                          //  Referenced by: '<S41>/ErrTempEvap'

    float ErrTempVent_defVal;          // Mask Parameter: ErrTempVent_defVal
                                          //  Referenced by: '<S43>/ErrTempVent'

    float ErrPresVent_defVal;          // Mask Parameter: ErrPresVent_defVal
                                          //  Referenced by: '<S38>/ErrPresVent'

    float ErrRotSpd_defVal;            // Mask Parameter: ErrRotSpd_defVal
                                          //  Referenced by: '<S39>/ErrRotSpd'

    float egyCalc_initVal;             // Mask Parameter: egyCalc_initVal
                                          //  Referenced by: '<S9>/egyCalc'

    float egyCalc_initVal_f;           // Mask Parameter: egyCalc_initVal_f
                                          //  Referenced by: '<S13>/egyCalc'

    float egyCalc_initVal_j;           // Mask Parameter: egyCalc_initVal_j
                                          //  Referenced by: '<S15>/egyCalc'

    float FTempDown_initVal;           // Mask Parameter: FTempDown_initVal
                                          //  Referenced by: '<S30>/FTempDown'

    float FTempUp_initVal;             // Mask Parameter: FTempUp_initVal
                                          //  Referenced by: '<S32>/FTempUp'

    float FTempXhst_initVal;           // Mask Parameter: FTempXhst_initVal
                                          //  Referenced by: '<S34>/FTempXhst'

    float FTempEvap_initVal;           // Mask Parameter: FTempEvap_initVal
                                          //  Referenced by: '<S31>/FTempEvap'

    float FTempVent_initVal;           // Mask Parameter: FTempVent_initVal
                                          //  Referenced by: '<S33>/FTempVent'

    float FPresVent_initVal;           // Mask Parameter: FPresVent_initVal
                                          //  Referenced by: '<S28>/FPresVent'

    float FRotSpd_initVal;             // Mask Parameter: FRotSpd_initVal
                                          //  Referenced by: '<S29>/FRotSpd'

    float ErrPresVent_opScaleRstTime;  // Mask Parameter: ErrPresVent_opScaleRstTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    float ErrPresVent_opScaleSetTime;  // Mask Parameter: ErrPresVent_opScaleSetTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    float ErrTempDown_staticRstTol;    // Mask Parameter: ErrTempDown_staticRstTol
                                          //  Referenced by: '<S40>/ErrTempDown'

    float ErrTempUp_staticRstTol;      // Mask Parameter: ErrTempUp_staticRstTol
                                          //  Referenced by: '<S42>/ErrTempUp'

    float ErrTempXhst_staticRstTol;    // Mask Parameter: ErrTempXhst_staticRstTol
                                          //  Referenced by: '<S44>/ErrTempXhst'

    float ErrTempEvap_staticRstTol;    // Mask Parameter: ErrTempEvap_staticRstTol
                                          //  Referenced by: '<S41>/ErrTempEvap'

    float ErrTempVent_staticRstTol;    // Mask Parameter: ErrTempVent_staticRstTol
                                          //  Referenced by: '<S43>/ErrTempVent'

    float ErrPresVent_staticRstTol;    // Mask Parameter: ErrPresVent_staticRstTol
                                          //  Referenced by: '<S38>/ErrPresVent'

    float ErrRotSpd_staticRstTol;      // Mask Parameter: ErrRotSpd_staticRstTol
                                          //  Referenced by: '<S39>/ErrRotSpd'

    ta_temp ErrTempDown_maxThrs;       // Mask Parameter: ErrTempDown_maxThrs
                                          //  Referenced by: '<S40>/ErrTempDown'

    ta_temp ErrTempUp_maxThrs;         // Mask Parameter: ErrTempUp_maxThrs
                                          //  Referenced by: '<S42>/ErrTempUp'

    ta_temp ErrTempXhst_maxThrs;       // Mask Parameter: ErrTempXhst_maxThrs
                                          //  Referenced by: '<S44>/ErrTempXhst'

    ta_temp ErrTempEvap_maxThrs;       // Mask Parameter: ErrTempEvap_maxThrs
                                          //  Referenced by: '<S41>/ErrTempEvap'

    ta_temp ErrTempVent_maxThrs;       // Mask Parameter: ErrTempVent_maxThrs
                                          //  Referenced by: '<S43>/ErrTempVent'

    ta_temp ErrTempDown_maxVar;        // Mask Parameter: ErrTempDown_maxVar
                                          //  Referenced by: '<S40>/ErrTempDown'

    ta_temp ErrTempUp_maxVar;          // Mask Parameter: ErrTempUp_maxVar
                                          //  Referenced by: '<S42>/ErrTempUp'

    ta_temp ErrTempXhst_maxVar;        // Mask Parameter: ErrTempXhst_maxVar
                                          //  Referenced by: '<S44>/ErrTempXhst'

    ta_temp ErrTempEvap_maxVar;        // Mask Parameter: ErrTempEvap_maxVar
                                          //  Referenced by: '<S41>/ErrTempEvap'

    ta_temp ErrTempVent_maxVar;        // Mask Parameter: ErrTempVent_maxVar
                                          //  Referenced by: '<S43>/ErrTempVent'

    ta_temp ErrTempDown_minThrs;       // Mask Parameter: ErrTempDown_minThrs
                                          //  Referenced by: '<S40>/ErrTempDown'

    ta_temp ErrTempUp_minThrs;         // Mask Parameter: ErrTempUp_minThrs
                                          //  Referenced by: '<S42>/ErrTempUp'

    ta_temp ErrTempXhst_minThrs;       // Mask Parameter: ErrTempXhst_minThrs
                                          //  Referenced by: '<S44>/ErrTempXhst'

    ta_temp ErrTempEvap_minThrs;       // Mask Parameter: ErrTempEvap_minThrs
                                          //  Referenced by: '<S41>/ErrTempEvap'

    ta_temp ErrTempVent_minThrs;       // Mask Parameter: ErrTempVent_minThrs
                                          //  Referenced by: '<S43>/ErrTempVent'

    ta_air_pres ErrPresVent_maxThrs;   // Mask Parameter: ErrPresVent_maxThrs
                                          //  Referenced by: '<S38>/ErrPresVent'

    ta_rot_spd ErrRotSpd_maxThrs;      // Mask Parameter: ErrRotSpd_maxThrs
                                          //  Referenced by: '<S39>/ErrRotSpd'

    ta_air_pres ErrPresVent_maxVar;    // Mask Parameter: ErrPresVent_maxVar
                                          //  Referenced by: '<S38>/ErrPresVent'

    ta_rot_spd ErrRotSpd_maxVar;       // Mask Parameter: ErrRotSpd_maxVar
                                          //  Referenced by: '<S39>/ErrRotSpd'

    ta_air_pres ErrPresVent_minThrs;   // Mask Parameter: ErrPresVent_minThrs
                                          //  Referenced by: '<S38>/ErrPresVent'

    ta_rot_spd ErrRotSpd_minThrs;      // Mask Parameter: ErrRotSpd_minThrs
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_opScaleRstTime;// Mask Parameter: ErrTempDown_opScaleRstTime
                                           //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_opScaleRstTime; // Mask Parameter: ErrTempUp_opScaleRstTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_opScaleRstTime;// Mask Parameter: ErrTempXhst_opScaleRstTime
                                           //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_opScaleRstTime;// Mask Parameter: ErrTempEvap_opScaleRstTime
                                           //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_opScaleRstTime;// Mask Parameter: ErrTempVent_opScaleRstTime
                                           //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrRotSpd_opScaleRstTime; // Mask Parameter: ErrRotSpd_opScaleRstTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_opScaleSetTime;// Mask Parameter: ErrTempDown_opScaleSetTime
                                           //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_opScaleSetTime; // Mask Parameter: ErrTempUp_opScaleSetTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_opScaleSetTime;// Mask Parameter: ErrTempXhst_opScaleSetTime
                                           //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_opScaleSetTime;// Mask Parameter: ErrTempEvap_opScaleSetTime
                                           //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_opScaleSetTime;// Mask Parameter: ErrTempVent_opScaleSetTime
                                           //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrRotSpd_opScaleSetTime; // Mask Parameter: ErrRotSpd_opScaleSetTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_staticRstTime;// Mask Parameter: ErrTempDown_staticRstTime
                                          //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_staticRstTime;  // Mask Parameter: ErrTempUp_staticRstTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_staticRstTime;// Mask Parameter: ErrTempXhst_staticRstTime
                                          //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_staticRstTime;// Mask Parameter: ErrTempEvap_staticRstTime
                                          //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_staticRstTime;// Mask Parameter: ErrTempVent_staticRstTime
                                          //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrPresVent_staticRstTime;// Mask Parameter: ErrPresVent_staticRstTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    uint16_t ErrRotSpd_staticRstTime;  // Mask Parameter: ErrRotSpd_staticRstTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_staticSetTime;// Mask Parameter: ErrTempDown_staticSetTime
                                          //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_staticSetTime;  // Mask Parameter: ErrTempUp_staticSetTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_staticSetTime;// Mask Parameter: ErrTempXhst_staticSetTime
                                          //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_staticSetTime;// Mask Parameter: ErrTempEvap_staticSetTime
                                          //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_staticSetTime;// Mask Parameter: ErrTempVent_staticSetTime
                                          //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrPresVent_staticSetTime;// Mask Parameter: ErrPresVent_staticSetTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    uint16_t ErrRotSpd_staticSetTime;  // Mask Parameter: ErrRotSpd_staticSetTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_varRstTime;   // Mask Parameter: ErrTempDown_varRstTime
                                          //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_varRstTime;     // Mask Parameter: ErrTempUp_varRstTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_varRstTime;   // Mask Parameter: ErrTempXhst_varRstTime
                                          //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_varRstTime;   // Mask Parameter: ErrTempEvap_varRstTime
                                          //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_varRstTime;   // Mask Parameter: ErrTempVent_varRstTime
                                          //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrPresVent_varRstTime;   // Mask Parameter: ErrPresVent_varRstTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    uint16_t ErrRotSpd_varRstTime;     // Mask Parameter: ErrRotSpd_varRstTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    uint16_t ErrTempDown_varSetTime;   // Mask Parameter: ErrTempDown_varSetTime
                                          //  Referenced by: '<S40>/ErrTempDown'

    uint16_t ErrTempUp_varSetTime;     // Mask Parameter: ErrTempUp_varSetTime
                                          //  Referenced by: '<S42>/ErrTempUp'

    uint16_t ErrTempXhst_varSetTime;   // Mask Parameter: ErrTempXhst_varSetTime
                                          //  Referenced by: '<S44>/ErrTempXhst'

    uint16_t ErrTempEvap_varSetTime;   // Mask Parameter: ErrTempEvap_varSetTime
                                          //  Referenced by: '<S41>/ErrTempEvap'

    uint16_t ErrTempVent_varSetTime;   // Mask Parameter: ErrTempVent_varSetTime
                                          //  Referenced by: '<S43>/ErrTempVent'

    uint16_t ErrPresVent_varSetTime;   // Mask Parameter: ErrPresVent_varSetTime
                                          //  Referenced by: '<S38>/ErrPresVent'

    uint16_t ErrRotSpd_varSetTime;     // Mask Parameter: ErrRotSpd_varSetTime
                                          //  Referenced by: '<S39>/ErrRotSpd'

    float precisionPower_Value;        // Computed Parameter: precisionPower_Value
                                          //  Referenced by: '<S6>/precisionPower'

    float hourIntoSec_Value;           // Computed Parameter: hourIntoSec_Value
                                          //  Referenced by: '<S8>/hourIntoSec'

    float precisionPwr_Value;          // Computed Parameter: precisionPwr_Value
                                          //  Referenced by: '<S10>/precisionPwr'

    float hourIntoSec_Value_b;         // Computed Parameter: hourIntoSec_Value_b
                                          //  Referenced by: '<S12>/hourIntoSec'

    float precisionPwr_Value_i;        // Computed Parameter: precisionPwr_Value_i
                                          //  Referenced by: '<S11>/precisionPwr'

    float hourIntoSec_Value_k;         // Computed Parameter: hourIntoSec_Value_k
                                          //  Referenced by: '<S14>/hourIntoSec'

    float TmaxPrev_InitialCondition;   // Computed Parameter: TmaxPrev_InitialCondition
                                          //  Referenced by: '<S19>/TmaxPrev'

    float AddConstant1_Bias;           // Computed Parameter: AddConstant1_Bias
                                          //  Referenced by: '<S19>/Add Constant1'

    float const3_Value;                // Computed Parameter: const3_Value
                                          //  Referenced by: '<S20>/const3'

    float const1_Value;                // Computed Parameter: const1_Value
                                          //  Referenced by: '<S20>/const1'

    float const2_Value;                // Computed Parameter: const2_Value
                                          //  Referenced by: '<S20>/const2'

    float Tnul_Value;                  // Computed Parameter: Tnul_Value
                                          //  Referenced by: '<S23>/Tnul'

    float Vnull1_Value;                // Computed Parameter: Vnull1_Value
                                          //  Referenced by: '<S23>/Vnull1'

    float getTcap_Gain;                // Computed Parameter: getTcap_Gain
                                          //  Referenced by: '<S23>/getTcap'

    float Vnull2_Value;                // Computed Parameter: Vnull2_Value
                                          //  Referenced by: '<S23>/Vnull2'

    float Tnul_Value_k;                // Computed Parameter: Tnul_Value_k
                                          //  Referenced by: '<S24>/Tnul'

    float Vnull1_Value_l;              // Computed Parameter: Vnull1_Value_l
                                          //  Referenced by: '<S24>/Vnull1'

    float getTcap_Gain_k;              // Computed Parameter: getTcap_Gain_k
                                          //  Referenced by: '<S24>/getTcap'

    float Vnull2_Value_e;              // Computed Parameter: Vnull2_Value_e
                                          //  Referenced by: '<S24>/Vnull2'

    float Tnul_Value_a;                // Computed Parameter: Tnul_Value_a
                                          //  Referenced by: '<S25>/Tnul'

    float Vnull1_Value_i;              // Computed Parameter: Vnull1_Value_i
                                          //  Referenced by: '<S25>/Vnull1'

    float getTcap_Gain_b;              // Computed Parameter: getTcap_Gain_b
                                          //  Referenced by: '<S25>/getTcap'

    float Vnull2_Value_eh;             // Computed Parameter: Vnull2_Value_eh
                                          //  Referenced by: '<S25>/Vnull2'

    float const1_Value_i;              // Computed Parameter: const1_Value_i
                                          //  Referenced by: '<S22>/const1'

    float const2_Value_e;              // Computed Parameter: const2_Value_e
                                          //  Referenced by: '<S22>/const2'

    te_ctry inVentStopSys1_Value;      // Expression: te_ctry.Germany
                                          //  Referenced by: '<S3>/inVentStopSys1'

    te_heat_stt Constant1_Value;       // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S10>/Constant1'

    te_heat_stt Constant1_Value_p;     // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S19>/Constant1'

    te_heat_stt Constant3_Value;       // Expression: te_heat_stt.Heating
                                          //  Referenced by: '<S19>/Constant3'

    te_on_off On_Value;                // Expression: te_on_off.on
                                          //  Referenced by: '<S6>/On'

    te_on_off On_Value_d;              // Expression: te_on_off.on
                                          //  Referenced by: '<S10>/On'

    te_on_off On_Value_c;              // Expression: te_on_off.on
                                          //  Referenced by: '<S11>/On'

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

    ta_pwr ventPwrMap_tableData[7140]; // Expression: Cm_cnsp_pwr_C
                                          //  Referenced by: '<S6>/ventPwrMap'

    uint32_t ventPwrMap_maxIndex[2];   // Computed Parameter: ventPwrMap_maxIndex
                                          //  Referenced by: '<S6>/ventPwrMap'

    ta_pwr ElecBoosterHeaterPower_Value;// Computed Parameter: ElecBoosterHeaterPower_Value
                                           //  Referenced by: '<S10>/ElecBoosterHeaterPower'

    ta_pwr NulPower_Value;             // Computed Parameter: NulPower_Value
                                          //  Referenced by: '<S10>/NulPower'

    ta_temp BpEsti_tableData[15];      // Computed Parameter: BpEsti_tableData
                                          //  Referenced by: '<S2>/BpEsti'

    ta_temp BpEsti_bp01Data[15];       // Expression: Ct_evap_temp_C
                                          //  Referenced by: '<S2>/BpEsti'

    ta_temp Constant_Value;            // Expression: Cs_ref_temp_C
                                          //  Referenced by: '<S19>/Constant'

    ta_temp Constant2_Value;           // Expression: Cs_in_temp_C
                                          //  Referenced by: '<S19>/Constant2'

    ta_rot_spd ventPwrMap_bp01Data[85];// Expression: Ct_bkp_rpm_C
                                          //  Referenced by: '<S6>/ventPwrMap'

    ta_rot_spd ventPwrMap_bp02Data[84];// Expression: Ct_bkp_pres_C
                                          //  Referenced by: '<S6>/ventPwrMap'

    ta_wtr_vol const2_Value_a;         // Computed Parameter: const2_Value_a
                                          //  Referenced by: '<S19>/const2'

    ta_wtr_vol const1_Value_a;         // Computed Parameter: const1_Value_a
                                          //  Referenced by: '<S19>/const1'

    bool Constant_Value_o;             // Computed Parameter: Constant_Value_o
                                          //  Referenced by: '<S35>/Constant'

    bool mergeErr_InitialOutput;       // Computed Parameter: mergeErr_InitialOutput
                                          //  Referenced by: '<S37>/mergeErr'

    uint8_t Constant_Value_i;          // Computed Parameter: Constant_Value_i
                                          //  Referenced by: '<S16>/Constant'

    uint8_t Constant_Value_m;          // Computed Parameter: Constant_Value_m
                                          //  Referenced by: '<S17>/Constant'

    uint8_t Constant_Value_o2;         // Computed Parameter: Constant_Value_o2
                                          //  Referenced by: '<S18>/Constant'

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

  // model instance variable for '<S9>/egyCalc'
  Integrator egyCalcMDLOBJ1;

  // model instance variable for '<S13>/egyCalc'
  Integrator egyCalcMDLOBJ2;

  // model instance variable for '<S15>/egyCalc'
  Integrator egyCalcMDLOBJ3;

  // model instance variable for '<S28>/FPresVent'
  firstOrderTF FPresVentMDLOBJ4;

  // model instance variable for '<S29>/FRotSpd'
  firstOrderTF FRotSpdMDLOBJ5;

  // model instance variable for '<S30>/FTempDown'
  firstOrderTF FTempDownMDLOBJ6;

  // model instance variable for '<S31>/FTempEvap'
  firstOrderTF FTempEvapMDLOBJ7;

  // model instance variable for '<S32>/FTempUp'
  firstOrderTF FTempUpMDLOBJ8;

  // model instance variable for '<S33>/FTempVent'
  firstOrderTF FTempVentMDLOBJ9;

  // model instance variable for '<S34>/FTempXhst'
  firstOrderTF FTempXhstMDLOBJ10;

  // model instance variable for '<S38>/ErrPresVent'
  SensErrorDetection ErrPresVentMDLOBJ11;

  // model instance variable for '<S39>/ErrRotSpd'
  SensErrorDetection ErrRotSpdMDLOBJ12;

  // model instance variable for '<S40>/ErrTempDown'
  SensErrorDetection ErrTempDownMDLOBJ13;

  // model instance variable for '<S41>/ErrTempEvap'
  SensErrorDetection ErrTempEvapMDLOBJ14;

  // model instance variable for '<S42>/ErrTempUp'
  SensErrorDetection ErrTempUpMDLOBJ15;

  // model instance variable for '<S43>/ErrTempVent'
  SensErrorDetection ErrTempVentMDLOBJ16;

  // model instance variable for '<S44>/ErrTempXhst'
  SensErrorDetection ErrTempXhstMDLOBJ17;

  // Block states
  DW_InPutMng_T InPutMng_DW;

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

  // private member function(s) for subsystem '<S21>/BottomCalc'
  void InPutMng_BottomCalc();

  // private member function(s) for subsystem '<S21>/MiddleCalc'
  void InPutMng_MiddleCalc();

  // private member function(s) for subsystem '<S21>/TopCalc'
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

  // private member function(s) for subsystem '<S27>/errorBypass'
  void InPutMng_errorBypass();

  // private member function(s) for subsystem '<S27>/errorDetect'
  void InPutMng_errorDetect();

  // private member function(s) for subsystem '<S5>/errorDetection'
  void InPutMng_errorDetection_Init();
  void InPutMng_errorDetection();

  // private member function(s) for subsystem '<Root>/sensSignalMng'
  void InPutMng_sensSignalMng_Init();
  void InPutMng_sensSignalMng_Reset();
  void InPutMng_sensSignalMng();
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
//  '<S8>'   : 'InPutMng/ConsumptionCalc/VentConsumption/WtoWhConverter'
//  '<S9>'   : 'InPutMng/ConsumptionCalc/VentConsumption/WtoWhConverter/egyCalc'
//  '<S10>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/BoosterHeaterPower'
//  '<S11>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/HeatPumpPower'
//  '<S12>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/BoosterHeaterPower/WtoWhConverter1'
//  '<S13>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/BoosterHeaterPower/WtoWhConverter1/egyCalc'
//  '<S14>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/HeatPumpPower/WtoWhConverter1'
//  '<S15>'  : 'InPutMng/ConsumptionCalc/WaterHeatingConsumption/HeatPumpPower/WtoWhConverter1/egyCalc'
//  '<S16>'  : 'InPutMng/TorInputMng/isTrue'
//  '<S17>'  : 'InPutMng/TorInputMng/isTrue1'
//  '<S18>'  : 'InPutMng/TorInputMng/isTrue2'
//  '<S19>'  : 'InPutMng/V40Estimation/Err_cons'
//  '<S20>'  : 'InPutMng/V40Estimation/V_ref_max_Calc'
//  '<S21>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc'
//  '<S22>'  : 'InPutMng/V40Estimation/V_ref_min_Calc'
//  '<S23>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/BottomCalc'
//  '<S24>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/MiddleCalc'
//  '<S25>'  : 'InPutMng/V40Estimation/V_ref_mean_Calc/TopCalc'
//  '<S26>'  : 'InPutMng/sensSignalMng/Filtering'
//  '<S27>'  : 'InPutMng/sensSignalMng/errorDetection'
//  '<S28>'  : 'InPutMng/sensSignalMng/Filtering/FPresVent'
//  '<S29>'  : 'InPutMng/sensSignalMng/Filtering/FRotSpd'
//  '<S30>'  : 'InPutMng/sensSignalMng/Filtering/FTempDown'
//  '<S31>'  : 'InPutMng/sensSignalMng/Filtering/FTempEvap'
//  '<S32>'  : 'InPutMng/sensSignalMng/Filtering/FTempUp'
//  '<S33>'  : 'InPutMng/sensSignalMng/Filtering/FTempVent'
//  '<S34>'  : 'InPutMng/sensSignalMng/Filtering/FTempXhst'
//  '<S35>'  : 'InPutMng/sensSignalMng/errorDetection/errorBypass'
//  '<S36>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect'
//  '<S37>'  : 'InPutMng/sensSignalMng/errorDetection/mergeByp'
//  '<S38>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrPresVent'
//  '<S39>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrRotSpd'
//  '<S40>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempDown'
//  '<S41>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempEvap'
//  '<S42>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempUp'
//  '<S43>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempVent'
//  '<S44>'  : 'InPutMng/sensSignalMng/errorDetection/errorDetect/ErrTempXhst'

#endif                                 // InPutMng_h_

//
// File trailer for generated code.
//
// [EOF]
//
