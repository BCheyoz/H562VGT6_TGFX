//
// File: SysConfigCalib.h
//
// Code generated for Simulink model 'SysConfigCalib'.
//
// Model version                  : 1.22
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr  3 17:24:00 2025
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
#ifndef SysConfigCalib_h_
#define SysConfigCalib_h_
#include <stdbool.h>
#include <stdint.h>
#include "SysConfigCalib_types.h"
#include <cstring>

// Class declaration for model SysConfigCalib
class SysConfigCalib final
{
  // public data and function members
 public:
  // Parameters (default storage)
  struct P_SysConfigCalib_T {
    ta_wtr_vol Cs_180L_tank_vol_up_C_Value;// Expression: Cs_180L_tank_vol_up_C
                                              //  Referenced by: '<Root>/Cs_180L_tank_vol_up_C'

    ta_wtr_vol Cs_180L_tank_vol_btwn_C_Value;// Expression: Cs_180L_tank_vol_btwn_C
                                                //  Referenced by: '<Root>/Cs_180L_tank_vol_btwn_C'

    ta_wtr_vol Cs_180L_tank_vol_down_C_Value;// Expression: Cs_180L_tank_vol_down_C
                                                //  Referenced by: '<Root>/Cs_180L_tank_vol_down_C'

    ta_wtr_vol Cs_180L_tank_vol_tot_C_Value;// Expression: Cs_180L_tank_vol_tot_C
                                               //  Referenced by: '<Root>/Cs_180L_tank_vol_tot_C'

    ta_wtr_vol Cs_105L_tank_vol_up_C_Value;// Expression: Cs_105L_tank_vol_up_C
                                              //  Referenced by: '<Root>/Cs_105L_tank_vol_up_C'

    ta_wtr_vol Cs_105L_tank_vol_btwn_C_Value;// Expression: Cs_105L_tank_vol_btwn_C
                                                //  Referenced by: '<Root>/Cs_105L_tank_vol_btwn_C'

    ta_wtr_vol Cs_105L_tank_vol_down_C_Value;// Expression: Cs_105L_tank_vol_down_C
                                                //  Referenced by: '<Root>/Cs_105L_tank_vol_down_C'

    ta_wtr_vol Cs_105L_tank_vol_tot_C_Value;// Expression: Cs_105L_tank_vol_tot_C
                                               //  Referenced by: '<Root>/Cs_105L_tank_vol_tot_C'

  };

  // Copy Constructor
  SysConfigCalib(SysConfigCalib const&) = delete;

  // Assignment Operator
  SysConfigCalib& operator= (SysConfigCalib const&) & = delete;

  // Move Constructor
  SysConfigCalib(SysConfigCalib &&) = delete;

  // Move Assignment Operator
  SysConfigCalib& operator= (SysConfigCalib &&) = delete;

  // model step function
  void step(const tb_Control_In *rtu_Control_In, tb_SysConfigCalib_Out
            *rty_SysConfigCalib_Out) const;

  // Constructor
  SysConfigCalib();

  // Destructor
  ~SysConfigCalib();

  // private data and function members
 private:
  // Tunable parameters
  static P_SysConfigCalib_T SysConfigCalib_rtP;
};

extern SysConfigCalib::P_SysConfigCalib_T SysConfigCalib_rtP;

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
//  '<Root>' : 'SysConfigCalib'

#endif                                 // SysConfigCalib_h_

//
// File trailer for generated code.
//
// [EOF]
//
