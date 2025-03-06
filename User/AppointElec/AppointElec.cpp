/*
 * AppointElec.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: m.faget
 */

#include "AppointElec.hpp"	// Pour accès à nos propres déclarations publiques



AppointElec::AppointElec(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, E_APPOINT_ELEC_MODES newMode)
{
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_mode = newMode;
}

E_APPOINT_ELEC_MODES AppointElec::GetMode(){
	return _mode;
}

void AppointElec::SetMode(E_APPOINT_ELEC_MODES newMode){
	HAL_GPIO_WritePin(_GPIOPort, _GPIOPin, (GPIO_PinState) newMode);
}
