//
// File: WaterHeatController_types.h
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
#ifndef WaterHeatController_types_h_
#define WaterHeatController_types_h_
#include <stdint.h>
#ifndef DEFINED_TYPEDEF_FOR_ta_temp_
#define DEFINED_TYPEDEF_FOR_ta_temp_

using ta_temp = int16_t;

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

#ifndef DEFINED_TYPEDEF_FOR_te_on_off_
#define DEFINED_TYPEDEF_FOR_te_on_off_

enum class te_on_off
  : int32_t {
  off = 0,                             // Default value
  on,
  forced
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rfrg_pres_
#define DEFINED_TYPEDEF_FOR_ta_rfrg_pres_

using ta_rfrg_pres = uint16_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ta_rot_spd_
#define DEFINED_TYPEDEF_FOR_ta_rot_spd_

using ta_rot_spd = uint16_t;

#endif

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

#ifndef DEFINED_TYPEDEF_FOR_te_pump_mode_
#define DEFINED_TYPEDEF_FOR_te_pump_mode_

enum class te_pump_mode
  : int32_t {
  Tank_lvl = 0,                        // Default value
  Tref,
  Tevap,
  Tdown,
  pumpOFF
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_te_tank_lvl_
#define DEFINED_TYPEDEF_FOR_te_tank_lvl_

enum class te_tank_lvl
  : int32_t {
  Empty = 0,                           // Default value
  Cold,
  Hot,
  Ready
};

#endif
#endif                                 // WaterHeatController_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
