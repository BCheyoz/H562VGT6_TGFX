/*
 * FirmwareGateway.c
 *
 *  Created on: 4 Feb 2023
 *  Original Author: B.Chhay
 *
 *	Passerelle unique qui regroupe les composants I/O du système et limite les access
 *
 */

#include "FirmwareStateMachine.hpp"
#include "FirmwareGateway.h"
#include "utils.h"
#include <list>

typedef enum
{
	E_FAN = 0,
	E_SENSOR,
	E_PARAM,
	E_INVERTER,
	E_ANODE,
	E_ELEC_NETWORK,
	E_RESISTIF_HEAT,
}e_device;

typedef struct {
	e_device device;
	std::list<e_softState> stateAllow;
}s_limitAcces;


std::list<s_limitAcces> writeLimitStateAcces = {
		{E_FAN, {E_FACTORY_STATE, E_FACTORY_BENCH_STATE}},
		{E_SENSOR, {E_FACTORY_STATE}}
};


void requestFanVoltage_mV(uint16_t newVoltage){
	FwMng *obj = FwMng::getInstance();

	if(E_FACTORY_STATE == obj->getState()	//  fonction autorisee uniquement en factory state et banc
		|| E_FACTORY_BENCH_STATE == obj->getState()){
		//setFanExhaustVoltage_mV(newVoltage);
	}
}

uint16_t fanVoltage_mV() { return 0; }
uint16_t fanFeedbackSpeed() { return 0; }
uint16_t fanLastFeedbackSpeed() { return 0; }
uint16_t fanLastDeltaTime() { return 0; }
uint8_t fanVoltage_V_x10() { return 0; }

