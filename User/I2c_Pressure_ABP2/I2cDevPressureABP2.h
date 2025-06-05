/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:24:15.265 for ALDES © 2024 !
 * I2cDevPressureABP2.h
 *
 *  Created on: 13 Fev. 2025
 *  Original Author: b.chhay
 *
 *  Ref sensor X116221ABP2
 *
 *  Version 1.0
 *
 */

#ifndef I2C_PRESS_ABP2_I2C_DEV_PRESSURE_ABP2_H_
#define I2C_PRESS_ABP2_I2C_DEV_PRESSURE_ABP2_H_

/************************************************************************************************/

#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

#define ABP2_UNKNOWN_PRESSURE_PA 		0.0f	// Par défaut, la Pression est initialisée à 0Pa
#define ABP2_UNKNOWN_TEMPERATURE_DEG_C	-999.0f	// Par défaut, la Température est initialisée à -999°c (= physiquement impossible)
#define ABP2_PRESS_MAX_ALLOWED_ERRORS	5

#define ABP2_MOY_PRES 	1	// > 1 Pour activer un Moyennage sur les échantillons des valeurs de pression
#define ABP2_MOY_TEMP 	1	// > 1 Pour activer un Moyennage sur les échantillons des valeurs de température

// Sensor Options :
#define ABP2_READ_TEMPERATURE		// Demander à récupérer la Température du Capteur

/******************************************************************************/

typedef union {
	struct {
		unsigned mathSat: 1;	// 1 = internal math saturation has occurred
		unsigned b1: 1;			// always 0
		unsigned memError: 1;	// 0 = passed; 1 = failed. Indicates whether the checksum-based integrity check passed or failed; the memory error status bit is calculated only during the power-up sequence.
		unsigned b3: 1;			// always 0
		unsigned b4: 1;			// always 0
		unsigned isBusy: 1;     // 1 = device is busy. Indicates that the data for the last command is not yet available. No new commands are processed if the device is busy.
		unsigned power: 1;		// 1 = device is powered; 0 = device is not powered
		unsigned b7: 1;			// always 0
	};
	uint8_t raw;
}ABP2_StatusFlags;

typedef struct _I2CCM_Pres_ABP2_ExtData
{
	I2CCM_ExternalBaseData base;	// Base requise pour I2C_ComMaster

//*******************************
// Variables en Sortie Publique :
	float	Pressure;
	float	Temperature;
	ABP2_StatusFlags status;
	uint16_t newFlags;

} I2CCM_Pres_ABP2_ExtData;	// External ABP2 Datas Struct
/******************************************************************************/

typedef enum
{
	I2cCmPressureAbp2NoNewFlags	= 0,
	I2cCmPressureAbp2NewValue	= (1 <<0),
	I2cCmPressureAbp2NewStatus	= (1 <<1),
} I2CCM_Pressure_ABP2_NewFlags;

int16_t i2cCM_PressureABP2_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);

#ifdef __cplusplus
}
#endif

#endif /* I2C_PRESS_ABP2_I2C_DEV_PRESSURE_ABP2_H_ */
