/*
 * AppointElecInterface.h
 *
 *  Created on: Mar 6, 2025
 *      Author: m.faget
 */

#ifndef APPOINTELEC_CPP_APPOINTELECINTERFACE_H_
#define APPOINTELEC_CPP_APPOINTELECINTERFACE_H_

#include "main.h"				// Pour accès au GPIO des Leds

typedef enum
{
	E_APPOINT_ELEC_OFF = 0,		// L'appoint elec est désactivé
	E_APPOINT_ELEC_ON,				// L'appoint elec est actif
} E_APPOINT_ELEC_MODES;

#endif /* APPOINTELEC_CPP_APPOINTELECINTERFACE_H_ */
