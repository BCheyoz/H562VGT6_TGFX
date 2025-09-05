/*
 * BluetoothUARTConf.h
 *
 * Mini lib pour tester la connexion UART
 */

#ifndef BLE_UART_BLE_UART_CONF_H_
#define BLE_UART_BLE_UART_CONF_H_

#include "stdint.h"
#include "UartComDepUtils.h"	// Pour accès aux outils de base spécifiques aux dépendants de UartCom

typedef struct
{
	// Pour l'identification : TODO
	uint16_t SrcId;
	uint8_t  Flags;
	uint8_t  nbNoReplies;
	uint8_t  StatusRead;
	uint8_t  PairingInfo;
} tBluetoothParams;

extern tBluetoothParams BluetoothManager[];

/******************************************************************************/
#define NB_BLUETOOTH_BUF_DEF_SIZE	1	// 1 seul Embraco Inverter, sur USART3
#define BLUETOOTH_SYS_MANAGER		&BluetoothManager[0]	// Les Paramètres de Gestion de l'Inverter Embraco

#define BLE_UART_MIN_RX_SIZE		1	// 1 octet en Rx
#define BLE_UART_HEADER_TX_SIZE		9	// 9 octets pour "BLE UART:"
#define BLE_UART_MIN_TX_SIZE		BLE_UART_HEADER_TX_SIZE + BLE_UART_MIN_RX_SIZE

#define BLE_UART_MAX_RX_SIZE		16	// 5 Bytes en Rx pour l'Inverter Embraco
#define BLE_UART_MAX_TX_SIZE		BLE_UART_HEADER_TX_SIZE + BLE_UART_MAX_RX_SIZE

/******************************************************************************/
// Pour "UartComUser.c"  :

#define BLE_UART_EOF_RX 		5		// 5ms (≥ 2 ms) pour sabEndOfRxFrame (base = IT @ 1ms)
#define BLE_UART_EOB_RX 		60		// 60ms (≥ 56 ms) pour 1270 Bytes @ 115200 sur sabEndOfRxFrame, si reçu qu'1 Bloc de Trame (base = IT @ 1ms)
#define BLE_UART_REPLY_TO	 	1000	// 1s pour sabTimeOut4Reply, car Slave (base = IT @ 1ms)
#define BLE_UART_NO_TX_INIT 	10		// 10ms (entre 2 et 56 ms) pour sabReady4Tx (base = IT @ 1ms)	-> EOF_RX < NO_TX_INIT < EOB_RX
#define BLE_UART_NO_TX_FRAME	3		// 3ms (< EOF_RX, car Slave) pour sabReady4Tx (base = IT @ 1ms)	-> NO_TX_FRAME < EOB_RX (en Slave)
#define BLE_UART_NO_TX_BYTE 	0		// 0ms pour sabMayTxNextByte (base = IT @ 1ms)
#define BLE_UART_NO_RX_TO		(60 *10) // 60s pour sabReSetRxBufPtr (base = IT @ 100ms)

#define BLE_UART_DEF_REPLY_SZ		BLE_UART_MAX_TX_SIZE // Par défaut : tout l'espace TX disponible

#define BLE_UART_ERR_RX_FRAME		100	// Tempo pour sabEndOfRxFrame en cas d'erreur de Réception (base = IT @ 1ms)
#define BLE_UART_ERR_TX_RETRY		150	// Tempo pour sabMayTxNextByte : Retry en cas d'erreur d'Envoi (base = IT @ 1ms)
#define BLE_UART_ERR_TX_FRAME		100	// Tempo pour sabMayTxNextByte : Silence après erreur d'Envoi (base = IT @ 1ms)

#define BLE_UART_CHECK_ECHO 		0	// S'il faut Contrôler la Réception d'un Echo RxTX
#define BLE_UART_MAY_SYNC_TX 		0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Requête Modbus
#define BLE_UART_MAY_SYNC_REPLY 	0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Réponse Modbus
#define BLE_UART_ON_TX_STATE		1	// Etat de la Pin GPIOx lors d'une Transmission Active de Trame
#define BLE_UART_INIT_DO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_DO_LOAD_AT_MST, BLE_UART_CHECK_ECHO, BLE_UART_MAY_SYNC_TX, BLE_UART_MAY_SYNC_REPLY, BLE_UART_ON_TX_STATE) // Synthèse des Flags UartComInitFlags
#define BLE_UART_INIT_NO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_NO_LOAD_AT_MST, BLE_UART_CHECK_ECHO, BLE_UART_MAY_SYNC_TX, BLE_UART_MAY_SYNC_REPLY, BLE_UART_ON_TX_STATE) // Synthèse des Flags UartComInitFlags

#endif /* BLE_UART_BLE_UART_CONF_H_ */
