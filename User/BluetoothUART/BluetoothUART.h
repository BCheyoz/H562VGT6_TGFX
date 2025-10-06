/*
 * BluetoothUART.h
 *
 * Mini lib pour tester la connexion UART
 *
 */

#ifndef BLE_UART_BLE_UART_H_
#define BLE_UART_BLE_UART_H_

#include "BluetoothUARTConf.h"		// Pour accès à la Configuration
#include "UartComUtils.h"			// Pour pouvoir s'appuyer sur la Librairie UART_COM

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
void InitBluetoothMST(void);
void GestionBluetoothUART(void);
/******************************************************************************/

#define BLE_UART_RX_FN_HANDLER  	BluetoothUART_RxHandler

int BLE_UART_RX_FN_HANDLER(tRxTxBufInfo* pRxTxBI, void* pVoidParam);

#ifdef __cplusplus
}
#endif

#endif /* BLE_UART_BLE_UART_H_ */
