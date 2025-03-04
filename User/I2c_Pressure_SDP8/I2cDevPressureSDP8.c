/*
 * I2cDevPressureSDP8.c
 *
 *  Created on: 12 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 03 Nov. 2022
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement ce Module "I2c_Pressure_SDP8" à la Librairie "I2cComMaster" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "I2cDevPressureSDP8.h"
 *
 *  Ce module est (théoriquement) compatible avec les références suivantes :
 *  (From "Sensirion_Differential_Pressure_Datasheet_SDP8xx_Digital.pdf" v1.1 of "April 2019")
 *-> SDP800-500Pa
 *-> SDP801-500Pa
 *-> SDP810-500Pa
 *-> SDP811-500Pa
 *-> SDP800-125Pa
 *-> SDP810-125Pa
 *
 */

#include "I2cDevPressureSDP8.h"		// Pour accès à nos propres déclarations publiques

#include "I2cComMasterConf.h"		// Pour accès à la Configuration User souhaitée
#include "I2cComMasterUtils.h"  	// Pour accès aux Outils de support I2cComMaster

#include "CrcSoft.h"				// Pour calcul du CRC8 de la Trame SDP8x

#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************/

// Prototypes des Fonctions de Configuration Externes (elles sont appelées en cas de besoin) :
// -> En cas d'absence, une fonction Interne équivalente par défaut est appelée à la place.

#ifdef SDP8_MOY_P_FROM_RAW
	uint16_t getI2CCM_SDP8_MaxConvPresCt4Moy(I2CCM_DevInitParams* pInitParams);
#endif // SDP8_MOY_P_FROM_RAW

#ifdef SDP8_MOY_P_FROM_CONV
	uint16_t getI2CCM_SDP8_MaxRawPresCt4Moy(I2CCM_DevInitParams* pInitParams);
#endif // SDP8_MOY_P_FROM_CONV

/******************************************************************************/

// Temporisations :
#define SDP8_TEMPO_BEFORE_FIRST_ACTION	100		// 100ms avant 1° Lecture (Power-up time = 25ms max)
#define SDP8_TEMPO_REPLY_TIMEOUT		100		// 100ms d'attente max pour répondre
#define SDP8_TEMPO_RETRY_ACTION			1000	// 1s avant Retry
#define SDP8_TEMPO_BEFORE_NEXT_ACTION	1 *1000	// 1s avant de redemander une nouvelle Lecture

/******************************************************************************/

// Capteur de Pression Sensirion SDP8xx :
#define SDP_8x0_ADR_CPT 	0x25// Adresse I2C SDP8x0 = 0x25 sur 7bits
#define SDP_8x1_ADR_CPT 	0x26// Adresse I2C SDP8x1 = 0x26 sur 7bits
#define SDP_8xx_CRC_INIT	0xFF// CRC init = 0xFF sur SDP8xx

#define SDP_8x_TX_CMD_SIZE	2	// Commande SDP8 sur 16bits
#define SDP_8x_DEF_TX_SIZE	2	// Envoi de 2 Bytes
#define SDP_8x_MAX_TX_SIZE	2	// Envoi de 2 Bytes max

#define SDP_8x_RX_BLOC_SIZE		(2 +1)	// Ajouter 1 byte de CRC pour chaque groupe de 2 Bytes de Data
#define SDP_8x_DEF_RX_SIZE		SDP_8x_RX_BLOC_SIZE	// Réception de 3 Bytes
#define SDP_8x_STD_BLOCS_2READ	SDP8_NB_BLOCS_2READ	// Chaque Data fait la taille d'1 bloc de 16bits
#define SDP_8x_STD_RX_SIZE		(SDP_8x_STD_BLOCS_2READ * SDP_8x_RX_BLOC_SIZE)

#define SDP_8x_SCALFTR_NB_BLOCS	3	// Lire le ScaleFactor implique de lire également les 2 champs précédents
#define SDP_8x_SCALFTR_RX_SIZE	(SDP_8x_SCALFTR_NB_BLOCS * SDP_8x_RX_BLOC_SIZE)

#ifdef SDP8_GET_SERIAL_NUMBER
	#define SDP_8x_SN_BLOCS_2READ	(2+4) // PID sur 2*16bits + SN sur 4*16bits
#else // SDP8_GET_SERIAL_NUMBER
	#define SDP_8x_SN_BLOCS_2READ	0	// Si on n'a pas besoin de récupérer !
#endif // SDP8_GET_SERIAL_NUMBER
#define SDP_8x_SN_SIZE_2READ	(SDP_8x_SN_BLOCS_2READ * SDP_8x_RX_BLOC_SIZE)
#define SDP_8x_MAX_RX_SIZE		MAX4(SDP_8x_DEF_RX_SIZE, SDP_8x_STD_RX_SIZE, SDP_8x_SCALFTR_RX_SIZE, SDP_8x_SN_SIZE_2READ) // Taille Max suivant synthese besoins

// Commandes I2C pour SDP-8xx (from "Sensirion_Differential_Pressure_Datasheet_SDP8xx_Digital.pdf") :
#define SDP8_CMD_TRIG_PRESSURE		0x372D	// Triggered Differential Pressure Measurement, with Clock stretching
//#define SDP8_CMD_TRIG_PRESSURE  	0x362F	// Triggered Differential Pressure Measurement, without Clock stretching

#define SDP8_CMD_DO_SOFT_RESET		0x0006	// General call reset : I2C address + W bit + command code (Hex) = 0x0006
// From "6.3.4 Soft Reset" : Note that the I2C address is 0x00, which is the general call address, and that the command is 8 bit
// -> Remarque_Jp le 29/10/2021 : Envoyer cette commande sur la "General Call Address 0" ré-initialise également tous les autres Devices I2C :-( !

#define SDP8_CMD_READ_PRODUCT_ID32	0x367C	// Il faut envoyer les 2 commandes PID & SN l'une juste après l'autre
#define SDP8_CMD_READ_SERIAL_NUM64	0xE102	// pour pouvoir récupérer le UINT32 & UINT64 à la fois !

/******************************************************************************/
/* Scale Factors, from "Sensirion_Differential_Pressure_Datasheet_SDP8xx_Digital.pdf" p10 #6.51 :
SDP8xx-500Pa	: 60 Pa-1	; 200 °C-1
SDP8xx-125Pa	: 240 Pa-1	; 200 °C-1
*/
#define SDP8_DEFAUT_SCALE_FACTOR_P  	60	//  60 pour SDP8xx-500Pa
//#define SDP8_DEFAUT_SCALE_FACTOR_P  	240	// 240 pour SDP8xx-125Pa

#define SDP8_DEFAUT_SCALE_FACTOR_T  	200	// 200 pour SDP8xx-500Pa
//#define SDP8_DEFAUT_SCALE_FACTOR_T  	200	// 200 pour SDP8xx-125Pa

/******************************************************************************/
// Opérations à effectuer à l'Init :
typedef enum
{
	I2cCmSdp8InitFrameResetSoft, 		// Demander un Reset Soft
	//---------------------------
	// Pour récupérer le ProductId & SerialNumber :
#ifdef SDP8_GET_SERIAL_NUMBER
	I2cCmSdp8InitFrameReadPID32,		// Demander le ProductIdentifier 32bits
	I2cCmSdp8InitFrameReadSN64,			// Demander le SerialNumber 64bits
#endif // SDP8_GET_SERIAL_NUMBER
	//---------------------------
	// Pour récupérer le ScaleFactor :
	I2cCmSdp8InitFrameReadScaleFactor,	// Faire une 1° Lecture complète pour obtenir le ScaleFactor
	//-------------------------
// Ajouter ci-dessus la Référence à d'autres éventuelles Requêtes @ "InitTime" ...
	//-------------------------
	I2cCmSdp8InitFrameLast,				// Fin des Requêtes à faire à l'Init
	I2cCmSdp8InitFrameFirst = I2cCmSdp8InitFrameResetSoft,
//	I2cCmSdp8InitFrameGetSerialNumbr = I2cCmSdp8InitFrameReadPID32, // Pour avoir le SerialNumber, il faut commencencer par demander le PID32
} eI2cCmSdp8InitFrame;

/******************************************************************************/

typedef struct _I2CCM_Pres_SDP8_IntData
{
	I2CCM_InternalBaseData base;	// Base requise pour I2C_ComMaster
	uint16_t ErrorsCt;
	uint16_t ScaleFactor;

	// Variables pour la Moyenne Glissante rapide de Pression :
#ifdef SDP8_MOY_P_FROM_RAW
	I2CCM_ADD_MOY_MANAGER_INT16(PresRaw, SDP8_MOY_P_FROM_RAW); // Gestionnaire & Datas pour moyenner la Pression en RAW (Int16)
#endif // SDP8_MOY_P_FROM_RAW
#ifdef SDP8_MOY_P_FROM_CONV
	I2CCM_ADD_MOY_MANAGER_FLOAT(PresConv, SDP8_MOY_P_FROM_CONV); // Gestionnaire & Datas pour moyenner la Pression en Pa (Float)
#endif // SDP8_MOY_P_FROM_CONV

	// Buffer de Réception & Envoi privatif, si demandé :
#ifdef SDP8_RX_TX_BUF_PER_DEVICE	// S'il faut réserver 1 Buffer de Rx & Tx spécifique par SDP8 :
	#if defined(SDP_8x_MAX_TX_SIZE) && (SDP_8x_MAX_TX_SIZE > 0)
		uint8_t TxBuf[SDP_8x_MAX_TX_SIZE];
	#endif // SDP_8x_MAX_TX_SIZE
	uint8_t RxBuf[SDP_8x_MAX_RX_SIZE];
#endif // SDP8_RX_TX_BUF_PER_DEVICE

} I2CCM_Pres_SDP8_IntData;	// Internal SDP8xx Datas Struct

/******************************************************************************/
// Prototypes locaux :

static int16_t i2cCM_PressureSDP8_Init_8xx(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams);
static uint16_t getI2cCM_PressureSDP8_NextActionFrame(I2CCM_Device *pDevice);
static uint16_t getI2cCM_PressureSDP8_NextRqInitDataActionFrame(I2CCM_Device *pDevice);

static uint16_t handleI2cCM_PressureSDP8_ActionComplete(I2CCM_Device *pDevice);

/******************************************************************************/
// Variables Internes :

I2CCM_Pres_SDP8_IntData mI2CCM_Pres_SDP8_IntData[I2CCM_NB_MAX_DEV_PRESS_SDP8];	// MAX_NB_I2C_DEV_PRESS_SDP est configuré dans "I2cComMasterConf.h"

#ifndef SDP8_RX_TX_BUF_PER_DEVICE	// S'il faut réserver 1 seul Buffer de Rx & Tx commun pour tous les SDP8 :
	#if defined(SDP_8x_MAX_TX_SIZE) && (SDP_8x_MAX_TX_SIZE > 0)
		uint8_t mI2CCM_Pressure_SDP8_TxBuf[SDP_8x_MAX_TX_SIZE];
	#endif // SDP_8x_MAX_TX_SIZE
	uint8_t mI2CCM_Pressure_SDP8_RxBuf[SDP_8x_MAX_RX_SIZE];
#endif // SDP8_RX_TX_BUF_PER_DEVICE

#ifdef I2CCM_ENABLE_I2C_IBUS_DEBUG	// cf. "I2cComMasterConf.h"
	uint8_t mI2CCM_Pres_SDP8_LoadCt[I2CCM_NB_MAX_DEV_PRESS_SDP8];
#endif //I2CCM_ENABLE_I2C_IBUS_DEBUG

/******************************************************************************/
// Point d'entrée Public pour l'Initialisation de tous les Capteurs de Pression SDP_8xx :

int16_t i2cCM_PressureSDP8_Init(I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pInitParams) return 0;	// Can't check Params !
	if(pInitParams->intVarId >= I2CCM_NB_MAX_DEV_PRESS_SDP8) return 0;	// Can't assign InternalData for this VarId !
	if(0 == pInitParams->pExtStruct) return 0;	// Can't assign NullPtr for External Var Struct !

	int16_t DeviceType = pInitParams->device2Load;
	if(DeviceType <= I2CCM_LOAD_DEFAULT_DEVICE)
	{
		switch(DeviceType)
		{
		case I2CCM_LOAD_DEFAULT_DEVICE:	// Première Recommandation par défaut :
			DeviceType = I2cDevPresType_SDP8_8x0; // SDP Type 8x0
			break;
		case I2CCM_LOAD_DEFAULT_DEVICE2:	// Seconde Recommandation par défaut :
			DeviceType = I2cDevPresType_SDP8_8x1; // SDP Type 8x1
			break;
		// Insérer ici pour ajouter une recommandation d'ordre 3 ou 4 ...
		default: // Recommandation non gérée :
			return 0;	// DeviceType not implemented !
			break;
		}
	}

	// Initialise selon le Type correspondant à la demande :
	switch(DeviceType)
	{
	case I2cDevPresType_SDP8_Unknown: // Retourne le nb de Types supportés :
		return I2cDevPresType_SDP8_OutOfRange -1; // = Last -1
		break;
	case I2cDevPresType_SDP8_8x0:
	case I2cDevPresType_SDP8_8x1:	// La même Fonction d'Init permet de faire les 2 !
		return i2cCM_PressureSDP8_Init_8xx(DeviceType, pDevice, pInitParams);
		break;
//	case I2cDevPresType_SDP8_8xx:
//		return i2cCM_PressureSDP6_Init_8xx(DeviceType, pDevice, pInitParams);
//		break;
	default:
		break;
	}
	return 0;   // DeviceType not implemented !
}

/******************************************************************************/

static int16_t i2cCM_PressureSDP8_Init_8xx(int16_t DeviceType, I2CCM_Device* pDevice, I2CCM_DevInitParams* pInitParams)
{
	if(0 == pDevice) return 0;		// Can't load Device !
	if(0 == pInitParams) return 0;	// Can't check Params !
	if((I2cDevPresType_SDP8_8x0 == DeviceType)	// Type n°1 :
	 ||(I2cDevPresType_SDP8_8x1 == DeviceType)) // Type n°2 :
	{
		// Renseigne les Références :
		if(I2cDevPresType_SDP8_8x0 == DeviceType) pDevice->DevAddr8 = I2CCM_MAKE_ADR8_WITH_RW_MASK(SDP_8x0_ADR_CPT);
		if(I2cDevPresType_SDP8_8x1 == DeviceType) pDevice->DevAddr8 = I2CCM_MAKE_ADR8_WITH_RW_MASK(SDP_8x1_ADR_CPT);
		pDevice->ActionId		= I2CCM_ACTION_INIT;
		pDevice->FrameId		= 0;   // Only 1 Frame => not used !

		// Renseigne les Fonctions de CallBack :
		pDevice->getNextActionFrame		= getI2cCM_PressureSDP8_NextActionFrame;
		pDevice->handleActionComplete	= handleI2cCM_PressureSDP8_ActionComplete;

		// Initialise les Pointeurs :
		pDevice->pIntData = &mI2CCM_Pres_SDP8_IntData[pInitParams->intVarId]; // Internal Data Struct
		pDevice->pExtData = pInitParams->pExtStruct; // External Data Struct
		pDevice->pTxBuf =
#if defined(SDP_8x_MAX_TX_SIZE) && (SDP_8x_MAX_TX_SIZE > 0)
	#ifdef SDP8_RX_TX_BUF_PER_DEVICE	// Si on a réservé 1 Buffer de Rx & Tx par SDP8 :
							mI2CCM_Pres_SDP8_IntData[pInitParams->intVarId].TxBuf;
	#else // ! SDP8_RX_TX_BUF_PER_DEVICE
							mI2CCM_Pressure_SDP8_TxBuf;	// notre Buffer d'Envoi
	#endif // SDP8_RX_TX_BUF_PER_DEVICE
#else // ! SDP_8x_MAX_TX_SIZE
							0; // No Transmit
#endif // SDP_8x_MAX_TX_SIZE
#ifdef SDP8_RX_TX_BUF_PER_DEVICE	// Si on a réservé 1 Buffer de Rx & Tx par SDP8 :
		pDevice->pRxBuf = mI2CCM_Pres_SDP8_IntData[pInitParams->intVarId].RxBuf;
#else // ! SDP8_RX_TX_BUF_PER_DEVICE
		pDevice->pRxBuf = mI2CCM_Pressure_SDP8_RxBuf;	// notre Buffer de Réception
#endif // SDP8_RX_TX_BUF_PER_DEVICE

		// Initialise les Structures & Buffers :
		I2CCM_FillMemory(pDevice->pIntData, 0, sizeof(I2CCM_Pres_SDP8_IntData));
		I2CCM_FillMemory(pDevice->pExtData, 0, sizeof(I2CCM_Pres_SDP8_ExtData));

#ifndef SDP8_RX_TX_BUF_PER_DEVICE	// Si le Buffer de Rx & Tx est commun pour tous les SDP8 :
	#if defined(SDP_8x_MAX_TX_SIZE) && (SDP_8x_MAX_TX_SIZE > 0)
		I2CCM_FillMemory(pDevice->pTxBuf, 0, SDP_8x_MAX_TX_SIZE);
	#endif // SDP_8x_MAX_TX_SIZE
		I2CCM_FillMemory(pDevice->pRxBuf, 0, SDP_8x_MAX_RX_SIZE);
#endif // SDP8_RX_TX_BUF_PER_DEVICE

		// Initialise les Internal Valeurs Spécifiques :
		I2CCM_MAKE_INT_DATA_PTR_AND_FILL_BASE(I2CCM_Pres_SDP8_IntData, pIntData, DeviceType);

		pIntData->ScaleFactor	= SDP8_DEFAUT_SCALE_FACTOR_P; // 60 = ScaleFactor sur SDP8xx-500Pa

		// Initialise le Moyennage des Valeurs en RAW :
#ifdef SDP8_MOY_P_FROM_RAW
		pIntData->PresRaw.maxCt = getI2CCM_SDP8_MaxRawPresCt4Moy(pInitParams);
#endif // SDP8_MOY_P_FROM_RAW

		// Initialise le Moyennage des Valeurs Converties :
#ifdef SDP8_MOY_P_FROM_CONV
		pIntData->PresConv.maxCt = getI2CCM_SDP8_MaxConvPresCt4Moy(pInitParams);
#endif // SDP8_MOY_P_FROM_CONV

		// Initialise les External Valeurs Spécifiques :
		I2CCM_MAKE_EXT_DATA_PTR_AND_FILL_BASE(I2CCM_Pres_SDP8_ExtData, pExtData, DeviceType | pInitParams->devHandler);

		pExtData->Pressure		= SDP8_UNKNOWN_PRESSURE_PA;

#if (SDP8_NB_BLOCS_2READ >= 2)
		pExtData->Temperature	= SDP8_UNKNOWN_TEMPERATURE_DEG_C;
#endif // SDP8_NB_BLOCS_2READ >= 2

#ifdef I2CCM_ENABLE_I2C_IBUS_DEBUG
		mI2CCM_Pres_SDP8_LoadCt[pInitParams->intVarId]++;
#endif //I2CCM_ENABLE_I2C_IBUS_DEBUG

	}
	//---------------------------------------------------
	else DeviceType = 0;	// DeviceType not implemented !
	return DeviceType;		// return loaded DeviceType
}

/******************************************************************************/

static uint16_t getI2cCM_PressureSDP8_NextActionFrame(I2CCM_Device *pDevice)
{
	if(0 == pDevice)	return I2C_ABORT_DEV;	// Impossible de continuer sur ce Device !
	switch(pDevice->ActionId)
	{
	case I2CCM_ACTION_INIT:   // Initialisations à effectuer :
		pDevice->FrameId  = I2cCmSdp8InitFrameFirst;
		pDevice->ActionId = I2CCM_RQ_INIT_DATA;
//		pDevice->DelayInMs = 0;	// Pour effectuer l'opération suivante tout de suite
//		return I2C_WAIT_TEMPO;	// Pseudo tempo + Action immédiate
//		noBreak;
	//---------------------
	case I2CCM_RQ_INIT_DATA:
		return getI2cCM_PressureSDP8_NextRqInitDataActionFrame(pDevice);
		break;
	//---------------------
	case I2CCM_QUERY_VALUES:  // Récupérer les nouvelles Valeurs :
		pDevice->DelayInMs   = SDP8_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
		return i2cComMaster_BuildRequest_Cmd16BEwithReply(pDevice, SDP8_CMD_TRIG_PRESSURE, SDP_8x_STD_RX_SIZE);
	//---------------------
	default:    // Not Handled correctly :
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

static uint16_t getI2cCM_PressureSDP8_NextRqInitDataActionFrame(I2CCM_Device *pDevice)
{
	pDevice->DelayInMs	= SDP8_TEMPO_REPLY_TIMEOUT; // Par défaut : 100ms de TimeOut pour répondre
	switch(pDevice->FrameId)
	{
	case I2cCmSdp8InitFrameResetSoft:	// Envoyer un Soft Reset :
// Correction_Jp le 29/10/2021 : Envoyer cette Commande sur "0" réinitialise également tous les autres Devices I2C, ce qui n'est pas souhaité :-( !
//	{
//		I2CCM_Pres_SDP8_IntData* pIntData = (I2CCM_Pres_SDP8_IntData*)pDevice->pIntData;
//		if(pDevice->DevAddr8 != 0) pIntData->tmpU16 = pDevice->DevAddr8;	// Capture une copie de sauvegarde
//		pDevice->DevAddr8	= I2CCM_GET_BYTE_N(SDP8_CMD_DO_SOFT_RESET, 1);	// HighByte to DevAdr
		pDevice->pTxBuf[0]	= I2CCM_GET_BYTE_N(SDP8_CMD_DO_SOFT_RESET, 0);	// LowByte as 8bit Command
		pDevice->nbBytes2Send = 1;
//		pDevice->DelayInMs	= SDP8_TEMPO_REPLY_TIMEOUT; // 100ms de TimeOut pour répondre
//	}
		return I2C_TRANSMIT | I2C_HANDLE_TX;
	//--------------------------------
#ifdef SDP8_GET_SERIAL_NUMBER
	case I2cCmSdp8InitFrameReadPID32:
		return i2cComMaster_BuildRequest_Cmd16BEwithReply(pDevice, SDP8_CMD_READ_PRODUCT_ID32, 0);
	case I2cCmSdp8InitFrameReadSN64:
		return i2cComMaster_BuildRequest_Cmd16BEwithReply(pDevice, SDP8_CMD_READ_SERIAL_NUM64, SDP_8x_SN_SIZE_2READ);
#endif // SDP8_GET_SERIAL_NUMBER
	//--------------------------------
	case I2cCmSdp8InitFrameReadScaleFactor:	// Simule une Lecture pour récupérer le ScaleFactor, en fin de Trame :
		return i2cComMaster_BuildRequest_Cmd16BEwithReply(pDevice, SDP8_CMD_TRIG_PRESSURE, SDP_8x_SCALFTR_RX_SIZE);
	//--------------------------------
// Ajouter ci-dessus la préparation d'autres éventuelles Requêtes d' "InitTime" ...
	//--------------------------------
	case I2cCmSdp8InitFrameLast:
		pDevice->ActionId	= I2CCM_QUERY_VALUES; // Prochaine Action = Lire le Capteur
		pDevice->DelayInMs	= 0;	// Pour effectuer l'opération suivante tout de suite
		I2CCM_Pres_SDP8_ExtData* pExtData = (I2CCM_Pres_SDP8_ExtData*)pDevice->pExtData; // Pointer sur les Datas Externes
		pExtData->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);	// Signale que la Procédure d'Init est Terminée
		return I2C_WAIT_TEMPO;	// Pseudo tempo + Action immédiate
	default:
		// Ne devrait jamais se produire !
		break;
	}
	pDevice->ActionId = I2CCM_ACTION_INIT;
	return I2C_END_BLOC;
}

/******************************************************************************/

static uint16_t handleI2cCM_PressureSDP8_ActionComplete(I2CCM_Device *pDevice)
{
	if(0 == pDevice)    return I2C_END_BLOC;
	I2CCM_Pres_SDP8_IntData* pIntData = (I2CCM_Pres_SDP8_IntData*)pDevice->pIntData; // Pointer sur les Datas Internes
	I2CCM_Pres_SDP8_ExtData* pExtData = (I2CCM_Pres_SDP8_ExtData*)pDevice->pExtData; // Pointer sur les Datas Externes
	uint8_t *pRxBuf = (uint8_t *)pDevice->pRxBuf; // Pour pointer le Buffer de Réception

	switch(pDevice->ActionId)
	{
	case I2CCM_QUERY_VALUES:
	{
		if(0 == pDevice->result) // No I2C error :
		{
			if(isValidMultiU16BlocsCRC8(pRxBuf, SDP8_NB_BLOCS_2READ, SDP_8xx_CRC_INIT)) // Vérifier uniquement le CRC des Blocs demandés : Pression &/ T°
			{
				float NewPressure;
				// OK, pas d'ereur de réception :
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pIntData->ErrorsCt,	I2CCM_MAX_ERROR_CT_ON_PERIF_OK)
				I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(pDevice->ErrorsCt,		I2CCM_MAX_ERROR_CT_ON_PERIF_OK)

				// Récupération de la Valeur RAW de Pression :
				uint16_t valeurBridgeBrut = I2CCM_MAKE_UINT16_FROM_BIG_ENDIAN(pRxBuf[0], pRxBuf[1]); // & 0x3F
#ifdef SDP8_GET_VALUE_BRUT
				pExtData->WheatstoneBridgeBrut = valeurBridgeBrut;
#endif // SDP8_GET_VALUE_BRUT

				// Récupération & Conversion de la Température à partir du RAW de T°, si demandée :
#if SDP8_NB_BLOCS_2READ >= 2
				uint16_t tmpTempBrut = I2CCM_MAKE_UINT16_FROM_BIG_ENDIAN(pRxBuf[3], pRxBuf[4]);
				pExtData->Temperature = ((float)tmpTempBrut/(float)SDP8_DEFAUT_SCALE_FACTOR_T);
	#ifdef SDP8_GET_TEMP_BRUT
				pExtData->TemperatureBrut = tmpTempBrut;
	#endif // SDP8_GET_TEMP_BRUT
#endif // SDP8_NB_BLOCS_2READ

				// Calcul d'une Moyenne glissante sur la base des "Echantillons RAW" de Pression du Capteur :
				float tmpBridgeValue;
#if defined(SDP8_MOY_P_FROM_RAW) && (SDP8_MOY_P_FROM_RAW > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				tmpBridgeValue = i2cComMaster_AddInt16ToMoyManager(&pIntData->PresRaw, (int16_t)valeurBridgeBrut, SDP8_MOY_P_FROM_RAW);
#else // ! SDP8_MOY_P_FROM_RAW
				tmpBridgeValue = (float)valeurBridgeBrut;
#endif // SDP8_MOY_P_FROM_RAW
#ifdef SDP8_GET_VALUE_MOY
				pExtData->BridgeMoy = tmpBridgeValue;
#endif // SDP8_GET_VALUE_MOY

				// Conversion du RAW moyenné -> Pascals :
				if(0 == pIntData->ScaleFactor) pIntData->ScaleFactor = SDP8_DEFAUT_SCALE_FACTOR_P; // Protection en cas de perte de Coefficient
/*	Exemple de Filtrage sur le Type de Device (partie 1) :
				switch(pIntData->base.idOfDevice)
				{
				case I2cDevPresType_SDP8_8x0:
				case I2cDevPresType_SDP8_8x1:
*/
					NewPressure = tmpBridgeValue/((float)pIntData->ScaleFactor); // Division par le ScaleFactor
/*	Exemple de Filtrage sur le Type de Device (partie 2) :
					break;
				default:
					NewPressure = 0;
					break;
				}
*/

#ifdef SDP8_GET_PRESS_BRUT
				pExtData->PresBrut = NewPressure;
#endif // SDP8_GET_PRESS_BRUT

				// Calcul d'une Moyenne glissante sur la base des Echantillons déjà Convertis en Pression :
#if defined(SDP8_MOY_P_FROM_CONV) && (SDP8_MOY_P_FROM_CONV > 0)	// QuickMoy by Jp for AldesAeraulique © 2021 :
				pExtData->Pressure = i2cComMaster_AddFloatToMoyManager(&pIntData->PresConv, NewPressure, SDP8_MOY_P_FROM_CONV);
#else // ! SDP8_MOY_P_FROM_CONV
				pExtData->Pressure = NewPressure;
#endif // SDP8_MOY_P_FROM_CONV

				// Délai de prochaine action :
				pDevice->DelayInMs  = SDP8_TEMPO_BEFORE_NEXT_ACTION; // 1s avant prochaine Lecture

				// Signaler à la tâche qu'il y a 1 nouvelle Data à récupérer :
				pExtData->newFlags |= I2cCmPressureSdp8NewPres; // Signale la nouvelle Data
				return I2C_NEW_VALUE | I2C_END_BLOC; // Signaler puis rendre la main
			}
		}
	}
	break;
	//---------------------
	case I2CCM_RQ_INIT_DATA:
		if(0 == pDevice->result) // No I2C error :
		{
			pDevice->DelayInMs = 0;	// Par défaut : effectuer l'opération suivante tout de suite dès le prochain tour
			switch(pDevice->FrameId)
			{
			case I2cCmSdp8InitFrameResetSoft:	// Soft Reset Done :
// Correction_Jp le 29/10/2021 : Envoyer cette Commande sur "0" réinitialise également tous les autres Devices I2C, ce qui n'est pas souhaité :-( !
//				pDevice->DevAddr8 = pIntData->tmpU16;	// Restaure le DevAdr
				pDevice->FrameId++;		// OK pour passer à la suite
				pDevice->DelayInMs = SDP8_TEMPO_BEFORE_FIRST_ACTION; // Tempo de récupération après ResetSoft
				return I2C_END_BLOC;	// Libère la ressource pour le moment
			//-----------------------------
#ifdef SDP8_GET_SERIAL_NUMBER
			case I2cCmSdp8InitFrameReadPID32:
				pDevice->FrameId++;		// OK pour passer à la suite
				return I2C_WAIT_TEMPO;	// Pseudo tempo + Action immédiate
				break;
			case I2cCmSdp8InitFrameReadSN64:
				if(isValidMultiU16BlocsCRC8(pRxBuf, SDP_8x_SN_BLOCS_2READ, SDP_8xx_CRC_INIT)) // CRC OK sur les 2+4 parties :
				{
					// ProductId = 0x3020a01 && SerialNumber = 0x8350b1e0 sur Manta01M6-002
					// ProductId = 0x3020a01 && SerialNumber = 0x8350b1af sur Manta01M6-013
					pExtData->ProductId = I2CCM_JOIN_16_16(I2CCM_JOIN_8_8_BE(pRxBuf[0], pRxBuf[1]), I2CCM_JOIN_8_8_BE(pRxBuf[3], pRxBuf[4]));
					pExtData->SerialNumber = I2CCM_JOIN_32_32(I2CCM_JOIN_16_16(I2CCM_JOIN_8_8_BE(pRxBuf[ 6], pRxBuf[ 7]), I2CCM_JOIN_8_8_BE(pRxBuf[ 9], pRxBuf[10])),
															  I2CCM_JOIN_16_16(I2CCM_JOIN_8_8_BE(pRxBuf[12], pRxBuf[13]), I2CCM_JOIN_8_8_BE(pRxBuf[15], pRxBuf[16])));
					pExtData->newFlags |= I2cCmPressureSdp8NewProdId | I2cCmPressureSdp8NewSerialN;
					pDevice->FrameId++;		// OK pour passer à la suite
					return I2C_END_BLOC;	// Libère la ressource pour le moment
				}
				// Erreur de CRC => Redemander également le PID32 :
				pDevice->FrameId = I2cCmSdp8InitFrameReadPID32;
				break;
#endif // SDP8_GET_SERIAL_NUMBER
			//-----------------------------
			case I2cCmSdp8InitFrameReadScaleFactor:
				if(isValidMultiU16BlocsCRC8(pRxBuf  +6, 1, SDP_8xx_CRC_INIT)) // CRC du ScaleFactor OK :
				{
					pIntData->ScaleFactor = I2CCM_JOIN_8_8_BE(pRxBuf[6], pRxBuf[7]);
					pDevice->FrameId++;		// OK pour passer à la suite
					return I2C_END_BLOC;	// Libère la ressource pour le moment
				}
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

	if(pIntData->ErrorsCt > SDP8_PRESS_MAX_ALLOWED_ERRORS)
	{
		pDevice->ActionId = I2CCM_ACTION_INIT; // Demander à Ré-Initialiser
		return I2C_ABORT_DEV; // +Recommander d'Abandonner l'usage de ce Device
	}

	pDevice->DelayInMs = SDP8_TEMPO_RETRY_ACTION; // Tempo avant Retry : 1s
	return I2C_END_BLOC | I2C_CHK_DEV_ERR; // Rendre la main + Demander à Vérifier si erreur sur le Device ...
}

/******************************************************************************/

#ifdef SDP8_MOY_P_FROM_RAW
	__attribute__((weak)) I2CCM_BUILD_GET_MAX_CT4MOY(getI2CCM_SDP8_MaxRawPresCt4Moy, SDP8_MOY_P_FROM_RAW)
#endif // SDP8_MOY_P_FROM_RAW

/******************************************************************************/

#ifdef SDP8_MOY_P_FROM_CONV
	__attribute__((weak)) I2CCM_BUILD_GET_MAX_CT4MOY(getI2CCM_SDP8_MaxConvPresCt4Moy, SDP8_MOY_P_FROM_CONV)
#endif // SDP8_MOY_P_FROM_CONV

#ifdef __cplusplus
}
#endif
