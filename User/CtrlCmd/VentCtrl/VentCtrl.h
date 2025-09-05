//
// File: VentCtrl.h
//
// Code generated for Simulink model 'VentCtrl'.
//
// Model version                  : 1.148
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Sep  4 18:36:49 2025
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
#ifndef VentCtrl_h_
#define VentCtrl_h_
#include <stdbool.h>
#include <stdint.h>
#include "VentCtrl_types.h"
#include "PID_backCalc.h"
#include "cntrlSaturation.h"
#include "firstOrderTF.h"
#include <cstring>

extern const bool VentCtrl_BGND;       // bool ground

// Class declaration for model VentCtrl
class VentCtrl final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'VentCtrl'
  struct DW_VentCtrl_T {
    ta_flow flowEsti;                  // '<S3>/flowEsti'
    ta_air_pres VentPresSwitch;        // '<S3>/VentPresSwitch'
    ta_air_pres Cs_press_rise_curv_sp; // '<S6>/SingToPres'
    ta_flow Cs_reg_fan_flow;           // '<S5>/cmd_pid'
    ta_flow VentFlowSwitch;            // '<S1>/VentFlowSwitch'
    ta_flow flowEsti_DSTATE;           // '<S3>/flowEsti'
  };

  // Parameters (default storage)
  struct P_VentCtrl_T {
    float pressSpfilt_K;               // Mask Parameter: pressSpfilt_K
                                          //  Referenced by: '<S11>/firstOrderTF'

    float firstOpressSpFilt_K;         // Mask Parameter: firstOpressSpFilt_K
                                          //  Referenced by: '<S10>/firstOrderTF'

    float presMesfilt_K;               // Mask Parameter: presMesfilt_K
                                          //  Referenced by: '<S9>/firstOrderTF'

    float presFilter_K;                // Mask Parameter: presFilter_K
                                          //  Referenced by: '<S12>/firstOrderTF'

    float pressSpfilt_Tau;             // Mask Parameter: pressSpfilt_Tau
                                          //  Referenced by: '<S11>/firstOrderTF'

    float firstOpressSpFilt_Tau;       // Mask Parameter: firstOpressSpFilt_Tau
                                          //  Referenced by: '<S10>/firstOrderTF'

    float presMesfilt_Tau;             // Mask Parameter: presMesfilt_Tau
                                          //  Referenced by: '<S9>/firstOrderTF'

    float presFilter_Tau;              // Mask Parameter: presFilter_Tau
                                          //  Referenced by: '<S12>/firstOrderTF'

    float pressSpfilt_initVal;         // Mask Parameter: pressSpfilt_initVal
                                          //  Referenced by: '<S11>/firstOrderTF'

    float firstOpressSpFilt_initVal;   // Mask Parameter: firstOpressSpFilt_initVal
                                          //  Referenced by: '<S10>/firstOrderTF'

    float presMesfilt_initVal;         // Mask Parameter: presMesfilt_initVal
                                          //  Referenced by: '<S9>/firstOrderTF'

    float presFilter_initVal;          // Mask Parameter: presFilter_initVal
                                          //  Referenced by: '<S12>/firstOrderTF'

    float pressSpfilt_sampleTime;      // Mask Parameter: pressSpfilt_sampleTime
                                          //  Referenced by: '<S11>/firstOrderTF'

    float firstOpressSpFilt_sampleTime;// Mask Parameter: firstOpressSpFilt_sampleTime
                                          //  Referenced by: '<S10>/firstOrderTF'

    float presMesfilt_sampleTime;      // Mask Parameter: presMesfilt_sampleTime
                                          //  Referenced by: '<S9>/firstOrderTF'

    float presFilter_sampleTime;       // Mask Parameter: presFilter_sampleTime
                                          //  Referenced by: '<S12>/firstOrderTF'

    double press_max_Value;            // Computed Parameter: press_max_Value
                                          //  Referenced by: '<S5>/press_max'

    double press_min_Value;            // Computed Parameter: press_min_Value
                                          //  Referenced by: '<S5>/press_min'

    double flow_max_Value;             // Computed Parameter: flow_max_Value
                                          //  Referenced by: '<S5>/flow_max'

    double flow_min_Value;             // Computed Parameter: flow_min_Value
                                          //  Referenced by: '<S5>/flow_min'

    float PressureRegulator_InitVal;   // Expression: Cs_press_reg_init_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_Kawu;      // Expression: Cs_reg_press_awu_gain_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_Kd;        // Expression: Cs_reg_press_tau_1_C*Cs_reg_press_tau_2_C*Cs_reg_press_gain_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_Ki;        // Expression: Cs_reg_press_gain_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_Kp;        // Expression: (Cs_reg_press_tau_1_C+Cs_reg_press_tau_2_C)*Cs_reg_press_gain_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_SampleTime;// Expression: Cs_PressCtrl_TimeSample_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    float PressureRegulator_Tau_f;     // Expression: Cs_reg_press_d_tau_filt_C
                                          //  Referenced by: '<S5>/PressureRegulator'

    ta_flow Cs_flow_fan_sp_C_Value;    // Computed Parameter: Cs_flow_fan_sp_C_Value
                                          //  Referenced by: '<S4>/Cs_flow_fan_sp_C'

    ta_flow NullFlowConstant_Value;    // Computed Parameter: NullFlowConstant_Value
                                          //  Referenced by: '<S1>/NullFlowConstant'

    ta_vltg flow2volt_tableData[11];   // Expression: Ct_mtr_volt_data_C
                                          //  Referenced by: '<S2>/flow2volt'

    ta_flow flow2volt_bp01Data[11];    // Expression: Ct_mtr_flow_bkpt_C
                                          //  Referenced by: '<S2>/flow2volt'

    ta_air_pres Rise_Curve_tableData[33];// Expression: Ct_rise_curv_data_C
                                            //  Referenced by: '<S6>/Rise_Curve'

    ta_flow Rise_Curve_bp01Data[33];   // Expression: Ct_rise_curv_bp_C
                                          //  Referenced by: '<S6>/Rise_Curve'

    uint16_t flowEsti_InitialCondition;// Computed Parameter: flowEsti_InitialCondition
                                          //  Referenced by: '<S3>/flowEsti'

  };

  // Initial conditions function
  void init();

  // model step function
  void step(const tb_VentCtrl_In *rtu_VentCtrl_In, tb_VentCtrl_Out *rty_VentCtrl_Out);

  // Copy Constructor
  VentCtrl(VentCtrl const&) = delete;

  // Assignment Operator
  VentCtrl& operator= (VentCtrl const&) & = delete;

  // Move Constructor
  VentCtrl(VentCtrl &&) = delete;

  // Move Assignment Operator
  VentCtrl& operator= (VentCtrl &&) = delete;

  // model instance variable for '<S7>/Saturation'
  cntrlSaturation SaturationMDLOBJ1;

  // model instance variable for '<S5>/PressureRegulator'
  PID_backCalc PressureRegulatorMDLOBJ2;

  // model instance variable for '<S10>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ3;

  // model instance variable for '<S11>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ4;

  // model instance variable for '<S9>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ5;

  // model instance variable for '<S12>/firstOrderTF'
  firstOrderTF firstOrderTFMDLOBJ6;

  // Block states
  DW_VentCtrl_T VentCtrl_DW;

  // Tunable parameters
  static P_VentCtrl_T VentCtrl_rtP;

  // Reset function
  void reset();

  // Constructor
  VentCtrl();

  // Destructor
  ~VentCtrl();

  // private data and function members
 private:
  // Declare private class scope variables for system: "model 'VentCtrl'"
  const tb_VentCtrl_In *VentCtrl_rtu_VentCtrl_In;// '<Root>/VentCtrl_In'
  tb_VentCtrl_Out *VentCtrl_rty_VentCtrl_Out;// '<Root>/VentCtrl_Out'

  // private member function(s) for subsystem '<Root>/ConsFlow'
  void VentCtrl_ConsFlow();

  // private member function(s) for subsystem '<Root>/ConsTens'
  void VentCtrl_ConsTens() const;

  // private member function(s) for subsystem '<S3>/ConsPres'
  void VentCtrl_ConsPres_Init();
  void VentCtrl_ConsPres_Reset();
  void VentCtrl_ConsPres();

  // private member function(s) for subsystem '<S3>/CourbMont'
  void VentCtrl_CourbMont_Init();
  void VentCtrl_CourbMont_Reset();
  void VentCtrl_CourbMont();

  // private member function(s) for subsystem '<Root>/PresMng'
  void VentCtrl_PresMng_Init();
  void VentCtrl_PresMng_Reset();
  void VentCtrl_PresMng();
};

extern VentCtrl::P_VentCtrl_T VentCtrl_rtP;

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
//  '<Root>' : 'VentCtrl'
//  '<S1>'   : 'VentCtrl/ConsFlow'
//  '<S2>'   : 'VentCtrl/ConsTens'
//  '<S3>'   : 'VentCtrl/PresMng'
//  '<S4>'   : 'VentCtrl/ConsFlow/ConstantFlowSpCalc'
//  '<S5>'   : 'VentCtrl/PresMng/ConsPres'
//  '<S6>'   : 'VentCtrl/PresMng/CourbMont'
//  '<S7>'   : 'VentCtrl/PresMng/ConsPres/PressureRangeControl'
//  '<S8>'   : 'VentCtrl/PresMng/ConsPres/SecondOrderSpFilter'
//  '<S9>'   : 'VentCtrl/PresMng/ConsPres/presMesfilt'
//  '<S10>'  : 'VentCtrl/PresMng/ConsPres/SecondOrderSpFilter/firstOpressSpFilt'
//  '<S11>'  : 'VentCtrl/PresMng/ConsPres/SecondOrderSpFilter/pressSpfilt'
//  '<S12>'  : 'VentCtrl/PresMng/CourbMont/presFilter'

#endif                                 // VentCtrl_h_

//
// File trailer for generated code.
//
// [EOF]
//
