/*
 * DigitalInputsInterface.h
 *
 *  Created on: Mar 5, 2025
 *      Author: m.faget
 */

#ifndef DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_
#define DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_

/******************************************************************************/
// Init & Configuration des DigitalInputs :

#define DI_NO_WORKING_STATE_IS_1	GPIO_PIN_SET	// Type NO => '1' = Travail
#define DI_NF_WORKING_STATE_IS_0	GPIO_PIN_RESET	// Type NF => '0' = Travail
#define DI_PU_WORKING_STATE_IS_0	GPIO_PIN_RESET	// Pull-Up => '0' = Travail

/******************************************************************************/
// Configuration de la plage Anti-Rebonds :

#define DI_MAX_CT_ON	5	// Valeur Maxi de nbPinOn
#define DI_MIN_CT_ON	1	// Valeur Mini de nbPinOn

#define DI_SEUIL_ON 	4	// Valeur Mini Considérée comme ON
#define DI_SEUIL_OFF	2	// Valeur Maxi considérée comme OFF

/********************************************************************************************/
// pour compatibilité avec la lib BaseDeTemps en C
#ifdef __cplusplus
extern "C" {
#endif
	void GestionDigitalInputs();
	void Handle_RT_10ms();
	void Handle_RT_100ms();
#ifdef __cplusplus
}
#endif
/********************************************************************************************/


#endif /*DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_*/
