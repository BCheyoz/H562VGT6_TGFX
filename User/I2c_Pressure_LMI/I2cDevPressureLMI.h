/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:25:33.337 for ALDES © 2024 !
 * I2cDevPressureLMI.h
 *
 *  Created on: 26 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 06 Feb. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 *
 */

#ifndef I2C_PRESS_LMI_I2C_DEV_PRESSURE_LMI_H_
#define I2C_PRESS_LMI_I2C_DEV_PRESSURE_LMI_H_

/*************************************************************************************************/

#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

#define LMI_UNKNOWN_PRESSURE_PA 		0.0f	// Par défaut, la Pression est initialisée à 0Pa
#define LMI_UNKNOWN_TEMPERATURE_DEG_C	-999.0f	// Par défaut, la Température est initialisée à -999°c (= physiquement impossible)
#define LMI_PRESS_MAX_ALLOWED_ERRORS	5

// Configuration des Moyennages internes :
#define LMI_MOY_P_FROM_RAW 		1	// Pour activer un Moyennage sur les échantillons bruts issus du Capteur
#define LMI_MOY_P_FROM_CONV		1	// Pour activer un Moyennage des valeurs déjà converties en Pression

// Valeurs RAW & Intermédiaires :
#define LMI_GET_VALUE_BRUT  		// Pour demander à récupérer la valeur "Brute" (= non convertie) fournie par le Capteur
#define LMI_GET_VALUE_MOY			// Pour demander à récupérer la valeur "Moyennée non convertie" issue du Capteur
#define LMI_GET_PRESS_BRUT  		// Pour demander à récupérer la valeur Moyennée Convertie en Pascals
//#define LMI_GET_TEMP_BRUT_  		// Pour demander à récupérer la valeur "Brute" (= non convertie) de la Température, fournie par le Capteur
// Remarque_Jp le 28/10/2021 : LMI_GET_TEMP_BRUT est volontairement forcé en commentaire, en attendant de pouvoir calibrer, si c'est possible

// Misc options :
#define LMI_RX_TX_BUF_PER_DEVICE	// Réserver 1 Buffer de RxTx par Device LMI, plutôt qu'1 seul commun à tous les LMI
#define LMI_DECODE_PRESS_RANGE
#define LMI_DECODE_SCALE_FACTOR
#define LMI_DECODE_SEQ_NUMBER

/******************************************************************************/

typedef struct _I2CCM_Pres_LMI_ElectronicSignature
// From "DS_Standard-LMI_E_11823.pdf" v1.1 of "April 2019" page 11 :
{
	union {
		struct {
			uint8_t FwVerMajor;
			uint8_t FwVerMinor;
		}__attribute__ ((__packed__));
		uint16_t FwVersionBE;	// Nécessaire : Offsets 0-1
	}__attribute__ ((__packed__));
	uint8_t PartNumber[11]; 	// Nécessaire : Offsets 2-12
	uint8_t LotNumber[7];		// Nécessaire : Offsets 13-19
	uint8_t PresRangeBE[2];		// Nécessaire : Offsets 20-21 = Pressure Range en BigEndian
	uint8_t OutputType; 		// Nécessaire : Offset 22	  = 'B' for Bidirectional, 'U' for Unidirectional
	uint8_t ScaleFactorBE[2];	// Nécessaire : Offsets 23-24 = ScaleFactor en BigEndian (ATTENTION : pas aligné 16bits !)
	uint8_t CalibrationID[2];	// Offset 25-26
	uint8_t WeekNumber; 		// Offset 27
	uint8_t YearNumber; 		// Offset 28
	uint8_t SequenceNumBE[2];	// Offsets 29-30 : SequenceNumber en BigEndian (ATTENTION : c'est pas aligné sur 16bits !)
//	uint8_t Reserved[23];		// Offsets 31-53 -> "Reserved for future use"
} I2CCM_Pres_LMI_ElectronicSignature;

typedef struct _I2CCM_Pres_LMI_ExtData
{
	I2CCM_ExternalBaseData base;	// Base requise pour I2C_ComMaster

//*******************************
// Variables en Sortie complémentaires :
	I2CCM_Pres_LMI_ElectronicSignature ElectronicSignature;

//*******************************
// Variables en Sortie Publique :
	float	Pressure;
	float	Temperature;
	uint16_t newFlags;

//****************************
// Derniers échantillons RAW :

#ifdef LMI_GET_VALUE_BRUT
	int16_t  ValueBrut;	// Valeur brute retournée par le capteur lui-même.
#endif // LMI_GET_VALUE_BRUT
#ifdef LMI_GET_TEMP_BRUT
	uint16_t TemperatureBrut;
#endif // SDP8_GET_TEMP_BRUT

//*******************************
// Moyenne des échantillons RAW :

#ifdef LMI_GET_VALUE_MOY
	float	ValueMoy;
#endif // LMI_GET_VALUE_MOY

//********************************************
// Conversion de la Moyenne des échantillons :

#ifdef LMI_GET_PRESS_BRUT
	float	PresBrut;
#endif // LMI_GET_PRESS_BRUT

//**********************
// Valeurs Recombinées :

#ifdef LMI_DECODE_PRESS_RANGE
	uint16_t PressureRange;
#endif // LMI_DECODE_PRESS_RANGE
#ifdef LMI_DECODE_SCALE_FACTOR
	uint16_t ScaleFactor;
#endif // LMI_DECODE_SCALE_FACTOR
#ifdef LMI_DECODE_SEQ_NUMBER
	uint16_t  SequenceNumber;
#endif // LMI_DECODE_SEQ_NUMBER

} I2CCM_Pres_LMI_ExtData;	// External LMI Datas Struct

/******************************************************************************/

typedef enum
{
	I2cCmPressureLmiNoNewFlags	= 0,
	I2cCmPressureLmiNewPres 	= (1 <<0),
//	I2cCmPressureLmiNewOffset	= (1 <<1),
	I2cCmPressureLmiNewSignat	= (1 <<8),
//	I2cCmPressureLmiNewProdId	= (1 <<9),
} I2CCM_Pressure_LMI_NewFlags;

typedef enum
{
	I2cDevPresType_LMI_Unknown = 0,	// Unknown LMI Type
	I2cDevPresType_LMI_Adr00,		// Linked to First Sensor LMI @ Adr00
	I2cDevPresType_LMI_Adr01,		// Linked to First Sensor LMI @ Adr01
	I2cDevPresType_LMI_Adr10,		// Linked to First Sensor LMI @ Adr10
	I2cDevPresType_LMI_Adr11,		// Linked to First Sensor LMI @ Adr11
	// Insérer ici tout nouveau type supporté par ce module de Pression LMI
	I2cDevPresType_LMI_OutOfRange,	// OutOfRange = nbEléments +1
} I2cDevPressureTypeLMI;

/******************************************************************************/

int16_t i2cCM_PressureLMI_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);

#ifdef __cplusplus
}
#endif

#endif /* I2C_PRESS_LMI_I2C_DEV_PRESSURE_LMI_H_ */
