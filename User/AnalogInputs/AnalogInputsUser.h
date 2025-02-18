/*
 * AnalogInputsUser.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
 *
 *  Pour intégrer facilement cette Librairie "AnalogInputs" dans un nouveau Projet :
 *   -> Suivre les indications dans "AnalogInputsConf.h"
 *
 */

#ifndef ANALOGINPUTS_ANALOGINPUTSUSER_H_
#define ANALOGINPUTS_ANALOGINPUTSUSER_H_

#include "AnalogInputsConf.h"		// Pour accès à notre propre Config & Outils utiles

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************************/

typedef struct {
	uint16_t nbPtADC;
	float value;
} tAI_FloatValue;

/******************************************************************************/

extern tAI_FloatValue tAiRefAlim;
extern tAI_FloatValue tAi0_0_10V;
extern tAI_FloatValue tAi1_0_10V;
extern tAI_FloatValue tAi2_0_10V;

#if defined(AI_ENABLE_SUPPORT_STATS) && (AI_NB_MAX_OF_INIT_PARAMS > 0)
	extern tAiStats mAiStats[AI_NB_MAX_OF_INIT_PARAMS];
#endif // AI_ENABLE_SUPPORT_STATS && AI_NB_MAX_OF_INIT_PARAMS

/******************************************************************************/

uint8_t getValueForMemHistoA0_0_10V(void);
uint8_t getValueForMemHistoA1_0_10V(void);
uint8_t getValueForMemHistoA2_0_10V(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSUSER_H_ */
