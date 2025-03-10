/*
 * AppointElec.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: m.faget
 */

#include "AppointElec.hpp"	// Pour accès à nos propres déclarations publiques



AppointElec::AppointElec(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState newMode)
{
	_GPIOPort = GPIOx;
	_GPIOPin = GPIO_Pin;
	_mode = newMode;
	this->SetMode(_mode);
}

GPIO_PinState AppointElec::GetMode(){
	return HAL_GPIO_ReadPin(_GPIOPort,_GPIOPin);
}

void AppointElec::SetMode(GPIO_PinState newMode){
	HAL_GPIO_WritePin(_GPIOPort, _GPIOPin, (GPIO_PinState) newMode);
}
