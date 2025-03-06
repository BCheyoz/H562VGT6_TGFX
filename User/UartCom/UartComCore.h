/*
 * UartComCore.h
 *
 *  Created on: Dec 15, 2020
 *  Original Author: j.proux
 *
 *  Updated on: 21 Feb. 2025
 *  Updated by: j.proux
 *
 *  Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 *	-> il te faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "UartCom" dans un nouveau Projet :
 *   -> Suivre les indications dans "UartComConf.h"
 *
 */

#ifndef __UartComCore_h
#define __UartComCore_h

#ifdef __cplusplus
 extern "C" {
#endif

/********************************************************************************************/

#include <stdint.h> // Pour accès aux types "int*_t" & "uint*_t"

// Exported functions prototypes :

void UartCom_Devices_Init(void);	// A appeler d'abord dans la partie Init Hardware
void UartCom_RunTime_Init(void);	// A appeler ensuite dans la partie Init Software

void UartCom_Handle_IT_1ms(void);	// Appeler dans l'Interruption @ 1ms (stm32*_it.c)

void Gestion_UartCom(void);			// A appeler dans la Boucle Principale

void UartCom_Devices_DeInit(void);	// Pour éventuellement Dé-Initialiser tous les Périphéques chargés par UartCom

/********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __UartComCore_h */
