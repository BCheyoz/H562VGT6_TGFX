/*
 * FirmwareStateMachine.c
 *
 *  Created on: 5 Feb 2025
 *  Original Author: B.Chhay
 *
 *	Main state machine of mother board
 *
 */

#include "FirmwareStateMachine.h"
#include "ParamProductInfo.h"
#include "utils.h"
#include "LedBlinker.hpp"

#ifdef USE_COMMISIONNING_STATE
#define COMMISSIONNING_END_PWD      204
#define COMMISSIONNING_RESET_PWD    76
#define CONFIG_RESET_PWD            41

static uint8_t 	resetConfig;
static uint8_t  resetCommissionning;
static uint8_t 	commissionningParamDone;
static uint8_t  commissionningParamSaved;
#endif

#ifdef USE_SAV_STATE
#define SAV_RESET_PWD               186

static uint8_t 	savReset;
#endif

#define PRODUCT_RESET_PWD           242
#define FACTORY_LICENSE_KEY         147
#define BENCH_LICENSE_KEY         	197
#define REG_RESET_PWD               18
#define POWER_ON_WAIT               50     // 2 sec avec un pas de temps de 100ms
#define ERASE_MEM_KEY               3854

static e_softState MSM_state;
static uint8_t 	timer_100ms;
static uint8_t 	ctrlCmdReset;
static uint8_t 	powerOnTimer;  // temps d'attente pour que les taches bas niveau puisse s'initialiser
static uint8_t  eraseMemory;
static uint8_t  regReset;

/*******************************************************************************************************/
// fonction redéfinie dans FirmwareGateway en "privé"
__attribute__((weak)) void setFanExhaustVoltage_mV(uint16_t cmd){}

/*******************************************************************************************************/

void handleMainStateMachineRT_100ms()
{
	++timer_100ms;
}

void MSM_Init(void)
{
#ifdef USE_COMMISIONNING_STATE
	resetCommissionning = FALSE;
	commissionningParamDone = FALSE;
	commissionningParamSaved = FALSE;
#endif

#ifdef USE_SAV_STATE
	savReset = FALSE;
#endif

	MSM_state = E_BOARD_READY_STATE;
	timer_100ms = 0;
	resetConfig = FALSE;
	ctrlCmdReset = TRUE;
	powerOnTimer = 0;
	eraseMemory = 0;
	regReset = TRUE;



}

void MSM_Mgt(void)
{
	if(timer_100ms == 0){
		return ;
	}

	timer_100ms = 0;

	if(powerOnTimer < POWER_ON_WAIT){
		powerOnTimer++;
		return;
	}

	switch(MSM_state)
	{
	case E_BOARD_READY_STATE:
		// on reste en Boot tant qu'il n'y a pas de code carte
		if(isCodeBoardCompliant()){
			MSM_state = E_BOARD_READY_STATE;
			if( isCodeIdCompliant() == TRUE)
			{
				if(isCommissionningDone() == TRUE)
					MSM_state = E_PRODUCT_COMPLETE_STATE;
				else
					MSM_state = E_PRODUCT_READY_STATE;
			}
			else {
				if(isSAVProduct() == TRUE){
					MSM_state = E_BOARD_SAV_READY_STATE;
				}
			}
		}
		break;

	case E_PRODUCT_READY_STATE:
		if(commissionningParamDone == TRUE){
			// on enregistre une fois quand la mise en service est terminé
			commissionningParamDone = FALSE;
			// sécurité, pas de reset mise en service une fois fini sinon on reste bloqué
			resetCommissionning = FALSE;
			resetConfig = FALSE;
			regReset = TRUE;
			setCommissionningState(1); // MES Fini
			MSM_state = E_PRODUCT_COMPLETE_STATE;
		}
		break;

	case E_PRODUCT_COMPLETE_STATE:
        if(resetConfig == TRUE){
			resetConfig = FALSE;
			resetParamProduct();
			MSM_state = E_PRODUCT_READY_STATE;
		}

        if(resetCommissionning == TRUE){
			resetCommissionning = FALSE;
			setCommissionningState(0);
			MSM_state = E_PRODUCT_READY_STATE;
		}

		if(savReset == TRUE){
		    savReset = FALSE;
            // Cette fonction efface le Product ID et les réglages du système
		    resetParamProduct();
            MSM_state = E_BOARD_SAV_READY_STATE; // necessite le changement d'état avant reset ID
            WriteProductId(0);
            //setFanExhaustVoltage_mV(0);
		}

		else{
			if(regReset == TRUE){
				regReset = FALSE;
				//mainRegulationInit();
			}
			else {
				if(powerOnTimer >= POWER_ON_WAIT){
					//mainRegulation();
				}
			}
		}
		break;

	case E_BOARD_SAV_READY_STATE:
		if( isCodeIdCompliant() == TRUE)
		{
			MSM_state = E_PRODUCT_READY_STATE;
		}
		break;

	case E_FACTORY_STATE:
	case E_FACTORY_BENCH_STATE:
	    if(savReset == TRUE){
            savReset = FALSE;
            // Cette fonction efface le Product ID et les réglages du système
            resetParamProduct();
            WriteProductId(0);
            setFanExhaustVoltage_mV(0);
        }
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
		MSM_state = E_BOARD_READY_STATE;
	}
}



/******************************************************************************/

e_softState firmwareState(void)
{
	return MSM_state;
}

void requestToSwitchToFactoryState(uint16_t value)
{
	if(FACTORY_LICENSE_KEY == value){
		MSM_state = E_FACTORY_STATE;
	}
	else if(BENCH_LICENSE_KEY == value){
		MSM_state = E_FACTORY_BENCH_STATE;
	}
}

/******************************************************************************/
void requestProductReset(uint16_t value){
	// fonction autorisee uniquement en factory state et si le mdp est bon
	if((E_FACTORY_STATE == MSM_state) && (PRODUCT_RESET_PWD == value))
		resetCodeProduct();
}

void requestResetMemories(uint16_t code){
    if(E_FACTORY_STATE != MSM_state || code != ERASE_MEM_KEY
            || eraseMemory != 0){ return;} // on accept pas la demande s'il est déja en cours
        eraseMemory = 1;
}
uint16_t resetMemoriesState() { return eraseMemory; }

void requestToInitRegulation(uint16_t value){
	if(value == REG_RESET_PWD){
		regReset = TRUE;
	}
}

/******************************************************************************/
#ifdef USE_COMMISIONNING_STATE
void requestConfigReset(uint8_t code){
	if(E_PRODUCT_COMPLETE_STATE != MSM_state || code != CONFIG_RESET_PWD){ return;}
	resetConfig = TRUE;
}

void resetCommissionningState(uint8_t code){
	if(E_PRODUCT_COMPLETE_STATE != MSM_state || code != COMMISSIONNING_RESET_PWD){ return;}
	resetCommissionning = TRUE;
}

void requestEndOfCommissionning(uint8_t code){
	if(E_PRODUCT_READY_STATE != MSM_state || code != COMMISSIONNING_END_PWD){ return;}
	commissionningParamDone = TRUE;
}
#endif

#ifdef USE_SAV_STATE
void requestSAVreset(uint8_t code){
	//HII-2140 - La fonction SAV Reset est disponible que pour des carte avec un code SAP en 111xxxxx
	if((E_PRODUCT_READY_STATE != MSM_state && E_FACTORY_STATE != MSM_state && E_FACTORY_BENCH_STATE != MSM_state)
	        || code != SAV_RESET_PWD){ return;}
		savReset = TRUE;
}
#endif

/******************************************************************************/
#ifdef USE_ALIVE_LED
void requestBlinkMode(uint16_t newBlinkMode){
	if(E_FACTORY_STATE == MSM_readSoftState())	// HII-1264: fonction autorisee uniquement en factory state
	{
			SetBlinkMode(newBlinkMode);
	}
}

uint16_t blinkMode() { return (uint16_t)GetBlinkMode(); }
#endif
