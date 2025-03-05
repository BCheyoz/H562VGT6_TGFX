/*
 * EmbracoInverter.c
 *
 *  Created on: Mar 3, 2025
 *  Original Author: j.proux
 *
 *  Updated on: 4 Mar. 2025
 *  Updated by: j.proux
 *
 */

#include "EmbracoInverter.h"	// Pour accès à nos propres déclarations publiques

//// ToDo: déplacer dans .h :
//#define DEF_SAB_SPEED_RAMP	(5 * 10) // 5s, par pas de 100ms
//#define MAX_CHANGE_SPEED_HZ 	2	 // 2Hz
//#define MAX_CHANGE_SPEED_RPM	HZ_TO_RPM(MAX_CHANGE_SPEED_HZ)

//uint16_t sabSpeedRamp = 0;
//uint16_t maxSpeedRamp = 0;
//uint16_t curSpeedRamp = 0;
//uint16_t curSpeedCons = 0;
//uint16_t newSpeedCons = 0;

tEmbracoInverterDatas EmbracoInverterDatas[NB_EMBRACO_INVERTER_BUF_DEF_SIZE] = {0};

uint16_t EmbracoInverterFinalizeRequest(tComFrameParams* TxFrame, uint16_t nbBytes);

inline void InitEmbracoInverterMST(void)
{
//	sabSpeedRamp = 0;
//	maxSpeedRamp = MAX_CHANGE_SPEED;
//	curSpeedRamp = 0;
//	curSpeedCons = 0;
//	newSpeedCons = 0;
}

inline void GestionEmbracoInverter(void)
{
/*
	if(0 == sabSpeedRamp) // C'est le moment d'appliquer la Rampe, si besoin :
	{
		if(curSpeedRamp != newSpeedCons)
		{
			if(newSpeedCons >= MIN_SPEED_START_RPM) // Demande Inverter ON :
			{
				if(curSpeedRamp < MIN_SPEED_START_RPM) // Actuellement OFF :
				{
#ifdef EMBRACO_INVERTER_CAN_START_ANY_SPEED
					curSpeedRamp = newSpeedCons; // If can Start at Any Speed
#else // !EMBRACO_INVERTER_CAN_START_ANY_SPEED
					curSpeedRamp = MIN_SPEED_START_RPM; // Start @ Min Speed
#endif // EMBRACO_INVERTER_CAN_START_ANY_SPEED
				} else if(newSpeedCons > (curSpeedRamp + MAX_CHANGE_SPEED_RPM))
				{
					curSpeedRamp += MAX_CHANGE_SPEED_RPM; // Limite la pente
				} else if((newSpeedCons + MAX_CHANGE_SPEED_RPM) < curSpeedRamp)
				{
					curSpeedRamp -= MAX_CHANGE_SPEED_RPM; // Limite la pente
				} else // As requested :
				{
					curSpeedRamp = newSpeedCons;
				}
			} else {	// Demande Inverter OFF :
#ifdef EMBRACO_INVERTER_CAN_STOP_ANY_SPEED
				curSpeedRamp = 0;
#else // !EMBRACO_INVERTER_CAN_STOP_ANY_SPEED

#endif // EMBRACO_INVERTER_CAN_STOP_ANY_SPEED
			}
			if(newSpeedCons > (curSpeedRamp + MAX_CHANGE_SPEED_RPM))
			{
				newSpeedCons = curSpeedRamp + MAX_CHANGE_SPEED_RPM;
			} else if(newSpeedCons < (curSpeedRamp - MAX_CHANGE_SPEED_RPM))
		}
		if(curSpeedCons != newSpeedCons)
		{

		}
	}
//*/
}

uint16_t EmbracoInverterRequestFactory(tComFrameParams* pFI, void* pVoidParam)
{
	if( (0 == pFI) || (0 == pVoidParam) ) return 0; // Invalid Arguments :-( !
	UART_MAKE_VAR_AND_CAST_VALUE(tEmbracoInverterDatas*, pInvDatas, pVoidParam);	// Récupère nos Paramètres

	// Process noReplies => TimeOut :
	if(pInvDatas->Flags.RequestPending)
	{
		if(pInvDatas->nbNoReplies < UINT8_MAX) pInvDatas->nbNoReplies++;
//	} else {
//		pInvDatas->nbNoReplies = 0; // RAZ compteur
	}

	// Build new Request Frame :
	UART_MAKE_VAR_AND_CAST_VALUE(uint8_t*, pBuf, pFI->pBufBase);	// Sur le Buffer fourni
	UartComPushDataByte(pBuf, EMBRACO_INVERTER_ID_TO_EMBRACO);

	switch(pInvDatas->nextFrameStep)
	{
	case EmbracoInverterStepReadSpeedCons:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_SPEED);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	case EmbracoInverterStepReadPower:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_POWER);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	case EmbracoInverterStepReadBusVoltage:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_BUS_VOLT);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	case EmbracoInverterStepReadTemperature:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_TEMPERATURE);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	case EmbracoInverterStepReadPowerLimit:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_POWER_LIMIT);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	case EmbracoInverterStepReadStartFails:	// TX OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_NB_TRIALS);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		break;
	//-----------------
	case EmbracoInverterStepWriteSpeed:	// TX speed 0 = OK
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_WRITE);
		UartComPushDataUIntLE(pBuf, pInvDatas->SpeedConsToSend);
		break;
	//-----------------
	case EmbracoInverterStepReadStatus:	// TX OK
	default:
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_CMD_READ);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_PARAM_STATUS);
		UartComPushDataByte(pBuf, EMBRACO_INVERTER_MSB_READ);
		pInvDatas->nextFrameStep = EmbracoInverterStepReadStatus;
		break;
	}
	pInvDatas->nextFrameStep++;
	pInvDatas->Flags.RequestPending = 1;
	return EmbracoInverterFinalizeRequest(pFI, EMBRACO_INVERTER_MIN_TX_SIZE);
}

int EmbracoInverterRxHandler(tRxTxBufInfo* pRxTxBI, void* pVoidParam)
{
	if( (0 == pRxTxBI) || (0 == pVoidParam) ) return 0; // Invalid Arguments :-( !
	UART_MAKE_VAR_AND_CAST_VALUE(tEmbracoInverterDatas*, pInvDatas, pVoidParam);	// Récupère nos Paramètres

	uint8_t* myRx = pRxTxBI->RxBuf.pBufBase;
	uint16_t nbBytesRecus = pRxTxBI->RxBuf.nbBytes;

	// Check Size and Emitter :
	if( (nbBytesRecus != EMBRACO_INVERTER_MIN_RX_SIZE) || (*myRx != EMBRACO_INVERTER_ID_FROM_EMBRACO) )
	{
		return 0; // Abandonner le traitement en l'état !
	}

	// Check CHK :
	uint16_t data = 0;
	while(nbBytesRecus-- > 0)
	{
		data += myRx[nbBytesRecus];
	}
	if(0 != (data & UINT8_MAX))	// Erreur de CheckSum :
	{
		return 0; // Abandonner le traitement en l'état !
	}

	// OK, this Frame is Valid :
	pInvDatas->Flags.RequestPending = 0;
	pInvDatas->nbNoReplies = 0; // RAZ compteur

	// Process Frame Content :
	data = JOIN_8_8_LE(myRx[2], myRx[3]);
	switch(myRx[1]) // Command :
	{
	case EMBRACO_INVERTER_PARAM_SPEED:
		pInvDatas->SpeedConsRead = data;	// Rx OK : 0 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_NB_TRIALS:
		pInvDatas->NbOfTrialsRead = data;	// Rx OK : vu 0 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_POWER:
		pInvDatas->PowerRead = data;	// Rx OK : 0 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_STATUS:
		pInvDatas->StatusRead.StatusData = data; // Rx OK : FF00 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_BUS_VOLT:
		pInvDatas->BusVoltageRead = data;	// Rx OK : 317 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_TEMPERATURE:
		pInvDatas->TemperatureX10Read = data;	// Rx OK : 469 à l'arrêt
		break;
	case EMBRACO_INVERTER_PARAM_POWER_LIMIT:
		pInvDatas->PowerLimitationRead = data;	// Rx OK : 0 à l'arrêt
		break;
	default:
#ifdef EMBRACO_INVERTER_GET_LAST_OTHER_DATA
		pInvDatas->LastOtherDataType = myRx[1];
		pInvDatas->LastOtherDataValue = data;
#endif // EMBRACO_INVERTER_GET_LAST_OTHER_DATA
		break;
	}
	return 0;	// Aucune réponse à renvoyer
}

inline void Handle_EmbracoInverter_RT_100ms(void)
{
#define DEC_NOT_EMPTY_VAR(v)	if((v) > 0) (v)--
//	DEC_NOT_EMPTY_VAR(sabSpeedRamp);
}

void SetEmbracoInverterSpeedRPM(uint16_t newSpeedRPM)
{
	EmbracoInverterDatas[0].SpeedConsToSend = newSpeedRPM;
	EmbracoInverterDatas[0].nextFrameStep = EmbracoInverterStepWriteSpeed;
}

uint16_t EmbracoInverterFinalizeRequest(tComFrameParams* TxFrame, uint16_t nbBytes)
{
	if(0 == TxFrame) return 0;
	if(EMBRACO_INVERTER_MIN_TX_SIZE > nbBytes) return 0;
	if(TxFrame->nbBytes < nbBytes) // Buffer alloué insuffisant :-( !
	{
//#ifdef IBUS_SUPPORT_STATS
//		if(UINT32_MAX > iBusErrTxBufTooSmall) { iBusErrTxBufTooSmall++; } // Pour les Stats
//#endif // IBUS_SUPPORT_STATS
		 return 0;
	}

	// Vérifications fonctionnelles :
	uint8_t* pBase = TxFrame->pBufBase;
	void* curPtr = pBase + nbBytes - EMBRACO_INVERTER_TX_FOOTER_SIZE;

	// Calcul du CheckSum :
	uint8_t chkSum = 0;
	while((void*)pBase < curPtr)
	{
		chkSum += (*pBase++);
	}

	UartComPushDataByte(curPtr, 0x100 - chkSum);

	TxFrame->nbBytes = nbBytes;
	return nbBytes;
}
