/*
 * LedBlinker.h
 *
 *  Created on: 5 décembre 2024
 *  Original Author: b.chhay
 *
 *  Updated on: 5 décembre 2024
 *  Updated by: b.chhay
 *
 *  Version 1.1
 *
 */

#pragma once

#include <list>
#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "LedBlinkerInterface.h" 	// Pour l'interface en C et accès au GPIO des Leds

/********************************************************************************************/

class LedBlinker {
public :

	LedBlinker(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, E_LED_BLINK_MODES newBlinkMode = E_LED_OFF);

	static void Handle_RT_100ms();	// A appeler dans une BaseDeTemps @ 100ms

	E_LED_BLINK_MODES GetBlinkMode(); 				// Pour récupérer le Mode de Clignotement actuel
	void SetBlinkMode(E_LED_BLINK_MODES newBlinkMode);	// Pour changer le Mode de Clignotement depuis l'extérieur

private :
	E_LED_BLINK_MODES _mode;
	uint16_t _sabLed;
	uint8_t _curLed;
	uint8_t _nbSwitch;
	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;

	static std::list<LedBlinker*> s_allLeds;

	void updateLedState(uint8_t s);
};

