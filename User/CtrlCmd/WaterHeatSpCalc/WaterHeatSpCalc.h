//
// File: WaterHeatSpCalc.h
//
// Code generated for Simulink model 'WaterHeatSpCalc'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 10:51:28 2025
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
#ifndef WaterHeatSpCalc_h_
#define WaterHeatSpCalc_h_
#include <stdbool.h>
#include <stdint.h>
#include "WaterHeatSpCalc_types.h"
#include <cstring>

// Class declaration for model WaterHeatSpCalc
class WaterHeatSpCalc final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'WaterHeatSpCalc'
  struct DW_WaterHeatSpCalc_T {
    ta_wtr_vol Cs_Boost_v40_sp_DSTATE; // '<S2>/Cs_Boost_v40_sp'
  };

  // Parameters (default storage)
  struct P_WaterHeatSpCalc_T {
    te_heat_mode v40_ctrl_Value;       // Expression: te_heat_mode.v40_ctrl
                                          //  Referenced by: '<S1>/v40_ctrl'

    te_heat_mode temp_ctrl_Value;      // Expression: te_heat_mode.temp_ctrl
                                          //  Referenced by: '<S1>/temp_ctrl'

    ta_temp Cs_cons_hyb_C_Value;       // Expression: Cs_cons_hyb_C
                                          //  Referenced by: '<S3>/Cs_cons_hyb_C'

    ta_temp Cs_cons_AL_C_Value;        // Expression: Cs_cons_AL_C
                                          //  Referenced by: '<S3>/Cs_cons_AL_C'

    ta_temp Cs_cons_hol_C_Value;       // Expression: Cs_cons_hol_C
                                          //  Referenced by: '<S3>/Cs_cons_hol_C'

    ta_temp Cs_cons_test_pac_C_Value;  // Expression: Cs_cons_test_pac_C
                                          //  Referenced by: '<S3>/Cs_cons_test_pac_C'

    ta_temp NullConstant_Value;        // Computed Parameter: NullConstant_Value
                                          //  Referenced by: '<S3>/NullConstant'

    ta_wtr_vol Cs_bain_eco_C_Value;    // Expression: Cs_bain_eco_C
                                          //  Referenced by: '<S2>/Cs_bain_eco_C'

    ta_wtr_vol ConsEco_tableData[5];   // Expression: Ct_V40j_eco_C
                                          //  Referenced by: '<S2>/ConsEco'

    ta_wtr_vol ConsAuto_tableData[5];  // Expression: Ct_V40j_auto_C
                                          //  Referenced by: '<S2>/ConsAuto'

    ta_wtr_vol Cs_bain_auto_elec_C_Value;// Expression: Cs_bain_auto_elec_C
                                            //  Referenced by: '<S2>/Cs_bain_auto_elec_C'

    uint16_t Cs_Boost_v40_sp_InitialConditio;// Computed Parameter: Cs_Boost_v40_sp_InitialConditio
                                                //  Referenced by: '<S2>/Cs_Boost_v40_sp'

    ta_wtr_vol v40NullSpConstant_Value;// Computed Parameter: v40NullSpConstant_Value
                                          //  Referenced by: '<S2>/v40NullSpConstant'

    ta_pers_nb ConsEco_bp01Data[5];    // Expression: Ct_nb_pres_C
                                          //  Referenced by: '<S2>/ConsEco'

    ta_pers_nb ConsAuto_bp01Data[5];   // Expression: Ct_nb_pres_C
                                          //  Referenced by: '<S2>/ConsAuto'

  };

  // Initial conditions function
  void init();

  // model step function
  void step(const te_op_mode *rtu_Ss_op_mode, const ta_pers_nb *rtu_Ns_pers_nb, te_heat_mode *rty_Ss_heat_mode, ta_wtr_vol *rty_Cs_v40_sp, ta_temp
            *rty_Cs_temp_sp);

  // Copy Constructor
  WaterHeatSpCalc(WaterHeatSpCalc const&) = delete;

  // Assignment Operator
  WaterHeatSpCalc& operator= (WaterHeatSpCalc const&) & = delete;

  // Move Constructor
  WaterHeatSpCalc(WaterHeatSpCalc &&) = delete;

  // Move Assignment Operator
  WaterHeatSpCalc& operator= (WaterHeatSpCalc &&) = delete;

  // Reset function
  void reset();

  // Constructor
  WaterHeatSpCalc();

  // Destructor
  ~WaterHeatSpCalc();

  // private data and function members
 private:
  // Block states
  DW_WaterHeatSpCalc_T WaterHeatSpCalc_DW;

  // Tunable parameters
  static P_WaterHeatSpCalc_T WaterHeatSpCalc_rtP;

  // Declare private class scope variables for system: "model 'WaterHeatSpCalc'"
  const te_op_mode *WaterHeatSpCalc_rtu_Ss_op_mode;// '<Root>/Ss_op_mode'
  const ta_pers_nb *WaterHeatSpCalc_rtu_Ns_pers_nb;// '<Root>/Ns_pers_nb'
  te_heat_mode *WaterHeatSpCal_rty_Ss_heat_mode;// '<Root>/Ss_heat_mode'
  ta_wtr_vol *WaterHeatSpCalc_rty_Cs_v40_sp;// '<Root>/Cs_v40_sp'
  ta_temp *WaterHeatSpCalc_rty_Cs_temp_sp;// '<Root>/Cs_temp_sp'

  // private member function(s) for subsystem '<Root>/ConsModeChauf'
  void WaterHeatSpCalc_ConsModeChauf() const;

  // private member function(s) for subsystem '<Root>/consigne_V40'
  void WaterHeatSpCa_consigne_V40_Init();
  void WaterHeatSpC_consigne_V40_Reset();
  void WaterHeatSpCalc_consigne_V40();

  // private member function(s) for subsystem '<Root>/consigne_temp'
  void WaterHeatSpCalc_consigne_temp() const;
};

extern WaterHeatSpCalc::P_WaterHeatSpCalc_T WaterHeatSpCalc_rtP;

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
//  '<Root>' : 'WaterHeatSpCalc'
//  '<S1>'   : 'WaterHeatSpCalc/ConsModeChauf'
//  '<S2>'   : 'WaterHeatSpCalc/consigne_V40'
//  '<S3>'   : 'WaterHeatSpCalc/consigne_temp'

#endif                                 // WaterHeatSpCalc_h_

//
// File trailer for generated code.
//
// [EOF]
//
