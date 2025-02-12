/*
 * FanPwmIcConf.h
 *
 *  Created on: 15 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 10 Fev. 2025
 *  Updated by: m.faget
 *
 *  Version 1.0
 *
 */

#ifndef FAN_PWM_IC_FAN_PWM_IC_CONF_H_
#define FAN_PWM_IC_FAN_PWM_IC_CONF_H_

/*****************************************************************************************
* Procédure pour intégrer facilement cette Librairie "FanPwmIC" dans un nouveau projet : *
******************************************************************************************


// Configuration des Variables pour les Datas :
#define FPIC_NB_FANS_WITH_FEEDBACK		1	// Indiquer le nombre de Ventilateurs AVEC retour Feedback à gérer
#define FPIC_NB_FANS_ONLY_COMMAND		0	// Indiquer le nombre de Ventilateurs SANS feedback à gérer
#define FPIC_NB_FANS_ONLY_FEEDBACK		0	// Indiquer le nombre d'entrées Feedback SANS commande à gérer

// Configuration du nombre total d'éléments à gérer :
#define FPIC_NB_MAX_OF_INIT_PARAMS		1 // 1 seul Ventilateur Configuré sur HII_CarteMere_App
//#define FPI_NB_MAX_OF_INIT_PARAMS		1+1 // 2 pour essais avec 1 vide

// Configuration de la Moyenne glissante :
#define FPIC_MOY_FEED_BACK_CONV 		10 // Pour calculer une Moyenne sur 10 intervalles de temps

// Configuration des Options :
#define FPIC_GET_LAST_DELTA_TIME		// Pour demander à récupérer le dernier deltaTime calculé
#define FPIC_GET_LAST_FEED_BACK 		// Pour demander à récupérer le dernier résultat de calcul Feedback
//#define FPIC_FEED_BACK_IS_FLOAT 		// Pour demander une précision accrue sur le calcul du Feedback

// Autorisations de Dynamic Swap (PWM & InputCaptures) :
#define FPIC_ENABLE_SWAP_PWM_COMMANDS	// Pour activer la gestion du Swap entre les commandes PWM
#define FPIC_ENABLE_SWAP_IC_FEED_BACK	// Pour activer la gestion du Swap entre les entrées InputCapture de Feedback

/*******************************************************************************************/
// Fonction d'appel en Cascade si l'une des InputCapture n'est pas gérée par FanPwmIC :

//#define NEXT_HAL_TIM_IC_CaptureCallback 	USER_HAL_TIM_IC_CaptureCallback2	// Remplacer par un nom User TIM_IC_CaptureCallback approprié

/*******************************************************************************************/
// Tableau d'Init des Paramètres :

#define FPIC_FIRST_INIT_PARAM	FPIC_BaseInitParam	// Nom de la Base du Tableau d'Init des Paramètres
#define FPIC_AFTER_INIT_PARAM	FPIC_EndInitParams	// Nom de Fin du Tableau d'Init des Paramètres

/*******************************************************************************************/
// Synthèse des InputCaptures nécessaires :

#if defined(FPIC_NB_FANS_WITH_FEEDBACK) && (FPIC_NB_FANS_WITH_FEEDBACK > 0)
	#if defined(FPIC_NB_FANS_ONLY_FEEDBACK) && (FPIC_NB_FANS_ONLY_FEEDBACK > 0)
		#define FPIC_NB_TOTAL_INPUT_CAPTURES	( (FPIC_NB_FANS_WITH_FEEDBACK) + (FPIC_NB_FANS_ONLY_FEEDBACK) )
	#else // !FPIC_NB_FANS_ONLY_FEEDBACK
		#define FPIC_NB_TOTAL_INPUT_CAPTURES	(FPIC_NB_FANS_WITH_FEEDBACK)
	#endif // FPIC_NB_FANS_ONLY_FEEDBACK
#else // !FPIC_NB_FANS_WITH_FEEDBACK
	#if defined(FPIC_NB_FANS_ONLY_FEEDBACK) && (FPIC_NB_FANS_ONLY_FEEDBACK > 0)
		#define FPIC_NB_TOTAL_INPUT_CAPTURES	(FPIC_NB_FANS_ONLY_FEEDBACK)
	#else // !FPIC_NB_FANS_ONLY_FEEDBACK
		#define FPIC_NB_TOTAL_INPUT_CAPTURES	0
	#endif // FPIC_NB_FANS_ONLY_FEEDBACK
#endif // FPIC_NB_FANS_WITH_FEEDBACK

#endif /* FAN_PWM_IC_FAN_PWM_IC_CONF_H_ */
