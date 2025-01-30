/*
 * BaseDeTemps.c
 *
 *  Created on: 30 juil. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 16 Janv. 2024
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "BaseDeTemps" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "BaseDeTemps.h"
 *
 */

#include <LedBlinker.hpp>
#include "BaseDeTemps.h"		// Pour accès à nos propres déclarations publiques

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/******************************************************************************/
// Zone des Variables utilisées dans l'Interruption Système :
// => Initialisation à la déclaration nécessaire !
#define NB_1ms_IN_10ms	10
int nb1ms = 0;
int flag10msIT = 0;

/******************************************************************************/
// Zone des Variables utilisées dans le Programe Principal :
// => Initialisations dans la Fonction d'Init ...
int nb10msRT; int nb100msRT;
#define NB_10ms_IN_100ms 10
#define NB_100ms_IN_1s	10

#ifdef BDT_SUPPORT_ALIVE_RT
	uint32_t AliveRunTime100ms;
	uint32_t AliveRunTime1s;
#endif // BDT_SUPPORT_ALIVE_RT

/******************************************************************************/

inline __attribute__((always_inline)) void InitBaseDeTemps(void)
{
	// Initialise ici les Variables utilisées au RunTime :
	nb10msRT = 0;
	nb100msRT = 0;

#ifdef BDT_SUPPORT_ALIVE_RT
	AliveRunTime1s = 0;
#endif // BDT_SUPPORT_ALIVE_RT

}

/******************************************************************************/

inline __attribute__((always_inline)) void HandleBaseDeTemps_IT_1ms(void)
{
// Début de Zone des Appels exécutés dans l'Interruption Sytème @ 1ms :

	/* USER CODE BEGIN IT_1ms */

	/* USER CODE END IT_1ms */

// Fin de Zone des Appels exécutés dans l'Interruption Sytème @ 1ms.

	if(++nb1ms >= NB_1ms_IN_10ms)
	{	 nb1ms -= NB_1ms_IN_10ms;

// Début de Zone des Appels exécutés dans l'Interruption Système @ 10ms :

	/* USER CODE BEGIN IT_10ms */

	/* USER CODE END IT_10ms */

// Fin de Zone des Appels exécutés dans l'Interruption Système @ 10ms.

		flag10msIT = 1; // Terminer par signaler au Programme Principal
	}
}

/******************************************************************************/

inline __attribute__((always_inline)) void GestionBaseDeTemps(void)
{
	if(flag10msIT != 0)
	{	flag10msIT = 0;

// Début de Zone des Appels exécutés au RunTime dans le Programme Principal @ 10ms :

	/* USER CODE BEGIN RT_10ms */


// Ajout_Jp for MultiExecution and Capture I2c avec PulseView @ 500K :
#ifdef RV3028_RTC_ENABLE_MANUAL_RW // from "I2cDevRtc_RV3028.h"
	#ifdef I2CCM_ENABLE_DEBUG_MULTI_EXECUTION // from "I2cComMasterConf.h"
		if( (0 == nb100msRT) ) // Permet d'exécuter chaque 10ms, pendant 100ms, 1 fois par seconde
		{
			//I2cSystem_HandleRtcManualMultiExecute_10ms();
		}
	#endif // I2C_ENABLE_DEBUG_MULTI_EXECUTION
#endif // RV3028_RTC_ENABLE_MANUAL_RW

	//	UserFn_RT_10ms();	// Exemple de fonction à exécuter au RunTime dans le Programme Principal @ 10ms

	/* USER CODE END RT_10ms */

// Fin de Zone des Appels exécutés au RunTime dans le Programme Principal @ 10ms.

		++nb10msRT;
		if(nb10msRT >= NB_10ms_IN_100ms)
		{
			nb10msRT -= NB_10ms_IN_100ms;

// Début de Zone des Appels exécutés au RumTime dans le Programme Principal @ 100ms :

		/* USER CODE BEGIN RT_100ms */
#ifdef BDT_SUPPORT_ALIVE_RT
			AliveRunTime100ms++;
#endif // BDT_SUPPORT_ALIVE_RT

			Handle_GestionLed_RT_100ms();

		//	UserFn_RT_100ms();	// Exemple de fonction à exécuter au RunTime dans le Programme Principal @ 100ms

		/* USER CODE END RT_100ms */

// Fin de Zone des Appels exécutés au RumTime dans le Programme Principal @ 100ms.

			++nb100msRT;
			if(nb100msRT >= NB_100ms_IN_1s)
			{
				nb100msRT -= NB_100ms_IN_1s;

#ifdef BDT_SUPPORT_ALIVE_RT
				AliveRunTime1s++;
#endif // BDT_SUPPORT_ALIVE_RT

// Début de Zone des Appels exécutés au RumTime dans le Programme Principal @ 1s :

			/* USER CODE BEGIN RT_1s */


			//	UserFn_RT_1s();	// Exemple de fonction à exécuter au RunTime dans le Programme Principal @ 1s

			/* USER CODE END RT_1s */

// Fin de Zone des Appels exécutés au RumTime dans le Programme Principal @ 1s.
			}
		}
	}
}

/******************************************************************************/

uint32_t getAliveRunTime100ms(void)
{

#ifdef BDT_SUPPORT_ALIVE_RT
    return AliveRunTime100ms;
#else // ! BDT_SUPPORT_ALIVE_RT
    return UINT32_MAX;	// -1 = Function Disabled
#endif // BDT_SUPPORT_ALIVE_RT

}

/******************************************************************************/

uint32_t getAliveRunTime1s(void)
{

#ifdef BDT_SUPPORT_ALIVE_RT
    return AliveRunTime1s;
#else // ! BDT_SUPPORT_ALIVE_RT
    return UINT32_MAX;	// -1 = Function Disabled
#endif // BDT_SUPPORT_ALIVE_RT

}
