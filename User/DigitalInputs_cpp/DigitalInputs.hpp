/*
 * DigitalInputs.hpp
 *
 *  Created on: Mar 4, 2025
 *      Author: m.faget
 */

#pragma once

#include <list>
#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "main.h"
#include "DigitalInputsInterface.h"

/******************************************************************************/
// Configuration des Délais de controle sur une fréqunce de 100ms :

#define DI_DELAY_1S 	10		// 1S = 10 * 100ms
#define DI_DELAY_3S 	30		// 3S = 30 * 100ms
#define DI_DELAY_10S	100		// 10S = 100 * 100ms
#define DI_DELAY_START_AUTO_FIRE	15	// 15 * 100ms = 1.5s
#define DI_DELAY_SIGNAL_AUTO_FIRE	2	// 2 * 100ms = 0.2s
/***************************************/


class DigitalInputs {
public:
	DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, E_DI_SAMPLE_FREQ freq = E_DI_SAMPLE_10ms);
	DigitalInputs(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, uint16_t thresoldOn, uint16_t thresoldOff, E_DI_SAMPLE_FREQ freq = E_DI_SAMPLE_10ms);
	virtual ~DigitalInputs();

	static void GestionDigitalInputs();
	static void Handle_RT_1ms();
	static void Handle_RT_10ms();
	static void Handle_RT_100ms();

	// get
	virtual uint8_t getcurState(void);
	inline uint16_t getFlags(void) {return _Flags;}

protected :
	uint16_t _nbPinOn;
	uint16_t _nb100ms;
	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;
	// Configuration de la plage Anti-Rebonds :
	uint16_t _thresoldOn;	// Valeur Mini Considérée comme ON
	uint16_t _thresoldOff;	// Valeur Maxi considérée comme OFF
	E_DI_SAMPLE_FREQ _sampleFreq;

	union {
		struct {
			unsigned _curState:1;
			unsigned _workState:1;
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
		};
		uint16_t _Flags;
	};

	static std::list<DigitalInputs*> s_allInputs;

	virtual void newStateHandler();
	virtual void newWorkHandler();
	virtual void newIdleHandler();
	virtual void work1sHandler();
	virtual void idle1sHandler();
	virtual void work3sHandler();
	virtual void idle3sHandler();
	virtual void work10sHandler();
	virtual void idle10sHandler();
	virtual void autoFireHandler();
};

