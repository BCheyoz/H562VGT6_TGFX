/*
 * UartComUser.h
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

#ifndef UART_COM_UARTCOMUSER_H_
#define UART_COM_UARTCOMUSER_H_

// Remarque_Jp le 22/10/2021 : Merci de conserver les commentaires, pour Exemple, svp.

/* USER CODE BEGIN Includes */

#include "usart.h"  		// Pour accès aux Variables & Fonctions d'Init UART & USART
//#include "usb_device.h" 	// Pour accès aux Variables & Fonctions d'Init USB_DEVICE
//#include "usb_host.h"		// Pour accès aux Variables & Fonctions d'Init USB_HOST

/* USER CODE END Includes */

//#if defined(NB_OF_COM_IBUS) && (NB_OF_COM_IBUS > 0)
//	UART_COM_MAKE_STRUCT_BUF(tIbusBuffer, IBUS_RX_SIZE, IBUS_TX_SIZE);
//	extern tIbusBuffer iBusBuf[NB_OF_COM_IBUS];
//#endif // NB_OF_COM_IBUS

//#if defined(NB_OF_COM_MODBUS) && (NB_OF_COM_MODBUS > 0)
//	UART_COM_MAKE_STRUCT_BUF(tModbusBuffer, MODBUS_RX_SIZE, MODBUS_TX_SIZE);
//	extern tModbusBuffer ModbusBuf[NB_OF_COM_MODBUS];
//#endif // NB_OF_COM_MODBUS

/********************************************************************************************/

// Configuration des Liens vers les Périphériques UARTs & USBx :

/* USER CODE BEGIN LinkToDevices */

//#define UART_IBUS_INT		&huart1
//#define UART_MODBUS_IHM 	&huart1
//#define UART_IBUS_EXT		&huart3
#define UART_MODBUS_USER	&huart5
//#define UART_MODBUS_EXT 	&huart8
#define UART_COM_INVERTER	&huart3

//#define USBD_MODBUS 		&hUsbDeviceFS
//#define USBD_CDC_DEVICE 	&hUsbDeviceFS	// CDC sur USB_Device via UsbDevice_FullSpeed
//#define USBH_CDC_DEVICE 	&hUsbHostFS		// CDC sur USB_Host via UsbHost_FullSpeed

/* USER CODE END LinkToDevices */

/********************************************************************************************/

void UartCom_InitUserMST(void);				// Pour Initialiser les UartCom Statiques @ MST

/******************************************************************************/
// Pour alterner ModbusSlave sur UART_IHM <-> ModbusMaster sur UART_IHM :
// -> Merci de conserver pour Exemple, svp.

/*
void ManageModbusIhmMode(void); // ATTENTION : Ne jamais appeler dans une IT, ni sur commande Modbus directe !

uint8_t ModbusIhmGetCurMode(void);			// Pour savoir quel est le Mode actuel
void ModbusIhmSetNewMode(uint8_t newMode);	// Pour Demander un Mode en particulier

void LoadUartIhmModbusSlave(void);  		// Pour activer le ModbusSlave sur UART_IHM
void UnloadUartIhmModbusSlave(void); 		// Pour libérer le ModbusSlave sur UART_IHM

void LoadUartIhmModbusMaster(void);  		// Pour activer le ModbusMaster sur UART_IHM
void UnloadUartIhmModbusMaster(void);		// Pour libérer le ModbusMaster sur UART_IHM
*/

/******************************************************************************/
// Pour alterner ModbusSlave sur USB_Device <-> ModbusSlave sur USB_Host :
// -> Merci de conserver pour Exemple, svp.

// Exported functions prototypes :
//void Init_UART_Com_User_Devices(void);
//void Init_UART_Com_User_Handlers(void);

void UartCom_InitUserMST(void);

// Ajout_Jp le 16/04/2024 pour Ticket #33 :
void initUartUserParamsFromMemHisto(void);
void checkUartUserParamsFromMemHisto(void);
void USART3_Custom_Init(void);			// Pour Initialiser l'UART3 avec des Paramètres User Custom
void HandleNewUart3Stops(uint8_t newStops); // Pour Activer le changement de BaudRate/Parity & Stops
void HandleValidateNewUart3Params(uint16_t key); // Pour Valider le changement de BaudRate/Parity & Stops

/*
void LoadUsbDeviceModbusSlave(void);		// Pour activer le ModbusSlave sur USB_Device
void UnloadUsbDeviceModbusSlave(void);  	// Pour libérer le ModbusSlave sur USB_Device

void LoadUsbHostModbusSlave(void);  		// Pour activer le ModbusSlave sur USB_Host
void UnloadUsbHostModbusSlave(void);		// Pour libérer le ModbusSlave sur USB_Host
*/

/******************************************************************************/

#endif /* UART_COM_UARTCOMUSER_H_ */
