/*
 * DigitalInputsInterface.h
 *
 *  Created on: Mar 5, 2025
 *      Author: m.faget
 */

#ifndef DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_
#define DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_
#include "utils.h"				// Pour accès aux éléments utiles génériques

/******************************************************************************/
// Init & Configuration des DigitalInputs :

#define DI_NO_WORKING_STATE_IS_1	GPIO_PIN_SET	// Type NO => '1' = Travail
#define DI_NF_WORKING_STATE_IS_0	GPIO_PIN_RESET	// Type NF => '0' = Travail
#define DI_PU_WORKING_STATE_IS_0	GPIO_PIN_RESET	// Pull-Up => '0' = Travail

typedef void(*pDI_FnHandler)(uint16_t EventId,int16_t *last_event);	// Prototype de Callback appelés en cas d'évènement DigitalInput
#define CASE_SET_VAR_VAL_BREAK(c,p,v)	case c:p=v;break	// Ajouter le ';' manuellement après la macro
/******************************************************************************/
// Configuration de la plage Anti-Rebonds :

#define DI_MAX_CT_ON	5	// Valeur Maxi de nbPinOn
#define DI_MIN_CT_ON	1	// Valeur Mini de nbPinOn

#define DI_SEUIL_ON 	4	// Valeur Mini Considérée comme ON
#define DI_SEUIL_OFF	2	// Valeur Maxi considérée comme OFF

/******************************************************************************/
// Configuration des Délais à détecter :

#define DI_DELAY_1S 	10		// 1S = 10 * 100ms
#define DI_DELAY_3S 	30		// 3S = 30 * 100ms
#define DI_DELAY_10S	100		// 10S = 100 * 100ms
#define DI_DELAY_START_AUTO_FIRE	15	// 15 * 100ms = 1.5s
#define DI_DELAY_SIGNAL_AUTO_FIRE	2	// 2 * 100ms = 0.2s


/******************************************************************************/
// Configuration des Evènements :
//#define DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS // Pour désactiver le support des Evènements DigitalInputs
#define USE_DIGITAL_INPUTS

// Enumération des Evènements supportés :
#define DI_EVENT_NEW_STATE  	(1<< 0)		// Un changement Working <-> Idle est intervenu
#define DI_EVENT_NEW_WORK_STATE	(1<< 1)		// L'entrée est passée en position Travail "Working"
#define DI_EVENT_NEW_IDLE_STATE	(1<< 2)		// L'entrée est passée en position Repos "Idle"
#define DI_EVENT_WORK_STATE_1S	(1<< 3)		// L'entrée est en position Working depuis 1s
#define DI_EVENT_IDLE_STATE_1S	(1<< 4)		// L'entrée est en position Idle depuis 1s
#define DI_EVENT_WORK_STATE_3S	(1<< 5)		// L'entrée est en position Working depuis 3s
#define DI_EVENT_IDLE_STATE_3S	(1<< 6)		// L'entrée est en position Idle depuis 3s
#define DI_EVENT_WORK_STATE_10S	(1<< 7)		// L'entrée est en position Working depuis 10s
#define DI_EVENT_IDLE_STATE_10S	(1<< 8)		// L'entrée est en position Idle depuis 10s
#define DI_EVENT_AUTO_FIRE  	(1<< 9)		// Un nouvel évènement régulier "AutoFire" est survenu avec l'entrée en position "Working"
#define DI_MAX_FN_HANDLERS  	10

/******************************************************************************/
typedef enum
{
	E_SINGLE_INPUT = 1, //
	E_GROUPED_INPUT,
	//---------------
	E_INPUT_NB_TYPES // A conserver en dernier élément : indique le nb Max d'états possibles
} E_DIGITAL_INPUT_TYPES;

/********************************************************************************************/
// pour compatibilité avec la lib BaseDeTemps en C
#ifdef __cplusplus
extern "C" {
#endif
	void GestionDigitalInputs();
	void Handle_DigitalInputs_RT_10ms();
	void Handle_DigitalInputs_RT_100ms();
#ifdef __cplusplus
}
#endif
/********************************************************************************************/


#endif /*DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_*/
