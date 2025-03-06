/*
 * DigitalInputs.hpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */
#include <vector>
#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "main.h"
#include "DigitalInputsInterface.h"

/***************************************/
class DigitalInputs {
public:
	DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState);// constructor prototype

	static void GestionDigitalInputs();
	static void Handle_RT_10ms();

	// accesseurs et mutateurs
	// set
	void setcurState(unsigned state);
	// get
	unsigned getcurState(void);

private :
	uint16_t _nbPinOn;

	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;

	union {
		struct {
			unsigned _curState:1;
			unsigned _workState:1;
		};
		uint16_t _Flags;
	};

	static std::vector<DigitalInputs*> allInputs;
};

