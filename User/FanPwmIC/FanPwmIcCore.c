/*
 * FanPwmIcCore.c
 *
 *  Created on: 15 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 19 Déc. 2023
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "FanPwmIC" dans un nouveau Projet :
 *   -> Suivre les indications dans "FanPwmIcConf.h"
 *
 */

#include "FanPwmIcCore.h"	// Pour accès à nos propres déclarations publiques

/******************************************************************************/

#if defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_FAN_PWM_IC)
	#define FPIC_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
//	#warning "DEBUG_FAN_PWM_IC is Active !!!"
#else // (! __DEBUG) && (! DEBUG_FAN_PWM_IC) :
	#define FPIC_HALT_IF_DEBUG()	// Nop();
#endif // __DEBUG ; DEBUG_FAN_PWM_IC

/******************************************************************************/

typedef struct {
	tFanPwmIcInitParams* pInitParams; // Pointeur vers les Paramètres de Configuration User

} tFanPwmIcManager;

/******************************************************************************/

tFanPwmIcManager mFPIC_Manager[FPIC_NB_MAX_OF_INIT_PARAMS] = {0};
FPIC_MAKE_CONST_END_OF_TABLE(tFanPwmIcManager, EndOfFPIC_Manager, mFPIC_Manager);

// Tableau des Liens à charger automatiquement à la Mise sous Tension (pendant InitFanPwmIC) :
FPIC_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(tFanPwmIcInitParams, FPIC_FIRST_INIT_PARAM, FPIC_AFTER_INIT_PARAM);

/******************************************************************************/

static void FanPwmIC_StartPwm(tFanPwmInitParams* pPwmInitParams);
static void FanPwmIC_StopPwm(tFanPwmInitParams* pPwmInitParams);

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS
	static uint8_t FanPwmIC_ApplyNewPwmInitParams(tFanPwmData* pPwmData, tFanPwmInitParams* pNewPwmInitParams);
#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK
	static uint8_t FanPwmIC_ApplyNewIcDataInOut(tFanIcData* pIcData, tFanIcSrcDatas* pNewIcSrcData);
#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK

#ifdef FPIC_FEED_BACK_IS_FLOAT
	static float FanPwmIC_AddFloatToMoyManager(FPIC_MoyManager_Float* pMoyManager, float newValue, uint16_t maxCt);
	static void FanPwmIC_ClearFloatMoyManager(FPIC_MoyManager_Float* pMoyManager, uint16_t maxCt);
#else // !FPIC_FEED_BACK_IS_FLOAT
	static float FanPwmIC_AddUInt16ToMoyManager(FPIC_MoyManager_UInt16* pMoyManager, uint16_t newValue, uint16_t maxCt);
	static void FanPwmIC_ClearUInt16MoyManager(FPIC_MoyManager_UInt16* pMoyManager, uint16_t maxCt);
#endif // FPIC_FEED_BACK_IS_FLOAT

/******************************************************************************/

inline __attribute__((always_inline)) void InitFanPwmIC(void)			// A appeler dans la partie Init Software (main.c)
{
	tFanPwmIcManager* pManager = mFPIC_Manager;
	FPIC_MAKE_VAR_AND_SET_VALUE(tFanPwmIcInitParams*, pInitParams, FPIC_FIRST_INIT_PARAM);
	for( ; (pInitParams < (tFanPwmIcInitParams*)FPIC_AFTER_INIT_PARAM)
		&& (pManager < (tFanPwmIcManager*)EndOfFPIC_Manager); pInitParams++)
	{
		if(0 != FanPwmIC_Register_InitParam(pInitParams, pManager))
		{
			pManager++; // Pointe le prochain emplacement prêt à recevoir les infos
		}
	}

	// Vérifier s'il reste d'éventuels InitParams non chargés :
	if(pInitParams < (tFanPwmIcInitParams*)FPIC_AFTER_INIT_PARAM)
	{
		FPIC_HALT_IF_DEBUG(); // Bloquer au RunTime en DEBUG si mauvaise Configuration => Recalculer FPIC_NB_MAX_OF_INIT_PARAMS (cf. "FanPwmIcConf.h") !
	}
}

/******************************************************************************/

uint16_t FanPwmIC_Register_InitParam(tFanPwmIcInitParams* pNewInitParam, void* pThisManager) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	if(0 == pNewInitParam) return 0;

	FPIC_MAKE_VAR_AND_SET_VALUE(tFanPwmIcManager*, pManager, pThisManager);
	if(0 == pManager) // Si inconnu : rechercher un emplacement libre :
	{
		for(pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
		{
			if(0 == pManager->pInitParams) break; // On a trouvé un emplacement dispo
		}
		if(pManager >= (tFanPwmIcManager*)EndOfFPIC_Manager) { pManager = 0; }
	}
	if(0 == pManager) return 0; // Failure : insufficient FPIC_NB_MAX_OF_INIT_PARAMS !
	if(0 != pManager->pInitParams) return 0; // Failure : This one is not free !

	// Tout est OK pour Préparer la liaison :
	FanPwmIc_FillMemory((void*)pManager, 0, sizeof(tFanPwmIcManager)); // Nettoyer notre propre Structure avant de la Configurer

	// Charger la partie Gestion PWM :
	tFanPwmInitParams* pPwmInitParams = &pNewInitParam->PwmInitParams;	// Considère les Params PWM :
	if(0 != pPwmInitParams)
	{
		if(0 != pPwmInitParams->pDatas)
		{
			FanPwmIc_FillMemory((void*)pPwmInitParams->pDatas, 0, sizeof(tFanPwmData));
			pPwmInitParams->pDatas->pPwmInitParams = pPwmInitParams;
		}
		if(0 != pPwmInitParams->htim)	HAL_TIM_PWM_Stop(pPwmInitParams->htim, pPwmInitParams->PwmChannelId);
	}

	// Charger la partie Gestion InputCaptures :
	tFanIcInitParams* pIcInitParams = &pNewInitParam->IcInitParams;	// Considère les Params InputCapture :
	if(0 != pIcInitParams)
	{
		FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcSrcDatas*, pSrcDatas, pIcInitParams->pSrcDatas);
		if(0 != pSrcDatas)
		{
			FanPwmIc_FillMemory((void*)pSrcDatas, 0, sizeof(tFanIcSrcDatas));
			pSrcDatas->pIcInitParams = pIcInitParams;
//			pSrcDatas->pDatasOut = pIcInitParams->pDatasOut;
		}

		FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcData*, pDatasOut, pIcInitParams->pIcDatas);
		if(0 != pDatasOut)
		{
			FanPwmIc_FillMemory((void*)pDatasOut, 0, sizeof(tFanIcData));
			pDatasOut->pIcInitParams = pIcInitParams;
			pDatasOut->pDatas = pIcInitParams->pSrcDatas;
		}
		if(0 != pIcInitParams->htim)	HAL_TIM_IC_Start_IT(pIcInitParams->htim, pIcInitParams->IcChannelId);
	}

	// Tout est OK pour Activer la liaison :
	pManager->pInitParams = pNewInitParam;	// Sauvegarde le lien vers les Infos d'Init pour s'y référer ultérieurement
	return 1;
}

/******************************************************************************/

inline void Handle_FanPwmIC_RT_10ms(void)	// A appeler depuis une Base de Temps RT @ 10ms
{
	tFanPwmIcInitParams* pInitParam = 0;
	tFanIcInitParams*	 pIcInitParams = 0;
	tFanIcSrcDatas*			 pDatas = 0;

	for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
	{
		pInitParam = pManager->pInitParams;
		if(0 == pInitParam) continue;

		pIcInitParams = &pInitParam->IcInitParams;
		if(0 == pIcInitParams) continue;

		pDatas = (tFanIcSrcDatas*)pIcInitParams->pSrcDatas;
		if(0 == pDatas) continue;

		// On a bien accès à un Sablier de TimeOut :
		if(pDatas->sabTimeOut > 0) pDatas->sabTimeOut--;
	}
}

/******************************************************************************/

void Gestion_FanPwmIC(void)
{
#define MAX_TRIES_COPY_CAPTURED_TACHY	3
	tFanPwmIcInitParams* pInitParam = 0;
	tFanIcInitParams*	 pIcInitParams = 0;
	tFanIcSrcDatas*		 pDatas = 0;

	int i = 0;
	uint32_t curVal[2] = {0};
	uint32_t newCt = 0;
	uint32_t newDelta = 0;
	float	 newFeedBack = 0.0f;

	for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
	{
		pInitParam = pManager->pInitParams;
		if(0 == pInitParam) continue;

		pIcInitParams = &pInitParam->IcInitParams;
		if(0 == pIcInitParams) continue;

		pDatas = (tFanIcSrcDatas*)pIcInitParams->pSrcDatas;
		if(0 == pDatas) continue;

		// Traite le TimeOut sur l'entrée InputCapture :
		if(0 == pDatas->sabTimeOut)	// En cas de TimeOut :
		{
			if(0 != pDatas->FeedBackValue)	// Premier TimeOut après un précédent Feedback :
			{
				pDatas->FeedBackValue = 0.0f; // Effacer la variable de Sortie générale

				// Effacer également les éventuelles Variables intermédiaires :
#ifdef FPIC_GET_LAST_DELTA_TIME
				pDatas->lastDeltaTime = 0;
#endif // FPIC_GET_LAST_DELTA_TIME

#ifdef FPIC_GET_LAST_FEED_BACK
				pDatas->lastFeedBackValue = 0.0f;
#endif // FPIC_GET_LAST_FEED_BACK

			}
			if(0 != pDatas->curCapturedCount)
			{
				pDatas->newCapturedCount = 0; //  Repartir du départ : ignorer les captures déjà présentes !
				pDatas->curCapturedCount = 0; //  Réinitialiser également notre capture locale

				// CleanUp de la Moyenne tournante :
#if defined(FPIC_MOY_FEED_BACK_CONV) && (FPIC_MOY_FEED_BACK_CONV > 0)
	#ifdef FPIC_FEED_BACK_IS_FLOAT
				FanPwmIC_ClearFloatMoyManager(&pDatas->FeedBackConv, FPIC_MOY_FEED_BACK_CONV);
	#else // !FPIC_FEED_BACK_IS_FLOAT
				FanPwmIC_ClearUInt16MoyManager(&pDatas->FeedBackConv, FPIC_MOY_FEED_BACK_CONV);
	#endif // FPIC_FEED_BACK_IS_FLOAT
#endif // FPIC_MOY_FEED_BACK_CONV

				// Vérification de la Variable partagée avec l'Interruption :
				if(0 != pDatas->newCapturedCount) pDatas->newCapturedCount = 0; //  Repartir du départ : ignorer les captures déjà présentes !
			}
			continue;
		}

		// Traite le signal sur l'entrée InputCapture :
		for(i = 0; i < MAX_TRIES_COPY_CAPTURED_TACHY; i++)
		{
			newCt = pDatas->newCapturedCount;
			FanPwmIc_CopyMemory((void*)&curVal, (void*)pDatas->CapturedValue, sizeof(curVal));
			if(newCt == pDatas->newCapturedCount) break;	// OK, si l'index n'a pas changé : on peut traiter
		}
		if(i >= MAX_TRIES_COPY_CAPTURED_TACHY)	continue;	// Trop d'échecs de Capture cette fois-ci !
		if(newCt == pDatas->curCapturedCount)	continue;	// Captures actuelles déjà traitées précédemment.
		if(newCt < 2) continue;	// Pas encore assez d'échantillons pour faire un calcul

		// Traitement du rebouclage entre les Captures :
		if(0 != newCt%2)	// Cas où lastValue est en [0] et prevValue en [1] :
		{
			newDelta = curVal[0];
			curVal[0] = curVal[1];
			curVal[1] = newDelta;
		}
		newDelta = curVal[1] - curVal[0]; // lastValue en [1] & prevValue en [0]
		if(curVal[1] <= curVal[0]) newDelta += UINT16_MAX; // OU pIcInitParams->htim->Init.Period; si != MAX

//		// Remarque_Jp : 9780 = deltaTime @ 4000RPM => Range = [ 520 ; 4000RPM ] :
//		if(newDelta < 9750) newDelta += UINT16_MAX; // OU pIcInitParams->htim->Init.Period; si != MAX -> suppose 1 tour complet en+

		newFeedBack = pIcInitParams->kFeedBack / ((float)newDelta);

#ifdef FPIC_GET_LAST_DELTA_TIME
		pDatas->lastDeltaTime = newDelta;
#endif // FPIC_GET_LAST_DELTA_TIME

#ifdef FPIC_GET_LAST_FEED_BACK
		pDatas->lastFeedBackValue = newFeedBack;
#endif // FPIC_GET_LAST_FEED_BACK

		// Insertion de la nouvelle valeur dans la Moyenne tournante :
#if defined(FPIC_MOY_FEED_BACK_CONV) && (FPIC_MOY_FEED_BACK_CONV > 0)
		newFeedBack =
			#ifdef FPIC_FEED_BACK_IS_FLOAT
						FanPwmIC_AddFloatToMoyManager(&pDatas->FeedBackConv, newFeedBack, FPIC_MOY_FEED_BACK_CONV);
			#else // !FPIC_FEED_BACK_IS_FLOAT
						FanPwmIC_AddUInt16ToMoyManager(&pDatas->FeedBackConv, (uint16_t)newFeedBack, FPIC_MOY_FEED_BACK_CONV);
			#endif // FPIC_FEED_BACK_IS_FLOAT
#endif //FPIC_MOY_FEED_BACK_CONV

#ifdef FPIC_FEED_BACK_IS_FLOAT
		pDatas->FeedBackValue = newFeedBack;
#else // !FPIC_FEED_BACK_IS_FLOAT
		pDatas->FeedBackValue = (uint16_t)newFeedBack;
#endif // FPIC_FEED_BACK_IS_FLOAT

		pDatas->curCapturedCount = newCt;	// Mémorise l'Index de Capture que l'on vient de traiter
	}
}

/******************************************************************************/

void FanPwmIC_UpdatePwmValue(tFanPwmData* pPwmData, uint16_t newValue)
{
	if(0 == pPwmData) return;	// Vérifie le Pointeur de Structure

	if(pPwmData->PwmValue == newValue) return;	// Ignore si déjà la valeur actuelle
	if(0 == pPwmData->pPwmInitParams) return;	// Abandonne si on a pas/perdu les infos d'Init

	// OK pour Mettre à Jour :
	FPIC_MAKE_VAR_AND_SET_VALUE(tFanPwmInitParams*, pPwmInitParams, pPwmData->pPwmInitParams);
	newValue &= INT16_MAX;	// Uniquement partie non signée prise en compte
	if(newValue > 0)
	{
		if(newValue > pPwmInitParams->MaxValue) newValue = pPwmInitParams->MaxValue;	// Plafonnement au Max autorisé
		__HAL_TIM_SET_COMPARE(pPwmInitParams->htim, pPwmInitParams->PwmChannelId, newValue);
		if(0 == pPwmData->PwmValue) FanPwmIC_StartPwm(pPwmInitParams);	// Démarrer la PWM si elle était arrêtée
		pPwmData->PwmValue = newValue;
	} else {
		FanPwmIC_StopPwm(pPwmInitParams);	// Forcer l'arrêt complet de la PWM
		pPwmData->PwmValue = 0;
	}
}

/******************************************************************************/

void FanPwmIC_StartPwm(tFanPwmInitParams* pPwmInitParams)
{
	if(0 == pPwmInitParams) return;
	if(0 == pPwmInitParams->htim) return;

	HAL_TIM_PWM_Start(pPwmInitParams->htim, pPwmInitParams->PwmChannelId);
}

/******************************************************************************/

void FanPwmIC_StopPwm(tFanPwmInitParams* pPwmInitParams)
{
	if(0 == pPwmInitParams) return;
	if(0 == pPwmInitParams->htim) return;

	HAL_TIM_PWM_Stop(pPwmInitParams->htim, pPwmInitParams->PwmChannelId);
}

/******************************************************************************/

void FanPwmIC_StopAllPwm(void)
{
	tFanPwmIcInitParams* pInitParam = 0;

	for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
	{
		pInitParam = pManager->pInitParams;
		if(0 == pInitParam) continue;

		FanPwmIC_StopPwm(&pInitParam->PwmInitParams);	// faire Appliquer la demande d'arrêt
	}
}

/******************************************************************************/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // Handler partagé par toutes les InputCaptures
{
	tFanPwmIcInitParams* pInitParam = 0;
	tFanIcInitParams*	 pIcInitParams = 0;

	for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
	{
		pInitParam = pManager->pInitParams;
		if(0 == pInitParam) continue;

		pIcInitParams = &pInitParam->IcInitParams;
		if(0 == pIcInitParams) continue;

		// Vérifier si ça nous concerne :
		if(htim != pIcInitParams->htim) continue;
		if(htim->Channel != pIcInitParams->IcChannelFlag) continue;

		// Oui, cet évènement nous concerne :
		FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcSrcDatas*, pDatas, pIcInitParams->pSrcDatas);
		if(0 == pDatas) continue; // Laisser une 2° chance au cas où une meilleure Configuration soit disponible !

		// Capturer la "date" exacte de l'évènement :
		pDatas->CapturedValue[pDatas->newCapturedCount%2] = HAL_TIM_ReadCapturedValue(htim, pIcInitParams->IcChannelId);
		pDatas->newCapturedCount++;	// Prépare pour le tour suivant
		pDatas->sabTimeOut = pIcInitParams->TimeOut_10ms;	// Recharge le TimeOut de gestion
		return; // Quitter directement le Handler
	}

	// L'évènement n'a pas pu être traité par notre Librairie => Poursuivre la chaine des Notifications :
#ifdef NEXT_HAL_TIM_IC_CaptureCallback
	NEXT_HAL_TIM_IC_CaptureCallback(htim);	// ATTENTION : Fonction Non Testée !
#endif // NEXT_HAL_TIM_IC_CaptureCallback

}

/******************************************************************************/

#ifdef FPIC_FEED_BACK_IS_FLOAT

float FanPwmIC_AddFloatToMoyManager(FPIC_MoyManager_Float* pMoyManager, float newValue, uint16_t maxCt)
{
	REFRAME_TO_MAX_OF_RANGE(pMoyManager->maxCt, 1, maxCt);
	if(pMoyManager->maxCt < 1) return 0.0f;
	float* pTabData = (void*)pMoyManager + sizeof(FPIC_MoyManager_Float);	// Pointe le Tableau de Data qui suit notre Structure
	uint16_t curIndex = pMoyManager->curIdx;
	if(curIndex >= pMoyManager->maxCt) { curIndex = 0; }	// reboucle en cas de dépassement de l'index
	pMoyManager->sumTab -= pTabData[curIndex];
	pMoyManager->sumTab += pTabData[curIndex] = newValue;
	pMoyManager->curIdx = curIndex +1;	// Décale vers l'emplacement suivant
	if(++pMoyManager->nbInTab > pMoyManager->maxCt) { pMoyManager->nbInTab = pMoyManager->maxCt; } // Plafonne le nb d'éléments au Max
	return ((float)(pMoyManager->sumTab) / (float)(pMoyManager->nbInTab));
}

#endif // FPIC_FEED_BACK_IS_FLOAT

/******************************************************************************/

#ifndef FPIC_FEED_BACK_IS_FLOAT

float FanPwmIC_AddUInt16ToMoyManager(FPIC_MoyManager_UInt16* pMoyManager, uint16_t newValue, uint16_t maxCt)
{
	REFRAME_TO_MAX_OF_RANGE(pMoyManager->maxCt, 1, maxCt);
	if(pMoyManager->maxCt < 1) return 0.0f;
	uint16_t* pTabData = (void*)pMoyManager + sizeof(FPIC_MoyManager_UInt16);	// Pointe le Tableau de Data qui suit notre Structure
	uint16_t curIndex = pMoyManager->curIdx;
	if(curIndex >= pMoyManager->maxCt) { curIndex = 0; }	// reboucle en cas de dépassement de l'index
	pMoyManager->sumTab -= pTabData[curIndex];
	pMoyManager->sumTab += pTabData[curIndex] = newValue;
	pMoyManager->curIdx = curIndex +1;	// Décale vers l'emplacement suivant
	if(++pMoyManager->nbInTab > pMoyManager->maxCt) { pMoyManager->nbInTab = pMoyManager->maxCt; } // Plafonne le nb d'éléments au Max
	return ((float)(pMoyManager->sumTab) / (float)(pMoyManager->nbInTab));
}

#endif // FPIC_FEED_BACK_IS_FLOAT

/******************************************************************************/

void FanPwmIC_ClearFloatMoyManager(FPIC_MoyManager_Float* pMoyManager, uint16_t maxCt)
{
	FanPwmIc_FillMemory((void*)pMoyManager, 0, sizeof(FPIC_MoyManager_Float) + maxCt * sizeof(float));
}

/******************************************************************************/

void FanPwmIC_ClearUInt16MoyManager(FPIC_MoyManager_UInt16* pMoyManager, uint16_t maxCt)
{
	FanPwmIc_FillMemory((void*)pMoyManager, 0, sizeof(FPIC_MoyManager_UInt16) + maxCt * sizeof(uint16_t));
}

/******************************************************************************/

inline uint16_t FanPwmIC_getFanVoltage_mV(tFanPwmData* pPwmData)
{
	if(0 != pPwmData) return pPwmData->PwmValue;
	return 0;
}

/******************************************************************************/

inline uint16_t FanPwmIC_getFanSpeed(tFanIcData* pIcData)
{
	if(0 == pIcData) return 0;
	FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcSrcDatas*, pDatas, pIcData->pDatas);
	if(0 == pDatas) return 0;
	// Tout est OK :
	return pDatas->FeedBackValue;
}

inline uint16_t FanPwmIC_getLastFanSpeed(tFanIcData* pIcData)
{
#ifdef FPIC_GET_LAST_FEED_BACK
	if(0 != pIcData)
	{
		FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcSrcDatas*, pDatas, pIcData->pDatas);
		if(0 == pDatas) return 0;
		// Tout est OK :
		return pDatas->lastFeedBackValue;
	}
#endif // FPIC_GET_LAST_FEED_BACK
	return 0;
}

inline uint16_t FanPwmIC_getLastDeltaTime(tFanIcData* pIcData)
{
#ifdef FPIC_GET_LAST_DELTA_TIME
	if(0 != pIcData)
	{
		FPIC_MAKE_VAR_AND_SET_VALUE(tFanIcSrcDatas*, pDatas, pIcData->pDatas);
		if(0 == pDatas) return 0;
		// Tout est OK :
		return pDatas->lastDeltaTime;
	}
#endif // FPIC_GET_LAST_DELTA_TIME
	return 0;
}

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS

void FanPwmIC_SwapPwmDataPtr(tFanPwmData* pPwmData1, tFanPwmData* pPwmData2, uint8_t swapAction)
{
	tFanPwmInitParams* pPwmInitParams1 = 0;
	tFanPwmInitParams* pPwmInitParams2 = 0;

	if(FanPwmIc_Swap_Toggle == swapAction) // Toggle :
	{
		if(0 != pPwmData2) pPwmInitParams1 = pPwmData2->pPwmInitParams;	// L'autre
		if(0 != pPwmData1) pPwmInitParams2 = pPwmData1->pPwmInitParams;	// Le 1er
	}
	else // Rechercher les Paramètres d'Origine :
	{
		tFanPwmIcInitParams* pInitParam = 0;
		tFanPwmInitParams*	 pPwmInitParams = 0;
		for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
		{
			pInitParam = pManager->pInitParams;
			if(0 == pInitParam) continue;

			pPwmInitParams = &pInitParam->PwmInitParams;
			if(0 == pPwmInitParams) continue;

			if(pPwmInitParams->pDatas == pPwmData1) pPwmInitParams1 = pPwmInitParams;
			if(pPwmInitParams->pDatas == pPwmData2) pPwmInitParams2 = pPwmInitParams;

			if( (0 != pPwmInitParams1) && (0 != pPwmInitParams2) ) break;
		}

		if(FanPwmIc_Swap_Apply == swapAction) // Demande d'inversion :
		{
			tFanPwmInitParams* tmpPwmInitParams = pPwmInitParams1;
			pPwmInitParams1 = pPwmInitParams2;
			pPwmInitParams2 = tmpPwmInitParams;
		}
	}

	// Applique les nouveaux Paramètres :
	FanPwmIC_ApplyNewPwmInitParams(pPwmData1, pPwmInitParams1);
	FanPwmIC_ApplyNewPwmInitParams(pPwmData2, pPwmInitParams2);
}

#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS

uint8_t FanPwmIC_ApplyNewPwmInitParams(tFanPwmData* pPwmData, tFanPwmInitParams* pNewPwmInitParams)
{
	if(0 == pPwmData) return 0;
	uint16_t prevPwmValue = pPwmData->PwmValue;
	if(0 == pNewPwmInitParams) FanPwmIC_StopPwm(pPwmData->pPwmInitParams);	// Pas de nouveaux Paramètres => Arrêt implicite !
	pPwmData->pPwmInitParams = pNewPwmInitParams;	// Lier les Nouveaus Paramètres
	pPwmData->PwmValue = 0;	// Force un restart de la PWM si nécessaire
	FanPwmIC_UpdatePwmValue(pPwmData, prevPwmValue | INT16_MIN); // Forcer Mise à jour avec la nouvelle consigne
	return 1;
}

#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS

uint8_t FanPwmIC_ArePwmSwapped(tFanPwmData* pPwmData1, tFanPwmData* pPwmData2)
{
	uint8_t nbSwap = 0;
	tFanPwmInitParams* pPwmInitParams;
	if(0 != pPwmData1)
	{
		pPwmInitParams = pPwmData1->pPwmInitParams;
		if(0 != pPwmInitParams) nbSwap |= (pPwmInitParams->pDatas == pPwmData2) ? 1 : 0;
	}
	if(0 != pPwmData2)
	{
		pPwmInitParams = pPwmData2->pPwmInitParams;
		if(0 != pPwmInitParams) nbSwap |= (pPwmInitParams->pDatas == pPwmData1) ? 2 : 0;
	}
	return nbSwap;
}

#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK

void FanPwmIC_SwapIcDataOutPtr(tFanIcData* pIcData1, tFanIcData* pIcData2, uint8_t swapAction)
{
	tFanIcSrcDatas* pIcSrcDatas1 = 0;
	tFanIcSrcDatas* pIcSrcDatas2 = 0;

	if(FanPwmIc_Swap_Toggle == swapAction) // Toggle :
	{
		if(0 != pIcData2) pIcSrcDatas1 = pIcData2->pDatas;	// L'autre
		if(0 != pIcData1) pIcSrcDatas2 = pIcData1->pDatas;	// Le 1er
	}
	else // Rechercher les Paramètres d'Origine :
	{
		tFanPwmIcInitParams* pInitParam = 0;
		tFanIcInitParams*	 pIcInitParams = 0;
		for(tFanPwmIcManager* pManager = mFPIC_Manager; pManager < (tFanPwmIcManager*)EndOfFPIC_Manager; pManager++)
		{
			pInitParam = pManager->pInitParams;
			if(0 == pInitParam) continue;

			pIcInitParams = &pInitParam->IcInitParams;
			if(0 == pIcInitParams) continue;

			if(pIcInitParams->pIcDatas == pIcData1) pIcSrcDatas1 = pIcInitParams->pSrcDatas;
			if(pIcInitParams->pIcDatas == pIcData2) pIcSrcDatas2 = pIcInitParams->pSrcDatas;

			if( (0 != pIcSrcDatas1) && (0 != pIcSrcDatas2) ) break;
		}

		if(FanPwmIc_Swap_Apply == swapAction) // Demande d'inversion :
		{
			tFanIcSrcDatas* tmpIcSrcDatas = pIcSrcDatas1;
			pIcSrcDatas1 = pIcSrcDatas2;
			pIcSrcDatas2 = tmpIcSrcDatas;
		}
	}

	// Applique les nouveaux Paramètres :
	FanPwmIC_ApplyNewIcDataInOut(pIcData1, pIcSrcDatas1);
	FanPwmIC_ApplyNewIcDataInOut(pIcData2, pIcSrcDatas2);
}

#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK

uint8_t FanPwmIC_ApplyNewIcDataInOut(tFanIcData* pIcData, tFanIcSrcDatas* pNewIcSrcData)
{
	if(0 == pIcData) return 0;
	pIcData->pDatas = pNewIcSrcData;
	return 1;
}

#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK

/******************************************************************************/

#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK

uint8_t FanPwmIC_AreIcSwapped(tFanIcData* pIcData1, tFanIcData* pIcData2)
{
	uint8_t nbSwap = 0;
	tFanIcInitParams* pIcInitParams;
	if(0 != pIcData1)
	{
		pIcInitParams = pIcData1->pIcInitParams;
		if(0 != pIcInitParams) nbSwap |= (pIcInitParams->pIcDatas == pIcData2) ? 1 : 0;
	}
	if(0 != pIcData2)
	{
		pIcInitParams = pIcData2->pIcInitParams;
		if(0 != pIcInitParams) nbSwap |= (pIcInitParams->pIcDatas == pIcData1) ? 1 : 0;
	}
	return nbSwap;
}

#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK


/******************************************************************************/

void FanPwmIC_SwapPwmIc(tFanPwmData* pPwmData1, tFanIcData* pIcData1, tFanPwmData* pPwmData2, tFanIcData* pIcData2, uint8_t swapAction)
{

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS
	FanPwmIC_SwapPwmDataPtr(pPwmData1, pPwmData2, swapAction);
#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK
	FanPwmIC_SwapIcDataOutPtr(pIcData1, pIcData2, swapAction);
#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK

}
