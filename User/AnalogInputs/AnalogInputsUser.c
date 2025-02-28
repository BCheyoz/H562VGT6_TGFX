/*
 * AnalogInputsUser.c
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 27 Feb. 2025
 *  Updated by: b.chhay
 *
 *  Version : 1.0
 *
 */

#include "AnalogInputsUser.h"	// Pour accès à nos propres déclarations publiques
#include "adc.h"				// Pour accès aux Variables & Fonctions d'Init ADC
#include "AnalogInputsConf.h"	// Pour accès à la Configuration User souhaitée

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
// Pour la Conversion au travers d'un pont diviseur (Tension aux bornes de R1, sur la base du ratio de R1/(R1+R2) :

#define AI_MAKE_K_ADC_R1_R2(Vref,R1,R2,Nmax)	((float)((((float)R1)+((float)R2))/((float)R1)*(((float)Vref)/((float)Nmax))))
#define AI_K_ADC_3_3V_10K_100K_12bits	AI_MAKE_K_ADC_R1_R2(3.3f, 10.f*1000.f, 100.f*1000.f, 4095)	// Vref = 3.3V, R1 = 10K, R2 = 100K, Nmax(12bits) = 2^12 -1 = 4095
#define AI_K_ADC_3_3V_10K_22K_12bits	AI_MAKE_K_ADC_R1_R2(3.3f, 10.f*1000.f,  22.f*1000.f, 4095)	// Vref = 3.3V, R1 = 10K, R2 = 22K,  Nmax(12bits) = 2^12 -1 = 4095

/******************************************************************************/
// Définitions pour la Gestion de l'ADC n°1 :

#define ADC1_HANDLE				&hadc1
#define ADC1_INIT_FN			MX_ADC1_Init
#define ADC1_DMA_BUF_NAME		AdcDmaBuf1
#define ADC1_RAW_BUF_NAME		AdcRawBuf1
#define ADC1_ACCU_RAW_BUF_NAME	AdcAccuRawBuf1
#define ADC1_MOY_FN_HANDLERS	Adc1FnNewFloatValueHandlers
#define ADC1_NB_OF_CHANNELS 	6			// tfl4_cartemere_app = VrefInt, ADC1_IN1 (Ai_T1), ADC1_IN0 (Ai_T2), ADC1_IN18 (Ai_T3), ADC1_IN15 (Ai_T4), ADC1_IN14 (Ai_T5)
#define ADC1_SAMPLES_PER_CH 	1			// 5 échantillons de chaque pour faire une première moyenne instantanée
#define ADC1_MOY_NB_VALUES		1			// La valeur de sortie sera moyennée sur les 20 dernières valeurs instantanées disponibles
#define ADC1_CONV_DELAY 		10			// Temps accordé pour la Conv : Base @ 10ms => 10 = 100ms
#define ADC1_ERROR_DELAY		10			// Tempo après une Erreur ADC : Base @ 10ms => 10 = 100ms
#define ADC1_REF_INT_CHANNEL	0			// Index du Rank qui est associé au "Channel Vrefint" ?
#define ADC1_REF_INT_PT_CONV	(AI_INTERNAL_VREF / AI_VALIM_TYPIC * (float)AI_MAX_PT_CONV)

/******************************************************************************/
// Définitions pour la Gestion de l'ADC n°2 :

//#define ADC2_HANDLE 			&hadc2
//#define ADC2_INIT_FN			MX_ADC2_Init
//#define ADC2_DMA_BUF_NAME		AdcDmaBuf2
//#define ADC2_RAW_BUF_NAME		AdcRawBuf2
//#define ADC2_ACCU_RAW_BUF_NAME	AdcAccuRawBuf2
//#define ADC2_MOY_FN_HANDLERS	Adc2FnNewFloatValueHandlers
//#define ADC2_NB_OF_CHANNELS 	8			// Configuration test Tone :  8 Chanels sur ADC2
//#define ADC2_SAMPLES_PER_CH 	40			// Configuration test Tone : 40 échantillons de chaque Channel
//#define ADC2_CONV_DELAY 		10			// Temps accordé pour la Conv : Base @ 10ms => 10 = 100ms
//#define ADC2_ERROR_DELAY		100			// Tempo après une Erreur ADC : Base @ 10ms => 10 = 100ms

/******************************************************************************/
// Variables Internes pour la Gestion de l'ADC n°1 :

AI_MAKE_DMA_RESULTS_BUF(ADC1_DMA_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH);
AI_MAKE_ADC_RAW_BUFFER (ADC1_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS);
AI_MAKE_ADC_ACCU_RAW_BUF(ADC1_ACCU_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_MOY_NB_VALUES);

/******************************************************************************/
// Variables Internes pour la Gestion de l'ADC n°2 :

//AI_MAKE_DMA_RESULTS_BUF(ADC2_DMA_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_SAMPLES_PER_CH);
//AI_MAKE_ADC_RAW_BUFFER (ADC2_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS);
//AI_MAKE_ADC_ACCU_RAW_BUF(ADC2_ACCU_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_MOY_NB_VALUES);

/******************************************************************************/
// Variables finales pour le Stockage des Résultats ADC :

tAI_FloatValue tAiRefAlim = {0}; // Pt Convertisseurs vRefInt & Tension d'Alim correspondante
tAI_IntValue tAi_CTN[NB_CTN_USE] = {0};
uint32_t nbConvDone = 0;

/******************************************************************************/
// Prototypes des Fonctions de Conversion pour obtenir le Résulat Final :

void AnalogInput_HandleNewFloat_RefInt(void* pVar, float newValue);
void AnalogInput_HandleNewFloat_Tx(void* pVar, float newValue);
void AnalogInput_HandleNewFloat_CTN(void* pVar, float newValue);
void AnalogInput_HandleEndOfConv(void* pVar);

/******************************************************************************/
// Tableau des Fonctions de Conversion à appeler & Variables Finales, en fonction du Channel considéré :

tAiFnNewFloatValueHandler ADC1_MOY_FN_HANDLERS[ADC1_NB_OF_CHANNELS] = {
	{ AnalogInput_HandleNewFloat_RefInt,	&tAiRefAlim },	// Valeur n°1 = vRefInt
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[0] },	// Valeur n°2 = ADC1_IN1 = tAi_CTN1
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[1] },	// Valeur n°3 = ADC1_IN0 = tAi_CTN2
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[2] },	// Valeur n°4 = ADC1_IN18 = tAi_CTN3
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[3] },	// Valeur n°5 = ADC1_IN15 = tAi_CTN4
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[4] },	// Valeur n°6 = ADC1_IN14 = tAi_CTN5
};

/******************************************************************************/
// Tableau des Paramètres d'Initialisation de la Librairie "AnalogInputs" (1 ligne par ADC) :

tAdcInitParams mAdcInitParam[] = {
	{ ADC1_HANDLE, ADC1_INIT_FN, ADC1_DMA_BUF_NAME, &ADC1_ACCU_RAW_BUF_NAME, ADC1_MOY_FN_HANDLERS, AnalogInput_HandleEndOfConv, ADC1_RAW_BUF_NAME,
		ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH, ADC1_MOY_NB_VALUES, ADC1_CONV_DELAY, ADC1_ERROR_DELAY, ADC1_REF_INT_CHANNEL, ADC1_REF_INT_PT_CONV },
//	{ ADC2_HANDLE, ADC2_INIT_FN, ADC2_DMA_BUF_NAME, &ADC2_ACCU_RAW_BUF_NAME, ADC2_MOY_FN_HANDLERS, AnalogInput_HandleEndOfConv, ADC2_RAW_BUF_NAME,
//		ADC2_NB_OF_CHANNELS, ADC2_SAMPLES_PER_CH, ADC2_MOY_NB_VALUES, ADC2_CONV_DELAY, ADC2_ERROR_DELAY, ADC2_REF_INT_CHANNEL, ADC2_REF_INT_PT_CONV },
};
AI_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tAdcInitParams, AI_FIRST_INIT_PARAM, AI_AFTER_INIT_PARAM, mAdcInitParam);

/******************************************************************************/
// Fonctions User de Finalisation de la Conversion :

void AnalogInput_HandleNewFloat_RefInt(void* pVar, float newValue)
{	// Recalcule une projection de l'Alim en fonction de la Référence obtenue :
	tAI_FloatValue* pData = pVar;
	pData->nbPtADC = (uint16_t) newValue; // Mémorise les Points Convertisseur ADC
	if(0 != newValue) pData->value = (AI_INTERNAL_VREF * (float)AI_MAX_PT_CONV / newValue);
	else pData->value = 0; // Invraissemblable, mais il faut quand même prendre en compte
}

void AnalogInput_HandleNewFloat_Tx(void* pVar, float newValue)
{
	tAI_FloatValue* pData = pVar;
	pData->nbPtADC = (uint16_t) newValue; // Mémorise les Points Convertisseur ADC
	pData->value = newValue * AI_K_ADC_3_3V_10K_22K_12bits;	// Effectue la Conversion PointsAdc -> Volts
}

void AnalogInput_HandleNewFloat_CTN(void* pVar, float newValue)
{
	tAI_IntValue* pData = pVar;
	pData->nbPtADC = (uint16_t)(newValue);
	pData->value = convertADC_to_CTN_10K(pData->nbPtADC);
}

void AnalogInput_HandleEndOfConv(void* pVar) // pVar contient le Pointeur vers les Paramètres d'Initialisation, dans mAdcInitParam, dont la Librairie vient de clôturer les Conversions
{	// Remarque_Jp le 24/12/2021 : Comme on n'a besoin de notifier personne que de nouvelles valeurs ADC sont disponibles ...
	nbConvDone++; // On se contente de compter de nb de Conversions effectuées ;-) !
}

#ifdef __cplusplus
}
#endif
