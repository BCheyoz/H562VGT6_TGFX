/*
 * GestionInputSensor.h
 *
 *  Created on: 14 nov. 2022
 *  Original Author: j.proux
 *
 * Version 1.0
 *
 * From HII_Manta_App au 14/11/2022
 *
 */

#ifndef GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_
#define GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_

#include <stdint.h>			// Pour les types "int*_t" & "uint*_t"

#define NB_MAX_EXT_SENSOR 3

/*** public Enums *******************************/

typedef enum
{
	NO_SENSOR = 0,
	SENSOR_INIT_PENDING,
	SENSOR_EE895,
	SENSOR_SCD30,
    SENSOR_BENCHTEST,
} E_SENSOR_ID;

typedef enum
{
	SENSOR_COV = 1,
	SENSOR_PM,
	SENSOR_CO2,
	NO_SENSOR_EXT,
} E_EXT_SENSOR_ID;

// Add above this line others public enums

/*** public Functions *******************************/

void GestionInputSensor();

// Add above this line others public functions

/*** public Getters *******************************/

uint16_t getCo2();
uint8_t getCo2Id();

// Pour essai systeme
void byPassCo2Value(uint16_t val);
void byPassCo2Id(uint16_t code);

// Add above this line others public Getters

#endif /* GESTIONINPUTSENSOR_GESTIONINPUTSENSOR_H_ */
