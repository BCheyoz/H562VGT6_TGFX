/*
 * AppointElec.hpp
 *
 *  Created on: Mar 6, 2025
 *      Author: m.faget
 */

#include "main.h"

class AppointElec {
public :

	AppointElec(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState mode = GPIO_PIN_RESET);

	GPIO_PinState GetMode(); 				// Pour récupérer le Mode dans lequel est l'appoint elec
	void SetMode(GPIO_PinState newMode);	// Pour changer le Mode de l'appoint elec depuis l'extérieur

private :
	GPIO_PinState _mode;

	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;
};
