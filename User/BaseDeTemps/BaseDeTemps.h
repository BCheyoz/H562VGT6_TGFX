/*
 * BaseDeTemps.h
 *
 *  Created on: 30 juil. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 03 Fev. 2025
 *  Updated by: b.chhay
 *
 *  version 1.0
 *
 */

#ifndef BASEDETEMPS_BASEDETEMPS_H_
#define BASEDETEMPS_BASEDETEMPS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>			// Pour les types "int*_t" & "uint*_t"

/*******************************************************************************************/
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
