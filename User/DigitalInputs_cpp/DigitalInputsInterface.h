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

#define DI_DEFAULT_SEUIL_ON 	6	// Valeur Mini Considérée comme ON par defaut
#define DI_DEFAULT_SEUIL_OFF	3	// Valeur Maxi considérée comme OFF par defaut

typedef enum{
	E_DI_SAMPLE_1ms,
	E_DI_SAMPLE_10ms,
} E_DI_SAMPLE_FREQ;


/********************************************************************************************/
// pour compatibilité avec la lib BaseDeTemps en C
#ifdef __cplusplus
extern "C" {
#endif
	void GestionDigitalInputs();
	void Handle_DigitalInputs_RT_1ms();
	void Handle_DigitalInputs_RT_10ms();
	void Handle_DigitalInputs_RT_100ms();
#ifdef __cplusplus
}
#endif
/********************************************************************************************/


#endif /*DIGITALINPUTS_CPP_DIGITALINPUTSINTERFACE_H_*/
