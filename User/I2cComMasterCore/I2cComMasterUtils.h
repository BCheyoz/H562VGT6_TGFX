/*
 * I2cComMasterUtils.h
 *
 *  Created on: 29 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 16 Nov. 2022
 *  Updated by: j.proux
 *
 *  Feature : Outils de support I2cComMaster
 *
 */

#ifndef USER_I2CCOMMASTER_I2CCOMMASTERUTILS_H_
#define USER_I2CCOMMASTER_I2CCOMMASTERUTILS_H_

// Pour accès automatique aux HAL :
#include "main.h"				// Pour accès aux bons includes des HAL

// En cas de besoin d'accès à des HAL spécifiques :
//#include "stm32f0xx_hal.h"  	// Pour accès aux HAL spécifiques STM32 F0

#include "utils.h"				// Pour accès aux éléments utiles génériques, dont "int*_t" &"uint*_t"

#include "I2cComMasterConf.h"	// Pour accès à la Configuration des Capteurs supportés

#ifdef I2CCM_USE_I2C_CUSTOM_DRIVER // Si on utilise le Driver I2C custom ALDES "I2cSoftComPort" à la place des HAL (cf. Himalaya1_Carte6 App v1.0.4)
	#include "I2cSoftComPort.h" 	// Pour accès aux Fonctions bas-niveau du Driver custom ALDES "I2cSoftComPort" à la place des HAL_I2C
#endif // I2CCM_USE_I2C_CUSTOM_DRIVER

/******************************************************************************/
// Définitions, Paramètres & Structures pour la Configuration de la Table d'Init :

// Charger un Device en fonction des recommandations de la Librairie concernée :
#define I2CCM_LOAD_DEFAULT_DEVICE	(-1)
#define I2CCM_LOAD_DEFAULT_DEVICE2	(-2)
#define I2CCM_LOAD_DEFAULT_DEVICE3	(-3)
#define I2CCM_LOAD_DEFAULT_DEVICE4	(-4)

#define I2CCM_DEV_GROUP_UNKNOWN		0	// Valeur de base des Enums I2CCM_*DeviceGroupId (avec * = Internal, External, System, ou autre)
#define I2CCM_DEV_FAMILY_UNKNOWN	0	// Valeur de base des Enums I2CCM_Device*Family  (avec * = Pressure, Co2, Cov, HR, ou autre)
#define I2CCM_DEV_FAMILY_MASK		0x7FF0	// Masque pour ne conserver que la I2CCM_Device*Family

#define I2CCM_CHANNEL_NONE			0xFF	// Pour ignorer un éventuel multiplexage I2C

#define I2CCM_SET_PIN_STATE(g,p,s)	HAL_GPIO_WritePin(g,p,s)	// Pour faire le lien avec la "vraie" fonction bas niveau correspondante

typedef struct
{
	uint16_t msBefore;
	uint16_t msAfter;
} I2CCM_AssertDelayParams;

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint16_t ActivePinState; // Parmi GPIO_PinState
	I2CCM_AssertDelayParams AssertDelay[2]; // 0 = DeAssertDelays (IDLE); 1 = AssertDelays (Active)
} I2CCM_AssertPinParams;
#define I2CCM_DO_ASSERT_PIN 	1
#define I2CCM_DO_DEASSERT_PIN	0

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint16_t usAfterWritePin;
	uint16_t usAfterWriteMode;
	uint16_t usBeforeReadPin;
} I2CCM_Soft_ComPin;

typedef struct {
	I2CCM_Soft_ComPin pinSDA;
	I2CCM_Soft_ComPin pinSCL;
} I2CCM_Soft_ComPort;

typedef struct _I2CCM_DevInitParams __I2CCM_DevInitParams;
typedef struct _I2CCM_Device __I2CCM_Device;
typedef int16_t (*pFn_Int16_From_I2CCM_Dev_DIP)(__I2CCM_Device* pDevice, __I2CCM_DevInitParams* pInitParams);

typedef struct _I2CCM_DevInitParams
{
	uint8_t deviceGroup;	// Référence du Groupe auquel appartient ce Device : 1 seul Device par Groupe en même temps
	uint8_t channel2Set;	// Indice du Channel à demander pour accéder au Device (si Extender)
	int16_t device2Load;	// Adresse I2C spécifique ou I2C_COM_MASTER_LOAD_DEFAULT_DEVICE*
	pFn_Int16_From_I2CCM_Dev_DIP pFnInit;	// Fonction à appeler pour Charger un Device
	uint16_t devHandler;	// CodeIdentifiant de la Famille/Library qui est prévue pour supporter le Device
	uint16_t intVarId;		// Index d'accès dans la Structure des Variables Interne (en cas de multiple Devices identiques à gérer en parallèle)
	void*   pExtStruct;		// Pointeur vers la Structure Publique de Sortie & Valeurs d'Entrée
	I2CCM_AssertPinParams* pAssertParams; // Pointeur vers un ou plusieurs éléments d'information pour l'Assertion de GPIO pin
	void*	pUserParam;		// Pointeur à usage général mis à disposition de l'Utilisateur
} __attribute__ ((__packed__)) I2CCM_DevInitParams;

/******************************************************************************/
// Définitions, Enums & Structures pour le DeviceManager (Internal, External, System, ou autre) :

typedef uint16_t (*pFn_UInt16_From_I2CCM_Device)(__I2CCM_Device *pDevice);
typedef struct _I2CCM_Device
{
	uint16_t DevAddr8;	// The device 7 bits address value already shifted to the left
	uint16_t ActionId;
	uint16_t FrameId;	// Pour les ActionId décomposées en plusieurs étapes
	uint8_t *pTxBuf;	// Pointeur sur le Buffer spécifique affecté à l'envoi des Trames
	uint8_t *pRxBuf;	// Pointeur sur le Buffer spécifique affecté à la réception des Datas
	void    *pIntData;	// Pointeur sur les Variables Internes réservées au Module Gestion
	void    *pExtData;	// Pointeur sur les Variables Externes publiquement accessibles dans les autres parties du code
	void	*pUserParam;// Pointeur à usage général mis à disposition de l'Utilisateur
	I2CCM_AssertPinParams* pAssertParams; // Pointeur vers un ou plusieurs éléments d'information pour l'Assertion de GPIO pin
	pFn_UInt16_From_I2CCM_Device getNextActionFrame;
	pFn_UInt16_From_I2CCM_Device handleActionComplete;
	pFn_UInt16_From_I2CCM_Device handleExternalAction;	// Ajout_Jp le 15/11/2022 : Supporter d'éventuelles Requêtes Externes spéciales
	pFn_UInt16_From_I2CCM_Device handleOnDeInitDevice;
	uint8_t  nbBytes2Send;	// Taille des Datas, pointées par pTxBuf, à envoyer en Tx
	uint8_t  nbBytes2Read;	// Taille des Datas, à récupérer en RX, dans le Buffer pointé par pRxBuf
	uint8_t	 ErrorsCt;
	uint32_t DelayInMs;	// Délai en millisecondes (lié à HAL_GetTick) accordé pour effectuer certaines opérations
	int32_t  result;	// Résultat de la dernière opération I2C_Transmit ou I2C_Receive
} I2CCM_Device;

typedef enum _I2C_DeviceStatus
{
	I2cDeviceStatusNotInit = 0,
	I2cDeviceStatusLoaded,
	I2cDeviceStatusDoReLoad,
	I2cDeviceStatusLoadNext,
} I2CCM_DeviceStatus;

#define I2CCM_DEV_DEINIT_INT_DATAS	(1 << 0)
#define I2CCM_DEV_DEINIT_EXT_DATAS	(1 << 1)
#define I2CCM_DEV_DEINIT_CORE_DATAS	(1 << 2)
#define I2CCM_DEV_DEINIT_ALL_DATAS	UINT8T_MAX

typedef struct _I2C_Manage_Device
{
	I2CCM_DevInitParams* pInitDevice; // Pointeur sur les infos d'Init utilisées
	int16_t			DeviceStatus;
	uint8_t			nbQueryRestart;
//	uint8_t			nbIgnoreQryRst;
	int8_t			nbComResultOK;	// Remplacement_Jp le 11/08/2022
	uint32_t		nextActionTime;
	I2CCM_Device	Device;
} I2CCM_Manage_Device; // 1 Item for each LoadGroup

typedef struct
{
	I2CCM_Manage_Device	*pManagedDevice;
	I2CCM_DevInitParams	*pNxtInitDevice;
	int nbRestart;
} I2CCM_DevManager;

typedef struct
{
	I2CCM_Manage_Device* pDevListBase;
	I2CCM_Manage_Device* pDevListEnd;
	I2CCM_DevInitParams* pInitListBase;
	I2CCM_DevInitParams* pInitListEnd;
} I2CCM_DevManagerParams;

#define I2CCM_Void void

typedef struct
{
	uint16_t ActionId;
	uint16_t MsgParam;
	I2CCM_Void* pParam;
} I2CCM_ExternalAction;

/******************************************************************************/
// Définitions, Paramètres & Structures pour les Modules de Gestion de chaque Device :

#define I2CCM_MAKE_TYPED_PTR(t,n,v)			t* n = (t*)v
#define I2CCM_DEVICE_RECENTLY_LOADED_MSK	0x8000

typedef struct
{
	uint16_t fullStructSize;	// Taille de la Structure de Datas finale, en vue du déchargement du DeviceHandler
//	uint16_t prevXtMsg;
	uint16_t idOfDevice;	// Type Interne de DeviceHandler chargé
} __attribute__ ((__packed__)) I2CCM_InternalBaseData; // For InternalDatas, reserved within the Library
#define I2CCM_MAKE_INT_DATA_PTR_AND_FILL_BASE(t,n,d)	t* n=(t*)pDevice->pIntData;n->base.fullStructSize=sizeof(t);n->base.idOfDevice=d

typedef struct
{
	uint16_t fullStructSize;	// Taille de la Structure de Datas finale, en vue du déchargement du DeviceHandler
//	uint16_t msgFromXt;
	uint16_t idOfDevice;	// Type de DeviceHandler chargé, pour discrimination ultérieure de la Source
} __attribute__ ((__packed__)) I2CCM_ExternalBaseData;	// For ExternalDatas, accessible outside of the Library
#define I2CCM_MAKE_EXT_DATA_PTR_AND_FILL_BASE(t,n,d)	t* n=(t*)pDevice->pExtData;n->base.fullStructSize=sizeof(t);n->base.idOfDevice=d|I2CCM_DEVICE_RECENTLY_LOADED_MSK
#define I2CCM_MAKE_EXT_DATA_PTR_AND_EXIT_INIT(t,n)		t* n=(t*)pDevice->pExtData;n->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);	// Signale que la Procédure d'Init est Terminée
#define I2CCM_EXT_DATA_PTR_EXIT_INIT(n)												n->base.idOfDevice &= (~I2CCM_DEVICE_RECENTLY_LOADED_MSK);	// Signale que la Procédure d'Init est Terminée

/******************************************************************************/
// Définitions & Structures pour le calcul de Moyennes Glissantes rapides :
// "QuickMoy by Jp for AldesAeraulique © 2021"

typedef struct
{
	uint16_t maxCt;		// Pour limiter à N valeurs Int16, ne doit pas dépasser la capacité configurée
	uint16_t curIdx;	// Index de stockage de la prochaine valeur INT16
	int32_t  sumTab;	// Somme instantanée 32bits signé du Tableau des valeurs INT16 recueillies
	uint16_t nbInTab;	// Nombre de valeurs INT16 actuellement recueillies dans le Tableau
} I2CCM_MoyManager_Int16; // Doit être immédiatement suivi par un tableau de Int16 !
#define I2CCM_ADD_MOY_MANAGER_INT16(n,s)	I2CCM_MoyManager_Int16 n; int16_t n##Data[s]

typedef struct
{
	uint16_t maxCt;		// Pour limiter à N valeurs UInt16, ne doit pas dépasser la capacité configurée
	uint16_t curIdx;	// Index de stockage de la prochaine valeur UINT16
	uint32_t sumTab;	// Somme instantanée 32bits non-signée du Tableau des valeurs UINT16 recueillies
	uint16_t nbInTab;	// Nombre de valeurs UINT16 actuellement recueillies dans le Tableau
} I2CCM_MoyManager_UInt16; // Doit être immédiatement suivi par un tableau de UInt16 !
#define I2CCM_ADD_MOY_MANAGER_UINT16(n,s)	I2CCM_MoyManager_UInt16 n; uint16_t n##Data[s]

typedef struct
{
	uint16_t maxCt;		// Pour limiter à N valeurs Float, ne doit pas dépasser la capacité configurée
	uint16_t curIdx;	// Index de stockage de la prochaine valeur Float
	uint16_t nbInTab;	// Nombre de valeurs Float actuellement recueillies dans le Tableau
	float	 sumTab;	// Somme instantanée du Tableau des valeurs Float recueillies
} I2CCM_MoyManager_Float; // Doit être immédiatement suivi par un tableau de Float !
#define I2CCM_ADD_MOY_MANAGER_FLOAT(n,s)	I2CCM_MoyManager_Float n; float n##Data[s]

/******************************************************************************/
// Définitions Génériques à usage général I2C :

#define I2CCM_SIZED_OBJ_BASE(Obj)			(Obj)
#define I2CCM_SIZED_OBJ_END(Obj)			((void*)(Obj)+sizeof(Obj))
#define I2CCM_SIZED_OBJ_WITH_BASE_END(Obj)	((void*)(Obj)), ((void*)(Obj)+sizeof(Obj))

#define I2CCM_JOIN_32_32(a,b)	JOIN_32_32_BE(a,b)	// ( ((((uint64_t)a) & 0xFFFFFFFF) << 32) | ((((uint64_t)b) & 0xFFFFFFFF) << 0) )
#define I2CCM_GET_32L(a)		GET_LOW_32(a)		// (((a) >>  0) & 0xFFFFFFFF)
#define I2CCM_GET_32H(a)		GET_HIGH_32(a)		// (((a) >> 32) & 0xFFFFFFFF)

#define I2CCM_JOIN_16_16(a,b)	JOIN_16_16_BE(a,b)	// ( (((a) & 0xFFFF) << 16) | (((b) & 0xFFFF) << 0) )
#define I2CCM_GET_16L(a)		GET_LOW_16(a)		// (((a) >>  0) & 0xFFFF)
#define I2CCM_GET_16H(a)		GET_HIGH_16(a)		// (((a) >> 16) & 0xFFFF)

#define I2CCM_JOIN_8_8_BE(a,b)	JOIN_8_8_BE(a,b)	// ( (((a) & 0xFF) << 8) | (((b) & 0xFF) << 0) )
#define I2CCM_GET_8LL(a)		GET_BYTE_0(a)		// (((a) >>  0) & 0xFF)
#define I2CCM_GET_8LH(a)		GET_BYTE_1(a)		// (((a) >>  8) & 0xFF)
#define I2CCM_GET_8HL(a)		GET_BYTE_2(a)		// (((a) >> 16) & 0xFF)
#define I2CCM_GET_8HH(a)		GET_BYTE_3(a)		// (((a) >> 24) & 0xFF)
#define I2CCM_SWAP_8_8(a)		I2CCM_JOIN_8_8_BE(I2CCM_GET_8LL(a),I2CCM_GET_8LH(a))

typedef enum _I2CCM_ReadWrite
{
	I2C_WRITE	= 0x00,	// Write bit for DevAddr8 = 0
	I2C_READ	= 0x01,	// Read  bit for DevAddr8 = 1
	I2C_RW_MASK	= 0x01,	// Raed/Write bit in DevAddr8
} I2CCM_ReadWrite;

#define I2CCM_MAKE_ADR8_WITH_RW_MASK(a)	(((a) << 1) & (~I2C_RW_MASK))
#define I2CCM_GET_ADR7_FROM_MASKED(a)	(((a) & (~I2C_RW_MASK)) >> 1)

typedef enum _I2CCM_ActionType
{
	I2C_NEED_PARAM	= (1 <<  0), //    1 : Demande à récupérer des Paramètres
	I2C_WAIT_TEMPO	= (1 <<  1), //    2 : Demande à effectuer une tempo de "TimeOut"
	I2C_TRANSMIT	= (1 <<  2), //    4 : Demande l'envoi de MayTxSize bytes de pTxBuf
	I2C_RECEIVE 	= (1 <<  3), //    8 : Demande la réception vers pRxBuf
	I2C_NEW_VALUE	= (1 <<  4), //   16 : Signale qu'on a de nouvelles Datas disponibles
	I2C_END_BLOC	= (1 <<  5), //   32 : Signale la fin d'1 Bloc unitaire
	I2C_RELOAD_DEV	= (1 <<  6), //   64 : Demande à recharger ce Device
	I2C_ABORT_DEV	= (1 <<  7), //  128 : Demande à abandonner ce Device
	I2C_CHK_DEV_ERR	= (1 <<  8), //  256 : Demande la vérification des erreurs du Périphérique
	I2C_HANDLE_TX	= (1 << 10), // 1024 : Demande le CallBack after Tx
	I2C_HANDLE_RX	= (1 << 11), // 2048 : Demande le CallBack after Rx
} I2CCM_ActionType;

typedef enum _I2CCM_ActionId
{
	I2CCM_ACTION_INIT = 0,	// Effectuer les Actions d'Initialisation
//	I2C_CHECK_NEW_VALUE,	// Demander s'il existe une nouvelle valeur
	I2CCM_RQ_INIT_DATA,		// Effectuer des Opérations uniques d'Init
	I2CCM_QUERY_VALUES, 	// Récupérer les nouvelles valeurs du Device
	I2CCM_RETURN_VALUES,	// Retourner les nouvelles valeurs au Core, via la Task
	I2CCM_ACCEPT_VALUES,	// Accepter des Valeurs internes
	I2CCM_APPLY_VALUES, 	// Transmettre des nouvelles valeurs au Device
//	I2C_APPLY_VALUES -> RELAY_VALUES
	I2CCM_REDIRECT_ACTION,	// Rediriger vers le Code Action dans 'result'
} I2CCM_ActionId;

/******************************************************************************/
// Prototypes des Fonctions publiques exportées par l'I2cComMasterCore :

uint8_t i2cComMasterHandleDeviceList(I2CCM_DevManager* pI2cDevManager, I2CCM_DevManagerParams* pI2cDevManParams);
void i2cComMasterHandleActiveDevice(I2CCM_DevManager* pI2cDevManager, I2CCM_Void* hi2c); // I2C_HandleTypeDef
void i2cComMasterAssertPin(I2CCM_AssertPinParams* pAssertPinParam, uint16_t newPinState);

I2CCM_Manage_Device* i2cComMasterGetDeviceManagerByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar);
uint8_t i2cComMasterHandleExternalActionByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar, I2CCM_ExternalAction* pExtAction);
uint8_t i2cComMasterForceDeviceNextActionDelayByExtVarPtr(I2CCM_DevManagerParams* pI2cDevManParams, I2CCM_Void* pExtVar, uint32_t DelayInMs);

uint16_t i2cComMaster_FinalizeSizedRequest(I2CCM_Device *pDevice, uint8_t nbBytes2Send, uint8_t nbBytes2Read);
uint16_t i2cComMaster_BuildRequest_Cmd16BEwithReply(I2CCM_Device *pDevice, uint16_t Cmd16, uint8_t nbBytes2Read);
uint16_t i2cComMaster_BuildRequest_Cmd16BEwithArg16BE(I2CCM_Device *pDevice, uint16_t Cmd16, uint16_t Arg16);
uint16_t i2cComMaster_BuildRequest_Cmd16BE_Arg16BE_CRC8(I2CCM_Device *pDevice, uint16_t Cmd16, uint16_t Arg16, uint8_t Crc8);

uint8_t i2cComMaster_PushCmd8_Arg16BigEndian(uint8_t* pBuf, uint8_t Cmd, uint16_t Arg16);
uint8_t i2cComMaster_PushCmd8_Arg16LittleEndian(uint8_t* pBuf, uint8_t Cmd8, uint16_t Arg16);

uint8_t i2cComMaster_PushCmd8_Arg16_16BigEndian(uint8_t* pBuf, uint8_t Cmd8, uint16_t Arg16A, uint16_t Arg16B);

uint8_t i2cComMaster_PushCmd16BigEndian(uint8_t* pBuf, uint16_t Cmd16);
uint8_t i2cComMaster_PushCmd16BE_Arg16BE(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16);
uint8_t i2cComMaster_PushCmd16BE_Arg16BE_CRC8(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16, uint8_t Crc8);
uint8_t i2cComMaster_PushCmd16BE_Arg16_16BE_CRC8(uint8_t* pBuf, uint16_t Cmd16, uint16_t Arg16A, uint8_t Crc8A, uint16_t Arg16B, uint8_t Crc8B);

float i2cComMaster_FloatFromRaw(uint32_t floatRaw);

float i2cComMaster_AddInt16ToMoyManager(I2CCM_MoyManager_Int16* pMoyManager, int16_t newValue, uint16_t maxCt);
float i2cComMaster_AddUInt16ToMoyManager(I2CCM_MoyManager_UInt16* pMoyManager, uint16_t newValue, uint16_t maxCt);
float i2cComMaster_AddFloatToMoyManager(I2CCM_MoyManager_Float* pMoyManager, float newValue, uint16_t maxCt);

/******************************************************************************/

// Links to utils.h :
#define I2CCM_FillMemory	FillMemory
#define I2CCM_CopyMemory	CopyMemory
void I2CCM_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void I2CCM_CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt);

/******************************************************************************/

#define I2CCM_MAX_ERROR_CT_ON_PERIF_OK			5
#define I2CCM_MAKE_DEC_CT_WITH_MAX_VALUE(c,m)	if((c) > 0) { (c)--; if((c) > (m)) { (c) = (m); } }
#define I2CCM_MAKE_INC_CT_WITH_MAX_VALUE(c,m)	if((c) < (m)) { (c)++; }

#define I2CCM_MAKE_UINT16_FROM_BIG_ENDIAN(a,b)	((((uint16_t)(a))<<8) + ((uint16_t)(b)))
#define I2CCM_MAKE_UINT16_FROM_LIT_ENDIAN(a,b)	((((uint16_t)(b))<<8) + ((uint16_t)(a)))
#define I2CCM_GET_BYTE_N(a,b)					(((a) >> ((b)*8)) & 0xFF) // Récupère le ByteN (n = [0, 3] sur 32 bits)

//#define I2CCM_GET_FLOAT_FROM_UINT32_RAW(a)  	(float)(*(float*)(&(a)))

#define I2CCM_REFRAME_TO_MAX_OF_RANGE(val, min, max)	if( (val < min) || (val > max) ) val = max
#define I2CCM_BUILD_GET_MAX_CT4MOY(name,val)			uint16_t name(I2CCM_DevInitParams* pInitParams) { return val; }

#define I2CCM_MAKE_NAME(PREFIX,NAME)		PREFIX##NAME	// Pour recomposer un Nom de define en plusieurs parties

#define I2CCM_GET_UINT8_PTR_FROM_VAR(a)		((uint8_t*)&(a))
#define I2CCM_GET_VOID_PTR_FROM_VAR(a)		((void*)&(a))

/******************************************************************************/
// Redirections vers les fonctions externes HAL correspondantes :

#ifdef I2CCM_USE_I2C_HAL_DRIVER
	#define I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout) 	(int32_t)HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout)
	#define I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout)  	(int32_t)HAL_I2C_Master_Receive (hi2c, DevAddress, pData, Size, Timeout)
	#define I2C_Master_GetFlag(__HANDLE__, __FLAG__)						__HAL_I2C_GET_FLAG((I2C_HandleTypeDef*)__HANDLE__, __FLAG__)
	#define I2C_Master_IsI2cBusy(__HANDLE__)								__HAL_I2C_GET_FLAG((I2C_HandleTypeDef*)__HANDLE__, I2C_FLAG_BUSY)
#endif // I2CCM_USE_I2C_HAL_DRIVER

#ifdef I2CCM_USE_I2C_CUSTOM_DRIVER
	#define I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout) 	(int32_t)I2C_Soft_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout)
	#define I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout)  	(int32_t)I2C_Soft_Master_Receive (hi2c, DevAddress, pData, Size, Timeout)
	#define I2C_Master_GetFlag(__HANDLE__, __FLAG__)						(0)
	#define I2C_Master_IsI2cBusy(__HANDLE__)								(RESET)	// Toujours RESET parce qu'on n'utilise que des fonctions custom bloquantes
#endif // I2CCM_USE_I2C_CUSTOM_DRIVER

#define I2C_SysTick()       HAL_GetTick()
#define I2C_Delay(millisec) HAL_Delay(millisec)

/******************************************************************************/

#endif /* USER_I2CCOMMASTER_I2CCOMMASTERUTILS_H_ */
