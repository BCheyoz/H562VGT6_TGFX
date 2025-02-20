/*
 * I2cDevPressureLMI.c
 *
 *  Created on: 26 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 03 Nov. 2022
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement ce Module "I2c_Pressure_LMI" à la Librairie "I2cComMaster" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "I2cDevPressureLMI.h"
 *
 *  Ce module est (théoriquement) compatible avec les références suivantes :
 *  (From "DS_Standard-LMI_E_11823.pdf" page 18)
 *-> LMI S025 [B/U] B 3 S (  25 Pa /  0.1 inH2O)
 *-> LMI S050 [B/U] B 3 S (  50 Pa /  0.2 inH2O)
 *-> LMI S100 [B/U] B 3 S ( 100 Pa /  0.4 inH2O)
 *-> LMI S250 [B/U] B 3 S ( 250 Pa /  1.0 inH2O)
 *-> LMI S500 [B/U] B 3 S ( 500 Pa /  2.0 inH2O)
 *-> LMI M012 [B/U] B 3 S (1250 Pa /  5.0 inH2O)
 *-> LMI M025 [B/U] B 3 S (2500 Pa / 10.0 inH2O)
 *-> LMI M050 [B/U] B 3 S (5000 Pa / 20.0 inH2O)
 *
 */

#include "I2cDevPressureLMI.h"		// Pour accès à nos propres déclarations publiques

#include "I2cComMasterConf.h"		// Pour accès à la Configuration User souhaitée
#include "I2cComMasterUtils.h"  	// Pour accès aux Outils de support I2cComMaster

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

// Prototypes des Fonctions de Configuration Externes (elles sont appelées en cas de besoin) :
// -> En cas d'absence, une fonction Interne équivalente par défaut est appelée à la place.

#ifdef LMI_MOY_P_FROM_RAW
	uint16_t getI2CCM_LMI_MaxRawPresCt4Moy(I2CCM_DevInitParams* pInitParams);
#endif // LMI_MOY_P_FROM_RAW

#ifdef LMI_MOY_P_FROM_CONV
	uint16_t getI2CCM_LMI_MaxConvPresCt4Moy(I2CCM_DevInitParams* pInitParams);
#endif // LMI_MOY_P_FROM_CONV

/******************************************************************************/

// Temporisations :
#define LMI_TEMPO_BEFORE_FIRST_ACTION	100		// 100ms avant 1° Lecture (Power-up time = 25ms max)
#define LMI_TEMPO_REPLY_TIMEOUT 		100		// 100ms d'attente max pour répondre
#define LMI_TEMPO_RETRY_ACTION			1000	// 1s avant Retry
#define LMI_TEMPO_BEFORE_NEXT_ACTION	1 *1000	// 1s avant de redemander une nouvelle Lecture

/******************************************************************************/

// Capteur de Pression FirstSensor LMI xxxx x B 3 S :
#define LMI_BASE_ADR_CPT	0x5C	// Adresse I2C LMI_Adr00 = 0x25 sur 7bits

#define LMI_STD_TX_SIZE 	2	// Envoi de 2 Bytes
#define LMI_MAX_TX_SIZE 	2	// Envoi de 2 Bytes max

#ifndef LMI_GET_TEMP_BRUT
	#define LMI_STD_RX_SIZE 	2	// Réception de 2 Bytes = Pressure brut
	#define LMI_MAX_RX_SIZE 	2	// Réception de 1 bloc de 2 Bytes max
#else
	#define LMI_STD_RX_SIZE 	4	// Réception de 2*2 Bytes = Pressure brut + Temperature brut
	#define LMI_MAX_RX_SIZE 	4	// Réception de 2 blocs de 2 Bytes max
#endif // LMI_GET_TEMP_BRUT
// Remarque : la lecture de "ElectronicSignature" se fera directement vers le Buffer de la Variable Externe de Sortie

// Commandes I2C pour les capteurs LMI (from "DS_Standard-LMI_E_11823.pdf" ) :
#define LMI_CMD_START_PRESS_CONV	0x20	// "Writing this command to the LMI device starts a conversion of pressure data."
#define LMI_CMD_DO_RESET_FIRMWARE	0x11	// "Writing this command to the LMI device resets the firmware’s program counter."
#define LMI_CMD_READ_ELEC_SIGNAT	0x23	// "Writing this command to the LMI devices retrieves the sensor’s electronic signature."

/******************************************************************************/
/* Scale Factors, from "DS_Standard-LMI_E_11823.pdf" p3 à 6 :
 *
 *
 */
#define LMI_DEFAUT_SCALE_FACTOR_P		24	// 24 pour 1250Pa
#define LMI_DEFAUT_SCALE_FACTOR_T		72	// Scale factor(TS) = 72 counts/°C
#define LMI_REF_TEMPERATURE				26.8f// 24//23//25
#define LMI_VALUE_AT_REF_TEMP			0	//-180//-100//294//100

/******************************************************************************/
// Opérations à effectuer à l'Init :
typedef enum
{
	I2cCmLmiInitFrameResetSoft, 		// Demander un Reset Soft
	//---------------------------
	I2cCmLmiInitFrameReadElecSignat,	// Demander à récupérer les infos "Electronic Signature"
	I2cCmLmiInitFrameStartPressConv,	// Lancer les mesures de Pression
	//-------------------------
// Ajouter ci-dessus la Référence à d'autres éventuelles Requêtes @ "InitTime" ...
	//-------------------------
	I2cCmLmiInitFrameLast,				// Fin des Requêtes à faire à l'Init
//	I2cCmLmiInitFrameFirst = I2cCmLmiInitFrameResetSoft,
	I2cCmLmiInitFrameFirst = I2cCmLmiInitFrameReadElecSignat, // Modif_Jp le 04/08/2022, en attendant de comprendre pourquoi "ResetSoft" pose problème !
	I2cCmLmiInitFrameGetSerialNumbr = I2cCmLmiInitFrameReadElecSignat, // Pour avoir le SerialNumber, il faut demander les infos "Electronic Signature"
} eI2cCmLmiInitFrame;

/******************************************************************************/

typedef struct _I2CCM_Pres_LMI_IntData
{
	I2CCM_InternalBaseData base;	// Base requise pour I2C_ComMaster
	uint16_t ErrorsCt;
	uint16_t ScaleFactor;
	uint8_t* tmpPtr;

	// Variables pour la Moyenne Glissante rapide de Pression :
#ifdef LMI_MOY_P_FROM_RAW
	I2CCM_ADD_MOY_MANAGER_INT16(PresRaw, LMI_MOY_P_FROM_RAW); // Gestionnaire & Datas pour moyenner la Pression en RAW (Int16)
#endif // LMI_MOY_P_FROM_RAW
#ifdef LMI_MOY_P_FROM_CONV
	I2CCM_ADD_MOY_MANAGER_FLOAT(PresConv, LMI_MOY_P_FROM_CONV); // Gestionnaire & Datas pour moyenner la Pression en Pa (Float)
#endif // LMI_MOY_P_FROM_CONV

	// Buffer de Réception & Envoi privatif, si demandé :
#ifdef LMI_RX_TX_BUF_PER_DEVICE	// S'il faut réserver 1 Buffer de Rx & Tx spécifique par LMI :
	#if defined(LMI_MAX_TX_SIZE) && (LMI_MAX_TX_SIZE > 0)
		uint8_t TxBuf[LMI_MAX_TX_SIZE];
	#endif // LMI_MAX_TX_SIZE
	uint8_t RxBuf[LMI_MAX_RX_SIZE];
#endif // LMI_RX_TX_BUF_PER_DEVICE

} I2CCM_Pres_LMI_IntData;	// Internal LMI Datas Struct

/******************************************************************************/
// Prototypes locaux :

static int16_t i2cCM_PressureLMI_Init_AdrX(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);
static uint16_t getI2cCM_PressureLMI_NextActionFrame(I2CCM_Device *pDevice);
static uint16_t getI2cCM_PressureLMI_NextRqInitDataActionFrame(I2CCM_Device *pDevice);

static uint16_t handleI2cCM_PressureLMI_ActionComplete(I2CCM_Device *pDevice);

/******************************************************************************/
// Variables Internes :

I2CCM_Pres_LMI_IntData mI2CCM_Pres_LMI_IntData[I2CCM_NB_MAX_DEV_PRESS_LMI];	// MAX_NB_I2C_DEV_PRESS_LMI est configuré dans "I2cComMasterConf.h"

#ifndef LMI_RX_TX_BUF_PER_DEVICE	// S'il faut réserver 1 seul Buffer de Rx & Tx commun pour tous les LMI :
	#if defined(LMI_MAX_TX_SIZE) && (LMI_MAX_TX_SIZE > 0)
		uint8_t mI2CCM_Pressure_LMI_TxBuf[LMI_MAX_TX_SIZE];
	#endif // LMI_MAX_TX_SIZE
	uint8_t mI2CCM_Pressure_LMI_RxBuf[LMI_MAX_RX_SIZE];
#endif // LMI_RX_TX_BUF_PER_DEVICE

#ifdef I2CCM_ENABLE_I2C_IBUS_DEBUG	// cf. "I2cComMasterConf.h"
	uint8_t mI2CCM_Pres_LMI_LoadCt[I2CCM_NB_MAX_DEV_PRESS_LMI];
#endif //I2CCM_ENABLE_I2C_IBUS_DEBUG

/******************************************************************************/
// Point d'entrée Public pour l'Initialisation de tous les Capteurs de Pression LMI :

int16_t i2cCM_PressureLMI_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pInitParams) {
		return 0;	// Can't check Params !
	}
	if(pInitParams->intVarId >= I2CCM_NB_MAX_DEV_PRESS_LMI){
		return 0;	// Can't assign InternalData for this VarId !
	}
	if(0 == pInitParams->pExtStruct){
		return 0;	// Can't assign NullPtr for External Var Struct !
	}

	int16_t DeviceType = pInitParams->device2Load;
	if(DeviceType <= I2CCM_LOAD_DEFAULT_DEVICE)
	{
		switch(DeviceType)
		{
		case I2CCM_LOAD_DEFAULT_DEVICE: 	// Première Recommandation par défaut :
			DeviceType = I2cDevPresType_LMI_Adr00; // LMI en Adresse "00"
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE2:	// Seconde Recommandation par défaut :
			DeviceType = I2cDevPresType_LMI_Adr01; // LMI en Adresse "01"
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE3:	// Troisième Recommandation par défaut :
			DeviceType = I2cDevPresType_LMI_Adr10; // LMI en Adresse "10"
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE4:	// Quatrième Recommandation par défaut :
			DeviceType = I2cDevPresType_LMI_Adr11; // LMI en Adresse "11"
			break;
		// Insérer ici pour ajouter une recommandation d'ordre 5 ou 6 ...
		default: // Recommandation non gérée :
			return 0;	// DeviceType not implemented !
			break;
		}
	}

	// Initialise selon le Type correspondant à la demande :
	switch(DeviceType)
	{
	case I2cDevPresType_LMI_Unknown: // Retourne le nb de Types supportés :
		return I2cDevPresType_LMI_OutOfRange -1; // = Last -1
		break;
	case I2cDevPresType_LMI_Adr00:
	case I2cDevPresType_LMI_Adr01:	// La même Fonction d'Init permet de faire les 4 !
	case I2cDevPresType_LMI_Adr10:
	case I2cDevPresType_LMI_Adr11:
		return i2cCM_PressureLMI_Init_AdrX(DeviceType, pDevice, pInitParams);
		break;
//	case I2cDevPresType_LIM_AdrY:
//		return i2cCM_PressureLMI_Init_AdrY(DeviceType, pDevice, pInitParams);
//		break;
	default:
		break;
	}
	return 0;   // DeviceType not implemented !
}

/******************************************************************************/

static int16_t i2cCM_PressureLMI_Init_AdrX(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pDevice){
		return 0;		// Can't load Device !
	}
	if(0 == pInitParams){
		return 0;	// Can't check Params !
	}
//	if((I2cDevPresType_LMI_Adr00 == DeviceType)	// Type n°1 :     // for information only: LMI sensor does not have any sub-models
//	 ||(I2cDevPresType_LMI_Adr01 == DeviceType)) // Type n°2 :
//	{
		// Renseigne les Références :
		pDevice->DevAddr8	= I2CCM_MAKE_ADR8_WITH_RW_MASK(LMI_BASE_ADR_CPT + DeviceType - I2cDevPresType_LMI_Adr00);
		pDevice->ActionId	= I2CCM_ACTION_INIT;
		pDevice->FrameId	= 0;   // Only 1 Frame => not used !

		// Renseigne les Fonctions de CallBack :
		pDevice->getNextActionFrame	  = getI2cCM_PressureLMI_NextActionFrame;
		pDevice->handleActionComplete = handleI2cCM_PressureLMI_ActionComplete;

		// Initialise les Pointeurs :
		pDevice->pIntData = &mI2CCM_Pres_LMI_IntData[pInitParams->intVarId]; // Internal Data Struct
		pDevice->pExtData = pInitParams->pExtStruct; // External Data Struct
		pDevice->pTxBuf =
#if defined(LMI_MAX_TX_SIZE) && (LMI_MAX_TX_SIZE > 0)
	#ifdef LMI_RX_TX_BUF_PER_DEVICE	// Si on a réservé 1 Buffer de Rx & Tx par LMI :
							mI2CCM_Pres_LMI_IntData[pInitParams->intVarId].TxBuf;
	#else // ! LMI_RX_TX_BUF_PER_DEVICE
							mI2CCM_Pressure_LMI_TxBuf;	// notre Buffer d'Envoi
	#endif // LMI_RX_TX_BUF_PER_DEVICE
#else // ! LMI_MAX_TX_SIZE
							0; // No Transmit
#endif // LMI_MAX_TX_SIZE
#ifdef LMI_RX_TX_BUF_PER_DEVICE	// Si on a réservé 1 Buffer de Rx & Tx par LMI :
		pDevice->pRxBuf = mI2CCM_Pres_LMI_IntData[pInitParams->intVarId].RxBuf;
#else // ! SHT4x_RX_TX_BUF_PER_DEVICE
		pDevice->pRxBuf = mI2CCM_Pressure_LMI_RxBuf;	// notre Buffer de Réception
#endif // SHT4x_RX_TX_BUF_PER_DEVICE

		// Initialise les Structures & Buffers :
		I2CCM_FillMemory(pDevice->pIntData, 0, sizeof(I2CCM_Pres_LMI_IntData));
		I2CCM_FillMemory(pDevice->pExtData, 0, sizeof(I2CCM_Pres_LMI_ExtData));

#ifndef LMI_RX_TX_BUF_PER_DEVICE	// Si le Buffer de Rx & Tx est commun pour tous les LMI :
	#if defined(LMI_MAX_TX_SIZE) && (LMI_MAX_TX_SIZE > 0)
		I2CCM_FillMemory(pDevice->pTxBuf, 0, LMI_MAX_TX_SIZE);
	#endif // LMI_MAX_TX_SIZE
		I2CCM_FillMemory(pDevice->pRxBuf, 0, LMI_MAX_RX_SIZE);
#endif // LMI_RX_TX_BUF_PER_DEVICE

		// Initialise les Internal Valeurs Spécifiques :
		I2CCM_MAKE_INT_DATA_PTR_AND_FILL_BASE(I2CCM_Pres_LMI_IntData, pIntData, DeviceType);

		pIntData->ScaleFactor	= LMI_DEFAUT_SCALE_FACTOR_P; // 24 = ScaleFactor sur LMI_1250Pa

#ifdef LMI_MOY_P_FROM_RAW
		pIntData->PresRaw.maxCt = getI2CCM_LMI_MaxRawPresCt4Moy(pInitParams);
#endif // LMI_MOY_P_FROM_RAW

#ifdef LMI_MOY_P_FROM_CONV
		pIntData->PresConv.maxCt = getI2CCM_LMI_MaxConvPresCt4Moy(pInitParams);
#endif // LMI_MOY_P_FROM_CONV

		// Initialise les External Valeurs Spécifiques :
		I2CCM_MAKE_EXT_DATA_PTR_AND_FILL_BASE(I2CCM_Pres_LMI_ExtData, pExtData, DeviceType | pInitParams->devHandler);

		pExtData->Pressure		= LMI_UNKNOWN_PRESSURE_PA;
		pExtData->Temperature	= LMI_UNKNOWN_TEMPERATURE_DEG_C;

#ifdef I2CCM_ENABLE_I2C_IBUS_DEBUG
		mI2CCM_Pres_LMI_LoadCt[pInitParams->intVarId]++;
#endif //I2CCM_ENABLE_I2C_IBUS_DEBUG

//	}
	//---------------------------------------------------
//	else DeviceType = 0;	// DeviceType not implemented !
	return DeviceType;		// return loaded DeviceType
}

/******************************************************************************/

static uint16_t getI2cCM_PressureLMI_NextActionFrame(I2CCM_Device *pDevice)
{
	if(0 == pDevice)	return I2C_ABORT_DEV;	// Impossible de continuer sur ce Device !
	switch(pDevice->ActionId)
	{
	case I2CCM_ACTION_INIT:   // Initialisations à effectuer :
		pDevice->FrameId  = I2cCmLmiInitFrameFirst;
		pDevice->ActionId = I2CCM_RQ_INIT_DATA;
//		pDevice->DelayInMs = 0;	// Pour effectuer l'opération suivante tout de suite
//		return I2C_WAIT_TEMPO;	// Pseudo tempo + Action immédiate
//		noBreak;
	//---------------------
	case I2CCM_RQ_INIT_DATA:
		return getI2cCM_PressureLMI_NextRqInitDataActionFrame(pDevice);
		break;
	//---------------------
	case I2CCM_QUERY_VALUES:  // Comment Récupérer les Valeurs ?
		pDevice->nbBytes2Read = LMI_STD_RX_SIZE; 			// 2 ou 4 Bytes to Receive : Pression + T°
		pDevice->DelayInMs	  = LMI_TEMPO_REPLY_TIMEOUT; 	// 100ms de TimeOut pour répondre
		return I2C_RECEIVE | I2C_HANDLE_RX; 				// Demande Lecture + CallBack de Réception
		break;
	//---------------------
	default:    // Not Handled correctly :
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

static uint16_t getI2cCM_PressureLMI_NextRqInitDataActionFrame(I2CCM_Device *pDevice)
{
	pDevice->DelayInMs	= LMI_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
	I2CCM_Pres_LMI_ExtData* pExtData = (I2CCM_Pres_LMI_ExtData*)pDevice->pExtData; // Pointer sur les Datas Externes

	switch(pDevice->FrameId)
	{
	case I2cCmLmiInitFrameResetSoft:	// Envoyer un Soft Reset :
		pDevice->pTxBuf[0]	= LMI_CMD_DO_RESET_FIRMWARE;
		pDevice->nbBytes2Send = 1;
		return I2C_TRANSMIT | I2C_HANDLE_TX;
	//--------------------------------
	case I2cCmLmiInitFrameReadElecSignat:
		pDevice->pTxBuf[0]	= LMI_CMD_READ_ELEC_SIGNAT;
		pDevice->nbBytes2Send = 1;
		if(pDevice->pRxBuf != (uint8_t*)&pExtData->ElectronicSignature)
		{
			I2CCM_Pres_LMI_IntData* pIntData = (I2CCM_Pres_LMI_IntData*)pDevice->pIntData;
			pIntData->tmpPtr = pDevice->pRxBuf; // Sauvegarde le Pointeur "normal"
			pDevice->pRxBuf = (uint8_t*)&pExtData->ElectronicSignature;
		}
		pDevice->nbBytes2Read = sizeof(I2CCM_Pres_LMI_ElectronicSignature); // Demander toute la Structure d'1 coup
		return I2C_TRANSMIT | I2C_RECEIVE | I2C_HANDLE_RX; // Demande Envoi + Lecture + CallBack de Réception
	case I2cCmLmiInitFrameStartPressConv:
		pDevice->pTxBuf[0] = LMI_CMD_START_PRESS_CONV;
		pDevice->nbBytes2Send = 1;
		return I2C_TRANSMIT | I2C_HANDLE_TX;
	//--------------------------------
// Ajouter ci-dessus la préparation d'autres éventuelles Requêtes d' "InitTime" ...
	//--------------------------------
	case I2cCmLmiInitFrameLast:
		pDevice->ActionId	= I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		pDevice->DelayInMs	= 0;	// Pour effectuer l'opération suivante tout de suite
//		pExtData->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);	// Signale que la Procédure d'Init est Terminée
		I2CCM_EXT_DATA_PTR_EXIT_INIT(pExtData);	// Signaler que la Procédure d'Init est Terminée
		return I2C_WAIT_TEMPO;	// Pseudo tempo + Action immédiate
	default:
		// Ne devrait jamais se produire !
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

static uint16_t handleI2cCM_PressureLMI_ActionComplete(I2CCM_Device *pDevice)
{
	if(0 == pDevice)    return I2C_END_BLOC;
	I2CCM_Pres_LMI_IntData* pIntData = (I2CCM_Pres_LMI_IntData*)pDevice->pIntData;  // Pointer sur les Datas Internes
	I2CCM_Pres_LMI_ExtData* pExtData = (I2CCM_Pres_LMI_ExtData*)pDevice->pExtData;  // Pointer sur les Datas Externes
	uint8_t *pRxBuf 				 = (uint8_t *)pDevice->pRxBuf; 					// Pour pointer le Buffer de Réception

	switch(pDevice->ActionId)
	{
	case I2CCM_QUERY_VALUES:
	{
		if(0 == pDevice->result) // No I2C error :
		{
				float NewPressure;
				// OK, pas d'ereur de réception :
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt,	I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,		I2CCM_MAX_ERROR_CT_ON_PERIF_OK)

				// Récupération de la Valeur RAW de Pression :
				uint16_t valeurBridgeBrut = I2CCM_MAKE_UINT16_FROM_LIT_ENDIAN(pRxBuf[0], pRxBuf[1]);
#ifdef LMI_GET_VALUE_BRUT
				pExtData->ValueBrut = valeurBridgeBrut;
#endif // LMI_GET_VALUE_BRUT

				// Récupération & Conversion de la Température à partir du RAW de T°, si demandée :
#if (LMI_STD_RX_SIZE >= 4)
				int16_t tmpTempBrut   = I2CCM_MAKE_UINT16_FROM_LIT_ENDIAN(pRxBuf[2], pRxBuf[3]);
				pExtData->Temperature = (float)LMI_REF_TEMPERATURE + (((float)tmpTempBrut - (float)LMI_VALUE_AT_REF_TEMP)/(float)LMI_DEFAUT_SCALE_FACTOR_T);
	#ifdef LMI_GET_TEMP_BRUT
				pExtData->TemperatureBrut = tmpTempBrut;
	#endif // LMI_GET_TEMP_BRUT
#endif // LMI_STD_RX_SIZE

				// Calcul d'une Moyenne glissante sur la base des "Echantillons RAW" de Pression du Capteur :
				float tmpBridgeValue;
#if defined(LMI_MOY_P_FROM_RAW) && (LMI_MOY_P_FROM_RAW > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				tmpBridgeValue =  i2cComMaster_AddInt16ToMoyManager(&pIntData->PresRaw, valeurBridgeBrut, LMI_MOY_P_FROM_RAW);
#else // ! LMI_MOY_P_FROM_RAW
				tmpBridgeValue = (float)valeurBridgeBrut;
#endif // LMI_MOY_P_FROM_RAW
#ifdef LMI_GET_VALUE_MOY
				pExtData->ValueMoy = tmpBridgeValue;
#endif // LMI_GET_VALUE_MOY

				// Calcul du RAW moyenné -> Pascals :
				if(0 == pIntData->ScaleFactor) {
					pIntData->ScaleFactor = LMI_DEFAUT_SCALE_FACTOR_P; // Protection en cas de perte de Coefficient
				}
/*	Exemple de Filtrage sur le Type de Device (partie 1) :
				switch(pIntData->base.idOfDevice)
				{
				case I2cDevPresType_LMI_Adr00:
				case I2cDevPresType_LMI_Adr01:
				case I2cDevPresType_LMI_Adr10:
				case I2cDevPresType_LMI_Adr11:
*/
				NewPressure = tmpBridgeValue/((float)pIntData->ScaleFactor); // Division par le ScaleFactor
/*	Exemple de Filtrage sur le Type de Device (partie 2) :
					break;
				default:
					NewPressure = 0;
					break;
				}
*/

#ifdef LMI_GET_PRESS_BRUT
				pExtData->PresBrut = NewPressure;
#endif // LMI_GET_PRESS_BRUT

				// Calcul d'une Moyenne glissante sur la base des Echantillons déjà convertis en Pression :
#if defined(LMI_MOY_P_FROM_CONV) && (LMI_MOY_P_FROM_CONV > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				pExtData->Pressure = i2cComMaster_AddFloatToMoyManager(&pIntData->PresConv, NewPressure, LMI_MOY_P_FROM_CONV);
#else // ! LMI_MOY_P_FROM_CONV
				pExtData->Pressure = NewPressure;
#endif // LMI_MOY_P_FROM_CONV

				// Délai de prochaine action :
				pDevice->DelayInMs  = LMI_TEMPO_BEFORE_NEXT_ACTION; // 1s avant prochaine Lecture

				// Signaler à la tâche qu'il y a 1 nouvelle Data à récupérer :
				pExtData->newFlags |= I2cCmPressureLmiNewPres; // Signale la nouvelle Data
				return I2C_NEW_VALUE | I2C_END_BLOC; // Signaler puis rendre la main
		}
	}
	break;
	//---------------------
	case I2CCM_RQ_INIT_DATA:
		if(0 == pDevice->result) // No I2C error :
		{
			switch(pDevice->FrameId)
			{
			case I2cCmLmiInitFrameResetSoft:	// Soft Reset Done :
			case I2cCmLmiInitFrameStartPressConv: // Mise en place des Mesures Auto = Done :
				pDevice->FrameId++;		// OK pour passer à la suite
				pDevice->DelayInMs = LMI_TEMPO_BEFORE_FIRST_ACTION; // Tempo de récupération après ResetSoft
				return I2C_END_BLOC;	// Libère la ressource pour le moment
			//-----------------------------
			case I2cCmLmiInitFrameReadElecSignat:
				// PartNumber = "SQ444-21007", LotNumber = "CB7L001", CalibrationID = "AE", PressureRange = 1200, Type = 'B' (Bidirectional), ScaleFactor = 25, SequenceNumber = 0x886 sur Manta01M6-006
				pIntData->ScaleFactor  = (uint16_t)I2CCM_JOIN_8_8_BE( pExtData->ElectronicSignature.ScaleFactorBE[0],
																	  pExtData->ElectronicSignature.ScaleFactorBE[1]);
#ifdef LMI_DECODE_PRESS_RANGE
				pExtData->PressureRange = (uint16_t)I2CCM_JOIN_8_8_BE(pExtData->ElectronicSignature.PresRangeBE[0],
																	  pExtData->ElectronicSignature.PresRangeBE[1]);
#endif // LMI_DECODE_PRESS_RANGE
#ifdef LMI_DECODE_SCALE_FACTOR
				pExtData->ScaleFactor = pIntData->ScaleFactor; // On vient de le calculer déjà pour nous en Interne
#endif // LMI_DECODE_SCALE_FACTOR
#ifdef LMI_DECODE_SEQ_NUMBER
				pExtData->SequenceNumber = (uint16_t)I2CCM_JOIN_8_8_BE( pExtData->ElectronicSignature.SequenceNumBE[0],
																		pExtData->ElectronicSignature.SequenceNumBE[1]);
#endif // LMI_DECODE_SEQ_NUMBER

				pDevice->pRxBuf = pIntData->tmpPtr;	// Restaure le Pointeur de Réception "officiel"
				pExtData->newFlags |= I2cCmPressureLmiNewSignat;
				pDevice->FrameId++;		// OK pour passer à la suite
				pDevice->DelayInMs = 0;	// Pour effectuer l'opération suivante tout de suite dès le prochain tour
				return I2C_END_BLOC;	// Libère la ressource pour le moment
				break;
			//-----------------------------
// Ajouter ici le Traitement d'autres éventuelles Requêtes d' "InitTime" ...
			//-----------------------------
			default:
				break;
			}
		} else {
			// L'erreur va incrémenter le compteur général ci-dessous ...
		}
		break;
	//---------------------
	default:    // Not Handled correctly :
		break;
	}

	// Incrémenter les compteurs d'erreur :
	I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt, UINT16_MAX)
	I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,  UINT16_MAX)

	if(pIntData->ErrorsCt > LMI_PRESS_MAX_ALLOWED_ERRORS)
	{
		pDevice->ActionId = I2CCM_ACTION_INIT;  // Demander à Ré-Initialiser
		return I2C_ABORT_DEV; 					// +Recommander d'Abandonner l'usage de ce Device
	}
	pDevice->DelayInMs = LMI_TEMPO_RETRY_ACTION; // Tempo avant Retry : 1s
	return I2C_END_BLOC | I2C_CHK_DEV_ERR; // Rendre la main + Demander à Vérifier si erreur sur le Device ...
}

/******************************************************************************/

#ifdef LMI_MOY_P_FROM_RAW
	__attribute__((weak)) I2CCM_BUILD_GET_MAX_CT4MOY(getI2CCM_LMI_MaxRawPresCt4Moy, LMI_MOY_P_FROM_RAW)
#endif // LMI_MOY_P_FROM_RAW

/******************************************************************************/

#ifdef LMI_MOY_P_FROM_CONV
	__attribute__((weak)) I2CCM_BUILD_GET_MAX_CT4MOY(getI2CCM_LMI_MaxConvPresCt4Moy, LMI_MOY_P_FROM_CONV)
#endif // LMI_MOY_P_FROM_CONV

#ifdef __cplusplus
}
#endif
