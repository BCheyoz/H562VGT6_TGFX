/*
 * DigitalInputs.hpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */
#include <list>
#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "main.h"
#include "DigitalInputsInterface.h"

/* declaration des entrees example
	anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NO_WORKING_STATE_IS_1);
	input_D2 = new DigitalInputs(input_D2_GPIO_Port, input_D2_Pin, DI_NO_WORKING_STATE_IS_1);
*/


/***************************************/
class DigitalInputs {
public:
	DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState);// constructor prototype

	static void GestionDigitalInputs();
	static void Handle_RT_10ms();
	static void Handle_RT_100ms();// TODO gestion des evenements (appuis long,...)

	// accesseurs et mutateurs
	// set
	void setState(unsigned state);
	// get
	unsigned getState(void);

private :

	uint16_t _nbPinOn;

	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;

	union {
		struct {
			unsigned _curState:1;
			unsigned _workState:1;
			/* TODO gestion des evenements (appuis long,...)
			// Event Flags :
			unsigned _newStateEvent:1;	// Changement de State Work <-> Idle
			unsigned _newWorkEvent:1;	// Basculement Idle -> Work state
			unsigned _newIdleEvent:1;	// Basculement Work -> Idle state
			unsigned _newWork1sEvent:1;	// State Work depuis 1s
			unsigned _newIdle1sEvent:1;	// State Idle depuis 1s
			unsigned _newWork3sEvent:1;	// State Work depuis 3s
			unsigned _newIdle3sEvent:1;	// State Idle depuis 3s
			unsigned _newWork10sEvent:1;	// State Work depuis 10s
			unsigned _newIdle10sEvent:1;	// State Idle depuis 10s
			unsigned _newAutoFireEvent:1;// AutoFire en State Work
			*/
		};
		uint16_t _Flags;
	};

	static std::list<DigitalInputs*> allInputs;

};

