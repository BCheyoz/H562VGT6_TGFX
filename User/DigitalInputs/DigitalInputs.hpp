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
	DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState,uint16_t diParam);// constructor prototype

	static void GestionDigitalInputs();
	static void Handle_RT_10ms();
	static void Handle_RT_100ms();

	// accesseurs et mutateurs
	// set
	void setcurState(unsigned state);
	void setFnHandler(pDI_FnHandler pFn,uint8_t index );
	// get
	unsigned getcurState(void);

private :
	uint16_t _nbPinOn;
	uint16_t _nb100ms;
	uint16_t _diParam;

	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;

	union {
		struct {
			unsigned _curState:1;
			unsigned _workState:1;
			unsigned _newStateEvent:1;	// Changement de State Work <-> Idle
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
			unsigned _newWorkEvent:1;	// Basculement Idle -> Work state
			unsigned _newIdleEvent:1;	// Basculement Work -> Idle state
			unsigned _newWork1sEvent:1;	// State Work depuis 1s
			unsigned _newIdle1sEvent:1;	// State Idle depuis 1s
			unsigned _newWork3sEvent:1;	// State Work depuis 3s
			unsigned _newIdle3sEvent:1;	// State Idle depuis 3s
			unsigned _newWork10sEvent:1;	// State Work depuis 10s
			unsigned _newIdle10sEvent:1;	// State Idle depuis 10s
			unsigned _newAutoFireEvent:1;// AutoFire en State Work
#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
		};
		uint16_t _Flags;
	};

	// Event Handlers :
#ifndef DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
	union {
		struct {
			pDI_FnHandler _pFnNewStateHandler;
			pDI_FnHandler _pFnNewWorkHandler;
			pDI_FnHandler _pFnNewIdleHandler;
			pDI_FnHandler _pFnWork1sHandler;
			pDI_FnHandler _pFnIdle1sHandler;
			pDI_FnHandler _pFnWork3sHandler;
			pDI_FnHandler _pFnIdle3sHandler;
			pDI_FnHandler _pFnWork10sHandler;
			pDI_FnHandler _pFnIdle10sHandler;
			pDI_FnHandler _pFnAutoFireHandler;
		};
		pDI_FnHandler _pFnHandler[DI_MAX_FN_HANDLERS];
	};
#endif // !DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS

	static std::vector<DigitalInputs*> allInputs;
};

void RegisterDigitalInput2EventFnHandler(uint16_t EventSrc, uint16_t EventFlags, DigitalInputs *input, pDI_FnHandler pFn); // Configurer le Handler de divers évènements supportés nativement

