/*
 * I2cDevPressureHSC.c
 *
 *  Created on: 29 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 25 Oct.  2021
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement ce Module "I2c_Pressure_HSC" à la Librairie "I2cComMaster" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "I2cDevPressureHSC.h"
 *
 */

#include "I2cDevPressureHSC.h"	// Pour accès à nos propres déclarations publiques

#include "I2cComMasterConf.h"	// Pour accès à la Configuration User souhaitée
#include "I2cComMasterUtils.h"	// Pour accès aux Outils de support I2cComMaster

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

typedef enum
{
	I2cDevPresType_HSC_Unknown = 0,	// Unknown HSC Type
	I2cDevPresType_HSC_28,			// Linked to CPT_HONEYWELL_1
	I2cDevPresType_HSC_38,			// Linked to CPT_HONEYWELL_2
	// Insérer ici tout nouveau type supporté par ce module de Pression HSC
	I2cDevPresType_HSC_OutOfRange,	// OutOfRange = nbEléments +1
} I2cDevPressureTypeHSC;

/******************************************************************************/

#define HSC_MOY_P_FROM_ECH_BRUT 	30	// Pour activer un Moyennage sur les échantillons bruts issus du Capteur
#define HSC_MOY_P_FROM_P_CALC		10	// Pour activer un Moyennage des valeurs déjà converties en Pression
#define HSC_NB_MOY_PRESSURE_AUTOZ	10	// Nb d'échantillons à prendre pour Déterminer le nouvel Offset

// Temporisations :
#define HSC_TEMPO_BEFORE_FIRST_ACTION	100		// 100ms avant 1° Lecture
#define HSC_TEMPO_REPLY_TIMEOUT			100		// 100ms d'attente max pour répondre
#define HSC_TEMPO_RETRY_ACTION			1000	// 1s avant Retry
#define HSC_TEMPO_BEFORE_NEXT_ACTION	1 *1000	// 1s avant de redemander une nouvelle Lecture

/******************************************************************************/

// Capteur de Pression HoneyWell AdrI2c $28 [0; -300Pa [ :
#define ADR_CPT_HONEYWELL_28	0x28	// Adresse I2C = $28 sur 7bits
#define MAX_TX_SIZE_CPT_28		0		// Envoi de 0 Bytes max
#define MAX_RX_SIZE_CPT_28		4		// Réception de 4 Bytes max
#define NEW_RX_SIZE_CPT_28		4		// NewValue = 4 Bytes à lire
#define OUTPUT_MAX_CPT_28		14745.0f
#define OUTPUT_MIN_CPT_28		1638.0f
#define PRESSURE_MAX_CPT_28		0.0f
#define PRESSURE_MIN_CPT_28		-3000.0f // <=> -300 Pa *10

/******************************************************************************/

// Capteur de Pression HoneyWell AdrI2c $38 [0; -1000Pa[ :
#define ADR_CPT_HONEYWELL_38	0x38 // Adresse I2C = $38 sur 7bits
#define MAX_TX_SIZE_CPT_38		0    // Envoi de 0 Bytes max
#define MAX_RX_SIZE_CPT_38		4    // Réception de 4 Bytes max
#define I2C_HSC_38_READ_PRESSURE_TX_SIZE	0	// QueryNewValue = 0 Bytes à envoyer
#define I2C_HSC_38_READ_PRESSURE_RX_SIZE	4	// NewValue = 4 Bytes à lire
#define OUTPUT_MIN_CPT_38		1638.0f
#define OUTPUT_MAX_CPT_38		14745.0f
#define PRESSURE_MIN_CPT_38		0.0f
#define PRESSURE_MAX_CPT_38		10000.0f // <=> 1000Pa *10

#define DEF_OFFSET_HONEYWELL_38	1638
#define MIN_OFFSET_HONEYWELL_38 1441	// = DEF_OFFSET_HONEYWELL_38 -15Pa
#define MAX_OFFSET_HONEYWELL_38 1834	// = DEF_OFFSET_HONEYWELL_38 +15Pa

/******************************************************************************/

// From "I2C Communications with Honeywell Digital Output Pressure Sensors.pdf" :
#define I2CCM_HSC_PRESSURE_FROM_OUTPUT_VALUE(a,b)	(((((a) - I2CCM_MAKE_NAME(OUTPUT_MIN_CPT_,b)) * (I2CCM_MAKE_NAME(PRESSURE_MAX_CPT_,b) - I2CCM_MAKE_NAME(PRESSURE_MIN_CPT_,b)))/(I2CCM_MAKE_NAME(OUTPUT_MAX_CPT_,b) - I2CCM_MAKE_NAME(OUTPUT_MIN_CPT_,b))) + I2CCM_MAKE_NAME(PRESSURE_MIN_CPT_,b) ) /10.0
#define I2CCM_HSC_PRESSURE_FROM_OUTPUT_DELTA(a,b)	((((a) * (I2CCM_MAKE_NAME(PRESSURE_MAX_CPT_,b) - I2CCM_MAKE_NAME(PRESSURE_MIN_CPT_,b)))/(I2CCM_MAKE_NAME(OUTPUT_MAX_CPT_,b) - I2CCM_MAKE_NAME(OUTPUT_MIN_CPT_,b))) + I2CCM_MAKE_NAME(PRESSURE_MIN_CPT_,b) ) /10.0
#define I2CCM_HSC_TEMPERATURE_FROM_OUTPUT_VALUE(a)	((((a) * 200.0f)/(2047.0f)) - 50.0f)

/******************************************************************************/

typedef struct _I2CCM_Pres_HSC_IntData
{
	I2CCM_InternalBaseData base;	// Base requise pour I2C_ComMaster
	uint16_t BridgeBrut;
	uint16_t TemperatureBrut;
	uint16_t ErrorsCt;
	uint16_t BridgeOffset;  // = Shared_Pressure_Offset (utilisé par DEVICE_TYPE_38)
	uint16_t BrdgOfstOpId;	// = Shared_PresOfst_Action (utilisé par DEVICE_TYPE_38)

	// Variables pour la Moyenne Glissante rapide :
#ifdef HSC_MOY_P_FROM_ECH_BRUT
	uint16_t TabMoyCurIdxE;
	uint16_t maxEchBrut4Pmoy;		// Pour limiter à N échantillons bruts parmi HSC_MOY_P_FROM_ECH_BRUT
	uint16_t TabMoyEch[HSC_MOY_P_FROM_ECH_BRUT];
	uint32_t TabMoyCumul;
#endif // HSC_MOY_P_FROM_ECH_BRUT
#ifdef HSC_MOY_P_FROM_P_CALC
	uint16_t TabMoyCurIdxP;
	uint16_t maxPresBrut4Moy;		// Pour limiter à N valeurs brutes de Pressions parmi HSC_MOY_P_FROM_ECH_BRUT
	float	 Tab_Moy_P[HSC_MOY_P_FROM_P_CALC];
	float	 CumulPressure;
#endif // HSC_MOY_P_FROM_P_CALC

} I2CCM_Pres_HSC_IntData;	// Internal HSC Datas Struct

/******************************************************************************/
// Prototypes locaux :

static int16_t i2cCM_PressureHSC_Init_28(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);
static uint16_t getI2cCM_PressureHSC_NextActionFrame_28(I2CCM_Device *pDevice);

static int16_t i2cCM_PressureHSC_Init_38(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);
static uint16_t getI2cCM_PressureHSC_NextActionFrame_38(I2CCM_Device *pDevice);

static uint16_t handleI2cCM_PressureHSC_ActionComplete(I2CCM_Device *pDevice);
//int getI2C_PressureHSC_RW_VarPtrSize(int varId, int varContext, void *pDevice, void* *pPtr, int *pSize);

// Pour version avec "Contrôle de l'Offset" déporté dans une fonction complémentaire :
//int handleI2C_PressureHSC_PostWriteOffset(int varId, int varContext, void *pDevice, void* *pPtr, int *pSize);

uint16_t getI2C_HSC_Pressure_UserOffset(I2CCM_DevInitParams* pInitParams);

#ifdef HSC_MOY_P_FROM_ECH_BRUT
	uint16_t i2cCM_ReframeHSC_MaxEchBrut4Pmoy(uint16_t newValue);
	uint16_t getI2CCM_HSC_MaxPresBrut4Moy(I2CCM_DevInitParams* pInitParams);
#endif // HSC_MOY_P_FROM_ECH_BRUT

#ifdef HSC_MOY_P_FROM_P_CALC
	uint16_t i2cCM_ReframeHSC_MaxPresBrut4Moy(uint16_t newValue);
	uint16_t getI2CCM_HSC_MaxEchBrut4Pmoy(I2CCM_DevInitParams* pInitParams);
#endif // HSC_MOY_P_FROM_P_CALC

/******************************************************************************/
// Variables Internes :

I2CCM_Pres_HSC_IntData mI2CCM_Pres_HSC_IntData[I2CCM_NB_MAX_DEV_PRESS_HSC];	// MAX_NB_I2C_DEV_PRESS_HSC est configuré dans "I2cComMasterConf.h"
#if defined(MAX_TX_SIZE_CPT_38) && (MAX_TX_SIZE_CPT_38 > 0)
	uint8_t mI2CCM_Pressure_HSC_TxBuf[MAX_TX_SIZE_CPT_38];
#endif // MAX_TX_SIZE_CPT_38
uint8_t mI2CCM_Pressure_HSC_RxBuf[MAX_RX_SIZE_CPT_38];

/******************************************************************************/
// Point d'entrée Public pour l'Initialisation de tous les Capteurs de Pression HSC :

int16_t i2cCM_PressureHSC_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(pInitParams->intVarId >= I2CCM_NB_MAX_DEV_PRESS_HSC) return 0;	// Can't assign InternalData for this VarId !
	if(0 == pInitParams->pExtStruct) return 0;	// Can't assign NullPtr for External Var Struct !

	int16_t DeviceType = pInitParams->device2Load;
	if(DeviceType <= I2CCM_LOAD_DEFAULT_DEVICE)
	{
		switch(DeviceType)
		{
		case I2CCM_LOAD_DEFAULT_DEVICE:	// Première Recommandation par défaut :
			DeviceType = I2cDevPresType_HSC_38; // HSC Type 38
			break;
#ifndef HSC_IGNORE_28
		case I2CCM_LOAD_DEFAULT_DEVICE2:	// Seconde Recommandation par défaut :
			DeviceType = I2cDevPresType_HSC_28; // HSC Type 28
			break;
#endif
		// Insérer ici pour ajouter une recommandation d'ordre 3 ou 4 ...
		default: // Recommandation non gérée :
			return 0;	// DeviceType not implemented !
			break;
		}
	}

	// Initialise selon le Type correspondant à la demande :
	switch(DeviceType)
	{
	case I2cDevPresType_HSC_Unknown: // Retourne le nb de Types supportés :
		return I2cDevPresType_HSC_OutOfRange -1; // = Last -1
		break;
	case I2cDevPresType_HSC_28:
		return i2cCM_PressureHSC_Init_28(DeviceType, pDevice, pInitParams);
		break;
	case I2cDevPresType_HSC_38:
		return i2cCM_PressureHSC_Init_38(DeviceType, pDevice, pInitParams);
		break;
	default:
		break;
	}
	return 0;	// DeviceType not implemented !
}

/******************************************************************************/

static int16_t i2cCM_PressureHSC_Init_28(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pDevice) return 0;		// Can't load Device !
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(I2cDevPresType_HSC_28 == DeviceType) // Type n°1 :
	{
		// Renseigne les Références :
		pDevice->DevAddr8		= I2CCM_MAKE_ADR8_WITH_RW_MASK(ADR_CPT_HONEYWELL_28);
		pDevice->ActionId		= I2CCM_ACTION_INIT;
		pDevice->FrameId		= 0;   // Only 1 Frame => not used !

		// Renseigne les Fonctions de CallBack :
		pDevice->getNextActionFrame		= getI2cCM_PressureHSC_NextActionFrame_28;
		pDevice->handleActionComplete	= handleI2cCM_PressureHSC_ActionComplete;

		// Initialise les Pointeurs :
		pDevice->pIntData = &mI2CCM_Pres_HSC_IntData[pInitParams->intVarId]; // Internal Data Struct
		pDevice->pExtData = pInitParams->pExtStruct; // External Data Struct
		pDevice->pTxBuf =
#if defined(MAX_TX_SIZE_CPT_28) && (MAX_TX_SIZE_CPT_28 > 0)
							mI2CCM_Pressure_HSC_TxBuf;	// notre Buffer d'Envoi
#else // ! MAX_TX_SIZE_CPT_38
							0; // No Transmit
#endif // MAX_TX_SIZE_CPT_38
		pDevice->pRxBuf = mI2CCM_Pressure_HSC_RxBuf;	// notre Buffer de Réception

		// Initialise les Structures & Buffers :
		I2CCM_FillMemory(pDevice->pIntData, 0, sizeof(I2CCM_Pres_HSC_IntData));
		I2CCM_FillMemory(pDevice->pExtData, 0, sizeof(I2CCM_Pres_HSC_ExtData));
#if defined(MAX_TX_SIZE_CPT_28) && (MAX_TX_SIZE_CPT_28 > 0)
		I2CCM_FillMemory(pDevice->pTxBuf, 0, MAX_TX_SIZE_CPT_28);
#endif // MAX_TX_SIZE_CPT_38
		I2CCM_FillMemory(pDevice->pRxBuf, 0, MAX_RX_SIZE_CPT_28);

		// Initialise les Internal Valeurs Spécifiques :
		I2CCM_Pres_HSC_IntData* pIntData = (I2CCM_Pres_HSC_IntData*)pDevice->pIntData;
		pIntData->base.fullStructSize = sizeof(I2CCM_Pres_HSC_IntData);
		pIntData->base.idOfDevice = DeviceType;	// For internal purpose only

		// Initialise les External Valeurs Spécifiques :
		I2CCM_Pres_HSC_ExtData* pExtData = (I2CCM_Pres_HSC_ExtData*)pDevice->pExtData;
		pExtData->base.fullStructSize = sizeof(I2CCM_Pres_HSC_ExtData);
		pExtData->base.idOfDevice = DeviceType | pInitParams->devHandler | I2CCM_DEVICE_RECENTLY_LOADED_MSK;
		pExtData->Pressure		= HSC_UNKNOWN_PRESSURE_PA;
		pExtData->Temperature	= HSC_UNKNOWN_TEMPERATURE_DEG_C;
	}
	//---------------------------------------------------
	else DeviceType = 0;	// DeviceType not implemented !
	return DeviceType;		// return loaded DeviceType
}

/******************************************************************************/

static uint16_t getI2cCM_PressureHSC_NextActionFrame_28(I2CCM_Device *pDevice)
{
	if(0 == pDevice)	return I2C_ABORT_DEV;	// Impossible de continuer sur ce Device !
	switch(pDevice->ActionId)
	{
	case I2CCM_ACTION_INIT:   // Initialisations à effectuer :
		// Comme il n'y a aucune Init à faire, passer directement à la lecture de la Pression :
		pDevice->ActionId = I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		pDevice->DelayInMs = HSC_TEMPO_BEFORE_FIRST_ACTION; // 100ms de Tempo
		return I2C_END_BLOC; // rendre la main pour la suite ...
		break;
	//---------------------
	case I2CCM_QUERY_VALUES:  // Comment Récupérer les Valeurs ?
		pDevice->nbBytes2Send = 0; // No Data to Transmit
		pDevice->nbBytes2Read = NEW_RX_SIZE_CPT_28; // 4 Bytes to Receive
		pDevice->DelayInMs   = HSC_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
		return I2C_RECEIVE | I2C_HANDLE_RX; // Demande Lecture + CallBack de Réception
		break;
	//---------------------
	default:    // Not Handled correctly :
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

static int16_t i2cCM_PressureHSC_Init_38(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pDevice) return 0;		// Can't load Device !
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(I2cDevPresType_HSC_38 == DeviceType) // Type n°2 :
	{
		// Renseigne les Références :
		pDevice->DevAddr8		= I2CCM_MAKE_ADR8_WITH_RW_MASK(ADR_CPT_HONEYWELL_38);
		pDevice->ActionId		= I2CCM_ACTION_INIT;
		pDevice->FrameId		= 0;   // Only 1 Frame => not used !

		// Renseigne les Fonctions de CallBack :
		pDevice->getNextActionFrame		= getI2cCM_PressureHSC_NextActionFrame_38;
		pDevice->handleActionComplete	= handleI2cCM_PressureHSC_ActionComplete;

		// Initialise les Pointeurs :
		pDevice->pIntData = &mI2CCM_Pres_HSC_IntData[pInitParams->intVarId]; // Internal Data Struct
		pDevice->pExtData = pInitParams->pExtStruct; // External Data Struct
		pDevice->pTxBuf =
#if defined(MAX_TX_SIZE_CPT_38) && (MAX_TX_SIZE_CPT_38 > 0)
							mI2CCM_Pressure_HSC_TxBuf;	// notre Buffer d'Envoi
#else // ! MAX_TX_SIZE_CPT_38
							0; // No Transmit
#endif // MAX_TX_SIZE_CPT_38
		pDevice->pRxBuf = mI2CCM_Pressure_HSC_RxBuf;	// notre Buffer de Réception

		// Initialise les Structures & Buffers :
		I2CCM_FillMemory(pDevice->pIntData, 0, sizeof(I2CCM_Pres_HSC_IntData));
		I2CCM_FillMemory(pDevice->pExtData, 0, sizeof(I2CCM_Pres_HSC_ExtData));
#if defined(MAX_TX_SIZE_CPT_38) && (MAX_TX_SIZE_CPT_38 > 0)
		I2CCM_FillMemory(pDevice->pTxBuf, 0, MAX_TX_SIZE_CPT_38);
#endif // MAX_TX_SIZE_CPT_38
		I2CCM_FillMemory(pDevice->pRxBuf, 0, MAX_RX_SIZE_CPT_38);

		// Initialise les Internal Valeurs Spécifiques :
		I2CCM_Pres_HSC_IntData* pIntData = (I2CCM_Pres_HSC_IntData*)pDevice->pIntData;
		pIntData->base.fullStructSize = sizeof(I2CCM_Pres_HSC_IntData);
		pIntData->base.idOfDevice = DeviceType;	// For internal purpose only
		pIntData->BridgeOffset	= i2cCM_ReframeOffsetInAllowedRange(getI2C_HSC_Pressure_UserOffset(pInitParams)); // uniquement pour DEVICE_TYPE_38

#ifdef HSC_MOY_P_FROM_ECH_BRUT
		uint16_t maxEchBrut4Pmoy = getI2CCM_HSC_MaxEchBrut4Pmoy(pInitParams);
		pIntData->maxEchBrut4Pmoy = i2cCM_ReframeHSC_MaxEchBrut4Pmoy(maxEchBrut4Pmoy);
#endif // HSC_MOY_P_FROM_ECH_BRUT

#ifdef HSC_MOY_P_FROM_P_CALC
		uint16_t maxPresBrut4Moy = getI2CCM_HSC_MaxPresBrut4Moy(pInitParams);
		pIntData->maxPresBrut4Moy = i2cCM_ReframeHSC_MaxPresBrut4Moy(maxPresBrut4Moy);
#endif // HSC_MOY_P_FROM_P_CALC

		// Initialise les External Valeurs Spécifiques :
		I2CCM_Pres_HSC_ExtData* pExtData = (I2CCM_Pres_HSC_ExtData*)pDevice->pExtData;
		pExtData->base.fullStructSize = sizeof(I2CCM_Pres_HSC_ExtData);
		pExtData->base.idOfDevice = DeviceType | pInitParams->devHandler | I2CCM_DEVICE_RECENTLY_LOADED_MSK;
		pExtData->BridgeOffset	= pIntData->BridgeOffset; // uniquement pour DEVICE_TYPE_38
		pExtData->Pressure		= HSC_UNKNOWN_PRESSURE_PA;
		pExtData->Temperature	= HSC_UNKNOWN_TEMPERATURE_DEG_C;
	}
	//---------------------------------------------------
	else DeviceType = 0;	// DeviceType not implemented !
	return DeviceType;		// return loaded DeviceType
}

/******************************************************************************/

uint16_t i2cCM_ReframeOffsetInAllowedRange(uint16_t newOffset)
{
	if( (newOffset < MIN_OFFSET_HONEYWELL_38) || (newOffset > MAX_OFFSET_HONEYWELL_38) )
	{
		newOffset = DEF_OFFSET_HONEYWELL_38; // Ré-affecte l'Offset par défaut si hors plage autorisée
	}
	return newOffset;
}

/******************************************************************************/

#ifdef HSC_MOY_P_FROM_ECH_BRUT
uint16_t i2cCM_ReframeHSC_MaxEchBrut4Pmoy(uint16_t newValue)
{
	return ((newValue < 1)||(newValue > HSC_MOY_P_FROM_ECH_BRUT)) ? HSC_MOY_P_FROM_ECH_BRUT : newValue;
}
#endif // HSC_MOY_P_FROM_ECH_BRUT

/******************************************************************************/

#ifdef HSC_MOY_P_FROM_P_CALC
uint16_t i2cCM_ReframeHSC_MaxPresBrut4Moy(uint16_t newValue)
{
	return ((newValue < 1)||(newValue > HSC_MOY_P_FROM_P_CALC)) ? HSC_MOY_P_FROM_P_CALC : newValue;
}
#endif // HSC_MOY_P_FROM_P_CALC

/******************************************************************************/

static uint16_t getI2cCM_PressureHSC_NextActionFrame_38(I2CCM_Device *pDevice)
{
	if(0 == pDevice)	return I2C_ABORT_DEV;	// Impossible de continuer sur ce Device !
	switch(pDevice->ActionId)
	{
	case I2CCM_ACTION_INIT:   // Initialisations à effectuer :
//		// On a besoin de l'Offset du Capteur de Pression !
//		pDevice->result = I2cPressureHscVarId_BridgeOffset;
		pDevice->ActionId = I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		pDevice->DelayInMs = HSC_TEMPO_BEFORE_FIRST_ACTION; // 100ms de Tempo
		return I2C_END_BLOC; // rendre la main pour la suite ...
//		return I2C_NEED_PARAM | I2C_END_BLOC;
		break;
	//---------------------
	case I2CCM_QUERY_VALUES:  // Comment Récupérer les Valeurs ?
		pDevice->nbBytes2Send = 0; // No Data to Transmit
		pDevice->nbBytes2Read = I2C_HSC_38_READ_PRESSURE_RX_SIZE; // 4 Bytes to Receive
		pDevice->DelayInMs   = HSC_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
		return I2C_RECEIVE | I2C_HANDLE_RX; // Demande Lecture + CallBack de Réception
		break;
	//---------------------
	case I2CCM_RETURN_VALUES:
		pDevice->result = pDevice->FrameId;	// Transfert vers le Code Variable à récupérer
		pDevice->ActionId = I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		return I2C_NEW_VALUE | I2C_END_BLOC; // Récupérer puis rendre la main;
		break;
	//---------------------
	case I2CCM_REDIRECT_ACTION:
		pDevice->ActionId = I2CCM_GET_16L(pDevice->result); // Nouveau CodeAction
		uint16_t ret      = I2CCM_GET_16H(pDevice->result); // Valeur à retourner
		pDevice->result   = pDevice->FrameId;
		return ret;
		break;
	//---------------------
	default:    // Not Handled correctly :
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

// Handler "ActionComplete" partagé HSC_28 & HSC_38 :
static uint16_t handleI2cCM_PressureHSC_ActionComplete(I2CCM_Device *pDevice)
{
	if(0 == pDevice)    return I2C_END_BLOC;

	if(I2CCM_QUERY_VALUES == pDevice->ActionId)
	{
		I2CCM_Pres_HSC_IntData* pIntData = (I2CCM_Pres_HSC_IntData*)pDevice->pIntData; // Pointer sur les Datas Internes
		I2CCM_Pres_HSC_ExtData* pExtData = (I2CCM_Pres_HSC_ExtData*)pDevice->pExtData; // Pointer sur les Datas Externes

		if(0 == pDevice->result) // No I2C error :
		{
			uint8_t *pBuf = (uint8_t *)pDevice->pRxBuf; // Pour pointer le Buffer de Réception

			if(0x00 == ((pBuf[0] >>6) & 0x03)) // Bits "Normal Operation" :
			{
				float NewPressure;
				// OK, pas d'ereur de réception :
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt,	I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,		I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				pExtData->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);

				// Préparation AutoZéro :
				if( (0 != (I2CCM_AUTOZERO_MAKE_ACTION_ID(I2cCmPressureHscAutoZeroActive) & pIntData->BrdgOfstOpId)) // Flag AutoZéro Actif ?
				 && (0 == I2CCM_AUTOZERO_GET_STEP_ID(pIntData->BrdgOfstOpId)) ) // + Pas initialisé ?
				{
					pIntData->BrdgOfstOpId += HSC_NB_MOY_PRESSURE_AUTOZ; // Pré-charge le nb de Captures à prendre ...
				}

				// Recomposition des Datas Brutes :
				uint16_t valeurBridgeBrut = I2CCM_MAKE_UINT16_FROM_BIG_ENDIAN(pBuf[0] & 0x3F, pBuf[1]);
#ifdef HSC_GET_BRIDGE_BRUT
				pExtData->WheatstoneBridgeBrut = valeurBridgeBrut;
#endif // HSC_GET_BRIDGE_BRUT

				uint16_t tmpTemperatureBrut = I2CCM_MAKE_UINT16_FROM_BIG_ENDIAN(pBuf[2], pBuf[3] & 0xE0)>>5;
				pExtData->Temperature = I2CCM_HSC_TEMPERATURE_FROM_OUTPUT_VALUE((float)tmpTemperatureBrut);
#ifdef HSC_GET_TEMP_BRUT
				pExtData->TemperatureBrut = tmpTemperatureBrut;
#endif // HSC_GET_TEMP_BRUT

				// Calcul d'une Moyenne glissante sur la base des "Echantillons Bruts" venant du Capteur :
				float tmpBridgeValue;
#if defined(HSC_MOY_P_FROM_ECH_BRUT) && (HSC_MOY_P_FROM_ECH_BRUT > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				uint16_t maxEchBrut4Pmoy = i2cCM_ReframeHSC_MaxEchBrut4Pmoy(pIntData->maxEchBrut4Pmoy);

				uint16_t curIndexEch = pIntData->TabMoyCurIdxE;
				if(curIndexEch >= maxEchBrut4Pmoy) { curIndexEch = 0; }
				pIntData->TabMoyCumul -= pIntData->TabMoyEch[curIndexEch];
				pIntData->TabMoyCumul += pIntData->TabMoyEch[curIndexEch] = valeurBridgeBrut;
				pIntData->TabMoyCurIdxE = curIndexEch +1; // Incrémenter pour préparer pour la valeur suivante

				tmpBridgeValue = ((float)(pIntData->TabMoyCumul) / (float)maxEchBrut4Pmoy);
#else // ! HSC_MOY_P_FROM_ECH_BRUT
//                tmpBridgeValue = pIntData->BridgeBrut;
				tmpBridgeValue = (float)valeurBridgeBrut;
#endif // HSC_MOY_P_FROM_ECH_BRUT
#ifdef HSC_GET_BRIDGE_MOY
				pExtData->BridgeMoy = tmpBridgeValue;
#endif // HSC_GET_BRIDGE_MOY

				if( (0 != (I2CCM_AUTOZERO_MAKE_ACTION_ID(I2cCmPressureHscAutoZeroActive) & pIntData->BrdgOfstOpId)) // Flag AutoZéro Actif ?
					&& (I2CCM_AUTOZERO_GET_STEP_ID(pIntData->BrdgOfstOpId) > 0) ) // + Captures en attente :
				{
					pIntData->BridgeOffset = tmpBridgeValue;	// Capture notre nouvel Offset !
					pIntData->BrdgOfstOpId--; // Signale qu'on a bien capturé le nouvel Offset
				}

				// Calcul des Résultats :
				//                switch(pDevice->DeviceType)
				switch(pIntData->base.idOfDevice)
				{
				case I2cDevPresType_HSC_38: // DEVICE_TYPE_38:
					NewPressure = I2CCM_HSC_PRESSURE_FROM_OUTPUT_DELTA(tmpBridgeValue - (float)pIntData->BridgeOffset, 38); // avec Params pour HSC_38
					break;
				case I2cDevPresType_HSC_28: // DEVICE_TYPE_28:
				default:
					NewPressure = I2CCM_HSC_PRESSURE_FROM_OUTPUT_VALUE(tmpBridgeValue, 28); // avec Params pour HSC_28
					break;
				}
#ifdef HSC_GET_PRESS_BRUT
				pExtData->PresBrut = NewPressure;
#endif // HSC_GET_PRESS_BRUT

				// Ajout_Jp le 08/01/2020 : Calcul de la Moyenne glissante :
#if defined(HSC_MOY_P_FROM_P_CALC) && (HSC_MOY_P_FROM_P_CALC > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				uint16_t maxPresBrut4Moy = i2cCM_ReframeHSC_MaxPresBrut4Moy(pIntData->maxPresBrut4Moy);

				uint16_t curIndexPress = pIntData->TabMoyCurIdxP;
				if(curIndexPress >= maxPresBrut4Moy) { curIndexPress = 0; }
				pIntData->CumulPressure -= pIntData->Tab_Moy_P[curIndexPress];
				pIntData->CumulPressure += pIntData->Tab_Moy_P[curIndexPress] = NewPressure;
				pIntData->TabMoyCurIdxP = curIndexPress +1; // Incrémenter pour préparer pour la valeur suivante

				pExtData->Pressure = (pIntData->CumulPressure / (float)maxPresBrut4Moy);
#else // ! HSC_MOY_P_FROM_P_CALC
				pExtData->Pressure = NewPressure;
#endif // HSC_MOY_P_FROM_P_CALC

				// Délai de prochaine action :
				pDevice->DelayInMs  = HSC_TEMPO_BEFORE_NEXT_ACTION; // 10s avant prochaine action

				if( (0 != (I2CCM_AUTOZERO_MAKE_ACTION_ID(I2cCmPressureHscAutoZeroActive) & pIntData->BrdgOfstOpId)) // Flag AutoZéro Actif ?
				 && (0 == I2CCM_AUTOZERO_GET_STEP_ID(pIntData->BrdgOfstOpId)) ) // + Terminé :
				{
					pIntData->BridgeOffset = i2cCM_ReframeOffsetInAllowedRange(pIntData->BridgeOffset);
					if(pExtData->BridgeOffset != pIntData->BridgeOffset)
					{
						pExtData->BridgeOffset = pIntData->BridgeOffset;
						pExtData->newFlags |= I2cCmPressureHscNewOffset; // Signale le Nouveau Offset
					}

					// Signaler au Gestionaire qu'on a un nouvel Offset :
					pIntData->BrdgOfstOpId ^= (I2cCmPressureHscAutoZeroEnded | I2CCM_AUTOZERO_MAKE_ACTION_ID(I2cCmPressureHscAutoZeroActive)); // Signaler Terminé + Désactiver le mode AutoZéro
					return I2C_END_BLOC;	// Rendre tout de suite la main
				}

				// Signaler à la tâche qu'il y a 1 nouvelle Data à récupérer :
				pExtData->newFlags |= I2cCmPressureHscNewPressure; // Signale la nouvelle Data
				return I2C_NEW_VALUE | I2C_END_BLOC; // Signaler puis rendre la main
			}
		} else { // Error :
			I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt, UINT16_MAX)
			I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,  UINT16_MAX)
		}
		if(pIntData->ErrorsCt > HSC_PRESS_MAX_ALLOWED_ERRORS)
		{
			pDevice->ActionId = I2CCM_ACTION_INIT; // Demander à Ré-Initialiser
			return I2C_ABORT_DEV; // +Recommander d'Abandonner l'usage de ce Device
		}
		pDevice->ActionId = I2CCM_QUERY_VALUES;
		pDevice->DelayInMs = HSC_TEMPO_RETRY_ACTION; // Tempo avant Retry : 1s
		return I2C_END_BLOC | I2C_CHK_DEV_ERR; // Rendre la main + Demander à Vérifier si erreur sur le Device ...
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

__attribute__((weak)) uint16_t getI2C_HSC_Pressure_UserOffset(I2CCM_DevInitParams* pInitParams)
{
	return DEF_OFFSET_HONEYWELL_38; // Internal Librairie option : UserOffset = DefaultOffset
}

/******************************************************************************/

#ifdef HSC_MOY_P_FROM_ECH_BRUT
__attribute__((weak)) uint16_t getI2CCM_HSC_MaxEchBrut4Pmoy(I2CCM_DevInitParams* pInitParams)
{
	return HSC_MOY_P_FROM_ECH_BRUT; // Internal Librairie option : maxEchBrut4Pmoy = HSC_MOY_P_FROM_ECH_BRUT
}
#endif // HSC_MOY_P_FROM_ECH_BRUT

/******************************************************************************/

#ifdef HSC_MOY_P_FROM_P_CALC
__attribute__((weak)) uint16_t getI2CCM_HSC_MaxPresBrut4Moy(I2CCM_DevInitParams* pInitParams)
{
	return HSC_MOY_P_FROM_P_CALC; // Internal Librairie option : maxPresBrut4Moy = HSC_MOY_P_FROM_P_CALC
}
#endif // HSC_MOY_P_FROM_P_CALC

#ifdef __cplusplus
}
#endif
