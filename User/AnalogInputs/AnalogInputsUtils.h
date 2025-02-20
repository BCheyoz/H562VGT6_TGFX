/*
 * AnalogInputsUtils.h
 *
 *  Created on: 8 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 18 Feb. 2025
 *  Updated by: m.faget
 *
 */

#ifndef ANALOGINPUTS_ANALOGINPUTSUTILS_H_
#define ANALOGINPUTS_ANALOGINPUTSUTILS_H_

#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
//#include "AnalogInputsUser.h"	// Pour accès aux réglages USER bas niveau USER
#include "main.h"				// Pour accès aux bons includes des HAL
#include "utils.h"				// Pour accès aux éléments utiles génériques

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
// Définitions, Enumérations & Macros founies par le Core pour aider à la Configuration :

typedef void (*pFn_void_From_void_MX_ADC_Init)(void); // Prototype standard des Fonctions d'Init ADC de CubeMx
typedef void (*pAiFnHandleNewFloatValue)(void* pVar, float newValue); // Prototype des Fonctions de CallBack de Valeur Moy disponible
typedef void (*pAiFnHandleEndOfConv)(void* pVar);	// Prototype des Fonctions de CallBack signalant la MàJ de tout un Bloc de Valeurs ADC

/******************************************************************************/

typedef struct _tAiFnNewFloatValueHandler
{
	pAiFnHandleNewFloatValue pFn;
	void*	pVar;
} tAiFnNewFloatValueHandler;

typedef struct _tAiStats
{
	uint32_t nbBsyBfSrt;
	uint32_t nbStartsOK;
	uint32_t nbStartFail;
	uint32_t nbConvTimOut;
	uint32_t nbConvDone;
	uint32_t nbAdcError;
//	uint32_t nbCpltByStep[6];
// Pour le calcul de la durée de Conversion :
	uint32_t convStartTicks;
	uint32_t convEndTicks;
	uint32_t convTimeTicks;
} tAiStats;

typedef struct _tAdcInitParams
{
	ADC_HandleTypeDef* hHandle;
	pFn_void_From_void_MX_ADC_Init pFnInit; // Pointer sur la Fonction d'Init CubeMx ou respectant le même Prototype
	void* pDmaBuffer;
	void* pAccuRawBuf;
	tAiFnNewFloatValueHandler* pFnHandler;
	pAiFnHandleEndOfConv pFnEndOfConv;
	uint16_t* pRawBuffer;
	uint16_t nbOfChannels;
	uint16_t SamplesPerCh;
	uint16_t nbValues4Moy;
	uint16_t convTimeOut;
	uint16_t errorTempo;
	uint16_t refRankID;	// Index de l'échantillon (dans la séquence) servant de Référence connue
	uint16_t refPtConv;	// Valeur constante attendue de l'échantillon de Référence
} tAdcInitParams;

uint16_t AnalogInputs_Register_InitParam(tAdcInitParams* pNewInitParam, void* pNullParam);

/******************************************************************************/

#define AI_MAKE_DMA_RESULTS_BUF(n,c,e)	uint16_t n[c*e]__attribute__((aligned(4)))={0} // Name, nbOfChannels, nbEchantillonsPerChannel
#define AI_MAKE_ADC_RAW_BUFFER(n,c)		uint16_t n[c]={0} // Name, nbOfChannels
#define AI_MAKE_ADC_ACCU_RAW_BUF(n,c,v) typedef struct _tAi##n { uint32_t accu[c]; uint16_t nxtId; uint16_t raw[c*v]; } tAi##n; tAi##n n={0}

/******************************************************************************/

// Links to utils.h :
#define AnalogInputs_FillMemory	FillMemory
#define AnalogInputs_CopyMemory	CopyMemory
void AnalogInputs_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void AnalogInputs_CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt);

/******************************************************************************/

#define AI_MAKE_CONST_END_OF_TABLE(t,n,b)					const t* n = ((void*)b) + (sizeof(b))
#define AI_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = ((void*)a) + (sizeof(a))
#define AI_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(t,b,e)	extern const t* b; extern const t* e

#ifdef __cplusplus
}
#endif

#endif /* ANALOGINPUTS_ANALOGINPUTSUTILS_H_ */
