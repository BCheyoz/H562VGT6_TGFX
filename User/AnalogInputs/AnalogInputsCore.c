/*
 * AnalogInputsCore.c
 *
 *  Created on: 7 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 09 Apr. 2025
 *  Updated by: j.proux
 *
 *  Version 1.0
 *
 */

#include "AnalogInputsCore.h"	// Pour accès à nos propres déclarations publiques
#include "AnalogInputsConf.h"	// Pour accès à la Configuration User souhaitée

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#if defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_ANALOG_INPUTS)
	#define AI_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
//	#warning "DEBUG_ANALOG_INPUTS is Active !!!"
#else // (! __DEBUG) && (! DEBUG_ANALOG_INPUTS) :
	#define AI_HALT_IF_DEBUG()	// Nop();
#endif // __DEBUG ; DEBUG_ANALOG_INPUTS

/******************************************************************************/

#define AI_SYNC_ADC_100ms	10	// 10 x10ms dans 100ms

typedef enum {
	ADC_STEP_NOT_INIT = 0,
	ADC_STEP_WAIT_SYNC,
	ADC_STEP_START_ADC,
	ADC_STEP_WAIT_ADC,
	ADC_STEP_COMPLETE,
	ADC_STEP_ERROR_ADC,
	//-----------------
	ADC_STEP_NB_STEPS	// Pour mémoire du nombre d'étapes possibles
} AI_ADC_STEP;

typedef struct _tAI_AdcManager
{
	tAdcInitParams* pInitParams; // Pointeur vers les Paramètres de Configuration User
	AI_ADC_STEP curStep;
	uint16_t sabSynchro;
	uint16_t sabTimeOut;

	union {
		uint8_t Flags;
		struct {
			unsigned isLoaded:1;	// Pour savoir si le Périphérique de Conversion nous est "connecté", ou pas.
			unsigned newValue:1;	// Pour signaler qu'il y a de nouvelles Valeurs disponibles
		};
	};
} tAI_AdcManager;

/******************************************************************************/

tAI_AdcManager mAI_AdcManager[AI_NB_MAX_OF_INIT_PARAMS] = {0};
AI_MAKE_CONST_END_OF_TABLE(tAI_AdcManager, EndOfAI_AdcManager, mAI_AdcManager);

// Quelques infos de Statistique, si nécessaire :
#if defined(AI_ENABLE_SUPPORT_STATS) && (AI_NB_MAX_OF_INIT_PARAMS > 0)
	tAiStats mAiStats[AI_NB_MAX_OF_INIT_PARAMS] = {0};	// 1 Struture de Stats par ADC
#endif // AI_ENABLE_SUPPORT_STATS && AI_NB_MAX_OF_INIT_PARAMS

/******************************************************************************/

static tAI_AdcManager* AnalogInputs_GetManagerFromHandle(void* hHandle);

#ifdef AI_SUPPORT_ADJUST_TO_CAL
	static void AI_MPU_Config_EnableAccessToVrefIntCal(void);
#endif // AI_SUPPORT_ADJUST_TO_CAL

/******************************************************************************/

inline void InitAnalogInputs(void)
{

#ifdef AI_SUPPORT_ADJUST_TO_CAL
	AI_MPU_Config_EnableAccessToVrefIntCal();
#endif // AI_SUPPORT_ADJUST_TO_CAL

	tAI_AdcManager* pManager = mAI_AdcManager;
	tAdcInitParams* pInitParams = (tAdcInitParams*)AI_FIRST_INIT_PARAM;
	for( ; (pInitParams < (tAdcInitParams*)AI_AFTER_INIT_PARAM)
		&& (pManager < (tAI_AdcManager*)EndOfAI_AdcManager); pInitParams++)
	{
		if(0 != AnalogInputs_Register_InitParam(pInitParams, pManager))
		{
			pManager++; // Pointe le prochain emplacement prêt à recevoir les infos
		}
	}

	// Vérifier s'il reste d'éventuels InitParams non chargés :
	if(pInitParams < (tAdcInitParams*)AI_AFTER_INIT_PARAM)
	{
			AI_HALT_IF_DEBUG(); // Bloquer au RunTime en DEBUG si mauvaise Configuration => Recalculer AI_NB_MAX_OF_INIT_PARAMS (cf. "AnalogInputsConf.h") !
	}
}

/******************************************************************************/

uint16_t AnalogInputs_Register_InitParam(tAdcInitParams* pNewInitParam, void* pThisManager) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	if(0 == pNewInitParam) return 0;
	if(0 == pNewInitParam->hHandle) return 0;
	if(0 == pNewInitParam->pDmaBuffer) return 0;
	if(0 == pNewInitParam->pAccuRawBuf) return 0;
	if(0 == pNewInitParam->nbOfChannels) return 0;
	if(0 == pNewInitParam->SamplesPerCh) return 0;
	if(0 == pNewInitParam->nbValues4Moy) return 0;

	tAI_AdcManager* pManager = (tAI_AdcManager*)pThisManager;
	if(0 == pManager) // Si inconnu : rechercher un emplacement libre :
	{
		for(pManager = mAI_AdcManager; pManager < (tAI_AdcManager*)EndOfAI_AdcManager; pManager++)
		{
			if(0 == pManager->pInitParams) break; // On a trouvé un emplacement dispo
		}
		if(pManager >= (tAI_AdcManager*)EndOfAI_AdcManager) { pManager = 0; }
	}
	if(0 == pManager) return 0; // Failure : insufficient AI_NB_MAX_OF_INIT_PARAMS !
	if(0 != pManager->pInitParams) return 0; // Failure : This one is not free !

	// Tout est OK pour Préparer la liaison :
	AnalogInputs_FillMemory((void*)pManager, 0, sizeof(tAI_AdcManager)); // Nettoyer notre propre Structure avant de la Configurer
	AnalogInputs_FillMemory((void*)pNewInitParam->pDmaBuffer, 0, pNewInitParam->nbOfChannels * pNewInitParam->SamplesPerCh * sizeof(uint16_t));
	if(0 != pNewInitParam->pRawBuffer) AnalogInputs_FillMemory((void*)pNewInitParam->pRawBuffer, 0, pNewInitParam->nbOfChannels * sizeof(uint16_t));
	AnalogInputs_FillMemory((void*)pNewInitParam->pAccuRawBuf, 0, pNewInitParam->nbOfChannels * (sizeof(uint32_t) + pNewInitParam->nbValues4Moy * sizeof(uint16_t)) + sizeof(uint16_t));

	// Tout est OK pour Activer la liaison :
	if(0 != pNewInitParam->pFnInit) { pNewInitParam->pFnInit(); }	// Appele la Fonction d'Init si elle est définie

#ifdef AI_REQUEST_CALIB_AT_MST
  #if defined(ADC_SINGLE_ENDED) && defined(ADC_DIFFERENTIAL_ENDED)	// S'il y a le choix entre les 2 Modes :
    HAL_ADCEx_Calibration_Start(pNewInitParam->hHandle, ADC_SINGLE_ENDED);	// Calibrate the ADC on Power-Up for Better Accuracy
  #else // Mode de Calibration standard defaults as ADC_SINGLE_ENDED :
    HAL_ADCEx_Calibration_Start(pNewInitParam->hHandle);					// Calibrate the ADC on Power-Up for Better Accuracy
  #endif // ADC_SINGLE_ENDED
#endif // AI_REQUEST_CALIB_AT_MST

	pManager->pInitParams = pNewInitParam;	// Sauvegarde le lien vers les Infos d'Init pour s'y référer ultérieurement
	pManager->curStep = ADC_STEP_WAIT_SYNC;
	pManager->isLoaded = 1;

#if defined(AI_SUPPORT_ADJUST_TO_CAL) && defined(AI_INTERNAL_VREF_CAL) && defined(AI_REF_INT_PT_CONV_CAL)
//float InternalVrefCalValue = AI_INTERNAL_VREF_CAL; // vu @ 1.21120882 sur "PCBA_TF4_4" pour 1.21v Typique d'après la doc
	pNewInitParam->refPtConv = ROUND_TO_UINT_CAST(uint16_t, AI_REF_INT_PT_CONV_CAL); // En cas de HardFault ici sur STM32H5* : Vérifier que "AI_MPU_Config_EnableAccessToVrefIntCal();" est bien appelé en début de "InitAnalogInputs()" !
#endif // AI_SUPPORT_ADJUST_TO_CAL && AI_INTERNAL_VREF_CAL && AI_REF_INT_PT_CONV_CAL

	return 1;
}

/******************************************************************************/

inline void Handle_AnalogInputs_RT_10ms(void)	// A appeler depuis une Base de Temps RT @ 10ms
{
	for(tAI_AdcManager* pManager = mAI_AdcManager; pManager < (tAI_AdcManager*)EndOfAI_AdcManager; pManager++)
	{
		if(pManager->sabSynchro > 0) pManager->sabSynchro--;
		if(pManager->sabTimeOut > 0) pManager->sabTimeOut--;
	}
}

/******************************************************************************/

void Gestion_AnalogInputs(void)
{
	tAdcInitParams* pInitParam = 0;

#ifdef AI_ENABLE_SUPPORT_STATS
	tAiStats* pAiStats = mAiStats;
#endif // AI_ENABLE_SUPPORT_STATS

	for(tAI_AdcManager* pManager = mAI_AdcManager; pManager < (tAI_AdcManager*)EndOfAI_AdcManager; pManager++)
	{
		pInitParam = pManager->pInitParams;
		if(0 == pInitParam) continue;

		// Gestion de la machine à états :
		switch (pManager->curStep)
		{
			case ADC_STEP_WAIT_SYNC:
				if(0 != pManager->sabSynchro) break;
				// noBreak;
			case ADC_STEP_START_ADC:
				pManager->sabSynchro = AI_SYNC_ADC_100ms;	// Relance tout de suite une prochaine Synchro dans 100ms

//				if(0 != (HAL_ADC_STATE_REG_BUSY & pInitParam->hHandle->State))
				if(0 == (HAL_ADC_STATE_READY & pInitParam->hHandle->State))
				{
					HAL_ADC_Stop_DMA(pInitParam->hHandle);	// Nécessaire en mode Continuous !
#ifdef AI_ENABLE_SUPPORT_STATS
					pAiStats->nbBsyBfSrt++;
#endif // AI_ENABLE_SUPPORT_STATS

				}

#ifdef AI_ENABLE_SUPPORT_STATS
					pAiStats->convStartTicks = HAL_GetTick();
#endif // AI_ENABLE_SUPPORT_STATS

				if(HAL_OK == HAL_ADC_Start_DMA(pInitParam->hHandle, (uint32_t*)pInitParam->pDmaBuffer,
					pInitParam->nbOfChannels * pInitParam->SamplesPerCh))
				{
					pManager->curStep = ADC_STEP_WAIT_ADC;
					pManager->sabTimeOut = pInitParam->convTimeOut;

#ifdef AI_ENABLE_SUPPORT_STATS
					pAiStats->nbStartsOK++;
#endif // AI_ENABLE_SUPPORT_STATS

				} else { // Impossible de lancer la Conversion :
					pManager->curStep = ADC_STEP_ERROR_ADC;

#ifdef AI_ENABLE_SUPPORT_STATS
					pAiStats->nbStartFail++;
#endif // AI_ENABLE_SUPPORT_STATS

				}
				break;
			case ADC_STEP_WAIT_ADC:
				if(0 == pManager->sabTimeOut)
				{
					HAL_ADC_Stop_DMA(pInitParam->hHandle);
					pManager->curStep = ADC_STEP_ERROR_ADC;	// Traiter comme une Erreur ADC ...

#ifdef AI_ENABLE_SUPPORT_STATS
					pAiStats->nbConvTimOut++;
#endif // AI_ENABLE_SUPPORT_STATS

				}
				break;
			case ADC_STEP_COMPLETE:
				// Conversion Terminée => C'est le moment de Traiter :

#ifdef AI_ENABLE_SUPPORT_STATS
				pAiStats->nbConvDone++;
				pAiStats->convTimeTicks = pAiStats->convEndTicks - pAiStats->convStartTicks;
#endif // AI_ENABLE_SUPPORT_STATS

			{
				uint32_t* pAccu = pInitParam->pAccuRawBuf;
				uint16_t* pId = (void*)&pAccu[pInitParam->nbOfChannels];
				uint16_t* pRawBase = &pId[1]; // Directement après le NextId (commun pour tous les éléments du Bloc)
				uint16_t* pRawValue;
				uint16_t* pSeqBase;
				uint16_t ptConvCh;

#ifdef AI_SUPPORT_ADJUST_TO_REF
				uint16_t ptConvRef;
#endif // AI_SUPPORT_ADJUST_TO_REF

				if((*pId & INT16_MAX) >= pInitParam->nbValues4Moy) *pId = INT16_MIN; // Si ça dépasse : re-boucler du début, avec b15 = Flag 0x8000 !
				for(int i = 0; i < pInitParam->nbOfChannels; i++)
				{
					float sum = 0; int j = 0;
#ifdef AI_DISCARD_MIN_MAX_VALUE
					uint16_t min = UINT16_MAX; uint16_t max = 0;
#endif // AI_DISCARD_MIN_MAX_VALUE
					for(; j < pInitParam->SamplesPerCh; j++)
					{
//						ptConvCh = ((uint16_t*)pInitParam->pDmaBuffer)[i + j*pInitParam->nbOfChannels];
//						ofstBaseSeq = j*pInitParam->nbOfChannels;	// Offset de la (j)ème séquence de conversion
						pSeqBase = &((uint16_t*)pInitParam->pDmaBuffer)[j*pInitParam->nbOfChannels];	// Pointeur sur Base de la (j)ème séquence de conversion
						ptConvCh = pSeqBase[i];//((uint16_t*)pInitParam->pDmaBuffer)[ofstBaseSeq + i];

#ifdef AI_SUPPORT_ADJUST_TO_REF
						if( (i != pInitParam->refRankID) && (pInitParam->refRankID < pInitParam->nbOfChannels) && (pInitParam->refPtConv > 0) )
						{
							ptConvRef = pSeqBase[pInitParam->refRankID];
							float kAdjust = (float)ptConvRef/(float)pInitParam->refPtConv;
							if(IS_IN_RANGE(kAdjust, AI_VALIM_TYPIC/AI_VALIM_MAX, AI_VALIM_TYPIC/AI_VALIM_MIN)) // cf. "ADC supply requirements" in "stm32f7xx_hal_adc.c"
							{
								ptConvCh = ROUND_TO_UINT_CAST(uint16_t, ((float)ptConvCh)/kAdjust);
							}
						}
#endif // AI_SUPPORT_ADJUST_TO_REF

						sum += ptConvCh;
#ifdef AI_DISCARD_MIN_MAX_VALUE
						if(ptConvCh > max) max = ptConvCh;
						if(ptConvCh < min) min = ptConvCh;
#endif // AI_DISCARD_MIN_MAX_VALUE
					}
#ifdef AI_DISCARD_MIN_MAX_VALUE
					if(j > 2) // Si on a compté plus de 2 échantillons dans le Buffer :
					{
						sum -= (max + min); // Retire le Min et le Max des valeurs
						j -= 2; // Décompte les 2 valeurs en moins
					}
#endif // AI_DISCARD_MIN_MAX_VALUE
					if(j > 0) sum/=(float)j; // Moyenne rapide directement en Flotant
					else sum = 0.0f; // Si problème : on met un 0 à la place
//					if(0 != pInitParam->pRawBuffer) pInitParam->pRawBuffer[i] = (uint16_t)(sum + 0.5f); // Conversion forcée en UInt16

					// Détermination de l'emplacement pour la nouvelle RawValue :
					pRawValue = &pRawBase[(i * pInitParam->nbValues4Moy) + (*pId & INT16_MAX)]; // Le bit de signe (b15 = Flag 0x8000) sert à indiquer qu'on a rebouclé sur notre Buffer

					// Méthode FastSum :
					pAccu[i] -= *pRawValue;							// Etape 1 : retire de la Somme la valeur actuelle
					*pRawValue = ROUND_TO_UINT_CAST(uint16_t, sum); // Etape 2 : Insertion forcée de la nouvelle valeur en UInt16
					pAccu[i] += *pRawValue;							// Etape 3 : Ajoute la Nouvelle Valeur à l'Accu

					// Capture de la valeur RAW UInt16 :
					if(0 != pInitParam->pRawBuffer) pInitParam->pRawBuffer[i] = *pRawValue;

				}
				(*pId)++;	// Incrémente l'index de stockage pour la prochaine RawValue

				// Transfert de la Valeur Moyenne à la Fonction de Transformation User :
				if(0 != pInitParam->pFnHandler)
				{
					uint16_t nbVal = *pId; // Récupère combien a de valeurs à moyenner, y compris la dernière récupérée
					if(nbVal & INT16_MIN) nbVal = pInitParam->nbValues4Moy; // Si on a rebouclé (b15 = Flag 0x8000), on a toutes nos Valeurs demandées
					for(int i = 0; i < pInitParam->nbOfChannels; i++)
					{
						tAiFnNewFloatValueHandler* pFnStruct = &pInitParam->pFnHandler[i];
						if(0 != pFnStruct->pFn) pFnStruct->pFn(pFnStruct->pVar, (((float)pAccu[i])/((float)nbVal)));
					}
				}
			}

				// Notifier l'applicatif que nous avons une nouvelle valeur disponible pour chaque entrée prévue :
				pManager->newValue = 1;
				if(0 != pInitParam->pFnEndOfConv)
				{
					pInitParam->pFnEndOfConv(pInitParam);	// Passe notre Pointeur sur la Structure d'Init pour différencier d'un Bloc à l'autre
				}
				pManager->curStep = ADC_STEP_WAIT_SYNC;	// Attendre la prochaine Synchro
				break;
			case ADC_STEP_ERROR_ADC:
				pManager->sabSynchro += pInitParam->errorTempo;	// Ajoute la Tempo d'erreur
				// noBreak;
			default:
				pManager->curStep = ADC_STEP_WAIT_SYNC;
				break;
		}

#ifdef AI_ENABLE_SUPPORT_STATS
		pAiStats++;
#endif // AI_ENABLE_SUPPORT_STATS

	}
}

/******************************************************************************/

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(0 == hadc) return;
// Remarque_Jp le 09/09/2021 : DMA_Abort est nécessaire si dma.mode = Circular, mais inutile en mode Normal !
//	if(0 != hadc->DMA_Handle) HAL_DMA_Abort(hadc->DMA_Handle); // Arrêt Transfert depuis ADC via DMA
	tAI_AdcManager* pManager = AnalogInputs_GetManagerFromHandle(hadc);
	if(0 != pManager)
	{

#ifdef AI_ENABLE_SUPPORT_STATS

		int idOfDev = (pManager - mAI_AdcManager)/sizeof(tAI_AdcManager);
		if(IS_IN_RANGE(idOfDev, 0, AI_NB_MAX_OF_INIT_PARAMS -1))
		{
			mAiStats[idOfDev].convEndTicks = HAL_GetTick();
		}

#endif // AI_ENABLE_SUPPORT_STATS

		if(ADC_STEP_WAIT_ADC == pManager->curStep) pManager->curStep = ADC_STEP_COMPLETE;
	}
}

/******************************************************************************/

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	tAI_AdcManager* pManager = AnalogInputs_GetManagerFromHandle(hadc);
	if(0 != pManager)
	{
		pManager->curStep = ADC_STEP_ERROR_ADC;
#ifdef AI_ENABLE_SUPPORT_STATS
		int idOfDev = (pManager - mAI_AdcManager)/sizeof(tAI_AdcManager);
		if(IS_IN_RANGE(idOfDev, 0, AI_NB_MAX_OF_INIT_PARAMS -1))
		{
			mAiStats[idOfDev].nbAdcError++;	// Pour le moment, on se contente de compter l'évènement !
		}
#endif // AI_ENABLE_SUPPORT_STATS
	}
}

/******************************************************************************/

static tAI_AdcManager* AnalogInputs_GetManagerFromHandle(void* hHandle)
{
	if(0 == hHandle)	return 0; // Erreur
	for(tAI_AdcManager* pManager = mAI_AdcManager; pManager < (tAI_AdcManager*)EndOfAI_AdcManager; pManager++)
	{
		if(0 == pManager->pInitParams) continue;
		if((void*)(pManager->pInitParams->hHandle) == hHandle) return pManager; // On a trouvé notre Structure de Gestion !
	}
	return 0; // NotFound !
}

/******************************************************************************/

#ifdef AI_SUPPORT_ADJUST_TO_CAL

// from https://community.st.com/t5/stm32-mcus/how-to-avoid-a-hardfault-when-icache-is-enabled-on-the-stm32h5/ta-p/630085

#define  WRITE_THROUGH          0x0U  /* Normal memory, write-through. */
#define  NOT_CACHEABLE          0x4U  /* Normal memory, non-cacheable. */
#define  WRITE_BACK             0x4U  /* Normal memory, write-back.    */

static inline void AI_MPU_Config_EnableAccessToVrefIntCal(void)	// Adapted from MPU_Config()
{
  MPU_Attributes_InitTypeDef   attr;
  MPU_Region_InitTypeDef       region;

  /* Disable MPU before perloading and config update */
  HAL_MPU_Disable();

  /* Define cacheable memory via MPU */
  attr.Number             = MPU_ATTRIBUTES_NUMBER0;
  attr.Attributes         = INNER_OUTER(NOT_CACHEABLE);
  HAL_MPU_ConfigMemoryAttributes(&attr);

  /* BaseAddress-LimitAddress configuration */
  region.Enable           = MPU_REGION_ENABLE;
  region.Number           = MPU_REGION_NUMBER0;
  region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
// Remarque_Jp le 09/04/2025 : Limite le déblocage à "ReadOnly", et uniquement sur le WORD "VREFINT_CAL_ADDR" qui intéresse la Librairie "AnalogImputs" :
  region.BaseAddress      = ((uint32_t)VREFINT_CAL_ADDR +0);//0x08FFF800;
  region.LimitAddress     = ((uint32_t)VREFINT_CAL_ADDR +1);//0x08FFFFFF;
  region.AccessPermission = MPU_REGION_ALL_RO; //MPU_REGION_ALL_RW;
  region.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  HAL_MPU_ConfigRegion(&region);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#endif // AI_SUPPORT_ADJUST_TO_CAL

/******************************************************************************/

#ifndef UTILS_UTILS_H_	// en cas d'absence de "utils.h" :
	inline void AnalogInputs_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt) { while(BytesCt-- > 0) { *pDest++ = Value; } }
	inline void AnalogInputs_CopyMemory(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt) { while(BytesCt-- > 0) { *pDest++ = *pSrc++; } }
#endif // UTILS_UTILS_H_

/******************************************************************************/

#ifdef __cplusplus
}
#endif
