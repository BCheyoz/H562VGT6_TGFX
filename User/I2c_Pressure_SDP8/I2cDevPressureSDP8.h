/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:27:03.359 for ALDES © 2024 !
 * I2cDevPressureSDP8.h
 *
 *  Created on: 12 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 06 Feb. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 */

#ifndef I2C_PRESS_SDP8_I2C_DEV_PRESSURE_SDP8_H_
#define I2C_PRESS_SDP8_I2C_DEV_PRESSURE_SDP8_H_

#ifdef __cplusplus
 extern "C" {
#endif

/************************************************************************************************/

#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

/******************************************************************************/

#define SDP8_UNKNOWN_PRESSURE_PA		0.0f	// Par défaut, la Pression est initialisée à 0Pa
#define SDP8_UNKNOWN_TEMPERATURE_DEG_C	-999.0f	// Par défaut, la Température est initialisée à -999°c (= physiquement impossible)
#define SDP8_PRESS_MAX_ALLOWED_ERRORS	5		// nb Max d'erreurs autorisées avant de recommander l'abandon de ce Device

#define SDP8_MOY_P_FROM_RAW 	1	// Pour activer un Moyennage sur les échantillons bruts issus du Capteur
#define SDP8_MOY_P_FROM_CONV	1	// Pour activer un Moyennage des valeurs déjà converties en Pression

// Sensor Options :
#define SDP8_READ_TEMPERATURE		// Demander à récupérer la Température du Capteur

// Valeurs Brutes :
#define SDP8_GET_VALUE_BRUT			// Pour demander à récupérer la valeur "Brute" (= non convertie) fournie par le Capteur
#define SDP8_GET_VALUE_MOY			// Pour demander à récupérer la valeur "Moyennée non convertie" issue du Capteur
#define SDP8_GET_PRESS_BRUT			// Pour demander à récupérer la valeur Moyennée Convertie en Pascals
#define SDP8_GET_TEMP_BRUT			// Pour demander à récupérer la valeur "Brute" (= non convertie) de la Température, fournie par le Capteur

// Misc options :
#define SDP8_RX_TX_BUF_PER_DEVICE	// Réserver 1 Buffer de RxTx par Device SDP8, plutôt qu'1 seul commun à tous les SDP8
#define SDP8_GET_SERIAL_NUMBER		// Demander à récupérer le SerialNumber

/******************************************************************************/

#ifdef SDP8_READ_TEMPERATURE
	#define SDP8_NB_BLOCS_2READ	2	// Pression + T°
#else // De toute façon, au moins la Pression :
	#define SDP8_NB_BLOCS_2READ	1	// Uniquement la Pression
#endif // SDP8_READ_TEMPERATURE

typedef struct _I2CCM_Pres_SDP8_ExtData
{
	I2CCM_ExternalBaseData base;	// Base requise pour I2C_ComMaster

//*******************************
// Variables en Sortie Publique :
	float	Pressure;
	uint16_t newFlags;

//**************************************
// Variables en Sortie complémentaires :

#ifdef SDP8_READ_TEMPERATURE
	float	Temperature;
#endif // SDP8_READ_TEMPERATURE

#ifdef SDP8_GET_SERIAL_NUMBER
	uint32_t ProductId;
	uint64_t SerialNumber;
#endif // SDP8_GET_SERIAL_NUMBER

//****************************
// Derniers échantillons RAW :

#ifdef SDP8_GET_VALUE_BRUT
	int16_t  WheatstoneBridgeBrut;	// Valeur brute retournée par le capteur lui-même.
#endif // SDP8_GET_VALUE_BRUT
#ifdef SDP8_GET_TEMP_BRUT
	uint16_t TemperatureBrut;
#endif // SDP8_GET_TEMP_BRUT

//*******************************
// Moyenne des échantillons RAW :

#ifdef SDP8_GET_VALUE_MOY
	float	BridgeMoy;
#endif // SDP8_GET_VALUE_MOY

//********************************************
// Conversion de la Moyenne des échantillons :

#ifdef SDP8_GET_PRESS_BRUT
	float	PresBrut;
#endif // SDP8_GET_PRESS_BRUT

} I2CCM_Pres_SDP8_ExtData;	// External SDP8xx Datas Struct

/******************************************************************************/

typedef enum
{
	I2cCmPressureSdp8NoNewFlags	= 0,
	I2cCmPressureSdp8NewPres	= (1 <<0),
//	I2cCmPressureSdp8NewOffset	= (1 <<1),
	I2cCmPressureSdp8NewSerialN	= (1 <<8),
	I2cCmPressureSdp8NewProdId	= (1 <<9),
} I2CCM_Pressure_SDP8_NewFlags;

typedef enum
{
	I2cDevPresType_SDP8_Unknown = 0,// Unknown SDP Type
	I2cDevPresType_SDP8_8x0,		// Linked to Sensirion SDP_8x0
	I2cDevPresType_SDP8_8x1,		// Linked to Sensirion SDP_8x1
	// Insérer ici tout nouveau type supporté par ce module de Pression SDP8
	I2cDevPresType_SDP8_OutOfRange,	// OutOfRange = nbEléments +1
} I2cDevPressureTypeSDP8;

/******************************************************************************/

int16_t i2cCM_PressureSDP8_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);

#ifdef __cplusplus
}
#endif

#endif /* I2C_PRESS_SDP8_I2C_DEV_PRESSURE_SDP8_H_ */
