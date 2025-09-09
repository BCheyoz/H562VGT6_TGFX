/*
 * VersionInfos.h
 *
 *  Created on: 13 févr. 2020
 *  Original Author: j.proux
 *
 *  Updated on: 06 Fév. 2024
 *  Updated by: j.proux
 *
 *  Version 1.1
 */

#ifndef VERSION_INFOS_VERSION_INFOS_H_
#define VERSION_INFOS_VERSION_INFOS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // Pour les types "int*_t" & "uint*_t"

/********************************************************************************************/
// Paramètres User à Activer / Configurer :

/*** Alpha 00.01.00 (v0.1.0) by BC le 11/02/2025 ****************************************************/
#define VERSION_FW_SOFT_TYPE    (0xA0)  // Bin type
#define VERSION_FW_SOFT_MAJOR   (0x00)  // major version
#define VERSION_FW_SOFT_MINOR   (0x03)  // Minor version
#define VERSION_FW_SOFT_DEV     (0x08)  // Dev version


#define VERSION_FW_SOFT			VI_MAKE_VER_SOFT_32(VERSION_FW_SOFT_TYPE, VERSION_FW_SOFT_MAJOR, VERSION_FW_SOFT_MINOR, VERSION_FW_SOFT_DEV)
#define VERSION_FW_SOFT_16		VI_MAKE_VER_SOFT_16(VERSION_FW_SOFT_TYPE, VERSION_FW_SOFT_MAJOR, VERSION_FW_SOFT_MINOR)

#define VI_SUPPORT_FW_CRC			// Pour activer le Calcul du CRC du FW
#define VI_SUPPORT_BL_CRC			// Pour activer le Calcul du CRC du BL
#define VI_SUPPORT_BL_VERSION		// Pour activer la Recherche de la Version du BL (requires FIRMWARE_IS_APPLICATIF)
#define VI_SUPPORT_JUMP_BL  		// Pour gérer les Reboot temporisés vers BL
//#define VI_SUPPORT_LINKER_INFOS 	// Pour activer la Récupération de qq Infos depuis le Linker

#define VI_FW_VERSION_KEYWORD		'T','F','L','4','C','M','_','F','W'	// Applicatif Signature Keyword to Tag in Applicatif
#define VI_BL_VERSION_KEYWORD		'T','F','L','4','C','M','_','B','L'	// BootLoader Signature Keyword to Find in Applicatif

#define FIRMWARE_IS_APPLICATIF  	// Considérer ce Firmware comme un Applicatif (Activer par défaut)
//#define FIRMWARE_IS_BOOTLOADER  	// Activer uniquement si ce Firmware doit être considéré comme un BootLoader

#ifdef FIRMWARE_IS_BOOTLOADER	// Options Spécifiques au mode BootLoader :
  #define BL_VER_MAJOR_MINOR	0xA001	// Versionning (16bits) du BootLoader
  #define VI_SUPPORT_APP_VERSION		// Pour activer la Recherche de la Version Applicative en BL
#endif // FIRMWARE_IS_BOOTLOADER

/********************************************************************************************/
// Recomposition du Versionning :

#define VI_MAKE_VER_SOFT_32(t,ma,mi,dev)	((((t) & 0xF0) << 24) | (((ma) & 0xFF) << 16) | (((mi) & 0xFF) << 8) | (((dev) & 0xFF) << 0))
#define VI_MAKE_VER_SOFT_16(t,ma,mi)		((((t) & 0xF0) << 8) | (((ma) & 0x0F) << 8) | ((mi) & 0xFF))

// Décomposition par Type de Version :
#define FW_VERSION_TYPE_ALPHA	0xA0000000	// Marquage d'une Version de type Alpha
#define FW_VERSION_TYPE_BETA	0xB0000000	// Marquage d'une Version de type Beta
#define FW_VERSION_TYPE_RELEASE	0x00000000	// Marquage d'une Version de type Release
#define FW_VERSION_TYPE_MASK	0xF0000000	// Masque du Type de Soft : Axxx = Alpha, Bxxx = Beta, 0xxx = Release, Autre = Spécial
#define FW_VERSION_TYPE_VALUE	(VERSION_FW_SOFT & FW_VERSION_TYPE_MASK)

// Transformation en Texte du Type de Version :
#if FW_VERSION_TYPE_VALUE == FW_VERSION_TYPE_RELEASE
    #define FW_VERSION_TYPE_STR "Release"
#elif FW_VERSION_TYPE_VALUE == FW_VERSION_TYPE_ALPHA
    #define FW_VERSION_TYPE_STR "Alpha"
#elif FW_VERSION_TYPE_VALUE == FW_VERSION_TYPE_BETA
    #define FW_VERSION_TYPE_STR "Beta"
#else
    #define FW_VERSION_TYPE_STR "Special"
#endif // Version Type

/********************************************************************************************/

uint32_t GetVersionSoft(void);
uint16_t GetVersionSoft16(void); // Ajout_Jp le 03/08/2023 pour Ticket #148

uint16_t getVersion16BootLoader(void);

#define RUN_MODE_BL		1
#define RUN_MODE_APP	2
extern const uint8_t curRunMode;
extern const uint8_t ProductKey[4];	// Identification Produit en 4 caractères
//extern const uint32_t ProductKey; // Equivalent DWORD32

void HandleRequest4NewRunMode(uint8_t newRunMode, uint16_t Requester);	// Pour accès via Clef USB (ou autre); Précédemment "setFW_RunModeForRequester" & "setBL_RunModeForRequester"

#define VI_RUN_STATUS_READY_4_CMD	1
#define VI_RUN_STATUS_BUSY_UPDATE	2
#define VI_RUN_STATUS_READY_4_APP	256
#define VI_RUN_STATUS_ON_JUMP_BL	512
uint16_t getRunStatus(void);	// Précédemment "getFW_RunStatus"
unsigned IsVersionInfosReady4UserAppFw(void);

void Handle_Infos_RT_100ms(void); // Timer vers Jump2BootLoader (à appeler depuis une Base de Temps RT @ 100ms)

void InitComputeInfos(void);
void ComputeMyInfos(void);
void stopAndDiscardComputedInfos(void);
void reStartComputeInfos(void);
void DeInitComputeInfos(void);

uint32_t getUserApp_MaxFwSize(void);
uint32_t getMcuFlashSize(void);
uint32_t getMcuDeviceId(void);

/******************************************************************************/
// Fonctions & Définitions publiques pour accès aux Infos du Linker :

#ifdef VI_SUPPORT_LINKER_INFOS

	#define VI_MAKE_PROTO_GET_U32_LINKER_VALUE(n,l) uint32_t getLinker_##n(void)
	#define VI_FUNCTION_GET_U32_LINKER_VALUE(n,l) VI_MAKE_PROTO_GET_U32_LINKER_VALUE(n,l) { extern uint8_t l; return (uint32_t)&l; }

	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(EndOfStack, _estack);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(OriginOfFlash, _oflash);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(EndOfFlash, _eflash);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(EndOfText, _etext);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(EndOfFW, _end_of_fw);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(StartOfData, _sdata);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(EndOfData, _edata);
	VI_MAKE_PROTO_GET_U32_LINKER_VALUE(SiData, _sidata);

#endif // VI_SUPPORT_LINKER_INFOS

/********************************************************************************************/

#ifdef VI_SUPPORT_JUMP_BL
	extern uint16_t BL_Requesters;
	extern uint16_t sabJumpToBL;
#endif // VI_SUPPORT_JUMP_BL

#if defined(FIRMWARE_IS_APPLICATIF) && defined(VI_SUPPORT_BL_VERSION)
	extern uint16_t BlVersionInfo;
#endif // FIRMWARE_IS_APPLICATIF && VI_SUPPORT_BL_VERSION

#ifdef VI_SUPPORT_FW_CRC
	extern uint32_t FwComputedCRC32;
#endif  // VI_SUPPORT_FW_CRC

#ifdef VI_SUPPORT_BL_CRC
	extern uint32_t BlComputedCRC32;
#endif	// VI_SUPPORT_BL_CRC

/********************************************************************************************/

#define UINT32_NB_BITS		(8 * sizeof(uint32_t))
#define UINT16_NB_BITS		(8 * sizeof(uint16_t))

/********************************************************************************************/

#ifdef FIRMWARE_IS_BOOTLOADER	// Options Spécifiques au mode BootLoader : Tempo de Maintient en BootLoader :
  #define SAB_STAY_BL_MST		(15 *10)	// Temps de maintient en BL @ MST, Base = RT @ 100ms
  #define SAB_STAY_BL_RETRY	(30 *10)	// Temps avant Retry si pas d'Applicatif détecté : 30s
  #define SAB_STAY_BL_WAIT_FW_VER	( 5)	// Temps avant Retry si pas de Version Applicatif détectée : 500ms
  #define SAB_STAY_BL_UPDTING	(500*10)	// Temps max estimé pour mettre à Jour 384K d'Applicatif : 500s (= 8min)
  #define SAB_STAY_BL_UPDATED	( 5 *10)	// Délai accordé avant de tenter le Jump en Applicatif juste après un Update du Firmware : 5s
  #define SAB_KEEP_BL_ON_EXIT	( 5 *10)	// Délai accordé avant de tenter le Jump en Applicatif sur demande de Basculer vers APP : 5s
  #define SAB_KEEP_BL_ON_STAY	(60 *10)	// Délai accordé avant de tenter le Jump en Applicatif sur demande de Rester en BL : 60s
  #define SAB_STAY_BL_EXPIRED	0
  extern uint16_t sabKeepInBL;	// WARNING : ne PAS utiliser en mode Applicatif !
#endif // FIRMWARE_IS_BOOTLOADER

#ifdef __cplusplus
}
#endif

#endif /* VERSION_INFOS_VERSION_INFOS_H_ */
