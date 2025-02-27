/*
 * UartComUtils.h
 *
 *  Created on: 23 mars 2021
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

#ifndef UART_COM_UARTCOMUTILS_H_
#define UART_COM_UARTCOMUTILS_H_

#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"
#include "utils.h"				// Pour accès aux éléments utiles génériques
#include "UartComDepUtils.h"	// Pour accès aux outils de base spécifiques aux dépendants de UartCom
#include "UartComUser.h"		// Pour accès aux réglages USER bas niveau USER
#include "main.h"				// Pour accès aux bons includes des HAL

/******************************************************************************/

// Définitions, Enumérations & Macros founies par le Core pour aider à la Configuration :
typedef void (*pFn_void_From_void_MX_UART_Init)(void); // Prototype standard des Fonctions d'Init UART de CubeMx

typedef enum // Liste des Protocoles actuellement suppotés :
{
	UartComDisabled	= 0, // Désactivé / aucun Protocole sélectionné / inconnu / non supporté
	UartComIbus 	= 1, // Protocole de type iBus
	UartComModbus	= 2, // Protocole de type Modbus
	UartComCustom	= 3, // Protocole Custom, par exemple "Inverter Embraco"
} eUartComType;
#define UART_COM_TYPE_MIN	UartComIbus
//#define UART_COM_TYPE_MAX	UartComModbus
#define UART_COM_TYPE_MAX	UartComCustom

typedef struct _tBufInfo
{
	void*	pBufBase;	// Pointeur sur la Base du Buffer
	uint16_t nbBytes;	// Nb de Bytes Disponibles ou Utiles
} tBufParams;

typedef struct _tSzBufInfo
{
	// Base tBufParams :
	void*	pBufBase;	// Pointeur sur la Base du Buffer
	uint16_t nbBytes;	// Nb de Bytes utilisés
	// Complément spécifique :
	uint16_t maxBytes;	// Taille du Buffer associé
} tBufSzBloc;

typedef struct _tRxTxBufInfo
{
	void*		hHandle;	// Pointeur sur la Structure *_HandleTypeDef du Périphérique concerné
	//void*		pVoidParam;	// Paramètre fourni à la Fonction de Traitement lors d'une Trame reçue
	tBufParams	RxBuf;
	tBufSzBloc	TxBuf;
} tRxTxBufInfo;

typedef struct _tFrameInfo
{
	void*	hHandle;	// Pointeur sur la Structure *_HandleTypeDef du Périphérique concerné
	// Base tBufInfo :
	void*	pBufBase;	// Pointeur sur la Base du Buffer
	uint16_t nbBytes;	// Nb de Bytes Disponibles ou Utiles
	// + Champs Optionnels :
	uint8_t	nbReTries;	// Nb de Tentatives d'envoi restantes
	uint8_t	time2Live;	// Sablier Expiration ?
} __attribute__ ((__packed__)) tComFrameParams;

#define COM_FRAME_TTL_DISABLED		0
#define COM_FRAME_TTL_EXPIRED		1
#define COM_FRAME_DEF_TTL_IBUS		50
#define COM_FRAME_DEF_TTL_MODBUS	UINT8_MAX
#define COM_FRAME_DEF_TTL_CUSTOM	UINT8_MAX

//// Mode Statique : RxIT && TxDMA :
//#define UART_COM_FN_RECEIVE 		HAL_UART_Receive_IT
//#define UART_COM_FN_TRANSMIT		HAL_UART_Transmit_DMA
//#define UART_COM_FN_ABORT_RECEIVE	HAL_UART_AbortReceive_IT
//#define UART_COM_FN_DE_INIT			HAL_UART_DeInit

typedef enum
{
	ErrorContextUnknown = 0,
	ErrorContextTxOnly = (1 << 0),	// bits[1:0] de HAL_UART_STATE_BUSY_TX = 1
	ErrorContextRxOnly = (1 << 1),	// bits[1:0] de HAL_UART_STATE_BUSY_RX = 2
	ErrorContextTxRx = ErrorContextRxOnly | ErrorContextTxOnly
} eUartComErrorContext;

// Remarque_Jp : "UART_HandleTypeDef*" a été remplacé par "void*" pour être aussi compatible "USBD_HandleTypeDef" & "USBH_HandleTypeDef"
typedef HAL_StatusTypeDef (*pUartCom_IoFn)(void* hHandleTypeDef, uint8_t *pData, uint16_t Size);
typedef HAL_StatusTypeDef (*pUartCom_CtrlFn)(void* HandleTypeDef);
typedef eUartComErrorContext (*pUartCom_CntxtFn)(void* hHandleTypeDef);

typedef int (*pUartCom_RxHandler)(tRxTxBufInfo* pRxTxBI, void* pVoidParam);
typedef uint16_t (*pUartCom_TxRegular)(tComFrameParams* pFI, void* pVoidParam);

// Mode configurable, à tester en paralèle avec les fonctions USB ?
typedef struct _tUartComClassFn
{
	const char	*Name;
	pUartCom_IoFn pFnStartTransmit;		// Fonction pour Initier un Envoi
	pUartCom_IoFn pFnStartReceive;		// Fonction pour Configurer une Réception
	pUartCom_CntxtFn pFnErrContext;		// Fonction pour déterminer si on est en Rx et/ou en TX
	pUartCom_CtrlFn pFnAbortTransmit;	// Fonction pour Abandonner un Envoi en cours
	pUartCom_CtrlFn pFnAbortReceive;	// Fonction pour Abandonner une Réception en cours ou à venir
	pUartCom_CtrlFn pFnDeInit;			// Fonction pour DéConfigurer le Périphérique
} tUartComClassFn;

extern const tUartComClassFn UartCom_TxDMA_RxIT;	// Pour le Modbus et chaque fois que possible
extern const tUartComClassFn UartCom_TxDMA_RxDMA;	// Pour Tests RxDMA
extern const tUartComClassFn UartCom_TxIT_RxIT; 	// Pour l'iBus esentiellement
#define UART_COM_CLASS_TX_DMA_RX_IT 	&UartCom_TxDMA_RxIT 	// Pour DMA & IT
#define UART_COM_CLASS_TX_DMA_RX_DMA	&UartCom_TxDMA_RxDMA	// Pour DMA & DMA
#define UART_COM_CLASS_TX_IT_RX_IT  	&UartCom_TxIT_RxIT  	// Pour IT & IT

typedef struct _tUartComInitRegularTx
{
	void*	hHandle;		// Doit être un Pointeur UART_HandleTypeDef valide
	pUartCom_TxRegular pFn;	// Doit être un Pointeur vers une fonction de type pUartCom_TxRegular
	void*	pVoidParam;		// Paramètre transmis lors de l'appel à la fonction configurée ci-dessus
	uint16_t FirstDelay;	// Temporisation avant le 1er appel pour Tx (base = RT @ 100ms)
	uint16_t NormalDelay;	// Intervalle entre chaque appel pour Tx (base = RT @ 100ms)
	uint16_t DefFrameSize;	// Taille par défaut à réserver pour cette Transmission
} tUartComInitRegularTx;
#define PVOID_INT(a)		((void*)(a))	// Pour mettre une valeur dans pVoidParam
#define INT_FROM_PVOID(a)	((int)(a))		// Pour Récupérer une valeur de pVoidParam
#define UART_COM_MAKE_REG_TX_FN_PARAM(f,p)	(f), PVOID_INT(p)

// Pour Mémo, Fonctions HAL appelées :
//HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
//HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
//HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)
// -> devrait remplacer l'actuel void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)

// Pour Mémo, Fonctions HAL non appelées :
//HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
//HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
//HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength)
//HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart)

// Pour Mémo, Fonctions HAL interceptées (Callbacks) :
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)

typedef union _tUartComInitFlags
{
	struct {
		unsigned loadMst:1;
		unsigned chkEcho:1;
		unsigned maySyncTx:1;
		unsigned replySync:1;
		unsigned onTxPin:1;		// GPIO_PinState pour passer (ou signaler) le mode Transmission
	};
	uint8_t All;
} __attribute__ ((__packed__)) tUartComInitFlags;

typedef struct _tUartComInitParams
{
	eUartComType ProtocolType;	// Type de Communication
	void *hHandle;	// Pointeur sur la Structure *_HandleTypeDef
	pFn_void_From_void_MX_UART_Init pFnInit; // Pointer sur la Fonction d'Init CubeMx ou respectant le même Prototype
	tBufParams RxInfos;			// Base & taille du Buffer de Réception
	tBufParams TxInfos;			// Base & taille du Buffer de Transmission

	pUartCom_RxHandler pFnRxHandler; // Pointeur sur la Fonction de Traitement d'une Trame reçue
	void*	pVoidParam;			// Paramètre fourni à la Fonction de Traitement lors d'une Trame reçue

	GPIO_TypeDef* GPIOx;		// Pointeur sur la Structure GPIO de la Pin à manoeuvrer pour passer en Tx
	uint16_t GPIO_Pin;			// Pin à manoeuvrer pour signaler le mode Transmission

	uint16_t sabEndOfRxFrame;	// Délai spécifique avant Fin de Trame (dépend notamment du Protocole, base = IT @ 1ms)
	uint16_t sabEndOfRxBloc;	// Délai spécifique avant TimeOut du Bloc suivant pour Fin de Trame
	uint16_t sabTimeOut4Reply;	// Délai maximal autorisé pour transmettre une Réponse immédiatement (base = IT @ 1ms)
	uint16_t sabInitNoTx;		// Délai Initial avant de pouvoir commencer à Emettre (base = IT @ 1ms)
	uint16_t sabReady4TxFrame;	// Délai à attendre avant de pouvoir à nouveau commencer à Emettre (base = IT @ 1ms)
	uint16_t sabMayTxNextByte;	// Délai à insérer entre chaque Byte d'envoi normal (base = IT @ 1ms)

	uint16_t sabReSetRxBufPtr;	// Délai maximal autorisé sans Réception avant Relance Interruption (base = IT @ 100ms)

	uint8_t  minRxFrameSize;	// Nb minimum de Bytes nécessaires pour déclencher le RxHandler
	uint8_t  minTxFrameSize;	// Taille Minimale d'une Trame à Envoyer

	uint16_t maxRxPacketSize;	// Taille Maximale d'un Bloc de Data à recevoir en une seule fois

	uint16_t defTxReplySize;	// Taille par défaut à allouer pour Répondre à une trame

	uint16_t sabErrorRxFrame;	// Temporisation d'attente avant de reprendre une Réception normale (base = IT @ 1ms)
	uint16_t sabErrorTxRetry;	// Temporisation d'attente avant un nouvel essai de renvoi de la Trame (base = IT @ 1ms)
	uint16_t sabErrorTxFrame;	// Temporisation d'attente après echec d'envoi d'une Trame (base = IT @ 1ms)

	tUartComInitFlags flag;
	const tUartComClassFn* pClassFn;
} tUartComInitParams;

// Pour RéInit UART = Changement de BaudRate / Parity & Stop (Ajout_Jp le 16/04/2024 pour Ticket #33) :

typedef union _UartReInitFlags
{
	struct {
		unsigned InitRS485Ex:1;
		unsigned InitDefault:1;
		unsigned CanValidate:1;
	};
	uint16_t AllFlags;
} UartReInitFlags;

typedef struct _UartReInitCoreVars
{
	UartReInitFlags ReInitFlags;
	uint16_t SabApply;
} UartReInitCoreVars;

typedef struct _UartReInitUserParams
{
	uint16_t BaudRateDiv100; // Parmi eUartReInitBaudRate
	uint8_t Parity;			 // Parmi eUartReInitParity
	uint8_t nbStops;		 // Parmi eUartReInitStop
	UART_AdvFeatureInitTypeDef* AdvancedInit;
} UartReInitUserParams;

// "The standard baud rates are the following: 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600."
// "CoolTerm Connection Options : 300, 600, 1200, 1800, 2400, 3600, 4800, 7200, 9600, 14400, 19200, 28800, 38400, 57600, 115200, 230400"
// "RealTerm, a freeware Windows terminal program, lists these UART rates in its Baud menu:
// 110, 150, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600"
typedef enum
{
	eUartReInitBaud300	  =    3,	//	  300 bauds (pas demandé, pas testé)
	eUartReInitBaud600	  =    6,	//	  600 bauds (pas demandé, pas testé)
	eUartReInitBaud1200	  =	  12,	//	 1200 bauds (pas demandé, pas testé)
	eUartReInitBaud1800	  =	  18,	//	 1800 bauds (pas demandé, pas testé)
	eUartReInitBaud2400	  =	  24,	//	 2400 bauds (pas demandé, pas testé)
	eUartReInitBaud3600	  =	  36,	//	 3600 bauds (pas demandé, pas testé)
	eUartReInitBaud4800	  =   48,	//   4800 bauds
	eUartReInitBaud7200	  =	  72,	//	 7200 bauds (pas demandé, pas testé)
	eUartReInitBaud9600	  =   96,	//   9600 bauds
	eUartReInitBaud14400  =	 144,	//	14400 bauds (pas demandé, pas testé)
	eUartReInitBaud19200  =  192,	//  19200 bauds
	eUartReInitBaud28800  =  288,	//	28800 bauds (pas demandé, pas testé)
	eUartReInitBaud38400  =  384,	//  38400 bauds
	eUartReInitBaud57600  =  576,	//	57600 bauds
	eUartReInitBaud115200 = 1152,	// 115200 bauds
	eUartReInitBaud230400 = 2304,	// 230400 bauds (pas demandé)
	eUartReInitBaud460800 = 4608,	// 460800 bauds (pas demandé)
	eUartReInitBaud921600 = 9216,	// 921600 bauds (pas demandé, Rx&Tx OK mais erreurs de réception PC sur Nükub au 12/04/2024)
	eUartReInitBaud2Def	  = 0,
//	eUartReInitBaudDefault = eUartReInitBaud9600,	// Default is 9600 bauds
	eUartReInitBaudDefault = eUartReInitBaud115200,	// Default is 115200 bauds
} eUartReInitBaudRate;

typedef enum
{
	eUartReInitParityNone = 0,
	eUartReInitParityOdd  = 1,
	eUartReInitParityEven = 2,
//	eUartReInitParityMark = 3,	not supported by ST !
//	eUartReInitParitySpace= 4,	not supported by ST !
	eUartReInitParity2Def = -1,	// Demande à revenir à la Parité par défaut
	eUartReInitParityDefault = eUartReInitParityNone,
} eUartReInitParity;

typedef enum
{
	eUartReInitStopOne	= 1,
	eUartReInitStopTwo	= 2,
	eUartReInitStop1_5	= 3, // 1.5 Stop is supported by ST (mais pas testé)
	eUartReInitStop2Def	= 0,
	eUartReInitStopDefault = eUartReInitStopOne,
} eUartReInitStop;

typedef struct
{
	UART_HandleTypeDef* huart;
	USART_TypeDef* Instance;
	UartReInitUserParams* pUserParams;
	UartReInitCoreVars* pCoreVars;
	UartReInitUserParams* pRealParams;
	const UartReInitUserParams* pDefParams;
} UartReInitItem;

extern const UART_AdvFeatureInitTypeDef UART_AdvInitTxInvRxInv;
extern const UartReInitUserParams UartReInit9600N1;
extern const UartReInitUserParams UartReInit115200N1;	// 115200, Parity_None, 1_Stop
extern const UartReInitUserParams UartReInitDefaults;
HAL_StatusTypeDef UartCom_ReInitUartFromUserParams(UartReInitUserParams* pUserParams);
HAL_StatusTypeDef UartCom_ReInitUartWithCustomParams(UartReInitItem* pReInitItem);

#define UART_COM_MAKE_STRUCT_BUF(n,r,t)	typedef struct _##n { uint8_t RxBuf[r]; uint8_t TxBuf[t]; } n
#define UART_COM_STRUCT_BUF_INFOS(n)	{ (void*)n.RxBuf, (uint16_t)sizeof(n.RxBuf) },	{ (void*)n.TxBuf, (uint16_t)sizeof(n.TxBuf) }
#define UART_COM_MAKE_VAR_ARRAY(s,n,c)	s n[c]
#define UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(s,r,t,n,c)	UART_COM_MAKE_STRUCT_BUF(s,r,t); UART_COM_MAKE_VAR_ARRAY(s,n,c)

#define UART_MAKE_REINIT_ITEM(h,i,u,c,d)	{ h, i, &u, &c, &d }
#define UART_MAKE_VAR_AND_CAST_VALUE(t,n,v)	t n = (t)v
#define UART_MAKE_FOR_VAR_FROM_TO(t,v,b,e)	for(UART_MAKE_VAR_AND_CAST_VALUE(t,v,b); v < (t)e; v++)

#define UART_COM_GPIO_TX_NONE			0,0 // Pour renseigner GPIOx = 0 ET GPIO_Pin = 0

//#define UART_COM_MAKE_CONST_END_OF_TABLE(t,n,b)	const t* n = ((void*)b) + (sizeof(b))
//#define UART_COM_GET_NB_OF_ITEMS(s)				(sizeof(s) / sizeof(s[0]))		// Pour une valeur figée à la Compilation
//#define UART_COM_NB_OF_STRUCT_ITEMS(t,e,b)		(((void*)e - (void*)b) / sizeof(t))
//#define UART_COM_MAKE_CONST_NB_OF_ITEMS(t,n,a)	const t n = UART_COM_GET_NB_OF_ITEMS(a)	// S'il est suffisant de savoir au RunTime

//#define UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = ((void*)a) + (sizeof(a))
//#define UART_COM_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(t,b,e)	extern const t* b; extern const t* e

#define UartComPushDataByte(ptr,data)		(*(unsigned char*)ptr++) = ((data) & 0xFF)
#define UartComPushStructByte(ptr,data)		(*(unsigned char*)ptr++) = ((*(unsigned char*)&data) & 0xFF)
#define UartComPushDataUIntLE(ptr,data)		UartComPushDataByte(ptr, (data >> 0)); UartComPushDataByte(ptr, (data >> 8)) // UInt16 LittleEndian
#define UartComPushDataUIntBE(ptr,data)		UartComPushDataByte(ptr, (data >> 8)); UartComPushDataByte(ptr, (data >> 0)) // UInt16 BigEndian
#define UartComPushDataULongLE(ptr,data)	UartComPushDataByte(ptr, (data >> 0)); UartComPushDataByte(ptr, (data >> 8)); UartComPushDataByte(ptr, (data >> 16)); UartComPushDataByte(ptr, (data >> 24))
#define UartComPushDataULongBE(ptr,data)	UartComPushDataByte(ptr, (data >> 24)); UartComPushDataByte(ptr, (data >> 16)); UartComPushDataByte(ptr, (data >> 8)); UartComPushDataByte(ptr, (data >> 0))

// Le Format Natif de nos µC, c'est en LittleEndian :
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define UartComPushDataUInt(ptr,data)	UartComPushDataUIntLE(ptr,data)
	#define UartComPushDataULong(ptr,data)	UartComPushDataULongLE(ptr,data)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define UartComPushDataUInt(ptr,data)	UartComPushDataUIntBE(ptr,data)
	#define UartComPushDataULong(ptr,data)	UartComPushDataULongBE(ptr,data)
#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
	#warning "__BYTE_ORDER__ == __ORDER_PDP_ENDIAN__"
#endif //__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

uint16_t UartCom_Register_InitParam(tUartComInitParams* pNewInitParam, void* pNullParam);
uint16_t UartCom_UnRegister_Handle(void* hUart);
#define UART_COM_REGISTER_INIT_PARAM(a)	UartCom_Register_InitParam((tUartComInitParams*)&a,0)

void UartCom_Register_LastRxFramePtr(void* hUart, uint32_t* pLastRxFrame);

//void UartComReInitWithCleanParams(void* hUart, uint8_t** pRxBuf);
void* UartCom_ReLoad_WithSameParams(void* hUart);
void UartCom_RequestDelay4Reply(void* hUart, uint16_t newTimeOut4Reply);
void UartCom_Discard_Handle(void* hUart);
void UartCom_Handle_ReceivedBloc(void* hUart, uint8_t* Buf, uint16_t Len);
#define UartCom_TxCpltCallback(hHandle)	HAL_UART_TxCpltCallback((void*) hHandle)

// OtherMisc :
#define UART_COM_IS_IN_RANGE(val,valMin,valMax)  ( ((val) >= (valMin)) && ((val) <= (valMax)) )

// Fonctions par défaut en cas de Classe nulle ou Pointeur de Fonction non renseigné :
#define UART_COM_DEF_FN_START_TRANSMIT	(pUartCom_IoFn)HAL_UART_Transmit_DMA		// pour HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
#define UART_COM_DEF_FN_START_RECEIVE	(pUartCom_IoFn)HAL_UART_Receive_IT			// pour HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
#define UART_COM_DEF_FN_GET_ERR_CNTXT	(pUartCom_CntxtFn)UartComGetUartErrorContext// pour eUartComErrorContext UartComGetUartErrorContext(UART_HandleTypeDef *hUart)
#define UART_COM_DEF_FN_ABORT_TRANSMIT	(pUartCom_CtrlFn)HAL_UART_AbortTransmit_IT	// pour HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart)
#define UART_COM_DEF_FN_ABORT_RECEIVE	(pUartCom_CtrlFn)HAL_UART_AbortReceive_IT	// pour HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart)
#define UART_COM_DEF_FN_DEINIT			(pUartCom_CtrlFn)HAL_UART_DeInit 			// pour HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)

tComFrameParams* UartCom_InitNewTxFrame(void* hHandle, uint16_t maxBytes);
tComFrameParams* UartCom_RequestLargerBuffer(tComFrameParams* pCurFI, uint16_t maxBytes);

uint16_t UartCom_PostFrame(tComFrameParams* pFI);
uint16_t UartCom_PushBloc(void** pDest, void* pSrc, uint16_t size);

uint16_t UartCom_RegisterRegularTxFrame(tUartComInitRegularTx* pNewRegTx);
uint16_t UartCom_UnRegisterRegTxFrame(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam);
uint16_t UartCom_SetRegTx_CurEndDelay(void* hHandle, pUartCom_TxRegular pFn, void* pVoidParam, int16_t newEndDelay, int16_t newCurDelay);
//uint16_t UartComSetThisRegTxCurDelays(void* pThisRegTx, int16_t newEndDelay, int16_t newCurDelay);
#define UART_COM_NO_SET_REG_TX_DELAY	(-1)
#define UART_COM_SET_DEF_REG_TX_DELAY	(-2)
#define UART_COM_SET_FIRST_REG_TX_DELAY	(-3)

typedef enum {
	UartComActivityNone = 0,	// Non reconnu / non chargé
	UartComActivityReady = 1,	// Chargé, prêt à travailler, mais pas d'activité récente
	UartComActivityRecent = 2,	// Activité récente relevée
} eUartComActivity;
eUartComActivity UartCom_GetRecentActivity(void* hHandle, uint32_t seuilRecent);
uint16_t getThisHandleRxBufSize(void *hHandle);

/******************************************************************************/
// Links to utils.h :

#define UartCom_FillMemory	FillMemory
#define UartCom_CopyMemory	CopyMemory
void UartCom_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void UartCom_CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt);
//void UartCom_CopyMemory(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt);

#endif /* UART_COM_UARTCOMUTILS_H_ */
