/*
 * ctn.h
 *
 *  Created on: Feb 20, 2025
 *  Author: m.faget
 *
 *  Updated on: 27 Feb. 2025
 *  Updated by: b.chhay
 *
 *  Version : 1.1
 */

#ifndef CTN_TT4_10KC3_CTN_H_
#define CTN_TT4_10KC3_CTN_H_

#include <stdint.h>

#define NB_CTN_USE 5

typedef struct {
	uint16_t nbPtADC;
	int16_t value;
} tAI_IntValue;

extern tAI_IntValue tAi_CTN[NB_CTN_USE];

void AnalogInput_HandleNewFloat_CTN(void* pVar, float newValue);



#endif /* CTN_TT4_10KC3_CTN_H_ */
