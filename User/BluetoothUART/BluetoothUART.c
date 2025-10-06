/*
 * BluetoothUART.c
 *
 * Mini lib pour tester la connexion UART
 *
 */

#include "BluetoothUART.h"	// Pour accès à nos propres déclarations publiques

tBluetoothParams BluetoothManager[NB_BLUETOOTH_BUF_DEF_SIZE] = {0};

uint8_t bleHeader[BLE_UART_HEADER_TX_SIZE] = {"BLE UART:"};

inline void InitBluetoothMST(void)
{
	// S'il y avait des Init particuliers à effectuer ...
}

/******************************************************************************/

inline void GestionBluetoothUART(void)
{
	// S'il y avait une Gestion particulière à effectuer ...
}

/******************************************************************************/

int BluetoothUART_RxHandler(tRxTxBufInfo* pRxTxBI, void* pVoidParam)
{
	if( (0 == pRxTxBI) || (0 == pVoidParam) ) return 0; // Invalid Arguments :-( !


	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	uint8_t* myTx = pRxTxBI->TxBuf.pBufBase;
	uint16_t nbBytesRecus = pRxTxBI->RxBuf.nbBytes;

	// Check Size :
	if( (nbBytesRecus < BLE_UART_MIN_RX_SIZE) || (nbBytesRecus >= BLE_UART_MAX_RX_SIZE))
	{
		return 0; // Abandonner le traitement en l'état !
	}

	CopyMemory(myTx, bleHeader, BLE_UART_HEADER_TX_SIZE);
	CopyMemory(&myTx[BLE_UART_HEADER_TX_SIZE], myRx, nbBytesRecus);

	pRxTxBI->TxBuf.nbBytes = BLE_UART_HEADER_TX_SIZE + nbBytesRecus;
	return 1;  // Envoyer la Réponse préparée
}

