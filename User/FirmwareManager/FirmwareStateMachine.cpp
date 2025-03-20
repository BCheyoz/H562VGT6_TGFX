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


/*******************************************************************************************************/
// fonction redéfinie dans FirmwareGateway en "privé"
__attribute__((weak)) void setFanExhaustVoltage_mV(uint16_t cmd){}

/******************************************************************************/
// Initialisation des variables static partagé entre toutes les instances de l'objet
uint8_t FwMng::timer_100ms = 0;
FwMng *FwMng::d = nullptr;

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

	state = E_BOARD_READY_STATE;
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

	di_Anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NO_WORKING_STATE_IS_1,E_SINGLE_INPUT);
	RegisterDigitalInput2EventFnHandler(DI_EVENT_NEW_STATE | DI_EVENT_NEW_WORK_STATE,di_Anode, HandleDI_Event);
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

	switch(state)
	{
	case E_BOARD_READY_STATE:
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
			//mainRegulationInit();
		}
		else {
			if(powerOnTimer >= POWER_ON_WAIT){
				//mainRegulation();
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
