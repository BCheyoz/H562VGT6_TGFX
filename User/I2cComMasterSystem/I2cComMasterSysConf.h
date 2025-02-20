/*
 * I2cComMasterSysConf.h
 *
 *  Created on: 8 nov. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 */

#ifndef I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSCONF_H_
#define I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSCONF_H_

/*********************************************************************************************/

#include "i2c.h"	// Pour accès aux Variables & Fonctions d'Init de la HAL I2C

/******************************************************************************/

#define I2CCM_HANDLE		&hi2c1
#define I2CCM_FN_INIT		MX_I2C1_Init

//#define I2CCM_RESET_PIN 	Rst_Sensor_Pin
//#define I2CCM_RESET_PORT	Rst_Sensor_GPIO_Port

#define I2CCM_MAX_DEV_SIM	1	// Max Devices Simultanément : Pression uniquement

#endif /* I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSCONF_H_ */
