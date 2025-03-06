/*
 * DigitalInputs.cpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */

#include "DigitalInputs.hpp"

// Initialisation des variables static partagé entre toutes les instances de l'objet
extern std::vector<DigitalInputs*> DigitalInputs::allInputs;

/******************************************************************************/
// Pour compatibilité avec la lib BaseDeTemps en C
void GestionDigitalInputs() 		{DigitalInputs::GestionDigitalInputs();}
void Handle_DigitalInputs_RT_10ms() { DigitalInputs::Handle_RT_10ms();}

/******************************************************************************/
DigitalInputs::DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,GPIO_PinState WorkState) {
	// init
	_nbPinOn = 0;

	_workState = (WorkState == GPIO_PIN_SET);
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;

	_Flags = {0};
	allInputs.insert(allInputs.begin(),this);
}

/******************************************************************************/
void DigitalInputs::GestionDigitalInputs()
{
	for(auto input : allInputs)
	{
		// update DigitalInputState
		unsigned newState = input->getcurState(); // Par défaut : comme actuellement
		// Gestion Anti-Rebond :
		if(newState > 0)	// Actuellement en position Travail :
		{
			if(input->_nbPinOn < DI_SEUIL_OFF) newState = 0;	// Retour en position IDLE
		} else {			// Actuellement en repos :
			if(input->_nbPinOn >= DI_SEUIL_ON) newState = 1;	// Passage en position WORKING
		}
		input->setcurState(newState);
	}
}

/******************************************************************************/
void DigitalInputs::Handle_RT_10ms()
{
	for(auto input : allInputs)
	{
		// handle DigitalInputs
		GPIO_PinState PinState = HAL_GPIO_ReadPin(input->_GPIOPort,input->_GPIOPin);// read pin state
		if(PinState == input->_workState) // Suivant l'Etat "Travail" configuré :
		{
			if(input->_nbPinOn < DI_MAX_CT_ON)	input->_nbPinOn++;
		} else {
			if(input->_nbPinOn > DI_MIN_CT_ON)	input->_nbPinOn--;
		}
	}
}

/******************************************************************************/

unsigned DigitalInputs::getcurState(void)
{
	return _curState;
}

void DigitalInputs::setcurState(unsigned state)
{
	_curState = state;
}
