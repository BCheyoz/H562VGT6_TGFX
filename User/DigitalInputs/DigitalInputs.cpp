/*
 * DigitalInputs.cpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */

#include "DigitalInputs.hpp"

// Initialisation des variables static partagé entre toutes les instances de l'objet
std::vector<DigitalInputs*> DigitalInputs::allInputs;

/******************************************************************************/
// Pour compatibilité avec la lib BaseDeTemps en C
void GestionDigitalInputs() 		{DigitalInputs::GestionDigitalInputs();}
void Handle_DigitalInputs_RT_10ms() { DigitalInputs::Handle_RT_10ms();}
void Handle_DigitalInputs_RT_100ms() { DigitalInputs::Handle_RT_100ms();}

/******************************************************************************/
DigitalInputs::DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,GPIO_PinState WorkState,uint16_t diParam) {
	// init
	_nbPinOn = 0;
	_nb100ms = 0;
	_diParam = diParam;
	_workState = (WorkState == GPIO_PIN_SET);
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_Flags = {0};
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
	pDI_FnHandler pFn = nullptr;
	for(int i = 0; i < DI_MAX_FN_HANDLERS; i++)
	{
		_pFnHandler[i] = pFn;
	}
#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
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
		input->_newStateEvent = (input->_curState != newState);
		input->setcurState(newState);

		if(input->_newStateEvent > 0)
		{
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS

			if(newState) input->_newWorkEvent = 1;	// Evènement Working now
			else input->_newIdleEvent = 1;			// Evènement Idle now

#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS

			input->_nb100ms = 0;					// Relance le Compteur
		}

		// DispatchingDigitalInputEvents
		// TODO gestion evenements et callback
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS

		pDI_FnHandler pFn = 0;

	//	if(input->Flags >= 4)	// Break sur présence d'au moins 1 Flag (bit 2 => 0x04) :
	//	{
	//		DIGITAL_INPUT_HALT_IF_DEBUG();
	//	}

		if(input->_newStateEvent)
		{
			pFn = input->_pFnNewStateHandler;
			if(0 != pFn) pFn(DI_EVENT_NEW_STATE, input->_diParam);
			input->_newStateEvent = 0;
		}

		if(input->_newWorkEvent)
		{
			pFn = input->_pFnNewWorkHandler;
			if(0 != pFn) pFn(DI_EVENT_NEW_WORK_STATE, input->_diParam);
			input->_newWorkEvent = 0;
		}

		if(input->_newIdleEvent)
		{
			pFn = input->_pFnNewIdleHandler;
			if(0 != pFn) pFn(DI_EVENT_NEW_IDLE_STATE, input->_diParam);
			input->_newIdleEvent = 0;
		}

		if(input->_newWork1sEvent)
		{
			pFn = input->_pFnWork1sHandler;
			if(0 != pFn) pFn(DI_EVENT_WORK_STATE_1S, input->_diParam);
			input->_newWork1sEvent = 0;
		}

		if(input->_newIdle1sEvent)
		{
			pFn = input->_pFnIdle1sHandler;
			if(0 != pFn) pFn(DI_EVENT_IDLE_STATE_1S, input->_diParam);
			input->_newIdle1sEvent = 0;
		}

		if(input->_newWork3sEvent)
		{
			pFn = input->_pFnWork3sHandler;
			if(0 != pFn) pFn(DI_EVENT_WORK_STATE_3S, input->_diParam);
			input->_newWork3sEvent = 0;
		}

		if(input->_newIdle3sEvent)
		{
			pFn = input->_pFnIdle3sHandler;
			if(0 != pFn) pFn(DI_EVENT_IDLE_STATE_3S, input->_diParam);
			input->_newIdle3sEvent = 0;
		}

		if(input->_newWork10sEvent)
		{
			pFn = input->_pFnWork10sHandler;
			if(0 != pFn) pFn(DI_EVENT_WORK_STATE_10S, input->_diParam);
			input->_newWork10sEvent = 0;
		}

		if(input->_newIdle10sEvent)
		{
			pFn = input->_pFnIdle10sHandler;
			if(0 != pFn) pFn(DI_EVENT_IDLE_STATE_10S, input->_diParam);
			input->_newIdle10sEvent = 0;
		}

		if(input->_newAutoFireEvent)
		{
			pFn = input->_pFnAutoFireHandler;
			if(0 != pFn) pFn(DI_EVENT_AUTO_FIRE, input->_diParam);
			input->_newAutoFireEvent = 0;
		}

#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS

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
void DigitalInputs::Handle_RT_100ms()
{
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
	for (auto input : allInputs)
	{
		// handle DigitalInputs
		//ManageDigitalInputStateMaintained
		if(((input->_curState != 0) && (input->_nbPinOn >= DI_SEUIL_ON))  // State = 1 => Working (indépendant de WorkState) !
		 || ((input->_curState == 0) && (input->_nbPinOn < DI_SEUIL_OFF)))	// State = 0 => Idle
		{

			if(input->_nb100ms < UINT16_MAX) input->_nb100ms++;

			if(input->_curState != 0) // State = 1 => Working (indépendant de WorkState) :
			{
				if(DI_DELAY_1S == input->_nb100ms) input->_newWork1sEvent = 1;
				if(DI_DELAY_3S == input->_nb100ms) input->_newWork3sEvent = 1;
				if(DI_DELAY_10S == input->_nb100ms) input->_newWork10sEvent = 1;
				if(input->_nb100ms >= DI_DELAY_START_AUTO_FIRE)
				{
					if(0 == ((input->_nb100ms - DI_DELAY_START_AUTO_FIRE) % DI_DELAY_SIGNAL_AUTO_FIRE))
					{
						input->_newAutoFireEvent = 1;
					}
				}
			} else { // State = 0 => IDLE :
				if(DI_DELAY_1S == input->_nb100ms) input->_newIdle1sEvent = 1;
				if(DI_DELAY_3S == input->_nb100ms) input->_newIdle3sEvent = 1;
				if(DI_DELAY_10S == input->_nb100ms) input->_newIdle10sEvent = 1;
			}
		}
	}
#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
}

void RegisterDigitalInput2EventFnHandler(uint16_t EventSrc, uint16_t EventId, DigitalInputs *input, pDI_FnHandler pFn)
{
	if(EventSrc & DI_PARAM_NO_1)
	{
		//RegisterDigitalInputEventFnHandler
		if(0 == input) return;
		if(EventId <= 0) return;

		for(int i = 0; i < DI_MAX_FN_HANDLERS; i++)
		{
			if(EventId & (1 << i) || EventSrc & (1 << i)){
				input->setFnHandler(pFn, i);
			}
		}
	}
}

unsigned DigitalInputs::getcurState(void)
{
	return _curState;
}

void DigitalInputs::setcurState(unsigned state)
{
	_curState = state;
}

void DigitalInputs::setFnHandler(pDI_FnHandler pFn,uint8_t index )
{
	if(IS_IN_RANGE(index,0,DI_MAX_FN_HANDLERS)){
		_pFnHandler[index] = pFn;
	}
}
