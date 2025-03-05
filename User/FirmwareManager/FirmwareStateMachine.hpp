#pragma once

#include <FirmwareCInterface.h>
#include <stdint.h>


#ifdef USE_ALIVE_LED
#include "LedBlinker.hpp"
#endif

#include "DigitalInputs.hpp"

/* Attention class Singleton (instance unique) */

class FwMng {
public :
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

private :
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
	DigitalInputs *Anode;
};


