/*
 * GestionInputSensor.h
 *
 *  Created on: 14 nov. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 28 Feb. 2025
 *  Updated by: b.chhay
 *
 *  Version 1.1
 */

#ifndef GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_
#define GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_

#include <stdint.h>			// Pour les types "int*_t" & "uint*_t"

#define USE_I2C_SENSOR TRUE
#define USE_ADC_SENSOR TRUE


#ifdef USE_I2C_SENSOR
#include "I2cComMasterConf.h"
#include "I2cComMasterSystem.h"

#define NB_PRESSURE_SENSOR_USED 1
#define NB_COV_SENSOR_USED 0
#define NB_CO2_SENSOR_USED 0
#define NB_HR_TEMP_SENSOR_USED 0
#endif

#ifdef USE_ADC_SENSOR
#include "AnalogInputsUser.h"
#endif


/*** public Enums *******************************/

typedef enum
{
	NO_SENSOR = 0,
    SENSOR_BENCHTEST,
	SENSOR_INIT_PENDING,
	SENSOR_EE894,
	SENSOR_EE895,
	SENSOR_SCD3x,
	SENSOR_SCD4x,
	SENSOR_HSC,
	SENSOR_SDP6,
	SENSOR_SDP8,
	SENSOR_LMI,
	SENSOR_ABP2,
	SENSOR_CCS811,
	SENSOR_SGP40,
	SENSOR_ENS160,
	SENSOR_SHT3x,
	SENSOR_SHT4x,
	SENSOR_HTU31,
	SENSOR_CTN_TT4_10KC3
} E_SENSOR_ID;

// Add above this line others public enums

#ifdef __cplusplus
extern "C" {
#endif

/*** public Functions *******************************/
void Handle_InputSensor_RT_100ms();
void InitInputSensor();
void GestionInputSensor();

// Add above this line others public functions

/*** public Getters *******************************/
#define GET_SET_PROTOTYPE_SENSOR(a, c)	c get##a(uint8_t idx); uint8_t getID##a(uint8_t idx); void byPass##a(uint8_t idx, c val);

uint16_t getBypassDuration(); // durée du court circuit en seconde
void setBypassDuration(uint16_t duration);

#if NB_PRESSURE_SENSOR_USED > 0
GET_SET_PROTOTYPE_SENSOR(Pressure, uint16_t)
#endif

#if NB_COV_SENSOR_USED > 0
GET_SET_PROTOTYPE_SENSOR(Cov, uint16_t)
#endif

#if NB_CO2_SENSOR_USED > 0
GET_SET_PROTOTYPE_SENSOR(Co2, uint16_t)
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
GET_SET_PROTOTYPE_SENSOR(Hr, uint16_t)
GET_SET_PROTOTYPE_SENSOR(Temp, int16_t)
#endif

#if NB_CTN_TT4_10KC3_USE > 0
GET_SET_PROTOTYPE_SENSOR(Ctn, int16_t)
#endif
// Add above this line others public Getters

#ifdef __cplusplus
}
#endif

#endif /* GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_ */
