/*
 * I2cDevPressureABP2.c
 *
 *  Created on: 13 Fev. 2025
 *  Original Author: b.chhay
 *
 *  Pour intégrer facilement ce Module "I2c_Pressure_ABP2" à la Librairie "I2cComMaster" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "I2cDevPressureABP2.h"
 *
 */

#include "I2cDevPressureABP2.h"	// Pour accès à nos propres déclarations publiques

#include "I2cComMasterConf.h"	// Pour accès à la Configuration User souhaitée
#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

#include "utils.h"				// Pour joindre les octets en un entier

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

typedef enum
{
	I2cDevPresType_ABP2_Unknown = 0,	// Unknown ABP2 Type
	I2cDevPresType_ABP2_28,			// Linked to CPT_HONEYWELL_1
	I2cDevPresType_ABP2_38,			// Linked to CPT_HONEYWELL_2
	I2cDevPresType_ABP2_48,			// Linked to CPT_HONEYWELL_3
	// Insérer ici tout nouveau type supporté par ce module de Pression ABP2
	I2cDevPresType_ABP2_OutOfRange,	// OutOfRange = nbEléments +1
} I2cDevPressureTypeABP2;

/******************************************************************************/
// Moyennage :
#define ABP2_AVERAGE_ARRAY_MAX_SIZE	255    // valeur limite a 0xFF -1 car l'index est en uint8

#if ABP2_MOY_PRES > ABP2_AVERAGE_ARRAY_MAX_SIZE
#define ABP2_AVG_PRES_ARRAY_MAX_SIZE 	ABP2_AVERAGE_ARRAY_MAX_SIZE
#else
#define ABP2_AVG_PRES_ARRAY_MAX_SIZE 	ABP2_MOY_PRES
#endif

#if ABP2_MOY_TEMP > ABP2_AVERAGE_ARRAY_MAX_SIZE
#define ABP2_AVG_TEMP_ARRAY_MAX_SIZE 	ABP2_AVERAGE_ARRAY_MAX_SIZE
#else
#define ABP2_AVG_TEMP_ARRAY_MAX_SIZE 	ABP2_MOY_TEMP
#endif

// Temporisations :
#define ABP2_TEMPO_BEFORE_FIRST_ACTION	100		// 100ms avant 1° Lecture
#define ABP2_TEMPO_REPLY_TIMEOUT		100		// 100ms d'attente max pour répondre
#define ABP2_TEMPO_RETRY_ACTION			1000	// 1s avant Retry
#define ABP2_TEMPO_BEFORE_NEXT_ACTION	1 *1000	// 1s avant de redemander une nouvelle Lecture

// Capteur de Pression HoneyWell AdrI2c $28 [0; 300Pa [ :
#define ADR_SENSOR_ABP2_28		0x28	// Adresse I2C = $28 sur 7bits
#define ADR_SENSOR_ABP2_38		0x38	// Adresse I2C = $38 sur 7bits
#define ADR_SENSOR_ABP2_48		0x48	// Adresse I2C = $48 sur 7bits
#define ABP2_MAX_TX_SIZE		3		// Envoi de 3 Bytes max
#define ABP2_MAX_RX_SIZE		7		// Réception de 7 Bytes max

#define ABP2_READ_CMD		0xAA		// commande de lecture 0xAA suive par 2 Octets à 0x0

#ifdef ABP2_READ_TEMPERATURE
#define ABP2_NEW_RX_SIZE		7		// NewValue = 7 Bytes à lire => status(1 Octet) + pression (3 Octets) + Température (3 Octets)
#else
#define ABP2_NEW_RX_SIZE		4		// NewValue = 4 Bytes à lire => status(1 Octet) + pression (3 Octets)
#endif

#define ABP2_INHO2_2_PA				   249.089f // 1 pouce d'eau = 249.089 Pa
#define ABP2_OUTPUT_MAX				15099494.0f
#define ABP2_OUTPUT_MIN				 1677722.0f
#define ABP2_PRESSURE_MAX	1 * ABP2_INHO2_2_PA // plage de mesure de la Doc -1 à +1 inHo2
#define ABP2_PRESSURE_MIN  -1 * ABP2_INHO2_2_PA
#define ABP2_PRES_COEF  	(ABP2_PRESSURE_MAX - ABP2_PRESSURE_MIN) / (ABP2_OUTPUT_MAX - ABP2_OUTPUT_MIN)

#define ABP2_TEMP_MAX  110.f // 110°C
#define ABP2_TEMP_MIN  -40.f // -40°C
#define ABP2_TEMP_COEF (ABP2_TEMP_MAX - ABP2_TEMP_MIN) / 16777215 // 16777215 = (2 puissance 24) - 1

/******************************************************************************/

typedef struct _I2CCM_Pres_ABP2_IntData
{
	I2CCM_InternalBaseData base;	// Base requise pour I2C_ComMaster
	uint32_t PreRaw;
	uint32_t TempRaw;
	uint16_t ErrorsCt;

	//****************************
	// Derniers échantillons converti sans moyennage pour du débug : (acces via stmMonitor)
	float PreConverted;
	float TempConverted;

	// Variables pour la Moyenne Glissante rapide :
	uint8_t AvgTabPresCurIdx;
	uint8_t MaxPresSample;
	float AvgTabPresEch[ABP2_AVG_PRES_ARRAY_MAX_SIZE];
	float AvgTabCumulPress;

	uint8_t AvgTabTempCurIdx;
	uint8_t MaxTempSample;
	float AvgTabTempEch[ABP2_AVG_TEMP_ARRAY_MAX_SIZE];
	float AvgTabCumulTemp;

} I2CCM_Pres_ABP2_IntData;	// Internal ABP2 Datas Struct

/******************************************************************************/
// Prototypes locaux :

static int16_t i2cSensorABP2_Init(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams, uint8_t adr);
static uint16_t i2cSensorABP2_NextActionFrame(I2CCM_Device *pDevice);
static uint16_t i2cSensorABP2_ActionComplete(I2CCM_Device *pDevice);

/******************************************************************************/
// Variables Internes :

I2CCM_Pres_ABP2_IntData mI2CCM_Pres_ABP2_IntData[I2CCM_NB_MAX_DEV_PRESS_ABP2];	// I2CCM_NB_MAX_DEV_PRESS_ABP2 est configuré dans "I2cComMasterConf.h"
#if defined(ABP2_MAX_TX_SIZE) && (ABP2_MAX_TX_SIZE > 0)
	uint8_t mI2CCM_Pressure_ABP2_TxBuf[ABP2_MAX_TX_SIZE];
#endif // MAX_TX_SIZE_CPT_38
uint8_t mI2CCM_Pressure_ABP2_RxBuf[ABP2_MAX_RX_SIZE];

/******************************************************************************/
// Point d'entrée Public pour l'Initialisation de tous les Capteurs de Pression ABP2 :

int16_t i2cCM_PressureABP2_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(pInitParams->intVarId >= I2CCM_NB_MAX_DEV_PRESS_ABP2) return 0;	// Can't assign InternalData for this VarId !
	if(0 == pInitParams->pExtStruct) return 0;	// Can't assign NullPtr for External Var Struct !

	int16_t DeviceType = pInitParams->device2Load;
	if(DeviceType <= I2CCM_LOAD_DEFAULT_DEVICE)
	{
		switch(DeviceType)
		{
		case I2CCM_LOAD_DEFAULT_DEVICE:	// Première Recommandation par défaut :
			DeviceType = I2cDevPresType_ABP2_38; // ABP2 Type 38
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE2:	// Seconde Recommandation par défaut :
			DeviceType = I2cDevPresType_ABP2_28; // ABP2 Type 28
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE3:	// Seconde Recommandation par défaut :
			DeviceType = I2cDevPresType_ABP2_48; // ABP2 Type 48
			break;
		// Insérer ici pour ajouter une recommandation d'ordre 4 ...
		default: // Recommandation non gérée :
			return 0;	// DeviceType not implemented !
			break;
		}
	}

	// Initialise selon le Type correspondant à la demande :
	switch(DeviceType)
	{
	case I2cDevPresType_ABP2_Unknown: // Retourne le nb de Types supportés :
		return I2cDevPresType_ABP2_OutOfRange -1; // = Last -1
		break;
	case I2cDevPresType_ABP2_28:
		return i2cSensorABP2_Init(DeviceType, pDevice, pInitParams, ADR_SENSOR_ABP2_28);
		break;
	case I2cDevPresType_ABP2_38:
		return i2cSensorABP2_Init(DeviceType, pDevice, pInitParams, ADR_SENSOR_ABP2_38);
		break;
	case I2cDevPresType_ABP2_48:
		return i2cSensorABP2_Init(DeviceType, pDevice, pInitParams, ADR_SENSOR_ABP2_48);
		break;
	default:
		break;
	}
	return 0;	// DeviceType not implemented !
}

/******************************************************************************/

static int16_t i2cSensorABP2_Init(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams, uint8_t adr)
{
	if(0 == pDevice) return 0;		// Can't load Device !
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(I2cDevPresType_ABP2_28 == DeviceType) // Type n°1 :
	{
		// Renseigne les Références :
		pDevice->DevAddr8		= I2CCM_MAKE_ADR8_WITH_RW_MASK(adr);
		pDevice->ActionId		= I2CCM_ACTION_INIT;
		pDevice->FrameId		= 0;   // Only 1 Frame => not used !

		// Renseigne les Fonctions de CallBack :
		pDevice->getNextActionFrame		= i2cSensorABP2_NextActionFrame;
		pDevice->handleActionComplete	= i2cSensorABP2_ActionComplete;

		// Initialise les Pointeurs :
		pDevice->pIntData = &mI2CCM_Pres_ABP2_IntData[pInitParams->intVarId]; // Internal Data Struct
		pDevice->pExtData = pInitParams->pExtStruct; // External Data Struct
		pDevice->pTxBuf =
#if defined(ABP2_MAX_TX_SIZE) && (ABP2_MAX_TX_SIZE > 0)
							mI2CCM_Pressure_ABP2_TxBuf;	// notre Buffer d'Envoi
#else // ! MAX_TX_SIZE_CPT_38
							0; // No Transmit
#endif // MAX_TX_SIZE_CPT_38
		pDevice->pRxBuf = mI2CCM_Pressure_ABP2_RxBuf;	// notre Buffer de Réception

		// Initialise les Structures & Buffers :
		I2CCM_FillMemory(pDevice->pIntData, 0, sizeof(I2CCM_Pres_ABP2_IntData));
		I2CCM_FillMemory(pDevice->pExtData, 0, sizeof(I2CCM_Pres_ABP2_ExtData));
#if defined(ABP2_MAX_TX_SIZE) && (ABP2_MAX_TX_SIZE > 0)
		I2CCM_FillMemory(pDevice->pTxBuf, 0, ABP2_MAX_TX_SIZE);
#endif // MAX_TX_SIZE_CPT_38
		I2CCM_FillMemory(pDevice->pRxBuf, 0, ABP2_MAX_RX_SIZE);

		// Initialise les Internal Valeurs Spécifiques :
		I2CCM_Pres_ABP2_IntData* pIntData = (I2CCM_Pres_ABP2_IntData*)pDevice->pIntData;
		pIntData->base.fullStructSize = sizeof(I2CCM_Pres_ABP2_IntData);
		pIntData->base.idOfDevice = DeviceType;	// For internal purpose only

		pIntData->MaxPresSample = 1;
		pIntData->MaxTempSample = 1;

		// Initialise les External Valeurs Spécifiques :
		I2CCM_Pres_ABP2_ExtData* pExtData = (I2CCM_Pres_ABP2_ExtData*)pDevice->pExtData;
		pExtData->base.fullStructSize = sizeof(I2CCM_Pres_ABP2_ExtData);
		pExtData->base.idOfDevice = DeviceType | pInitParams->devHandler | I2CCM_DEVICE_RECENTLY_LOADED_MSK;
		pExtData->Pressure		= ABP2_UNKNOWN_PRESSURE_PA;
		pExtData->Temperature	= ABP2_UNKNOWN_TEMPERATURE_DEG_C;

		// initialise la commande par defaut de lecture
		mI2CCM_Pressure_ABP2_TxBuf[0] = ABP2_READ_CMD;
	}
	//---------------------------------------------------
	else DeviceType = 0;	// DeviceType not implemented !
	return DeviceType;		// return loaded DeviceType
}

/******************************************************************************/

static uint16_t i2cSensorABP2_NextActionFrame(I2CCM_Device *pDevice)
{
	if(0 == pDevice)	return I2C_ABORT_DEV;	// Impossible de continuer sur ce Device !
	switch(pDevice->ActionId)
	{
	case I2CCM_ACTION_INIT:   // Initialisations à effectuer :
		// Comme il n'y a aucune Init à faire, passer directement à la lecture de la Pression :
		pDevice->ActionId = I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		pDevice->DelayInMs = ABP2_TEMPO_BEFORE_FIRST_ACTION; // 100ms de Tempo
		return I2C_END_BLOC; // rendre la main pour la suite ...
		break;
	//---------------------
	case I2CCM_QUERY_VALUES:  // Comment Récupérer les Valeurs ?
		pDevice->nbBytes2Send = ABP2_MAX_TX_SIZE;
		pDevice->nbBytes2Read = ABP2_NEW_RX_SIZE;
		pDevice->DelayInMs   = ABP2_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
		return I2C_TRANSMIT | I2C_RECEIVE | I2C_HANDLE_RX; // Demande Envoi + Lecture + CallBack de Réception
		break;
	//---------------------
	default:    // Not Handled correctly :
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

// Handler "ActionComplete"
static uint16_t i2cSensorABP2_ActionComplete(I2CCM_Device *pDevice)
{
	if(0 == pDevice)    return I2C_END_BLOC;

	if(I2CCM_QUERY_VALUES == pDevice->ActionId)
	{
		I2CCM_Pres_ABP2_IntData* pIntData = (I2CCM_Pres_ABP2_IntData*)pDevice->pIntData; // Pointer sur les Datas Internes
		I2CCM_Pres_ABP2_ExtData* pExtData = (I2CCM_Pres_ABP2_ExtData*)pDevice->pExtData; // Pointer sur les Datas Externes

		if(0 == pDevice->result) // No I2C error :
		{
			uint8_t *pBuf = (uint8_t *)pDevice->pRxBuf; // Pour pointer le Buffer de Réception
			pExtData->status.raw = pBuf[0];
			pExtData->newFlags |= I2cCmPressureAbp2NewStatus; // Signale un nouvel etat capteur
			if(pExtData->status.mathSat == 1 || pExtData->status.memError == 1 || pExtData->status.power == 0){
				// Error :
				I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt, UINT16_MAX)
				I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,  UINT16_MAX)
			}
			else if(pExtData->status.isBusy == 0) { // sensor ready
				// OK, pas d'ereur de réception :
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt,	I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,		I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				pExtData->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);

				// Recomposition des Datas Brutes :
				uint16_t pword = pBuf[1];
				pIntData->PreRaw = JOIN_16_16_BE(pword, JOIN_8_8_BE(pBuf[2], pBuf[3]));
				pIntData->PreConverted = ((pIntData->PreRaw - ABP2_OUTPUT_MIN) * ABP2_PRES_COEF) + ABP2_PRESSURE_MIN;

#ifdef ABP2_READ_TEMPERATURE
				uint16_t tword = pBuf[4];
				pIntData->TempRaw = JOIN_16_16_BE(tword, JOIN_8_8_BE(pBuf[5], pBuf[6]));
				pIntData->TempConverted = (pIntData->TempRaw * ABP2_TEMP_COEF) + ABP2_TEMP_MIN;
#endif

				// Calcul de la Moyenne glissante
#if defined(ABP2_MOY_PRES) && (ABP2_MOY_PRES > 1)
				if(pIntData->AvgTabPresCurIdx >= ABP2_AVG_PRES_ARRAY_MAX_SIZE) { pIntData->AvgTabPresCurIdx = 0; }
				// divise la valeur cumuler par le nombre d'echantillons limité à la taille max du tableau
				if(pIntData->MaxPresSample >= ABP2_AVG_PRES_ARRAY_MAX_SIZE){ pIntData->MaxPresSample = ABP2_AVG_PRES_ARRAY_MAX_SIZE; }
				else if(pIntData->MaxPresSample == 0){ pIntData->MaxPresSample = 1; }
				else { pIntData->MaxPresSample++; }

				pIntData->AvgTabCumulPress -= pIntData->AvgTabPresEch[pIntData->AvgTabPresCurIdx];
				pIntData->AvgTabCumulPress += pIntData->AvgTabPresEch[pIntData->AvgTabPresCurIdx] = pIntData->PreConverted;
				pIntData->AvgTabPresCurIdx++; // Incrémenter pour préparer pour la valeur suivante

				pExtData->Pressure = pIntData->AvgTabCumulPress / pIntData->MaxPresSample;
#else // ! ABP2_MOY_PRES
				pExtData->Pressure = pIntData->PreConverted;
#endif // ABP2_MOY_PRES

#if defined(ABP2_MOY_TEMP) && (ABP2_MOY_TEMP > 1)
				if(pIntData->AvgTabTempCurIdx >= ABP2_AVG_TEMP_ARRAY_MAX_SIZE) { pIntData->AvgTabTempCurIdx = 0; }
				// divise la valeur cumuler par le nombre d'echantillons limité à la taille max du tableau
				if(pIntData->MaxTempSample >= ABP2_AVG_TEMP_ARRAY_MAX_SIZE){ pIntData->MaxTempSample = ABP2_AVG_TEMP_ARRAY_MAX_SIZE; }
				else if(pIntData->MaxTempSample == 0){ pIntData->MaxTempSample = 1; }
				else { pIntData->MaxTempSample++; }

				pIntData->AvgTabCumulTemp -= pIntData->AvgTabTempEch[pIntData->AvgTabTempCurIdx];
				pIntData->AvgTabCumulTemp += pIntData->AvgTabTempEch[pIntData->AvgTabTempCurIdx] = pIntData->TempConverted;
				pIntData->AvgTabTempCurIdx++; // Incrémenter pour préparer pour la valeur suivante

				pExtData->Temperature = pIntData->AvgTabCumulTemp / pIntData->MaxTempSample;
#else // ! ABP2_MOY_TEMP
				pExtData->Temperature = pIntData->TempConverted;
#endif // ABP2_MOY_TEMP

				// Délai de prochaine action :
				pDevice->DelayInMs  = ABP2_TEMPO_BEFORE_NEXT_ACTION; // 1s avant prochaine action

				// Signaler à la tâche qu'il y a 1 nouvelle Data à récupérer :
				pExtData->newFlags |= I2cCmPressureAbp2NewValue; // Signale la nouvelle Data
				return I2C_NEW_VALUE | I2C_END_BLOC; // Signaler puis rendre la main
			}
		} else { // Error :
			I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt, UINT16_MAX)
			I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,  UINT16_MAX)
		}
		if(pIntData->ErrorsCt > ABP2_PRESS_MAX_ALLOWED_ERRORS)
		{
			pDevice->ActionId = I2CCM_ACTION_INIT; // Demander à Ré-Initialiser
			return I2C_ABORT_DEV; // +Recommander d'Abandonner l'usage de ce Device
		}
		pDevice->ActionId = I2CCM_QUERY_VALUES;
		pDevice->DelayInMs = ABP2_TEMPO_RETRY_ACTION; // Tempo avant Retry : 1s
		return I2C_END_BLOC | I2C_CHK_DEV_ERR; // Rendre la main + Demander à Vérifier si erreur sur le Device ...
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}


#ifdef __cplusplus
}
#endif
