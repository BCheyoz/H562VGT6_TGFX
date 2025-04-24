//
// File: WaterHeatSpCalc_types.h
//
// Code generated for Simulink model 'WaterHeatSpCalc'.
//
// Model version                  : 1.75
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Apr 24 11:22:03 2025
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
#ifndef WaterHeatSpCalc_types_h_
#define WaterHeatSpCalc_types_h_
#include <stdint.h>
#ifndef DEFINED_TYPEDEF_FOR_te_op_mode_
#define DEFINED_TYPEDEF_FOR_te_op_mode_

enum class te_op_mode
  : int32_t {
  HeatPumpTest = 0,
  AntiLegionella = 1,
  FullElec = 5,                        // Default value
  SmartGrid = 6,
  Hybrid = 9,
  Auto = 10,
  Eco = 20,
  Boost = 30,
  Holidays = 40
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_pers_nb_
#define DEFINED_TYPEDEF_FOR_ta_pers_nb_

using ta_pers_nb = uint8_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_heat_mode_
#define DEFINED_TYPEDEF_FOR_te_heat_mode_

enum class te_heat_mode
  : int32_t {
  v40_ctrl = 0,                        // Default value
  temp_ctrl
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_wtr_vol_
#define DEFINED_TYPEDEF_FOR_ta_wtr_vol_

using ta_wtr_vol = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_temp_
#define DEFINED_TYPEDEF_FOR_ta_temp_

using ta_temp = int16_t;

#endif
#endif                                 // WaterHeatSpCalc_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
