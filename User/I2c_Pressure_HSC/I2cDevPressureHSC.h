/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:24:15.265 for ALDES © 2024 !
 * I2cDevPressureHSC.h
 *
 *  Created on: 29 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 07 Feb. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 *
 */

#ifndef I2C_PRESS_HSC_I2C_DEV_PRESSURE_HSC_H_
#define I2C_PRESS_HSC_I2C_DEV_PRESSURE_HSC_H_

/************************************************************************************************/

#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

#define HSC_UNKNOWN_PRESSURE_PA 		0.0f	// Par défaut, la Pression est initialisée à 0Pa
#define HSC_UNKNOWN_TEMPERATURE_DEG_C	-999.0f	// Par défaut, la Température est initialisée à -999°c (= physiquement impossible)
#define HSC_PRESS_MAX_ALLOWED_ERRORS	5

// Valeurs RAW & Intermédiaires :
#define HSC_GET_BRIDGE_BRUT
#define HSC_GET_BRIDGE_MOY
#define HSC_GET_PRESS_BRUT
#define HSC_GET_TEMP_BRUT

/******************************************************************************/

typedef struct _I2CCM_Pres_HSC_ExtData
{
	I2CCM_ExternalBaseData base;	// Base requise pour I2C_ComMaster

//*******************************
// Variables en Sortie Publique :
	float	Pressure;
	float	Temperature;
	uint16_t BridgeOffset;
	uint16_t BrdgOfstOpId;
	uint16_t newFlags;

//****************************
// Derniers échantillons RAW :

	// Valeurs Brutes :
#ifdef HSC_GET_BRIDGE_BRUT
	int16_t  WheatstoneBridgeBrut;	// Valeur brute "pont de Wheatstone" retournée par le capteur lui-même.
#endif // HSC_GET_BRIDGE_BRUT
#ifdef HSC_GET_TEMP_BRUT
	uint16_t TemperatureBrut;
#endif // HSC_GET_TEMP_BRUT

//********************************************
// Conversion de la Moyenne des échantillons :

	// Valeurs Intermédiaires :
#ifdef HSC_GET_BRIDGE_MOY
	float	BridgeMoy;
#endif // HSC_GET_BRIDGE_MOY
#ifdef HSC_GET_PRESS_BRUT
	float	PresBrut;
#endif // HSC_GET_PRESS_BRUT

} I2CCM_Pres_HSC_ExtData;	// External HSC Datas Struct

typedef enum
{
	I2cCmPressureHscNoNewFlags	= 0,
	I2cCmPressureHscNewPressure	= (1 <<0),
	I2cCmPressureHscNewOffset	= (1 <<1),
} I2CCM_Pressure_HSC_NewFlags;

/******************************************************************************/

typedef enum _I2CCM_Pressure_HSC_AZ_OP_ID
{
	I2cCmPressureHscAutoZeroReady	= 0,
	I2cCmPressureHscAutoZeroActive	= (1 <<0), // Flag 0x01xx
	I2cCmPressureHscAutoZeroDoSave	= (1 <<1), // Flag 0x02xx
} I2CCM_Pressure_HSC_AZ_OP_ID; // Auto Zero Operation ID

typedef enum _I2CCM_Pressure_HSC_AZ_Status
{
	I2cCmPressureHscAutoZeroIdle	= 0,
	I2cCmPressureHscAutoZeroEnded	= (1 <<0),
	I2cCmPressureHscAutoZeroSaved	= (1 <<1),
} I2CCM_Pressure_HSC_AZ_Status; // Auto Zéro Staus

#define I2CCM_AUTOZERO_GET_ACTION_ID(a)		I2CCM_GET_8LH(a) // Byte 1
#define I2CCM_AUTOZERO_GET_STEP_ID(a)		I2CCM_GET_8LL(a) // Byte 0
#define I2CCM_AUTOZERO_MAKE_ACTION_ID(a)	(((a) & 0xFF) << 8)
#define I2CCM_AUTOZERO_MSK_STEP_ID			0xFF // StepId sur Byte0 uniquement

/******************************************************************************/

int16_t i2cCM_PressureHSC_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);
uint16_t i2cCM_ReframeOffsetInAllowedRange(uint16_t newOffset);

#ifdef __cplusplus
}
#endif

#endif /* I2C_PRESS_HSC_I2C_DEV_PRESSURE_HSC_H_ */
