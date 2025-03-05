/*
 * DigitalInputs.cpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */

#include "DigitalInputs.hpp"

// Initialisation des variables static partagé entre toutes les instances de l'objet
std::list<DigitalInputs*> DigitalInputs::allInputs;

/******************************************************************************/
DigitalInputs::DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,GPIO_PinState WorkState) {
	// init
	_workState = (WorkState == GPIO_PIN_SET);
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;

	// etat par défaut de l'entrée (1 ou 0 ) -> WORKING_STATE_IS_0/GPIO_PIN_RESET = 0 / WORKING_STATE_IS_1/GPIO_PIN_SET = 1
}

/******************************************************************************/
void DigitalInputs::GestionDigitalInputs()// while loop
{
	for (auto input : allInputs)
	{
		unsigned newState = input->getState(); // Par défaut : comme actuellement
		// Gestion Anti-Rebond :
		if(newState > 0)	// Actuellement en position Travail :
		{
			if(input->_nbPinOn < DI_SEUIL_OFF) newState = 0;	// Retour en position IDLE
		} else {			// Actuellement en repos :
			if(input->_nbPinOn >= DI_SEUIL_ON) newState = 1;	// Passage en position WORKING
		}
		//input->newStateEvent = (pDI->curState != newState); TODO event part
		input->setState(newState);

		// TODO event part
	}
}

/******************************************************************************/
void DigitalInputs::Handle_RT_10ms()
{
	// handle each DigitalInput pin
	for (auto input : allInputs)
	{
		GPIO_PinState PinState = HAL_GPIO_ReadPin(input->_GPIOPort,input->_GPIOPin);// read pin state
		if(PinState == input->_workState) // Suivant l'Etat "Travail" configuré :
		{
			if(input->_nbPinOn < DI_MAX_CT_ON)	input->_nbPinOn++;
		} else {
			if(input->_nbPinOn > DI_MIN_CT_ON)	input->_nbPinOn--;
		}
	}
}

unsigned DigitalInputs::getState(void)
{
	return _curState;
}

void DigitalInputs::setState(unsigned state)
{
	_curState = state;
}
