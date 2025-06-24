/*
 * FirmwareStateMachine.cpp
 *
 *  Created on: 5 Feb 2025
 *  Original Author: B.Chhay
 *
 *	Main state machine of mother board
 *
 */

#include "FirmwareStateMachine.hpp"
#include "ParamProductInfo.h"
#include "utils.h"
#include "main.h"
#include "GestionInputSensor.h"
#include "FanPwmIcUser.h"
#include "EmbracoInverter.h"

#include "Display_FF028T010.h"
#include "imgTest.h"

#ifdef USE_COMMISIONNING_STATE
#define COMMISSIONNING_END_PWD      204
#define COMMISSIONNING_RESET_PWD    76
#define CONFIG_RESET_PWD            41
#endif

#ifdef USE_SAV_STATE
#define SAV_RESET_PWD               186
#endif

#define PRODUCT_RESET_PWD           242
#define FACTORY_LICENSE_KEY         147
#define BENCH_LICENSE_KEY         	197
#define REG_RESET_PWD               18
#define POWER_ON_WAIT               50     // 2 sec avec un pas de temps de 100ms
#define ERASE_MEM_KEY               3854

#define CTRL_CMD_TIMER 10 // cadencement à 1 sec : 10 * 100ms


/******************************************************************************/
// Initialisation des variables static partagé entre toutes les instances de l'objet
uint8_t FwMng::timer_100ms = 0;
FwMng *FwMng::d = nullptr;
tb_Control_In FwMng::cc_input = TFLOW4_Ctrl_rtZtb_Control_In; // initialise la structure avec les valeurs par defaut
tb_Control_Out FwMng::cc_out = TFLOW4_Ctrl_rtZtb_Control_Out; // initialise la structure avec les valeurs par defaut
TFLOW4_Ctrl::DW_TFLOW4_Ctrl_T FwMng::cc_DW;

FwMng * FwMng::getInstance(){
	FwMng *obj;
	if(d == nullptr){
		obj = new FwMng;
		d = obj;
	}
	else {
		obj = d;
	}

	return obj;
}

/******************************************************************************/
// Pour compatibilité avec les libs en C
extern "C" {
/* Interdiction de crée une instance d'objet en variable global ou static.
 * Car le compilateur tente de crée l'objet avant l'init des HAL !!
 * static FwMng *fwp = FwMng::getInstance(); -> interdit en global
 */

void handleFirmwareManager_RT_100ms(){FwMng::it_100ms();}


e_softState firmwareState(){
	FwMng *fwp = FwMng::getInstance();
	return fwp->getState();
}
void requestToSwitchToFactoryState(uint16_t value){
	FwMng *fwp = FwMng::getInstance();
	fwp->requestToSwitchToFactoryState(value);
}

void requestProductReset(uint16_t value){
	FwMng *fwp = FwMng::getInstance();
	fwp->requestProductReset(value);
}
void requestResetMemories(uint16_t code){
	FwMng *fwp = FwMng::getInstance();
	fwp->requestResetMemories(code);
}
uint16_t resetMemoriesState(){
	FwMng *fwp = FwMng::getInstance();
	return fwp->resetMemoriesState();
}

void requestToInitRegulation(uint16_t value){
	FwMng *fwp = FwMng::getInstance();
	fwp->requestToInitRegulation(value);
}

#ifdef USE_COMMISIONNING_STATE
void resetCommissionningState(uint8_t code){
	fwp->resetCommissionningState(code);
}
void requestEndOfCommissionning(uint8_t code){
	fwp->requestEndOfCommissionning(code);
}
void requestConfigReset(uint8_t code){
	fwp->requestConfigReset(code);
}
#endif

#ifdef USE_SAV_STATE
void requestSAVreset(uint8_t code){
	fwp->requestSAVreset(code);
}
#endif

#ifdef USE_ALIVE_LED
void requestBlinkMode(uint16_t newBlinkMode){
	FwMng *fwp = FwMng::getInstance();
	fwp->requestBlinkMode(newBlinkMode);
}
uint16_t blinkMode(){
	FwMng *fwp = FwMng::getInstance();
	return fwp->blinkMode();
}
#endif

/********************************************************************************************/
// User function
uint8_t isAnodeFlags(){
	FwMng *fwp = FwMng::getInstance();
	return fwp->isAnodeFlags();
}
uint8_t isAnodeState(){
	FwMng *fwp = FwMng::getInstance();
	return fwp->isAnodeState();
}

uint8_t getControlTick(){
	return rtP_Control_tick_C;
}

uint8_t getWaterHeatCtrlTick(){
	return rtP_WaterHeatCtrl_tick_C;
}

uint8_t getInputMngTick(){
	return rtP_input_mng_tick;
}

void setControlTick(uint8_t v){
	rtP_Control_tick_C = v;
}

void setWaterHeatCtrlTick(uint8_t v){
	rtP_WaterHeatCtrl_tick_C = v;
}

void setInputMngTick(uint8_t v){
	rtP_input_mng_tick = v;
}



#define GET_SET_CC_DEFINITION(a, b, c, d)		c get##a(void){return (c)FwMng::getInstance()->get##b()->a;} \
												void set##a(c val){FwMng::getInstance()->get##b()->a = (d)val;}

GET_SET_CC_DEFINITION(Ss_heat_mode_simu, CC_SimuVar, uint16_t, te_heat_mode)
GET_SET_CC_DEFINITION(Cs_v40_sp_simu, CC_SimuVar, uint16_t, ta_wtr_vol)
GET_SET_CC_DEFINITION(Cs_temp_sp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_heat_pump_rot_spd_sp_simu, CC_SimuVar, uint16_t, ta_rot_spd)
GET_SET_CC_DEFINITION(Ss_elec_bstr_htr_sp_simu, CC_SimuVar, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Cs_vent_pres_sp_simu, CC_SimuVar, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Cs_vent_flow_sp_simu, CC_SimuVar, uint16_t, ta_flow)
GET_SET_CC_DEFINITION(Cs_vent_vltg_sp_simu, CC_SimuVar, uint16_t, ta_vltg)
GET_SET_CC_DEFINITION(Cs_tank_down_temp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_tank_up_temp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_xhst_temp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_evap_temp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_temp_simu, CC_SimuVar, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_pres_simu, CC_SimuVar, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Bs_tank_down_temp_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_tank_up_temp_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_xhst_temp_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_evap_temp_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_temp_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_pres_err_simu, CC_SimuVar, uint16_t, bool)
GET_SET_CC_DEFINITION(Cs_pump_xhst_pres_simu, CC_SimuVar, uint16_t, ta_rfrg_pres)
GET_SET_CC_DEFINITION(Cs_pump_evap_pres_simu, CC_SimuVar, uint16_t, ta_rfrg_pres)
GET_SET_CC_DEFINITION(Cs_v40_min_simu, CC_SimuVar, uint16_t, ta_wtr_vol)
GET_SET_CC_DEFINITION(Cs_vent_rot_spd_simu, CC_SimuVar, uint16_t, ta_rot_spd)

GET_SET_CC_DEFINITION(Bs_heat_mode_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_v40_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_temp_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_heat_pump_freq_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_elec_bstr_htr_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_pres_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_flow_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_vltg_sp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_tank_down_temp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_tank_up_temp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_xhst_temp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_evap_temp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_temp_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_pres_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_tank_down_temp_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_tank_up_temp_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_xhst_temp_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_evap_temp_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_temp_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_pres_err_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_xhst_pres_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_pump_evap_pres_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_v40_min_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_vent_rot_spd_simu_ena, CC_SimuEna, uint16_t, bool)
GET_SET_CC_DEFINITION(Bs_err_dtct_shnt_ena, CC_HmiExpert, uint16_t, bool)
GET_SET_CC_DEFINITION(Ss_ctry, CC_HmiTech, uint16_t, te_ctry)
GET_SET_CC_DEFINITION(Ss_tech_mode, CC_HmiTech, uint16_t, te_tech_mode)
GET_SET_CC_DEFINITION(Ns_anti_lgn_day, CC_HmiTech, uint16_t, ta_time_day)
GET_SET_CC_DEFINITION(Ss_heat_pump_test_rqst, CC_HmiTech, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_tank_size, CC_HmiTech, uint16_t, te_tank_size)
GET_SET_CC_DEFINITION(Ss_sys_ver, CC_HmiTech, uint16_t, te_sys_ver)
GET_SET_CC_DEFINITION(Cs_vent_pres_min, CC_HmiTech, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Cs_vent_pres_sys, CC_HmiTech, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Ns_hldy_nb, CC_HmiUser, uint16_t, ta_time_day)
GET_SET_CC_DEFINITION(Ns_pers_nb, CC_HmiUser, uint16_t, ta_pers_nb)
GET_SET_CC_DEFINITION(Ss_user_mode, CC_HmiUser, uint16_t, te_user_mode)
GET_SET_CC_DEFINITION(Ss_anti_lgn_ena, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_heat_wtr_cnsp_rst, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_vent_cnsp_rst, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_tot_cnsp_rst, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_sg_mode_ena, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_oph_mode_ena, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_hldy_rqst, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_bst_rqst, CC_HmiUser, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Cs_tank_down_temp_raw, CC_HW, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_tank_up_temp_raw, CC_HW, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_xhst_temp_raw, CC_HW, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_evap_temp_raw, CC_HW, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_temp_raw, CC_HW, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_pres_raw, CC_HW, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Cs_vent_rot_spd_raw, CC_HW, uint16_t, ta_rot_spd)
GET_SET_CC_DEFINITION(Cs_heat_pump_pwr, CC_HW, uint16_t, ta_pwr)

GET_SET_CC_DEFINITION(Cs_v40_lvl, CC_output, uint16_t, uint8_t)
GET_SET_CC_DEFINITION(Ss_op_mode, CC_output, uint16_t, te_op_mode)
GET_SET_CC_DEFINITION(Ss_sg_stt, CC_output, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Ss_oph_stt, CC_output, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Cs_tank_down_temp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_tank_up_temp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_xhst_temp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_pump_evap_temp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_temp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Ss_elec_bstr_htr_sp, CC_output, uint16_t, te_on_off)
GET_SET_CC_DEFINITION(Cs_heat_pump_rot_spd_sp, CC_output, uint16_t, ta_rot_spd)
GET_SET_CC_DEFINITION(Cs_vent_rot_spd, CC_output, uint16_t, ta_rot_spd)
GET_SET_CC_DEFINITION(Cs_heat_wtr_cnsp, CC_output, uint16_t, ta_pwr)
GET_SET_CC_DEFINITION(Cs_tot_cnsp, CC_output, uint16_t, ta_pwr)
GET_SET_CC_DEFINITION(Cs_v40_sp, CC_output, uint16_t, ta_wtr_vol)
GET_SET_CC_DEFINITION(Cs_temp_sp, CC_output, uint16_t, ta_temp)
GET_SET_CC_DEFINITION(Cs_vent_pres_sp, CC_output, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Cs_vent_flow_sp, CC_output, uint16_t, ta_flow)
GET_SET_CC_DEFINITION(Cs_vent_vltg_sp, CC_output, uint16_t, ta_vltg)
GET_SET_CC_DEFINITION(Cs_vent_pres, CC_output, uint16_t, ta_air_pres)

GET_SET_CC_DEFINITION(Cs_pres_mes_filt, CC_subVentCtrlOutput, uint16_t, ta_air_pres)
GET_SET_CC_DEFINITION(Cs_vent_cnsp, CC_subInputSecuOutput, uint16_t, ta_egy)
GET_SET_CC_DEFINITION(Cs_vent_pwr, CC_subInputSecuOutput, uint16_t, ta_pwr)

GET_SET_CC_DEFINITION(pressSpfilt_K, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(firstOpressSpFilt_K, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presMesfilt_K, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presFilter_K, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(pressSpfilt_Tau, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(firstOpressSpFilt_Tau, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presMesfilt_Tau, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presFilter_Tau, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(pressSpfilt_initVal, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(firstOpressSpFilt_initVal, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presMesfilt_initVal, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presFilter_initVal, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(pressSpfilt_sampleTime, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(firstOpressSpFilt_sampleTime, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presMesfilt_sampleTime, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(presFilter_sampleTime, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(press_max_Value, CC_VentCtrlParam, float, double)
GET_SET_CC_DEFINITION(press_min_Value, CC_VentCtrlParam, float, double)
GET_SET_CC_DEFINITION(flow_max_Value, CC_VentCtrlParam, float, double)
GET_SET_CC_DEFINITION(flow_min_Value, CC_VentCtrlParam, float, double)
GET_SET_CC_DEFINITION(PressureRegulator_InitVal, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_Kawu, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_Kd, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_Ki, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_Kp, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_SampleTime, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(PressureRegulator_Tau_f, CC_VentCtrlParam, float, float)
GET_SET_CC_DEFINITION(Cs_flow_fan_sp_C_Value, CC_VentCtrlParam, uint16_t, ta_flow)
GET_SET_CC_DEFINITION(NullFlowConstant_Value, CC_VentCtrlParam, uint16_t, ta_flow)
GET_SET_CC_DEFINITION(flowEsti_InitialCondition, CC_VentCtrlParam, uint16_t, uint16_t)

GET_SET_CC_DEFINITION(FPresVent_Tau, CC_InputMngParam, float, float)

}
/*******************************************************************************************************/

FwMng::FwMng()
{
#ifdef USE_COMMISIONNING_STATE
	resetCommissionning = FALSE;
	commissionningParamDone = FALSE;
	commissionningParamSaved = FALSE;
	resetConfig = FALSE;
#endif

#ifdef USE_SAV_STATE
	savReset = FALSE;
#endif

	//TODO: pour les 1ere versions on force a être en E_PRODUCT_COMPLETE_STATE à basculer en E_BOARD_READY_STATE une fois que la partie parametrage est fait
	//state = E_BOARD_READY_STATE;
	state = E_PRODUCT_COMPLETE_STATE;
	timer_100ms = 0;
	ctrlCmdReset = TRUE;
	powerOnTimer = 0;
	eraseMemory = 0;
	regReset = TRUE;

#ifdef USE_ALIVE_LED
	ledAlive = new LedBlinker(LED_ALIVE_GPIO_Port, LED_ALIVE_Pin, E_LED_SLOW_BLINK);
#endif

/********************************************************************************************/
// User Init
	appointElec = new AppointElec(DO_Appoint_GPIO_Port, DO_Appoint_Pin);

	di_Anode = new DigitalInputs(DI_Anode_GPIO_Port, DI_Anode_Pin, DI_NO_WORKING_STATE_IS_1, E_SINGLE_INPUT);
	RegisterDigitalInput2EventFnHandler(DI_EVENT_NEW_STATE | DI_EVENT_NEW_WORK_STATE, di_Anode, HandleDI_Event);

	ctrlCmd = new TFLOW4_Ctrl;
	initCtrlCmd();

	/*
	TODO données récuperer de la mémoire et a MAJ lors d'action utilisateur
	cc_input.HMI.TECH.Ss_ctry; // te_ctry enum France = 33, Germany = 49, Spain = 34, Marocco = 212, WesternSahara = 213
	cc_input.HMI.TECH.Ss_tech_mode; // te_tech_mode enum tech_FullElec = 0, tech_Hybrid = 1, tech_HeatPump = 2
	cc_input.HMI.TECH.Ns_anti_lgn_day; // ta_time_day uint8 NB de jour d'activation
	cc_input.HMI.TECH.Ss_heat_pump_test_rqst; // te_on_off : enum off = 0; on = 1; force = 2
	cc_input.HMI.TECH.Ss_tank_size; // te_tank_size enum L105 = 105, L180 = 180
	cc_input.HMI.TECH.Ss_sys_ver; // te_sys_ver enum Collective = 0, Individual
	cc_input.HMI.TECH.Cs_vent_pres_min; // ta_air_pres uint16 Pa x10
	cc_input.HMI.TECH.Cs_vent_pres_sys; // ta_air_pres uint16 Pa x10
	cc_input.HMI.TECH.St_tor_mode[2]; // te_tor_mode enum tor_NotConnected = 0, tor_SmartGrid, tor_OffPeakHour, tor_VentSysStop

	cc_input.HMI.USER.Ns_hldy_nb; // ta_time_day uint8 Nb de jour de vacance
	cc_input.HMI.USER.Ns_pers_nb; // ta_pers_nb uint8 Nb de personne dans le foyer
	cc_input.HMI.USER.Ss_anti_lgn_ena; // te_on_off : enum off = 0; on = 1; force = 2
	*/


	/*** affiche une couleur uni ***********************************/
	const uint32_t size = LCD_WIDTH * LCD_HEIGHT;
	uint16_t pData[size];

	for(uint32_t i = 0; i < size; i++){
	  pData[i] = 3968; // Green
	}

	BSP_LCD_SetDisplayWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
	BSP_LCD_WriteData((uint8_t*)pData, size * BSP_LCD_GetPixelDepth());
}

void FwMng::run(void)
{
	if(timer_100ms == 0){
		return ;
	}

	timer_100ms = 0;

	if(powerOnTimer < POWER_ON_WAIT){
		powerOnTimer++;
		return;
	}

	const uint32_t size = LCD_WIDTH * LCD_HEIGHT * BSP_LCD_GetPixelDepth();

	switch(state)
	{
	case E_BOARD_READY_STATE:
#ifdef USE_ALIVE_LED
		ledAlive->SetBlinkMode(E_LED_SLOW_BLINK);
#endif
		// on reste en Board Ready tant qu'il n'y a pas de code carte
		if(isCodeBoardCompliant()){

			if( isCodeIdCompliant() == TRUE)
			{
#ifdef USE_COMMISIONNING_STATE
				if(isCommissionningDone() == FALSE)
					state = E_PRODUCT_READY_STATE;
				else
#endif
					state = E_PRODUCT_COMPLETE_STATE;
			}
#ifdef USE_SAV_STATE
			else {
				if(isSAVProduct() == TRUE){
					state = E_BOARD_SAV_READY_STATE;
				}
			}
#endif
		}
		break;

#ifdef USE_COMMISIONNING_STATE
	case E_PRODUCT_READY_STATE:
#ifdef USE_ALIVE_LED
		ledAlive->SetBlinkMode(E_LED_SLOW_BLINK);
#endif
		if(commissionningParamDone == TRUE){
			// on enregistre une fois quand la mise en service est terminé
			commissionningParamDone = FALSE;
			// sécurité, pas de reset mise en service une fois fini sinon on reste bloqué
			resetCommissionning = FALSE;
			resetConfig = FALSE;
			regReset = TRUE;
			setCommissionningState(1); // MES Fini
			state = E_PRODUCT_COMPLETE_STATE;
		}
		break;
#endif

	case E_PRODUCT_COMPLETE_STATE:
		/*** affiche l'image de test par defaut ***********************************/
		BSP_LCD_SetDisplayWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
		BSP_LCD_WriteData((uint8_t*)imgData, size);

#ifdef USE_ALIVE_LED
	ledAlive->SetBlinkMode(E_LED_HEARTBEAT_BLINK);
#endif
#ifdef USE_COMMISIONNING_STATE
        if(resetConfig == TRUE){
			resetConfig = FALSE;
			resetParamProduct();
			state = E_PRODUCT_READY_STATE;
			break;
		}

        if(resetCommissionning == TRUE){
			resetCommissionning = FALSE;
			setCommissionningState(0);
			state = E_PRODUCT_READY_STATE;
			break;
		}
#endif
#ifdef USE_SAV_STATE
		if(savReset == TRUE){
		    savReset = FALSE;
            // Cette fonction efface le Product ID et les réglages du système
		    resetParamProduct();
            state = E_BOARD_SAV_READY_STATE; // necessite le changement d'état avant reset ID
            WriteProductId(0);
            //setFanExhaustVoltage_mV(0);
            break;
		}
#endif
		if(regReset == TRUE){
			regReset = FALSE;
			initCtrlCmd();
		}
		else {
			if(powerOnTimer >= POWER_ON_WAIT){
				CtrlCmdTask();
			}
		}
		break;
#ifdef USE_SAV_STATE
	case E_BOARD_SAV_READY_STATE:
		if( isCodeIdCompliant() == TRUE)
		{
			state = E_BOARD_READY_STATE;
		}
		break;
#endif
	case E_FACTORY_STATE:
	case E_FACTORY_BENCH_STATE:
#ifdef USE_SAV_STATE
	    if(savReset == TRUE){
            savReset = FALSE;
            // Cette fonction efface le Product ID et les réglages du système
            resetParamProduct();
            WriteProductId(0);
            setFanExhaustVoltage_mV(0);
        }
#endif
	    if(eraseMemory == 1){
	        eraseMemory++;
            setFanExhaustVoltage_mV(0);
	        //MemoryChipErase(); // effacement de la mémoire Histo + App de maj
	        //initNvmemAuxValeursParDefaut();   // effacement des parametrages
            eraseMemory = 0;
	    }

		// rien a faire, c'est le boulot du module modbus de repondre aux requetes
		break;

	case E_BOOT_STATE: // Boot reserver au bootloader
	default:
#ifdef USE_ALIVE_LED
		ledAlive->SetBlinkMode(E_LED_FAST_BLINK);
#endif
		// robustness case. We shall never get here
		state = E_BOARD_READY_STATE;
	}
}

/******************************************************************************/
void FwMng::it_100ms(){
	timer_100ms++;
}

void FwMng::requestToSwitchToFactoryState(uint16_t value)
{
	if(FACTORY_LICENSE_KEY == value){
		state = E_FACTORY_STATE;
	}
	else if(BENCH_LICENSE_KEY == value){
		state = E_FACTORY_BENCH_STATE;
	}
}

/******************************************************************************/
void FwMng::requestProductReset(uint16_t value){
	// fonction autorisee uniquement en factory state et si le mdp est bon
	if((E_FACTORY_STATE == state) && (PRODUCT_RESET_PWD == value))
		resetCodeProduct();
}

void FwMng::requestResetMemories(uint16_t code){
    if(E_FACTORY_STATE != state || code != ERASE_MEM_KEY
            || eraseMemory != 0){ return;} // on accept pas la demande s'il est déja en cours
        eraseMemory = 1;
}
uint16_t FwMng::resetMemoriesState() { return eraseMemory; }

void FwMng::requestToInitRegulation(uint16_t value){
	if(value == REG_RESET_PWD){
		regReset = TRUE;
	}
}

/******************************************************************************/
#ifdef USE_COMMISIONNING_STATE
void FwMng::requestConfigReset(uint8_t code){
	if(E_PRODUCT_COMPLETE_STATE != state || code != CONFIG_RESET_PWD){ return;}
	resetConfig = TRUE;
}

void FwMng::resetCommissionningState(uint8_t code){
	if(E_PRODUCT_COMPLETE_STATE != state || code != COMMISSIONNING_RESET_PWD){ return;}
	resetCommissionning = TRUE;
}

void FwMng::requestEndOfCommissionning(uint8_t code){
	if(E_PRODUCT_READY_STATE != state || code != COMMISSIONNING_END_PWD){ return;}
	commissionningParamDone = TRUE;
}
#endif

#ifdef USE_SAV_STATE
void FwMng::requestSAVreset(uint8_t code){
	//HII-2140 - La fonction SAV Reset est disponible que pour des carte avec un code SAP en 111xxxxx
	if(E_PRODUCT_COMPLETE_STATE == state || code != SAV_RESET_PWD){ return;}
		savReset = TRUE;
}
#endif

/******************************************************************************/
#ifdef USE_ALIVE_LED
void FwMng::requestBlinkMode(uint16_t newBlinkMode){
	if(E_FACTORY_STATE == state)	// HII-1264: fonction autorisee uniquement en factory state
	{
		ledAlive->SetBlinkMode((E_LED_BLINK_MODES)newBlinkMode);
	}
}
#endif

void FwMng::initCtrlCmd(){
	if(ctrlCmd == nullptr) return;

	ctrlCmd->initialize();
	ctrlCmdCounter = 0;

	// Calibration
//	float Cs_reg_pres_tau_1_C = 8;
//	float Cs_reg_pres_tau2_C = 3;
//	float Cs_reg_pres_gain_C = 0.05;
	VentCtrl::VentCtrl_rtP.PressureRegulator_Kd = 1.32; //Cs_reg_pres_tau_1_C * Cs_reg_pres_tau2_C * Cs_reg_pres_gain_C;
	VentCtrl::VentCtrl_rtP.PressureRegulator_Ki = 0.04; //Cs_reg_pres_gain_C;
	VentCtrl::VentCtrl_rtP.PressureRegulator_Kp = 0.56; //(Cs_reg_pres_tau_1_C + Cs_reg_pres_tau2_C)* Cs_reg_pres_gain_C;
	VentCtrl::VentCtrl_rtP.presFilter_Tau = 0.05;
	VentCtrl::VentCtrl_rtP.presMesfilt_Tau = 0.05;
	InPutMng::InPutMng_rtP.FPresVent_Tau = 8;

	// set default value
	cc_input = TFLOW4_Ctrl_rtZtb_Control_In; // initialise la structure avec les valeurs par defaut
	cc_out = TFLOW4_Ctrl_rtZtb_Control_Out; // initialise la structure avec les valeurs par defaut
	cc_input.HMI.TECH.Cs_vent_pres_min = 1050;
	cc_input.HMI.TECH.Cs_vent_pres_sys = 1050;
	cc_input.HMI.TECH.Ss_sys_ver = te_sys_ver::Individual;
	cc_input.HMI.TECH.Ss_ctry = te_ctry::France;
	cc_input.HMI.TECH.Ss_tech_mode = te_tech_mode::HeatPump;
	cc_input.HMI.TECH.Ss_tank_size = te_tank_size::L180;
	cc_input.HMI.USER.Ss_user_mode = te_user_mode::Eco;
	cc_input.HMI.USER.Ns_pers_nb = 3;
}

void FwMng::CtrlCmdTask(){
	if(ctrlCmdCounter < CTRL_CMD_TIMER){
		ctrlCmdCounter++;
		return;
	}

	ctrlCmdCounter = 0;

	// maj des données d'entrées ******************************************
/* TODO
	cc_input.HMI.USER.Ss_user_mode; // te_user_mode : enum Auto = 10; Eco = 20; Boost = 30; Holidays = 40
	cc_input.HMI.USER.Ss_heat_wtr_cnsp_rst; // te_on_off : enum off = 0; on = 1; force = 2 reset conso ECS
	cc_input.HMI.USER.Ss_vent_cnsp_rst; // te_on_off : enum off = 0; on = 1; force = 2 reset conso FAN
	cc_input.HMI.USER.Ss_tot_cnsp_rst; // te_on_off : enum off = 0; on = 1; force = 2 reset conso Global
	cc_input.HMI.USER.Ss_sg_mode_ena; // te_on_off : enum off = 0; on = 1; force = 2
	cc_input.HMI.USER.Ss_oph_mode_ena; // te_on_off : enum off = 0; on = 1; force = 2
	cc_input.HMI.USER.Ss_hldy_rqst; // te_on_off : enum off = 0; on = 1; force = 2
	cc_input.HMI.USER.Ss_bst_rqst; // te_on_off : enum off = 0; on = 1; force = 2
*/
	cc_input.HW.Cs_tank_down_temp_raw = getCtn(0) / 10.; // ta_temps : int16 °C x10  // valeur retourné en °C *100
	cc_input.HW.Cs_tank_up_temp_raw = getCtn(1) / 10.; // ta_temps : int16 °C x10
	cc_input.HW.Cs_pump_xhst_temp_raw = getCtn(2) / 10.; // ta_temps : int16 °C x10
	cc_input.HW.Cs_pump_evap_temp_raw = getCtn(3) / 10.; // ta_temps : int16 °C x10
	cc_input.HW.Cs_vent_temp_raw = getCtn(4) / 10.; // ta_temps : int16 °C x10
	cc_input.HW.Cs_vent_pres_raw = getPressure(0); // ta_air_pres : uint16 Pa x10
	cc_input.HW.Cs_vent_rot_spd_raw = getFanExhaustFeedbackSpeed(); // ta_rot_spd : uint16 RPM x1
	cc_input.HW.Cs_heat_pump_pwr = GetEmbracoInverterPowerRead(); // ta_pwr : uint32 Watt x10

	// TODO
	//cc_input.HW.St_tor_stt_raw[2]; // te_on_off : enum off = 0; on = 1; force = 2

	// TODO
	/*
	cc_input.ERR.Bs_hw_fan_err; // bool
	cc_input.ERR.Bs_hw_pres_err; // bool
	cc_input.ERR.Bs_hw_anod_err; // bool
	*/

	TFLOW4_Ctrl::ExtU_TFLOW4_Ctrl_T input = {cc_input};
	ctrlCmd->setExternalInputs(&input);

	// Execute un pas de calcul *******************************************
	ctrlCmd->step();

	// maj de la commande *************************************************
	cc_out = ctrlCmd->getExternalOutputs().Control_Out;
	cc_DW = ctrlCmd->getDWork();

	setFanExhaustVoltage_mV(cc_out.Cs_vent_vltg_sp); // Cs_vent_vltg_sp sortie en milliVolt
	SetEmbracoInverterSpeedConsRPM(cc_out.Cs_heat_pump_rot_spd_sp);

	if(cc_out.Ss_elec_bstr_htr_sp != te_on_off::off){
		appointElec->SetMode(E_APPOINT_ELEC_ON);
	}
	else {
		appointElec->SetMode(E_APPOINT_ELEC_OFF);
	}
}

