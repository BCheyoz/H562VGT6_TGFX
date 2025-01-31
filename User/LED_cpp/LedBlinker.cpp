/*
 * GestionLedAlive.c
 *
 *  Created on: 9 août 2021
 *  Original Author: j.proux
 *
 *  Updated on: 19 Déc. 2023
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "GestionLed" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "GestionLed.h"
 *
 */

#include <LedBlinker.hpp>	// Pour accès à nos propres déclarations publiques


/******************************************************************************/
// Pour compatibilité avec la lib BaseDeTemps en C
inline __attribute__((always_inline)) void Handle_GestionLed_RT_100ms() { LedBlinker::Handle_RT_100ms(); }

/******************************************************************************/

LedBlinker::LedBlinker(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, E_LED_BLINK_MODES newBlinkMode)
{
	_sabLed = 0;
	_curLed = 0;
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_mode = newBlinkMode;

	s_allLeds.push_front(this);
}

/******************************************************************************/

void LedBlinker::Handle_RT_100ms(void) // Gestion Clignotement Led de vie
{
	for (auto led : s_allLeds){

		if(led->_sabLed <= 0) // <= 0 car décomptage systématique en fin de Traitement (mettre "<=1" si c'est dans Else)
		{
			led->_curLed++;

			switch (led->_mode) {
				case E_LED_ETEINTE:
					led->_curLed += (led->_curLed & 1); // Retomber sur un chiffre pair pour être sûr de la maintenir éteinte
					led->_sabLed = 5; // Recalculer dans 5*100ms = 500ms
					break;
				case E_LED_FIXE:
					led->_curLed |= 1; // Forcer ON
					led->_sabLed = 5; // Recalculer dans 5*100ms = 500ms
					break;
				case E_LED_CLIGN_LENT:
					led->_sabLed = (led->_curLed & 1) ? 5 : 5; // 5=500ms => 500ms ON + 500ms OFF
					break;
				case E_LED_CLIGN_RAPIDE_1:
					led->_sabLed = (led->_curLed & 1) ? 1 : 1; // 1=100ms => 100ms ON + 100ms OFF
					break;
				case E_LED_CLIGN_RAPIDE_2:
					led->_sabLed = (led->_curLed & 1) ? 2 : 2; // 2=200ms => 200ms ON + 200ms OFF
					break;
				default:
					// Normalement, on ne devrait jamais passer par ici
					led->_sabLed = 10; // Recalculer dans 10*100ms = 1s
					break;
			}
			led->updateLedState(led->_curLed & 1);
		}
		led->_sabLed--;
	}
}

/******************************************************************************/

E_LED_BLINK_MODES LedBlinker::GetBlinkMode(){
	return _mode;
}

void LedBlinker::SetBlinkMode(E_LED_BLINK_MODES newBlinkMode){
	_mode = newBlinkMode;
}

void LedBlinker::updateLedState(uint8_t s){
	HAL_GPIO_WritePin(_GPIOPort, _GPIOPin, (GPIO_PinState) s);
}
