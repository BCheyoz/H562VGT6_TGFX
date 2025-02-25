/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:29:41.104 for ALDES © 2024 !
 * AnalogInputsConf.h
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

#ifndef ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_
#define ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "AnalogInputsUtils.h"	// Pour accès aux Outils d'aide à la Configuration

// Synthèse pour "AnalogInputsCore.c" des différentes Sources & Applications :
#define AI_NB_MAX_OF_INIT_PARAMS		1 // 2 ADC utilisés mais 1 réservé pour touch GFX, donc 1 adc disponible (ADC1)
//#define AI_NB_MAX_OF_INIT_PARAMS		1+1 // 2 pour essais avec 1 vide

#define AI_DISCARD_MIN_MAX_VALUE	// Pour demander à ignorer les valeurs Min & Max des échantillons ADC_DMA
//#define AI_REQUEST_CALIB_AT_MST 	// Pour demander une Calibration de l'ADC à l'Init
//#define AI_SUPPORT_ADJUST_TO_REF	// Pour demander un ajustement des échantillons à une Référence fixe connue

#define AI_ENABLE_SUPPORT_STATS		// Pour demander à collecter des Statistiques spécifiques par ADC

#define AI_MAX_PT_CONV				4095	// Valeur Maximale de la Conversion ADC : 12 bits -> 4095
#define AI_INTERNAL_VREF			1.21f	// Tension Interne de Référence "Vrefint"

#define AI_VALIM_TYPIC				3.3f // 3.3v Typique
#define AI_VALIM_MIN				1.8f // 1.8v Minimum
#define AI_VALIM_MAX				3.6f // 3.6v Maximum

#define AI_OFFSET_PT_CTN 			66.0f

#define AI_FIRST_INIT_PARAM	AI_BaseInitParam // Nom de la Base du Tableau d'Init des Paramètres
#define AI_AFTER_INIT_PARAM	AI_EndInitParams // Nom de Fin du Tableau d'Init des Paramètres

// Tableau des Liens à charger automatiquement à la Mise sous Tension (pendant InitAnalogInputs) :
AI_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(tAdcInitParams, AI_FIRST_INIT_PARAM, AI_AFTER_INIT_PARAM);

#ifdef __cplusplus
}
#endif

#endif /* ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_ */
