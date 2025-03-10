/*
 * AppointElec.hpp
 *
 *  Created on: Mar 6, 2025
 *      Author: m.faget
 */

#include "main.h"

typedef enum
{
	E_APPOINT_ELEC_OFF = 0,		// L'appoint elec est désactivé
	E_APPOINT_ELEC_ON,				// L'appoint elec est actif
} E_APPOINT_ELEC_MODES;

/******************************************************************************/
class AppointElec {
public :

	AppointElec(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, E_APPOINT_ELEC_MODES mode = E_APPOINT_ELEC_OFF);

	E_APPOINT_ELEC_MODES GetMode(); // Pour récupérer le Mode dans lequel est l'appoint elec
	void SetMode(E_APPOINT_ELEC_MODES newMode);	// Pour changer le Mode de l'appoint elec depuis l'extérieur

private :
	E_APPOINT_ELEC_MODES _mode;

	GPIO_TypeDef *_GPIOPort;
	uint16_t _GPIOPin;
};
