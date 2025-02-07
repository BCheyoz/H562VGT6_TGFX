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

#ifndef LED_CPP_LEDBLINKERINTERFACE_H_
#define LED_CPP_LEDBLINKERINTERFACE_H_

#include "main.h"				// Pour accès au GPIO des Leds

typedef enum
{
	E_LED_OFF = 0,		// La Led sera maintenue éteinte
	E_LED_FIX,				// La Led sera maintenue allumée en permanence
	E_LED_SLOW_BLINK,		// La Led va effectuer un Clignotement lent
	E_LED_FAST_BLINK,		// La Led va effectuer le Clignotement Rapide #1
	E_LED_VERY_FAST_BLINK,	// La Led va effectuer le Clignotement Rapide #2
	E_LED_HEARTBEAT_BLINK,	// La Led va effectuer le Clignotement facon battement de coeur
	//---------------
	E_LED_NB_ETATS // A conserver en dernier élément : indique le nb Max d'états possibles
} E_LED_BLINK_MODES;

/********************************************************************************************/
// pour compatibilité avec la lib BaseDeTemps en C
#ifdef __cplusplus
extern "C" {
#endif
  void Handle_Led_RT_100ms();
#ifdef __cplusplus
}
#endif
/********************************************************************************************/

#endif /* LED_CPP_LEDBLINKERINTERFACE_H_ */
