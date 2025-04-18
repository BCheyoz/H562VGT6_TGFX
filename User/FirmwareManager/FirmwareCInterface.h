#ifndef FIRMWARE_MANAGER_C_INTERFACE_H
#define FIRMWARE_MANAGER_C_INTERFACE_H

#include <stdint.h>

//#define USE_SAV_STATE
//#define USE_COMMISIONNING_STATE
#define USE_ALIVE_LED

typedef enum
{
	E_BOOT_STATE = 0, // Etat réserver au Bootlaoder
	E_BOARD_READY_STATE, // Etat initial de la partie applicatif
#ifdef USE_SAV_STATE
	E_BOARD_SAV_READY_STATE = 2,
#endif
#ifdef USE_COMMISIONNING_STATE
	E_PRODUCT_READY_STATE = 3,
#endif
	E_PRODUCT_COMPLETE_STATE = 4,
	E_FACTORY_BENCH_STATE,
	E_FACTORY_STATE,
} e_softState;

/********************************************************************************************/
// pour compatibilité avec les libs en C
#ifdef __cplusplus
extern "C" {
#endif
void handleFirmwareManager_RT_100ms();

e_softState firmwareState();
void requestToSwitchToFactoryState(uint16_t value);

void requestProductReset(uint16_t value);
void requestResetMemories(uint16_t code);
uint16_t resetMemoriesState();

void requestToInitRegulation(uint16_t value);

#ifdef USE_COMMISIONNING_STATE
void resetCommissionningState(uint8_t code);
void requestEndOfCommissionning(uint8_t code);
void requestConfigReset(uint8_t code);
#endif

#ifdef USE_SAV_STATE
void requestSAVreset(uint8_t code);
#endif

#ifdef USE_ALIVE_LED
void requestBlinkMode(uint16_t newBlinkMode);
uint16_t blinkMode();
#endif

/********************************************************************************************/
// User varaible
uint8_t isAnodeFlags();
uint8_t isAnodeState();
uint8_t getControlTick();
uint8_t getWaterHeatCtrlTick();
uint8_t getInputMngTick();
void setControlTick(uint8_t v);
void setWaterHeatCtrlTick(uint8_t v);
void setInputMngTick(uint8_t v);

#define GET_SET_CC_UINT16_PROTOTYPE(a)	uint16_t get##a(void); void set##a(uint16_t val);
#define GET_SET_CC_FLOAT_PROTOTYPE(a)	float get##a(void); void set##a(float val);

GET_SET_CC_UINT16_PROTOTYPE(Ss_heat_mode_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_v40_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_temp_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_heat_pump_rot_spd_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Ss_elec_bstr_htr_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_pres_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_flow_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_vltg_sp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_tank_down_temp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_tank_up_temp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_pump_xhst_temp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_pump_evap_temp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_temp_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_pres_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_down_temp_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_up_temp_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_xhst_temp_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_evap_temp_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_temp_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_pres_err_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_pump_xhst_pres_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_pump_evap_pres_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_v40_min_simu)
GET_SET_CC_UINT16_PROTOTYPE(Cs_vent_rot_spd_simu)
GET_SET_CC_UINT16_PROTOTYPE(Bs_heat_mode_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_v40_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_temp_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_heat_pump_freq_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_elec_bstr_htr_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_pres_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_flow_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_vltg_sp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_down_temp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_up_temp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_xhst_temp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_evap_temp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_temp_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_pres_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_down_temp_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_tank_up_temp_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_xhst_temp_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_evap_temp_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_temp_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_pres_err_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_xhst_pres_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_pump_evap_pres_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_v40_min_simu_ena)
GET_SET_CC_UINT16_PROTOTYPE(Bs_vent_rot_spd_simu_ena)

GET_SET_CC_FLOAT_PROTOTYPE(pressSpfilt_K)
GET_SET_CC_FLOAT_PROTOTYPE(firstOpressSpFilt_K)
GET_SET_CC_FLOAT_PROTOTYPE(presMesfilt_K)
GET_SET_CC_FLOAT_PROTOTYPE(presFilter_K)
GET_SET_CC_FLOAT_PROTOTYPE(pressSpfilt_Tau)
GET_SET_CC_FLOAT_PROTOTYPE(firstOpressSpFilt_Tau)
GET_SET_CC_FLOAT_PROTOTYPE(presMesfilt_Tau)
GET_SET_CC_FLOAT_PROTOTYPE(presFilter_Tau)
GET_SET_CC_FLOAT_PROTOTYPE(pressSpfilt_initVal)
GET_SET_CC_FLOAT_PROTOTYPE(firstOpressSpFilt_initVal)
GET_SET_CC_FLOAT_PROTOTYPE(presMesfilt_initVal)
GET_SET_CC_FLOAT_PROTOTYPE(presFilter_initVal)
GET_SET_CC_FLOAT_PROTOTYPE(pressSpfilt_sampleTime)
GET_SET_CC_FLOAT_PROTOTYPE(firstOpressSpFilt_sampleTime)
GET_SET_CC_FLOAT_PROTOTYPE(presMesfilt_sampleTime)
GET_SET_CC_FLOAT_PROTOTYPE(presFilter_sampleTime)
GET_SET_CC_FLOAT_PROTOTYPE(press_max_Value)
GET_SET_CC_FLOAT_PROTOTYPE(press_min_Value)
GET_SET_CC_FLOAT_PROTOTYPE(flow_max_Value)
GET_SET_CC_FLOAT_PROTOTYPE(flow_min_Value)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_InitVal)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_Kawu)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_Kd)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_Ki)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_Kp)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_SampleTime)
GET_SET_CC_FLOAT_PROTOTYPE(PressureRegulator_Tau_f)
GET_SET_CC_UINT16_PROTOTYPE(Cs_flow_fan_sp_C_Value)
GET_SET_CC_UINT16_PROTOTYPE(NullFlowConstant_Value)
GET_SET_CC_UINT16_PROTOTYPE(flowEsti_InitialCondition)

#ifdef __cplusplus
}
#endif
/********************************************************************************************/


#endif // FIRMWARE_MANAGER_C_INTERFACE_H
