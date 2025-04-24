#pragma once

#include <FirmwareCInterface.h>
#include <stdint.h>


#ifdef USE_ALIVE_LED
#include "LedBlinker.hpp"
#endif

/********************************************************************************************/
// User Include
#include "AppointElec.hpp"
#include "DigitalInputs.hpp"
#include "TFLOW4_Ctrl.h"
/* Attention class Singleton (instance unique) */

class FwMng {
public :
/********************************************************************************************/
// Core method
	static FwMng* getInstance();


	static void it_100ms();	// cadencement de la fonction
	void run();

	inline e_softState getState() {return state;}
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
	inline uint16_t blinkMode(){return (uint16_t)ledAlive->GetBlinkMode();}
#endif

/********************************************************************************************/
// User method
	void setAppointEnable(uint8_t enable){
		if(enable == 1)	appointElec->SetMode(E_APPOINT_ELEC_ON);
		else appointElec->SetMode(E_APPOINT_ELEC_OFF);
	}

	inline uint8_t isAppointEnable(){return appointElec->GetMode();}

	inline uint8_t isAnodeFlags(){return (uint8_t)di_Anode->getFlags();}
	inline uint8_t isAnodeState(){return (uint8_t)di_Anode->getcurState();}

	inline tb_simu_ena* getCC_SimuEna(){return &cc_input.SIMU.ENA;}
	inline tb_simu_var* getCC_SimuVar(){return &cc_input.SIMU.VAR;}
	inline tb_Control_Out* getCC_output(){return &cc_out;}
	inline VentCtrl::P_VentCtrl_T* getCC_VentCtrlParam(){return &VentCtrl::VentCtrl_rtP;}


private :
/********************************************************************************************/
// Core variable
	FwMng();

	static FwMng *d; // instance singleton

	e_softState state;
	static uint8_t timer_100ms;
	uint8_t ctrlCmdReset;
	uint8_t powerOnTimer;  // temps d'attente pour que les taches bas niveau puisse s'initialiser
	uint8_t eraseMemory;
	uint8_t regReset;

#ifdef USE_COMMISIONNING_STATE
	uint8_t resetConfig;
	uint8_t resetCommissionning;
	uint8_t commissionningParamDone;
	uint8_t commissionningParamSaved;
#endif

#ifdef USE_SAV_STATE
	uint8_t	savReset;
#endif

#ifdef USE_ALIVE_LED
	LedBlinker *ledAlive;
#endif


/********************************************************************************************/
// User variable

	//Permet d'executer la regulation au bon cadencement et d'alimenter les structures d'entrée/sortie
	void CtrlCmdTask();

	AppointElec *appointElec;
	DigitalInputs *di_Anode;

	TFLOW4_Ctrl *ctrlCmd;
	static tb_Control_In cc_input; // structure d'entrée déclaré en static pour acces via cubeMonitor
	static tb_Control_Out cc_out;  // structure de sortie déclaré en static pour acces via cubeMonitor
	uint8_t ctrlCmdCounter; // Timer pour executer la régulation a un cadencement donnée
};


