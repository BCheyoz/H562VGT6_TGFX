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

#ifdef __cplusplus
extern "C" {
#endif

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


/*** private prototype functions *******************************/

#define GET_SET_ARRAY_DEFINITION(a, b, c)		c get##a##b(void){return get##a(b);} \
												uint8_t getID##a##b(void){return getID##a(b);} \
												void byPass##a##b(c val){ byPass##a(b, val);}



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


#if NB_PRESSURE_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION(Pressure, 0, uint16_t)
#endif

#if NB_COV_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION(Cov, 0, uint16_t)
#endif

#if NB_CO2_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION(Co2, 0, uint16_t)
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION(Hr, 0, uint16_t)
GET_SET_ARRAY_DEFINITION(Temp, 0, int16_t)
#endif

#if NB_CTN_TT4_10KC3_USE > 0
GET_SET_ARRAY_DEFINITION(Ctn, 0, int16_t)
GET_SET_ARRAY_DEFINITION(Ctn, 1, int16_t)
GET_SET_ARRAY_DEFINITION(Ctn, 2, int16_t)
GET_SET_ARRAY_DEFINITION(Ctn, 3, int16_t)
GET_SET_ARRAY_DEFINITION(Ctn, 4, int16_t)
#endif

#ifdef __cplusplus
}
#endif
