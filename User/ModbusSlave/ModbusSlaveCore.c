/*
 * ModbusSlaveCore.c
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 25 Feb. 2025
 *  Updated by: j.proux
 *
 * Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 * -> il te faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#include "ModbusSlaveCore.h"	// Pour nos propres déclarations publiques
#include "main.h"				// Pour accès aux bons includes des HAL
#include "crc.h"				// Pour les calculs de CRC nécessaires sur le Modbus

/******************************************************************************/
// Includes Optionnels :

//#include "VersionInfos.h"	// Pour les infos de Version / Run / CRC
//#include <math.h>			// Pour l'arrondi correct en négatif aussi

/******************************************************************************/
// Includes automatiques suivant les besoins :

#if defined(MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY)
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR" (cf. "ModbusSlaveConf.h"), par exemple "MEM_DEV_MAX_ADR" from "MemHistoCore.h"
	#include "MemHistoCore.h"	// Pour accès à MEM_DEV_MAX_ADR
	#include "Memoire.h"		// Pour accès aux Fonctions sur la Mémoire Externe, Requires Library "MemoryBridge" (cf. FW HII_CarteMere_App)
#endif // MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY

#if defined(MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE)
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_EXT_RES_MAX_SIZE" (cf. "ModbusSlaveConf.h"), par exemple "MEM_HISTO_RES_MAX_SIZE", Max = 64K words
	#include "MemHistoConf.h"	// Pour accès à MEM_HISTO_RES_MAX_SIZE
	#include "IhmRes.h" 		// Pour accès aux Fonctions de Gestion des Ressources en Mémoire Externe, Requires Library "IhmRes" (cf. FW IHM EasyKub)
#endif // MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE

#if defined(MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM)
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_FIRMWARE_BASE_ADR" (cf. "ModbusSlaveConf.h"), par exemple "FIRMWARE_BASE_ADR" from "VersionInfos.h"
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE" (cf. "ModbusSlaveConf.h"), par exemple "FIRMWARE_MAX_SIZE" from "VersionInfos.h"
	#include "FlashInfos.h"		// Pour accès à "FIRMWARE_BASE_ADR" & "FIRMWARE_MAX_SIZE", Requires Library "VersionInfos" (cf. FW HII_CarteMere_App)
#endif // MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

#if defined(MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC)
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE" (cf. "ModbusSlaveConf.h"), par exemple "FIRMWARE_MAX_SIZE" from "VersionInfos.h"
	#include "UpgradeFirmware.h"// Pour accès aux Fonctions de Mise à Jour du Firmware, Requires Library "UpgradeFirmware" (cf. FW HII_CarteMere_App)
	#include "FlashInfos.h"		// Pour accès à "FIRMWARE_MAX_SIZE", Requires Library "VersionInfos" (cf. FW HII_CarteMere_App)
#endif // MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC

#ifdef MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE
// Requiert la Configuration de "MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE" (cf. "ModbusSlaveConf.h"), par exemple "FIRMWARE_MAX_SIZE" from "VersionInfos.h"
	#include "UpgradeFirmware.h"// Pour accès aux Fonctions de Mise à Jour du Firmware, Requires Library "UpgradeFirmware" (cf. FW HII_CarteMere_App)
#endif // MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE

/******************************************************************************/

#define MODBUS_SLAVE_BROADCAST_ALL_ADR	0x00

// Liste des Fonctions Modbus Standard supportées :
#define MODBUS_SLAVE_FCT_READ_REG	0x03 // Read Holding Registers   ("Modbus_Application_Protocol_V1_1b3.pdf" page 15)
#define MODBUS_SLAVE_FCT_WR_1REG	0x06 // Write Single Register	 ("Modbus_Application_Protocol_V1_1b3.pdf" page 19)
#define MODBUS_SLAVE_FCT_WRITE_REG	0x10 // Write Multiple registers ("Modbus_Application_Protocol_V1_1b3.pdf" page 30)

// Liste des "User Defined Function codes" supportés ("Modbus_Application_Protocol_V1_1b3.pdf" page 10) :
#define MODBUS_FCT_FLASH_FIRMWARE	0x69 // $69 = 105 : Write a Firmware Program Bloc
#define MODBUS_FCT_CHECK_FIRMWARE	0x68 // $68 = 104 : Check & Validate a Firmware Program
#define MODBUS_FCT_READ_INT_PRGM	0x6A // $6A = 106 : Read an Internal Firmware Program Bloc

#define MODBUS_FCT_WRITE_EXT_RES	0x74 // $74 = 116 : Write Custom Resources to External Memory
#define MODBUS_FCT_WRITE_EXT_MEM	0x75 // $75 = 117 : Write External Memory Bloc
#define MODBUS_FCT_ERASE_EXT_MEM	0x76 // $76 = 118 : Erase External Memory Bloc
#define MODBUS_FCT_READ_EXT_MEM 	0x77 // $77 = 119 : Read External Memory Bloc
#define MODBUS_FCT_READ_EXT_EEP 	0x78 // $78 = 120 : Read External EEPROM Bloc

// Liste des Codes Exceptions supportés (cf. "Modbus_Application_Protocol_V1_1b3.pdf" page 48) :
#define MODBUS_SLAVE_ERR_NO_ERR 	0x00 // No Error
#define MODBUS_SLAVE_ERR_ILL_FN 	0x01 // Modbus "ILLEGAL FUNCTION" Exception Code ("Modbus_Application_Protocol_V1_1b3.pdf" page 48)
#define MODBUS_SLAVE_ERR_ILL_ADR	0x02 // Modbus "ILLEGAL DATA ADDRESS" Exception Code ("Modbus_Application_Protocol_V1_1b3.pdf" page 48)
#define MODBUS_SLAVE_ERR_ILL_DATA	0x03 // Modbus "ILLEGAL DATA VALUE" Exception Code ("Modbus_Application_Protocol_V1_1b3.pdf" page 48)

// Réponses attendues des Fonctions supportées :
#define MODBUS_SLAVE_FRAME_NOT_HANDLED  	0	// Non traité + Chercher un (autre) Handler
#define MODBUS_SLAVE_FRAME_WAS_HANDLED  	1	// Traité, la Réponse est dans le Buffer Tx
#define MODBUS_SLAVE_FRAME_NOT_SUPPORTED	2	// Traité, pas de Réponse à transmettre
#define MODBUS_SLAVE_FRAME_WAS_EXCEPTION	3	// Traité, la Réponse sera une Exception

/******************************************************************************/

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT

#define MODBUS_GET_BYTE_N(a,b)		(uint8_t)(((a) >> ((b)*8)) & 0xFF) // Récupère le ByteN (n = [0, 3] sur 32 bits)
#define MODBUS_MAKE_WORD_LE(l,h)	(uint16_t)((((h) & 0xFF)<<8) | (((l) & 0xFF) << 0)) // h = Byte1; l = Byte0
#define MODBUS_MAKE_WORD_BE(h,l)	(uint16_t)((((h) & 0xFF)<<8) | (((l) & 0xFF) << 0)) // h = Byte1; l = Byte0
#define MODBUS_SWAP_BYTES(a)		(MODBUS_MAKE_WORD_BE(MODBUS_GET_BYTE_N(a,0), MODBUS_GET_BYTE_N(a,1)))

#define MODBUS_MAKE_DWORD_BE(hh,hl,lh,ll)	(uint32_t)((((hh) & 0xFF)<<24) | (((hl) & 0xFF) << 16) | (((lh) & 0xFF)<<8) | (((ll) & 0xFF) << 0)) // hh = Byte3 ... ll = Byte0
#define MODBUS_MAKE_DWORD_LE(ll,lh,hl,hh)	(uint32_t)((((hh) & 0xFF)<<24) | (((hl) & 0xFF) << 16) | (((lh) & 0xFF)<<8) | (((ll) & 0xFF) << 0)) // hh = Byte3 ... ll = Byte0

#define MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(c,a)	(*((c*)a))               // c = cast; a = Variable
#define MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(c,fn)	((*(c(*)(void))(fn))())  // c = cast; fn = Fonction

#define MODBUS_MAKE_CASTED_POINTED_VAR(c,a)		*((c*)a)                // c = cast; a = Variable
#define MODBUS_MAKE_CASTED_POINTED_FCT(c,fn)		(*(void(*)(c))(fn))     // c = cast; fn = Fonction

tModbusSlaveParams ModbusSlaveParam[UART_COM_NB_OF_MODBUS_SLAVE];
tModbusSlaveParams* pCurModbus = 0;		// Pour certaines Fonctions qui ont besoin d'Infos sur le Handler lui-même
tRxTxBufInfo* pCurRxTxBI = 0;			// Pour certaines Fonctions qui ont besoin d'Infos sur la Source elle-même

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

/******************************************************************************/
// Prototypes des Fonctions Internes à usage local :

#ifdef MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS		// (cf. "ModbusSlaveConf.h")
	static int HandleModbusFunctionReadHoldingRegisters(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS 	// (cf. "ModbusSlaveConf.h")
	static int HandleModbusFunctionWriteMultipleRegisters(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER		// (cf. "ModbusSlaveConf.h")
	static int HandleModbusFunctionWriteSingleRegister(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE
	static int HandleModbusFunctionWriteExternalRessource(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE

#ifdef MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY
	static int HandleModbusFunctionDumpExternalMemory(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY

#ifdef MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM
	static int HandleModbusFunctionReadInternalProgram(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC
	static int HandleModbusFunctionWriteFirmwareBloc(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC

#ifdef MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE
	static int HandleModbusFunctionValidateFirmwareProgram(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue);
#endif // MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE

static uint16_t getThisModbusItemSizeW(eTVarGetSet varType);
static inline uint16_t getAccessLevelAccordingToPswd(uint16_t Pswd);

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")
	static int16_t Float_to_Int16(float value, float coef);
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")
	static int32_t Float_to_Int32(float value, float coef);
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

/******************************************************************************/

void InitModbusSlaveStruct(tModbusSlaveParams* pStruct, uint16_t srcId, uint16_t slaveAdr) //, tModbusItem *baseTable)
{

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT

	if(0 == pStruct) return;
	ModbusSlave_FillMemory((void*)pStruct, 0, sizeof(tModbusSlaveParams));

	// Mémorise les Paramètres spécifiques à la Gestion de cette Structure :
	pStruct->SrcId = srcId;
	pStruct->SlaveAdr = slaveAdr;

#if defined(__DEBUG) || defined(DEBUG) // Only under DEBUG conditions :
	#ifndef APP_WITH_BOOT	// Hors Target for BootLoader

		#if defined(COM_UART_MODBUS_IHM_SRC_ID) && defined(MODBUS_SLAVE_DEF_LVL_ACCESS_IHM)
			#warning "Debug MODBUS_SLAVE_DEF_LVL_ACCESS_IHM enabled !"
			if(COM_UART_MODBUS_IHM_SRC_ID  == srcId) { pStruct->AccessLevel = MODBUS_SLAVE_DEF_LVL_ACCESS_IHM; } // 1 = src_IHM -> start DEBUG with Level 5
		#endif // COM_UART_MODBUS_IHM_SRC_ID & MODBUS_SLAVE_DEF_LVL_ACCESS_IHM

		#if defined(COM_UART_MODBUS_USER_SRC_ID) && defined(MODBUS_SLAVE_DEF_LVL_ACCESS_USER)
			#warning "Debug MODBUS_SLAVE_DEF_LVL_ACCESS_USER enabled !"
			if(COM_UART_MODBUS_USER_SRC_ID == srcId) { pStruct->AccessLevel = MODBUS_SLAVE_DEF_LVL_ACCESS_USER; } // 2 = src_User -> start DEBUG with Level 5
		#endif // COM_UART_MODBUS_USER_SRC_ID & MODBUS_SLAVE_DEF_LVL_ACCESS_USER

	#endif // !APP_WITH_BOOT
#endif // __DEBUG || DEBUG

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

}

/******************************************************************************/

int ModbusSlaveRxHandler(tRxTxBufInfo* pRxTxBI, void* pVoidParam)
{

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT

	if(0 == pVoidParam) return 0;
	tModbusSlaveParams* pModbusSlave = pVoidParam;

	int wasHandled = MODBUS_SLAVE_FRAME_NOT_HANDLED;	// Par défaut : not Handled yet !
	int nbBytes = 0;	// ToDo: voir pour passer en uint16_t, ainsi que les prototypes des Handlers de CodeFonction ...

#ifdef MODBUS_SLAVE_SUPPORT_STATS
	if(UINT32_MAX > pModbusSlave->nbFramesRx) { pModbusSlave->nbFramesRx++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

	uint16_t nbBytesRecus = pRxTxBI->RxBuf.nbBytes;
	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
	if(nbBytesRecus >= MODBUS_SLAVE_MIN_RX_SIZE)	// S'il y a suffisamment de Bytes pour une Trame valide :
	{
		uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;

		// Calcul du CRC (transmis en LittleEndian):
		uint16_t CRC_Trame = CRC16(myRx, nbBytesRecus -2, 0xFFFF);
		uint16_t CRC_Recu = MODBUS_MAKE_WORD_LE(myRx[nbBytesRecus -2], myRx[nbBytesRecus -1]);	// lowByte First, then highByte

		if(CRC_Trame == CRC_Recu) // Vérification du CRC :
		{
#ifdef MODBUS_SLAVE_SUPPORT_STATS
			if(UINT32_MAX > pModbusSlave->nbFramesRxCrcOk) { pModbusSlave->nbFramesRxCrcOk++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

			// Vérifie si on est concerné (en direct ou en Broadcast) :
			if( (MODBUS_GET_BYTE_N(pModbusSlave->SlaveAdr, 0) == myRx[0]) || // Teste si c'est notre adresse principale
				(MODBUS_GET_BYTE_N(pModbusSlave->SlaveAdr, 1) == myRx[0]) || // Teste si c'est notre adresse de secours
				(MODBUS_SLAVE_BROADCAST_ALL_ADR == myRx[0]) )
			{
#ifdef MODBUS_SLAVE_SUPPORT_STATS
				if(UINT32_MAX > pModbusSlave->nbFramesRxAdrOk) { pModbusSlave->nbFramesRxAdrOk++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

				pCurModbus = pModbusSlave;	// Pour quelques fonctions dans la Table qui ont besoin de contrôler d'autres Variables
				pCurRxTxBI = pRxTxBI; 		// Pour quelques fonctions qui auraient besoin d'Infos sur le Source de la Requête

				//--------------------------------------------
				// Gestion Ordre de Lecture (sauf Broadcast) :
				//--------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS		// (cf. "ModbusSlaveConf.h")
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionReadHoldingRegisters(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS

				//----------------------------------------------
				// Gestion Ordre Ecriture (Broadcast autorisé) :
				//----------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS	// (cf. "ModbusSlaveConf.h")
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionWriteMultipleRegisters(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER		// (cf. "ModbusSlaveConf.h")
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionWriteSingleRegister(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER

				//--------------------------------------------------
				// Gestion Ecriture Ressource (Broadcast autorisé) :
				//--------------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionWriteExternalRessource(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE

				//------------------------------------------------
				// Gestion Ordre de Dump Memory (sauf Broadcast) :
				//------------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionDumpExternalMemory(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY

				//----------------------------------------------
				// Gestion Ordre Ecriture Programme Interne (Broadcast autorisé) :
				//----------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionWriteFirmwareBloc(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_WRITE_INTERNAL_FLASH_PRGM

				//----------------------------------------------
				// Gestion Ordre Vérification Programme (sauf Broadcast) :
				//----------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionValidateFirmwareProgram(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE

				//----------------------------------------------
				// Gestion Ordre Lecture Programme (sauf Broadcast) :
				//----------------------------------------------
#ifdef MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					wasHandled = HandleModbusFunctionReadInternalProgram(pRxTxBI, pModbusSlave, &nbBytes);
				}
#endif // MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

				//----------------------------------------------
				// ToDo: Add above here support for new Modbus functions ...
				//----------------------------------------------

#ifdef MODBUS_SLAVE_SUPPORT_STATS
				if(MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled)
				{
					if(UINT32_MAX > pModbusSlave->nbFramesNoFnHand) { pModbusSlave->nbFramesNoFnHand++; } // Pour les Stats
				}
				else if(MODBUS_SLAVE_FRAME_WAS_EXCEPTION == wasHandled)
				{
					if(UINT32_MAX > pModbusSlave->nbFramesTxExcept) { pModbusSlave->nbFramesTxExcept++; } // Pour les Stats
				}
#endif // MODBUS_SLAVE_SUPPORT_STATS

				// Referme les accès spéciaux aux infos locales :
				pCurModbus = 0;
				pCurRxTxBI = 0;

				//----------------------------------------------
				// Bilan du Traitement Modbus (sauf Broadcast) :
				//----------------------------------------------
				if( ((MODBUS_SLAVE_FRAME_NOT_HANDLED == wasHandled) || (MODBUS_SLAVE_FRAME_WAS_EXCEPTION == wasHandled) ) &&
					(MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) )
				{
					if(pRxTxBI->TxBuf.maxBytes >= MODBUS_SLAVE_MIN_TX_SIZE)
					{
						//uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
						uint8_t CodeErr = MODBUS_SLAVE_ERR_ILL_FN;	// Par défaut : Code Exception "Fonction non supportée"
						if(MODBUS_SLAVE_FRAME_WAS_EXCEPTION == wasHandled) // Si la Réponse à envoyer est une Exception en particulier :
						{
							CodeErr = (uint8_t)nbBytes;// & 0xFF; // Récupère le CodeErreur à partir du 'nb de Bytes' retourné par le Handler ;-).
						} else {
							wasHandled = MODBUS_SLAVE_FRAME_NOT_SUPPORTED; // Demande l'envoi quand même de la Réponse (Trame d'Exception)
						}
						// Contruit la Réponse d'Exception :
						nbBytes = 0;
						myTx[nbBytes++] = myRx[0];			// Destinataire -> Emmeteur
						myTx[nbBytes++] = myRx[1] | 0x80;	// Code Fonction identique + Flag Exception
						myTx[nbBytes++] = CodeErr;			// Transmet le Code Exception demandé
					} else {
						nbBytes = 0;
					}
				}
			} else {
				// L'identifiant ne nous correspond pas !
#ifdef MODBUS_SLAVE_SUPPORT_STATS
            	if(UINT32_MAX > pModbusSlave->nbFramesRxAdrErr) { pModbusSlave->nbFramesRxAdrErr++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS
			}
		} else {
			// Le CRC n'est pas correct !
#ifdef MODBUS_SLAVE_SUPPORT_STATS
        	if(UINT32_MAX > pModbusSlave->nbFramesRxCrcErr) { pModbusSlave->nbFramesRxCrcErr++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS
		}
	} else { // Taille Trame Insuffisante :
#ifdef MODBUS_SLAVE_SUPPORT_STATS
		if(UINT32_MAX > pModbusSlave->nbFramesRxLowSz) { pModbusSlave->nbFramesRxLowSz++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

	}

	// ToDo: ajouter ifdef registerLastFrameResult
	pModbusSlave->lastFrameResult = (uint16_t)wasHandled;	// Pour Mémoire du résultat du dernier traitement

	//------------------------------------------------------
	// Si ça a été Traité et qu'il y a des Bytes à envoyer :
	//------------------------------------------------------
	if( (MODBUS_SLAVE_FRAME_NOT_HANDLED != wasHandled) && (0 != nbBytes) &&
		(pRxTxBI->TxBuf.maxBytes >= (nbBytes +2)) ) // Et assez de place pour ajouter le CRC
	{
#ifdef MODBUS_SLAVE_SUPPORT_STATS
		if(UINT32_MAX > pModbusSlave->NbTramesTx) { pModbusSlave->NbTramesTx++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

		//uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;

		// Ajouter le CRC16 (+ placer en Little Endian) :
		uint16_t CRC_Trame = CRC16(myTx, (uint16_t)nbBytes, 0xFFFF);
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(CRC_Trame, 0); // LowByte first
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(CRC_Trame, 1); // then HighByte

		// Activer le Tranfert :
		pRxTxBI->TxBuf.nbBytes = (uint16_t)nbBytes;
		return 1;  // Envoyer la Réponse préparée
	}

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

	return 0;
}

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SLAVE_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS)	// (cf. "ModbusSlaveConf.h")

static int HandleModbusFunctionReadHoldingRegisters(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
#define MIN_READ_REGISTERS_RX_FRAME_SIZE	(MODBUS_SLAVE_HEADER_SIZE + 2 + 2 + MODBUS_SLAVE_FOOTER_SIZE)	// idRegister = 2, nbRegister = 2 => 8
#define MIN_READ_REGISTERS_TX_FRAME_SIZE	(MODBUS_SLAVE_HEADER_SIZE + 1 + 2 + MODBUS_SLAVE_FOOTER_SIZE)	// byteCount = 1, Data >= 2 => 7
#define MAX_READ_REGISTERS_ONCE		125

	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if( (MODBUS_SLAVE_FCT_READ_REG != myRx[1]) || (MODBUS_SLAVE_BROADCAST_ALL_ADR == myRx[0]) ) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; }	// Not Handled
	if(pRxTxBI->RxBuf.nbBytes < MIN_READ_REGISTERS_RX_FRAME_SIZE) { *retValue = MODBUS_SLAVE_ERR_ILL_DATA; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; }		// Taille Trame Insuffisante
//	if(pRxTxBI->RxBuf.nbBytes > (2 +4 +1 +2)) { *retValue = MODBUS_SLAVE_ERR_ILL_DATA; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; }	// Taille Trame Trop large

// inutile	*retValue = 0; // Par défaut : rien à Répondre
	if(pRxTxBI->TxBuf.maxBytes < MIN_READ_REGISTERS_TX_FRAME_SIZE) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

#ifdef MODBUS_SLAVE_SUPPORT_STATS
	if(UINT32_MAX > pModbusSlave->nbFramesRead03) { pModbusSlave->nbFramesRead03++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

	// Récupère les Paramètres :
	uint16_t idRegister = MODBUS_MAKE_WORD_BE(myRx[2], myRx[3]); // HighByte @2, LowByte @ 3
	uint16_t nbRegister = MODBUS_MAKE_WORD_BE(myRx[4], myRx[5]); // HighByte @4, LowByte @ 5
	uint16_t nbBytes = (pRxTxBI->TxBuf.maxBytes - MODBUS_SLAVE_HEADER_SIZE - 1 - MODBUS_SLAVE_FOOTER_SIZE) >> 1; // Max allowed Registers

	if(MAX_READ_REGISTERS_ONCE < nbRegister) { nbRegister = MAX_READ_REGISTERS_ONCE; } // Max 125 registres 16bits pour rentrer dans 256 Bytes
	if(nbRegister > nbBytes) { nbRegister = nbBytes; }	// Plafonne au nb max de Registres qu'on peut envoyer sur le Buffer de Réponse

	// Début de la Réponse :
	nbBytes = 0;
	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
	myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
	myTx[nbBytes++] = myRx[1];  // Code Fonction identique
	myTx[nbBytes++] = (unsigned char)((nbRegister << 1) & 0xFF); // nb Bytes correspondants

	// Variables spécifiques à l'opération :
	uint16_t Data16;
	uint16_t curModBusItmSize;
	tModbusSlaveQWordVar tmpQWordVar = { .ULongLong = UINT64_MAX };

	// Parcours de la Table Modbus :
	tModbusSlaveItem* curModbusItem = (tModbusSlaveItem*)MODBUS_SLAVE_BASE_OF_TABLE; // Pointe la base du Tableau à parcourir

	while(0 < nbRegister)
	{
        Data16 = MODBUS_SLAVE_DEFAULT_UINT16_VALUE;
        while( (curModbusItem < MODBUS_SLAVE_END_OF_TABLE) && (idRegister >= curModbusItem->Adr) )
        {
            curModBusItmSize = getThisModbusItemSizeW(curModbusItem->typeGetSet);
            if( (idRegister >= curModbusItem->Adr) && (idRegister < (curModbusItem->Adr + curModBusItmSize)) )
            {
                if(0 != curModbusItem->getPtr) // On a un Pointeur d'accès en Lecture :
                {
                    if(curModbusItem->readMinLevel <= pModbusSlave->AccessLevel)
                    {
                        if(idRegister == curModbusItem->Adr)
                        {
                            tmpQWordVar.ULongLong = 0;
                            switch (curModbusItem->typeGetSet) {
                            	// Type Signed Char (1 Byte = 8 bits) :
                            case TVarSCharGetVarSetVar:
                            case TVarSCharGetVarSetFct:
                                tmpQWordVar.SChar0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(int8_t, curModbusItem->getPtr));
                                break;
                            case TVarSCharGetFctSetVar:
                            case TVarSCharGetFctSetFct:
                                tmpQWordVar.SChar0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(int8_t, curModbusItem->getPtr));
                                break;
                                // Type Unsigned Char (1 Byte = 8 bits) :
                            case TVarUCharGetVarSetVar:
                            case TVarUCharGetVarSetFct:
                                tmpQWordVar.UChar0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint8_t, curModbusItem->getPtr));
                                break;
                            case TVarUCharGetFctSetVar:
                            case TVarUCharGetFctSetFct:
                                tmpQWordVar.UChar0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(uint8_t, curModbusItem->getPtr));
                                break;

                                // Type Signed Int (2 Bytes = 1x 16 bits) :
                            case TVarSIntGetVarSetVar:
                            case TVarSIntGetVarSetFct:
                                tmpQWordVar.SInt0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(int16_t, curModbusItem->getPtr));
                                break;
                            case TVarSIntGetFctSetVar:
                            case TVarSIntGetFctSetFct:
                                tmpQWordVar.SInt0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(int16_t, curModbusItem->getPtr));
                                break;
                                // Type Unsigned Int (2 Bytes = 1x 16 bits) :
                            case TVarUIntGetVarSetVar:
                            case TVarUIntGetVarSetFct:
                                tmpQWordVar.UInt0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint16_t, curModbusItem->getPtr));
                                break;
                            case TVarUIntGetFctSetVar:
                            case TVarUIntGetFctSetFct:
                                tmpQWordVar.UInt0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(uint16_t, curModbusItem->getPtr));
                                break;

#ifdef MODBUS_SLAVE_SUPPORT_LONG_INT32	// (cf. "ModbusSlaveConf.h")
                                //----------------------------------------
                                // Type Signed Long (4 Bytes = 2x 16 bits) :
                            case TVarSLongGetVarSetVar:
                            case TVarSLongGetVarSetFct:
                                tmpQWordVar.SLong0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(int32_t, curModbusItem->getPtr));
                                break;
                            case TVarSLongGetFctSetVar:
                            case TVarSLongGetFctSetFct:
                                tmpQWordVar.SLong0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(int32_t, curModbusItem->getPtr));
                                break;
                                // Type Unsigned Long (4 Bytes = 2x 16 bits) :
                            case TVarULongGetVarSetVar:
                            case TVarULongGetVarSetFct:
                                tmpQWordVar.ULong0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint32_t, curModbusItem->getPtr));
                                break;
                            case TVarULongGetFctSetVar:
                            case TVarULongGetFctSetFct:
                                tmpQWordVar.ULong0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(uint32_t, curModbusItem->getPtr));
                                break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_INT32

#ifdef MODBUS_SLAVE_SUPPORT_LONG_LONG	// (cf. "ModbusSlaveConf.h")
                                //----------------------------------------
                                // Type Signed LongLong (8 Bytes = 4x 16 bits) :
                            case TVarSLongLongGetVarSetVar:
                            case TVarSLongLongGetVarSetFct:
                                tmpQWordVar.SLongLong = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(int64_t, curModbusItem->getPtr));
                                break;
                            case TVarSLongLongGetFctSetVar:
                            case TVarSLongLongGetFctSetFct:
                                tmpQWordVar.SLongLong = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(int64_t, curModbusItem->getPtr));
                                break;

                                // Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
                            case TVarULongLongGetVarSetVar:
                            case TVarULongLongGetVarSetFct:
                                tmpQWordVar.ULongLong = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint64_t, curModbusItem->getPtr));
                                break;
                            case TVarULongLongGetFctSetVar:
                            case TVarULongLongGetFctSetFct:
                                tmpQWordVar.ULongLong = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(uint64_t, curModbusItem->getPtr));
                                break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")
                                //----------------------------------------
                                // Type Float Int x1 (2 Bytes = 1x 16 bits) :
                            case TVarFloatIntX1GetVarSetVar:
                            case TVarFloatIntX1GetVarSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 1.0f);
                                break;
                            case TVarFloatIntX1GetFctSetVar:
                            case TVarFloatIntX1GetFctSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 1.0f);
                                break;

                                // Type Float Int x10 (2 Bytes = 1x 16 bits) :
                            case TVarFloatIntX10GetVarSetVar:
                            case TVarFloatIntX10GetVarSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 10.0f);
                                break;
                            case TVarFloatIntX10GetFctSetVar:
                            case TVarFloatIntX10GetFctSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 10.0f);
                                break;

                                // Type Float Int x100 (2 Bytes = 1x 16 bits) :
                            case TVarFloatIntX100GetVarSetVar:
                            case TVarFloatIntX100GetVarSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 100.0f);
                                break;
                            case TVarFloatIntX100GetFctSetVar:
                            case TVarFloatIntX100GetFctSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 100.0f);
                                break;

                                // Type Float Int x1000 (2 Bytes = 1x 16 bits) :
                            case TVarFloatIntX1000GetVarSetVar:
                            case TVarFloatIntX1000GetVarSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 1000.0f);
                                break;
                            case TVarFloatIntX1000GetFctSetVar:
                            case TVarFloatIntX1000GetFctSetFct:
                                tmpQWordVar.SInt0 = Float_to_Int16(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 1000.0f);
                                break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")
                                //----------------------------------------
                                // Type Float Long x10 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX10GetVarSetVar:
                            case TVarFloatLongX10GetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 10.0f);
                                break;
                            case TVarFloatLongX10GetFctSetVar:
                            case TVarFloatLongX10GetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 10.0f);
                                break;

                                // Type Float Long x100 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX100GetVarSetVar:
                            case TVarFloatLongX100GetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 100.0f);
                                break;
                            case TVarFloatLongX100GetFctSetVar:
                            case TVarFloatLongX100GetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 100.0f);
                                break;

                                // Type Float Long x1000 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX1KGetVarSetVar:
                            case TVarFloatLongX1KGetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 1000.0f);
                                break;
                            case TVarFloatLongX1KGetFctSetVar:
                            case TVarFloatLongX1KGetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 1000.0f);
                                break;

                                // Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX10KGetVarSetVar:
                            case TVarFloatLongX10KGetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 10000.0f);
                                break;
                            case TVarFloatLongX10KGetFctSetVar:
                            case TVarFloatLongX10KGetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 10000.0f);
                                break;

                                // Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX100KGetVarSetVar:
                            case TVarFloatLongX100KGetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 100000.0f);
                                break;
                            case TVarFloatLongX100KGetFctSetVar:
                            case TVarFloatLongX100KGetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 100000.0f);
                                break;

                                // Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongX1MGetVarSetVar:
                            case TVarFloatLongX1MGetVarSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr), 1000000.0f);
                                break;
                            case TVarFloatLongX1MGetFctSetVar:
                            case TVarFloatLongX1MGetFctSetFct:
                                tmpQWordVar.SLong0 = Float_to_Int32(MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr), 1000000.0f);
                                break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW	// (cf. "ModbusSlaveConf.h")
                                //----------------------------------------
                                // Type Float Long Raw (4 Bytes = 2x 16 bits) :
                            case TVarFloatLongRawGetVarSetVar:
                            case TVarFloatLongRawGetVarSetFct:
                                tmpQWordVar.Float0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(float, curModbusItem->getPtr));
                                break;
                            case TVarFloatLongRawGetFctSetVar:
                            case TVarFloatLongRawGetFctSetFct:
                                tmpQWordVar.Float0 = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(float, curModbusItem->getPtr));
                                break;

                                // Type Double LongLong Raw (8 Bytes = 4x 16 bits) :
                            case TVarDoubleLongLongRawGetVarSetVar:
                            case TVarDoubleLongLongRawGetVarSetFct:
                                tmpQWordVar.Double = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(double, curModbusItem->getPtr));
                                break;
                            case TVarDoubleLongLongRawGetFctSetVar:
                            case TVarDoubleLongLongRawGetFctSetFct:
                                tmpQWordVar.Double = (MODBUS_GET_CASTED_VALUE_FROM_POINTED_FCT(double, curModbusItem->getPtr));
                                break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

                            case TVarUnknown:
                                break;
                            default:
                                break;
                            }
                        }
                        Data16 = tmpQWordVar.Word[(curModBusItmSize - (idRegister - curModbusItem->Adr) -1)];
                    }
                }
                if(idRegister == (curModbusItem->Adr + curModBusItmSize -1))
                {
                    curModbusItem++;
                }
                break; // Passer tout de suite à la demande suivante ...
            } else {
                curModbusItem++;
            }
        }

		// Placer le Word en BigIndian dans la Trame de Réponse :
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(Data16, 1); // HighByte first
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(Data16, 0); // then LowByte

// Comment_Jp le 28/11/2024 : Déjà intégré dans le calcul préalable de "Max allowed Registers" ...
//        if(nbBytes >= (pRxTxBI->TxBuf.maxBytes -3)) // OK pour (>= 3)
//        {
//            myTx[2] -= ((uint8_t)((nbRegister << 1) & 0xFF)); // Retire le nb de Registers manquants
//            break;
//        }

        idRegister++;
        nbRegister--;
    }

	*retValue = nbBytes;
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SLAVE_SUPPORT && MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SLAVE_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER)	// (cf. "ModbusSlaveConf.h")

static int HandleModbusFunctionWriteSingleRegister(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, unsigned int *retValue)
{
#define INDEX_WRITE_REGISTER_VALUE			(MODBUS_SLAVE_HEADER_SIZE + 2)	// idRegister = 2 => 4
#define MIN_WRITE_REGISTER_RX_FRAME_SIZE	(INDEX_WRITE_REGISTER_VALUE + 2 + MODBUS_SLAVE_FOOTER_SIZE) // Data = 2 => 8
#define MIN_WRITE_REGISTER_TX_FRAME_SIZE	(INDEX_WRITE_REGISTER_VALUE + 2 + MODBUS_SLAVE_FOOTER_SIZE) // Data = 2 => 8

	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if(MODBUS_SLAVE_FCT_WR_1REG != myRx[1]) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	if(pRxTxBI->RxBuf.nbBytes < MIN_WRITE_REGISTER_RX_FRAME_SIZE) { *retValue = MODBUS_SLAVE_ERR_ILL_DATA; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; }		// Taille Trame Insuffisante

// inutile	*retValue = 0; // Par défaut : rien à  Répondre
//	if(pRxTxBI->TxBuf.maxBytes < MIN_WRITE_REGISTER_TX_FRAME_SIZE) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

#ifdef MODBUS_SLAVE_SUPPORT_STATS
//	if(UINT32_MAX > pModbusSlave->nbFramesWrite6) { pModbusSlave->nbFramesWrite6++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

	// Récupère les Paramètres :
	uint16_t idRegister = MODBUS_MAKE_WORD_BE(myRx[2], myRx[3]); // HighByte @2, LowByte @ 3
	//uint16_t valRegister = MODBUS_MAKE_WORD_BE(myRx[4], myRx[5]); // HighByte @4, LowByte @ 5

	// Début de la Réponse :
	if( (MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) && (pRxTxBI->TxBuf.maxBytes >= MIN_WRITE_REGISTER_TX_FRAME_SIZE) )
	{
		uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
		uint16_t nbBytes = 0;
		myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
		myTx[nbBytes++] = myRx[1];  // Code Fonction identique
		myTx[nbBytes++] = myRx[2];	// Starting Address Hi
		myTx[nbBytes++] = myRx[3];	// Starting Address Lo
		myTx[nbBytes++] = myRx[4];	// Register Value Hi
		myTx[nbBytes++] = myRx[5];	// Register Value Lo
	    *retValue = nbBytes;	// Renseigne déjà la taille finale de la Réponse !
	}

	// Variables spécifiques à l'opération :
	uint16_t Data16 = MODBUS_MAKE_WORD_BE(myRx[INDEX_WRITE_REGISTER_VALUE], myRx[INDEX_WRITE_REGISTER_VALUE +1]); // Recompose en BigEndian;
	uint16_t curModBusItmSize;
	tModbusSlaveQWordVar VarTampon;

	// Parcours de la Table Modbus :
	tModbusSlaveItem* curModbusItem = (tModbusSlaveItem*)MODBUS_SLAVE_BASE_OF_TABLE; // Pointe la base du Tableau à parcourir
	while( (curModbusItem < MODBUS_SLAVE_END_OF_TABLE) && (idRegister >= curModbusItem->Adr) )
	{
		curModBusItmSize = getThisModbusItemSizeW((eTVarGetSet)curModbusItem->typeGetSet);
		if( (idRegister >= curModbusItem->Adr) && (idRegister < (curModbusItem->Adr + curModBusItmSize)) )
		{
			if(0 != curModbusItem->setPtr) // On a un Pointeur d'accès en Ecriture :
			{
				if(curModbusItem->writeMinLevel <= pModbusSlave->AccessLevel)
				{
					// Etape 1 : Mise en Mémoire Tampon :
					VarTampon.Word[(curModBusItmSize - (idRegister - curModbusItem->Adr) -1)] = Data16;

					// Etape 2 : Ecrire si Valeur est Complète :
					if(idRegister == (curModbusItem->Adr + curModBusItmSize -1)) // AdrEnCours = LowWord
					{
						// Transfère la Data Complète :
						switch(curModbusItem->typeGetSet)
						{
							// Type Signed Char (1 Byte = 8 bits) :
						case TVarSCharGetVarSetVar:
						case TVarSCharGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int8_t, curModbusItem->setPtr) = VarTampon.SChar0;
							break;
						case TVarSCharGetVarSetFct:
						case TVarSCharGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int8_t, curModbusItem->setPtr)(VarTampon.SChar0);
							break;
							// Type Unsigned Char (1 Byte = 8 bits) :
						case TVarUCharGetVarSetVar:
						case TVarUCharGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint8_t, curModbusItem->setPtr) = VarTampon.UChar0;
							break;
						case TVarUCharGetVarSetFct:
						case TVarUCharGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint8_t, curModbusItem->setPtr)(VarTampon.UChar0);
							break;

							// Type Signed Int (2 Bytes = 1x 16 bits) :
						case TVarSIntGetVarSetVar:
						case TVarSIntGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int16_t, curModbusItem->setPtr) = VarTampon.SInt0;
							break;
						case TVarSIntGetVarSetFct:
						case TVarSIntGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int16_t, curModbusItem->setPtr)(VarTampon.SInt0);
							break;
							// Type Unsigned Int (2 Bytes = 1x 16 bits) :
						case TVarUIntGetVarSetVar:
						case TVarUIntGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint16_t, curModbusItem->setPtr) = VarTampon.UInt0;
							break;
						case TVarUIntGetVarSetFct:
						case TVarUIntGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint16_t, curModbusItem->setPtr)(VarTampon.UInt0);
							break;

#ifdef MODBUS_SLAVE_SUPPORT_LONG_INT32	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Signed Long (4 Bytes = 2x 16 bits) :
						case TVarSLongGetVarSetVar:
						case TVarSLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int32_t, curModbusItem->setPtr) = VarTampon.SLong0;
							break;
						case TVarSLongGetVarSetFct:
						case TVarSLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int32_t, curModbusItem->setPtr)(VarTampon.SLong0);
							break;
							// Type Unsigned Long (4 Bytes = 2x 16 bits) :
						case TVarULongGetVarSetVar:
						case TVarULongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint32_t, curModbusItem->setPtr) = VarTampon.ULong0;
							break;
						case TVarULongGetVarSetFct:
						case TVarULongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint32_t, curModbusItem->setPtr)(VarTampon.ULong0);
							break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_INT32

#ifdef MODBUS_SLAVE_SUPPORT_LONG_LONG	// (cf. "ModbusSlaveConf.h")
                            //----------------------------------------
							// Type Signed LongLong (8 Bytes = 4x 16 bits) :
						case TVarSLongLongGetVarSetVar:
						case TVarSLongLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int64_t, curModbusItem->setPtr) = VarTampon.SLongLong;
							break;
						case TVarSLongLongGetVarSetFct:
						case TVarSLongLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int64_t, curModbusItem->setPtr)(VarTampon.SLongLong);
							break;

							// Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
						case TVarULongLongGetVarSetVar:
						case TVarULongLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint64_t, curModbusItem->setPtr) = VarTampon.ULongLong;
							break;
						case TVarULongLongGetVarSetFct:
						case TVarULongLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint64_t, curModbusItem->setPtr)(VarTampon.ULongLong);
							break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Int x1 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX1GetVarSetVar:
						case TVarFloatIntX1GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 1.0f);
							break;
						case TVarFloatIntX1GetVarSetFct:
						case TVarFloatIntX1GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 1.0f);
							break;

							// Type Float Int x10 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX10GetVarSetVar:
						case TVarFloatIntX10GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 10.0f);
							break;
						case TVarFloatIntX10GetVarSetFct:
						case TVarFloatIntX10GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 10.0f);
							break;

							// Type Float Int x100 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX100GetVarSetVar:
						case TVarFloatIntX100GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 100.0f);
							break;
						case TVarFloatIntX100GetVarSetFct:
						case TVarFloatIntX100GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 100.0f);
							break;

							// Type Float Int x1000 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX1000GetVarSetVar:
						case TVarFloatIntX1000GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 1000.0f);
							break;
						case TVarFloatIntX1000GetVarSetFct:
						case TVarFloatIntX1000GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 1000.0f);
							break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Long x10 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX10GetVarSetVar:
						case TVarFloatLongX10GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 10.0f);
							break;
						case TVarFloatLongX10GetVarSetFct:
						case TVarFloatLongX10GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 10.0f);
							break;

							// Type Float Long x100 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX100GetVarSetVar:
						case TVarFloatLongX100GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 100.0f);
							break;
						case TVarFloatLongX100GetVarSetFct:
						case TVarFloatLongX100GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 100.0f);
							break;

							// Type Float Long x1000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX1KGetVarSetVar:
						case TVarFloatLongX1KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 1000.0f);
							break;
						case TVarFloatLongX1KGetVarSetFct:
						case TVarFloatLongX1KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 1000.0f);
							break;

							// Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX10KGetVarSetVar:
						case TVarFloatLongX10KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 10000.0f);
							break;
						case TVarFloatLongX10KGetVarSetFct:
						case TVarFloatLongX10KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 10000.0f);
							break;

							// Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX100KGetVarSetVar:
						case TVarFloatLongX100KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 100000.0f);
							break;
						case TVarFloatLongX100KGetVarSetFct:
						case TVarFloatLongX100KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 100000.0f);
							break;

							// Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX1MGetVarSetVar:
						case TVarFloatLongX1MGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 1000000.0f);
							break;
						case TVarFloatLongX1MGetVarSetFct:
						case TVarFloatLongX1MGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 1000000.0f);
							break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Long Raw (4 Bytes = 2x 16 bits) :
						case TVarFloatLongRawGetVarSetVar:
						case TVarFloatLongRawGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = VarTampon.Float0;
							break;
						case TVarFloatLongRawGetVarSetFct:
						case TVarFloatLongRawGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)(VarTampon.Float0);
							break;

							// Type Double LongLong Raw (4 Bytes = 2x 16 bits) :
						case TVarDoubleLongLongRawGetVarSetVar:
						case TVarDoubleLongLongRawGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(double, curModbusItem->setPtr) = VarTampon.Double;
							break;
						case TVarDoubleLongLongRawGetVarSetFct:
						case TVarDoubleLongLongRawGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(double, curModbusItem->setPtr)(VarTampon.Double);
							break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

						case TVarUnknown:
						default:
							break;
						}
					}
				}
			}
			if(idRegister == (curModbusItem->Adr + curModBusItmSize -1))
			{
				curModbusItem++;
			}
			break;
		} else {
			curModbusItem++;
		}
	}

//    *retValue = nbBytes;	// Déjà à 0 si Broadcast OU Buffer insuffisant !
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SLAVE_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS)	// (cf. "ModbusSlaveConf.h")

static int HandleModbusFunctionWriteMultipleRegisters(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
#define INDEX_WRITE_REGISTERS_FIRST_VALUE	(MODBUS_SLAVE_HEADER_SIZE + 2 + 2 + 1)	// idRegister = 2, nbRegister = 2, byteCount = 1 => 7
#define MIN_WRITE_REGISTERS_RX_FRAME_SIZE	(INDEX_WRITE_REGISTERS_FIRST_VALUE + 2 + MODBUS_SLAVE_FOOTER_SIZE) // Data >= 2 => 11
#define MIN_WRITE_REGISTERS_TX_FRAME_SIZE	(MODBUS_SLAVE_HEADER_SIZE + 2 + 2 + MODBUS_SLAVE_FOOTER_SIZE)	// idRegister = 2, nbRegister = 2 => 8
#define MAX_WRITE_REGISTERS_ONCE	124	// Officiellement, la limite devrait être 123 (cf. "Modbus_Application_Protocol_V1_1b3.pdf" page 30)

	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if(MODBUS_SLAVE_FCT_WRITE_REG != myRx[1]) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	if(pRxTxBI->RxBuf.nbBytes < MIN_WRITE_REGISTERS_RX_FRAME_SIZE) { *retValue = MODBUS_SLAVE_ERR_ILL_DATA; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; }		// Taille Trame Insuffisante

// inutile	*retValue = 0; // Par défaut : rien à  Répondre
	if(pRxTxBI->TxBuf.maxBytes < MIN_WRITE_REGISTERS_TX_FRAME_SIZE) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

#ifdef MODBUS_SLAVE_SUPPORT_STATS
	if(UINT32_MAX > pModbusSlave->nbFramesWrit16) { pModbusSlave->nbFramesWrit16++; } // Pour les Stats
#endif // MODBUS_SLAVE_SUPPORT_STATS

	// Récupère les Paramètres :
	uint16_t idRegister = MODBUS_MAKE_WORD_BE(myRx[2], myRx[3]); // HighByte @2, LowByte @ 3
	uint16_t nbRegister = MODBUS_MAKE_WORD_BE(myRx[4], myRx[5]); // HighByte @4, LowByte @ 5
	if(0 == nbRegister) { nbRegister = myRx[6] >> 1; }	// Au cas où nbRegister soit caché uniquement dans le nb de Bytes !

	if(pRxTxBI->RxBuf.nbBytes < (INDEX_WRITE_REGISTERS_FIRST_VALUE + (nbRegister <<1) + MODBUS_SLAVE_FOOTER_SIZE)) { *retValue = MODBUS_SLAVE_ERR_ILL_ADR; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Taille Trame Incorecte

#ifndef DISABLE_MAX_WRITE_REGISTERS_ONCE	// (cf. "ModbusSlaveConf.h")
	if(MAX_WRITE_REGISTERS_ONCE < nbRegister) { nbRegister = MAX_WRITE_REGISTERS_ONCE; } // Max 124 registres 16bits pour rentrer dans 256 Bytes
#endif // DISABLE_MAX_WRITE_REGISTERS_ONCE

	// Début de la Réponse :
	if( (MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) && (pRxTxBI->TxBuf.maxBytes >= MIN_WRITE_REGISTERS_TX_FRAME_SIZE) )
	{
		uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
		uint16_t nbBytes = 0;
		myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
		myTx[nbBytes++] = myRx[1];  // Code Fonction identique
		myTx[nbBytes++] = myRx[2];	// Starting Address Hi
		myTx[nbBytes++] = myRx[3];	// Starting Address Lo
#ifdef DISABLE_MAX_WRITE_REGISTERS_ONCE	// (cf. "ModbusSlaveConf.h")
		myTx[nbBytes++] = myRx[4];	// Quantity of Registers Hi
		myTx[nbBytes++] = myRx[5];	// Quantity of Registers Lo
#else // ! DISABLE_MAX_WRITE_REGISTERS_ONCE
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(nbRegister, 1); // HighByte first
		myTx[nbBytes++] = MODBUS_GET_BYTE_N(nbRegister, 0); // then LowByte
#endif // DISABLE_MAX_WRITE_REGISTERS_ONCE
	    *retValue = nbBytes;	// Renseigne déjà la taille finale de la Réponse !
	}

	// Variables spécifiques à l'opération :
	uint16_t Data16;
	uint16_t curModBusItmSize;
	tModbusSlaveQWordVar VarTampon;
	uint16_t IndexTrame = INDEX_WRITE_REGISTERS_FIRST_VALUE; // 7 sur MODBUS_FCT_WRITE

	// Parcours de la Table Modbus :
	tModbusSlaveItem* curModbusItem = (tModbusSlaveItem*)MODBUS_SLAVE_BASE_OF_TABLE; // Pointe la base du Tableau à parcourir
    while(0 < nbRegister)
    {
        Data16 = MODBUS_MAKE_WORD_BE(myRx[IndexTrame], myRx[IndexTrame +1]); // Recompose en BigEndian
        IndexTrame += 2;	// Chaque valeur Modbus est sur 16bits, soit 2 Bytes
	while( (curModbusItem < MODBUS_SLAVE_END_OF_TABLE) && (idRegister >= curModbusItem->Adr) )
	{
            curModBusItmSize = getThisModbusItemSizeW((eTVarGetSet)curModbusItem->typeGetSet);
		if( (idRegister >= curModbusItem->Adr) && (idRegister < (curModbusItem->Adr + curModBusItmSize)) )
		{
			if(0 != curModbusItem->setPtr) // On a un Pointeur d'accès en Ecriture :
			{
				if(curModbusItem->writeMinLevel <= pModbusSlave->AccessLevel)
				{
					// Etape 1 : Mise en Mémoire Tampon :
					VarTampon.Word[(curModBusItmSize - (idRegister - curModbusItem->Adr) -1)] = Data16;

					// Etape 2 : Ecrire si Valeur est Complète :
					if(idRegister == (curModbusItem->Adr + curModBusItmSize -1)) // AdrEnCours = LowWord
					{
						// Transfère la Data Complète :
						switch(curModbusItem->typeGetSet)
						{
							// Type Signed Char (1 Byte = 8 bits) :
						case TVarSCharGetVarSetVar:
						case TVarSCharGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int8_t, curModbusItem->setPtr) = VarTampon.SChar0;
							break;
						case TVarSCharGetVarSetFct:
						case TVarSCharGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int8_t, curModbusItem->setPtr)(VarTampon.SChar0);
							break;
							// Type Unsigned Char (1 Byte = 8 bits) :
						case TVarUCharGetVarSetVar:
						case TVarUCharGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint8_t, curModbusItem->setPtr) = VarTampon.UChar0;
							break;
						case TVarUCharGetVarSetFct:
						case TVarUCharGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint8_t, curModbusItem->setPtr)(VarTampon.UChar0);
							break;

							// Type Signed Int (2 Bytes = 1x 16 bits) :
						case TVarSIntGetVarSetVar:
						case TVarSIntGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int16_t, curModbusItem->setPtr) = VarTampon.SInt0;
							break;
						case TVarSIntGetVarSetFct:
						case TVarSIntGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int16_t, curModbusItem->setPtr)(VarTampon.SInt0);
							break;
							// Type Unsigned Int (2 Bytes = 1x 16 bits) :
						case TVarUIntGetVarSetVar:
						case TVarUIntGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint16_t, curModbusItem->setPtr) = VarTampon.UInt0;
							break;
						case TVarUIntGetVarSetFct:
						case TVarUIntGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint16_t, curModbusItem->setPtr)(VarTampon.UInt0);
							break;

#ifdef MODBUS_SLAVE_SUPPORT_LONG_INT32	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Signed Long (4 Bytes = 2x 16 bits) :
						case TVarSLongGetVarSetVar:
						case TVarSLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int32_t, curModbusItem->setPtr) = VarTampon.SLong0;
							break;
						case TVarSLongGetVarSetFct:
						case TVarSLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int32_t, curModbusItem->setPtr)(VarTampon.SLong0);
							break;
							// Type Unsigned Long (4 Bytes = 2x 16 bits) :
						case TVarULongGetVarSetVar:
						case TVarULongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint32_t, curModbusItem->setPtr) = VarTampon.ULong0;
							break;
						case TVarULongGetVarSetFct:
						case TVarULongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint32_t, curModbusItem->setPtr)(VarTampon.ULong0);
							break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_INT32

#ifdef MODBUS_SLAVE_SUPPORT_LONG_LONG	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Signed LongLong (8 Bytes = 4x 16 bits) :
						case TVarSLongLongGetVarSetVar:
						case TVarSLongLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(int64_t, curModbusItem->setPtr) = VarTampon.SLongLong;
							break;
						case TVarSLongLongGetVarSetFct:
						case TVarSLongLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(int64_t, curModbusItem->setPtr)(VarTampon.SLongLong);
							break;

							// Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
						case TVarULongLongGetVarSetVar:
						case TVarULongLongGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(uint64_t, curModbusItem->setPtr) = VarTampon.ULongLong;
							break;
						case TVarULongLongGetVarSetFct:
						case TVarULongLongGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(uint64_t, curModbusItem->setPtr)(VarTampon.ULongLong);
							break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Int x1 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX1GetVarSetVar:
						case TVarFloatIntX1GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 1.0f);
							break;
						case TVarFloatIntX1GetVarSetFct:
						case TVarFloatIntX1GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 1.0f);
							break;

							// Type Float Int x10 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX10GetVarSetVar:
						case TVarFloatIntX10GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 10.0f);
							break;
						case TVarFloatIntX10GetVarSetFct:
						case TVarFloatIntX10GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 10.0f);
							break;

							// Type Float Int x100 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX100GetVarSetVar:
						case TVarFloatIntX100GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 100.0f);
							break;
						case TVarFloatIntX100GetVarSetFct:
						case TVarFloatIntX100GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 100.0f);
							break;

							// Type Float Int x1000 (2 Bytes = 1x 16 bits) :
						case TVarFloatIntX1000GetVarSetVar:
						case TVarFloatIntX1000GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SInt0 / 1000.0f);
							break;
						case TVarFloatIntX1000GetVarSetFct:
						case TVarFloatIntX1000GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SInt0 / 1000.0f);
							break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Long x10 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX10GetVarSetVar:
						case TVarFloatLongX10GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 10.0f);
							break;
						case TVarFloatLongX10GetVarSetFct:
						case TVarFloatLongX10GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 10.0f);
							break;

							// Type Float Long x100 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX100GetVarSetVar:
						case TVarFloatLongX100GetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 100.0f);
							break;
						case TVarFloatLongX100GetVarSetFct:
						case TVarFloatLongX100GetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 100.0f);
							break;

							// Type Float Long x1000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX1KGetVarSetVar:
						case TVarFloatLongX1KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 1000.0f);
							break;
						case TVarFloatLongX1KGetVarSetFct:
						case TVarFloatLongX1KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 1000.0f);
							break;

							// Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX10KGetVarSetVar:
						case TVarFloatLongX10KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 10000.0f);
							break;
						case TVarFloatLongX10KGetVarSetFct:
						case TVarFloatLongX10KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 10000.0f);
							break;

							// Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX100KGetVarSetVar:
						case TVarFloatLongX100KGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 100000.0f);
							break;
						case TVarFloatLongX100KGetVarSetFct:
						case TVarFloatLongX100KGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 100000.0f);
							break;

							// Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
						case TVarFloatLongX1MGetVarSetVar:
						case TVarFloatLongX1MGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = ((float)VarTampon.SLong0 / 1000000.0f);
							break;
						case TVarFloatLongX1MGetVarSetFct:
						case TVarFloatLongX1MGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)((float)VarTampon.SLong0 / 1000000.0f);
							break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW	// (cf. "ModbusSlaveConf.h")
							//----------------------------------------
							// Type Float Long Raw (4 Bytes = 2x 16 bits) :
						case TVarFloatLongRawGetVarSetVar:
						case TVarFloatLongRawGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(float, curModbusItem->setPtr) = VarTampon.Float0;
							break;
						case TVarFloatLongRawGetVarSetFct:
						case TVarFloatLongRawGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(float, curModbusItem->setPtr)(VarTampon.Float0);
							break;

							// Type Double LongLong Raw (4 Bytes = 2x 16 bits) :
						case TVarDoubleLongLongRawGetVarSetVar:
						case TVarDoubleLongLongRawGetFctSetVar:
							MODBUS_MAKE_CASTED_POINTED_VAR(double, curModbusItem->setPtr) = VarTampon.Double;
							break;
						case TVarDoubleLongLongRawGetVarSetFct:
						case TVarDoubleLongLongRawGetFctSetFct:
							MODBUS_MAKE_CASTED_POINTED_FCT(double, curModbusItem->setPtr)(VarTampon.Double);
							break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

						case TVarUnknown:
						default:
							break;
						}
					}
				}
			}
			if(idRegister == (curModbusItem->Adr + curModBusItmSize -1))
			{
				curModbusItem++;
			}
			break;
		} else {
			curModbusItem++;
		}
	}
        idRegister++;
        nbRegister--;
    }

//	// Finaliser la Réponse :
//	if(MODBUS_SLAVE_BROADCAST_ALL_ADR == myRx[0]) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Broadcast => Commands handled, but no reply to send

//	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
//	uint16_t nbBytes = 0;
//	myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
//	myTx[nbBytes++] = myRx[1];  // Code Fonction identique
//	myTx[nbBytes++] = myRx[2];
//	myTx[nbBytes++] = myRx[3];
//	myTx[nbBytes++] = myRx[4];
//	myTx[nbBytes++] = myRx[5];

//    *retValue = nbBytes;	// Déjà à 0 si Broadcast OU Buffer insuffisant !
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SUPPORT_WRITE_MULTIPLE_REGISTERS

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY)

static int HandleModbusFunctionDumpExternalMemory(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
// inutile	*retValue = 0; // Par défaut : rien à répondre
	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if( (MODBUS_FCT_READ_EXT_MEM != myRx[1]) || (MODBUS_SLAVE_BROADCAST_ALL_ADR == myRx[0]) ) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	if(pRxTxBI->TxBuf.maxBytes < 7) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

	// Vérification du Protocole :
	if(pRxTxBI->RxBuf.nbBytes != (2 +4 +2 +2)) { *retValue = MODBUS_SLAVE_ERR_ILL_FN; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourne une Exception

	// Récupère les Paramètres :
	uint32_t AdresseMem = MODBUS_MAKE_DWORD_BE(myRx[2], myRx[3], myRx[4], myRx[5]); // HighByte @2 ... LowByte @ 5
	uint16_t nbOctets	= MODBUS_MAKE_WORD_BE(myRx[6], myRx[7]); // HighByte @6, LowByte @ 7

	// Vérification des Paramètres :
	*retValue = MODBUS_SLAVE_ERR_ILL_ADR; // Par défaut, s'il y a une Exception, ce sera un problème d'Adresse !
#if MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR < UINT32_MAX // Filtrage pour éviter une alerte CodeSonar :
	if(AdresseMem > MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if((AdresseMem + nbOctets) > (MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR +1)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
#else // MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR == UINT32_MAX :
	if(AdresseMem > ((uint32_t)0 - (uint32_t)nbOctets)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
#endif // MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR <= UINT32_MAX
	if(pRxTxBI->TxBuf.maxBytes < (nbOctets +8 +2)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	// Préparation de la Réponse :
	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
	uint16_t nbBytes = 0;
	do
	{
		myTx[nbBytes] = myRx[nbBytes];
	} while(++nbBytes < 8);

	// Récupération du Contenu demandé :
	MODBUS_SLAVE_CALLER_ReadArrayFromExternalMemory(AdresseMem, nbOctets, (void*)&myTx[nbBytes]);
	nbBytes += nbOctets;

	// Finalisation des Paramètres de sortie :
	*retValue = nbBytes;
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE)

static int HandleModbusFunctionWriteExternalRessource(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
// inutile	*retValue = 0; // Par défaut : rien à répondre
	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if(MODBUS_FCT_WRITE_EXT_RES != myRx[1]) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	if(pRxTxBI->TxBuf.maxBytes < 7) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

	// Récupère les paramètres ([2:3] = Offset BigEndian en Words ; [4:5] = NbBytes en BigEndian):
	uint32_t AdresseMem	= MODBUS_MAKE_WORD_BE(myRx[2], myRx[3]) <<1; // Conversion immédiate Words -> Offset en Bytes
	uint16_t nbOctets	= MODBUS_MAKE_WORD_BE(myRx[4], myRx[5]); // HighByte @6, LowByte @ 7

	// Vérification des Paramètres :
	*retValue = MODBUS_SLAVE_ERR_ILL_ADR; // Par défaut, s'il y a une Exception, ce sera un problème d'Adresse !
	if(AdresseMem >= MODBUS_SLAVE_GETTER_EXT_RES_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if((AdresseMem + nbOctets) > MODBUS_SLAVE_GETTER_EXT_RES_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	//uint16_t nbRx = pRxTxBI->RxBuf.nbBytes;
	if(pRxTxBI->RxBuf.nbBytes != (2 +2 +2 + nbOctets +2)) { *retValue = MODBUS_SLAVE_ERR_ILL_DATA; return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	uint8_t Result =
#ifdef USE_EXTERNAL_RES // from "ExternalRes.h" via "IhmRes.h :
							ImportRessourceBlocFromBuf(AdresseMem, nbOctets, &myRx[6], getThisModbusSrcId()); // Requires Library "IhmRes"
#else // !USE_EXTERNAL_RES :
							UINT8_MAX; // Impossible to Handle => Failure !
#endif // USE_EXTERNAL_RES

	// Préparation & Réponse :
	uint16_t nbBytes = 0;	// Par défaut : rien à répondre
	if( (pRxTxBI->TxBuf.maxBytes >= (3 +2)) && (MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) )
	{
		uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
		myTx[nbBytes++] = myRx[0];	// Destinataire -> Emmeteur
		myTx[nbBytes++] = 0x99;		// Code Fonction Spécial
		myTx[nbBytes++] = Result;	// Résultat : 1 = OK; 255 = Fail
	}

	// Finalisation des Paramètres de sortie :
	*retValue = nbBytes;
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC)

static int HandleModbusFunctionWriteFirmwareBloc(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
	// inutile	*retValue = 0; // Par défaut : rien à répondre
	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if(MODBUS_FCT_FLASH_FIRMWARE != myRx[1]) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	uint16_t thisModbusSrcId = getThisModbusSrcId();

	// Activer le Verrouillage sur la Source de la Mise à Jour :
	if(MODBUS_SLAVE_GETTER_FLASH_SRC_LOCKED != MODBUS_SLAVE_CALLER_LockThisFlashProgramSrc(thisModbusSrcId))
	{
		*retValue = MODBUS_SLAVE_ERR_ILL_FN; // Refuser d'exécuter la Fonction
		return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; // Retourner une Exception
	}

	// Récupère les Paramètres :
	uint32_t AdresseFlash = MODBUS_MAKE_DWORD_BE(myRx[2], myRx[3], myRx[4], myRx[5]); // HighByte @2 ... LowByte @ 5
	uint16_t nbOctets	  = MODBUS_MAKE_WORD_BE(myRx[6], myRx[7]); // HighByte @6, LowByte @ 7

	// Check for Illegal Address :
	*retValue = MODBUS_SLAVE_ERR_ILL_ADR; // Par défaut, s'il y a une Exception, ce sera un problème d'Adresse !
	if( (0 != AdresseFlash) && (MODBUS_SLAVE_GETTER_FLASH_SRC_LOCKED != MODBUS_SLAVE_CALLER_CanFlashProgramFromSrc(thisModbusSrcId))) // Check PndgFlashTimeOut
	{
		return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; // Retourner une Exception
	}
	if(AdresseFlash >= MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if((AdresseFlash + nbOctets) >= MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	// Check for Illegal Datas :
	*retValue = MODBUS_SLAVE_ERR_ILL_DATA; // Par défaut, s'il y a une Exception, ce sera un problème de Datas !
	if( (0 == AdresseFlash) && (nbOctets < sizeof(uint32_t)) ) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if(pRxTxBI->RxBuf.nbBytes != (2 +4 +2 + nbOctets +2)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	// Ecrire le Contenu reçu :
	uint16_t Result = MODBUS_SLAVE_CALLER_WriteFirmwareBlocFromBuf(AdresseFlash, nbOctets, &myRx[8], thisModbusSrcId);

	// Préparation & Réponse :
	uint16_t nbBytes = 0;	// Par défaut : rien à répondre
	if( (pRxTxBI->TxBuf.maxBytes >= (3 +2)) && (MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) )
	{
		uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
		myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
		myTx[nbBytes++] = myRx[1];  // Code Fonction identique : MODBUS_FCT_FLASH_FIRMWARE
		myTx[nbBytes++] = (UPGRADE_ERROR_NO_ERROR == Result) ? (MODBUS_SLAVE_PROGRAM_SUCCESS & 0xFF) : (uint8_t)Result;
	}

	// Finalisation des Paramètres de sortie :
	*retValue = nbBytes;
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE)

static int HandleModbusFunctionValidateFirmwareProgram(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
	// inutile	*retValue = 0; // Par défaut : rien à répondre
	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if(MODBUS_FCT_CHECK_FIRMWARE != myRx[1]) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled
	uint16_t thisModbusSrcId = getThisModbusSrcId();

	// Check for Illegal Function :
	if(MODBUS_SLAVE_GETTER_FLASH_SRC_LOCKED != MODBUS_SLAVE_CALLER_CanFlashProgramFromSrc(thisModbusSrcId))
	{
		*retValue = MODBUS_SLAVE_ERR_ILL_FN; // Refuser d'exécuter la Fonction
		return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; // Retourner une Exception
	}

	// Récupère les Paramètres :
	uint32_t AdresseFlash = MODBUS_MAKE_DWORD_BE(myRx[2], myRx[3], myRx[4], myRx[5]); // HighByte @2 ... LowByte @ 5
	uint32_t nbOctetsLong = MODBUS_MAKE_DWORD_BE(myRx[6], myRx[7], myRx[8], myRx[9]); // HighByte @6 ... LowByte @ 9

	// Check for Illegal Datas :
	*retValue = MODBUS_SLAVE_ERR_ILL_DATA; // Par défaut, s'il y a une Exception, ce sera un problème de Datas !
	if(nbOctetsLong < MODBUS_SLAVE_GETTER_CHECK_FIMWARE_MIN_BLOC_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if(pRxTxBI->RxBuf.nbBytes != (2 +4 +4 +MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE +2)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	// Vérification du Contenu demandé :
#if (MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE == 4) // 4 <=> sizeof(uint32_t)))
#warning "La partie 'HandleModbusValidateFirmware' via CRC32 n'a pas encore ete validee (au 18/10/2023) !"
	uint32_t expectedValue = MODBUS_MAKE_DWORD_LE(myRx[10], myRx[11], myRx[12], myRx[13]); // 32bits LittleEndian
//#elif (MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE == 2) // 2 <=> sizeof(uint16_t)
#else // MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE
	uint16_t expectedValue = MODBUS_MAKE_WORD_LE(myRx[10], myRx[11]);	// 16bits LittleEndian
#endif // MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE
	uint16_t Result = MODBUS_SLAVE_CALLER_ValidateFirmwareProgram(AdresseFlash, nbOctetsLong, &expectedValue, thisModbusSrcId);

	// Préparation & Réponse :
	uint16_t nbBytes = 0;	// Par défaut : rien à répondre
	if( (pRxTxBI->TxBuf.maxBytes >= (3 +2)) && (MODBUS_SLAVE_BROADCAST_ALL_ADR != myRx[0]) )
	{
		uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
		myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
		myTx[nbBytes++] = myRx[1];  // Code Fonction identique : MODBUS_FCT_CHECK_FIRMWARE
		myTx[nbBytes++] = (UPGRADE_ERROR_NO_ERROR == Result) ? (MODBUS_SLAVE_PROGRAM_SUCCESS & 0xFF) : (uint8_t)Result;
	}

	// Finalisation des Paramètres de sortie :
	*retValue = nbBytes;
	UartCom_RequestDelay4Reply(pRxTxBI->hHandle, 0); // Cas particulier de la Vérification, toujours trop longue pour le Modbus standard !
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SUPPORT && MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE

/******************************************************************************/

#if !defined(DISABLE_MODBUS_SUPPORT) && defined(MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM)

static int HandleModbusFunctionReadInternalProgram(tRxTxBufInfo* pRxTxBI, tModbusSlaveParams* pModbusSlave, int *retValue)
{
	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	if( (MODBUS_FCT_READ_INT_PRGM != myRx[1]) || (MODBUS_SLAVE_BROADCAST_ALL_ADR == myRx[0]) ) { return MODBUS_SLAVE_FRAME_NOT_HANDLED; } // Not Handled

// inutile	*retValue = 0; // Par défaut : rien à répondre
	if(pRxTxBI->TxBuf.maxBytes < 7) { return MODBUS_SLAVE_FRAME_WAS_HANDLED; } // Handled, mais Impossible de Préparer une Réponse !

	// Récupère les Paramètres :
	uint32_t AdresseFlash = MODBUS_MAKE_DWORD_BE(myRx[2], myRx[3], myRx[4], myRx[5]); // HighByte @2 ... LowByte @ 5
	uint16_t nbOctets	  = MODBUS_MAKE_WORD_BE(myRx[6], myRx[7]); // HighByte @6, LowByte @ 7

	// Check for Illegal Address :
	*retValue = MODBUS_SLAVE_ERR_ILL_ADR; // Par défaut, s'il y a une Exception, ce sera un problème d'Adresse !
	if(AdresseFlash >= MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if((AdresseFlash + nbOctets) > MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception
	if(pRxTxBI->TxBuf.maxBytes < (nbOctets +2 +2 +2)) { return MODBUS_SLAVE_FRAME_WAS_EXCEPTION; } // Retourner une Exception

	// Prépare la réponse :
	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
	uint16_t nbBytes = 0;
	myTx[nbBytes++] = myRx[0];  // Destinataire -> Emmeteur
	myTx[nbBytes++] = myRx[1];  // Code Fonction identique : MODBUS_FCT_READ_INT_PRGM
	myTx[nbBytes++] = MODBUS_GET_BYTE_N(nbOctets, 1); // HighByte first
	myTx[nbBytes++] = MODBUS_GET_BYTE_N(nbOctets, 0); // then LowByte
	AdresseFlash += MODBUS_SLAVE_GETTER_FIRMWARE_BASE_ADR;
	while(nbOctets > 0)
	{
		myTx[nbBytes++] = *(uint8_t*)AdresseFlash; // Lecture @ Adresse demandée
		AdresseFlash++;
		nbOctets--;
	}

	*retValue = nbBytes;
	return MODBUS_SLAVE_FRAME_WAS_HANDLED; // Traitement Terminé avec succès
}

#endif // !DISABLE_MODBUS_SLAVE_SUPPORT && MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

/******************************************************************************/

uint16_t getCurAccessLevel(void)
{
	if(0 == pCurModbus) { return 0; }
	return pCurModbus->AccessLevel;
}

/******************************************************************************/

uint16_t getThisModbusSrcId(void)
{
	if(0 == pCurModbus) { return UINT16_MAX; }
	return pCurModbus->SrcId;
}

/******************************************************************************/

uint16_t getThisModbusRxBufSize(void)
{
	if(0 == pCurRxTxBI) { return 0; }
	return getThisHandleRxBufSize(pCurRxTxBI->hHandle);
}

/******************************************************************************/

uint16_t getThisModbusTxBufSize(void)
{
	if(0 == pCurRxTxBI) { return 0; }
	return pCurRxTxBI->TxBuf.maxBytes;
}

/******************************************************************************/

static uint16_t getThisModbusItemSizeW(eTVarGetSet varType)
{

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT

    switch(varType)
        {
            // Type Signed Char (1 Byte = 8 bits) :
        case TVarSCharGetVarSetVar:
        case TVarSCharGetVarSetFct:
        case TVarSCharGetFctSetVar:
        case TVarSCharGetFctSetFct:
            // Type Unsigned Char (1 Byte = 8 bits) :
        case TVarUCharGetVarSetVar:
        case TVarUCharGetVarSetFct:
        case TVarUCharGetFctSetVar:
        case TVarUCharGetFctSetFct:
            return 1;   // Même si ça remplit pas tout le Word en entier
            break;

            // Type Signed Int (2 Bytes = 1x 16 bits) :
        case TVarSIntGetVarSetVar:
        case TVarSIntGetVarSetFct:
        case TVarSIntGetFctSetVar:
        case TVarSIntGetFctSetFct:
            // Type Unsigned Int (2 Bytes = 1x 16 bits) :
        case TVarUIntGetVarSetVar:
        case TVarUIntGetVarSetFct:
        case TVarUIntGetFctSetVar:
        case TVarUIntGetFctSetFct:
            return 1;
            break;

#ifdef MODBUS_SLAVE_SUPPORT_LONG_INT32	// (cf. "ModbusSlaveConf.h")
            //----------------------------------------
            // Type Signed Long (4 Bytes = 2x 16 bits) :
        case TVarSLongGetVarSetVar:
        case TVarSLongGetVarSetFct:
        case TVarSLongGetFctSetVar:
        case TVarSLongGetFctSetFct:
            // Type Unsigned Long (4 Bytes = 2x 16 bits) :
        case TVarULongGetVarSetVar:
        case TVarULongGetVarSetFct:
        case TVarULongGetFctSetVar:
        case TVarULongGetFctSetFct:
            return 2;
            break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_INT32

#ifdef MODBUS_SLAVE_SUPPORT_LONG_LONG	// (cf. "ModbusSlaveConf.h")
            //----------------------------------------
            // Type Signed LongLong (8 Bytes = 4x 16 bits) :
        case TVarSLongLongGetVarSetVar:
        case TVarSLongLongGetVarSetFct:
        case TVarSLongLongGetFctSetVar:
        case TVarSLongLongGetFctSetFct:

            // Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
        case TVarULongLongGetVarSetVar:
        case TVarULongLongGetVarSetFct:
        case TVarULongLongGetFctSetVar:
        case TVarULongLongGetFctSetFct:
            return 4;
            break;
#endif // MODBUS_SLAVE_SUPPORT_LONG_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")
            //----------------------------------------
            // Type Float Int x1 (2 Bytes = 1x 16 bits) :
        case TVarFloatIntX1GetVarSetVar:
        case TVarFloatIntX1GetVarSetFct:
        case TVarFloatIntX1GetFctSetVar:
        case TVarFloatIntX1GetFctSetFct:
            return 1;
            break;
            // Type Float Int x10 (2 Bytes = 1x 16 bits) :
        case TVarFloatIntX10GetVarSetVar:
        case TVarFloatIntX10GetVarSetFct:
        case TVarFloatIntX10GetFctSetVar:
        case TVarFloatIntX10GetFctSetFct:
            return 1;
            break;

            // Type Float Int x100 (2 Bytes = 1x 16 bits) :
        case TVarFloatIntX100GetVarSetVar:
        case TVarFloatIntX100GetVarSetFct:
        case TVarFloatIntX100GetFctSetVar:
        case TVarFloatIntX100GetFctSetFct:
            return 1;
            break;

            // Type Float Int x1000 (2 Bytes = 1x 16 bits) :
        case TVarFloatIntX1000GetVarSetVar:
        case TVarFloatIntX1000GetVarSetFct:
        case TVarFloatIntX1000GetFctSetVar:
        case TVarFloatIntX1000GetFctSetFct:
            return 1;
            break;
#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")
        	//----------------------------------------
        	// Type Float Long x10 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX10GetVarSetVar:
        case TVarFloatLongX10GetVarSetFct:
        case TVarFloatLongX10GetFctSetVar:
        case TVarFloatLongX10GetFctSetFct:
            return 2;
            break;

        	// Type Float Long x100 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX100GetVarSetVar:
        case TVarFloatLongX100GetVarSetFct:
        case TVarFloatLongX100GetFctSetVar:
        case TVarFloatLongX100GetFctSetFct:
            return 2;
            break;

        	// Type Float Long x1000 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX1KGetVarSetVar:
        case TVarFloatLongX1KGetVarSetFct:
        case TVarFloatLongX1KGetFctSetVar:
        case TVarFloatLongX1KGetFctSetFct:
            return 2;
            break;

        	// Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX10KGetVarSetVar:
        case TVarFloatLongX10KGetVarSetFct:
        case TVarFloatLongX10KGetFctSetVar:
        case TVarFloatLongX10KGetFctSetFct:
            return 2;
            break;

        	// Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX100KGetVarSetVar:
        case TVarFloatLongX100KGetVarSetFct:
        case TVarFloatLongX100KGetFctSetVar:
        case TVarFloatLongX100KGetFctSetFct:
            return 2;
            break;

        	// Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
        case TVarFloatLongX1MGetVarSetVar:
        case TVarFloatLongX1MGetVarSetFct:
        case TVarFloatLongX1MGetFctSetVar:
        case TVarFloatLongX1MGetFctSetFct:
            return 2;
            break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW	// (cf. "ModbusSlaveConf.h")
        	//----------------------------------------
        	// Type Float Long Raw (4 Bytes = 2x 16 bits) :
        case TVarFloatLongRawGetVarSetVar:
        case TVarFloatLongRawGetVarSetFct:
        case TVarFloatLongRawGetFctSetVar:
        case TVarFloatLongRawGetFctSetFct:
            return 2;
            break;

        	// Type Double LongLong Raw (8 Bytes = 4x 16 bits) :
        case TVarDoubleLongLongRawGetVarSetVar:
        case TVarDoubleLongLongRawGetVarSetFct:
        case TVarDoubleLongLongRawGetFctSetVar:
        case TVarDoubleLongLongRawGetFctSetFct:
            return 4;
            break;

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

        case TVarUnknown:
        default:
            return 0;
            break;
        }

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

    return 0;
}

/******************************************************************************/

void RequestRunMode4ThisModbus(uint8_t newRunMode)
{
#if defined(MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC)
	HandleRequest4NewRunMode(newRunMode, getThisModbusSrcId());
#else
	UNUSED(newRunMode);	// Pour éviter le Warning de la Variable non utilisée
#endif // MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC
}

/******************************************************************************/

void RegisterNewAccessPswd(uint16_t NewPswd)
{

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT
	if(0 != pCurModbus)
	{
		if(10 > pCurModbus->nbPswdTries)
		{
			pCurModbus->nbPswdTries++;
			pCurModbus->AccessLevel = getAccessLevelAccordingToPswd(NewPswd);
		}
	}
#endif // DISABLE_MODBUS_SLAVE_SUPPORT

}

// ToDo : Remarque_Jp le 05/01/2022 : Voir pour déplacer éventuellement la liste des codes dans la partie User ...

// volontairement limite a 6 niveaux d'acces car les autres ne sont pas utilises pour l'instant
#define NB_NIVEAUX_EFFECTIVEMENT_UTILISES 		6
const uint16_t accessPswdTable[NB_NIVEAUX_EFFECTIVEMENT_UTILISES] =
{
	0,		// Level 0 : Normal & Default Access
	9781,	// Level 1
	5476,	// Level 2 : Configurator
	32184,	// Level 3 : IHM du Produit
	2794,	// Level 4 : Banc de Test fin de chaîne ALDES
	941		// Level 5 : Banc de Test sous-traitant carte nue
};

/******************************************************************************/

static inline uint16_t getAccessLevelAccordingToPswd(uint16_t Pswd)
{
	int lvl = 0;
	uint16_t* pPwdTable = (uint16_t*)MODBUS_SLAVE_BASE_OF_PSWD; //accessPswdTable;
	while(0 != *pPwdTable)
	{
		if(++lvl > ACCESS_MIN_LEVEL_MAX) return 0;	// OutOfLevel
		if(*pPwdTable++ == Pswd) return lvl;
	}

	// Si on passe ici c'est qu'on n'a pas trouve notre bonheur dans la Table
	return 0;
}


/******************************************************************************/

void SaveParamsOnOrder(uint16_t Order2Save)
{
#define SAVE_FACT_PARAMS_NOW	0x7934	// Save Factory Params now (0x7934 = 31028)
#define SAVE_USER_PARAMS_NOW	0x5831	// Save User Params Now (0x5831 = 22577)
/*
	int targetFlags = 0;
	switch(Order2Save)
	{
	case SAVE_FACT_PARAMS_NOW:
		if(pCurModbus != 0)
		{
			targetFlags = (pCurModbus->AccessLevel >= ACCESS_MIN_LEVEL_4) ? 1 :0;
		} else {
			targetFlags = 1;
		}
		if(0 != (targetFlags & 1))
		{
			ForceSaveSystemParams(MEM_HISTO_FORCE_CAPTURE_NOW);
		} // ToDo: Ajouter l'info comme quoi l'opération se serait bien passée (ou pas) !
		break;
	case SAVE_USER_PARAMS_NOW:
		ForceCaptureStats2(MEM_HISTO_FORCE_CAPTURE_NOW | MEM_HISTO_RESYNC_AUTO_CAPTURE);
		targetFlags |= 2;
		break;
	default:
		break;
	}
	UNUSED(targetFlags); // ToDo : Transférer le résultat "targetFlags" vers une variable RAM pour Modbus
*/
}

/******************************************************************************/

void handleQueryReInitFactory(uint16_t pswd)
{
#define CONFIG_RESET	0x4725	// RESET_TO_PRODUCT_READY
#define PRODUCT_RESET	0x9653	// RESET_TO_BOARD_READY
	int tmpFlags = (pswd == CONFIG_RESET) ? 1 : 0; // Uniquement le Niveau 1
	if(pCurModbus->AccessLevel >= ACCESS_MIN_LEVEL_4) // Niveau 4 requis !
	{
		if(pswd == PRODUCT_RESET) { tmpFlags = 3; } // RéInitialiser les Niveaux 1 & 2
	}
	if(tmpFlags != 0)
	{
//		doReInitFactory(tmpFlags);
//		ForceCaptureStats2(MEM_HISTO_FORCE_CAPTURE_NOW | MEM_HISTO_RESYNC_AUTO_CAPTURE);
	}
}

/******************************************************************************/

#ifndef UTILS_UTILS_H_	// en cas d'absence de "utils.h" :
inline void ModbusSlave_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt)
{
	while(BytesCt-- > 0) { *pDest++ = Value; }
}
#endif // UTILS_UTILS_H_

/******************************************************************************/

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_INT	// (cf. "ModbusSlaveConf.h")

static int16_t Float_to_Int16(float value, float coef)
{
#ifndef _MATH_H_
    return (int16_t)((value * coef) + 0.49f); // Ajoute une demi-portion d'entier pour "forcer" l'arrondi
#else // _MATH_H_
    return (int16_t)roundf(value * coef);	// Arrondi correct en valeur négative également !
#endif // _MATH_H_
}

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_INT

/******************************************************************************/

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// (cf. "ModbusSlaveConf.h")

static int32_t Float_to_Int32(float value, float coef)
{
#ifndef _MATH_H_
    return (int32_t)((value * coef) + 0.49f); // Ajoute une demi-portion d'entier pour "forcer" l'arrondi
#else // _MATH_H_
    return (int32_t)roundf(value * coef);	// Arrondi correct en valeur négative également !
#endif // _MATH_H_
}

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

/******************************************************************************/
// Misc Read discard Functions and Write discard Handlers :

uint16_t GetNullU16(void)
{
	return 0;
}
uint32_t GetNullU32(void)
{
	return 0;
}

void DiscardWriteU16(uint16_t __attribute__((unused)) NewValue)
{
	UNUSED(NewValue); // Avoid compilation warning
	return; // Do Nothing
}

/******************************************************************************/
// Mise à disposition Modbus des Identifiants & Infos spécifiques ST :

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT

#ifdef UID_BASE
	uint32_t GetUID_0(void) { return MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint32_t, (UID_BASE + (0 <<2))); }
	uint32_t GetUID_1(void) { return MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint32_t, (UID_BASE + (1 <<2))); }
	uint32_t GetUID_2(void) { return MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint32_t, (UID_BASE + (2 <<2))); }
	uint32_t GetUID_3(void) { return MODBUS_GET_CASTED_VALUE_FROM_POINTED_VAR(uint32_t, (UID_BASE + (3 <<2))); }
	//uint32_t GetUID_n(uint8_t nDWord) { return *(uint32_t *) (UID_BASE + (nDWord <<2)); }
#else // UID_BASE
	uint32_t GetUID_0(void) { return UINT32_MAX; }
	uint32_t GetUID_1(void) { return UINT32_MAX; }
	uint32_t GetUID_2(void) { return UINT32_MAX; }
	uint32_t GetUID_3(void) { return UINT32_MAX; }
#endif // UID_BASE

#ifdef FLASHSIZE_BASE
	uint16_t GetFlashSize(void) { return *(uint16_t *) (FLASHSIZE_BASE); }
#else // ! FLASHSIZE_BASE
	uint16_t GetFlashSize(void) { return UINT16_MAX; }
#endif // FLASHSIZE_BASE

#ifdef PACKAGE_BASE
	uint16_t GetPackageData(void) { return *(uint16_t *) (PACKAGE_BASE); }
#else // !PACKAGE_BASE :
	uint16_t GetPackageData(void) { return UINT16_MAX; }
#endif // PACKAGE_BASE

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

/******************************************************************************/
