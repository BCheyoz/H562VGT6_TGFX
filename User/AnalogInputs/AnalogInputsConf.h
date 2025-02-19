/* Export Original from ConfFormatter by j.proux on 13/03/2024 @ 12:29:41.104 for ALDES © 2024 !
 * AnalogInputsConf.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
 *
 *  History Usage :
 *-> 08/09/2021 : Added by Jp	to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
 *-> 23/12/2021 : Added by AM	to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
 *-> 07/01/2022 : Added by Jp	to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 30/11/2023 : Added by AB	to MV_By_Aldes (STM32G030K8Tx : innoprojects/mv_by_aldes)
 *-> 18/02/2025 : Added by Mf	to Tfl4 (STM32H562VGTX : /be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)
 *
 */

#ifndef ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_
#define ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************
* Procédure pour intégrer facilement cette Librairie "AnalogInputs" dans un nouveau projet : *
**********************************************************************************************

 +-----------------------------+
 | Etape I : Configurer CubeMX |
 +-----------------------------+
===> Dans CubeMX :

I.1) Configurer toutes les entrées Analogiques concernées comme ADC#_IN*
 et leur attribuer un Nom commençant par "Ai_" en fonction de leur usage prévu sur le schéma.

I.2) Configurer chaque Pin analogique concernée de "GPIO" -> "ADC" telle que :
 -> GPIO mode = "Analog mode"
 -> GPIO Pull-up/Pull-down = "No pull-up and no pull-down"

I.3) Pour chacun des Périphériques ADC# concernés, configurer les "Parameter Settings" -> "ADC_Settings" :
-> Clock Prescaler = (par exemple) "PCLK2 divided by 4" OU "PCLK2 divided by 8" OU "Synchronous clock mode divided by 2"
-> Resolution = 12bits (ou mieux si possible ?)
-> Data Alignment = "Right alignment"
-> Sequencer (si proposé) : généralement "Not fully configurable" est suffisant,
 mais "Fully configurable" fonctionne également, il y a juste un peu plus de Configuration manuelle à faire
-> Scan Conversion Mode (si proposé) : généralement "Forward" est suffisant (à défaut prendre "Enabled"),
 mais "Backward" peut être utile si, par exemple, on veut mettre Vbat ou Vrefint avant les autres Channels
-> Continuous Conversion Mode = "Enabled", pour remplir plusieurs lots d'échantillons d'1 coup
-> Discontinuous Conversion Mode = "Disabled"
-> DMA Continuous Requests = "Disabled", pour que le DMA s'arrête une fois le Buffer rempli
-> End Of Conversion Selection = "End of sequence of conversion" OU "EOC flag at the end of all conversions"
-> Overrun behaviour (si proposé) : "Overrun data overwritten" marche habituellement bien
-> Low Power Auto Wait (si proposé) : "Disabled" marche habituellement bien
-> Auto Off (si proposé) : "Disabled" marche habituellement bien

I.4a) Pour chaque Périphérique ADC# concerné, configurer "Parameter Settings" -> "ADC_Regular_ConversionMode" :
-> SamplingTime Common 1 (si demandé) = (par exemple) "160.5 Cycles", voire "480 Cycles"
-> Number Of Conversion (si demandé) : indiquer le nb total d'entrées à Convertir via ce Périphérique ADC
-> External Trigger Conversion Source = "Regular Conversion launched by software"
-> External Trigger Conversion Edge = "None"
-> Trigger Frequency (si proposé) : "High frequency" marche habituellement bien

I.4b) Pour chacun des RANK (si demandé, par exemple si le "Sequencer" est "Fully configurable"), renseigner :
-> Channel : Numéro de Channel correspondant à l'entrée ADC à acquérir, selon l'ordre souhaité
-> Sampling Time : Durée de chaque échantilonnage, par exemple "112 Cycles" OU "480 Cycles" OU "Sampling time commun 1"

Remarque : Dans la mesure du possible, je recommande de n'activer AUCUN "Analog Watchdog Mode"

I.5) Pour chaque Périphérique ADC# concerné, configurer "DMA Settings" tels que :
-> Mode = "Normal" ("Circular" pourrait convenir, mais c'est inutile)
-> Increment Address -> Peripheral = ne PAS cocher
-> Increment Address -> Memory = Cocher
-> Use Fifo (si proposé) = ne PAS cocher
-> Data Width -> Peripheral = "Half Word"
-> Data Width -> Memory = "Half Word"
-> Enable synchronisation (si proposé) : à priori pas besoin de cocher
-> Enable event (si proposé) : à priori pas besoin de cocher

I.6) Pour chaque Périphérique ADC# concerné, configurer "NVIC Settings" tel que :
-> "DMA* ch* interrupt" (ou "DMA* stream* global interrupt") soit bien coché "Enabled"
 + si proposé : laisser "Preemption Priority" à 0,
 + si proposé : laisser "Sub Priority" à 0.
-> "ADC* interrupt" : voir si ça peut fonctionner si ça reste not Enabled ...

I.7) Dans la Catégorie "System Core" -> "NVIC" -> "Code generation", s'assurer que, pour chaque
 "DMA* channel * interrupt" associé à un ADC, soient cochés "Generate IRQ handler" & "Call HAL handler"

I.8) Dans l'onglet général "Clock Configuration", s'assurer que la Clock "to ADCx" (probablement PCLK2) soit à
 une fréquence suffisante, sans pour autant être en rouge. Dans la plupart des cas, 30 MHz peut être suffisant.

I.9) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
-> "Generate peripheral initialization as a pair of '.c//h' files per peripheral" -> coché
-> "Keep User Code when re-generating" -> coché
-> Je recommande de cocher également "Delete previously generated files when not re-generated".

I.10) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
-> s'assurer que tous les Périphériques ADC pour cette Librairie soient de type "HAL".

I.11) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Generated Function Calls" :
-> Pour chaque Périphérique ADC à utiliser, s'assurer que soient bien cochés :
 "Generate Code" (à gauche) + "Do Not Generate Function Call" + "Visibility (Static)"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX !


 +--------------------------------------+
 | Etape II : Configurer "AnalogInputs" |
 +--------------------------------------+
===> Dans ce fichier "AnalogInputsConf.h" (cf. plus bas) :

II.1) Indiquer dans "AI_NB_MAX_OF_INIT_PARAMS" (cf. plus bas) le nombre d'ADC que devra gérer cette Librairie

II.2) Décommenter ou Commenter "AI_DISCARD_MIN_MAX_VALUE" (cf. plus bas) pour ignorer les valeurs Min & Max des échantillons ADC_DMA

II.3) Décommenter ou Commenter "AI_REQUEST_CALIB_AT_MST" (cf. plus bas) pour demander (ou pas) une Calibration de l'ADC à l'Init
 INFO : cette fonctionalité est recommandée, si possible, mais elle n'est pas présente sur tous les µC !

Remarque : il est recommandé de laisser "AI_FIRST_INIT_PARAM" & "AI_AFTER_INIT_PARAM" à leurs Paramètres par défaut,
 respectivement "AI_BaseInitParam" et "AI_EndInitParams".


 +---------------------------------------+
 | Etape III : Configurer la partie User |
 +---------------------------------------+
===> Dans le fichier User.c ("AnalogInputsUser.c") :

III.1) Pour chaque Périphérique ADC qui sera utilisé, définir :
-> ADC#_HANDLE	: Indiquer le hadc* correspondant, d'après "adc.h", par exemple "&hadc1". Ne pas oublier le '&' devant.
-> ADC#_INIT_FN : Indiquer le nom de la fonction d'Init CubeMx, d'après "adc.h", par exemple "MX_ADC1_Init",
-> ADC#_DMA_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée par le DMA pour le stockage des échantillons de l'ADC, par exemple "AdcDmaBuf1"
-> ADC#_RAW_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée pour le stockage de chaque dernière valeur RAW, par exemple "AdcRawBuf1"
-> ADC#_ACCU_RAW_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée pour la gestion Interne des échantillons RAW, par exemple "AdcAccuRawBuf1"
-> ADC#_MOY_FN_HANDLERS : Indiquer le Nom du Tableau, qui contiendra, pour chaque entrée ADC, la fonction à appeler pour finaliser la Conversion
-> ADC#_NB_OF_CHANNELS : Indiquer le nombre de Channels configurés (= nb de "HAL_ADC_ConfigChannel") pour cet ADC dans "adc.c"
-> ADC#_SAMPLES_PER_CH : Indiquer le nombre d'échantillons successifs à prendre en rafale pour obtenir 1 valeur ADC
-> ADC#_MOY_NB_VALUES : Indiquer le nombre de valeurs ADC à intégrer pour le calcul de la Moyenne finale

III.2) Pour chaque Périphérique ADC qui sera utilisé, définir les Variables nécessaires à la Librairie,
 via les macros "AI_MAKE_DMA_RESULTS_BUF" & "AI_MAKE_ADC_ACCU_RAW_BUF" proposées (et éventuellement "AI_MAKE_ADC_RAW_BUFFER")

Exemple 1 :
AI_MAKE_DMA_RESULTS_BUF(ADC1_DMA_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH);
AI_MAKE_ADC_RAW_BUFFER (ADC1_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS);
AI_MAKE_ADC_ACCU_RAW_BUF(ADC1_ACCU_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_MOY_NB_VALUES);

Exemple 2 :
AI_MAKE_DMA_RESULTS_BUF(ADC2_DMA_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_SAMPLES_PER_CH);
AI_MAKE_ADC_RAW_BUFFER (ADC2_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS);
AI_MAKE_ADC_ACCU_RAW_BUF(ADC2_ACCU_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_MOY_NB_VALUES);

III.3) Définir chaque Variable finale nécessaire pour le Stockage du Résultat en RAM

Exemple 1 :
tAI_FloatValue tAi0_0_10V = {0};

Exemple 2 :
tAI_FloatValue tAi_12vAlim = {0};

III.4) Si besoin, Ajouter / Corriger ou Implémenter la(les) Fonctions User de Conversion Finales appropriées,
 en respectant le prototype "pAiFnHandleNewFloatValue" (cf. "AnalogInputsUtils.h" & exemples fournis)

III.5) Pour chaque Périphérique ADC qui sera utilisé, Remplir / Configurer le Tableau des Fonctions de
 Conversion & Variables Finales associées, en respectant la structuration "tAiFnNewFloatValueHandler"
  (1 ligne par Channel et 1 Tableau par Périphérique ADC)

Exemple :
	{ AnalogInput_HandleNewFloat_0_10V,	&tAi0_0_10V },

III.6) Pour chaque Périphérique ADC qui sera utilisé, Ajouter / Configurer la ligne correspondante dans le Tableau
 "mAdcInitParam", en respectant la structuration "tAdcInitParams" (1 ligne d'init par Périphérique ADC)

Exemple :
	{ ADC1_HANDLE, ADC1_INIT_FN, ADC1_DMA_BUF_NAME, &ADC1_ACCU_RAW_BUF_NAME, ADC1_MOY_FN_HANDLERS, AnalogInput_HandleEndOfConv, ADC1_RAW_BUF_NAME, \
		ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH, ADC1_MOY_NB_VALUES, ADC1_CONV_DELAY, ADC1_ERROR_DELAY, ADC1_REF_INT_CHANNEL, ADC1_REF_INT_PT_CONV },

III.7) Ajouter d'éventuelles fonctions "Get" pour récupérer la/les valeur(s) utile(s) à partir des variables crées au III.3)

Exemple 1 : Récupérer sur 8bits la valeur de "tAi0_0_10V"
uint8_t getA0_0_10V(void)
{
	return (uint8_t)(tAi0_0_10V.value);
}

Exemple 2 : Récupérer sur 8bits la valeur de "tAi0_0_10V" * 10 
uint8_t getA1_0_10V_x10(void)
{
	return (uint8_t)(tAi1_0_10V.value * 10.0f);
}

Exemple 3 : Récupérer sur 16bits la valeur de "tAi2_0_10V" * 10
uint16_t getA2_0_10V_x10(void)
{
	return (uint16_t)(tAi2_0_10V.value * 10.0f);
}


III.8) Dans le fichier User.h ("AnalogInputsUser.h"),
 1] Ajouter éventuellement comme "extern" les variables crées au  III.3)

Exemple 1 : Autoriser l'accès externe à la Variable "tAi0_0_10V"
extern tAI_FloatValue tAi0_0_10V;

Exemple 2 : Autoriser l'accès externe à la Variable "tAi1_0_10V"
extern tAI_FloatValue tAi1_0_10V;

 2] Ajouter éventuellement "publiquement" le(s) prototype(s) de fonction(s) de "GET" crée(s) au III.7)

Exemple 1 : Donner accès sur 8bits à la valeur de "A0_0_10V"
uint8_t getA0_0_10V(void);

Exemple 2 : Donner accès sur 8bits à la valeur de "A1_0_10V" * 10
uint8_t getA1_0_10V_x10(void);

Exemple 3 : Donner accès sur 16bits à la valeur de "A2_0_10V" * 10
uint16_t getA2_0_10V_x10(void);


 +---------------------------------+
 | Etape IV : Configurer le Projet |
 +---------------------------------+
===> Dans le nouveau Projet :

IV.1) Ajouter l'Include du .h dans le "main.c" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans "USER CODE * Includes" de "BaseDeTemps.c")
	#include "AnalogInputsCore.h"		// Pour Gestion des Entrées ADC

IV.2) Ajouter l'appel d'Init dans le "main" du "main.c" (par exemple parmi les "USER CODE * 2") :
	InitAnalogInputs();					// A appeler dans la partie Init Software (main.c)

IV.3) Ajouter le Handler @ 10ms dans une sous-partie de la Boucle Principale (RT) à 10ms :
 (par exemple dans la zone @ 10ms "USER CODE * RT_10ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
	Handle_AnalogInputs_RT_10ms();		// A appeler depuis une Base de Temps RT @ 10ms

IV.4) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
	Gestion_AnalogInputs();				// A appeler dans la Boucle Principale (main.c)

IV.5) Ajouter le Dossier "AnalogInputs" à l' "IncludePath" pour toutes les Configs de Build
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/AnalogInputs" (sans les guillemets)
Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

IV.6) Vérifier que le Dossier "AnalogInputs" ne soit "Exclude From Build" d'aucune des Configs.
 (y compris "Debug" & "Release")
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

IV.7) Ajouter au projet l'une des Librairie "Utils" compatibles, comme celle de "RMD_Firmware" ou "HII_Manta_App".
 -> se reporter à "utils.h" pour intégrer plus facilement votre Librairie "Utils".


 +-------------------------------------------+
 | Etape V : Configurer le "build-job" du CI |
 +-------------------------------------------+
===> Dans le MakeFile :

V.1) Ajouter aux "C_SOURCES" le chemin relatif vers les fichiers "AnalogInputsCore.c" & "AnalogInputsUser.c",
 très probablement : "User/AnalogInputs/AnalogInputsCore.c \" (sans les guillemets mais avec '\' en fin de ligne)
 ET : "User/AnalogInputs/AnalogInputsUser.c \" (toujours sans les guillemets mais aussi avec '\' en fin de ligne)

Remarque : pour le chemin, il faut bien utiliser la bare oblique de la division '/' à la place du '\' habituel de Windows.

V.2) Ajouter aux "C_INCLUDES" le chemin relatif vers notre dossier "AnalogInputs" avec le préfixe "-I" devant,
 très probablement : "-IUser/AnalogInputs \" (sans les guillemets mais également avec '\' en fin de ligne)

Même Remarque : pour le chemin, utiliser la bare obliques de la division '/' à la place du '\' habituel de Windows.

=> Félicitations, c'est prêt :-) !

*************************************************************************************************/

#include "AnalogInputsUtils.h"	// Pour accès aux Outils d'aide à la Configuration

// Synthèse pour "AnalogInputsCore.c" des différentes Sources & Applications :
#define AI_NB_MAX_OF_INIT_PARAMS		1 // 2 ADC utilisés mais 1 réservé pour touch GFX, donc 1 adc disponible (ADC1)
//#define AI_NB_MAX_OF_INIT_PARAMS		1+1 // 2 pour essais avec 1 vide

#define AI_DISCARD_MIN_MAX_VALUE	// Pour demander à ignorer les valeurs Min & Max des échantillons ADC_DMA
//#define AI_REQUEST_CALIB_AT_MST 	// Pour demander une Calibration de l'ADC à l'Init
#define AI_SUPPORT_ADJUST_TO_REF	// Pour demander un ajustement des échantillons à une Référence fixe connue

#define AI_ENABLE_SUPPORT_STATS		// Pour demander à collecter des Statistiques spécifiques par ADC

#define AI_MAX_PT_CONV				4095	// Valeur Maximale de la Conversion ADC : 12 bits -> 4095
#define AI_INTERNAL_VREF			1.21f	// Tension Interne de Référence "Vrefint"

#define AI_VALIM_TYPIC				3.3f // 3.3v Typique
#define AI_VALIM_MIN				1.8f // 1.8v Minimum
#define AI_VALIM_MAX				3.6f // 3.6v Maximum

#define AI_FIRST_INIT_PARAM	AI_BaseInitParam // Nom de la Base du Tableau d'Init des Paramètres
#define AI_AFTER_INIT_PARAM	AI_EndInitParams // Nom de Fin du Tableau d'Init des Paramètres

// Tableau des Liens à charger automatiquement à la Mise sous Tension (pendant InitAnalogInputs) :
AI_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(tAdcInitParams, AI_FIRST_INIT_PARAM, AI_AFTER_INIT_PARAM);

#ifdef __cplusplus
}
#endif

#endif /* ANALOG_INPUTS_ANALOG_INPUTS_CONF_H_ */
