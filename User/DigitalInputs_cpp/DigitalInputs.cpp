/*
 * DigitalInputs.cpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */

#include "DigitalInputs.hpp"

// Initialisation des variables statiques partagé entre toutes les instances de l'objet
std::list<DigitalInputs*> DigitalInputs::s_allInputs;

/******************************************************************************/
// Pour compatibilité avec la lib BaseDeTemps en C
void GestionDigitalInputs() 		{DigitalInputs::GestionDigitalInputs();}
void Handle_DigitalInputs_RT_1ms() { DigitalInputs::Handle_RT_1ms();}
void Handle_DigitalInputs_RT_10ms() { DigitalInputs::Handle_RT_10ms();}
void Handle_DigitalInputs_RT_100ms() { DigitalInputs::Handle_RT_100ms();}

/******************************************************************************/
DigitalInputs::DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,GPIO_PinState WorkState, E_DI_SAMPLE_FREQ freq) {
	_nbPinOn = 0;
	_nb100ms = 0;
	_Flags = {0};
	_workState = (WorkState == GPIO_PIN_SET);// NO/NF
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_thresoldOn = DI_DEFAULT_SEUIL_ON;
	_thresoldOff = DI_DEFAULT_SEUIL_OFF;
	_sampleFreq = freq;

	s_allInputs.push_back(this);
}

DigitalInputs::DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, uint16_t thresoldOn, uint16_t thresoldOff, E_DI_SAMPLE_FREQ freq) {
	_nbPinOn = 0;
	_nb100ms = 0;
	_Flags = {0};
	_workState = (WorkState == GPIO_PIN_SET);// NO/NF
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_thresoldOn = thresoldOn;
	_thresoldOff = thresoldOff;
	_sampleFreq = freq;

	s_allInputs.push_back(this);
}

DigitalInputs::~DigitalInputs(){
	s_allInputs.remove(this);
}
/******************************************************************************/
void DigitalInputs::GestionDigitalInputs()
{
	for(auto input : s_allInputs)
	{
		// DispatchingDigitalInputEvents
		if(input->_newStateEvent){
			input->newStateHandler();
			input->_newStateEvent = 0;
		}

		if(input->_newWorkEvent){
			input->newWorkHandler();
			input->_newWorkEvent = 0;
		}

		if(input->_newIdleEvent){
			input->newIdleHandler();
			input->_newIdleEvent = 0;
		}

		if(input->_newWork1sEvent){
			input->work1sHandler();
			input->_newWork1sEvent = 0;
		}

		if(input->_newIdle1sEvent){
			input->idle1sHandler();
			input->_newIdle1sEvent = 0;
		}

		if(input->_newWork3sEvent){
			input->work3sHandler();
			input->_newWork3sEvent = 0;
		}

		if(input->_newIdle3sEvent){
			input->idle3sHandler();
			input->_newIdle3sEvent = 0;
		}

		if(input->_newWork10sEvent){
			input->work10sHandler();
			input->_newWork10sEvent = 0;
		}

		if(input->_newIdle10sEvent){
			input->idle10sHandler();
			input->_newIdle10sEvent = 0;
		}

		if(input->_newAutoFireEvent){
			input->autoFireHandler();
			input->_newAutoFireEvent = 0;
		}

	}
}

/****** Echantillonnage ************************************************************************/
void DigitalInputs::Handle_RT_1ms()
{
	for(auto input : s_allInputs)
	{
		// On met a jour l'échantillon cadencé a 1ms
		if(input->_sampleFreq != E_DI_SAMPLE_1ms) continue;

		// handle DigitalInputs
		GPIO_PinState PinState = HAL_GPIO_ReadPin(input->_GPIOPort,input->_GPIOPin);// read pin state
		if(PinState == input->_workState) // Suivant l'Etat "Travail" configuré :
		{
			input->_nbPinOn++;
		}
	}
}

void DigitalInputs::Handle_RT_10ms()
{
	for(auto input : s_allInputs)
	{
		// On met a jour l'échantillon cadencé a 10ms
		if(input->_sampleFreq != E_DI_SAMPLE_10ms) continue;

		// handle DigitalInputs
		GPIO_PinState PinState = HAL_GPIO_ReadPin(input->_GPIOPort,input->_GPIOPin);// read pin state
		if(PinState == input->_workState) // Suivant l'Etat "Travail" configuré :
		{
			input->_nbPinOn++;
		}
	}
}

/****** Controle de l'etat ************************************************************************/
void DigitalInputs::Handle_RT_100ms()
{
	for (auto input : s_allInputs)
	{
		// update DigitalInputState
		unsigned newState = input->_curState; // Par défaut : comme actuellement
		// Gestion Anti-Rebond :
		if(newState > 0)	// Actuellement en position Travail :
		{
			if(input->_nbPinOn < input->_thresoldOff) newState = 0;	// Retour en position IDLE
		} else {			// Actuellement en repos :
			if(input->_nbPinOn >= input->_thresoldOn) newState = 1;	// Passage en position WORKING
		}
		input->_newStateEvent = (input->_curState != newState);
		input->_curState = newState;

		if(input->_newStateEvent > 0)
		{
			if(newState) input->_newWorkEvent = 1;	// Evènement Working now
			else input->_newIdleEvent = 1;			// Evènement Idle now

			input->_nb100ms = 0;					// Relance le Compteur
		}

		//ManageDigitalInputStateMaintained
		if(((input->_curState != 0) && (input->_nbPinOn >= input->_thresoldOn))  // State = 1 => Working (indépendant de WorkState) !
		 || ((input->_curState == 0) && (input->_nbPinOn < input->_thresoldOff)))	// State = 0 => Idle
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
		// reset le compteur pour garder le ratio pinOn/pinOff tout les 100ms
		input->_nbPinOn = 0;
	}
}

/****** Controle de l'etat ************************************************************************/
uint8_t DigitalInputs::getcurState(void) {return (uint8_t)_curState;}

void DigitalInputs::newStateHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::newWorkHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::newIdleHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::work1sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::idle1sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::work3sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::idle3sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::work10sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::idle10sHandler(){
	// Action à définir dans les classe filles
}

void DigitalInputs::autoFireHandler(){
	// Action à définir dans les classe filles
}

