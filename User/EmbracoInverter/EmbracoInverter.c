/*
 * EmbracoInverter.c
 *
 *  Created on: Mar 3, 2025
 *  Original Author: j.proux
 *
 *  Updated on: 6 Mar. 2025
 *  Updated by: j.proux
 *
 */

#include "EmbracoInverter.h"	// Pour accès à nos propres déclarations publiques


/******************************************************************************/
// Variables de Gestion de l'Inverter Embraco :

tEmbracoInverterManager EmbracoInverterManager[NB_EMBRACO_INVERTER_BUF_DEF_SIZE] = {0};

/******************************************************************************/
// Prototypes privés :

uint16_t EmbracoInverterFinalizeRequest(tComFrameParams* TxFrame, uint16_t nbBytes);

/******************************************************************************/

inline void InitEmbracoInverterMST(void)
{
	// S'il y avait des Init particuliers à effectuer ...
}

/******************************************************************************/

inline void GestionEmbracoInverter(void)
{
	// S'il y avait une Gestion particulière à effectuer ...
}

/******************************************************************************/

uint16_t EmbracoInverterRequestFactory(tComFrameParams* pFI, void* pVoidParam)
{
	if( (0 == pFI) || (0 == pVoidParam) ) return 0; // Invalid Arguments :-( !
	UART_MAKE_VAR_AND_CAST_VALUE(tEmbracoInverterManager*, pInvDatas, pVoidParam);	// Récupère nos Paramètres

	// Process noReplies => TimeOut :
	if(pInvDatas->Flags.RequestPending)
	{
		if(pInvDatas->nbNoReplies < UINT8_MAX) pInvDatas->nbNoReplies++;
		if(pInvDatas->nbNoReplies > EMBRACO_INVERTER_MAX_ALLOW_NO_REPLY)
		{
			pInvDatas->Flags.IsConnected = 0;
			pInvDatas->Flags.IsDriverLost = 1;
			if(0 != pInvDatas->SpeedConsToSend)
			{
				pInvDatas->SpeedConsToSend = 0; // Force une demande d'arrêt
				pInvDatas->nextFrameStep = EmbracoInverterStepWriteSpeed; // Force l'envoi immédiat
			}
		}
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

/******************************************************************************/

int EmbracoInverterRxHandler(tRxTxBufInfo* pRxTxBI, void* pVoidParam)
{
	if( (0 == pRxTxBI) || (0 == pVoidParam) ) return 0; // Invalid Arguments :-( !
	UART_MAKE_VAR_AND_CAST_VALUE(tEmbracoInverterManager*, pInvDatas, pVoidParam);	// Récupère nos Paramètres

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

	// OK, this Frame is Valid => Update Flags :
	pInvDatas->Flags.RequestPending = 0;
	pInvDatas->Flags.WasDetected = 1;
	pInvDatas->Flags.IsConnected = 1;
	pInvDatas->Flags.IsDriverLost = 0;
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

/******************************************************************************/

inline void Handle_EmbracoInverter_RT_100ms(void)
{
#define DEC_NOT_EMPTY_VAR(v)	if((v) > 0) (v)--
//	DEC_NOT_EMPTY_VAR(sabSpeedRamp);
}

/******************************************************************************/

uint8_t GetEmbracoManagerFlags(void)
{
	return EmbracoInverterManager[0].Flags.AllFlags;
}

unsigned WasEmbracoInverterDetected(void)
{
	return EmbracoInverterManager[0].Flags.WasDetected;
}

unsigned IsEmbracoInverterConnected(void)
{
	return EmbracoInverterManager[0].Flags.IsConnected;
}

unsigned IsEmbracoInverterDriverLost(void)
{
	return EmbracoInverterManager[0].Flags.IsDriverLost;
}

/******************************************************************************/

uint8_t GetEmbracoInverterNbNoReplies(void)
{
	return EmbracoInverterManager[0].nbNoReplies;
}

void SetEmbracoInverterNbNoReplies(uint8_t newValue)
{
	EmbracoInverterManager[0].nbNoReplies = newValue;
}

/******************************************************************************/

uint8_t GetEmbracoInverterComError(void)
{
	return EmbracoInverterManager[0].CommunicationError;
}

void SetEmbracoInverterComError(uint8_t newValue)
{
	EmbracoInverterManager[0].CommunicationError = newValue;
}

/******************************************************************************/

void SetEmbracoInverterSpeedConsRPM(uint16_t newSpeedRPM)
{
	if(EmbracoInverterManager[0].SpeedConsToSend != newSpeedRPM)
	{
		EmbracoInverterManager[0].SpeedConsToSend = newSpeedRPM;
		EmbracoInverterManager[0].nextFrameStep = EmbracoInverterStepWriteSpeed;
	}
}

uint16_t GetEmbracoInverterSpeedConsRPM(void)
{
	return EmbracoInverterManager[0].SpeedConsToSend;
}

/******************************************************************************/

uint16_t GetEmbracoInverterSpeedConsRead(void)
{
	return EmbracoInverterManager[0].SpeedConsRead;
}

/******************************************************************************/

uint8_t GetEmbracoInverterStatusFlags(void)
{
	return EmbracoInverterManager[0].StatusRead.LSB;
}

void RazEmbracoInverterStatusFlags(uint8_t flags2Raz)
{
	EmbracoInverterManager[0].StatusRead.LSB &= (~flags2Raz);
}

//#define MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(f)	unsigned IsEmbracoInverterStatusFlag##f##(void) { return EmbracoInverterManager[0].StatusRead.##f##; }
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(StartFailure)	// bit 0 <-> 01h : Start Failure
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(OverLoadProtect)	// bit 1 <-> 02h : Overload protection, If the Data High byte is 00h, compressor is still running.
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(UnderSpeed)	// bit 2 <-> 04h : Under speed (1550 rpm or lower)
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(WrongRotorPos)	// bit 3 <-> 08h : Wrong rotor position
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(ShortCircuit)	// bit 4 <-> 10h : Short circuit
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(OverTemperature)	// bit 5 <-> 20h : Over temperature failure : when the inverter turns off due to over temperature.
//MAKE_IS_EMBRACO_INVERTER_STATUS_FLAG_FN(ConsOutOfSpec)	// bit 4 <-> 80h : Set speed data out of specification (cf. Notes 2 & 4)

unsigned IsEmbracoInverterStatusFlagStartFailure(void)		{ return EmbracoInverterManager[0].StatusRead.StartFailure; }
unsigned IsEmbracoInverterStatusFlagOverLoadProtect(void)	{ return EmbracoInverterManager[0].StatusRead.OverLoadProtect; }
unsigned IsEmbracoInverterStatusFlagUnderSpeed(void)		{ return EmbracoInverterManager[0].StatusRead.UnderSpeed; }
unsigned IsEmbracoInverterStatusFlagWrongRotorPos(void) 	{ return EmbracoInverterManager[0].StatusRead.WrongRotorPos; }
unsigned IsEmbracoInverterStatusFlagShortCircuit(void)  	{ return EmbracoInverterManager[0].StatusRead.ShortCircuit; }
unsigned IsEmbracoInverterStatusFlagOverTemperature(void)	{ return EmbracoInverterManager[0].StatusRead.OverTemperature; }
unsigned IsEmbracoInverterStatusFlagConsOutOfSpec(void) 	{ return EmbracoInverterManager[0].StatusRead.ConsOutOfSpec; }

/******************************************************************************/

unsigned IsEmbracoCompressorRunning(void) 	{ return EMBRACO_INVERTER_COMPRESSOR_RUNNING == EmbracoInverterManager[0].StatusRead.MSB ? 1 : 0; }

uint16_t GetEmbracoInverterStatus16(void)
{
	return EmbracoInverterManager[0].StatusRead.StatusData;
}

void SetEmbracoInverterStatus16(uint16_t newStatus)
{
	EmbracoInverterManager[0].StatusRead.StatusData = newStatus;
}

/******************************************************************************/

//#define MAKE_GET_EMBRACO_INVERTER_VALUE(t,n)	t GetEmbracoInverter##n(void) { return EmbracoInverterManager[0].##n; }
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, PowerRead)			// Power [W]
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, NbOfTrialsRead)		// Number of trials
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, BusVoltageRead)		// Voltage [V]
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, TemperatureX10Read)	// Temperature [°C x 10]
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, PowerLimitationRead)	// Power limitation [W]

uint16_t  GetEmbracoInverterPowerRead(void) { return EmbracoInverterManager[0].PowerRead; }
uint16_t  GetEmbracoInverterNbOfTrialsRead(void) { return EmbracoInverterManager[0].NbOfTrialsRead; }
uint16_t  GetEmbracoInverterBusVoltageRead(void) { return EmbracoInverterManager[0].BusVoltageRead; }
uint16_t  GetEmbracoInverterTemperatureX10Read(void) { return EmbracoInverterManager[0].TemperatureX10Read; }
uint16_t  GetEmbracoInverterPowerLimitationRead(void) { return EmbracoInverterManager[0].PowerLimitationRead; }

#ifdef EMBRACO_INVERTER_GET_LAST_OTHER_DATA
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint8_t,  LastOtherDataType)
//MAKE_GET_EMBRACO_INVERTER_VALUE(uint16_t, LastOtherDataValue)

uint8_t  GetEmbracoInverterLastOtherDataType(void) { return EmbracoInverterManager[0].LastOtherDataType; }
uint16_t  GetEmbracoInverterLastOtherDataValue(void) { return EmbracoInverterManager[0].LastOtherDataValue; }
#endif // EMBRACO_INVERTER_GET_LAST_OTHER_DATA

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
