/*
 * AnalogInputsUser.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
 *
 *  Version 1.0
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

/******** CTN TT4 10CK3 part ****************************************************************/

typedef struct {
	uint16_t nbPtADC;
	int16_t TempValue;
} tAI_IntValue;

#define SIZE_TAB_CTN        146
#define TEMPERATURE_MIN     -4000
#define TEMPERATURE_MAX     10500
#define ADC 0
#define TEMP 1

const int TableConversionsAdc12bCtn3977[2][SIZE_TAB_CTN] = {{3980,3972,3964,3955,3946,3936,3926,3915,3904,3892,3880,3866,3853,3838,3823,
        3807,3791,3773,3755,3736,3716,3696,3675,3652,3629,3605,3581,3555,3528,3501,3472,3443,3413,3381,3349,3316,3282,3248,3212,
        3176,3139,3101,3062,3022,2982,2941,2900,2858,2815,2772,2728,2684,2639,2595,2549,2504,2459,2413,2367,2321,2275,2229,
        2184,2138,2093,2048,2003,1958,1914,1870,1826,1783,1741,1699,1657,1616,1576,1536,1497,1459,1421,1384,1347,1312,1277,
        1242,1209,1176,1144,1112,1082,1052,1022,994,966,939,912,886,861,837,813,790,767,745,724,703,683,663,644,626,608,590,
        573,557,541,525,510,496,482,468,455,442,429,417,405,394,383,372,362,352,342,333,323,314,306,297,289,281,274,266,259,
        252,245,239,233,226},
        {-40,-39,-38,-37,-36,-35,-34, -33,-32, -31,-30, -29, -28, -27,-26,-25,-24,-23, -22, -21,-20,-19,-18, -17, -16,  -15,-14, -13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
        26, 27,28,29,30,31,32,33,34,35,36,37,38,39,40, 41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,
        94, 95,96,97,98,99,100, 101,102, 103,104,105}
           };

/******************************************************************************/
extern tAI_FloatValue tAiRefAlim;
extern tAI_IntValue tAi1_T1;
extern tAI_FloatValue tAi0_T2;
extern tAI_FloatValue tAi18_T3;
extern tAI_FloatValue tAi15_T4;
extern tAI_FloatValue tAi14_T5;

#if defined(AI_ENABLE_SUPPORT_STATS) && (AI_NB_MAX_OF_INIT_PARAMS > 0)
	extern tAiStats mAiStats[AI_NB_MAX_OF_INIT_PARAMS];
#endif // AI_ENABLE_SUPPORT_STATS && AI_NB_MAX_OF_INIT_PARAMS

/******************************************************************************/

uint16_t getAi18_T3_x10(void);
uint16_t getAi15_T4_x10(void);
uint16_t getAi14_T5_x10(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSUSER_H_ */
