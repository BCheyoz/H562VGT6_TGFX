/*
 * BaseDeTemps.h
 *
 *  Created on: 30 juil. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 30/07/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
 *-> 21/10/2021 : Added by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
 *-> 22/12/2021 : Added by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 24/02/2022 : Added by AB to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *
 */

#ifndef BASEDETEMPS_BASEDETEMPS_H_
#define BASEDETEMPS_BASEDETEMPS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>			// Pour les types "int*_t" & "uint*_t"

/********************************************************************************************
* Procédure pour intégrer facilement cette Librairie "BaseDeTemps" dans un nouveau projet : *
*********************************************************************************************

 +-----------------------------+
 | Etape I : Configurer CubeMX |
 +-----------------------------+

I.1 ) Dans CubeMX, aller dans "ProjectManager -> Project" :
 -> Sélectionner "Toolchain / IDE" = " STM32CubeIDE" + cocher "Generate Under Root"
 -> Habituellement "Minimum Heap Size" peut rester à 0x200
 -> Habituellement "Minimum Stack Size" peut rester à 0x400
 -> A la 1° Configuration, s'assurer d'avoir déjà bien sélectionné le "Firmware Package"
  le plus récent parmi ceux compatibles !

I.2) Dans "ProjectManager -> Code Generator" s'assurer des Paramètres suivants :
 -> "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" -> Coché
 -> "Keep User Code when re-generating" -> Coché
 -> Je recommande de cocher également "Delete previously generated files when not re-generated"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus est modifié !


 +------------------------------------------------------------------------------------+
 | Etape II : Dans ce fichier "BaseDeTemps.h", Configurer ci-dessous (cf. plus bas) : |
 +------------------------------------------------------------------------------------+

II.1) Décommenter (ci-dessous) "BDT_SUPPORT_ALIVE_RT" si vous souhaitez utiliser le Compteur
 de RunTime natif "AliveRunTime1s" de cette Librairie

 +------------------------------------+
 | Etape III : Configurer le Projet : |
 +------------------------------------+

III.1) Ajouter l'Include du .h dans le "main.c" et "stm32*_it.c" (par exemple parmi les "USER CODE * Includes") :
	#include "BaseDeTemps.h"

III.2) Ajouter l'appel d'Init dans le "main" du "main.c" (par exemple parmi les "USER CODE * 2") :
	InitBaseDeTemps();

III.3) Ajouter le Handler @ 1ms dans une Interruption :
(par exemple dans le "USER CODE * SysTick_IRQn 1" du "SysTick_Handler" de "stm32*_it.c")
	HandleBaseDeTemps_IT_1ms();

III.4) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
	GestionBaseDeTemps();

III.5) Ajouter le Dossier "BaseDeTemps" à l' "IncludePath", pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/BaseDeTemps" (sans les guillemets)

III.6) Vérifier que le Dossier "BaseDeTemps" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

=> Félicitations, c'est prêt :-) !

*******************************************************************************************/
// Paramètres User à Activer / Configurer :

#define BDT_SUPPORT_ALIVE_RT			// Pour gérer le Compteur de RunTime "AliveRunTime1s"

/********************************************************************************************/

void InitBaseDeTemps(void); 			// A appeler dans la partie Init du "main.c"
void GestionBaseDeTemps(void);			// A appeler dans la Boucle Principale du "main.c"

void HandleBaseDeTemps_IT_1ms(void);	// A appeler dans une Interruption @ 1ms (par exemple le "SysTick_Handler", dans "stm32*_it.c")

uint32_t getAliveRunTime100ms(void);
uint32_t getAliveRunTime1s(void);		// Pour récupérer le temps de RunTime x1s

#ifdef __cplusplus
}
#endif

#endif /* BASEDETEMPS_BASEDETEMPS_H_ */
