/*
 * AnalogInputsCore.h
 *
 *  Created on: 7 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
 *
 *  Version 1.0
 *
 */

#ifndef ANALOGINPUTS_ANALOGINPUTSCORE_H_
#define ANALOGINPUTS_ANALOGINPUTSCORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
// Prototypes Publics :

void InitAnalogInputs(void);			// A appeler dans la partie Init Hardware (main.c)
void Gestion_AnalogInputs(void);		// A appeler dans la Boucle Principale (main.c)

void Handle_AnalogInputs_RT_10ms(void);	// A appeler dans la Partie @ 10ms de RunTime

#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSCORE_H_ */
