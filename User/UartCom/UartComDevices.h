/*
 * UartComDevices.h
 *
 *  Created on: May 18, 2021
 *  Original Author: j.proux
 *
 *  Updated on: 26 Feb. 2025
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

#ifndef UART_COM_UARTCOMDEVICES_H_
#define UART_COM_UARTCOMDEVICES_H_

// Remarque_Jp le 22/10/2021 : Merci de conserver les commentaires, pour Exemple, svp.

/**********************************************************************************
 * Explication du "Fitful" (= alternance) d'un UART entre 1 ou plusieurs Protocoles :
 *
 * Si un UART est 100% du temps associé à un même Protocole => le compter parmi *_ONLY
 * Si un UART peut passer d'un Protocole à un autre en cours=> le compter parmi *_FITFUL
 *
 * Exemple 1 : 2x UART vers ModbusSlave + 1x UART vers ModbusSlave <-> ModbusMaster :
 * UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY 		2	// UART_A & UART_B
 * UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL		1	// UART_C (intermittent avec ModbusMaster)
 * UART_COM_NB_OF_UART_MODBUS_MASTER_ONLY		0	// 0 = non prévu
 * UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL		1	// UART_C (intermittent avec ModbusSlave)
 * UART_COM_NB_OF_UART_CHANGEABLE_USAGE			1	// UART_C (intermittent ModbusSlave <-> ModbusMaster)
 *
 * Exemple 2 : 1x USB_Device vers ModbusSlave <-> USB_Host vers ModbusSlave :
 * UART_COM_NB_OF_USB_CHANGEABLE_USAGE			1	// USB_FS (intermittent USB_Device <-> USB_Host)
 * UART_COM_NB_OF_USB_DEVICE_FITFUL 			1	// USB_FS est aussi partagé avec USB_Host
 * UART_COM_NB_OF_USB_HOST_FITFUL				1	// USB_FS est aussi partagé avec USB_Device
 * UART_COM_NB_OF_USB_DEVICE_ONLY				0	// 0 car intermittent USB_Device <-> USB_Host
 * UART_COM_NB_OF_USB_HOST_ONLY 				0	// 0 car intermittent USB_Device <-> USB_Host
 *
 * UART_COM_NB_OF_USBD_CHANGEABLE_USAGE 		0	// 0 car USB partagé entre USB_Host & USB_Device
 * UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY		0	// 0 car USB partagé entre USB_Host & USB_Device
 * UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL		1	// USB_FS_Device -> ModbusSlave partagé avec USB_Host
 * UART_COM_NB_OF_USBD_MODBUS_MASTER_ONLY		0	// 0 car Non prévu
 * UART_COM_NB_OF_USBD_MODBUS_MASTER_FTFL		0	// 0 car Non prévu
 *
 * UART_COM_NB_OF_USBH_CHANGEABLE_USAGE 		0	// 0 car USB partagé entre USB_Host & USB_Device
 * UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY		0	// 0 car USB partagé entre USB_Host & USB_Device
 * UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL		1	// USB_FS_HOST -> ModbusSlave partagé avec USB_Device
 * UART_COM_NB_OF_USBH_MODBUS_MASTER_ONLY		0	// 0 car Non prévu
 * UART_COM_NB_OF_USBH_MODBUS_MASTER_FTFL		0	// 0 car Non prévu
 *
 * Remarque : le basculement d'un Protocole à un autre (par ex : ModbusSlave <-> ModbusMaster)
 * est à la discrétion de l'Utilisateur, à l'aide des Fonctions prévues à cet effet,
 * mais ne doit en aucun cas être initié dans une Interruption, ni dans un CallBack de UartCom.
 *
 *********************************************************************************/

// Répartition des UART par usage :
#define UART_COM_NB_OF_UART_IBUS_ONLY			0	// UART1 + UART3 -> iBus
#define UART_COM_NB_OF_UART_IBUS_FITFUL 		0	// 0 car Alternance (= Fitful) avec un autre Protocole non prévu
// ...
#define UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY	1	// UART5 -> ModbusSlave User
#define UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL	0	// 0 car Alternance (= Fitful) avec un autre Protocole non prévu
// ...
#define UART_COM_NB_OF_UART_MODBUS_MASTER_ONLY	1	// UART3 -> Inverter Embraco
#define UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL	0	// 0 car Alternance (= Fitful) avec un autre Protocole non prévu
// ...
#define UART_COM_NB_OF_UART_CHANGEABLE_USAGE	0	// 0 car Non prévu


// Répartition des USB par usage :
#define UART_COM_NB_OF_USB_DEVICE_ONLY			0	// PAS de COM sur USB (Non partagé entre USB_Host & USB_Device)
#define UART_COM_NB_OF_USB_HOST_ONLY			0	// PAS de COM sur USB (Non partagé entre USB_Host & USB_Device)
// ...
#define UART_COM_NB_OF_USB_DEVICE_FITFUL		0	// PAS de COM sur USB_FS (Non partagé avec USB_Host)
#define UART_COM_NB_OF_USB_HOST_FITFUL  		0	// PAS de COM sur USB_FS (Non partagé avec USB_Device)
#define UART_COM_NB_OF_USB_CHANGEABLE_USAGE 	0	// PAS de COM sur USB_FS (Non partagé entre USB_Device et USB_Host)

// Pour partager des Ressources entre USB_Host & USB_Device :
#define UART_COM_NB_OF_USB_MODBUS_SLAVE_FTFL	0	// ModbusSlave NON partagé entre USB_Host & USB_Device
#define UART_COM_NB_OF_USB_MODBUS_MASTER_FTFL	0	// 0 car Non prévu

// Détail des USB_DEVICE par usage :
#define UART_COM_NB_OF_USBD_CHANGEABLE_USAGE	0	// 0 car USB partagé entre USB_Host & USB_Device
#define UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY	0	// 0 car USB partagé entre USB_Host & USB_Device
#define UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL	0	// USB_FS_Device -> ModbusSlave NON partagé avec USB_Host
#define UART_COM_NB_OF_USBD_MODBUS_MASTER_ONLY	0	// 0 car Non prévu
#define UART_COM_NB_OF_USBD_MODBUS_MASTER_FTFL	0	// 0 car Non prévu

// Détail des USB_HOST par usage :
#define UART_COM_NB_OF_USBH_CHANGEABLE_USAGE	0	// 0 car USB partagé entre USB_Host & USB_Device
#define UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY	0	// 0 car USB partagé entre USB_Host & USB_Device
#define UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL	0	// USB_FS_HOST -> ModbusSlave NON partagé avec USB_Device
#define UART_COM_NB_OF_USBH_MODBUS_MASTER_ONLY	0	// 0 car Non prévu
#define UART_COM_NB_OF_USBH_MODBUS_MASTER_FTFL	0	// 0 car Non prévu

// Attribution d'un ID différent pour chaque Source, Type & Protocole :
#define COM_NONE_RESERVED_NO_SRC_ID 	0	// Source = None
#define COM_UART_IBUS_USER_SRC_ID		1	// Source = Uart1
#define COM_UART_MODBUS_IHM_SRC_ID  	2	// Source = Uart2
#define COM_UART_MODBUS_USER_SRC_ID 	3	// Source = Uart3
#define COM_USBD_MODBUS_USER_SRC_ID 	4	// Source = USB_FS_Device
#define COM_USBH_MODBUS_USER_SRC_ID 	5	// Source = USB_FS_Host
#define COM_UART_MODBUS_EXT_SRC_ID  	6	// Source = Uart8
#define COM_UART_IBUS_EXT_SRC_ID		7	// Source = Uart3
#define COM_UART_INVERTER_SRC_ID		8	// Source = Uart3

// Affectation d'un Flag correspondant à chaque Source :
#define COM_IBUS_USER_SRC_FLAG			(1 << COM_UART_IBUS_USER_SRC_ID)	// Source = Uart1
#define COM_IBUS_EXT_SRC_FLAG			(1 << COM_UART_IBUS_EXT_SRC_ID) 	// Source = Uart3
// ...
#define COM_MODBUS_SLAVE_IHM_SRC_FLAG 	(1 << COM_UART_MODBUS_IHM_SRC_ID)	// Source = Uart2
#define COM_MODBUS_SLAVE_USER_SRC_FLAG	(1 << COM_UART_MODBUS_USER_SRC_ID)	// Source = Uart3
#define COM_MODBUS_SLAVE_EXT_SRC_FLAG 	(1 << COM_UART_MODBUS_EXT_SRC_ID)	// Source = Uart8
// ...
#define COM_MODBUS_SLAVE_USBD_SRC_FLAG 	(1 << COM_USBD_MODBUS_USER_SRC_ID)	// Source = USB_Device
#define COM_MODBUS_SLAVE_USBH_SRC_FLAG 	(1 << COM_USBH_MODBUS_USER_SRC_ID)	// Source = USB_Host
// ...
#define COM_SPECIAL_INVERTER_SRC_FLAG 	(1 << COM_UART_INVERTER_SRC_ID) 	// Source = Uart3

/******************************************************************************/

// Synthèse des usages prévus par Périphérique (ne pas modifer) :
#define UART_COM_NB_OF_UARTS_USED				(UART_COM_NB_OF_UART_CHANGEABLE_USAGE + UART_COM_NB_OF_UART_IBUS_ONLY + UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY + UART_COM_NB_OF_UART_MODBUS_MASTER_ONLY)
#define UART_COM_NB_OF_USB_USED					(UART_COM_NB_OF_USB_CHANGEABLE_USAGE + UART_COM_NB_OF_USB_DEVICE_ONLY + UART_COM_NB_OF_USB_HOST_ONLY)
#define UART_COM_NB_OF_USBD_ONLY_USED			(UART_COM_NB_OF_USBD_CHANGEABLE_USAGE + UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY + UART_COM_NB_OF_USBD_MODBUS_MASTER_ONLY)
#define UART_COM_NB_OF_USBH_ONLY_USED			(UART_COM_NB_OF_USBH_CHANGEABLE_USAGE + UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY + UART_COM_NB_OF_USBH_MODBUS_MASTER_ONLY)
#define UART_COM_NB_OF_DEVICES_USED				(UART_COM_NB_OF_UARTS_USED + UART_COM_NB_OF_USB_USED)
// ...
#define UART_COM_NB_MAX_OF_USBD					(UART_COM_NB_OF_USBD_ONLY_USED + UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL + UART_COM_NB_OF_USBD_MODBUS_MASTER_FTFL)
#define UART_COM_NB_MAX_OF_USBH					(UART_COM_NB_OF_USBH_ONLY_USED + UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL + UART_COM_NB_OF_USBH_MODBUS_MASTER_FTFL)

// Synthèse transversale par Usage (ne pas modifer) :
#define UART_COM_NB_OF_IBUS_ONLY				(UART_COM_NB_OF_UART_IBUS_ONLY)
#define UART_COM_NB_OF_MODBUS_SLAVE_ONLY		(UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY	+ UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY	+ UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY)
#define UART_COM_NB_OF_MODBUS_MASTER_ONLY		(UART_COM_NB_OF_UART_MODBUS_MASTER_ONLY	+ UART_COM_NB_OF_USBD_MODBUS_MASTER_ONLY + UART_COM_NB_OF_USBH_MODBUS_MASTER_ONLY)
#define UART_COM_NB_OF_CHANGEABLE_USAGE			(UART_COM_NB_OF_UART_CHANGEABLE_USAGE	+ UART_COM_NB_OF_USB_CHANGEABLE_USAGE	+ UART_COM_NB_OF_USBD_CHANGEABLE_USAGE	+ UART_COM_NB_OF_USBH_CHANGEABLE_USAGE)
// ...
#define UART_COM_NB_OF_MODBUS_SLAVE				(UART_COM_NB_OF_MODBUS_SLAVE_ONLY + UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL + UART_COM_NB_OF_USB_MODBUS_SLAVE_FTFL)
#define UART_COM_NB_OF_MODBUS_MASTER			(UART_COM_NB_OF_MODBUS_MASTER_ONLY+ UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL+ UART_COM_NB_OF_USB_MODBUS_MASTER_FTFL)
// ...
#define UART_COM_NB_MAX_OF_UART_IBUS			(UART_COM_NB_OF_UART_IBUS_ONLY + UART_COM_NB_OF_UART_IBUS_FITFUL)
#define UART_COM_NB_MAX_OF_UART_MODBUS_SLAVE	(UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY	+ UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL)
#define UART_COM_NB_MAX_OF_UART_MODBUS_MASTER	(UART_COM_NB_OF_UART_MODBUS_MASTER_ONLY + UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL)
// ...
#define UART_COM_NB_MAX_OF_USBD_MODBUS_SLAVE	(UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY	+ UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL)
#define UART_COM_NB_MAX_OF_USBD_MODBUS_MASTER	(UART_COM_NB_OF_USBD_MODBUS_MASTER_ONLY + UART_COM_NB_OF_USBD_MODBUS_MASTER_FTFL)
// ...
#define UART_COM_NB_MAX_OF_USBH_MODBUS_SLAVE	(UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY	+ UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL)
#define UART_COM_NB_MAX_OF_USBH_MODBUS_MASTER	(UART_COM_NB_OF_USBH_MODBUS_MASTER_ONLY + UART_COM_NB_OF_USBH_MODBUS_MASTER_FTFL)
// ...
#define UART_COM_NB_MAX_OF_UART_MODBUS			(UART_COM_NB_MAX_OF_UART_MODBUS_SLAVE	+ UART_COM_NB_MAX_OF_UART_MODBUS_MASTER)
#define UART_COM_NB_MAX_OF_USBD_MODBUS			(UART_COM_NB_MAX_OF_USBD_MODBUS_SLAVE	+ UART_COM_NB_MAX_OF_USBD_MODBUS_MASTER)
#define UART_COM_NB_MAX_OF_USBH_MODBUS			(UART_COM_NB_MAX_OF_USBH_MODBUS_SLAVE	+ UART_COM_NB_MAX_OF_USBH_MODBUS_MASTER)
// ...
#define UART_COM_NB_MAX_OF_IBUS					UART_COM_NB_MAX_OF_UART_IBUS
#define UART_COM_NB_MAX_OF_MODBUS_SLAVE			(UART_COM_NB_MAX_OF_UART_MODBUS_SLAVE	+ UART_COM_NB_MAX_OF_USBD_MODBUS_SLAVE + UART_COM_NB_MAX_OF_USBH_MODBUS_SLAVE)
#define UART_COM_NB_MAX_OF_MODBUS_MASTER		(UART_COM_NB_MAX_OF_UART_MODBUS_MASTER	+ UART_COM_NB_MAX_OF_USBD_MODBUS_MASTER + UART_COM_NB_MAX_OF_USBH_MODBUS_MASTER)
// ...
#define UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL	(UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL + UART_COM_NB_OF_UART_MODBUS_MASTER_FTFL)
#define UART_COM_NB_USB_MODBUS_MAST_SLAV_FTFL	(UART_COM_NB_OF_USB_MODBUS_SLAVE_FTFL + UART_COM_NB_OF_USB_MODBUS_MASTER_FTFL)
#define UART_COM_NB_MODBUS_MASTER_SLAVE_FTFL	(UART_COM_NB_UART_MODBUS_MAST_SLAV_FTFL + UART_COM_NB_USB_MODBUS_MAST_SLAV_FTFL)

#endif /* UART_COM_UARTCOMDEVICES_H_ */
