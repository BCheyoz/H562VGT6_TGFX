/*
 * FanPwmIcCore.h
 *
 *  Created on: 17 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 04 Mars 2022
 *  Updated by: j.proux
 *
 *  Version 1.0
 *
 */

#ifndef FANPWMIC_FANPWMICCORE_H_
#define FANPWMIC_FANPWMICCORE_H_

#include "FanPwmIcUtils.h"			// Pour accès aux Outils d'aide à la Configuration

#define REFRAME_TO_MAX_OF_RANGE(val,min,max)	if( ((val) < (min)) || ((val) > (max)) ) (val) = (max)

/******************************************************************************/
// Prototypes Publics :

void InitFanPwmIC(void);			// A appeler dans la partie Init Software (main.c)
void Gestion_FanPwmIC(void);		// A appeler dans la Boucle Principale (main.c)

void Handle_FanPwmIC_RT_10ms(void);	// A appeler dans la Partie @ 10ms de RunTime

#endif /* FANPWMIC_FANPWMICCORE_H_ */
