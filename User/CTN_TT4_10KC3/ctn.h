/*
 * ctn.h
 *
 *  Created on: Feb 20, 2025
 *      Author: m.faget
 */

#ifndef CTN_TT4_10KC3_CTN_H_
#define CTN_TT4_10KC3_CTN_H_

#include <stdint.h>

#define SIZE_TAB_CTN        146
#define TEMPERATURE_MIN     -4000
#define TEMPERATURE_MAX     10500
#define ADC 0
#define TEMP 1

int16_t convertADC_to_CTN_10K(uint16_t Val_ADC);



#endif /* CTN_TT4_10KC3_CTN_H_ */
