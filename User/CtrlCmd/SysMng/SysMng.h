//
// File: SysMng.h
//
// Code generated for Simulink model 'SysMng'.
//
// Model version                  : 1.149
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 17 12:15:32 2025
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
#ifndef SysMng_h_
#define SysMng_h_
#include <stdbool.h>
#include <stdint.h>
#include "SysMng_types.h"
#include "R_S_FlipFlop.h"
#include "RisingEdgeDelay.h"
#include "RisingEdgeDetect.h"
#include "Timer.h"
#include <cstring>

// Model block global parameters (default storage)
extern uint8_t rtP_Control_tick_C;     // Variable: Control_tick_C
                                          //  Referenced by: '<S13>/RisingEdgeDelay'


// Class declaration for model SysMng
class SysMng final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'SysMng'
  struct DW_SysMng_T {
    te_on_off Ss_heat_pump_ena;        // '<Root>/modeSelection'
    te_on_off Ss_elec_bstr_htr_ena;    // '<Root>/modeSelection'
    te_op_mode opModeDelay;            // '<Root>/opModeDelay'
    te_op_mode Ss_op_mode;             // '<Root>/modeSelection'
    te_op_mode opModeDelay_DSTATE;     // '<Root>/opModeDelay'
    ta_time_day conv2Days;             // '<S3>/conv2Days'
    uint8_t is_active_c3_SysMng;       // '<Root>/modeSelection'
    uint8_t is_c3_SysMng;              // '<Root>/modeSelection'
    uint8_t is_SystemMode;             // '<Root>/modeSelection'
    uint8_t is_AntiLegionella;         // '<Root>/modeSelection'
    uint8_t is_ModeSelection;          // '<Root>/modeSelection'
    uint8_t is_HeatPumpUsed;           // '<Root>/modeSelection'
    uint8_t is_UsualMode;              // '<Root>/modeSelection'
    uint8_t is_HeatPumpMode;           // '<Root>/modeSelection'
    uint8_t is_SmartGrid;              // '<Root>/modeSelection'
    bool AntiLegionnellaModeActDelay;  // '<Root>/AntiLegionnellaModeActDelay'
    bool HolidaysModeActDelay;         // '<Root>/HolidaysModeActDelay'
    bool SmartGridActDelay;            // '<Root>/SmartGridActDelay'
    bool bstModeActDelay;              // '<Root>/bstModeActDelay'
    bool heatPumpTestModeActDelay;     // '<Root>/heatPumpTestModeActDelay'
    bool Bs_clk_on;                    // '<Root>/modeSelection'
    bool isSgOn;                       // '<S9>/isSgOn'
    bool stt;                          // '<S14>/R_S_FlipFlop'
    bool stt_i;                        // '<S12>/R_S_FlipFlop'
    bool stt_a;                        // '<S11>/R_S_FlipFlop'
    bool stt_d;                        // '<S10>/R_S_FlipFlop'
    bool AntiLegionnellaModeActDelay_DST;// '<Root>/AntiLegionnellaModeActDelay'
    bool HolidaysModeActDelay_DSTATE;  // '<Root>/HolidaysModeActDelay'
    bool SmartGridActDelay_DSTATE;     // '<Root>/SmartGridActDelay'
    bool bstModeActDelay_DSTATE;       // '<Root>/bstModeActDelay'
    bool heatPumpTestModeActDelay_DSTATE;// '<Root>/heatPumpTestModeActDelay'
  };

  // Parameters (default storage)
  struct P_SysMng_T {
    float RisingEdgeDelay_delay;       // Mask Parameter: RisingEdgeDelay_delay
                                          //  Referenced by: '<S13>/RisingEdgeDelay'

    float secTimer_sampleTime;         // Mask Parameter: secTimer_sampleTime
                                          //  Referenced by: '<S15>/Timer'

    te_heat_stt Constant4_Value;       // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S5>/Constant4'

    te_heat_stt Constant6_Value;       // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S5>/Constant6'

    te_heat_stt Constant7_Value;       // Expression: te_heat_stt.Waiting
                                          //  Referenced by: '<S5>/Constant7'

    te_heat_stt Constant8_Value;       // Expression: te_heat_stt.Waiting
                                          //  Referenced by: '<S5>/Constant8'

    te_heat_stt Constant3_Value;       // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S6>/Constant3'

    te_heat_stt Constant5_Value;       // Expression: te_heat_stt.Waiting
                                          //  Referenced by: '<S6>/Constant5'

    te_heat_stt Constant4_Value_j;     // Expression: te_heat_stt.Stopped
                                          //  Referenced by: '<S6>/Constant4'

    te_heat_stt Constant6_Value_j;     // Expression: te_heat_stt.Waiting
                                          //  Referenced by: '<S6>/Constant6'

    te_on_off Contant_Value;           // Expression: te_on_off.on
                                          //  Referenced by: '<S5>/Contant'

    te_on_off Constant_Value;          // Expression: te_on_off.on
                                          //  Referenced by: '<S6>/Constant'

    te_on_off Constant1_Value;         // Expression: te_on_off.on
                                          //  Referenced by: '<S6>/Constant1'

    te_on_off Constant2_Value;         // Expression: te_on_off.off
                                          //  Referenced by: '<S6>/Constant2'

    te_on_off Constant4_Value_i;       // Expression: te_on_off.on
                                          //  Referenced by: '<S8>/Constant4'

    te_on_off Constant1_Value_k;       // Expression: te_on_off.on
                                          //  Referenced by: '<S8>/Constant1'

    te_on_off Constant5_Value_a;       // Expression: te_on_off.on
                                          //  Referenced by: '<S9>/Constant5'

    te_on_off on_Value;                // Expression: te_on_off.on
                                          //  Referenced by: '<S2>/on'

    te_op_mode Constant2_Value_i;      // Expression: te_op_mode.Boost
                                          //  Referenced by: '<S5>/Constant2'

    te_op_mode Constant7_Value_h;      // Expression: te_op_mode.AntiLegionella
                                          //  Referenced by: '<S6>/Constant7'

    te_op_mode HeatPumpTestCstt_Value; // Expression: te_op_mode.HeatPumpTest
                                          //  Referenced by: '<S7>/HeatPumpTestCstt'

    te_op_mode Constant5_Value_p;      // Expression: te_op_mode.Holidays
                                          //  Referenced by: '<S8>/Constant5'

    te_op_mode opModeDelay_InitialCondition;// Expression: te_op_mode.FullElec
                                               //  Referenced by: '<Root>/opModeDelay'

    te_sys_ver Collective_Value;       // Expression: te_sys_ver.Collective
                                          //  Referenced by: '<S2>/Collective'

    te_tech_mode Constant1_Value_a;    // Expression: te_tech_mode.HeatPump
                                          //  Referenced by: '<S5>/Constant1'

    te_tech_mode Constant_Value_i;     // Expression: te_tech_mode.FullElec
                                          //  Referenced by: '<S8>/Constant'

    te_vent_op_mode Inoperative_Value; // Expression: te_vent_op_mode.Inoperative
                                          //  Referenced by: '<S2>/Inoperative'

    te_vent_op_mode Stopped_Value;     // Expression: te_vent_op_mode.Stopped
                                          //  Referenced by: '<S2>/Stopped'

    te_vent_op_mode ConstantFlow_Value;// Expression: te_vent_op_mode.ConstantFlow
                                          //  Referenced by: '<S2>/ConstantFlow'

    te_vent_op_mode Normal_Value;      // Expression: te_vent_op_mode.Normal
                                          //  Referenced by: '<S2>/Normal'

    bool AntiLegionnellaModeActDelay_Ini;// Computed Parameter: AntiLegionnellaModeActDelay_Ini
                                            //  Referenced by: '<Root>/AntiLegionnellaModeActDelay'

    bool HolidaysModeActDelay_InitialCon;// Computed Parameter: HolidaysModeActDelay_InitialCon
                                            //  Referenced by: '<Root>/HolidaysModeActDelay'

    bool SmartGridActDelay_InitialCondit;// Computed Parameter: SmartGridActDelay_InitialCondit
                                            //  Referenced by: '<Root>/SmartGridActDelay'

    bool bstModeActDelay_InitialConditio;// Computed Parameter: bstModeActDelay_InitialConditio
                                            //  Referenced by: '<Root>/bstModeActDelay'

    bool heatPumpTestModeActDelay_Initia;// Computed Parameter: heatPumpTestModeActDelay_Initia
                                            //  Referenced by: '<Root>/heatPumpTestModeActDelay'

    uint8_t OneMoreDay_Bias;           // Computed Parameter: OneMoreDay_Bias
                                          //  Referenced by: '<S8>/OneMoreDay'

  };

  // model initialize function
  void initialize();

  // Initial conditions function
  void init();

  // model step function
  void step(const tb_SysMng_In *rtu_SysMng_In, tb_SysMng_Out *rty_SysMng_Out);

  // Copy Constructor
  SysMng(SysMng const&) = delete;

  // Assignment Operator
  SysMng& operator= (SysMng const&) & = delete;

  // Move Constructor
  SysMng(SysMng &&) = delete;

  // Move Assignment Operator
  SysMng& operator= (SysMng &&) = delete;

  // Tunable parameters
  static P_SysMng_T SysMng_rtP;

  // Reset function
  void reset();

  // Constructor
  SysMng();

  // Destructor
  ~SysMng();

  // private data and function members
 private:
  // Block states
  DW_SysMng_T SysMng_DW;

  // Declare private class scope variables for system: "model 'SysMng'"
  const tb_SysMng_In *SysMng_rtu_SysMng_In;// '<Root>/SysMng_In'
  tb_SysMng_Out *SysMng_rty_SysMng_Out;// '<Root>/SysMng_Out'

  // private member function(s) for subsystem '<S1>/BoostModeActivation'
  void SysMn_BoostModeActivation_Reset();
  void SysMng_BoostModeActivation();

  // private member function(s) for subsystem '<S1>/antiLegionellaModeActivation'
  void antiLegionellaModeActivat_Reset();
  void Sy_antiLegionellaModeActivation();

  // private member function(s) for subsystem '<S1>/heatPumpTestModeActivation'
  void heatPumpTestModeActivatio_Reset();
  void SysM_heatPumpTestModeActivation();

  // private member function(s) for subsystem '<S1>/holidaysModeActivation'
  void Sy_holidaysModeActivation_Reset();
  void SysMng_holidaysModeActivation();

  // private member function(s) for subsystem '<S1>/smartGridModeActivation'
  void SysMng_smartGridModeActivation();

  // private member function(s) for subsystem '<Root>/ConditionForModeSelection'
  void ConditionForModeSelection_Reset();
  void SysMn_ConditionForModeSelection();

  // private member function(s) for subsystem '<Root>/VentModeSelection'
  void SysMng_VentModeSelection() const;

  // private member function(s) for subsystem '<Root>/dayCounter'
  void SysMng_dayCounter_Reset();
  void SysMng_dayCounter();

  // private member function(s) for subsystem '<Root>/modeSelection'
  void SysMng_modeSelection_Init();
  void SysMng_modeSelection_Reset();
  void SysMng_modeSelection();
  void Sys_enter_internal_HeatPumpUsed(void);
  void SysMng_ModeSelection(void);

  // model instance variable for '<S5>/BoostRqstRisingEdge'
  RisingEdgeDetect BoostRqstRisingEdgeMDLOBJ1;

  // model instance variable for '<S10>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ2;

  // model instance variable for '<S11>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ3;

  // model instance variable for '<S7>/HeatPumpTestRqstRisingEdge'
  RisingEdgeDetect HeatPumpTestRqstRisiMDLOBJ4;

  // model instance variable for '<S7>/HeatPumpTestRqstRstRisingEdge'
  RisingEdgeDetect HeatPumpTestRqstRstRMDLOBJ5;

  // model instance variable for '<S12>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ6;

  // model instance variable for '<S13>/RisingEdgeDelay'
  RisingEdgeDelay RisingEdgeDelayMDLOBJ7;

  // model instance variable for '<S14>/R_S_FlipFlop'
  R_S_FlipFlop R_S_FlipFlopMDLOBJ8;

  // model instance variable for '<S8>/clkRchRisingEdge'
  RisingEdgeDetect clkRchRisingEdgeMDLOBJ9;

  // model instance variable for '<S8>/hldyRqstRisingEdge'
  RisingEdgeDetect hldyRqstRisingEdgeMDLOBJ10;

  // model instance variable for '<S15>/Timer'
  Timer TimerMDLOBJ11;
};

extern SysMng::P_SysMng_T SysMng_rtP;

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
//  '<Root>' : 'SysMng'
//  '<S1>'   : 'SysMng/ConditionForModeSelection'
//  '<S2>'   : 'SysMng/VentModeSelection'
//  '<S3>'   : 'SysMng/dayCounter'
//  '<S4>'   : 'SysMng/modeSelection'
//  '<S5>'   : 'SysMng/ConditionForModeSelection/BoostModeActivation'
//  '<S6>'   : 'SysMng/ConditionForModeSelection/antiLegionellaModeActivation'
//  '<S7>'   : 'SysMng/ConditionForModeSelection/heatPumpTestModeActivation'
//  '<S8>'   : 'SysMng/ConditionForModeSelection/holidaysModeActivation'
//  '<S9>'   : 'SysMng/ConditionForModeSelection/smartGridModeActivation'
//  '<S10>'  : 'SysMng/ConditionForModeSelection/BoostModeActivation/R_S_Boost'
//  '<S11>'  : 'SysMng/ConditionForModeSelection/antiLegionellaModeActivation/R_S_anti_Lgn'
//  '<S12>'  : 'SysMng/ConditionForModeSelection/heatPumpTestModeActivation/R_S_anti_Lgn'
//  '<S13>'  : 'SysMng/ConditionForModeSelection/heatPumpTestModeActivation/RisingEdgeDelay'
//  '<S14>'  : 'SysMng/ConditionForModeSelection/holidaysModeActivation/R_S_Holidays'
//  '<S15>'  : 'SysMng/dayCounter/secTimer'

#endif                                 // SysMng_h_

//
// File trailer for generated code.
//
// [EOF]
//
