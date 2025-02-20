/*
 * AnalogInputsUser.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
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
extern tAI_FloatValue tAi1_T1;
extern tAI_FloatValue tAi0_T2;
extern tAI_FloatValue tAi18_T3;
extern tAI_FloatValue tAi15_T4;
extern tAI_FloatValue tAi14_T5;

#if defined(AI_ENABLE_SUPPORT_STATS) && (AI_NB_MAX_OF_INIT_PARAMS > 0)
	extern tAiStats mAiStats[AI_NB_MAX_OF_INIT_PARAMS];
#endif // AI_ENABLE_SUPPORT_STATS && AI_NB_MAX_OF_INIT_PARAMS

/******************************************************************************/

uint16_t getAi1_T1_x10(void);
uint16_t getAi0_T2_x10(void);
uint16_t getAi18_T3_x10(void);
uint16_t getAi15_T4_x10(void);
uint16_t getAi14_T5_x10(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSUSER_H_ */
