/*
 * UartComCore.c
 *
 *  Created on: Dec 15, 2020
 *  Original Author: j.proux
 *
 *  Updated on: 21 Feb. 2025
 *  Updated by: j.proux
 *
 *  Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 *	-> il faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "UartCom" dans un nouveau Projet :
 *   -> Suivre les indications dans "UartComConf.h"
 *
 */

//#include <stdint.h>			// Pour accès aux types "int*_t" & "uint*_t"
#include "UartComCore.h"	// Pour accès à nos propres déclarations publiques
#include "UartComConf.h"	// Pour accès à la Configuration User souhaitée

/******************************************************************************/

#if defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_UART_COM)
	#define UART_COM_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
//	#warning "DEBUG_UART_COM is Active !!!"
#else // (! __DEBUG) && (! DEBUG_UART_COM) :
	#define UART_COM_HALT_IF_DEBUG()	// Nop();
#endif // __DEBUG ; DEBUG_UART_COM

/******************************************************************************/

// Synthèse des Fonctions par défaut, si pas définies par le User :
#ifndef UART_COM_USR_FN_START_TRANSMIT
	#define UART_COM_USR_FN_START_TRANSMIT UART_COM_DEF_FN_START_TRANSMIT	// HAL_UART_Transmit_DMA		// pour HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
#endif //UART_COM_USR_FN_START_TRANSMIT

#ifndef UART_COM_USR_FN_START_RECEIVE
	#define UART_COM_USR_FN_START_RECEIVE UART_COM_DEF_FN_START_RECEIVE 	// HAL_UART_Receive_IT			// pour HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
#endif //UART_COM_USR_FN_START_RECEIVE

#ifndef UART_COM_USR_FN_ABORT_TRANSMIT
	#define UART_COM_USR_FN_ABORT_TRANSMIT UART_COM_DEF_FN_ABORT_TRANSMIT	// HAL_UART_AbortTransmit_IT	// pour HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart)
#endif // UART_COM_USR_FN_ABORT_TRANSMIT

#ifndef UART_COM_USR_FN_ABORT_RECEIVE
	#define UART_COM_USR_FN_ABORT_RECEIVE UART_COM_DEF_FN_ABORT_RECEIVE 	// HAL_UART_AbortReceive_IT		// pour HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart)
#endif //UART_COM_USR_FN_ABORT_RECEIVE

#ifndef UART_COM_USR_FN_DEINIT
	#define UART_COM_USR_FN_DEINIT UART_COM_DEF_FN_DEINIT					// HAL_UART_DeInit				// pour HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)
#endif // UART_COM_USR_FN_DEINIT

#ifndef UART_COM_USR_FN_GET_ERR_CNTXT
	#define UART_COM_USR_FN_GET_ERR_CNTXT UART_COM_DEF_FN_GET_ERR_CNTXT 	// UartComGetUartErrorContext	// pour eUartComErrorContext UartComGetUartErrorContext(UART_HandleTypeDef *hUart)
#endif // UART_COM_USR_FN_GET_ERR_CNTXT

/******************************************************************************/

typedef struct _tBusyFree
{
	void* pBusy;
	void* pFree;
} tBusyFree;

typedef struct _tBufManager
{
	tBufParams*	pBuf;
	tBusyFree	pHigh;
	tBusyFree	pLow;
} tBufManager;

/******************************************************************************/

#if defined(NB_SHARED_UART_COM_FRAME_PARAMS) && (NB_SHARED_UART_COM_FRAME_PARAMS > 0)
	tComFrameParams	shrdBufInfo[NB_SHARED_UART_COM_FRAME_PARAMS];
#endif // NB_SHARED_UART_COM_FRAME_PARAMS

#if defined(UART_COM_SHARED_BUF_TX_SIZE) && (UART_COM_SHARED_BUF_TX_SIZE > 0)
	uint8_t shrdTxBuffer[UART_COM_SHARED_BUF_TX_SIZE];
	const tBufParams shrdBufBloc = { (void*)shrdTxBuffer, sizeof(shrdTxBuffer) };
#endif // UART_COM_SHARED_BUF_TX_SIZE

tBufManager shrdBufTxMgr = {0};

typedef struct _tUartComRegularTx
{
	tUartComInitRegularTx*	pInitParams;
	uint16_t curDelay;
	uint16_t endDelay;
#ifdef UART_COM_SUPPORT_VAR_DELAY
	uint16_t regDelay;	// Pour Modification au RunTime d'un Intervalle de temps Variable
#endif // UART_COM_SUPPORT_VAR_DELAY
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	uint16_t nbChangeDelay;
#endif // UART_COM_SUPPORT_STATS
} tUartComRegularTx;

#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	tUartComRegularTx shrdRegTx[UART_COM_MAX_REG_TX];
	UART_COM_MAKE_CONST_END_OF_TABLE(tUartComRegularTx, EndOfShrdRegTx, shrdRegTx);
#endif // UART_COM_MAX_REG_TX

/******************************************************************************/

uint8_t UartComRedivIT_100ms = 0;

typedef struct _tUartComManager
{
	tUartComInitParams* pInitParams; // Pointeur vers les Paramètres de Configuration User
	tBufManager bufMgr;			// Gestionnaire du Buffer associé

	tComFrameParams	bufInfo[NB_FRAME_PARAMS_PER_COM_MANAGER]; // Pour un stockage privé des FrameInfos qui nous sont affectés
	tBufSzBloc	curRxBufInfo;	// Structure de travail de Réception bas niveau

	tBufParams	curTxBufInfo;	// Structure de travail d'envoi bas niveau
	tComFrameParams* pCurFrameInfo;	// Pointeur sur la Frame Info en cours d'envoi par le bas niveau
	tComFrameParams* pNxtFrameInfo;	// Pointeur sur la prochaine Frame Info à envoyer, si nécessaire
	void*		pChkTxEcho;		// Pointeur sur le prochain Byte dont il faut Vérifier l'Echo
	uint8_t*	pNextRxByte;	// Pointeur sur l'emplacement du prochain Byte à recevoir
	uint32_t*	pLastRxFrame;	// Pointeur sur une variable externe remise à Zéro à chaque Réception de Trame et auto-Incrémentée (base = IT @ 100ms)

	uint16_t sabEndOfRxFrame;	// Délai restant avant Fin de Trame (dépend notamment du Protocole, base = IT @ 1ms)
	uint16_t sabTimeOut4Reply;	// Délai maximal autorisé pour transmettre une Réponse immédiatement (base = IT @ 1ms)

	uint16_t sabReSetRxBufPtr;	// Délai maximal restant sans Réception avant Relance Réception (base = IT @ 100ms)
	uint16_t sabReady4Tx;		// Délai restant avant autorisation d'Emettre à nouveau (base = IT @ 1ms)

	// Variables pour la Gestion des Envois espacés dans le temps :
	uint16_t sabMayTxNextByte;	// Tempo avant autorisation envoi du prochain Byte (base = IT @ 1ms)

#ifdef UART_COM_ENABLE_IBUS	// cf. "UartComConf.h"
	uint8_t  sabPrediv8ms;		// Spécial iBus : Rediviseur 1ms -> 8ms pour la Synchro iBus
	uint8_t  iBusSync;			// Valeur de Synchro Temporelle iBus
#endif // UART_COM_ENABLE_IBUS

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	uint32_t nbFramesRx;
	uint32_t nbBytesRx;
	uint32_t nbBlocsRx;
	uint32_t nbBlocsCpy;
	uint32_t nbChkEcho;
	uint32_t nbErrEcho;
#define NB_ERR_ECHO_COUNT	5
	uint32_t nbErrEchId[NB_ERR_ECHO_COUNT];
	uint32_t nbEchoVld;
	uint32_t nbEchoEOF;
	uint32_t nbReplyPdg;
	uint32_t nbRxSmall;
	uint32_t nbReSetRx;
	uint32_t nbInitRxBuf;
	uint32_t nbAbortRx;
	uint32_t nbReStartRx;
	uint32_t nbReSetRxFail;
	uint32_t nbTxPrivBuf;
	uint32_t nbTxShrdBuf;
	uint32_t nbTxBufLoaded;
	uint32_t nbTxBufDiscarded;
	uint32_t nbRegTxLoaded;
	uint32_t nbRegTxSent;
	uint32_t nbTxMainRelease;
	uint32_t nbTxReLoopRelease;
	uint32_t nbTxQuickRelease;
	uint32_t nbTxRegTxRelease;
	uint32_t nbTxAck2;
	uint32_t nbTxBytesSentIT;
	uint32_t nbTxBytesNotSentIT;
	uint32_t nbTxBytesDlyd4Sync;
	uint32_t nbTxBytesDlyd4RdyTx;
	uint32_t nbTxBytesDlyd4SabTx;
	uint32_t nbRqLargerBuffer;
	uint32_t nbRqEmptyBuffer;
	uint32_t nbRqInitNewTxFrame;
	uint32_t nbDoTransmit;
	uint32_t nbDoPostFrame;
	uint32_t nbMayPostFrameNow;
	uint32_t nbPostFrameNowErr;
	uint32_t nbPostFrameDelayed;
	uint32_t nbPostFrameLater;
	uint32_t nbTxBytesSentCB;
	uint32_t nbTxFramesSentCB;
	uint32_t nbErrCallBack;
	uint32_t nbErrXxCallBack;
	uint32_t nbErrTxCallBack;
	uint32_t nbErrRxCallBack;
	uint32_t nbErrRxCbDscrd[2];
	uint32_t nbErrTxHandled;
	uint32_t nbErrTxRetries;
//#ifdef UART_COM_SUPPORT_TX_RX_PIN
//	uint32_t nbTxPinChange[1];
//#endif // UART_COM_SUPPORT_TX_RX_PIN
#define NB_COUNT_RX_SMALL	6
	uint32_t nbErrRxSmlSz[NB_COUNT_RX_SMALL];
	uint32_t nbErrRxSml3F;
	uint32_t nbErrRxSmlFF;
	uint32_t nbErrRxSmlXX;
	uint16_t lastRxSmall;
	uint8_t lastErrRxSml[2];

#endif // UART_COM_SUPPORT_STATS

	union {
		uint8_t Flags;
		struct {
			unsigned mayDiscardRx:1; // Pour demander à ignorer la Réception en cours (Par exemple : Erreur de Réception détectée)
			unsigned isLoaded:1;	// Pour savoir si le Périphérique de Transmission nous est "connecté", ou pas.
			unsigned wait4Sync:1;	// Pour savoir s'il faut attendre le signal de Synchro avant d'envoyer la Trame
			unsigned canTxNow:1;	// Pour savoir si on peut envoyer la suite de la Trame
			unsigned lockTxIt:1;	// Pour empêcher l'envoi par Interruption d'une Trame en cours de Préparation
		};
	};
} tUartComManager;

/******************************************************************************/

tUartComManager mUartComManager[NB_OF_COM_INIT_PARAMS] = {0};
UART_COM_MAKE_CONST_END_OF_TABLE(tUartComManager, EndOfUartFrame, mUartComManager);

uint8_t UartComOverflowByte;
#define UART_COM_OVERFLOW_BYTE	&UartComOverflowByte

/******************************************************************************/

static tUartComManager* UartCom_GetManagerFromHandle(void* hHandle);
static uint16_t UartCom_DoTransmit(tUartComManager* pComManager, uint16_t nbBytes);
static void UartCom_ReSetRx(tUartComManager* pComManager, uint16_t reInitRxBuf);
void UartCom_HandleTxError(tUartComManager* pComManager);

#define UART_COM_FLAG_DISCARD_TX_NONE	0		// 0 = Normal
#define UART_COM_FLAG_DISCARD_TX_SYNC	(1<< 0)	// = 1
#define UART_COM_FLAG_DISCARD_TX_ECHO	(1<< 1)	// = 2
#define UART_COM_FLAG_DISCARD_TX_SAB	(1<< 2)	// = 4
uint16_t UartCom_GetMaxAllowedTxBlocSize(tUartComInitParams* pInitParam, uint16_t FrameSize, uint16_t Flags);

static void UartCom_ReleaseFrameInfo(tUartComManager* pComManager, tComFrameParams* pFI);
static void* UartCom_LockBuf4Size(tBufManager* pBM, tComFrameParams* pFI, uint16_t nbBytes);
static void* UartCom_LockBufIfEnough(tBusyFree* pBF, tBufParams* pBuf, void* pEnd, tComFrameParams* pFI, uint16_t nbBytes);
static void UartCom_UnlockFrameInfoAndBF(tComFrameParams* pFI, tBusyFree* pBF);
static tUartComRegularTx* UartCom_getRegTxFromParams(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam);
static tBusyFree* UartCom_getBusyFreeInBufManagers(void* pBuf, tBufManager* pBM);
static void UartCom_unLockBusyFree(tBusyFree* pBF);
static unsigned UartCom_isLockedBusyFree(tBusyFree* pBF);

static eUartComErrorContext UartComGetUartErrorContext(UART_HandleTypeDef *hUart); // Ne pas utiliser pour un autre type de Périphérique !

/******************************************************************************/

const __attribute__((unused)) tUartComClassFn UartCom_TxDMA_RxIT = { // Pour le Modbus sur Uart et chaque fois que possible
		"Uart_TxDMA_RxIT",
		(pUartCom_IoFn)HAL_UART_Transmit_DMA,
		(pUartCom_IoFn)HAL_UART_Receive_IT,
		(pUartCom_CntxtFn)UartComGetUartErrorContext,
		(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT,
		(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT,
		(pUartCom_CtrlFn)HAL_UART_DeInit,
};

const __attribute__((unused)) tUartComClassFn UartCom_TxDMA_RxIdleIT = { // Pour Test Rx to IDLE en IT :
// Remarque_Jp le 13/03/2025 : Avec cette Classe, il FAUT configurer le *_MAX_RX_BLOC_SZ à la taille du Buffer de Réception
		"Uart_TxDMA_RxIdleIT",
		(pUartCom_IoFn)HAL_UART_Transmit_DMA,
		(pUartCom_IoFn)HAL_UARTEx_ReceiveToIdle_IT,
		(pUartCom_CntxtFn)UartComGetUartErrorContext,
		(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT,
		(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT,
		(pUartCom_CtrlFn)HAL_UART_DeInit,
};

const __attribute__((unused)) tUartComClassFn UartCom_TxDMA_RxDMA = { // Pour Test Rx en DMA :
		"Uart_TxDMA_RxDMA",
		(pUartCom_IoFn)HAL_UART_Transmit_DMA,
		(pUartCom_IoFn)HAL_UART_Receive_DMA,
		(pUartCom_CntxtFn)UartComGetUartErrorContext,
		(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT,
		(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT,
		(pUartCom_CtrlFn)HAL_UART_DeInit,
};

const __attribute__((unused)) tUartComClassFn UartCom_TxDMA_RxIdleDMA = { // Pour Test Rx to IDLE en DMA :
// Remarque_Jp le 26/02/2025 : Avec cette Classe, il faudra pê configurer le *_MAX_RX_BLOC_SZ à la taille du Buffer de Réception ?
		"Uart_TxDMA_RxIdleDMA",
		(pUartCom_IoFn)HAL_UART_Transmit_DMA,
		(pUartCom_IoFn)HAL_UARTEx_ReceiveToIdle_DMA,
		(pUartCom_CntxtFn)UartComGetUartErrorContext,
		(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT,
		(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT,
		(pUartCom_CtrlFn)HAL_UART_DeInit,
};

const __attribute__((unused)) tUartComClassFn UartCom_TxIT_RxIT = { // Pour l'iBus sur Uart esentiellement
		"Uart_TxIT_RxIT",
		(pUartCom_IoFn)HAL_UART_Transmit_IT,
		(pUartCom_IoFn)HAL_UART_Receive_IT,
		(pUartCom_CntxtFn)UartComGetUartErrorContext,
		(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT,
		(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT,
		(pUartCom_CtrlFn)HAL_UART_DeInit,
};

/******************************************************************************/

#ifdef UART_COM_SUPPORT_REINIT // Si la reConfiguration UART est autorisée :

const UART_AdvFeatureInitTypeDef UART_AdvInitTxInvRxInv = { UART_ADVFEATURE_TXINVERT_INIT|UART_ADVFEATURE_RXINVERT_INIT,
			.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE,	.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE };

const UartReInitUserParams UartReInit9600N1   = { eUartReInitBaud9600,	 eUartReInitParityNone, eUartReInitStopOne };
const UartReInitUserParams UartReInit115200N1 = { eUartReInitBaud115200, eUartReInitParityNone, eUartReInitStopOne };	// 115200, Parity_None, 1_Stop
const UartReInitUserParams UartReInitDefaults = { eUartReInitBaud2Def,	 eUartReInitParity2Def, eUartReInitStop2Def };

#endif // UART_COM_SUPPORT_REINIT

/******************************************************************************/

#ifdef UART_COM_SUPPORT_TX_RX_PIN	// cf. "UartComConf.h"
	static void UartCom_Manage_PinDirection(tUartComInitParams* pInitParams, uint16_t nbPdgBytes2Tx);
#endif // UART_COM_SUPPORT_TX_RX_PIN

#define UART_COM_RESET_RX_INIT_BUF	(1 << 0)	// = 1
#define UART_COM_RESET_RX_KEEP_ECHO	(1 << 1)	// = 2

/******************************************************************************/

void UartCom_Devices_Init(void) // A appeler dans la partie Init Hardware
{
	tUartComInitParams* pComInitParams = (tUartComInitParams*)FIRST_COM_INIT_PARAM;
	tUartComManager* pComManager = mUartComManager;
	for( ; (pComInitParams < (tUartComInitParams*)AFTER_COM_INIT_PARAM)
		&& (pComManager < (tUartComManager*)EndOfUartFrame); pComInitParams++)
	{
		if(0 == pComInitParams->flag.loadMst) continue;	// Ignore si pas besoin de charger à la Mise sous Tension

		if(0 != UartCom_Register_InitParam(pComInitParams, pComManager))
		{
			pComManager++; // Pointe le prochain emplacement prêt à recevoir les infos
		}
	}

	// Regarder si on peut ignorer les éventuels InitParams restants :
#ifndef UART_COM_IGNORE_EXCEDENT_INIT_PARAMS
	while(pComInitParams < (tUartComInitParams*)AFTER_COM_INIT_PARAM)
	{
		if(0 != pComInitParams->flag.loadMst) // On devait charger celui-là aussi !
		{
			UART_COM_HALT_IF_DEBUG(); // Bloquer au RunTime en DEBUG si mauvaise Configuration => Recalculer NB_OF_COM_INIT_PARAMS !
			break;	// Tant pis : on passe quand même à la suite, avec ce qu'on aura pu charger ;-) ...
		}
		pComInitParams++; // On peut tout de même ignorer celui-ci
	}
#endif // UART_COM_IGNORE_EXCEDENT_INIT_PARAMS
}

/******************************************************************************/

void UartCom_RunTime_Init(void)	// A appeler dans la partie Init Software
{
	// Appelle la fonction d'Init du User :
#ifdef UART_COM_INIT_USER_MST	// cf. "UartComConf.h"
	UART_COM_INIT_USER_MST();
#endif // UART_COM_INIT_USER_MST
}

/******************************************************************************/

uint16_t UartCom_Register_InitParam(tUartComInitParams* pNewInitParam, void* pThisComManager) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	// Vérification de la Config Minimale d'Init :
	if(0 == pNewInitParam) return 0;	// Ajout_Jp le 29/09/2021 : Protection contre le NullPtr
	if(pNewInitParam->ProtocolType < UART_COM_TYPE_MIN) return 0;
	if(pNewInitParam->ProtocolType > UART_COM_TYPE_MAX) return 0;
	if(0 == pNewInitParam->hHandle) return 0;
	if(0 == pNewInitParam->pFnRxHandler) return 0;
	if(0 == pNewInitParam->RxInfos.pBufBase) return 0;

	tUartComManager* pComManager = (tUartComManager*)pThisComManager;
	if(0 == pComManager) // Si inconnu : rechercher un emplacement libre :
	{
		for(pComManager = mUartComManager; pComManager < (tUartComManager*)EndOfUartFrame; pComManager++)
		{
			if(0 == pComManager->pInitParams) break; // On a trouvé un emplacement dispo
		}
		if(pComManager >= (tUartComManager*)EndOfUartFrame) { pComManager = 0; }
	}
	if(0 == pComManager) return 0; // Failure : insufficient NB_OF_COM_INIT_PARAMS !
	if(0 != pComManager->pInitParams) return 0; // Failure : not free !

	// Tout est OK pour Initier la liaison :
	UartCom_FillMemory((void*)pComManager, 0, sizeof(tUartComManager)); // Nettoyer notre propre Structure avant de la Configurer
	if(0 != pNewInitParam->pFnInit) { pNewInitParam->pFnInit(); }		// Appele la Fonction d'Init

	// Vérification de Disponibilité :
#ifndef UART_COM_DISABLE_CHECK_READY_AFTER_INIT
	if(IS_UART_INSTANCE(pNewInitParam->hHandle))	// Uniquement si UART ou USART :
	{
		if(0 == (HAL_UART_GetState(pNewInitParam->hHandle) & HAL_UART_STATE_READY)) return 0; // Failure : le Device n'est pas Ready !
	}
#endif // UART_COM_DISABLE_CHECK_READY_AFTER_INIT

	// Tout est OK pour Activer la liaison :
	pComManager->pInitParams = (tUartComInitParams*)pNewInitParam;	// Sauvegarde le lien vers les Infos d'Init
	pComManager->sabReady4Tx = pNewInitParam->sabInitNoTx;			// Applique la Tempo Initiale avant la 1ère Emission
	pComManager->isLoaded = 1;
	return 1;
}

/******************************************************************************/

uint16_t UartCom_UnRegister_Handle(void* hThisHandle) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	tUartComInitParams* 	pInitParam = 0;
	tUartComInitRegularTx*	pInitRegTx = 0;
	tComFrameParams* 		pFI = 0;
	tBufManager*			pBM = 0;
	tBusyFree* 				pBF = 0;
	tUartComClassFn*		pClassFn = 0;
	pUartCom_CtrlFn			pCtrlFn = 0;

	for(tUartComManager* pComManager = mUartComManager; pComManager < (tUartComManager*)EndOfUartFrame; pComManager++)
	{
		if(0 == pComManager->pInitParams) continue;
		pInitParam = pComManager->pInitParams;
		if(pInitParam->hHandle != hThisHandle) continue;
		// On vient de trouver notre élément :

		// Déconnecter les éventuels RegularTx sur cet Uart :
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
		for(tUartComRegularTx* pRegTx = shrdRegTx; pRegTx < EndOfShrdRegTx; pRegTx++)
		{
			if(0 == pRegTx->pInitParams) continue;
			pInitRegTx = pRegTx->pInitParams;
			if(pInitRegTx->hHandle != hThisHandle) continue; // Filtre sur l'Uart recherché
			// On a trouvé un RegularTx associé à cet UART :
			pRegTx->pInitParams = 0;
		}
#endif // UART_COM_MAX_REG_TX

		// Vider les Trames en attente, parmi les BufInfos privés :
		for(int i = 0; i < NB_FRAME_PARAMS_PER_COM_MANAGER; i++)
		{
			pFI = &(pComManager->bufInfo[i]);
			if(0 == pFI->pBufBase) continue;
			if(pFI->hHandle != hThisHandle) continue;

			// On a un Buffer privé à libérer :
			pBM = &(pComManager->bufMgr); // Propose de chercher d'abord dans notre propre BufManager
			pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, pBM); // Au pire, il y a en plus une chance avec le BufManager partagé

			// Libère le FrameInfo & le Bloc dans le BufferManager :
			UartCom_UnlockFrameInfoAndBF(pFI, pBF);
		}

		// Vider les Trames en attente, parmi les BufInfos partagés :
#if defined(NB_SHARED_UART_COM_FRAME_PARAMS) && (NB_SHARED_UART_COM_FRAME_PARAMS > 0)	// cf. "UartComConf.h"
		for(int i = 0; i < NB_SHARED_UART_COM_FRAME_PARAMS; i++)
		{
			pFI = &shrdBufInfo[i];
			if(0 == pFI->pBufBase) continue;
			if(pFI->hHandle != hThisHandle) continue;

			// On a un Buffer partagé à libérer :
			pBM = &(pComManager->bufMgr); // Propose de chercher d'abord dans notre propre BufManager
			pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, pBM); // Au pire, il y a en plus une chance avec le BufManager partagé

			// Libère le FrameInfo & le Bloc dans le BufferManager :
			UartCom_UnlockFrameInfoAndBF(pFI, pBF);
		}
#endif // NB_SHARED_UART_COM_FRAME_PARAMS

		// Prépare la Déconnexion Rx, Tx & DeInit :
		pClassFn = (tUartComClassFn*)pInitParam->pClassFn;

		// Déconnexion de la Réception :
		if(0 != pClassFn) { pCtrlFn = pClassFn->pFnAbortReceive; }
#ifdef UART_COM_USR_FN_ABORT_RECEIVE
		if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_ABORT_RECEIVE; } // Par défaut : HAL_UART_AbortReceive_IT
#endif // UART_COM_USR_FN_ABORT_RECEIVE
		if(0 != pCtrlFn)
		{
			pCtrlFn(pInitParam->hHandle); // Appel de la Fonction configurée
		}

		// Déconnexion de la Transmission :
		if(0 != pClassFn) { pCtrlFn = pClassFn->pFnAbortTransmit; }
#ifdef UART_COM_USR_FN_ABORT_TRANSMIT
		if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_ABORT_TRANSMIT; }
#endif // UART_COM_USR_FN_ABORT_TRANSMIT
		if(0 != pCtrlFn)
		{
			pCtrlFn(pInitParam->hHandle); // Appel de la Fonction configurée
		}

		// DéInitialisation du Périphérique matériel (partie de DeInit_UART_Com_Devices) :
		if(0 != pClassFn) { pCtrlFn = pClassFn->pFnDeInit; }
#ifdef UART_COM_USR_FN_DEINIT
		if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_DEINIT; }
#endif // UART_COM_USR_FN_DEINIT
		if(0 != pCtrlFn)
		{
			pCtrlFn(pInitParam->hHandle); // Appel de la Fonction configurée
		}

		// Finalise la libération :
		pComManager->isLoaded = 0;
		pComManager->pInitParams = 0;
		return 1; // Success :-) !
	}
	return 0; // Failure : Not found :-( !
}

/******************************************************************************/

void UartCom_Register_LastRxFramePtr(void* hHandle, uint32_t* pLastRxFrame)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return;	// Handle pas dans la Liste => Impossible de déterminer les Paramètres associés !

	// Mémorise le Pointeur fourni :
	pComManager->pLastRxFrame = pLastRxFrame;
	if(0 != pLastRxFrame) { *pLastRxFrame = UINT32_MAX; } 	// Initialise LastRxFrame au Max possible (équivalent de -1)
}

/******************************************************************************/

HAL_StatusTypeDef UartCom_ReInitUartFromUserParams(UartReInitUserParams* pUserParams)
{
#ifdef UART_COM_SUPPORT_REINIT	// cf. "UartComConf.h"
	UART_MAKE_FOR_VAR_FROM_TO(UartReInitItem*, pUartReInitItem, FIRST_COM_REINIT_ITEMS, AFTER_COM_REINIT_ITEMS)
	{
		if(pUartReInitItem->pUserParams != pUserParams) continue;

		// On a trouvé les UserParams dans notre liste connue :
		return UartCom_ReInitUartWithCustomParams(pUartReInitItem);
	}
#endif // UART_COM_SUPPORT_REINIT
	return HAL_ERROR;
}

/******************************************************************************/

/*HAL_StatusTypeDef UartCom_RegisterUart4ReInit(UartReInitUserParams* pUserParams)
{
#ifdef UART_COM_SUPPORT_REINIT

	//for(UartReInitItem* pUartReInitItem = (UartReInitItem*)mUartReInitItems; pUartReInitItem < (UartReInitItem*)EndOfUartReInitItems; pUartReInitItem++)
	UART_MAKE_FOR_VAR_FROM_TO(UartReInitItem*, pUartReInitItem, FIRST_COM_REINIT_ITEMS, AFTER_COM_REINIT_ITEMS)
	{
		if(pUserParams != pUartReInitItem->pUserParams) continue;
		if(0 == pUartReInitItem->pCoreVars) continue;

		pUartReInitItem->pCoreVars->ReInitFlags.InitDefault = 0; // Autoriser la Config User
		pUartReInitItem->pCoreVars->SabApply = COM_FRAME_TTL_EXPIRED; // Pour RéInit immédiat
		return HAL_OK;
	}
#endif // UART_COM_SUPPORT_REINIT
	return HAL_ERROR;
}*/

/******************************************************************************/

#define MAX_RxUartFlag	50
#define MAX_RxBufSize	7
typedef struct
{
	uint32_t Flags;
	uint32_t Tick;
	//uint8_t RxEventType;
	uint16_t Size;
	uint8_t Buf[MAX_RxBufSize];
} RxUartFlag;

uint16_t nxtRxUartFlagId = 0;
RxUartFlag RxUartFlags[MAX_RxUartFlag] = {0};

void Gestion_UartCom(void)
{
	tUartComManager* pComManager;
	tRxTxBufInfo mRxTxBI = {0};
	uint8_t mayReSetRx;
	uint8_t mayReleaseTx;
	tUartComInitParams* pInitParam;
	tComFrameParams* pFI;
	pUartCom_RxHandler pFnRxHandler;
	int ret;
	tUartComInitRegularTx* pInitRegTx;
	pUartCom_TxRegular pFnTxBuilder;
	uint16_t nbRxBytes;
	uint16_t reSetRxOptions;
	tBusyFree* pBF;

#ifdef UART_COM_SUPPORT_REINIT	// cf. "UartComConf.h"
	UartReInitCoreVars* pCoreVars;
	UART_MAKE_FOR_VAR_FROM_TO(UartReInitItem*, pUartReInitItem, FIRST_COM_REINIT_ITEMS, AFTER_COM_REINIT_ITEMS)
	{
		if(0 == pUartReInitItem->pCoreVars) continue;

		pCoreVars = pUartReInitItem->pCoreVars;
		if(COM_FRAME_TTL_EXPIRED != pCoreVars->SabApply) continue;

		// It's time to ReInit with new Custom Params :
		if(HAL_OK == UartCom_ReInitUartWithCustomParams(pUartReInitItem)) // OK, done :
		{
			pCoreVars->ReInitFlags.CanValidate = 1; // OK pour Validation & Sauvegarde
			pCoreVars->SabApply = COM_FRAME_TTL_DISABLED; // OK, effectué : on part du principe que c'est aussi OK pour le Modbus Master
//			if(0 != pCoreVars->ReInitFlags.InitDefault) // From DefaultParams :
//			{
//				pCoreVars->SabApply = COM_FRAME_TTL_DISABLED; // OK, effectué
//			} else { // From UserParams => Validation nécessaire :
//				pCoreVars->SabApply = UART_COM_SAB_VALID_REINIT; // Tempo pour Valider
//			}
		} else { // Failure :
			pCoreVars->ReInitFlags.CanValidate = 0; // Impossible de Valider !
			pCoreVars->SabApply = UART_COM_SAB_RETRY_REINIT; // Tempo pour Retry
			// ToDo: Vérifier si gestion uart peut gérer pb uart pas init !
		}
		pCoreVars->ReInitFlags.InitDefault = 1;	// Prochaine tentative avec Default Params
	}
#endif // UART_COM_SUPPORT_REINIT

	for(pComManager = mUartComManager; pComManager < EndOfUartFrame; pComManager++)
	{
		pInitParam = pComManager->pInitParams;
		if(0 == pInitParam) continue;
		if(0 == pInitParam->hHandle) continue;

		// Tout d'abord : (Notifier puis) Libérer d'un Envoi Terminé (équivalent ReleaseCurFrameInfo) :
		if( (0 == pComManager->curTxBufInfo.nbBytes) && (0 != pComManager->curTxBufInfo.pBufBase) )
		{
			UartCom_ReleaseFrameInfo(pComManager, 0); // Libérer le FrameInfo courant, s'il existe toujours

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbTxMainRelease) { pComManager->nbTxMainRelease++; }
#endif // UART_COM_SUPPORT_STATS
		}

		// CleanUp sur rebouclage de FrameInfo :
		if( (0 != pComManager->pCurFrameInfo) && (pComManager->pCurFrameInfo == pComManager->pNxtFrameInfo) )
		{
			UartCom_ReleaseFrameInfo(pComManager, 0); // Libérer le FrameInfo courant, s'il existe toujours

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbTxReLoopRelease) { pComManager->nbTxReLoopRelease++; }
#endif // UART_COM_SUPPORT_STATS
		}

		// Traiter la Synchro Réception :
		if(0 == pComManager->sabEndOfRxFrame) // Si TimeOut EndOfFrame => Passer au Traitement :
		{
/*
			if(0 != pInitParam->hHandle)
			{
				UART_MAKE_VAR_AND_CAST_VALUE(UART_HandleTypeDef*, huart, pInitParam->hHandle);

				HAL_UART_GetState(hUart);
				__HAL_UART_GET_FLAG(hUart, UART_FLAG_IDLE)
				//UART_CheckIdleState
				UART_FLAG_IDLE

//				tUartComInitParams* pInitParam = pComManager->pInitParams;
//				if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !
				tUartComClassFn* pClassFn = (tUartComClassFn*)pInitParam->pClassFn;

				eUartComErrorContext errContext = ErrorContextUnknown;

				#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
					if(UINT32_MAX > pComManager->nbErrCallBack) { pComManager->nbErrCallBack++; }
				#endif // UART_COM_SUPPORT_STATS

					if(0 != pClassFn) { pFnErrContext = pClassFn->pFnErrContext; }
				#ifdef UART_COM_USR_FN_GET_ERR_CNTXT
					if(0 == pFnErrContext) { pFnErrContext = UART_COM_USR_FN_GET_ERR_CNTXT; }
				#endif // UART_COM_USR_FN_GET_ERR_CNTXT
					if(0 != pFnErrContext)
					{
						errContext = pFnErrContext(hUart); // Appel de la Fonction correspondante
					}

			}
*/

	        // Gestion prioritaire du Buffer invalide ou Inactivité sur l'UART :
			mayReSetRx = ( (0 != pComManager->mayDiscardRx) || (0 == pComManager->sabReSetRxBufPtr) ) ? 1 : 0;
			reSetRxOptions = UART_COM_RESET_RX_INIT_BUF; // par défaut : RéInit au moins le Buffer

			nbRxBytes = pComManager->curRxBufInfo.nbBytes; // Attention : un Byte a pu tomber depuis que sabEndOfRxFrame a été testé à 0 !
	        if( (0 == mayReSetRx) && (nbRxBytes > 0) && (0 == pComManager->sabEndOfRxFrame) ) // S'il y a eu des Bytes reçus et toujours Valide :
	        {

// ToDo: #if support DMA rx delays
//				if(IS_UART_INSTANCE(pInitParam->hHandle))	// Uniquement si UART ou USART :
				{
					UART_MAKE_VAR_AND_CAST_VALUE(UART_HandleTypeDef*, hUart, pInitParam->hHandle);
					// https://deepbluembedded.com/stm32-usart-uart-tutorial/
					// https://github.com/MaJerle/stm32-usart-uart-dma-rx-tx
/*
					// dans HAL_UART_Receive_IT :
					huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
					return (UART_Start_Receive_IT(huart, pData, Size));

					// Et dans UART_Start_Receive_IT :
					  huart->ErrorCode = HAL_UART_ERROR_NONE;
					  huart->RxState = HAL_UART_STATE_BUSY_RX;
					  huart->RxISR = UART_RxISR_8BIT;

					// dans HAL_UARTEx_ReceiveToIdle_IT :
				    huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
				    huart->RxEventType = HAL_UART_RXEVENT_TC;
				    (void)UART_Start_Receive_IT(huart, pData, Size);

					// Et dans UART_Start_Receive_IT :
					  huart->ErrorCode = HAL_UART_ERROR_NONE;
					  huart->RxState = HAL_UART_STATE_BUSY_RX;
					  huart->RxISR = UART_RxISR_8BIT;

				 // dans HAL_UARTEx_ReceiveToIdle_DMA :
					huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE;
					huart->RxEventType = HAL_UART_RXEVENT_TC;
					status =  UART_Start_Receive_DMA(huart, pData, Size);

				 // Et dans UART_Start_Receive_DMA :
					huart->ErrorCode = HAL_UART_ERROR_NONE;
					huart->RxState = HAL_UART_STATE_BUSY_RX;
					huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;
					huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;
					status = HAL_DMAEx_List_Start_IT(huart->hdmarx);
					OU
					status = HAL_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->RDR, (uint32_t)huart->pRxBuffPtr, nbByte);

				// Dans UART_RxISR_8BIT, si huart->RxXferCount == 0U :
					huart->RxState = HAL_UART_STATE_READY;
					huart->RxEventType = HAL_UART_RXEVENT_TC;
					HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);
					OU
					HAL_UART_RxCpltCallback(huart);

				// Dans UART_DMARxHalfCplt :
					huart->RxEventType = HAL_UART_RXEVENT_HT;
					HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize / 2U);
					OU
					HAL_UART_RxHalfCpltCallback(huart);

				// Dans UART_DMAReceiveCplt :
					huart->RxState = HAL_UART_STATE_READY; // si hdma->Mode != DMA_LINKEDLIST_CIRCULAR
					huart->RxEventType = HAL_UART_RXEVENT_TC;
					HAL_UARTEx_RxEventCallback(huart, huart->RxXferSize);
					OU
					HAL_UART_RxCpltCallback(huart);
*/

					if(nxtRxUartFlagId >= MAX_RxUartFlag) nxtRxUartFlagId = 0;
					{
						RxUartFlags[nxtRxUartFlagId].Tick = (uint16_t)HAL_GetTick();
						RxUartFlags[nxtRxUartFlagId].Flags = hUart->Instance->ISR;
						RxUartFlags[nxtRxUartFlagId].Size = nbRxBytes;
						//if(Size > 0)
						{ UartCom_CopyMemory(RxUartFlags[nxtRxUartFlagId].Buf, pComManager->curRxBufInfo.pBufBase, MAX_RxBufSize); } // Copie la Réception dans notre Emplacement prévu
						nxtRxUartFlagId++;
					}

/*
	        		if(0 == (__HAL_UART_GET_FLAG(hUart, UART_FLAG_IDLE))) // Si pas IDLE => Réception en cours ?
	        		{
	        			nbRxBytes = 0; // Retarde le traitement ...
	        		    pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame; 	// Recharge le Sablier de Fin de Trame
	        		    pComManager->sabTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Recharge le Délai pour Répondre
        		    	pComManager->sabReady4Tx = pInitParam->sabReady4TxFrame;		// Recharge le Sablier pour Nouvelle Trame
	        		}
*/
	        	}

//	        	if(nbRxBytes > 0)
	        	{
				mayReSetRx = 1; // Il faudra aussi ré-Initialiser complètement la Réception après le Traitement !

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	        	if(UINT32_MAX > pComManager->nbFramesRx) { pComManager->nbFramesRx++; }
#endif // UART_COM_SUPPORT_STATS

	        	// Initialise la Struture pour le Décodage, puis la Réponse :
	        	if(nbRxBytes >= pInitParam->minRxFrameSize)
	        	{
	        		if(0 != pComManager->pLastRxFrame) { *(pComManager->pLastRxFrame) = 0; } // S'il faut renseigner qu'on vient de recevoir une Trame
	        		pFnRxHandler = pInitParam->pFnRxHandler; // Récupère le Pointeur de Fonction pour le Décodage
	        		if(0 != pFnRxHandler)
	        		{
	        			mRxTxBI.hHandle = pInitParam->hHandle;
						pFI = UartCom_InitNewTxFrame(mRxTxBI.hHandle, pInitParam->defTxReplySize);
						if(0 != pFI)
						{
							mRxTxBI.RxBuf.pBufBase = pComManager->curRxBufInfo.pBufBase;
							mRxTxBI.RxBuf.nbBytes = nbRxBytes;

							mRxTxBI.TxBuf.pBufBase = pFI->pBufBase;
							mRxTxBI.TxBuf.maxBytes = pFI->nbBytes;
							mRxTxBI.TxBuf.nbBytes = 0;

							//mRxTxBI.pVoidParam = pCurInitParam->pVoidParam;
							mayReleaseTx = 1; // Par défaut, il faudra libérer tout de suite le TxFrameBuffer

							ret = pFnRxHandler(&mRxTxBI, pInitParam->pVoidParam);	// Appel du Handler du Protocole associé
							if(pComManager->sabTimeOut4Reply > 0) // s'il est encore temps de transmettre une Réponse immédiate
							{
								if( (0 != ret) && (mRxTxBI.TxBuf.nbBytes > 0) ) // Il y a une Réponse à transmettre :
								{
									// => Renseigner (sans trop de précaution) la structure de travail bas niveau :
									pComManager->lockTxIt = 1;	// Empêcher pour le moment l'envoi immédiat par Interruption
									pComManager->curTxBufInfo.nbBytes = mRxTxBI.TxBuf.nbBytes;
									pComManager->curTxBufInfo.pBufBase = mRxTxBI.TxBuf.pBufBase;
									pComManager->pCurFrameInfo = pFI;

#if defined(UART_COM_ENABLE_IBUS) && !defined(UART_COM_DISABLE_SYNC_TX)	// cf. "UartComConf.h"
									pComManager->wait4Sync = pInitParam->flag.replySync; // Option Spéciale Synchro pour répondre
									if(0 != pComManager->wait4Sync) // S'il faut attendre une Synchro ultérieure :
									{
										mayReleaseTx = 0; // Ne pas libérer tout de suite !
									} else
#endif // UART_COM_ENABLE_IBUS && !UART_COM_DISABLE_SYNC_TX
									{
										uint16_t canPostNow = UartCom_GetMaxAllowedTxBlocSize(pInitParam, pComManager->curTxBufInfo.nbBytes, UART_COM_FLAG_DISCARD_TX_SYNC); // 1 = Discard SyncTx normal
										if(0 != UartCom_DoTransmit(pComManager, canPostNow))
										{
											if( (canPostNow != mRxTxBI.TxBuf.nbBytes) || (0 != pComManager->curTxBufInfo.nbBytes) ) // Si on n'a pas tout envoyé, ou s'il reste encore à envoyer :
											{
												mayReleaseTx = 0; // La Transmission est initiée => Ne libérer qu'une fois Terminée
											}
										}
									}
									pComManager->lockTxIt = 0;	// Autorise maintenant les envois complémentaires par Interruption
								}
							}
							if(0 != mayReleaseTx)	// Si on peut libérer tout de suite :
							{
								UartCom_ReleaseFrameInfo(pComManager, pFI);	// Libérer le FrameInfo local
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
								if(UINT32_MAX > pComManager->nbTxQuickRelease) { pComManager->nbTxQuickRelease++; }
#endif // UART_COM_SUPPORT_STATS
							} else { // S'il faut Préserver les infos pour envoi ultérieur :
								if(0 != pInitParam->flag.chkEcho) { reSetRxOptions |= UART_COM_RESET_RX_KEEP_ECHO; }
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
								if(UINT32_MAX > pComManager->nbReplyPdg) { pComManager->nbReplyPdg++; }
#endif // UART_COM_SUPPORT_STATS
							}
						}
	        		}
	        	}
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	        	else {
	        		pComManager->lastRxSmall = nbRxBytes;
	        		if(UINT32_MAX > pComManager->nbRxSmall) { pComManager->nbRxSmall++; }

	        		// Capture la Répartition par Taille :
	        		if(nbRxBytes < NB_COUNT_RX_SMALL)
	        		{
	        			if(UINT32_MAX > pComManager->nbErrRxSmlSz[nbRxBytes]) { pComManager->nbErrRxSmlSz[nbRxBytes]++; }
	        		}

				// Capture la Répartition par 1er Byte reçu :
	        		uint8_t* pRx = pComManager->curRxBufInfo.pBufBase;
	        		if( (pRx[0] != 0xFF) && (pRx[1] != 135) )
	        		{
						pComManager->lastErrRxSml[0] = pRx[0];
						pComManager->lastErrRxSml[1] = pRx[1];
	        		}
	        		switch (*pRx) {
						case 0x3F:
							if(UINT32_MAX > pComManager->nbErrRxSml3F) { pComManager->nbErrRxSml3F++; }
							break;
						case 0xFF: // Probablement un ACK
							if(UINT32_MAX > pComManager->nbErrRxSmlFF) { pComManager->nbErrRxSmlFF++; }
							break;
						default:
							if(UINT32_MAX > pComManager->nbErrRxSmlXX) { pComManager->nbErrRxSmlXX++; }
							break;
					}
	        	}
#endif // UART_COM_SUPPORT_STATS

	        	}
	        }

	        // Post-Traitement :
			if(0 != mayReSetRx)
			{
				UartCom_ReSetRx(pComManager, reSetRxOptions); // (Re)lancer la Réception sur l'UART
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
				if(UINT32_MAX > pComManager->nbReSetRx) { pComManager->nbReSetRx++; }
#endif // UART_COM_SUPPORT_STATS
			}
		}

		// Si on peut, Vérifie s'il y a des envois en attente :
		if(0 == pComManager->sabReady4Tx) // Sablier favorable :
		{
			if(0 == pComManager->pCurFrameInfo) // Rien en cours :
			{
				pFI = pComManager->pNxtFrameInfo; // Tente le FrameInfo déjà pré-suggéré ...

#if defined(NB_FRAME_PARAMS_PER_COM_MANAGER) && (NB_FRAME_PARAMS_PER_COM_MANAGER > 0)	// cf. "UartComConf.h"
				if(0 == pFI) // On n'a pas trouvé un besoin déjà enregistré ...
				{
					// Recherche parmi les BufInfos privés :
					for(int i = 0; i < NB_FRAME_PARAMS_PER_COM_MANAGER; i++)
					{
						if(0 == pComManager->bufInfo[i].pBufBase) continue;
						if(pInitParam->hHandle != pComManager->bufInfo[i].hHandle) continue;

						// On a un Buffer privé à envoyer :
						pFI = &(pComManager->bufInfo[i]);
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
						if(UINT32_MAX > pComManager->nbTxPrivBuf) { pComManager->nbTxPrivBuf++; }
#endif // UART_COM_SUPPORT_STATS
						break;
					}
				}
#endif // NB_FRAME_PARAMS_PER_COM_MANAGER

				// Partie 2 : Recherche parmi les BufInfos partagés :
#if defined(NB_SHARED_UART_COM_FRAME_PARAMS) && (NB_SHARED_UART_COM_FRAME_PARAMS > 0)	// cf. "UartComConf.h"
				if(0 == pFI)
				{
					for(int i = 0; i < NB_SHARED_UART_COM_FRAME_PARAMS; i++)
					{
						if(0 == shrdBufInfo[i].pBufBase) continue;
						if(pInitParam->hHandle != shrdBufInfo[i].hHandle) continue;

						// On a un Buffer partagé à envoyer :
						pFI = &shrdBufInfo[i];
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
						if(UINT32_MAX > pComManager->nbTxShrdBuf) { pComManager->nbTxShrdBuf++; }
#endif // UART_COM_SUPPORT_STATS
						break;
					}
				}
#endif // NB_SHARED_UART_COM_FRAME_PARAMS

				// Partie 3 : Transférer les Infos si disponibles :
				if(0 != pFI)
				{
					pComManager->pCurFrameInfo = pFI;
					if(pFI == pComManager->pNxtFrameInfo) { pComManager->pNxtFrameInfo = 0; } // Efface si bien pris en charge dans le CurFrameInfo !

					if(COM_FRAME_TTL_EXPIRED != pFI->time2Live)
					{
						pComManager->lockTxIt = 1;	// Empêcher pour le moment l'envoi immédiat par Interruption
						pComManager->curTxBufInfo.nbBytes = pFI->nbBytes;
						pComManager->curTxBufInfo.pBufBase = pFI->pBufBase;
						pComManager->lockTxIt = 0;	// Autorise maintenant l'envoi par l'Interruption

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbTxBufLoaded) { pComManager->nbTxBufLoaded++; }
#endif // UART_COM_SUPPORT_STATS
					} else {	// La Trame a expiré => Détruire tout de suite :
						UartCom_ReleaseFrameInfo(pComManager, 0); // Libérer le FrameInfo courant, s'il existe toujours

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbTxBufDiscarded) { pComManager->nbTxBufDiscarded++; }
#endif // UART_COM_SUPPORT_STATS
					}
				}
			}
		}
	}

	// Vérifie s'il y a des RegularTx à organiser :
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	for(tUartComRegularTx* pRegTx = shrdRegTx; pRegTx < EndOfShrdRegTx; pRegTx++)
	{
		if(pRegTx->endDelay <= 0) continue; // Pas de Fin = désactivé
		if(pRegTx->curDelay < pRegTx->endDelay)	continue; // Pas au terme => Ignorer

		// Cet élément est arrivé à terme :
		pInitRegTx = pRegTx->pInitParams;
		if(0 == pInitRegTx) continue;
		if( (0 == pInitRegTx->pFn) || (0 == pInitRegTx->hHandle) )
		{
			pRegTx->endDelay = 0; // Désactive ce RégularTx !
			continue;	// Poursuivre à la recherche de Paramètres fonctionnels ...
		}

		// On a tous les éléments pour Organiser le Tx :
		pRegTx->curDelay = 0; // Repart tout de suite de 0

#ifdef UART_COM_SUPPORT_VAR_DELAY
		pRegTx->endDelay = pRegTx->regDelay;	// Pour Modification au RunTime d'un Intervalle de temps Variable
#else // ! UART_COM_SUPPORT_VAR_DELAY :
		pRegTx->endDelay = pInitRegTx->NormalDelay; // Recharge le Délai officiel
#endif // UART_COM_SUPPORT_VAR_DELAY

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbRegTxLoaded) { pComManager->nbRegTxLoaded++; }
#endif // UART_COM_SUPPORT_STATS

		// Prépare la Récupération de la Trame à envoyer :
		pFI = UartCom_InitNewTxFrame(pInitRegTx->hHandle, pInitRegTx->DefFrameSize);
		if(0 == pFI)
		{
#define UART_COM_RETRY_REG_TX	10	// @ RT_100ms => 1s
			pRegTx->endDelay = UART_COM_RETRY_REG_TX; // Planifie un nouvel essai ultérieur
			continue; // On ne peut pas pour le moment ...
		}

		mayReleaseTx = 1; // Par défaut, il faudra libérer tout de suite le TxFrameBuffer
		pFnTxBuilder = pInitRegTx->pFn;

		if(pFnTxBuilder(pFI, pInitRegTx->pVoidParam) > 0) // La fonction doit retourner 1 ou plus pour Activer l'envoi !
		{
			if(0 != UartCom_PostFrame(pFI)) { mayReleaseTx = 0; } // Si l'opération est acceptée : ne pas libérer maintenant ...
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbRegTxSent) { pComManager->nbRegTxSent++; }
#endif // UART_COM_SUPPORT_STATS
		}

		if(0 != mayReleaseTx) // S'il faut déjà libérer les Variables :
		{
			// S'il n'y a finalement rien à envoyer cette fois-ci (une partie de UART_Com_RequestLargerBuffer) :
			pComManager = UartCom_GetManagerFromHandle(pInitRegTx->hHandle);
			tBufManager* pBM = 0;
			if(0 != pComManager) { pBM = &(pComManager->bufMgr); } // Propose de chercher d'abord dans notre propre BufManager
			pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, pBM); // Au pire, il y a en plus une chance avec le BufManager partagé

			// Libère le FrameInfo & le Bloc dans le BufferManager :
			UartCom_UnlockFrameInfoAndBF(pFI, pBF);
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbTxRegTxRelease) { pComManager->nbTxRegTxRelease++; }
#endif // UART_COM_SUPPORT_STATS
		}
	}
#endif // UART_COM_MAX_REG_TX
}

/******************************************************************************/

void UartCom_Handle_IT_1ms(void) // Appeler dans l'Interruption @ 1ms
{
#define DEC_NOT_EMPTY_VAR(v)	if((v) > 0) (v)--
#define DEC_NOT_ELAPSED_VAR(v)	if((v) > COM_FRAME_TTL_EXPIRED) (v)--
#define INC_OR_LOOP_VAR_IN_RANGE(var,min,max)	if(++(var) >= (max)) (var)=(min)

//	if(UartComRedivIT_100ms >= 100) { UartComRedivIT_100ms = 0; }
	INC_OR_LOOP_VAR_IN_RANGE(UartComRedivIT_100ms, 0, 100);

	for(tUartComManager* pComManager = mUartComManager; pComManager < EndOfUartFrame; pComManager++)
	{
		// Sablier pour Basculer en mode Traitement (Synchro Temporelle) :
		DEC_NOT_EMPTY_VAR(pComManager->sabEndOfRxFrame);	// Sablier du Temps restant avant Fin de Trame (= synchro temporelle)
		DEC_NOT_EMPTY_VAR(pComManager->sabTimeOut4Reply);	// Sablier du Temps restant pour Envoyer une Réponse immédiatement
		DEC_NOT_EMPTY_VAR(pComManager->sabReady4Tx);		// Sablier du Temps restant avant de pouvoir envoyer une Trame
		DEC_NOT_EMPTY_VAR(pComManager->sabMayTxNextByte);	// Sablier du Temps restant avant de pouvoir envoyer le prochain Byte

		if(0 == UartComRedivIT_100ms) // Si ça fait déjà 100ms :
		{
			DEC_NOT_EMPTY_VAR(pComManager->sabReSetRxBufPtr);	// Sablier du Temps restant avant de (re)configurer la Réception

			// S'il faut incrémenter le Temps qui vient de passer depuis la dernière Trame reçue :
			if(0 != pComManager->pLastRxFrame)
    		{
    			if(UINT32_MAX > *(pComManager->pLastRxFrame))
    			{
    				(*(pComManager->pLastRxFrame))++;
    			}
    		}

			// S'il y a des Trames privées avec une durée de Vie spéciale :
#ifdef UART_COM_SUPPORT_FRAME_TTL	// cf. "UartComConf.h"
			for(int i = 0; i < NB_FRAME_PARAMS_PER_COM_MANAGER; i++)
			{
				DEC_NOT_ELAPSED_VAR(pComManager->bufInfo[i].time2Live);
			}
#endif // UART_COM_SUPPORT_FRAME_TTL
		}

		// Sablier Spécial iBus :
#ifdef UART_COM_ENABLE_IBUS	// cf. "UartComConf.h"
		if(0 == (++pComManager->sabPrediv8ms & 0x07)) { pComManager->iBusSync++; }	// Pour la Synchro iBus
#endif // UART_COM_ENABLE_IBUS

		// Traitement des Envois spécifiques :
		if( (pComManager->curTxBufInfo.nbBytes > 0) && (0 != pComManager->curTxBufInfo.pBufBase) ) // S'il y a des envois "bas niveau" restants :
		{
			if( (0 == pComManager->sabMayTxNextByte) && (0 == pComManager->lockTxIt) ) // Si le Sablier entre les Bytes le permet ET autorisé par le Programme Principal :
			{
				if( (0 == pComManager->sabReady4Tx) || (0 != pComManager->canTxNow) )	// Si le Bus est libre pour une Emission Tx
				{
#if defined(UART_COM_ENABLE_IBUS) && !defined(UART_COM_DISABLE_SYNC_TX)	// cf. "UartComConf.h"
					if( (0 == pComManager->wait4Sync) || (*(uint8_t*)(pComManager->curTxBufInfo.pBufBase) == pComManager->iBusSync) ) // Si la Synchro Tx est acceptée
#endif // UART_COM_ENABLE_IBUS && !UART_COM_DISABLE_SYNC_TX
					{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
						if(2 == pComManager->curTxBufInfo.nbBytes) { pComManager->nbTxAck2++; }
#endif // UART_COM_SUPPORT_STATS

						uint16_t canPostNow = UartCom_GetMaxAllowedTxBlocSize(pComManager->pInitParams, pComManager->curTxBufInfo.nbBytes, UART_COM_FLAG_DISCARD_TX_SYNC); // 1 = Discard Sync
						if(0 != UartCom_DoTransmit(pComManager, canPostNow))
						{
							pComManager->canTxNow = 1;	// Enable bypass sabReady4Tx (nécessaire si echo attendu)
#if defined(UART_COM_ENABLE_IBUS) && !defined(UART_COM_DISABLE_SYNC_TX)	// cf. "UartComConf.h"
							pComManager->wait4Sync = 0;	// Enable bypass iBusSync (nécessaire si Synchro iBus)
#endif // UART_COM_ENABLE_IBUS && !UART_COM_DISABLE_SYNC_TX

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
							if(UINT32_MAX > pComManager->nbTxBytesSentIT) { pComManager->nbTxBytesSentIT++; }
#endif // UART_COM_SUPPORT_STATS
						} else {
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
							if(UINT32_MAX > pComManager->nbTxBytesNotSentIT) { pComManager->nbTxBytesNotSentIT++; }
#endif // UART_COM_SUPPORT_STATS
						}
#ifdef UART_COM_ENABLE_IBUS	// cf. "UartComConf.h"
					} else {
  #ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
						if(UINT32_MAX > pComManager->nbTxBytesDlyd4Sync) { pComManager->nbTxBytesDlyd4Sync++; }
  #endif // UART_COM_SUPPORT_STATS
#endif // UART_COM_ENABLE_IBUS
					}
				} else {
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
					if(UINT32_MAX > pComManager->nbTxBytesDlyd4RdyTx) { pComManager->nbTxBytesDlyd4RdyTx++; }
#endif // UART_COM_SUPPORT_STATS
				}
			} else {
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
				if(UINT32_MAX > pComManager->nbTxBytesDlyd4SabTx) { pComManager->nbTxBytesDlyd4SabTx++; }
#endif // UART_COM_SUPPORT_STATS
			}
		}
	}

	// Terminer par effectuer les tâches très rapides @ 100ms :
	if(0 == UartComRedivIT_100ms)
	{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
		for(tUartComRegularTx* pRegTx = shrdRegTx; pRegTx < EndOfShrdRegTx; pRegTx++)
		{
			if(0 == pRegTx->pInitParams) continue;
			// Cette Structure a été associée à des Paramètres d'Init :
			if(pRegTx->curDelay < UINT16_MAX) { pRegTx->curDelay++; }
		}
#endif // UART_COM_MAX_REG_TX

		// S'il y a des Trames partagées avec une durée de Vie spéciale :
#ifdef UART_COM_SUPPORT_FRAME_TTL	// cf. "UartComConf.h"
	#if defined(NB_SHARED_UART_COM_FRAME_PARAMS) && (NB_SHARED_UART_COM_FRAME_PARAMS > 0)	// cf. "UartComConf.h"
		for(int i = 0; i < NB_SHARED_UART_COM_FRAME_PARAMS; i++)
		{
			DEC_NOT_ELAPSED_VAR(shrdBufInfo[i].time2Live);
		}
	#endif // NB_SHARED_UART_COM_FRAME_PARAMS
#endif // UART_COM_SUPPORT_FRAME_TTL

		// S'il y a des UARTs à Ré-Initialiser :
#ifdef UART_COM_SUPPORT_REINIT	// cf. "UartComConf.h"
		UartReInitCoreVars* pCoreVars;
		UART_MAKE_FOR_VAR_FROM_TO(UartReInitItem*, pUartReInitItem, FIRST_COM_REINIT_ITEMS, AFTER_COM_REINIT_ITEMS)
		{
			if(0 == pUartReInitItem->pCoreVars) continue;
			pCoreVars = pUartReInitItem->pCoreVars;
			DEC_NOT_ELAPSED_VAR(pCoreVars->SabApply);
		}
#endif // UART_COM_SUPPORT_REINIT

	}
}

/******************************************************************************/

uint16_t UartCom_SetRegTx_CurEndDelay(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam, int16_t newEndDelay, int16_t newCurDelay)
{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	tUartComRegularTx* pRegTx = UartCom_getRegTxFromParams(hHandle, pFn, pVoidParam);
	if(0 == pRegTx) return 0; // Not Found !

	// On a notre élément :
	if(UART_COM_SET_DEF_REG_TX_DELAY >= newEndDelay) // Cas particuliers :
	{
		if(UART_COM_SET_DEF_REG_TX_DELAY == newEndDelay) { newEndDelay = (int16_t)pRegTx->pInitParams->NormalDelay; }
		if(UART_COM_SET_FIRST_REG_TX_DELAY == newEndDelay) { newEndDelay = (int16_t)pRegTx->pInitParams->FirstDelay; }
	}
	if(UART_COM_SET_DEF_REG_TX_DELAY >= newCurDelay) { newCurDelay = 0; }
	if(0 <= newEndDelay) { pRegTx->endDelay = (uint16_t)newEndDelay; }
	if(0 <= newCurDelay) { pRegTx->curDelay = (uint16_t)newCurDelay; }

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT16_MAX > pRegTx->nbChangeDelay) { pRegTx->nbChangeDelay++; }
#endif // UART_COM_SUPPORT_STATS

	return 1; // EndDelay updated
#else
	return 0; // Failure
#endif // UART_COM_MAX_REG_TX
}

/******************************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUart) // Handler partagé par tous les UARTs & USARTs (uniquement)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hUart);
	if(0 == pComManager)	// Handle pas dans la Liste => Impossible de déterminer le Manager associé :
	{
#ifdef NEXT_HAL_UART_RxCpltCallback	// cf. "UartComConf.h"
		NEXT_HAL_UART_RxCpltCallback(hUart);	// ATTENTION : Fonction Non Testée !
#endif // NEXT_HAL_UART_RxCpltCallback
		return;
	}
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbBytesRx) { pComManager->nbBytesRx++; }
#endif // UART_COM_SUPPORT_STATS

#ifndef UART_COM_DISABLE_CHECK_ECHO
	uint8_t chkEchoFail = 0; // Par défaut : pas d'erreur sur l'echo

	if( (0 != pInitParam->flag.chkEcho) && (0 != pComManager->pChkTxEcho) ) // Si on est bien configurés pour Vérifier l'echo :
	{
		if(0 != pComManager->pNextRxByte)
		{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbChkEcho) { pComManager->nbChkEcho++; }
#endif // UART_COM_SUPPORT_STATS

			if(*(uint8_t*)pComManager->pChkTxEcho != *pComManager->pNextRxByte) // Erreur d'Echo :
			{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
				if(UINT32_MAX > pComManager->nbErrEcho) { pComManager->nbErrEcho++; }

				// Répartition de l'Indice du Byte qui a été corrompu par l'Echo :
				if(0 != pComManager->pCurFrameInfo)
				{
					tComFrameParams* pCurFrameInfo = pComManager->pCurFrameInfo;
					uint16_t idByte = pCurFrameInfo->nbBytes - pComManager->curTxBufInfo.nbBytes;
					if(idByte < NB_ERR_ECHO_COUNT)
					{
						if(UINT32_MAX > pComManager->nbErrEchId[idByte]) { pComManager->nbErrEchId[idByte]++; }
					}
				}

#endif // UART_COM_SUPPORT_STATS

				UartCom_HandleTxError(pComManager); // Appelle la sous-fonction de Gestion
				// ToDo : voir pour signaler et laisser plutôt le RunTime s'en occuper ?
				chkEchoFail = 1;
				pComManager->mayDiscardRx = 1; // Réception inattendue => Discard Frame
// Test_Jp le 17/05/2021				pComManager->pChkTxEcho = 0; // Annuler la Vérification de l'echo en cours
			}
		}

		if(0 == chkEchoFail)
		{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbEchoVld) { pComManager->nbEchoVld++; }
#endif // UART_COM_SUPPORT_STATS

			if(pComManager->curTxBufInfo.nbBytes > 0)
			{
				// Remarque_Jp le 27/05/2021 : probablement pas besoin de Verrouiller ici (pComManager->lockTxIt = 1) l'envoi normal par l'IT !
				pComManager->curTxBufInfo.pBufBase++; // On n'a normalement reçu qu'1 seul byte
				pComManager->curTxBufInfo.nbBytes--;
				pComManager->pChkTxEcho++; // Pointe sur pBuf
			}
			if(0 == pComManager->curTxBufInfo.nbBytes)
			{
				pComManager->pChkTxEcho = 0; // Annuler la Vérification de l'echo pour les prochains Bytes reçus
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
				if(UINT32_MAX > pComManager->nbEchoEOF) { pComManager->nbEchoEOF++; }
#endif // UART_COM_SUPPORT_STATS
			}
		}
		// dans tous les cas : Relancer la Réception au même endroit
	} else
#else // !UART_COM_DISABLE_CHECK_ECHO :
  #ifdef UART_COM_ENABLE_IBUS	// cf. "UartComConf.h"
	#warning "UART_COM_ENABLE_IBUS && UART_COM_DISABLE_CHECK_ECHO !"
  #endif // UART_COM_ENABLE_IBUS
#endif // UART_COM_DISABLE_CHECK_ECHO
	{ // On est dans un cas de Réception normale :
		if(UART_COM_OVERFLOW_BYTE == pComManager->pNextRxByte) // Si on pointe déjà sur l'OverflowByte :
		{
			pComManager->mayDiscardRx = 1; // Réception Overflow => Discard Frame !
		} else {
			pComManager->curRxBufInfo.nbBytes += hUart->RxXferSize; // Accepte le(s) Byte(s) reçu(s)
		}
	}

#ifndef UART_COM_DISABLE_CHECK_ECHO
	if(0 == chkEchoFail) // Si pas d'echo ou pas d'erreur d'echo :
#endif // UART_COM_DISABLE_CHECK_ECHO
	{
		UartCom_ReSetRx(pComManager, 0); // Relancer la Réception SANS RéInitialiser le Buffer
	}
    pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame; 	// Recharge le Sablier de Fin de Trame
    pComManager->sabTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Recharge le Délai pour Répondre

#ifndef UART_COM_DISABLE_CHECK_ECHO
    if(0 == pComManager->pChkTxEcho)
#endif // UART_COM_DISABLE_CHECK_ECHO
    {
    	pComManager->sabReady4Tx = pInitParam->sabReady4TxFrame; // Recharge le Sablier pour Nouvelle Trame
    }
}

/******************************************************************************/

#define MAX_RxEventInfo	50
//#define MAX_RxBufSize	7
typedef struct
{
	uint16_t Tick;
	uint8_t RxEventType;
	uint8_t Size;
	uint8_t nbBytes;
	uint8_t Buf[MAX_RxBufSize];
} RxEventInfo;

uint16_t nxtRxEventInfoId = 0;
RxEventInfo RxEventInfos[MAX_RxEventInfo] = {0};

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *hUart, uint16_t Size) // Handler partagé par tous les UARTs & USARTs (uniquement)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hUart);
	if(0 == pComManager)	// Handle pas dans la Liste => Impossible de déterminer le Manager associé :
	{
#ifdef NEXT_HAL_UARTEx_RxEventCallback	// cf. "UartComConf.h"
		NEXT_HAL_UARTEx_RxEventCallback(hUart, Size);	// ATTENTION : Fonction Non Testée !
#endif // NEXT_HAL_UARTEx_RxEventCallback
		return;
	}
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !

	if(nxtRxEventInfoId >= MAX_RxEventInfo) nxtRxEventInfoId = 0;
	{
		/*
  *           HAL_UART_RXEVENT_TC                 = 0x00U,
  *           HAL_UART_RXEVENT_HT                 = 0x01U,
  *           HAL_UART_RXEVENT_IDLE               = 0x02U,
  *
  *             * @note  When HAL_UARTEx_ReceiveToIdle_IT() or HAL_UARTEx_ReceiveToIdle_DMA() API are called, progress
  *        of reception process is provided to application through calls of Rx Event callback (either default one
  *        HAL_UARTEx_RxEventCallback() or user registered one). As several types of events could occur (IDLE event,
  *        Half Transfer, or Transfer Complete), this function allows to retrieve the Rx Event type that has lead
  *        to Rx Event callback execution.
  * @note  This function is expected to be called within the user implementation of Rx Event Callback,
  *        in order to provide the accurate value :
  *        In Interrupt Mode :
  *           - HAL_UART_RXEVENT_TC : when Reception has been completed (expected nb of data has been received)
  *           - HAL_UART_RXEVENT_IDLE : when Idle event occurred prior reception has been completed (nb of
  *             received data is lower than expected one)
  *        In DMA Mode :
  *           - HAL_UART_RXEVENT_TC : when Reception has been completed (expected nb of data has been received)
  *           - HAL_UART_RXEVENT_HT : when half of expected nb of data has been received
  *           - HAL_UART_RXEVENT_IDLE : when Idle event occurred prior reception has been completed (nb of
  *             received data is lower than expected one).
  *        In DMA mode, RxEvent callback could be called several times;
  *        When DMA is configured in Normal Mode, HT event does not stop Reception process;
  *        When DMA is configured in Circular Mode, HT, TC or IDLE events don't stop Reception process;
  * @param  huart UART handle.
  * HAL_UART_RxEventTypeTypeDef HAL_UARTEx_GetRxEventType(const UART_HandleTypeDef *huart)
  *
  * #if defined(HAL_DMA_MODULE_ENABLED)
		 */
		RxEventInfos[nxtRxEventInfoId].Tick = (uint16_t)HAL_GetTick();
		RxEventInfos[nxtRxEventInfoId].RxEventType = (uint8_t)hUart->RxEventType;
		RxEventInfos[nxtRxEventInfoId].Size = (uint8_t)Size;
		RxEventInfos[nxtRxEventInfoId].nbBytes = (uint8_t)pComManager->curRxBufInfo.nbBytes;
		//if(Size > 0)
		{ UartCom_CopyMemory(RxEventInfos[nxtRxEventInfoId].Buf, pComManager->curRxBufInfo.pBufBase, MAX_RxBufSize); } // Copie la Réception dans notre Emplacement prévu
		nxtRxEventInfoId++;
	}

	//if(Size > 0)
//	{
		if(UART_COM_OVERFLOW_BYTE == pComManager->pNextRxByte) // Si on pointe déjà sur l'OverflowByte :
		{
			if(Size > 0) { pComManager->mayDiscardRx = 1; } // Réception Overflow => Discard Frame !
		} else {	// Tant qu'on est pas sur un DMA circulaire :
			// si HAL_UART_RXEVENT_HT | HAL_UART_RXEVENT_IDLE | HAL_UART_RXEVENT_TC :
//			pComManager->curRxBufInfo.nbBytes = Size; // Accepte la nouvelle taille !
			if(HAL_UART_RXEVENT_HT == hUart->RxEventType)	// Half Transfer event => Wait (long) for last part of Bloc :
			{
				pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
			} else { // HAL_UART_RXEVENT_IDLE or HAL_UART_RXEVENT_TC or else :
				pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
				pComManager->curRxBufInfo.nbBytes += Size; // en Idle OU TC => Ajoute la taille supplémentaire !
			}
		}
//	}

	if(HAL_UART_RXEVENT_TC == hUart->RxEventType)	// Transfer Complete event => all Requested bytes are received :
//	if( (HAL_UART_RXEVENT_HT != hUart->RxEventType) && (HAL_UART_RXEVENT_IDLE != hUart->RxEventType) ) // HAL_UART_RXEVENT_TC OR default :
	{
		UartCom_ReSetRx(pComManager, 0); // Relancer la Réception SANS RéInitialiser le Buffer => Pointe sur OverFlowByte !
	} else {
		pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
	}

//	switch(hUart->RxEventType)
//	{
//	case HAL_UART_RXEVENT_HT:	// Half Transfer event => Continue :
//		if(Size > 0)
//		{
//			if(UART_COM_OVERFLOW_BYTE == pComManager->pNextRxByte) // Si on pointe déjà sur l'OverflowByte :
//			{
//				pComManager->mayDiscardRx = 1; // Réception Overflow => Discard Frame !
//			} else {	// Tant qu'on est pas sur un DMA circulaire :
//				pComManager->curRxBufInfo.nbBytes = Size; // Accepte la nouvelle taille !
//			}
//			pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
////		} else {
////			pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
//		}
////		//pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
////		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
////		Si size > 0 && overflow => discard
//		break;
//	case HAL_UART_RXEVENT_IDLE:	// IDLE event => Trame complète :
//		//pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
//		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
//		break;
//	case HAL_UART_RXEVENT_TC:	// Transfer Complete event => Buffer Overflow :
//	default:
//
//		pComManager->mayDiscardRx = 1; // Trop long => Discard Frame
//		UartCom_ReSetRx(pComManager, 0); // Relancer la Réception SANS RéInitialiser le Buffer
//	//	pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
//		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
//		break;
//	}
//	if(HAL_UART_RXEVENT_IDLE == hUart->RxEventType)	// IDLE => Trame complète :
//	{
//		pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
//		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
//	}

//	if(Len < pInitParam->maxRxPacketSize) // On a reçu moins que la taille max autorisé => C'est une trame déjà Complète :
//	{
//		pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
//		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
//	} else { // On a reçu la capacité totale du Buffer => Il manque peut-être encore des Bytes :
//		UartCom_ReSetRx(pComManager, 0); // Relancer la Réception SANS RéInitialiser le Buffer
//		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
//	}

    pComManager->sabTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Recharge le Délai pour Répondre
    pComManager->sabReady4Tx = pInitParam->sabReady4TxFrame;		// Recharge le Sablier pour Nouvelle Trame

//    pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame; 	// Recharge le Sablier de Fin de Trame
//    pComManager->sabTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Recharge le Délai pour Répondre
//    if(0 == pComManager->pChkTxEcho) pComManager->sabReady4Tx = pInitParam->sabReady4TxFrame; // Recharge le Sablier pour Nouvelle Trame
}

/******************************************************************************/

void* UartCom_ReLoad_WithSameParams(void* hHandle)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return 0;	// Handle pas dans la Liste => Impossible de déterminer le Manager associé !
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam) 	return 0;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !
	uint32_t* pLastRxFrame = pComManager->pLastRxFrame;		// Récupère le Pointeur de LastRxFrame

	UartCom_FillMemory((void*)pComManager, 0, sizeof(tUartComManager)); // Nettoyer notre propre Structure avant de la Configurer
	pComManager->pInitParams = pInitParam; 					// Restaure le lien vers les Infos d'Init
	pComManager->pLastRxFrame = pLastRxFrame;				// Restaure le lien vers le Pointeur de LastRxFrame
	pComManager->sabReady4Tx = pInitParam->sabInitNoTx;		// Applique la Tempo Initiale avant la 1ere Emission
	if(0 != pLastRxFrame) { *pLastRxFrame = UINT32_MAX; }	// Initialise LastRxFrame au Max possible (équivalent de -1)

	pComManager->isLoaded = 1;				// Confirmer le Chargement Actif du Lien vers UartCom
	return pInitParam->RxInfos.pBufBase;	// Retourne la Base du Pointeur de Réception, pour Configuration par l'appelant
}

/******************************************************************************/

void UartCom_RequestDelay4Reply(void* hHandle, uint16_t newTimeOut4Reply)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return;	// Handle pas dans la Liste => Impossible de déterminer le Buffer associé !

	if(0 == newTimeOut4Reply) // Si demande le TimeOut par défaut :
	{
		tUartComInitParams* pInitParam = pComManager->pInitParams;
		if(0 != pInitParam) newTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Délai par défaut pour Répondre
	}
	if(0 == newTimeOut4Reply) newTimeOut4Reply = 1; // forcer au Minimum non nul.
	pComManager->sabTimeOut4Reply = newTimeOut4Reply;
}

/******************************************************************************/

HAL_StatusTypeDef UartCom_ReInitUartWithCustomParams(UartReInitItem* pReInitItem)
{
#ifdef UART_COM_SUPPORT_REINIT	// cf. "UartComConf.h"

	// Vérification des Paramètres d'entrée :
	if(0 == pReInitItem) return HAL_ERROR;
	if(0 == pReInitItem->huart) return HAL_ERROR;
	if(0 == pReInitItem->Instance) return HAL_ERROR;
	if(0 == pReInitItem->pUserParams) return HAL_ERROR;
	if(0 == pReInitItem->pCoreVars) return HAL_ERROR;
	if(0 == pReInitItem->pDefParams) return HAL_ERROR;

	// Variables locales pour accès facilité :
	UART_HandleTypeDef* huart   		= pReInitItem->huart;
	UartReInitUserParams* pUserParams	= pReInitItem->pUserParams;
	UartReInitCoreVars* pCoreVars		= pReInitItem->pCoreVars;
	const UartReInitUserParams* pDefParams = pReInitItem->pDefParams;
	uint32_t tmpU32;

	// Si RéInit All to Default :
	if(0 != pCoreVars->ReInitFlags.InitDefault)
	{
		pUserParams->BaudRateDiv100	= pDefParams->BaudRateDiv100;	// Parmi eUartReInitBaudRate
		pUserParams->Parity 		= pDefParams->Parity;			// Parmi eUartReInitParity
		pUserParams->nbStops		= pDefParams->nbStops;			// Parmi eUartReInitStop
		pUserParams->AdvancedInit	= pDefParams->AdvancedInit;
	}

	// Register UART Instance :
	huart->Instance = pReInitItem->Instance;

	// Check & Apply allowed BaudRate :
	switch(pUserParams->BaudRateDiv100)
	{
	case eUartReInitBaud4800:	// 4800 bauds
	case eUartReInitBaud9600:	// 9600 bauds
	case eUartReInitBaud19200:	// 19200 bauds
	case eUartReInitBaud38400:	// 38400 bauds
	case eUartReInitBaud57600:	// 57600 bauds
	case eUartReInitBaud115200:	// 115200 bauds
//	case eUartReInitBaud230400:	// 230400 bauds (pas demandé)
//	case eUartReInitBaud460800:	// 460800 bauds (pas demandé)
		break;
	case eUartReInitBaud2Def:
	default:
		pUserParams->BaudRateDiv100 = pDefParams->BaudRateDiv100;
		if(eUartReInitBaud2Def == pUserParams->BaudRateDiv100) pUserParams->BaudRateDiv100 = eUartReInitBaudDefault;
		break;
	}
	huart->Init.BaudRate = (uint32_t)100 * pUserParams->BaudRateDiv100;

	// Check & Apply allowed Parity :
	switch (pUserParams->Parity)
	{
	case eUartReInitParityNone:
	case eUartReInitParityOdd:
	case eUartReInitParityEven:
		break;
	case (uint8_t)eUartReInitParity2Def:
	default:
		pUserParams->Parity = pDefParams->Parity;
		if((uint8_t)eUartReInitParity2Def == pUserParams->Parity) pUserParams->Parity = eUartReInitParityDefault;
		break;
	}
	switch (pUserParams->Parity)
	{
		case eUartReInitParityOdd:
			tmpU32 = UART_PARITY_ODD;
			break;
		case eUartReInitParityEven:
			tmpU32 = UART_PARITY_EVEN;
			break;
		case eUartReInitParityNone:
		default:
			tmpU32 = UART_PARITY_NONE;
			break;
	}
	huart->Init.Parity = tmpU32;
	huart->Init.WordLength = (UART_PARITY_NONE == tmpU32) ? UART_WORDLENGTH_8B : UART_WORDLENGTH_9B;

	// Check & Apply allowed Stop bits :
	switch (pUserParams->nbStops)
	{
	case eUartReInitStopOne:
	case eUartReInitStopTwo:
	case eUartReInitStop1_5:
		break;
	case eUartReInitStop2Def:
	default:
		pUserParams->nbStops = pDefParams->nbStops;
		if(eUartReInitStop2Def == pUserParams->nbStops) pUserParams->nbStops = eUartReInitStopDefault;
		break;
	}
	switch (pUserParams->nbStops)
	{
		case eUartReInitStopTwo:
			tmpU32 = UART_STOPBITS_2;
			break;
		case eUartReInitStop1_5:
			tmpU32 = UART_STOPBITS_1_5;
			break;
		case eUartReInitStop2Def:
		case eUartReInitStopOne:
		default:
			tmpU32 = UART_STOPBITS_1;
			break;
	}
	huart->Init.StopBits = tmpU32;

	// Common Params :
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	// Advanced Params :
	if(0 == pUserParams->AdvancedInit)
	{
		huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	} else { // Copie directe depuis pUserParams->AdvancedInit to huart->AdvancedInit :
		UartCom_CopyMemory((void*)&huart->AdvancedInit, (void*)pUserParams->AdvancedInit, sizeof(UART_AdvFeatureInitTypeDef));
	}

	// Finalise Init :
	if(0 != pCoreVars->ReInitFlags.InitRS485Ex) // Init as RS485 Extended :
	{
		return HAL_RS485Ex_Init(huart, pCoreVars->ReInitFlags.DePolarity ? UART_DE_POLARITY_LOW : UART_DE_POLARITY_HIGH, 0, 0);
	} else {
		return HAL_UART_Init(huart); // Init as standard classic UART
	}
#else // !UART_COM_SUPPORT_REINIT :
	return HAL_ERROR;
#endif // UART_COM_SUPPORT_REINIT
}

/******************************************************************************/

void UartCom_Discard_Handle(void* hHandle)
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return;	// Handle pas dans la Liste => Impossible de déterminer le Buffer associé !

	pComManager->mayDiscardRx = 1;	// Ignorer toute nouvelle Réception
	pComManager->isLoaded = 0;		// Enregistrer la déconnexion du module
}

/******************************************************************************/
//ToDo: If enabled handle bloc from external (as usb)

void UartCom_Handle_ReceivedBloc(void* hHandle, uint8_t* pRx, uint16_t Len)	// A appeler dans tous les Handlers de réception Externes, par exemple USB :
{
	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return;	// Handle pas dans la Liste => Impossible de déterminer le Manager associé !
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !

	if( (0 != pRx) && (pRx != pComManager->pNextRxByte) ) // Si la Réception n'a pas eu lieu à l'emplacement prévu :
	{
		if(0 == pComManager->pNextRxByte) { Len = 0; } // la Réception n'était pas attendue => Abandonne le traitement
		else {
			if((pComManager->curRxBufInfo.nbBytes + Len) > pComManager->curRxBufInfo.maxBytes)	// Filtre d'un éventuel débordement de Buffer :
			{
				if(pComManager->curRxBufInfo.maxBytes > pComManager->curRxBufInfo.nbBytes) // Si on n'a pas déjà rempli le Buffer :
				{
					Len = pComManager->curRxBufInfo.maxBytes - pComManager->curRxBufInfo.nbBytes;	// Plafonne au max dispobible
				} else { Len = 0; }
				pComManager->mayDiscardRx = 1; // Ca déborde => impossible de traiter correctement !
			}
			if(Len > 0) { UartCom_CopyMemory(pComManager->pNextRxByte, pRx, Len); } // Copie la Réception dans notre Emplacement prévu
		}
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbBlocsCpy) { pComManager->nbBlocsCpy++; }
#endif // UART_COM_SUPPORT_STATS
	}

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if((UINT32_MAX - Len) >= pComManager->nbBytesRx) { pComManager->nbBytesRx += Len; }
	if(UINT32_MAX > pComManager->nbBlocsRx) { pComManager->nbBlocsRx++; }
#endif // UART_COM_SUPPORT_STATS

	pComManager->curRxBufInfo.nbBytes += Len;
	if(Len < pInitParam->maxRxPacketSize) // On a reçu moins que la taille max autorisé => C'est une trame déjà Complète :
	{
		pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxFrame;	// Recharge le Sablier de Fin de Trame
	} else { // On a reçu la capacité totale du Buffer => Il manque peut-être encore des Bytes :
		UartCom_ReSetRx(pComManager, 0); // Relancer la Réception SANS RéInitialiser le Buffer
		pComManager->sabEndOfRxFrame = pInitParam->sabEndOfRxBloc;	// Recharge le Sablier TimeOut Fin de Bloc de Trame
	}
    pComManager->sabTimeOut4Reply = pInitParam->sabTimeOut4Reply;	// Recharge le Délai pour Répondre
    pComManager->sabReady4Tx = pInitParam->sabReady4TxFrame;		// Recharge le Sablier d'indisponibilité pour Envoi Tx
}

/******************************************************************************/

eUartComActivity UartCom_GetRecentActivity(void* hHandle, uint32_t seuilRecent)
{
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 != pComManager)
	{
		if(0 != pComManager->isLoaded)
		{
			if(0 != pComManager->pLastRxFrame)
			{
				if(*(pComManager->pLastRxFrame) <= seuilRecent)
				{
					return UartComActivityRecent;
				}
			}
			return UartComActivityReady;
		}
	}
	return UartComActivityNone;
}

/******************************************************************************/

void UartCom_ReSetRx(tUartComManager* pComManager, uint16_t reInitRxBuf)
{
	if(0 == pComManager) return; // On ne peut rien faire !
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de poursuivre !

	pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Recharge le Sablier de reconfiguration de la Réception
	tUartComClassFn* pClassFn = (tUartComClassFn*)pInitParam->pClassFn;

	// S'il faut RéInitialiser la Réception sur l'UART :
    if(0 != (reInitRxBuf & UART_COM_RESET_RX_INIT_BUF) )
    {
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbInitRxBuf) { pComManager->nbInitRxBuf++; }
#endif // UART_COM_SUPPORT_STATS

    	if(0 != pInitParam->hHandle)
    	{
    		pUartCom_CtrlFn pCtrlFn = 0;
    		if(0 != pClassFn) { pCtrlFn = pClassFn->pFnAbortReceive; }
#ifdef UART_COM_USR_FN_ABORT_RECEIVE
		if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_ABORT_RECEIVE; } // Par défaut : HAL_UART_AbortReceive_IT
#endif // UART_COM_USR_FN_ABORT_RECEIVE
    		if(0 != pCtrlFn)
    		{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
    			if(UINT32_MAX > pComManager->nbAbortRx) { pComManager->nbAbortRx++; }
#endif // UART_COM_SUPPORT_STATS

    			pCtrlFn(pInitParam->hHandle); // Appel de la Fonction configurée
    		}
    	}
		pComManager->curRxBufInfo.pBufBase = 0; // Force RéInit ci-dessous suivant les Paramètres d'origine
    	if(0 == (reInitRxBuf & UART_COM_RESET_RX_KEEP_ECHO) ) { pComManager->pChkTxEcho = 0; } // Annuler l'éventuelle Vérification de l'echo en cours
    }

    // Prépare la prochaine Réception :
    if(pComManager->curRxBufInfo.pBufBase < pInitParam->RxInfos.pBufBase)
    {
    	pComManager->curRxBufInfo.pBufBase = pInitParam->RxInfos.pBufBase;
    	pComManager->curRxBufInfo.maxBytes = pInitParam->RxInfos.nbBytes;
    	pComManager->curRxBufInfo.nbBytes = 0;
    }

    // Protège le Buffer de Réception d'un éventuel débordement de Bloc :
//	if((pComManager->curRxBufInfo.nbBytes + pInitParam->maxRxPacketSize) > pComManager->curRxBufInfo.maxBytes)
//	{
//		pComManager->curRxBufInfo.nbBytes = pComManager->curRxBufInfo.maxBytes - pInitParam->maxRxPacketSize;
//		pComManager->mayDiscardRx = 1; // Trop long => Discard Frame
//	}
    uint16_t maxRxSize = pInitParam->maxRxPacketSize;
    if((pComManager->curRxBufInfo.nbBytes + maxRxSize) > pComManager->curRxBufInfo.maxBytes)
	{
    	maxRxSize = pComManager->curRxBufInfo.maxBytes - pComManager->curRxBufInfo.nbBytes;
	}

	// Relancer la Réception :
    if(0 != pInitParam->hHandle)
    {
    	if(maxRxSize > 0)
    	{
    		pComManager->pNextRxByte = pComManager->curRxBufInfo.pBufBase + pComManager->curRxBufInfo.nbBytes;
    	} else {
    		pComManager->pNextRxByte = UART_COM_OVERFLOW_BYTE;	// Pointe sur le Byte d'Overflow
    		maxRxSize = 1;
    	}
    	pUartCom_IoFn pIoFn = 0;
    	if(0 != pClassFn) { pIoFn = pClassFn->pFnStartReceive; }
#ifdef UART_COM_USR_FN_START_RECEIVE
	if(0 == pIoFn) { pIoFn = UART_COM_USR_FN_START_RECEIVE; } // Par défaut : HAL_UART_Receive_IT
#endif // UART_COM_USR_FN_START_RECEIVE
    	if(0 != pIoFn)
    	{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
    		if(UINT32_MAX > pComManager->nbReStartRx) { pComManager->nbReStartRx++; }
#endif // UART_COM_SUPPORT_STATS

//    		if(HAL_OK == pIoFn(pInitParam->hHandle, pComManager->pNextRxByte, pInitParam->maxRxPacketSize)) // Si la demande de Rx a été acceptée :
    		if(HAL_OK == pIoFn(pInitParam->hHandle, pComManager->pNextRxByte, maxRxSize)) // Si la demande de Rx a été acceptée :
    		{
    			if(0 != reInitRxBuf) { pComManager->mayDiscardRx = 0; }
    			return;
    		}
    	}
    }

    // Autres cas => Erreur :
    pComManager->mayDiscardRx = 1; // Il faudra ignorer cette Trame
    pComManager->sabEndOfRxFrame = pInitParam->sabErrorRxFrame; // Temporiser un peu avant de relancer une Réception

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
    if(UINT32_MAX > pComManager->nbReSetRxFail) { pComManager->nbReSetRxFail++; }
#endif // UART_COM_SUPPORT_STATS

}

/******************************************************************************/

#ifdef UART_COM_SUPPORT_TX_RX_PIN	// cf. "UartComConf.h"
static void UartCom_Manage_PinDirection(tUartComInitParams* pInitParams, uint16_t nbPdgBytes2Tx)
{
	if(0 == pInitParams) return;
	if(0 == pInitParams->GPIOx) return;
	GPIO_PinState newPinState = (pInitParams->flag.onTxPin ^ (0 == nbPdgBytes2Tx ? 1: 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(pInitParams->GPIOx, pInitParams->GPIO_Pin, newPinState);
}
#endif // UART_COM_SUPPORT_TX_RX_PIN

/****************************************************************************/

static void* UartCom_LockBuf4Size(tBufManager* pBM, tComFrameParams* pFI, uint16_t nbBytes)
{
	if(0 == pFI) return 0; // Unavailable Param
	if(0 == pBM) return 0; // Failure to Manage
	tBufParams* pBI = pBM->pBuf; // Pointeur sur BufInfos
	if(0 == pBI) return 0; // Failure to Manage
	if(nbBytes > pBI->nbBytes) { nbBytes = pBI->nbBytes; } // Si dépassement : plafonne au max dispo
	void* pRet = 0;

	// Recherche en "bas" du Buffer, si on a déjà rempli par le "haut" :
	if(0 != pBM->pLow.pFree)
	{
		pRet = UartCom_LockBufIfEnough(&(pBM->pLow), pBI, pBM->pHigh.pBusy, pFI, nbBytes);
		if(0 != pRet) return pRet;
	}

	// Recherche vers le "haut" du Buffer, si disponible :
	pRet = UartCom_LockBufIfEnough(&(pBM->pHigh), pBI, 0, pFI, nbBytes);
	if(0 != pRet) return pRet;

	// Recherche de nouveau en "bas" du Buffer :
	pRet = UartCom_LockBufIfEnough(&(pBM->pLow), pBI, pBM->pHigh.pBusy, pFI, nbBytes);
	if(0 != pRet) return pRet;

	return 0;
}

/****************************************************************************/

static void* UartCom_LockBufIfEnough(tBusyFree* pBF, tBufParams* pBuf, void* pEnd, tComFrameParams* pFI, uint16_t nbBytes)
{
	if(0 == pBF->pFree)	{ pBF->pFree = pBuf->pBufBase; }	// Init en cas de besoin : Libre = début du Buffer affecté
	if(0 == pBF->pBusy) { pBF->pBusy = pBF->pFree; }		// Init en cas de besoin : Busy = début de la zone libre
	if(0 == pEnd) { pEnd = pBuf->pBufBase + pBuf->nbBytes; } // Si absent : plafond = après la fin du Buffer
	if(pEnd <= pBF->pFree) return 0;						// Sécurité pour ne pas calculer un nombre négatif !
	uint16_t maxBytes = (uint16_t)(pEnd - pBF->pFree);		// Calcule la disponibilité : Plafond - début de la Zonelibre
	if(nbBytes <= maxBytes) // On a trouvé un emplacement dans la Zone :
	{
		pFI->pBufBase = pBF->pFree;	// Emplacement libre actuel
		pFI->nbBytes = nbBytes;		// Taille attribuée = taille demandée
		pBF->pFree += nbBytes;		// Décale notre pointeur de ZoneLibre
		return pFI;
	}
	return 0;
}

/****************************************************************************/

static tUartComManager* UartCom_GetManagerFromHandle(void* hHandle)
{
	if(0 == hHandle)	return 0; // Erreur
	for(tUartComManager* pTmpUartFrame = mUartComManager; pTmpUartFrame < (tUartComManager*)EndOfUartFrame; pTmpUartFrame++)
	{
		if(0 == pTmpUartFrame->pInitParams) continue;
		if((void*)(pTmpUartFrame->pInitParams->hHandle) == hHandle) return pTmpUartFrame; // On a trouvé notre Structure de Gestion !
	}
	return 0; // NotFound !
}

/****************************************************************************/

static tComFrameParams* UartCom_GetFreeBufInfo(tUartComManager* pComManager)
{
	// Tout d'abord, recherche parmi les BufInfos Réservés :
	if(0 != pComManager)
	{
		for(int i = 0; i < NB_FRAME_PARAMS_PER_COM_MANAGER; i++)
		{
			if(0 == pComManager->bufInfo[i].pBufBase)
			{
				return &(pComManager->bufInfo[i]);
			}
		}
	}

	// Si aucun n'est disponible, recherche parmi les BufInfos partagés :
	for(int i = 0; i < NB_SHARED_UART_COM_FRAME_PARAMS; i++)
	{
		if(0 == shrdBufInfo[i].pBufBase)
		{
			return &shrdBufInfo[i];
		}
	}

	// Si toujours rien de dispo :
	return 0;
}

/****************************************************************************/

tComFrameParams* UartCom_RequestLargerBuffer(tComFrameParams* pFI, uint16_t maxBytes)
{
	// ToDo : Attention ... Fonction non encore testée !
	if(0 == pFI) return 0;

	// Libérer le Buffer actuel :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(pFI->hHandle);
	tBufManager* pBM = 0;
	if(0 != pComManager) { pBM = &(pComManager->bufMgr); } // Propose de chercher d'abord dans notre propre BufManager
	tBusyFree* pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, pBM); // Au pire, il y a en plus une chance avec le BufManager partagé
	UartCom_UnlockFrameInfoAndBF(pFI, pBF);	// Libère le FrameInfo & le Bloc dans le BufferManager

	// Demander l'attribution d'un nouveau Buffer avec assez de Bytes (partie de UART_Com_InitNewTxFrame) :
	if(maxBytes > 0)
	{
		void* pRet = UartCom_LockBuf4Size(pBM, pFI, maxBytes); // Tenter avec notre propre BufManager ...
		if(0 == pRet) // Si on n'a pas pu trouver de place dans la Zone spécifique :
		{
			pRet = UartCom_LockBuf4Size(&shrdBufTxMgr, pFI, maxBytes); // Tente avec le BufManager partagé !
		}

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbRqLargerBuffer) { pComManager->nbRqLargerBuffer++; }

	} else {
		if(UINT32_MAX > pComManager->nbRqEmptyBuffer) { pComManager->nbRqEmptyBuffer++; }
#endif // UART_COM_SUPPORT_STATS

	}
	return pFI;
}

/****************************************************************************/

tComFrameParams* UartCom_InitNewTxFrame(void* hHandle, uint16_t maxBytes)
{
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager)	return 0;	// Handle pas dans la Liste => Impossible de déterminer le Manager associé !

	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return 0;		// Infos d'Init non disponibles => Impossible de déterminer la Config associée !

	tComFrameParams* pFI = UartCom_GetFreeBufInfo(pComManager);
	if(0 == pFI)	return 0;			// Aucun emplacement de FrameInfo disponible !

	if(0 == pComManager->bufMgr.pBuf) { pComManager->bufMgr.pBuf = &(pInitParam->TxInfos); } // Lien vers les infos de TX
	void* pRet = 0; tBufManager* pBM;

	// Recherche d'un emplacement disponible de taille suffisante :
	for(int nbTries = 0; nbTries < 4; nbTries++)
	{
		pBM = (nbTries & 1) ? &shrdBufTxMgr : &(pComManager->bufMgr);
		if( (nbTries >= 2) && (maxBytes <= pBM->pBuf->nbBytes) ) // Si c'est le moment et uniquement si ça peut rentrer :
		{
			UartCom_unLockBusyFree(&(pBM->pHigh));	// Détruire les Infos du Haut
			UartCom_unLockBusyFree(&(pBM->pLow));	// + les infos du Bas du Buffer
		}

		pRet = UartCom_LockBuf4Size(pBM, pFI, maxBytes);
		if(0 != pRet) break;

#if defined(UART_COM_SHARED_BUF_TX_SIZE) && (UART_COM_SHARED_BUF_TX_SIZE > 0)	// cf. "UartComConf.h"
		// Pour Préparer la demande sur le Buffer partagé, il faut qu'il soit bien renseigné :
		if(0 == shrdBufTxMgr.pBuf) { shrdBufTxMgr.pBuf = (tBufParams*)&shrdBufBloc; }
#endif // UART_COM_SHARED_BUF_TX_SIZE
	}
	if(0 == pRet) return 0; // On n'a pas réussi :-( !

	// On a toutes les infos nécessaires => Terminer de remplir la Structure :
	pFI->hHandle = hHandle;
	pFI->nbReTries = 0; // Par défaut : pas de retry

#ifdef UART_COM_SUPPORT_FRAME_TTL	// cf. "UartComConf.h"
	switch(pInitParam->ProtocolType)
	{
	case UartComIbus:
		pFI->time2Live = COM_FRAME_DEF_TTL_IBUS;
		break;
	case UartComModbus:
		pFI->time2Live = COM_FRAME_DEF_TTL_MODBUS;
		break;
	case UartComCustom:
		pFI->time2Live = COM_FRAME_DEF_TTL_CUSTOM;
		break;
	case UartComDisabled:
	default:
		pFI->time2Live = COM_FRAME_TTL_DISABLED;
		break;
	}
#endif // UART_COM_SUPPORT_FRAME_TTL

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
    if(UINT32_MAX > pComManager->nbRqInitNewTxFrame) { pComManager->nbRqInitNewTxFrame++; }
#endif // UART_COM_SUPPORT_STATS
	return pFI;
}

/****************************************************************************/

static uint16_t UartCom_DoTransmit(tUartComManager* pComManager, uint16_t nbBytes)
{
	// Vérification des Paramètres & Infos nécessaires :
	if(0 == pComManager) return 0;
	if(0 == pComManager->curTxBufInfo.pBufBase) return 0;
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam) return 0;
	tUartComClassFn* pClassFn = (tUartComClassFn*)pInitParam->pClassFn;

	pUartCom_IoFn pFn = 0;
	if(0 != pClassFn) { pFn = pClassFn->pFnStartTransmit; }
#ifdef UART_COM_USR_FN_START_TRANSMIT
	if(0 == pFn) { pFn = UART_COM_USR_FN_START_TRANSMIT; }
#endif // UART_COM_USR_FN_START_TRANSMIT
	if(0 == pFn) return 0;

	// Plafonnement si nécessaire :
	if(nbBytes > pComManager->curTxBufInfo.nbBytes) { nbBytes = pComManager->curTxBufInfo.nbBytes; }

	// Lance la séquence d'envoi :
	HAL_StatusTypeDef ret = HAL_OK;
#ifdef UART_COM_SUPPORT_TX_RX_PIN	// cf. "UartComConf.h"
	UartCom_Manage_PinDirection(pInitParam, nbBytes);
#endif // UART_COM_SUPPORT_TX_RX_PIN

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
    if(UINT32_MAX > pComManager->nbDoTransmit) { pComManager->nbDoTransmit++; }
#endif // UART_COM_SUPPORT_STATS
	if(nbBytes > 0)
	{
		pComManager->pChkTxEcho = pComManager->curTxBufInfo.pBufBase;	// Prépare le Pointeur pour Check Echo, si nécessaire
		ret = pFn(pInitParam->hHandle, pComManager->pChkTxEcho, nbBytes); // Appel de la Fonction Tx renseignée dans la Config
		pComManager->sabMayTxNextByte = pInitParam->sabMayTxNextByte;	// Recharger le Délai avant le Prochain Byte
	}

	if(HAL_OK == ret)
	{
		if(0 == pInitParam->flag.chkEcho) // Si pas de Contrôle d'Echo :
		{
			pComManager->curTxBufInfo.pBufBase += nbBytes;	// Prépare le Ptr pour la prochaine fois
			pComManager->curTxBufInfo.nbBytes -= nbBytes;	// Décompte les Bytes restants
		}
		return 1;
	}
	return 0;
}

/****************************************************************************/

static unsigned UartCom_isInBufInfo(void* pVoid, tBufParams* pBI)
{
	if(0 == pBI) return 0; // Protection Paramètre null !
	return ( (pVoid >= pBI->pBufBase) && (pVoid < (pBI->pBufBase + pBI->nbBytes)) ) ? 1 : 0;
}

/****************************************************************************/

static unsigned UartCom_isInBusyFree(void* pVoid, tBusyFree* pBF)
{
	if(0 == pBF) return 0;
	return ( (pVoid >= pBF->pBusy) && (pVoid < pBF->pFree) ) ? 1 : 0;
}

/****************************************************************************/

static inline unsigned UartCom_isLockedBusyFree(tBusyFree* pBF) { return ( (0 != pBF->pBusy) && (0 != pBF->pFree) ) ? 1 : 0; }
static inline void UartCom_unLockBusyFree(tBusyFree* pBF) { pBF->pBusy = pBF->pFree = 0; }

/****************************************************************************/

static tBusyFree* UartCom_getBusyFreeInBufManager(void* pBuf, tBufManager* pBM)
{
	if(0 == pBM) return 0; // Ne peut être Présent dans le vide ;-) !
	tBusyFree* pBF = 0;
	if(UartCom_isInBufInfo(pBuf, pBM->pBuf))
	{
		pBF = &(pBM->pHigh);
		if(0 == UartCom_isInBusyFree(pBuf, pBF))
		{
			pBF = &(pBM->pLow);
			if(0 == UartCom_isInBusyFree(pBuf, pBF))
			{
				return 0;
			}
		}
	}
	return pBF;
}

/****************************************************************************/

uint16_t UartCom_PostFrame(tComFrameParams* pFI)
{
	// Filtre les ereurs de Paramètres & Configuration :
	if(0 == pFI)	return 0;
	if(0 == pFI->hHandle)	return 0;
	if(0 == pFI->pBufBase)	return 0;

	tUartComManager* pComManager = UartCom_GetManagerFromHandle(pFI->hHandle);
	if(0 == pComManager)	return 0;	// Handle pas dans la Liste => Impossible de déterminer le Manager associé !

	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return 0;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !

	tBusyFree* pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, &(pComManager->bufMgr));
	if(0 != pBF) // Mettre à Jour le Pointeur de ZoneFree :
	{
		pBF->pFree = (pFI->pBufBase + pFI->nbBytes); // Part du principe qu'il s'agit forcément du dernier LockBuf
	}

	// Vérifie les options d'envoi :
	uint16_t canPostNow = UartCom_GetMaxAllowedTxBlocSize(pInitParam, pFI->nbBytes, UART_COM_FLAG_DISCARD_TX_NONE);	// No Discard = Normal Request

	// Vérification générale de la disponibilité :
	if(0 != pComManager->sabReady4Tx)
	{
		canPostNow = 0; // Désolé, ce n'est pas un moment approprié pour envoyer maintenant !
	}

	if(pFI->nbBytes > 0) // S'il y avait bien qqch à envoyer :
	{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
		if(UINT32_MAX > pComManager->nbDoPostFrame) { pComManager->nbDoPostFrame++; }
#endif // UART_COM_SUPPORT_STATS

		if(0 == pComManager->pCurFrameInfo) // Si Disponible tout de suite ...
		{
			// => Renseigner la structure de travail bas niveau :
			pComManager->lockTxIt = 1;	// Empêcher pour le moment l'envoi immédiat par Interruption
			pComManager->curTxBufInfo.nbBytes = pFI->nbBytes;
			pComManager->curTxBufInfo.pBufBase = pFI->pBufBase;
			pComManager->pCurFrameInfo = pFI;
#if defined(UART_COM_ENABLE_IBUS) && !defined(UART_COM_DISABLE_SYNC_TX)	// cf. "UartComConf.h"
			pComManager->wait4Sync = pInitParam->flag.maySyncTx;
#endif // UART_COM_ENABLE_IBUS && !UART_COM_DISABLE_SYNC_TX
			pComManager->canTxNow = 0;
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbMayPostFrameNow) { pComManager->nbMayPostFrameNow++; }
#endif // UART_COM_SUPPORT_STATS

			if(canPostNow > 0) // Si on peut toujours envoyer qqch :
			{
				if(0 == UartCom_DoTransmit(pComManager, canPostNow)) // En cas d'erreur d'envoi :
				{
					pComManager->curTxBufInfo.nbBytes = 0; // Simule un Envoi OK !
// Test_Jp le 17/05/2021						pComManager->curTxBufInfo.pBufBase = 0;
// Test_Jp le 17/05/2021						pComManager->pCurFrameInfo = 0; // Libère le FrameInfo courant
					canPostNow = 0; // On peut détruire tout de suite !
// voir s'il faut pas appeler abortTxFn / Errorcallback ...
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
					if(UINT32_MAX > pComManager->nbPostFrameNowErr) { pComManager->nbPostFrameNowErr++; }
#endif // UART_COM_SUPPORT_STATS

				}
				//Todo : voir s'il y a pê des sabliers à relancer ...
			} else {
				canPostNow = UINT16_MAX; // Conserver pour plus tard ...
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
				if(UINT32_MAX > pComManager->nbPostFrameDelayed) { pComManager->nbPostFrameDelayed++; }
#endif // UART_COM_SUPPORT_STATS

			}
			pComManager->lockTxIt = 0;	// Autorise maintenant les envois complémentaires par Interruption
		} else //if(0 == pComManager->pNxtFrameInfo)
		{
			if(0 == pComManager->pNxtFrameInfo) { pComManager->pNxtFrameInfo = pFI; } // Si on peut l'enregistrer comme le prochain
			canPostNow = UINT16_MAX; // Dans tous les cas : Conserver pour plus tard ...
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
			if(UINT32_MAX > pComManager->nbPostFrameLater) { pComManager->nbPostFrameLater++; }
#endif // UART_COM_SUPPORT_STATS

		}
	} else { // S'il n'y a rien à faire :
		canPostNow = 0; // Ne pas conserver le FrameInfos
	}

	return canPostNow;
}

/****************************************************************************/

static void UartCom_UnlockFrameInfoAndBF(tComFrameParams* pFI, tBusyFree* pBF)
{
	if(0 == pFI) return;

	// Libère dans le BusyFree de ce BufferManager :
	if(0 != pBF)
	{
		if(0 != pBF->pFree)	pBF->pFree -= pFI->nbBytes; // Libère de notre FrameInfo actuel
		if(pBF->pFree == pBF->pBusy)
		{
			pBF->pFree = pBF->pBusy = 0; // A Vérifier
		}
	}

	// Libère le FrameInfo :
	pFI->pBufBase = 0;

}

/******************************************************************************/

uint16_t getThisHandleRxBufSize(void *hHandle)
{
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hHandle);
	if(0 == pComManager) { return 0; }
	return pComManager->curRxBufInfo.maxBytes;
}

/****************************************************************************/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUart)	// A appeler par tous les Handlers Externes lorsque l'envoi est Terminé (nécessaire si support TxRxPin) !
{
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hUart);
	if(0 == pComManager) // Handle pas dans la Liste => Impossible de déterminer le Manager associé :
	{
#ifdef NEXT_HAL_UART_TxCpltCallback	// cf. "UartComConf.h"
		NEXT_HAL_UART_TxCpltCallback(hUart);	// ATTENTION : Fonction Non Testée !
#endif // NEXT_HAL_UART_TxCpltCallback
		return;
	}
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbTxBytesSentCB) { pComManager->nbTxBytesSentCB++; }
#endif // UART_COM_SUPPORT_STATS

	if(0 != pComManager->curTxBufInfo.nbBytes) // S'il reste des Datas à envoyer :
	{
		return; // Pas fini, mais pas plus ici ...
	}

	// Si l'envoi est Complètement Terminé :
#ifdef UART_COM_SUPPORT_TX_RX_PIN	// cf. "UartComConf.h"
	UartCom_Manage_PinDirection(pComManager->pInitParams, 0); // Signale que c'est Terminé !
#endif // UART_COM_SUPPORT_TX_RX_PIN

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbTxFramesSentCB) { pComManager->nbTxFramesSentCB++; }
#endif // UART_COM_SUPPORT_STATS
}

/****************************************************************************/

static void UartCom_ReleaseFrameInfo(tUartComManager* pComManager, tComFrameParams* pFI)
{
	if(0 == pComManager) return;
	if(0 == pFI) pFI = pComManager->pCurFrameInfo;
	if(0 == pFI) return;

	if(pComManager->pCurFrameInfo == pFI) // S'il s'agit du Release du FrameInfo en cours :
	{
		pComManager->curTxBufInfo.pBufBase = 0;
		pComManager->pCurFrameInfo = 0;
		pComManager->canTxNow = 0;
	}

	tBusyFree* pBF = UartCom_getBusyFreeInBufManagers(pFI->pBufBase, &(pComManager->bufMgr));

	// Libère le FrameInfo & le Bloc dans le BufferManager :
	UartCom_UnlockFrameInfoAndBF(pFI, pBF);
}

/****************************************************************************/

static tBusyFree* UartCom_getBusyFreeInBufManagers(void* pBuf, tBufManager* pBM)
{
	tBusyFree* pBF = 0;
	if(0 != pBM) { pBF = UartCom_getBusyFreeInBufManager(pBuf, pBM); }	// Recherche tout d'abord dans le BufManager suggéré
	if(0 == pBF) { pBF = UartCom_getBusyFreeInBufManager(pBuf, &shrdBufTxMgr); }	// Puis dans le BufManager partagé
	return pBF;
}

/****************************************************************************/

eUartComErrorContext UartComGetUartErrorContext(UART_HandleTypeDef *hUart) // hUart de type UART_HandleTypeDef obligatoire !
{
	// Déterminer si c'est une Erreur de Tx ou de Rx :
	uint32_t state = HAL_UART_STATE_BUSY_TX_RX & HAL_UART_GetState(hUart);
//	if(hUart->TxXferSize > 0) { state |= HAL_UART_STATE_BUSY_TX; }
//	if(hUart->RxXferSize > 0) { state |= HAL_UART_STATE_BUSY_RX; }
	if(state <= HAL_UART_STATE_READY)	// Aucun des Flags attendus n'est vraiment apparu :
	{
		state = HAL_UART_STATE_BUSY_TX_RX; // => On part du principe que ça peut être les 2 !
	}
	return ErrorContextTxRx & state;
}

/****************************************************************************/

void HAL_UART_ErrorCallback(UART_HandleTypeDef *hUart)	// Peut être appelé en (void*) pour n'importe quel Handle supporté
{
#define IS_ALL_FLAGS_PRESENT(a,f)	(((a) & (f)) == (f))

	// Prépare la Gestion :
	tUartComManager* pComManager = UartCom_GetManagerFromHandle(hUart);
	if(0 == pComManager) // Handle pas dans la Liste => Impossible de déterminer le Manager associé :
	{
#ifdef NEXT_HAL_UART_ErrorCallback	// cf. "UartComConf.h"
		NEXT_HAL_UART_ErrorCallback(hUart);	// ATTENTION : Fonction Non Testée !
#endif // NEXT_HAL_UART_ErrorCallback
		return;
	}
	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !
	tUartComClassFn* pClassFn = (tUartComClassFn*)pInitParam->pClassFn;
	pUartCom_CntxtFn pFnErrContext = 0;
	eUartComErrorContext errContext = ErrorContextUnknown;

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrCallBack) { pComManager->nbErrCallBack++; }
#endif // UART_COM_SUPPORT_STATS

	if(0 != pClassFn) { pFnErrContext = pClassFn->pFnErrContext; }
#ifdef UART_COM_USR_FN_GET_ERR_CNTXT
	if(0 == pFnErrContext) { pFnErrContext = UART_COM_USR_FN_GET_ERR_CNTXT; }
#endif // UART_COM_USR_FN_GET_ERR_CNTXT
	if(0 != pFnErrContext)
	{
		errContext = pFnErrContext(hUart); // Appel de la Fonction correspondante
	}

	if(ErrorContextUnknown == errContext)
	{
		errContext = ErrorContextTxRx;

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrXxCallBack) { pComManager->nbErrXxCallBack++; }
#endif // UART_COM_SUPPORT_STATS
	}
	pComManager->sabReSetRxBufPtr = pInitParam->sabReSetRxBufPtr; // Dans tous les cas : Recharge le Sablier de reconfiguration de la Réception

	// Traite spécialement si c'est une erreur de TX :
	if(IS_ALL_FLAGS_PRESENT(errContext, ErrorContextTxOnly))
	{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrTxCallBack) { pComManager->nbErrTxCallBack++; }
#endif // UART_COM_SUPPORT_STATS

		UartCom_HandleTxError(pComManager);
	}

	// Traite spécialement si c'est une erreur de RX :
	if(IS_ALL_FLAGS_PRESENT(errContext, ErrorContextRxOnly))
	{
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrRxCallBack) { pComManager->nbErrRxCallBack++; }

	// Répartition si 1ere Erreur de cette Frame ou pas :
	if(UINT32_MAX > pComManager->nbErrRxCbDscrd[pComManager->mayDiscardRx])
	{
		pComManager->nbErrRxCbDscrd[pComManager->mayDiscardRx]++;
	}

#endif // UART_COM_SUPPORT_STATS

		pUartCom_CtrlFn pCtrlFn = 0;
		if(0 != pClassFn) { pCtrlFn = pClassFn->pFnAbortReceive; }
#ifdef UART_COM_USR_FN_ABORT_RECEIVE
		if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_ABORT_RECEIVE; }
#endif // UART_COM_USR_FN_ABORT_RECEIVE
		if(0 != pCtrlFn)
		{
			pCtrlFn(hUart); // Appel de la Fonction configurée
		}
		pComManager->mayDiscardRx		= 1;   // Il faudra ignorer cette Trame
		pComManager->sabEndOfRxFrame	= pInitParam->sabErrorRxFrame;	// Charge la Tempo d'erreur
		// ToDo : voir s'il faut pê aussi une Tempo d'interdiction d'envoi ?
	}
}

/****************************************************************************/

void UartCom_HandleTxError(tUartComManager* pComManager) // pê toujours appelé dans une Interruption ?
{
	if(0 == pComManager) return;

	tUartComInitParams* pInitParam = pComManager->pInitParams;
	if(0 == pInitParam)	return;	// Infos d'Init non disponibles => Impossible de déterminer la Config associée !
	tUartComClassFn* pClassFn = (tUartComClassFn*)pInitParam->pClassFn;
	pUartCom_CtrlFn pCtrlFn = 0;

#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrTxHandled) { pComManager->nbErrTxHandled++; }
#endif // UART_COM_SUPPORT_STATS

	if(0 != pClassFn) { pCtrlFn = pClassFn->pFnAbortTransmit; }
#ifdef UART_COM_USR_FN_ABORT_TRANSMIT
	if(0 == pCtrlFn) { pCtrlFn = UART_COM_USR_FN_ABORT_TRANSMIT; }
#endif // UART_COM_USR_FN_ABORT_TRANSMIT
	if(0 != pCtrlFn)
	{
		pCtrlFn(pInitParam->hHandle); // Appel de la Fonction configurée
	}

	tComFrameParams* pFI = pComManager->pCurFrameInfo;
	if(0 != pFI) // Si on a toujours les infos pour Relancer :
	{
		if( (pFI->nbReTries > 0) && (pFI->time2Live != COM_FRAME_TTL_EXPIRED) )
		{ // Re-Configurer le BufInfo "bas niveau" :
// pê pas besoin ?	pComManager->lockTxIt = 1;	// Empêcher pour le moment l'envoi immédiat par Interruption
			pComManager->curTxBufInfo.nbBytes = pFI->nbBytes;
			pComManager->curTxBufInfo.pBufBase = pFI->pBufBase;
// pê pas besoin ?	pComManager->lockTxIt = 0;	// Autorise maintenant les envois complémentaires par Interruption
			pFI->nbReTries--;
			pComManager->sabMayTxNextByte = pInitParam->sabErrorTxRetry;
			// apparemment, c'est aussi fait dans la partie en commun ! pComManager->canTxNow = 0;
#ifdef UART_COM_SUPPORT_STATS	// cf. "UartComConf.h"
	if(UINT32_MAX > pComManager->nbErrTxRetries) { pComManager->nbErrTxRetries++; }
#endif // UART_COM_SUPPORT_STATS

		} else {
			pFI = 0;
		}
	}

	if(0 == pFI) // S'il n'y a plus les infos ou inutiles :
	{
		pComManager->curTxBufInfo.nbBytes = 0;		// Simule un envoi Complètement Terminé
#ifdef UART_COM_SUPPORT_TX_RX_PIN	// cf. "UartComConf.h"
		UartCom_Manage_PinDirection(pInitParam, 0);	// Met à jour la Pin de fin de Tx
#endif // UART_COM_SUPPORT_TX_RX_PIN
// Laisser faire au PP !		ReleaseCurFrameInfo(pComManager);				// Simule un envoi Complètement Terminé
		pComManager->sabMayTxNextByte = pInitParam->sabErrorTxFrame;
	}
	pComManager->sabReady4Tx = pComManager->sabMayTxNextByte;
	pComManager->canTxNow = 0;
// Test_Jp le 17/05/2021	pComManager->pChkTxEcho = 0; // Annuler l'éventuelle Vérification de l'echo en cours
}

/****************************************************************************/

uint16_t UartCom_GetMaxAllowedTxBlocSize(tUartComInitParams* pInitParam, uint16_t FrameSize, uint16_t Flags)
{
	if(0 == pInitParam) return 0;
	if(0 == FrameSize) return 0;

	// Regarder si on doit respecter une certaine Synchro :
	if( (0 == (Flags & UART_COM_FLAG_DISCARD_TX_SYNC)) && (0 != pInitParam->flag.maySyncTx) )
	{
		return 0; // Pas maintenant : attendre la Synchro !
	}

	// Regarder si un Echo est attendu :
	if( (0 == (Flags & UART_COM_FLAG_DISCARD_TX_ECHO)) && (0 != pInitParam->flag.chkEcho) )
	{
		return 1; // Uniquement 1 par 1 !
	}

	// Regarder si un Intervalle de Temps est requis entre les Blocs :
	if( (0 == (Flags & UART_COM_FLAG_DISCARD_TX_SAB)) && (0 != pInitParam->sabMayTxNextByte) )
	{
		return 1; // Par défaut 1 par 1, mais si besoin : on peut ajouter et tenir compte d'un Paramètre d'Init !
	}

	// Comme rien ne s'y oppose : on pourra envoyer l'intégralité de la Trame demandée :
	return FrameSize;
}

/****************************************************************************/

uint16_t UartCom_PushBloc(void** pDest, void* pSrc, uint16_t size)
{
	// ToDo : à Vérifier ... Est-ce que pDest a bien avancé de size après la sortie de la fonction ?
	if(0 == pDest)	return 0;
	if(0 == *pDest)	return 0;
	if(0 == pSrc)	return 0;
	uint16_t nbBytes = 0;

	while(size > 0)
	{
		*(uint8_t*)(*pDest) = *(uint8_t*)pSrc;
		(*pDest)++;
		pSrc++;
		size--;
		nbBytes++;
	}
	return nbBytes;
}

/****************************************************************************/

tUartComRegularTx* UartCom_getFreeRegularTx(void)
{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)
	for(tUartComRegularTx* pRegTx = shrdRegTx; pRegTx < EndOfShrdRegTx; pRegTx++)
	{
		if(0 == pRegTx->pInitParams) return pRegTx;
	}
#endif // UART_COM_MAX_REG_TX
	return 0;
}

/****************************************************************************/

uint16_t UartCom_RegisterRegularTxFrame(tUartComInitRegularTx* pNewRegTx)
{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	if(0 == pNewRegTx) return 0; // Failure
	if(0 == pNewRegTx->hHandle) return 0; // Failure
	if(0 == pNewRegTx->pFn) return 0; // Failure
	tUartComRegularTx* pRegTx = UartCom_getFreeRegularTx();
	if(0 == pRegTx)
	{
		UART_COM_HALT_IF_DEBUG(); // Bloquer au RunTime en DEBUG si mauvaise Configuration => Recalculer UART_COM_MAX_REG_TX
		return 0; // Failure
	}

	UartCom_FillMemory((void*)pRegTx, 0, sizeof(tUartComRegularTx)); // Nettoyer notre propre Structure avant de la Configurer
	pRegTx->pInitParams = pNewRegTx;
	pRegTx->endDelay = pNewRegTx->FirstDelay; // Charge le Délai initial demandé par l'Init
	pRegTx->curDelay = 0; // Partir de 0
	return 1; // Success
#else
	return 0; // Failure
#endif // UART_COM_MAX_REG_TX
}

/****************************************************************************/

tUartComRegularTx* UartCom_getRegTxFromParams(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam)
{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	tUartComInitRegularTx* pInitRegTx;
	for(tUartComRegularTx* pRegTx = shrdRegTx; pRegTx < EndOfShrdRegTx; pRegTx++)
	{
		pInitRegTx = pRegTx->pInitParams;
		if(0 == pInitRegTx) continue;

		// Cette Structure a été associée à des Paramètres d'Init :
		if( (0 != hHandle) && (pInitRegTx->hHandle != hHandle) ) continue; // 0 pour ignorer le Handle dans la recherche
		if( (0 != pFn) && (pInitRegTx->pFn != pFn) ) continue;	// 0 pour ignorer le Pointeur de Fonction dans la recherche
		if(pInitRegTx->pVoidParam != pVoidParam) continue;		// Attention : Il n'est pas posible d'ignorer le VoidParam

		// Tout semble correspondre :
		return pRegTx;
	}
#endif // UART_COM_MAX_REG_TX
	return 0;
}

/****************************************************************************/

uint16_t UartCom_UnRegisterRegTxFrame(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam)
{
#if defined(UART_COM_MAX_REG_TX) && (UART_COM_MAX_REG_TX > 0)	// cf. "UartComConf.h"
	tUartComRegularTx* pRegTx = UartCom_getRegTxFromParams(hHandle, pFn, pVoidParam);
	if(0 == pRegTx) return 0; // Not Found !

	// On a trouvé notre élément dans la Liste :
	pRegTx->pInitParams = 0; // Détruit le lien actif !
	return 1; // Unregister done
#else // ! UART_COM_MAX_REG_TX
	return 0;
#endif // UART_COM_MAX_REG_TX
}

/****************************************************************************/

void UartCom_Devices_DeInit(void)
{
	tUartComInitParams* pInitParam;
	tUartComClassFn* pClassFn;
	pUartCom_CtrlFn pFnDeInit;
	for(tUartComManager* pComManager = mUartComManager; pComManager < EndOfUartFrame; pComManager++)
	{
		pInitParam = pComManager->pInitParams;
		if(0 == pInitParam) continue;
		if(0 == pInitParam->hHandle) continue;
		pClassFn = (tUartComClassFn*)pInitParam->pClassFn;
		pFnDeInit = 0;
		if(0 != pClassFn) { pFnDeInit = pClassFn->pFnDeInit; }
#ifdef UART_COM_USR_FN_DEINIT
		if(0 == pFnDeInit) { pFnDeInit = UART_COM_USR_FN_DEINIT; }
#endif // UART_COM_USR_FN_DEINIT
		if(0 != pFnDeInit) { pFnDeInit(pInitParam->hHandle); }
		pComManager->isLoaded = 0;
	}
}

/******************************************************************************/

#ifndef UTILS_UTILS_H_	// en cas d'absence de "utils.h" :
inline void UartCom_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt)
{
	while(BytesCt-- > 0) { *pDest++ = Value; }
}
#endif // UTILS_UTILS_H_

/******************************************************************************/

#ifndef UTILS_UTILS_H_	// en cas d'absence de "utils.h" :
void inline UartCom_CopyMemory(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt)
{
	while(BytesCt-- > 0) { *pDest++ = *pSrc++; }
}
#endif // UTILS_UTILS_H_

/******************************************************************************/

#ifdef NEXT_HAL_UART_RxCpltCallback
__attribute__((weak)) void NEXT_HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUart)
{
  /* NOTE : This function should not be modified, when the callback is needed,
			the HAL_UART_RxCpltCallback2 could be implemented in the user file
   */
	UNUSED(0);	// Nothing to do here !
}
#endif // NEXT_HAL_UART_RxCpltCallback

/******************************************************************************/

#ifdef NEXT_HAL_UART_TxCpltCallback
__attribute__((weak)) void NEXT_HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUart)
{
  /* NOTE : This function should not be modified, when the callback is needed,
			the HAL_UART_TxCpltCallback2 could be implemented in the user file
   */
	UNUSED(0);	// Nothing to do here !
}
#endif // NEXT_HAL_UART_TxCpltCallback

/******************************************************************************/

#ifdef NEXT_HAL_UART_ErrorCallback
__attribute__((weak)) void NEXT_HAL_UART_ErrorCallback(UART_HandleTypeDef *hUart)
{
  /* NOTE : This function should not be modified, when the callback is needed,
			the HAL_UART_ErrorCallback2 could be implemented in the user file
   */
	UNUSED(0);	// Nothing to do here !
}
#endif // NEXT_HAL_UART_ErrorCallback
