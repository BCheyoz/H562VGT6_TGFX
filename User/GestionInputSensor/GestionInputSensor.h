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
#define GET_SET_ARRAY_PROTOTYPE(a, b, c)	c get##a##b(void); uint8_t getID##a##b(void); void byPass##a##b(c val);

uint16_t getBypassDuration(); // durée du court circuit en seconde
void setBypassDuration(uint16_t duration);

#if NB_PRESSURE_SENSOR_USED > 0
uint16_t getPressure(uint8_t idx);
uint8_t getPressureId(uint8_t idx);

// interface modbus
GET_SET_ARRAY_PROTOTYPE(Pressure, 0, uint16_t)
#endif

#if NB_COV_SENSOR_USED > 0
uint16_t getCov(uint8_t idx);
uint8_t getCovId(uint8_t idx);

// interface modbus
GET_SET_ARRAY_PROTOTYPE(Cov, 0, uint16_t)
#endif

#if NB_CO2_SENSOR_USED > 0
uint16_t getCo2(uint8_t idx);
uint8_t getCo2Id(uint8_t idx);

// interface modbus
GET_SET_ARRAY_PROTOTYPE(Co2, 0, uint16_t)
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
uint16_t getHr(uint8_t idx);
uint8_t getHrId(uint8_t idx);
int16_t getTemp(uint8_t idx);
uint8_t getTempId(uint8_t idx);

// interface modbus
GET_SET_ARRAY_PROTOTYPE(Hr, 0, uint16_t)
GET_SET_ARRAY_PROTOTYPE(Temp, 0, int16_t)
#endif

#if NB_CTN_TT4_10KC3_USE > 0
int16_t getCtn(uint8_t idx);
uint8_t getCtnId(uint8_t idx);

// interface modbus
GET_SET_ARRAY_PROTOTYPE(Ctn, 0, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 1, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 2, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 3, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 4, int16_t)
#endif
// Add above this line others public Getters

#ifdef __cplusplus
}
#endif

#endif /* GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_ */
