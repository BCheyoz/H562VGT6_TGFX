/*
 * AnalogInputsUser.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 09 Apr. 2025
 *  Updated by: j.proux
 *
 *  Version 1.0
 *
 */

#ifndef ANALOGINPUTS_ANALOGINPUTSUSER_H_
#define ANALOGINPUTS_ANALOGINPUTSUSER_H_

#include "AnalogInputsConf.h"		// Pour accès à notre propre Config & Outils utiles

#include "ctn_tt4_10kc3.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NB_CTN_TT4_10KC3_USE 5

/********************************************************************************************/

typedef struct {
	uint16_t nbPtADC;
	float value;
} tAI_FloatValue;

typedef struct {
	uint16_t nbPtADC;
	int16_t value;
} tAI_IntValue;

/******************************************************************************/
extern tAI_FloatValue tAiRefAlim;
extern tAI_IntValue tAi_CTN[NB_CTN_TT4_10KC3_USE];

#if defined(AI_ENABLE_SUPPORT_STATS) && (AI_NB_MAX_OF_INIT_PARAMS > 0)
	extern tAiStats mAiStats[AI_NB_MAX_OF_INIT_PARAMS];
#endif // AI_ENABLE_SUPPORT_STATS && AI_NB_MAX_OF_INIT_PARAMS

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSUSER_H_ */
