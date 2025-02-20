/*
 * FanPwmIcUtils.h
 *
 *  Created on: 16 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 07 Mars 2022
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "FanPwmIC" dans un nouveau Projet :
 *   -> Suivre les indications dans "FanPwmIcConf.h"
 *
 */

#ifndef FANPWMIC_FANPWMICUTILS_H_
#define FANPWMIC_FANPWMICUTILS_H_

#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "main.h"				// Pour accès aux bons includes des HAL
//#include "utils.h"				// Pour accès aux éléments utiles génériques
#include "FanPwmIcConf.h"		// Pour accès à la Configuration User souhaitée

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
// Init Params :

typedef struct _tFanPwmInitParams {
	TIM_HandleTypeDef *htim;	// Handle to the Timer for PWM generation
	uint32_t PwmChannelId;		// ChannelId for PWM within the above Timer
	uint16_t MaxValue;			// True Period of the above Timer (= "Counter Period" +1)
	struct _tFanPwmData* pDatas;
} tFanPwmInitParams;

typedef struct _tFanIcInitParams {
	TIM_HandleTypeDef* htim;	// Handle to the Timer for InputCapture detection
	uint32_t IcChannelId;		// ChannelId for InputCapture within the above Timer
	HAL_TIM_ActiveChannel IcChannelFlag; // Flag for the InputCapture channel identification
	uint32_t TimeOut_10ms;		// TimeOut absence de signal (base @ 10ms)
	float	kFeedBack;			// Coefficient de Conversion vers l'unité de sortie attendue
	struct _tFanIcSrcDatas* pSrcDatas;
	struct _tFanIcData* pIcDatas;
} tFanIcInitParams;

typedef struct _tFanPwmIcInitParams {
	tFanPwmInitParams PwmInitParams;
	tFanIcInitParams  IcInitParams;
} tFanPwmIcInitParams;

/******************************************************************************/
// PWM Datas :

typedef struct _tFanPwmData {
	tFanPwmInitParams* pPwmInitParams;
	uint16_t PwmValue;
} tFanPwmData;

/******************************************************************************/
// MoyManager Datas :

typedef struct
{
	uint16_t maxCt;		// Pour limiter à N valeurs Converties
	uint16_t curIdx;	// Index de stockage de la prochaine valeur Convertie
	uint32_t sumTab;	// Somme instantanée 32bits non-signée du Tableau des derniers échantillons Convertis
	uint16_t nbInTab;	// Nombre d'échantillons Convertis actuellement dans le Tableau
} FPIC_MoyManager_UInt16; // Doit être immédiatement suivi par un tableau de UInt16 !
#define FPIC_ADD_MOY_MANAGER_UINT16(n,s)	FPIC_MoyManager_UInt16 n; uint16_t n##Data[s]

typedef struct
{
	uint16_t maxCt;		// Pour limiter à N valeurs Converties
	uint16_t curIdx;	// Index de stockage de la prochaine valeur Convertie
	uint16_t nbInTab;	// Nombre d'échantillons Convertis actuellement dans le Tableau
	float	 sumTab;	// Somme instantanée du Tableau des derniers échantillons Convertis
} FPIC_MoyManager_Float; // Doit être immédiatement suivi par un tableau de Float !
#define FPIC_ADD_MOY_MANAGER_FLOAT(n,s)	FPIC_MoyManager_Float n; float n##Data[s]

/******************************************************************************/
// InputCaptures Datas :

typedef struct _tFanIcSrcDatas {
	// Référence à l'Init :
	tFanIcInitParams* pIcInitParams;

	// Variables de Capture :
	uint32_t CapturedValue[2];	// Valeur brute lors des 2 dernières Captures
	uint32_t newCapturedCount;	// nb Capturés
	uint32_t sabTimeOut;		// Sablier d'attente entre les impulsions

	// Variables de Traitement :
	uint32_t curCapturedCount;	// nb Traités
#ifdef FPIC_FEED_BACK_IS_FLOAT
  #if defined(FPIC_MOY_FEED_BACK_CONV) && (FPIC_MOY_FEED_BACK_CONV > 0)
	FPIC_ADD_MOY_MANAGER_FLOAT(FeedBackConv, FPIC_MOY_FEED_BACK_CONV);
  #endif // FPIC_MOY_FEED_BACK_CONV
#else // !FPIC_FEED_BACK_IS_FLOAT
  #if defined(FPIC_MOY_FEED_BACK_CONV) && (FPIC_MOY_FEED_BACK_CONV > 0)
	FPIC_ADD_MOY_MANAGER_UINT16(FeedBackConv, FPIC_MOY_FEED_BACK_CONV);
  #endif // FPIC_MOY_FEED_BACK_CONV
#endif // FPIC_FEED_BACK_IS_FLOAT

	// Variables de Sortie :
#ifdef FPIC_FEED_BACK_IS_FLOAT
	float FeedBackValue;		// Valeur précise du Feedback, dans l'unité résultant de IC_K_UNIT
#else // !FPIC_FEED_BACK_IS_FLOAT
	uint16_t FeedBackValue;		// Valeur entière du Feedback, dans l'unité résultant de IC_K_UNIT
#endif // FPIC_FEED_BACK_IS_FLOAT

#ifdef FPIC_GET_LAST_DELTA_TIME
	uint32_t lastDeltaTime;		// Pour mettre à dispo la dernière durée entre 2 impulsions
#endif // FPIC_GET_LAST_DELTA_TIME

#ifdef FPIC_GET_LAST_FEED_BACK
	float lastFeedBackValue;	// Pour mettre à dispo la dernière valeur Feedback calculée
#endif // FPIC_GET_LAST_FEED_BACK

} tFanIcSrcDatas;

typedef struct _tFanIcData {
	tFanIcInitParams* pIcInitParams;	// Ref to Initial Params
	tFanIcSrcDatas* pDatas;				// Pointeur to SourceDatas
} tFanIcData;

/******************************************************************************/
// General Public Datas :

typedef struct _tFanPwmIc {
	tFanPwmData PwmDatas;
	tFanIcData	IcDatas;
} tFanPwmIc;

typedef enum {
	FanPwmIc_Swap_Restore	= 0,	// Restaurer la Source / Destination originale
	FanPwmIc_Swap_Apply		= 1,	// Inverser les 2 Sources / Destinations originales
	FanPwmIc_Swap_Toggle	= 2,	// Inverser les 2 Sources / Destinations entre elles
} eFanPwmIcSwapAcion;

/******************************************************************************/
// Public Prototypes :

uint16_t FanPwmIC_Register_InitParam(tFanPwmIcInitParams* pNewInitParam, void* pThisManager);

// Fonctions pour la Gestion de la PWM :
void FanPwmIC_UpdatePwmValue(tFanPwmData* pPwmData, uint16_t newValue);
uint16_t FanPwmIC_getFanVoltage_mV(tFanPwmData* pPwmData);


// Fonctions d'accès aux Infos Feedback :
uint16_t FanPwmIC_getFanSpeed(tFanIcData* pIcData);
uint16_t FanPwmIC_getLastFanSpeed(tFanIcData* pIcData);
uint16_t FanPwmIC_getLastDeltaTime(tFanIcData* pIcData);

// Fonctions d'inversion de PWM & InputCaptures :
void FanPwmIC_SwapPwmIc(tFanPwmData* pPwmData1, tFanIcData* pIcData1, tFanPwmData* pPwmData2, tFanIcData* pIcData2, uint8_t swapAction);

void FanPwmIC_SwapPwmDataPtr(tFanPwmData* pPwmData1, tFanPwmData* pPwmData2, uint8_t swapAction);
uint8_t FanPwmIC_ArePwmSwapped(tFanPwmData* pPwmData1, tFanPwmData* pPwmData2);

void FanPwmIC_SwapIcDataOutPtr(tFanIcData* pPwmData1, tFanIcData* pPwmData2, uint8_t swapAction);
uint8_t FanPwmIC_AreIcSwapped(tFanIcData* pIcData1, tFanIcData* pIcData2);

/******************************************************************************/
// Links to utils.h :

#define FanPwmIc_FillMemory	FillMemory
#define FanPwmIc_CopyMemory	CopyMemory
void FanPwmIc_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void FanPwmIc_CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt);

/******************************************************************************/
// Public Macros :

#define FPIC_MAKE_TIME_OUT_ms(a)							(1.0f + (((float)(a) -5.0f)/10.0f))		// Valeur de TimeOut en Base @ 10ms
#define FPIC_TIME_OUT_100ms									(FPIC_MAKE_TIME_OUT_ms(100))	// Valeur de TimeOut correspondant à 100ms
#define FPIC_TIME_OUT_1000ms								(FPIC_MAKE_TIME_OUT_ms(1000))	// Valeur de TimeOut correspondant à 1000ms <=> 1s

#define FPIC_MAKE_FAPB_KHz(a)								(((float)(a)) *1000.0f)
#define FPIC_MAKE_FAPB_MHz(a)								((((float)(a)) *1000.0f) *1000.0f)

#define FPIC_MAKE_CHANNEL_FLAG(id)							(1 << (id >> 2))
#define FPIC_MAKE_K_FEED_BACK(clk,psc,ppt,k)				((float)( ((float)(k)) * ( ((float)(clk))/((float)( ((uint32_t)(ppt)) * ((uint32_t)(psc)+1) )) ) ))

#define FPIC_MAKE_VAR_AND_SET_VALUE(t,n,v)					t n = (t)v
#define FPIC_MAKE_CONST_END_OF_TABLE(t,n,b)					const t* n = ((void*)b) + (sizeof(b))
#define FPIC_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = ((void*)a) + (sizeof(a))
#define FPIC_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(t,b,e)	extern const t* b; extern const t* e

#ifdef __cplusplus
}
#endif

#endif /* FANPWMIC_FANPWMICUTILS_H_ */
