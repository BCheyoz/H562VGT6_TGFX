/*
 * LedBlinker.h
 *
 *  Created on: 5 décembre 2024
 *  Original Author: b.chhay
 *
 *  Updated on: 5 décembre 2024
 *  Updated by: b.chhay
 *
 *  Version 1.0
 *
 */

#pragma once

#include <list>
#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "main.h"				// Pour accès au GPIO des Leds

typedef enum
{
	E_LED_ETEINTE = 0,		// La Led sera maintenue éteinte
	E_LED_FIXE,				// La Led sera maintenue allumée en permanence
	E_LED_CLIGN_LENT,		// La Led va effectuer un Clignotement lent
	E_LED_CLIGN_RAPIDE_1,	// La Led va effectuer le Clignotement Rapide #1
	E_LED_CLIGN_RAPIDE_2,	// La Led va effectuer le Clignotement Rapide #2
	//---------------
	E_LED_NB_ETATS // A conserver en dernier élément : indique le nb Max d'états possibles
} E_LED_BLINK_MODES;

/********************************************************************************************/
// pour compatibilité avec la lib BaseDeTemps en C
#ifdef __cplusplus
extern "C" {
#endif
  void Handle_GestionLed_RT_100ms();
#ifdef __cplusplus
}
#endif
/********************************************************************************************/

class LedBlinker {
public :

	LedBlinker(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, E_LED_BLINK_MODES newBlinkMode = E_LED_ETEINTE);

	static void Handle_RT_100ms();	// A appeler dans une BaseDeTemps @ 100ms

	E_LED_BLINK_MODES GetBlinkMode(); 				// Pour récupérer le Mode de Clignotement actuel
	void SetBlinkMode(E_LED_BLINK_MODES newBlinkMode);	// Pour changer le Mode de Clignotement depuis l'extérieur

private :
	E_LED_BLINK_MODES _mode;
	uint16_t _sabLed;
	uint8_t _curLed;
	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;

	static std::list<LedBlinker*> s_allLeds;

	void updateLedState(uint8_t s);
};

