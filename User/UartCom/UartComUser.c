/*
 * UartComUser.c
 *
 *  Created on: Dec 15, 2020
 *  Original Author: j.proux
 *
 *  Updated on: 6 Mar. 2025
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

#include "UartComUser.h"	// Pour accès à nos propres déclarations publiques
#include "UartComConf.h"	// Pour accès à la Configuration User souhaitée

// Remarque_Jp le 22/10/2021 : Merci de conserver les commentaires, pour Exemple, svp.

/******************************************************************************/

#ifdef UART_COM_ENABLE_IBUS 	// cf. "UartComConf.h"
	#if defined(UART_COM_NB_MAX_OF_IBUS) && (UART_COM_NB_MAX_OF_IBUS > 0)
		#include "iBusUser.h"	// Pour accès aux Paramètres & Traitement iBus
	#endif // UART_COM_NB_OF_UART_IBUS_ONLY
#endif // UART_COM_ENABLE_IBUS

#ifdef UART_COM_ENABLE_MODBUS_SLAVE 	// cf. "UartComConf.h"
	#if defined(UART_COM_NB_MAX_OF_MODBUS_SLAVE) && (UART_COM_NB_MAX_OF_MODBUS_SLAVE > 0)
		#include "ModbusSlaveUser.h"	// Pour accès aux Paramètres & Traitement ModbusSlave
	#endif // UART_COM_NB_MAX_OF_MODBUS_SLAVE
#endif // UART_COM_ENABLE_MODBUS_SLAVE

#ifdef UART_COM_ENABLE_MODBUS_MASTER	// cf. "UartComConf.h"
	#if defined(UART_COM_NB_MAX_OF_MODBUS_MASTER) && (UART_COM_NB_MAX_OF_MODBUS_MASTER > 0)
		#include "ModbusMasterUser.h"	// Pour accès aux Paramètres & Traitement ModbusMaster
	#endif // UART_COM_NB_MAX_OF_MODBUS_MASTER
#endif // UART_COM_ENABLE_MODBUS_MASTER

#if defined(UART_COM_NB_MAX_OF_USBD) && (UART_COM_NB_MAX_OF_USBD > 0)	// cf. "UartComConf.h"
	#include "UsbDeviceCDC.h"	// Pour accès aux Paramètres & Fonctions USB_Device_CDC
//	#include "UsbModbusUtils.h"		// Pour accès aux Paramètres & Fonctions USB_Device_CDC
//	#include "UsbDeviceSlaveUser.h"
//	#include "UsbModbusCore.h"
#endif // UART_COM_NB_MAX_OF_USBD

#if defined(UART_COM_NB_MAX_OF_USBH) && (UART_COM_NB_MAX_OF_USBH > 0)	// cf. "UartComConf.h"
//(temporaire)	#include "UsbHostCDC.h"		// Pour accès aux Paramètres & Fonctions USB_Host_CDC
#endif // UART_COM_NB_MAX_OF_USBH

#ifdef UART_COM_ENABLE_EMBRACO_INVERTER
	#if defined(NB_EMBRACO_INVERTER_BUF_DEF_SIZE) && (NB_EMBRACO_INVERTER_BUF_DEF_SIZE > 0)
		#include "EmbracoInverter.h"	// Pour accès aux Paramètres & Traitement EmbracoInverter
	#endif // NB_EMBRACO_INVERTER_BUF_DEF_SIZE
#endif // UART_COM_ENABLE_EMBRACO_INVERTER

/******************************************************************************/
// Buffers spécifiques RX & TX, de Taille differenciés suivant le besoin :

#if defined(NB_IBUS_BUF_DEF_SIZE) && (NB_IBUS_BUF_DEF_SIZE > 0)
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tIbusBuffer, IBUS_BUF_DEF_RX_SIZE, IBUS_BUF_DEF_TX_SIZE, iBusBuf, NB_IBUS_BUF_DEF_SIZE);
#endif // NB_COM_IBUS

#if defined(NB_MODBUS_SLAVE_BUF_DEF_SIZE) && (NB_MODBUS_SLAVE_BUF_DEF_SIZE > 0)	// cf. "ModbusSlaveConf.h"
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tModbusSlaveBuf, MODBUS_SLAVE_BUF_DEF_RX_SIZE, MODBUS_SLAVE_BUF_DEF_TX_SIZE, ModbusSlaveBuf, NB_MODBUS_SLAVE_BUF_DEF_SIZE);
#endif // NB_COM_MODBUS

#if defined(NB_MODBUS_SLAVE_BUF_SIZE_2) && (NB_MODBUS_SLAVE_BUF_SIZE_2 > 0)	// cf. "ModbusSlaveConf.h"
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tModbusSlaveBuf2, MODBUS_SLAVE_BUF_SIZE_2_RX, MODBUS_SLAVE_BUF_SIZE_2_TX, ModbusSlaveBuf2, NB_MODBUS_SLAVE_BUF_SIZE_2);
#endif // NB_COM_MODBUS

#if defined(NB_MODBUS_SLAVE_BUF_SIZE_3) && (NB_MODBUS_SLAVE_BUF_SIZE_3 > 0)	// cf. "ModbusSlaveConf.h"
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tModbusSlaveBuf3, MODBUS_SLAVE_BUF_SIZE_3_RX, MODBUS_SLAVE_BUF_SIZE_3_TX, ModbusSlaveBuf3, NB_MODBUS_SLAVE_BUF_SIZE_3);
#endif // NB_COM_MODBUS


#if defined(NB_MODBUS_MASTER_BUF_DEF_SIZE) && (NB_MODBUS_MASTER_BUF_DEF_SIZE > 0)
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tModbusMasterBuf, MODBUS_MASTER_BUF_DEF_RX_SIZE, MODBUS_MASTER_BUF_DEF_TX_SIZE, ModbusMasterBuf, NB_MODBUS_MASTER_BUF_DEF_SIZE);
#endif // NB_COM_IBUS

#if defined(UART_COM_NB_OF_UART_CHANGEABLE_USAGE) && (UART_COM_NB_OF_UART_CHANGEABLE_USAGE > 0)
	// Composer, Décommenter & Modifier selon les besoins Custom :
//	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tUartIbusBufFtFl,		IBUS_BUF_DEF_RX_SIZE, IBUS_BUF_DEF_TX_SIZE, iBusBufFtFl, UART_COM_NB_OF_UART_IBUS_FITFUL);
//	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tUartModbusSlaveBufFtFl, MODBUS_SLAVE_BUF_DEF_RX_SIZE, MODBUS_SLAVE_BUF_DEF_TX_SIZE, UartModbusSlaveBufFtFl, UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL);
//	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tUartModbusMasterBufFtFl, MODBUS_MASTER_BUF_DEF_RX_SIZE, MODBUS_MASTER_BUF_DEF_TX_SIZE, UartModbusMasterBufFtFl, UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL);
#endif // UART_COM_NB_OF_UART_CHANGEABLE_USAGE

#if defined(NB_EMBRACO_INVERTER_BUF_DEF_SIZE) && (NB_EMBRACO_INVERTER_BUF_DEF_SIZE > 0)	// cf. "EmbracoInverterConf.h"
	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tEmbracoInverterBuf, EMBRACO_INVERTER_BUF_DEF_RX_SIZE, EMBRACO_INVERTER_BUF_DEF_TX_SIZE, EmbracoInverterBuf, NB_EMBRACO_INVERTER_BUF_DEF_SIZE);
#endif // NB_EMBRACO_INVERTER_BUF_DEF_SIZE

//#if defined(NB_OF_USB_DEVICE_MODBUS) && (NB_OF_USB_DEVICE_MODBUS > 0)
////	UART_COM_MAKE_STRUCT_BUF(tUsbModbusSlaveBuf, USBD_RX_BUF_SIZE, USBD_TX_BUF_SIZE);
////	tUsbModbusSlaveBuf UsbModbusSlaveBuf[NB_OF_USB_DEVICE_MODBUS];
//	UART_COM_MAKE_STRUCT_BUF_VAR_ARRAY(tUsbModbusSlaveBuf, USBD_RX_BUF_SIZE, USBD_TX_BUF_SIZE,
//			UsbModbusSlaveBuf, NB_OF_USB_DEVICE_MODBUS);
//#endif // NB_OF_USB_DEVICE_MODBUS

/******************************************************************************/
// Pour Basculement ModbusSlave <-> ModbusMaster :

#if defined(UART_COM_NB_MODBUS_MASTER_SLAVE_FTFL) && (UART_COM_NB_MODBUS_MASTER_SLAVE_FTFL > 0)

typedef enum _eModbusMode
{
	ModbusModeNone = 0,
	ModbusModeSlave,
	ModbusModeMaster,
} eModbusMode;

// Pour alterner ModbusSlave sur UART_IHM / ModbusMaster sur UART_IHM :
uint8_t ModbusIhmCurMode = ModbusModeNone;	// Aucun Mode chargé par défaut à l'Init
uint8_t ModbusIhmNewMode = ModbusModeSlave;	// Mode Slave demandé à l'Init

//// Pour alterner ModbusSlave sur UART_User / ModbusMaster sur UART_User :
//uint8_t ModbusUserCurMode = ModbusModeNone;	// Aucun Mode chargé par défaut à l'Init
//uint8_t ModbusUserNewMode = ModbusModeSlave;	// Mode Slave demandé à l'Init

#endif // UART_COM_NB_MODBUS_MASTER_SLAVE_FTFL

/******************************************************************************/

const tUartComInitParams mUartComInitParam[] = {
// Type de Com, 		&hHandleTypeDef,	MX_Fn_Init,				UART_COM_STRUCT_BUF_INFOS(varBuffer),			pFnHandler,				pVoidParam,			GPIO_TX,				sabEndOfFrame,

/* USER CODE BEGIN Static_InitParams */

#if defined(NB_IBUS_BUF_DEF_SIZE) && (NB_IBUS_BUF_DEF_SIZE > 0)
 	{ UartComIbus,		UART_IBUS_INT,		MX_USART1_UART_Init,	UART_COM_STRUCT_BUF_INFOS(iBusBuf[0]),			iBusRxHandler,	 		IBUS_USER_PARAMS,			UART_COM_GPIO_TX_NONE,	IBUS_EOF_RX,			IBUS_EOB_RX,			IBUS_ACK_TO,			IBUS_NO_TX_INIT,			IBUS_NO_TX_FRAME,			IBUS_NO_TX_BYTE,			IBUS_NO_RX_TO,			IBUS_MIN_RX_SIZE,			IBUS_MIN_TX_SIZE,			IBUS_MAX_RX_BLOC_SZ,			IBUS_DEF_REPLY_SZ,	 		IBUS_ERR_RX_FRAME,	  		IBUS_ERR_TX_RETRY,   		IBUS_ERR_TX_FRAME,   		IBUS_INIT_DO_LOAD_FLAGS,			UART_COM_CLASS_TX_IT_RX_IT },	// iBus sur Uart1 (Fin de Trame après 20ms)
	{ UartComIbus,		UART_IBUS_EXT,		MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(iBusBuf[1]),			iBusRxHandler,	 		IBUS_EXT_PARAMS,			UART_COM_GPIO_TX_NONE,	IBUS_EOF_RX,			IBUS_EOB_RX,			IBUS_ACK_TO,			IBUS_NO_TX_INIT,			IBUS_NO_TX_FRAME,			IBUS_NO_TX_BYTE,			IBUS_NO_RX_TO,			IBUS_MIN_RX_SIZE,			IBUS_MIN_TX_SIZE,			IBUS_MAX_RX_BLOC_SZ,			IBUS_DEF_REPLY_SZ,	 		IBUS_ERR_RX_FRAME,	  		IBUS_ERR_TX_RETRY,   		IBUS_ERR_TX_FRAME,   		IBUS_INIT_DO_LOAD_FLAGS,			UART_COM_CLASS_TX_IT_RX_IT },	// iBus sur Uart3
#endif // NB_IBUS_BUF_DEF_SIZE > 0

#if defined(NB_MODBUS_SLAVE_BUF_DEF_SIZE) && (NB_MODBUS_SLAVE_BUF_DEF_SIZE > 0)	// cf. "ModbusSlaveConf.h"
//	{ UartComModbus,	UART_MODBUS_IHM,	MX_USART1_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave IHM sur Uart1
	{ UartComModbus,	UART_MODBUS_USER,	MX_UART5_Init,			UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_USER_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave User sur Uart5 en TX_DMA & RX_IT
//	{ UartComModbus,	UART_MODBUS_USER,	USART3_Custom_Init, 	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[1]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_USER_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave User sur Uart3

// Essai de ModbusMaster sur Uart3 (désactivé @ MST) :
//	{ UartComModbus,	UART_MODBUS_USER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusBuf[1]),	ModbusMasterRxHandler,	MODBUS_MASTER_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_MASTER_EOF_RX,	MODBUS_MASTER_EOB_RX,	MODBUS_MASTER_REPLY_TO,	MODBUS_MASTER_NO_TX_INIT,	MODBUS_MASTER_NO_TX_FRAME,	MODBUS_MASTER_NO_TX_BYTE,	MODBUS_MASTER_NO_RX_TO,	MODBUS_MASTER_MIN_RX_SIZE,	MODBUS_MASTER_MIN_TX_SIZE,	MODBUS_MASTER_MAX_RX_BLC_SZ,	MODBUS_MASTER_DEF_REPLY_SZ,	MODBUS_MASTER_ERR_RX_FRAME,	MODBUS_MASTER_ERR_TX_RETRY,	MODBUS_MASTER_ERR_TX_FRAME,	MODBUS_MASTER_INIT_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusMaster User sur Uart3
#endif // NB_MODBUS_SLAVE_BUF_DEF_SIZE > 0

#if defined(NB_MODBUS_SLAVE_BUF_SIZE_2) && (NB_MODBUS_SLAVE_BUF_SIZE_2 > 0)	// cf. "ModbusSlaveConf.h"
	{ UartComModbus,	UART_MODBUS_EXT,	MX_UART8_Init,			UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf2[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_EXT_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_REPLY_SIZE_2,	MODBUS_SLAVE_ERR_RX_FRAME2,	MODBUS_SLAVE_ERR_TX_RETRY2,	MODBUS_SLAVE_ERR_TX_FRAME2,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave Ext sur Uart8
#endif // NB_MODBUS_SLAVE_BUF_SIZE_2

//// Essai de ModbusSlave sur USB_Device :
//#if defined(NB_OF_USB_DEVICE_MODBUS) && (NB_OF_USB_DEVICE_MODBUS > 0)
//	{ UartComModbus,	USBD_MODBUS,		UsbDeviceSlaveInitUser, 	UART_COM_STRUCT_BUF_INFOS(UsbModbusBuf[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_USBD,	UART_COM_GPIO_TX_NONE,	USBD_EOF_RX,			USBD_CDC_MODBUS_SLAVE_EOB_RX,			USBD_REPLY_TO,			USBD_CDC_MODBUS_SLAVE_NO_TX_INIT,			USBD_CDC_MODBUS_SLAVE_NO_TX_FRAME,			USBD_CDC_MODBUS_SLAVE_NO_TX_BYTE,			USBD_CDC_MODBUS_SLAVE_NO_RX_TO,			USBD_CDC_MODBUS_SLAVE_MIN_RX_SIZE,			USBD_CDC_MODBUS_SLAVE_MIN_TX_SIZE,			USBD_MAX_RX_BLOC_SZ,			USBD_DEF_REPLY_SZ,			USBD_ERR_RX_FRAME,			USBD_ERR_TX_RETRY,			USBD_ERR_TX_FRAME,			USBD_INIT_LOAD_FLAGS,			USBD_CDC_MODBUS_CLASS },	// ModbusSlave User sur USB_Device
//#endif // NB_OF_USB_DEVICE_MODBUS

//	{ UartComCustom,	UART_COM_INVERTER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(EmbracoInverterBuf[0]), EMBRACO_INVERTER_RX_FN_HANDLER,	EMBRACO_INVERTER_SYS_MANAGER,	UART_COM_GPIO_TX_NONE,	EMBRACO_INVERTER_EOF_RX,	EMBRACO_INVERTER_EOB_RX,	EMBRACO_INVERTER_REPLY_TO,	EMBRACO_INVERTER_NO_TX_INIT,	EMBRACO_INVERTER_NO_TX_FRAME,	EMBRACO_INVERTER_NO_TX_BYTE,	EMBRACO_INVERTER_NO_RX_TO,	EMBRACO_INVERTER_MIN_RX_SIZE,	EMBRACO_INVERTER_MIN_TX_SIZE,	EMBRACO_INVERTER_MAX_RX_BLOC_SZ,	EMBRACO_INVERTER_DEF_REPLY_SZ,	EMBRACO_INVERTER_ERR_RX_FRAME,	EMBRACO_INVERTER_ERR_TX_RETRY,	EMBRACO_INVERTER_ERR_TX_FRAME,	EMBRACO_INVERTER_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT }, // EmbracoInverter sur Uart3
//	{ UartComCustom,	UART_COM_INVERTER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(EmbracoInverterBuf[0]), EMBRACO_INVERTER_RX_FN_HANDLER,	EMBRACO_INVERTER_SYS_MANAGER,	UART_COM_GPIO_TX_NONE,	EMBRACO_INVERTER_EOF_RX,	EMBRACO_INVERTER_EOB_RX,	EMBRACO_INVERTER_REPLY_TO,	EMBRACO_INVERTER_NO_TX_INIT,	EMBRACO_INVERTER_NO_TX_FRAME,	EMBRACO_INVERTER_NO_TX_BYTE,	EMBRACO_INVERTER_NO_RX_TO,	EMBRACO_INVERTER_MIN_RX_SIZE,	EMBRACO_INVERTER_MIN_TX_SIZE,	EMBRACO_INVERTER_MAX_RX_BLOC_SZ,	EMBRACO_INVERTER_DEF_REPLY_SZ,	EMBRACO_INVERTER_ERR_RX_FRAME,	EMBRACO_INVERTER_ERR_TX_RETRY,	EMBRACO_INVERTER_ERR_TX_FRAME,	EMBRACO_INVERTER_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IDLE_IT }, // EmbracoInverter sur Uart3
//	{ UartComCustom,	UART_COM_INVERTER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(EmbracoInverterBuf[0]), EMBRACO_INVERTER_RX_FN_HANDLER,	EMBRACO_INVERTER_SYS_MANAGER,	UART_COM_GPIO_TX_NONE,	EMBRACO_INVERTER_EOF_RX,	EMBRACO_INVERTER_EOB_RX,	EMBRACO_INVERTER_REPLY_TO,	EMBRACO_INVERTER_NO_TX_INIT,	EMBRACO_INVERTER_NO_TX_FRAME,	EMBRACO_INVERTER_NO_TX_BYTE,	EMBRACO_INVERTER_NO_RX_TO,	EMBRACO_INVERTER_MIN_RX_SIZE,	EMBRACO_INVERTER_MIN_TX_SIZE,	EMBRACO_INVERTER_MAX_RX_BLOC_SZ,	EMBRACO_INVERTER_DEF_REPLY_SZ,	EMBRACO_INVERTER_ERR_RX_FRAME,	EMBRACO_INVERTER_ERR_TX_RETRY,	EMBRACO_INVERTER_ERR_TX_FRAME,	EMBRACO_INVERTER_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_DMA }, // EmbracoInverter sur Uart3
	{ UartComCustom,	UART_COM_INVERTER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(EmbracoInverterBuf[0]), EMBRACO_INVERTER_RX_FN_HANDLER,	EMBRACO_INVERTER_SYS_MANAGER,	UART_COM_GPIO_TX_NONE,	EMBRACO_INVERTER_EOF_RX,	EMBRACO_INVERTER_EOB_RX,	EMBRACO_INVERTER_REPLY_TO,	EMBRACO_INVERTER_NO_TX_INIT,	EMBRACO_INVERTER_NO_TX_FRAME,	EMBRACO_INVERTER_NO_TX_BYTE,	EMBRACO_INVERTER_NO_RX_TO,	EMBRACO_INVERTER_MIN_RX_SIZE,	EMBRACO_INVERTER_MIN_TX_SIZE,	EMBRACO_INVERTER_MAX_RX_BLOC_SZ,	EMBRACO_INVERTER_DEF_REPLY_SZ,	EMBRACO_INVERTER_ERR_RX_FRAME,	EMBRACO_INVERTER_ERR_TX_RETRY,	EMBRACO_INVERTER_ERR_TX_FRAME,	EMBRACO_INVERTER_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IDLE_DMA }, // EmbracoInverter sur Uart3

/* USER CODE END Static_InitParams */

};
UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tUartComInitParams, UC_BaseInitParam, UC_EndInitParams, mUartComInitParam);

/******************************************************************************/
/* USER CODE BEGIN Dynamic_InitParams */

#if defined(UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL) && (UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL > 0)
const tUartComInitParams mUartComInitModbusIhmSlave =
	{ UartComModbus,	UART_MODBUS_IHM,	MX_USART2_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusSlave IHM sur Uart2
//const tUartComInitParams mUartComInitModbusUserSlave =
//	{ UartComModbus,	UART_MODBUS_USER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[1]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_USER_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusSlave User sur Uart3
#endif // UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL

#if defined(UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL) && (UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL > 0)
const tUartComInitParams mUartComInitModbusIhmMaster =
	{ UartComModbus,	UART_MODBUS_IHM,	MX_USART2_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),	ModbusMasterRxHandler,	MODBUS_MASTER_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_MASTER_EOF_RX,	MODBUS_MASTER_EOB_RX,	MODBUS_MASTER_REPLY_TO,	MODBUS_MASTER_NO_TX_INIT,	MODBUS_MASTER_NO_TX_FRAME,	MODBUS_MASTER_NO_TX_BYTE,	MODBUS_MASTER_NO_RX_TO,	MODBUS_MASTER_MIN_RX_SIZE,	MODBUS_MASTER_MIN_TX_SIZE,	MODBUS_MASTER_MAX_RX_BLC_SZ,	MODBUS_MASTER_DEF_REPLY_SZ,	MODBUS_MASTER_ERR_RX_FRAME,	MODBUS_MASTER_ERR_TX_RETRY,	MODBUS_MASTER_ERR_TX_FRAME,	MODBUS_MASTER_INIT_NO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusMaster IHM sur Uart2
//	const tUartComInitParams mUartComInitModbusUserMaster =
//	{ UartComModbus,	UART_MODBUS_USER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[1]),	ModbusMasterRxHandler,	MODBUS_MASTER_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_MASTER_EOF_RX,	MODBUS_MASTER_EOB_RX,	MODBUS_MASTER_REPLY_TO,	MODBUS_MASTER_NO_TX_INIT,	MODBUS_MASTER_NO_TX_FRAME,	MODBUS_MASTER_NO_TX_BYTE,	MODBUS_MASTER_NO_RX_TO,	MODBUS_MASTER_MIN_RX_SIZE,	MODBUS_MASTER_MIN_TX_SIZE,	MODBUS_MASTER_MAX_RX_BLC_SZ,	MODBUS_MASTER_DEF_REPLY_SZ,	MODBUS_MASTER_ERR_RX_FRAME,	MODBUS_MASTER_ERR_TX_RETRY,	MODBUS_MASTER_ERR_TX_FRAME,	MODBUS_MASTER_INIT_NO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusMaster User sur Uart3
#endif // UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL

// ModbusSlave sur UsbDevice intermittent :
#if defined(UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL) && (UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL > 0)
const tUartComInitParams mUartComInitUsbDeviceModbusSlave = { UartComModbus,	USBD_CDC_MODBUS_SLAVE,		UsbDeviceCdc_InitModbusSlaveParams,
	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf3[0]),	ModbusSlaveRxHandler,	MODBUS_SLAVE_USBD_CDC_PARAMS, UART_COM_GPIO_TX_NONE,
	USBD_CDC_MODBUS_SLAVE_EOF_RX, USBD_CDC_MODBUS_SLAVE_EOB_RX, USBD_CDC_MODBUS_SLAVE_REPLY_TO, USBD_CDC_MODBUS_SLAVE_NO_TX_INIT, USBD_CDC_MODBUS_SLAVE_NO_TX_FRAME, USBD_CDC_MODBUS_SLAVE_NO_TX_BYTE, USBD_CDC_MODBUS_SLAVE_NO_RX_TO,
	USBD_CDC_MODBUS_SLAVE_MIN_RX_SIZE, USBD_CDC_MODBUS_SLAVE_MIN_TX_SIZE, USBD_CDC_MODBUS_SLAVE_MAX_RX_BLOC_SZ, MODBUS_SLAVE_REPLY_SIZE_3, USBD_CDC_MODBUS_SLAVE_ERR_RX_FRAME, USBD_CDC_MODBUS_SLAVE_ERR_TX_RETRY,
	USBD_CDC_MODBUS_SLAVE_ERR_TX_FRAME, USBD_CDC_MODBUS_SLAVE_INIT_LOAD_FLAGS, USB_DEVICE_CDC_COM_CLASS };	// ModbusSlave User sur USB_Device
#endif // UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL

// ModbusSlave sur UsbHost intermittent :
#if defined(UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL) && (UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL > 0)
const tUartComInitParams mUartComInitUsbHostModbusSlave = { UartComModbus, USBH_CDC_MODBUS_SLAVE,	UsbHostCdc_InitModbusSlaveParams,
	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf3[0]), ModbusSlaveRxHandler, MODBUS_SLAVE_USBH_CDC_PARAMS, UART_COM_GPIO_TX_NONE,
	USBH_CDC_MODBUS_SLAVE_EOF_RX, USBH_CDC_MODBUS_SLAVE_EOB_RX, USBH_CDC_MODBUS_SLAVE_REPLY_TO, USBH_CDC_MODBUS_SLAVE_NO_TX_INIT,
	USBH_CDC_MODBUS_SLAVE_NO_TX_FRAME, USBH_CDC_MODBUS_SLAVE_NO_TX_BYTE, USBH_CDC_MODBUS_SLAVE_NO_RX_TO,
	USBH_CDC_MODBUS_SLAVE_MIN_RX_SIZE, USBH_CDC_MODBUS_SLAVE_MIN_TX_SIZE, USBH_CDC_MODBUS_SLAVE_MAX_RX_BLOC_SZ,
	MODBUS_SLAVE_REPLY_SIZE_3, USBH_CDC_MODBUS_SLAVE_ERR_RX_FRAME, USBH_CDC_MODBUS_SLAVE_ERR_TX_RETRY,
	USBH_CDC_MODBUS_SLAVE_ERR_TX_FRAME, USBH_CDC_MODBUS_SLAVE_INIT_LOAD_FLAGS, USB_HOST_CDC_COM_CLASS };	// ModbusSlave User sur USB_Host
#endif // UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL

/* USER CODE END Dynamic_InitParams */

// Variables pour le changement de BaudRate / Parity / Stop sur l'un des UART parmi mUartComInitParam (Ajout_Jp le 16/04/2024 pour Ticket #33):
UartReInitUserParams UartModbusUser_ReInitUserParams = { eUartReInitBaud9600, eUartReInitParityNone, eUartReInitStopOne, 0 }; // 9600, None, 1 Stop, Normal Init
UartReInitUserParams UartModbusUser_ReInitRealParams = { 0 };	// Config réelle pour Sauvegarde en Mémoire Externe
//UART_AdvFeatureInitTypeDef Uart3ReInitAdvInit = { .TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE,	.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE };
//UartReInitUserParams Uart3ReInitUserParams = { eUartReInitBaud9600, eUartReInitParityNone, eUartReInitStopOne, &Uart3ReInitAdvInit }; // 9600, None, 1 Stop + Advanced Init
UartReInitCoreVars UartModbusUser_ReInitCoreVars = { .ReInitFlags.InitRS485Ex = 1, }; // Pour Init via "HAL_RS485Ex_Init", comme dans "MX_USART3_UART_Init"

//#define UART_MODBUS_USER_REINIT_ITEM	UART_MAKE_REINIT_ITEM(UART_MODBUS_USER, USART3, Uart3ReInitUserParams, Uart3ReInitCoreVars, UartReInit9600N1)

const UartReInitItem mUartReInitItems[] = {
//	UART_MODBUS_USER_REINIT_ITEM,
//	{ UART_MODBUS_USER, USART3, &UartModbusUser_ReInitUserParams, &UartModbusUser_ReInitCoreVars, &UartModbusUser_ReInitRealParams, &UartReInit9600N1 },
	{ UART_MODBUS_USER, UART5, &UartModbusUser_ReInitUserParams, &UartModbusUser_ReInitCoreVars, &UartModbusUser_ReInitRealParams, &UartReInit115200N1 },
};
UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(UartReInitItem, UC_BaseUartReInitItems, UC_EndUartReInitItems, mUartReInitItems);

//#if nb_of_params != NB_OF_COM_INIT_PARAMS
//	#error "Merci de Configurer NB_OF_COM_INIT_PARAMS à " ## nb_of_params
//#else
//
//#endif

/******************************************************************************/

//uint16_t iBusPostFireDamper(tComFrameParams* pFI, void* pVoidParam);
//uint16_t iBusPostBCA(tComFrameParams* pFI, void* pVoidParam);
//uint16_t ModbusPostRead(tComFrameParams* pFI, void* pVoidParam);

const tUartComInitRegularTx mInitRegTx[] = {
/* USER CODE BEGIN Register_RegularTx */

//	{ UART_IBUS_INT,		IBUS_REG_TX_FN_MANTA_HUB_0,		IBUS_MAKE_FIRST_NEXT_DELAY(IBUS_DELAY_FIRST_TX_TO_MANTA_HUB_0, IBUS_DELAY_NEXT_TX_TO_MANTA_HUB_0), IBUS_FRAME_SIZE_TO_MANTA },
//	{ UART_IBUS_EXT,		IBUS_REG_TX_FN_MANTA_HUB_1,		IBUS_MAKE_FIRST_NEXT_DELAY(IBUS_DELAY_FIRST_TX_TO_MANTA_HUB_1, IBUS_DELAY_NEXT_TX_TO_MANTA_HUB_1), IBUS_FRAME_SIZE_TO_MANTA },
//	{ UART_MODBUS_USER,	ModbusPostRead,		PVOID_INT(0),	5 *10, 3 *10, 10 },
//	{ UART_IBUS_INT,		IBUS_REG_TX_FN_FIRE_DAMPERS,	IBUS_MAKE_FIRST_NEXT_DELAY(IBUS_DELAY_FIRST_TX_TO_FIRE_DAMPER, IBUS_DELAY_NEXT_TX_TO_FIRE_DAMPER), IBUS_FRAME_SIZE_FOR_FIRE_DAMPERS },
//	{ UART_IBUS_INT,		IBUS_REG_TX_FN_BCA_03,			60 *10, 20 *10, IBUS_FRAME_SIZE_TO_BCA_03 },
//	{ UART_IBUS_INT,		IBUS_REG_TX_FN_BCA_04,			60 *10, 30 *10, IBUS_FRAME_SIZE_TO_BCA_04 },

//	{ UART_IBUS,	IBUS_REG_TX_FN_FIRE_DAMPERS,		IBUS_MAKE_FIRST_NEXT_DELAY(IBUS_DELAY_FIRST_TX_TO_FIRE_DAMPER, IBUS_DELAY_NEXT_TX_TO_FIRE_DAMPER), IBUS_FRAME_SIZE_FOR_FIRE_DAMPERS },
//	{ UART_IBUS,	iBusPostSpecialFrames1s,	0,		IBUS_MAKE_FIRST_NEXT_DELAY(IBUS_MIN(IBUS_CLAVIER_CLAVIERS_FIRST_DELAY, IBUS_CLAVIER_GROUPE_FIRST_DELAY), 1000), IBUS_MAX_2FRAMES_SIZE(IBUS_CLAVIER_CLAVIERS_TRAME_SIZE, IBUS_CLAVIER_GROUPE_TRAME_SIZE) },
////	{ UART_IBUS,	iBusPostClavier2Claviers,	0,		0, IBUS_CLAVIER_CLAVIERS_FIRST_DELAY/100, IBUS_CLAVIER_CLAVIERS_TRAME_SIZE },
////	{ UART_IBUS,	iBusPostClavier2Groupe,		0,		IBUS_CLAVIER_GROUPE_POST_PERIOD/100, IBUS_CLAVIER_GROUPE_FIRST_DELAY/100, IBUS_CLAVIER_GROUPE_TRAME_SIZE },
//	{ UART_IBUS,	IBUS_REG_TX_FN_BCA_03, 60 *10, 20 *10, IBUS_FRAME_SIZE_TO_BCA_03 },
//	{ UART_IBUS,	IBUS_REG_TX_FN_BCA_04, 60 *10, 30 *10, IBUS_FRAME_SIZE_TO_BCA_04 },

//		{ &huart1,	0,					PVOID_INT(75), 30, 10, 25 },
//		{ &huart1,	iBusPostFireDamper,	PVOID_INT(75), 30, 10, 25 },

#if defined(NB_EMBRACO_INVERTER_BUF_DEF_SIZE) && (NB_EMBRACO_INVERTER_BUF_DEF_SIZE > 0)	// cf. "EmbracoInverterConf.h"
	{ UART_COM_INVERTER,	EMBRACO_INVERTER_TX_REGULAR_FN,		EMBRACO_INVERTER_SYS_MANAGER,	EMBRACO_INVERTER_TX_FIRST_DELAY, EMBRACO_INVERTER_TX_NORMAL_DELAY, EMBRACO_INVERTER_TX_DEF_FRAME_SIZE },
#endif // NB_EMBRACO_INVERTER_BUF_DEF_SIZE

/* USER CODE END Register_RegularTx */
};
UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tUartComInitRegularTx, UC_BaseRegTxInitParam, UC_EndRegTxInitParam, mInitRegTx);

//// Essai de RegularTx sur ModbusMaster :
//const tUartComInitRegularTx mModbusMasterRegTx = // RégularTx spécial ModbusMaster sur UartUser :
//		{ UART_MODBUS_IHM,	ModbusMasterPostReadExemple, 0, 10 *10, 5*10, 20};

extern uint32_t UsbDeviceLastRxFrame;

//// INIT - Option 1 : définir une fonction Init_UART_Com_Devices() dans le User :
//void Init_UART_Com_Devices(void) // Si Présente : Remplace la fontion du même nom dans le Core
//{
//	Init_UART_Com_User_Devices();
//}
//// INIT - Option 1 ou 2 : S'il faut appeler la sous-fonction Custom User :
//void inline Init_UART_Com_User_Devices(void)
//{
//	MX_USART1_UART_Init();	// iBus
//	MX_USART2_UART_Init();	// Modbus IHM
//	MX_USART3_UART_Init();	// Modbus User
//	MX_UART8_Init();		// Modbus Externe
//}
// INIT - Option 3 : Si les Fonctions ont directement été appelées par la macro depuis le Core ... Rien à faire ici !

/******************************************************************************/

//void Init_UART_COM_Handlers(void) // Si Présente : Remplace la fontion du même nom dans le Core
//{
//	Init_UART_Com_User_Handlers();
//}
//void inline Init_UART_Com_User_Handlers(void)
//{
//
//}

void UartCom_InitUserMST(void)
{
	// Initialise les CallBack d'envoi Réguliers :
	for(tUartComInitRegularTx* pRegTx = (tUartComInitRegularTx*)UC_BaseRegTxInitParam; pRegTx < UC_EndRegTxInitParam; pRegTx++)
	{
		UartCom_RegisterRegularTxFrame(pRegTx);
	}

	// Initialise l'Identité & Adresse Esclave de chaque Handler déjà configuréss :
#if defined(UART_COM_ENABLE_IBUS) && defined(IBUS_INIT_USER_MST)	// cf. "UartComConf.h"
	IBUS_INIT_USER_MST();		// Initialise @ MST les Variables spécifiques iBus
#endif // UART_COM_ENABLE_IBUS & IBUS_INIT_USER_MST

#ifdef UART_COM_ENABLE_MODBUS_SLAVE	// cf. "UartComConf.h"
	ModbusSlaveInitUserMST();	// Initialise @ MST les ModbusSlave Statiques sur UART
#endif // UART_COM_ENABLE_MODBUS_SLAVE

#ifdef UART_COM_ENABLE_MODBUS_MASTER	// cf. "UartComConf.h"
	ModbusMasterInitUserMST();	// Initialise @ MST les ModbusMaster Statiques sur UART
#endif // UART_COM_ENABLE_MODBUS_MASTER

//	UsbDeviceSlaveInitUser();	// Initialise @ MST les ModbusSlave Statiques sur USB_Device

#ifdef UART_COM_ENABLE_EMBRACO_INVERTER	// cf. "UartComConf.h"
	InitEmbracoInverterMST();	// Initialise @ MST les EmbracoInverter sur UART
#endif // UART_COM_ENABLE_EMBRACO_INVERTER

}

/******************************************************************************/
// Pour Basculer automatiquement ModbusSlave <-> ModbusMaster sur UART_IHM :

#if defined(UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL) && (UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL > 0)

void ManageModbusIhmMode(void) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	if(ModbusIhmCurMode != ModbusIhmNewMode)
	{
		UartCom_UnRegister_Handle(UART_MODBUS_IHM);
		switch(ModbusIhmNewMode)
		{
		case ModbusModeSlave:	// Activer & Configurer le mode Modbus Slave :
#if defined(UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL) && (UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL > 0)
			UART_COM_REGISTER_INIT_PARAM(mUartComInitModbusIhmSlave);	// Charger la nouvelle Connexion
			InitModbusSlaveStruct(MODBUS_SLAVE_UART_IHM_PARAMS, 	COM_UART_MODBUS_IHM_SRC_ID,	MODBUS_SLAVE_IHM_DEF_SLAVE_ID);
#endif // UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL
			break;
		case ModbusModeMaster:	// Activer & Configurer le mode Modbus Master :
#if defined(UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL) && (UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL > 0)
			UART_COM_REGISTER_INIT_PARAM(mUartComInitModbusIhmMaster);	// Charger la nouvelle Connexion
			InitModbusMasterStruct(MODBUS_MASTER_UART_IHM_PARAMS,	COM_UART_MODBUS_IHM_SRC_ID,	MODBUS_MASTER_IHM_DEF_SLAVE_ID);
			UartCom_RegisterRegularTxFrame((tUartComInitRegularTx*)&mModbusMasterRegTx);
#endif // UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL
			break;
		default:
			break;
		}
		ModbusIhmCurMode = ModbusIhmNewMode;
	}
}

/******************************************************************************/

uint8_t ModbusIhmGetCurMode(void) { return ModbusIhmCurMode; }
void ModbusIhmSetNewMode(uint8_t newMode) { ModbusIhmNewMode = newMode; }

#endif // UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL

/******************************************************************************/
// Pour Basculer Manuellement ModbusSlave <-> ModbusMaster sur UART_IHM :

#if defined(UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL) && (UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL > 0)

void LoadUartIhmModbusSlave(void)
{
	UartCom_UnRegister_Handle(UART_MODBUS_IHM);	// Libérer d'un éventuel usage actuel du Périphérique UART_IHM
#if defined(UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL) && (UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL > 0)
	UART_COM_REGISTER_INIT_PARAM(mUartComInitModbusIhmSlave);	// Charger la nouvelle Connexion
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_IHM_PARAMS, 	COM_UART_MODBUS_IHM_SRC_ID,	MODBUS_SLAVE_IHM_DEF_SLAVE_ID);
#endif // UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL
	ModbusIhmCurMode = ModbusModeSlave;
}

void UnloadUartIhmModbusSlave(void)
{
	UartCom_UnRegister_Handle(UART_MODBUS_IHM);	// Libérer d'un éventuel usage actuel du Périphérique UART_IHM
	ModbusIhmCurMode = ModbusModeNone;
}

/******************************************************************************/

void LoadUartIhmModbusMaster(void)
{
	UartCom_UnRegister_Handle(UART_MODBUS_IHM);	// Libérer d'un éventuel usage actuel du Périphérique UART_IHM
#if defined(UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL) && (UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL > 0)
	UART_COM_REGISTER_INIT_PARAM(mUartComInitModbusIhmMaster);	// Charger la nouvelle Connexion
#endif // UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL
	ModbusIhmCurMode = ModbusModeMaster;
}

void UnloadUartIhmModbusMaster(void)
{
	UartCom_UnRegister_Handle(UART_MODBUS_IHM);	// Libérer d'un éventuel usage actuel du Périphérique UART_IHM
	ModbusIhmCurMode = ModbusModeNone;
}

#endif // UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL

/******************************************************************************/
// Pour Basculer automatiquement ModbusSlave <-> ModbusMaster sur UART_USER :

#if defined(UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL) && (UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL > 0)

void ManageModbusUserMode(void) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
	if(ModbusUserCurMode != ModbusUserNewMode)
	{
		UnRegisterUartComInitParam(UART_MODBUS_USER);
		switch(ModbusUserNewMode)
		{
		case ModbusModeSlave:	// Activer & Configurer le mode Modbus Slave :
			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserSlave, 0); // mUartComInitParam[2]
			InitModbusSlaveStruct(MODBUS_SLAVE_USER,	COM_UART_MODBUS_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);
			break;
		case ModbusModeMaster:	// Activer & Configurer le mode Modbus Master :
			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserMaster, 0); // mUartComInitParam[4]
			InitModbusMasterStruct(MODBUS_MASTER_UART_IHM_PARAMS,	COM_MODBUS_MASTER_USER_SRC_ID,	0);
			UartComRegisterRegularTxFrame((tUartComInitRegularTx*)&mModbusMasterRegTx);
			break;
		default:
			break;
		}
		ModbusUserCurMode = ModbusUserNewMode;
	}
}

#endif // UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL

/******************************************************************************/
// Pour Basculer Manuellement ModbusSlave sur USB_DEVICE <-> ModbusSlave sur USB_HOST :

#if defined(UART_COM_NB_USB_MODBUS_MAST_SLAV_FTFL) && (UART_COM_NB_USB_MODBUS_MAST_SLAV_FTFL > 0)

void LoadUsbDeviceModbusSlave(void)
{
	UartCom_UnRegister_Handle(USBD_CDC_MODBUS_SLAVE);	// Libérer d'un éventuel usage actuel du Périphérique USB_Device
	UART_COM_REGISTER_INIT_PARAM(mUartComInitUsbDeviceModbusSlave);	// Charger la nouvelle Connexion
//	UsbDeviceSlaveInitUser(); est automatiquement appelé au moment du Register !
	UartCom_Register_LastRxFramePtr(USBD_CDC_MODBUS_SLAVE, &UsbDeviceLastRxFrame);
	//UsbDeviceLastRxFrame = UINT32_MAX;
}

void UnloadUsbDeviceModbusSlave(void)
{
	UartCom_UnRegister_Handle(USBD_CDC_MODBUS_SLAVE);	// Libérer d'un éventuel usage actuel du Périphérique USB_Device
}

void LoadUsbHostModbusSlave(void)
{
	UartCom_UnRegister_Handle(USBH_CDC_MODBUS_SLAVE);	// Libérer d'un éventuel usage actuel du Périphérique USB_Host
	UART_COM_REGISTER_INIT_PARAM(mUartComInitUsbHostModbusSlave);	// Charger la nouvelle Connexion
	UartCom_Register_LastRxFramePtr(USBH_CDC_MODBUS_SLAVE, &UsbDeviceLastRxFrame);
}

void UnloadUsbHostModbusSlave(void)
{
	UartCom_UnRegister_Handle(USBH_CDC_MODBUS_SLAVE);	// Libérer d'un éventuel usage actuel du Périphérique USB_Host
}

#endif // UART_COM_NB_USB_MODBUS_MAST_SLAV_FTFL

void ManageModbusUserMode(void) // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !
{
/*
	if(ModbusUserCurMode != ModbusUserNewMode)
	{
		UnRegisterUartComInitParam(UART_MODBUS_USER);
		switch(ModbusUserNewMode)
		{
		case ModbusModeSlave:	// Activer & Configurer le mode Modbus Slave :
			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserSlave, 0); // mUartComInitParam[2]
			InitModbusSlaveStruct(MODBUS_SLAVE_USER,	COM_MODBUS_SLAVE_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);
			break;
		case ModbusModeMaster:	// Activer & Configurer le mode Modbus Master :
			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserMaster, 0); // mUartComInitParam[4]
			InitModbusMasterStruct(MODBUS_MASTER_USER,	COM_MODBUS_MASTER_USER_SRC_ID,	0);
			UartComRegisterRegularTxFrame((tUartComInitRegularTx*)&mModbusMasterRegTx);
			break;
		default:
			break;
		}
//		if(ModbusModeMaster == ModbusUserNewMode)
//		{
//			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserMaster, 0); // mUartComInitParam[4]
//			InitModbusMasterStruct(MODBUS_MASTER_USER,	COM_MODBUS_MASTER_USER_SRC_ID,	0);
//			UartComRegisterRegularTxFrame(&mModbusMasterRegTx);
//		} else if(ModbusModeSlave == ModbusUserNewMode)
//		{
//			UartCom_Register_InitParam((tUartComInitParams*)&mUartComInitModbusUserSlave, 0); // mUartComInitParam[2]
//			InitModbusSlaveStruct(MODBUS_SLAVE_USER,	COM_MODBUS_SLAVE_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);
//		}
		ModbusUserCurMode = ModbusUserNewMode;
	}
*/
}

/******************************************************************************/

void initUartUserParamsFromMemHisto(void)	// En cas de ReInitFact
{
//	UartCom_CopyMemory((void*)&Uart3ReInitRealParams, (void*)&UartReInit9600N1, sizeof(UartReInitUserParams));
//	UartCom_CopyMemory((void*)&Uart3ReInitUserParams, (void*)&UartReInit9600N1, sizeof(UartReInitUserParams));
	UartModbusUser_ReInitCoreVars.ReInitFlags.InitDefault = 1; // Demande l'Init avec DefaultParams
	UartModbusUser_ReInitCoreVars.SabApply = UART_COM_SAB_APPLY_REINIT; // Active le Sablier d'application d
}

/******************************************************************************/

void checkUartUserParamsFromMemHisto(void) // Vérifier & Appliquer après chaque MST :
{
	UartCom_CopyMemory((void*)&UartModbusUser_ReInitUserParams, (void*)&UartModbusUser_ReInitRealParams, sizeof(UartReInitUserParams)); // Récupération depuis la Sauvegarde en Mémoire
	UartModbusUser_ReInitCoreVars.ReInitFlags.InitDefault = 0; // Autorise l'Init avec UserParams
	UartModbusUser_ReInitCoreVars.SabApply = UART_COM_SAB_APPLY_AT_MST; // Active le Sablier d'application différé pour la MST -> Sera Vérifié au moment d'appliquer
}

/******************************************************************************/

void USART3_Custom_Init(void) // Demande une Initialisation Custom d'après nos UserParams :
{
	if(HAL_OK != UartCom_ReInitUartFromUserParams(&UartModbusUser_ReInitUserParams)) // Si é
	{
		MX_USART3_UART_Init(); // Tente une Initialisation par la fonction CubeMx
	}
}

/******************************************************************************/

void HandleNewUartModbusUserStops(uint8_t newStops)
{
	UartModbusUser_ReInitUserParams.nbStops = newStops; // Mémorise la nouvelle valeur demandée
	UartModbusUser_ReInitCoreVars.ReInitFlags.InitDefault = 0; // Autorise l'Init avec UserParams
	UartModbusUser_ReInitCoreVars.SabApply = UART_COM_SAB_APPLY_REINIT; // Active le Sablier d'application d
}

/******************************************************************************/

void HandleValidateNewUart3Params(uint16_t key)
{
	if((UART_COM_REINIT_VALID_KEY == key) && (0 != UartModbusUser_ReInitCoreVars.ReInitFlags.CanValidate))
	{
		UartModbusUser_ReInitCoreVars.SabApply = COM_FRAME_TTL_DISABLED;	// Validate current Params
		UartCom_CopyMemory((void*)&UartModbusUser_ReInitRealParams, (void*)&UartModbusUser_ReInitUserParams, sizeof(UartReInitUserParams)); // OK pour Sauvegarde en Mémoire
	} else if(UART_COM_FORCE_REINIT_NOW == key)
	{
		UartModbusUser_ReInitCoreVars.SabApply = COM_FRAME_TTL_EXPIRED;	// Force Apply Now
	}
}
