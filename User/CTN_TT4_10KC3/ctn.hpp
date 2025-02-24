/*
 * ctn.hpp
 *
 *  Created on: Feb 20, 2025
 *      Author: m.faget
 */

#ifndef CTN_TT4_10KC3_CTN_HPP_
#define CTN_TT4_10KC3_CTN_HPP_

#include <stdint.h>

#define SIZE_TAB_CTN        146
#define TEMPERATURE_MIN     -4000
#define TEMPERATURE_MAX     10500
#define ADC 0
#define TEMP 1

// pour compatibilité avec la lib AnalogInputs en C
#ifdef __cplusplus
extern "C" {
#endif
	int16_t convertADC_to_CTN_10K(uint16_t Val_ADC);
#ifdef __cplusplus
}
#endif


#endif /* CTN_TT4_10KC3_CTN_HPP_ */
