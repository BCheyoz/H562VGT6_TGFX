/*
 * crc.h
 *
 *  Created on: 4 avr. 2019
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 14/01/2020 : Added by AA to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
 *-> 09/04/2020 : Added by AB to IHM_Easykub_Firmware (STM32F722RC : productprojects/ventilation/ihm/easykub/FW_Easykub)
 *-> 30/07/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
 *-> 21/10/2021 : Added by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
 *-> 23/12/2021 : Added by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 25/02/2022 : Added by AB to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *
 */

#ifndef CRC_H_
#define CRC_H_

/********************************************************************************
* Procédure pour intégrer facilement les modules "CRC" dans un nouveau projet : *
*********************************************************************************

 1) Ajouter l'Include du .h parmi ceux des Fichiers.c qui ont besoin des Fonctions de CRC :
  (par exemple parmi les "USER CODE * Includes")
	#include "crc.h"

 2) Activer ou mettre en commentaire les define ci-dessous suivant le(s) module(s) CRC
  souhaité(s) : CRC_SUPPORT_CRC8 / CRC_SUPPORT_CRC16 / CRC_SUPPORT_CRC32

 3) Pour le module CRC32, appeler les fonctions "CreateCRC32_Table" et "FreeCRC32_Table"
  à un moment approprié, surtout avant le 1° appel à la fonction de calcul "CRC32" :
	CreateCRC32_Table();	// Pour Initialiser la Table de CRC32 avec le Polynôme choisi
	FreeCRC32_Table();		// Pour Libérer la Table de CRC32 lorsqu'elle n'est plus utile

 4) Ajouter le Dossier "CRC" à l' "IncludePath", pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/CRC" (sans les guillemets)

 5) Vérifier que le Dossier "CRC" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

=> Félicitations, c'est prêt :-) !

********************************************************************************/

// Paramètres User à Activer, si nécessaire :
#define CRC_SUPPORT_CRC8	// Nécessaire pour SHT_30, SDP_6xx
#define CRC_SUPPORT_CRC16	// Nécessaire pour Trames Modbus
#define CRC_SUPPORT_CRC32	// Nécessaire pour Calcul signature du Soft

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC8	//	Support du CRC8 :

#define INIT_CRC_8			0xFF
uint8_t CRC8(uint8_t* ptr, uint8_t nbBytes, uint8_t CrcInit);
//uint8_t CRC8_Poly(uint8_t* ptr, uint8_t NumBytes, uint8_t CrcInit, uint8_t Polynomial);
uint8_t isValidMultiU16BlocsCRC8(uint8_t* pBuf, uint8_t nbU16Blocs, uint8_t CrcInit);

#endif // CRC_SUPPORT_CRC8

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC16	// Support du CRC16 :

#define POLYNOME_16			0xA001  // Polynôme CRC16 : 2^15 + 2^13 + 2^0 = 0xA001.
#define INIT_CRC_16			0xFFFF
uint16_t CRC16(uint8_t* ptr, uint16_t nbBytes, uint16_t Crc);

#endif // CRC_SUPPORT_CRC16

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32	// Support du CRC32 :

#define POLYNOME_32			0xEDB88320L     	// Standard CRC-32 Polynomial
#define INIT_CRC_32			0xFFFFFFFFUL
uint32_t CRC32(uint32_t crc_accum, uint8_t* ptr, uint32_t nbBytes);
void CreateCRC32_Table(void);
void FreeCRC32_Table(void);

#endif // CRC_SUPPORT_CRC32

#endif /* CRC_H_ */
