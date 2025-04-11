/*
 * VersionInfos.c
 *
 *  Created on: 13 févr. 2020
 *  Original Author: j.proux
 *
 *  Updated on: 06 Fév. 2024
 *  Updated by: j.proux
 *
 */

#include "VersionInfos.h"	// Pour nos propres déclarations publiques
#include "FlashInfos.h"		// Pour accès aux Infos sur la Flash
//#include "main.h"			// Pour accès direct aux bons includes des HAL

#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_CRC)
	#include "CrcSoft.h"	// Pour le Calcul du CRC32 du Firmware et/ou BootLoader
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC

#ifdef FIRMWARE_IS_BOOTLOADER
	#include "UpgradeFirmware.h"
	#include "UpdateManager.h"
#endif // FIRMWARE_IS_BOOTLOADER


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/* USER CODE BEGIN Includes */

//#include "modbus.h" // Pour la Gestion des demandes de basculement en BootLoader
//#include "ModbusSlaveUtils.h" // Pour demander à Basculer en BootLoader

/* USER CODE END Includes */

/******************************************************************************/

#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
	#define VER_INFOS_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
#else // ! DEBUG :
	#define VER_INFOS_HALT_IF_DEBUG()	// Nop();
#endif // DEBUG

/******************************************************************************/

#define VERINFO_MAKE_UINT32_BE(h, m1, m0, l)    ((((h) & 0xFF)<<24) | (((m1) & 0xFF)<<16) | (((m0) & 0xFF)<<8) | (((l) & 0xFF) << 0)) // h = Byte3; m1 = Byte2; m0 = Byte1; l = Byte0
#define VERINFO_MAKE_UINT16_LE(l,h) 			((((h) & 0xFF)<<8) | (((l) & 0xFF) << 0)) // LittleEndian : h = Byte1; l = Byte0

#ifdef VI_SUPPORT_JUMP_BL
	uint16_t BL_Requesters = 0;	// Pour savoir qui a demandé le BootLoader
	#define BL_REQUESTER_FLAG_NO_APP	(1 << 0) // Flag réservé au Système

	uint16_t sabJumpToBL = 0;
	#define SAB_JUMP_TO_BL			150	// RunTime @ base 100ms => 150 = 15s
	#define SAB_RUN_TO_BL			50	// RunTime @ base 100ms =>  50 =  5s
	#define SAB_JUMP_BL_EXPIRED		1	// Expiré => To Do now
	#define SAB_JUMP_BL_DISABLED	0	// Désactivé => Ignorer
#endif // VI_SUPPORT_JUMP_BL

typedef enum
{
	eComputeStepNotInit = 0,

#if defined(FIRMWARE_IS_BOOTLOADER) && defined(VI_SUPPORT_APP_VERSION)
	eComputeStepInitFwVer,	// Uniquement dans le Soft BootLoader
	eComputeStepFindFwVer,	// Uniquement dans le Soft BootLoader
#endif // FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_APP_VERSION

#ifdef VI_SUPPORT_FW_CRC
	eComputeStepInitFwCRC,
	eComputeStepCalcFwCRC,
#endif // VI_SUPPORT_FW_CRC

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	eComputeStepInitBlVer,	// Uniquement dans le Soft Applicatif
	eComputeStepFindBlVer,	// Uniquement dans le Soft Applicatif
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#ifdef VI_SUPPORT_BL_CRC
	eComputeStepInitBlCRC,
	eComputeStepCalcBlCRC,
#endif // VI_SUPPORT_BL_CRC

#if defined(FIRMWARE_IS_BOOTLOADER)
	eComputeStepChkCanJump,	// Uniquement dans le Soft BootLoader
#endif // FIRMWARE_IS_BOOTLOADER

	eComputeStepNoAction,
	eComputeStepReInitNfo,	// Réinitialiser les Infos sur le FW déjà calculées
} eComputeFwStepId;

#define COMPUTE_STEP_SIZE	256 // Taille du Bloc CRC à parcourir à chaque pas
uint8_t  ComputeInfosStepId; // parmi eComputeFwStepId

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	uint16_t BlVersionInfo = UINT16_MAX;	// BootLoader Version (= 16bits), une fois détectée par l'App
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#if defined(FIRMWARE_IS_BOOTLOADER) && defined(VI_SUPPORT_APP_VERSION)
	uint32_t FwVersion32 = UINT32_MAX;	// Application Firmware Version, une fois détectée par BL
	uint16_t FwVersion16 = UINT16_MAX;
#endif // FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_APP_VERSION

#ifdef FIRMWARE_IS_BOOTLOADER
	uint16_t sabKeepInBL = SAB_STAY_BL_MST; // Temps à rester encore en BootLoader
#endif // FIRMWARE_IS_BOOTLOADER

#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_CRC) || defined(VI_SUPPORT_BL_VERSION)
	uint8_t* pComputeCurPos;
	uint32_t curComputedCRC32;
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC

#ifdef VI_SUPPORT_FW_CRC
	uint32_t FwComputedCRC32;
#endif // VI_SUPPORT_FW_CRC

#ifdef VI_SUPPORT_BL_CRC
	uint32_t BlComputedCRC32;
#endif // VI_SUPPORT_BL_CRC

	// Info publique attendue sur le Modbus :
	const uint8_t __attribute__((unused)) curRunMode =
#ifdef FIRMWARE_IS_APPLICATIF
	(uint8_t)(RUN_MODE_APP);
#elif defined(FIRMWARE_IS_BOOTLOADER) // ! FIRMWARE_IS_APPLICATIF && FIRMWARE_IS_BOOTLOADER :
	(uint8_t)(RUN_MODE_BL);
#else	// Indéterminé :
	UINT8_MAX; // => Non supporté !
#endif // FIRMWARE_IS_BOOTLOADER

/******************************************************************************/

#define VI_MAKE_VER_KEYWORD_HEADER(k)			{ '[', k }	// Juste l'En-tête
#define VI_MAKE_VER_KEYWORD_BE32(k,t,ma,mi,dev)	{ '[', k, t, ma, mi, dev, ']' } // This VersionKeyword includes Ver32 in BigEndian
#define VI_MAKE_VER_KEYWORD_LE16(k,v)			{ '[', k, ((v)>>0)&0xFF, ((v)>>8)&0xFF, ']' } // This VersionKeyword includes Ver16 in LittleEndian

#pragma GCC push_options
#pragma GCC optimize ("O0")	// Nécessaire pour que la variable ne soit pas supprimée

const uint8_t __attribute__((unused)) __attribute__((section(".ProductKey"))) ProductKey[4] = "TF4A"; // Identification Produit : 'TF4A' = TFlow 4 App ; 'TF4B' = TFlow 4 BootLoader
//const uint32_t __attribute__((unused)) __attribute__((section(".ProductKey"))) ProductKey = 0x41494948; // Equivalent DWORD32

uint32_t __attribute__((used)) GetVersionSoft(void)
{
#ifndef FIRMWARE_IS_BOOTLOADER
//	const uint8_t __attribute__((unused)) FwKwVerInfo[] = { '[', VI_FW_VERSION_KEYWORD, VERSION_FW_SOFT_TYPE, VERSION_FW_SOFT_MAJOR, VERSION_FW_SOFT_MINOR, VERSION_FW_SOFT_DEV, ']' };
	const uint8_t __attribute__((unused)) FwKwVerInfo[] = VI_MAKE_VER_KEYWORD_BE32(VI_FW_VERSION_KEYWORD, VERSION_FW_SOFT_TYPE, VERSION_FW_SOFT_MAJOR, VERSION_FW_SOFT_MINOR, VERSION_FW_SOFT_DEV);
	return (uint32_t) (VERSION_FW_SOFT);
#else // FIRMWARE_IS_BOOTLOADER :
	return  FwVersion32;	// Application Firmware Version, une fois détectée par le BootLoader
#endif // FIRMWARE_IS_BOOTLOADER
}

uint16_t __attribute__((used)) getVersion16BootLoader(void)
{
#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	return BlVersionInfo;	// BootLoader Version, une fois détectée par l'Applicatif
#elif defined(FIRMWARE_IS_BOOTLOADER) // ! FIRMWARE_IS_APPLICATIF && FIRMWARE_IS_BOOTLOADER :
//	const uint8_t __attribute__((unused)) BlKwVerInfo[] = { '[', VI_BL_VERSION_KEYWORD, ((BL_VER_MAJOR_MINOR >>0) & 0xFF), ((BL_VER_MAJOR_MINOR >>8) & 0xFF), ']' };
	const uint8_t __attribute__((unused)) BlKwVerInfo[] = VI_MAKE_VER_KEYWORD_LE16(VI_BL_VERSION_KEYWORD, BL_VER_MAJOR_MINOR);
	return (uint16_t) (BL_VER_MAJOR_MINOR);
#else // ! FIRMWARE_IS_APPLICATIF && ! FIRMWARE_IS_BOOTLOADER :
	return  UINT16_MAX; 	// => Non supporté !
#endif // FIRMWARE_IS_APPLICATIF / FIRMWARE_IS_BOOTLOADER
}

#pragma GCC pop_options

/******************************************************************************/

uint16_t __attribute__((used)) GetVersionSoft16(void) // Ajout_Jp le 03/08/2023 pour Ticket #148
{
#ifndef FIRMWARE_IS_BOOTLOADER
	return (uint16_t)VERSION_FW_SOFT_16;	// Conversion automatique Version32bits -> Version16bits
#else // FIRMWARE_IS_BOOTLOADER :
	return  FwVersion16;
#endif // FIRMWARE_IS_BOOTLOADER
}

/******************************************************************************/

void HandleRequest4NewRunMode(uint8_t newRunMode, uint16_t Requester)
{ // Précédemment "setFW_RunModeForRequester" & "setBL_RunModeForRequester" :

#if defined(VI_SUPPORT_JUMP_BL) || defined(FIRMWARE_IS_BOOTLOADER)
	if(Requester >= UINT16_NB_BITS) { Requester = UINT16_NB_BITS -1; }	// Ajout_Jp le 18/06/2020 pour protection CodeSonar "Shift Amount Exceeds Bit Width"
	switch(newRunMode)
	{
	case RUN_MODE_BL:
#ifdef FIRMWARE_IS_BOOTLOADER
		if(sabKeepInBL < SAB_KEEP_BL_ON_STAY) { sabKeepInBL = SAB_KEEP_BL_ON_STAY; } // Ramener pour maintenir en BL
#elif defined(VI_SUPPORT_JUMP_BL) // ! FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_JUMP_BL :
		if(SAB_JUMP_BL_DISABLED == sabJumpToBL) { sabJumpToBL = SAB_JUMP_TO_BL; }	// Initier la Tempo de départ vers BL
		if((sabJumpToBL > SAB_RUN_TO_BL) && ((BL_Requesters & (uint16_t)(1 << Requester)) > 0)) sabJumpToBL = SAB_RUN_TO_BL; // Shorten Tempo to BL
#endif // FIRMWARE_IS_BOOTLOADER
		BL_Requesters |= (uint16_t)(1 << Requester); // Mémorise qui Demande le BL
		break;
	case RUN_MODE_APP:
		BL_Requesters &= (uint16_t)~(1 << Requester); // Retire le Demandeur de la Liste
#ifdef FIRMWARE_IS_BOOTLOADER
		if(FLASH_SRC_UNLOCKED == isUpgradePending()) //|| (0 != isModbusSrcActiveFlashWriter()) )
		{
			if(sabKeepInBL > SAB_KEEP_BL_ON_EXIT) { sabKeepInBL = SAB_KEEP_BL_ON_EXIT; } // Ecourter la Tempo pour tenter prochainement
		}
#else // !FIRMWARE_IS_BOOTLOADER
		if(0 == BL_Requesters) { sabJumpToBL = SAB_JUMP_BL_DISABLED; } // Si on était le dernier Demandeur : Annule l'opération
#endif // FIRMWARE_IS_BOOTLOADER
		break;
	default:
		break;
	}
#else // ! VI_SUPPORT_JUMP_BL && ! FIRMWARE_IS_BOOTLOADER :
	UNUSED(newRunMode); UNUSED(Requester);	// Pour éviter le Warning de la Variable non utilisée
#endif // VI_SUPPORT_JUMP_BL || FIRMWARE_IS_BOOTLOADER

}

/******************************************************************************/

uint16_t __attribute__((weak)) getRunStatus(void) // en l'absence de la Librairie "UpgradeFirmware" :
{
	uint16_t tmpUINT16 = VI_RUN_STATUS_READY_4_CMD;
#ifdef VI_SUPPORT_JUMP_BL
	if(sabJumpToBL > 0) { tmpUINT16 |= VI_RUN_STATUS_ON_JUMP_BL; }
#endif // VI_SUPPORT_JUMP_BL
	return tmpUINT16;
}

/******************************************************************************/

inline __attribute__((always_inline)) void Handle_Infos_RT_100ms(void)	// A appeler (uniquement) depuis une Base de Temps RT @ 100ms
{

#ifdef VI_SUPPORT_JUMP_BL
	if(sabJumpToBL > SAB_JUMP_BL_EXPIRED) { sabJumpToBL--; }
#endif // VI_SUPPORT_JUMP_BL

#ifdef FIRMWARE_IS_BOOTLOADER
	if(sabKeepInBL > 0) { sabKeepInBL--; }
#endif // FIRMWARE_IS_BOOTLOADER

}

/******************************************************************************/

inline __attribute__((always_inline)) void InitComputeInfos(void)
{
	ComputeInfosStepId = eComputeStepNotInit;

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	BlVersionInfo = UINT16_MAX; // Unknown BootLoader Version (uniquement sur Applicatif)
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#if defined(FIRMWARE_IS_BOOTLOADER) && defined(VI_SUPPORT_APP_VERSION)
	FwVersion32 = UINT32_MAX; // Unknown Firmware Version (Uniquement sur BootLoader)
	FwVersion16 = UINT16_MAX,
#endif // FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_APP_VERSION

#ifdef VI_SUPPORT_FW_CRC
	FwComputedCRC32 = UINT32_MAX; // Unknown Firmware CRC32
#endif // VI_SUPPORT_FW_CRC
#ifdef VI_SUPPORT_BL_CRC
	BlComputedCRC32 = UINT32_MAX; // Unknown BootLoader CRC32
#endif // VI_SUPPORT_BL_CRC

#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_CRC) || defined(VI_SUPPORT_BL_VERSION)
	pComputeCurPos = 0;	// 0 = Invalid Pointeur
	curComputedCRC32 = UINT32_MAX;
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC || VI_SUPPORT_BL_VERSION
#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_CRC)
	CreateCRC32_Table();
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC

}

/******************************************************************************/

void ComputeMyInfos(void)
{

#ifdef VI_SUPPORT_JUMP_BL
	if(SAB_JUMP_BL_EXPIRED == sabJumpToBL) // Si c'est le moment de Redémarrer :
	{
		NVIC_SystemReset();
	}
#endif // VI_SUPPORT_JUMP_BL

#ifdef FIRMWARE_IS_BOOTLOADER
	if(SAB_STAY_BL_EXPIRED == sabKeepInBL)
	{

#ifdef VI_SUPPORT_APP_VERSION
	#define IS_MISSING_COMPUTE_INFOS	(ComputeInfosStepId <= eComputeStepFindFwVer || ComputeInfosStepId > eComputeStepNoAction)
	#define IS_INVALID_COMPUTE_INFOS	(UINT32_MAX == FwVersion32)
#else // ! VI_SUPPORT_APP_VERSION
	#define IS_MISSING_COMPUTE_INFOS	0	// No Problemo
	#define IS_INVALID_COMPUTE_INFOS	0	// No Problemo
#endif // VI_SUPPORT_APP_VERSION

		// Vérifie qu'on soit déjà prêt localement :
		if(IS_MISSING_COMPUTE_INFOS || !IsAutoUpgradeFirmwareReady()) // Si on est juste "presque" prêt :
		{
			sabKeepInBL = SAB_STAY_BL_WAIT_FW_VER;	// Initier une petite Tempo de Retry
		} else {
			// Teste qq éléments de présence d'un Firmware Applicatif valide :
			if(IS_INVALID_COMPUTE_INFOS || !IsUserFirmwarePresentAtFwBaseAdr()) // Si on n'est pas du tout prêt :
			{
				sabKeepInBL = SAB_STAY_BL_RETRY;			// Initier une Tempo de Retry standard (30s)
				BL_Requesters |= BL_REQUESTER_FLAG_NO_APP;	// Le Système s'oppose, pour le moment, au Jump
			} else { // OK pour le Passage immédiat en Applicatif User :

#ifdef ENABLE_BL_JUMP_TO_APP
				DoJump2App();				// Passer en Applicatif User :-) !
				__builtin_unreachable();	// Indiquer au Compilateur qu'on ne reviendra pas du passage en Applicatif User
#else // !ENABLE_BL_JUMP_TO_APP
				BL_Requesters &= ~(BL_REQUESTER_FLAG_NO_APP); // Le Système n'est pourtant pas opposé au Jump
				sabKeepInBL = SAB_KEEP_BL_ON_STAY;			  // Relancer une Tempo pour rester en BL
#endif // ENABLE_BL_JUMP_TO_APP

			}
		}
	}
#endif // FIRMWARE_IS_BOOTLOADER

	// Machine à Etats des Calculs CRC & Détection des Versions :
#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_VERSION) || defined(VI_SUPPORT_BL_CRC) || defined(VI_SUPPORT_APP_VERSION)
	switch (ComputeInfosStepId) {

#if defined(FIRMWARE_IS_BOOTLOADER) && defined(VI_SUPPORT_APP_VERSION)
	case eComputeStepInitFwVer:
		pComputeCurPos = (uint8_t*)(FIRMWARE_BASE_ADR + VECTORS_BLOC_SIZE);	// Le KeyWord recherché ne peut pas être dans la Zone des Vecteurs (ni en dessous)
		FwVersion32 = UINT32_MAX; // Unknown Firmware Version
		FwVersion16 = UINT16_MAX;
		curComputedCRC32 = 0; // Recycle la variable comme compteur dans la Signature recherchée
		ComputeInfosStepId++; // Activer la Recherche
		// noBreak;
	case eComputeStepFindFwVer:
		if(pComputeCurPos <= (uint8_t*)FIRMWARE_LAST_ADR)
		{
			const uint8_t FwKwVerInfo[] = VI_MAKE_VER_KEYWORD_HEADER(VI_FW_VERSION_KEYWORD); // Rechercher En-Tête Signature Version App
			uint32_t computeStepSize = FIRMWARE_LAST_ADR - (uint32_t)pComputeCurPos +1;
			if(computeStepSize > COMPUTE_STEP_SIZE) computeStepSize = COMPUTE_STEP_SIZE;
			for(uint32_t i = 0; i < computeStepSize ; i++)
			{
				if(FwKwVerInfo[curComputedCRC32] == *pComputeCurPos)
				{
					if(++curComputedCRC32 >= sizeof(FwKwVerInfo))
					{
						FwVersion32 = VI_MAKE_VER_SOFT_32(*(pComputeCurPos +1), *(pComputeCurPos +2), *(pComputeCurPos +3), *(pComputeCurPos +4));
						FwVersion16 = VI_MAKE_VER_SOFT_16(*(pComputeCurPos +1), *(pComputeCurPos +2), *(pComputeCurPos +3)); // Type + Major + Minor
						ComputeInfosStepId++; // Passer à l'étape suivante
						break;
					}
				} else {
					curComputedCRC32 = 0;
				}
				pComputeCurPos++;
			}
		}
		if(pComputeCurPos > (uint8_t*)FIRMWARE_LAST_ADR)
		{
			ComputeInfosStepId++; // Abandonner et passer à l'étape suivante
		}
		break;
#endif // FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_APP_VERSION

#ifdef VI_SUPPORT_FW_CRC
	case eComputeStepInitFwCRC:
		pComputeCurPos = (uint8_t*)FIRMWARE_BASE_ADR;
		FwComputedCRC32 = curComputedCRC32 = INIT_CRC_32;
		ComputeInfosStepId++; // Activer le Calcul
		// noBreak;
	case eComputeStepCalcFwCRC:
		if(pComputeCurPos <= (uint8_t*)FIRMWARE_LAST_ADR)
		{
			uint32_t computeStepSize = ((uint8_t*)FIRMWARE_LAST_ADR - pComputeCurPos +1);
			if(computeStepSize > COMPUTE_STEP_SIZE) computeStepSize = COMPUTE_STEP_SIZE;
			curComputedCRC32 = CRC32(curComputedCRC32, pComputeCurPos, computeStepSize);
			pComputeCurPos += computeStepSize;
		}
		if(pComputeCurPos > (uint8_t*)FIRMWARE_LAST_ADR)
		{
			FwComputedCRC32 = ~curComputedCRC32;
			ComputeInfosStepId++; // Passer à l'étape suivante
		}
		break;
#endif //VI_SUPPORT_FW_CRC

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	case eComputeStepInitBlVer:
		pComputeCurPos = (uint8_t*)(BOOTLOADER_BASE_ADR + VECTORS_BLOC_SIZE); // Le KeyWord recherché ne peut pas être dans la Zone des Vecteurs
		BlVersionInfo = UINT16_MAX; // Unknown BootLoader Version
		curComputedCRC32 = 0; // Recycle la variable comme compteur dans la Signature recherchée
		ComputeInfosStepId++; // Activer la Recherche
		// noBreak;
	case eComputeStepFindBlVer:
		if(pComputeCurPos < (uint8_t*)FIRMWARE_BASE_ADR)
		{
			const uint8_t BlKwVerInfo[] = VI_MAKE_VER_KEYWORD_HEADER(VI_BL_VERSION_KEYWORD); // Rechercher En-Tête Signature Version du BL
			uint32_t computeStepSize = (uint8_t*)FIRMWARE_BASE_ADR - pComputeCurPos;
			if(computeStepSize > COMPUTE_STEP_SIZE) computeStepSize = COMPUTE_STEP_SIZE;
			for(uint32_t i = 0; i < computeStepSize ; i++)
			{
				if(BlKwVerInfo[curComputedCRC32] == *pComputeCurPos)
				{
					if(++curComputedCRC32 >= sizeof(BlKwVerInfo))
					{
						//BlVersionInfo = VERINFO_MAKE_UINT32_BE(*(pComputeCurPos +1), *(pComputeCurPos +2), *(pComputeCurPos +3), *(pComputeCurPos +4));
						BlVersionInfo = VERINFO_MAKE_UINT16_LE(*(pComputeCurPos +1), *(pComputeCurPos +2));
						ComputeInfosStepId++; // Passer à l'étape suivante
						break;
					}
				} else {
					curComputedCRC32 = 0;
				}
				pComputeCurPos++;
			}
		}
		if(pComputeCurPos >= (uint8_t*)FIRMWARE_BASE_ADR)
		{
			ComputeInfosStepId++; // Abandonner et passer à l'étape suivante
		}
		break;
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#ifdef VI_SUPPORT_BL_CRC
	case eComputeStepInitBlCRC:
		pComputeCurPos = (uint8_t*)BOOTLOADER_BASE_ADR;
		BlComputedCRC32 = curComputedCRC32 = INIT_CRC_32;
		ComputeInfosStepId++; // Activer le Calcul
		// noBreak;
	case eComputeStepCalcBlCRC:
		if(pComputeCurPos < (uint8_t*)FIRMWARE_BASE_ADR)
		{
			uint32_t computeStepSize = (uint8_t*)FIRMWARE_BASE_ADR - pComputeCurPos;
			if(computeStepSize > COMPUTE_STEP_SIZE) computeStepSize = COMPUTE_STEP_SIZE;
			curComputedCRC32 = CRC32(curComputedCRC32, pComputeCurPos, computeStepSize);
			pComputeCurPos += computeStepSize;
		}
		if(pComputeCurPos >= (uint8_t*)FIRMWARE_BASE_ADR)
		{
			BlComputedCRC32 = ~curComputedCRC32;
			ComputeInfosStepId++; // Passer à l'étape suivante
		}
		break;
#endif // VI_SUPPORT_BL_CRC

#ifdef FIRMWARE_IS_BOOTLOADER
	case eComputeStepChkCanJump:
		if(IS_INVALID_COMPUTE_INFOS || !IsUserFirmwarePresentAtFwBaseAdr())
		{
			BL_Requesters |= BL_REQUESTER_FLAG_NO_APP;	// Le Système s'oppose, dès maintenant, au Jump
		}
		ComputeInfosStepId++; // Dans tous les cas : Passer à l'étape suivante
		break;
#endif // FIRMWARE_IS_BOOTLOADER

	case eComputeStepNoAction:
		break; // Nothing to do now :-) !
	case eComputeStepReInitNfo:

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
		BlVersionInfo = UINT16_MAX; // Unknown BootLoader Version
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#if defined(FIRMWARE_IS_BOOTLOADER) && defined(VI_SUPPORT_APP_VERSION)
		FwVersion32 = UINT32_MAX; // Unknown Firmware Version
		FwVersion16 = UINT16_MAX;
#endif // FIRMWARE_IS_BOOTLOADER && VI_SUPPORT_APP_VERSION

#ifdef VI_SUPPORT_FW_CRC
		FwComputedCRC32 = UINT32_MAX; // Unknown Firmware CRC32
#endif // VI_SUPPORT_FW_CRC

#ifdef VI_SUPPORT_BL_CRC
		BlComputedCRC32 = UINT32_MAX; // Unknown BootLoader CRC32
#endif // VI_SUPPORT_BL_CRC

		ComputeInfosStepId = eComputeStepNoAction;
		break;
	case eComputeStepNotInit:
	default:
		ComputeInfosStepId = eComputeStepNotInit +1;
		break;
	}
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC || VI_SUPPORT_BL_VERSION

}

/******************************************************************************/

inline __attribute__((always_inline)) void stopAndDiscardComputedInfos(void)
{
	ComputeInfosStepId = eComputeStepReInitNfo;
}

/******************************************************************************/

inline __attribute__((always_inline)) void reStartComputeInfos(void)
{
	ComputeInfosStepId = eComputeStepNotInit;
}

/******************************************************************************/

#ifdef FIRMWARE_IS_BOOTLOADER

unsigned IsVersionInfosReady4UserAppFw(void)
{
#ifdef VI_SUPPORT_APP_VERSION
	if(UINT32_MAX == FwVersion32) return 0;
#endif // VI_SUPPORT_APP_VERSION
	return (eComputeStepNoAction == ComputeInfosStepId);
}

#endif // FIRMWARE_IS_BOOTLOADER

/******************************************************************************/

void DeInitComputeInfos(void)
{

#if defined(VI_SUPPORT_FW_CRC) || defined(VI_SUPPORT_BL_CRC)
	FreeCRC32_Table();
#endif // VI_SUPPORT_FW_CRC || VI_SUPPORT_BL_CRC

}

/******************************************************************************/

#if defined(APP_WITH_BOOT) && !defined(FIRMWARE_IS_BOOTLOADER)

__attribute__((constructor)) void UpdateVectorTableForUserAppInReleaseModeWithBootLoader(void)
{ // Remarque_Jp le 18/12/2023 : Attribute "Constructor" required !
	  // Reconfigure la Table des Vecteurs dans l'espace de notre Firmware :
	__disable_irq();	// Disable IRQ : Normalement déjà fait en quittant le BL, mais au cas où ...
	  SCB->VTOR = FIRMWARE_BASE_ADR; // Vectors @ Emplacement début du Firmware (= après BootLoader)
	__DSB();			// Update Modify vector table location
	__ISB();
	__enable_irq();		// Réactive le fonctionnement normal des Interruptions
}

#endif // APP_WITH_BOOT && ! FIRMWARE_IS_BOOTLOADER

/******************************************************************************/

uint32_t getUserApp_MaxFwSize(void)	{ return (uint32_t)FIRMWARE_MAX_SIZE; } // from "FlashInfos.h"
uint32_t getMcuFlashSize(void)		{ return (uint32_t)FLASH_DEVICE_SIZE; } // from "FlashInfos.h"
uint32_t getMcuDeviceId(void)		{ return DBGMCU->IDCODE; } // Cf. "Reference manual" RM0431 for stm32F7*
// OR https://community.st.com/t5/stm32-mcus-products/where-is-the-mcu-s-id/m-p/382974/highlight/true#M104477

/******************************************************************************/

#ifdef VI_SUPPORT_LINKER_INFOS

VI_FUNCTION_GET_U32_LINKER_VALUE(EndOfStack, _estack);
VI_FUNCTION_GET_U32_LINKER_VALUE(OriginOfFlash, _oflash);
VI_FUNCTION_GET_U32_LINKER_VALUE(EndOfFlash, _eflash);
VI_FUNCTION_GET_U32_LINKER_VALUE(EndOfText, _etext);
VI_FUNCTION_GET_U32_LINKER_VALUE(EndOfFW, _end_of_fw);
VI_FUNCTION_GET_U32_LINKER_VALUE(StartOfData, _sdata);
VI_FUNCTION_GET_U32_LINKER_VALUE(EndOfData, _edata);
VI_FUNCTION_GET_U32_LINKER_VALUE(SiData, _sidata);

#endif // VI_SUPPORT_LINKER_INFOS

/******************************************************************************/

#ifdef __cplusplus
}
#endif
