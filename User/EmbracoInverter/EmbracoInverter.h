/*
 * EmbracoInverter.h
 *
 *  Created on: Mar 3, 2025
 *  Original Author: j.proux
 *
 *  Updated on: 6 Mar. 2025
 *  Updated by: j.proux
 *
 */

#ifndef EMBRACOINVERTER_EMBRACOINVERTER_H_
#define EMBRACOINVERTER_EMBRACOINVERTER_H_

#include "EmbracoInverterConf.h"	// Pour accès à la Configuration EmbracoInverter
#include "UartComUtils.h"			// Pour pouvoir s'appuyer sur la Librairie UART_COM

#define HZ_TO_RPM(h)		((h) * 60)

#define EMBRACO_INVERTER_TX_FOOTER_SIZE 	1

/******************************************************************************/

#define EMBRACO_INVERTER_ID_TO_EMBRACO		0xA5
#define EMBRACO_INVERTER_ID_FROM_EMBRACO	0x5A

#define EMBRACO_INVERTER_CMD_READ			0x3C
#define EMBRACO_INVERTER_CMD_WRITE			0xC3

#define EMBRACO_INVERTER_PARAM_SPEED		0x80
#define EMBRACO_INVERTER_PARAM_STATUS		0x83
#define EMBRACO_INVERTER_PARAM_POWER		0x82
#define EMBRACO_INVERTER_PARAM_NB_TRIALS	0x81
#define EMBRACO_INVERTER_PARAM_BUS_VOLT		0x84
#define EMBRACO_INVERTER_PARAM_TEMPERATURE	0x88
#define EMBRACO_INVERTER_PARAM_POWER_LIMIT	0x8A

#define EMBRACO_INVERTER_MSB_READ			0x39

// Communication Error Codes :
#define EMBRACO_INVERTER_COM_ERROR_CMD  	0xF4
#define EMBRACO_INVERTER_COM_ERROR_BYTE_3	0xF8
#define EMBRACO_INVERTER_COM_ERROR_BYTE_4	0xF0
#define EMBRACO_INVERTER_COM_ERROR_CHK  	0xF2

#define EMBRACO_INVERTER_COMPRESSOR_RUNNING	0x00 // Note 1 p46 : If the Data High byte is 00h, compressor is still running.
#define EMBRACO_INVERTER_COMPRESSOR_STOPPED	0xFF

typedef union _tEmbracoInverterManagerFlags
{
	struct
	{
		unsigned WasDetected	: 1;	// Si détecté 1 fois depuis MST
		unsigned IsConnected	: 1;	// Si réponse récente du Driver
		unsigned IsDriverLost	: 1;	// Si communication avec le Driver perdue
		unsigned RequestPending : 1;	// Pour détection non-réponses
	} __attribute__ ((__packed__));
	uint8_t AllFlags;
} tEmbracoInverterManagerFlags;

typedef union _tEmbracoInverterStatusData
{
	struct
	{
		union
		{
			struct
			{
				unsigned StartFailure	: 1;	// bit 0 <-> 01h : Start Failure
				unsigned OverLoadProtect: 1;	// bit 1 <-> 02h : Overload protection, If the Data High byte is 00h, compressor is still running.
				unsigned UnderSpeed 	: 1;	// bit 2 <-> 04h : Under speed (1550 rpm or lower)
				unsigned WrongRotorPos	: 1;	// bit 3 <-> 08h : Wrong rotor position
				unsigned ShortCircuit	: 1;	// bit 4 <-> 10h : Short circuit
				unsigned OverTemperature: 1;	// bit 5 <-> 20h : Over temperature failure : when the inverter turns off due to over temperature.
				unsigned ReservedBit6	: 1;	// bit 6 <-> 40h : Flag non affecté
				unsigned ConsOutOfSpec	: 1;	// bit 4 <-> 80h : Set speed data out of specification (cf. Notes 2 & 4)
			} __attribute__ ((__packed__));
			uint8_t LSB;
		} __attribute__ ((__packed__));
		uint8_t MSB;	// 00h <-> Compressor running / FFh <-> Compressor stopped
	} __attribute__ ((__packed__));
	uint16_t StatusData;
} tEmbracoInverterStatusData;

typedef enum {
	EmbracoInverterStepReadStatus = 0,
	EmbracoInverterStepReadSpeedCons,
	EmbracoInverterStepReadPower,
	EmbracoInverterStepReadBusVoltage,
	EmbracoInverterStepReadTemperature,
	EmbracoInverterStepReadPowerLimit,
	EmbracoInverterStepReadStartFails,
	//-----------------
#ifdef EMBRACO_INVERTER_CHECK_INVALID_CMD	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepGetErrorCommand,
#endif // EMBRACO_INVERTER_POST_INVALID_CMD
#ifdef EMBRACO_INVERTER_CHECK_INVALID_BYTE_3	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepGetErrorByte3,
#endif // EMBRACO_INVERTER_POST_INVALID_BYTE_3
#ifdef EMBRACO_INVERTER_CHECK_INVALID_BYTE_4	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepGetErrorByte4,
#endif // EMBRACO_INVERTER_POST_INVALID_BYTE_4
#ifdef EMBRACO_INVERTER_CHECK_INVALID_CHK	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepGetErrorChecksum,
#endif // EMBRACO_INVERTER_POST_INVALID_CHK
	//-----------------
#if defined(EMBRACO_INVERTER_PAUSES_IN_LOOP) && (EMBRACO_INVERTER_PAUSES_IN_LOOP > 0)	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause1,
 #ifdef __ARM_FEATURE_CMSE
	EmbracoInverterStepPauseLast = EmbracoInverterStepPause1 + EMBRACO_INVERTER_PAUSES_IN_LOOP -1,
 #else // __ARM_FEATURE_CMSE
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 1	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause2,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 1
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 2	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause3,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 2
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 3	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause4,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 3
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 4	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause5,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 4
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 5	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause6,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 5
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 6	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause7,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 6
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 7	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause8,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 7
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 8	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause9,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 8
  #if EMBRACO_INVERTER_PAUSES_IN_LOOP > 9	// cf. "EmbracoInverterConf.h"
	EmbracoInverterStepPause10,
  #endif // EMBRACO_INVERTER_PAUSES_IN_LOOP > 9
 #endif // __ARM_FEATURE_CMSE
#endif // EMBRACO_INVERTER_PAUSES_IN_LOOP
	//-----------------
	EmbracoInverterStepWriteSpeed,
	//-----------------
	EmbracoInverterStepEndOfMainLoop,
} eEmbracoInverterComStep;

typedef struct
{
	tEmbracoInverterManagerFlags Flags;
	uint8_t  nbNoReplies;
	uint8_t  nextFrameStep;			// Parmi eEmbracoInverterComStep
	uint8_t  CommunicationError;	// Parmi EMBRACO_INVERTER_COM_ERROR_*
	//---------------------
	uint16_t SpeedConsToSend;		// Speed [RPM]
	uint16_t SpeedConsRead; 		// Speed [RPM]
	//---------------------
	tEmbracoInverterStatusData StatusRead;
	uint16_t PowerRead;				// Power [W]
	uint16_t NbOfTrialsRead;		// Number of trials
	uint16_t BusVoltageRead;		// Voltage [V]
	uint16_t TemperatureX10Read;	// Temperature [°C x 10]
	uint16_t PowerLimitationRead;	// Power limitation [W]
	//---------------------
#ifdef EMBRACO_INVERTER_GET_LAST_OTHER_DATA	// cf. "EmbracoInverterConf.h"
	uint8_t  LastOtherDataType;
	uint16_t LastOtherDataValue;
#endif // EMBRACO_INVERTER_GET_LAST_OTHER_DATA
} tEmbracoInverterManager;

extern tEmbracoInverterManager EmbracoInverterManager[];

/******************************************************************************/

void InitEmbracoInverterMST(void);
void GestionEmbracoInverter(void);
void Handle_EmbracoInverter_RT_100ms(void);

uint8_t GetEmbracoManagerFlags(void);
unsigned WasEmbracoInverterDetected(void);
unsigned IsEmbracoInverterConnected(void);
unsigned IsEmbracoInverterDriverLost(void);

uint8_t GetEmbracoInverterNbNoReplies(void);
void SetEmbracoInverterNbNoReplies(uint8_t newValue);

uint8_t GetEmbracoInverterComError(void);
void SetEmbracoInverterComError(uint8_t newValue);

void SetEmbracoInverterSpeedConsRPM(uint16_t newSpeedRPM);
uint16_t GetEmbracoInverterSpeedConsRPM(void);
uint16_t GetEmbracoInverterSpeedConsRead(void);

uint8_t GetEmbracoInverterStatusFlags(void);
void RazEmbracoInverterStatusFlags(uint8_t flags2Raz);
unsigned IsEmbracoInverterStatusFlagStartFailure(void);
unsigned IsEmbracoInverterStatusFlagOverLoadProtect(void);
unsigned IsEmbracoInverterStatusFlagUnderSpeed(void);
unsigned IsEmbracoInverterStatusFlagWrongRotorPos(void);
unsigned IsEmbracoInverterStatusFlagShortCircuit(void);
unsigned IsEmbracoInverterStatusFlagOverTemperature(void);
unsigned IsEmbracoInverterStatusFlagConsOutOfSpec(void);

uint16_t GetEmbracoInverterStatus16(void);
void SetEmbracoInverterStatus16(uint16_t newStatus);
unsigned IsEmbracoCompressorRunning(void);

uint16_t GetEmbracoInverterPowerRead(void);
uint16_t GetEmbracoInverterNbOfTrialsRead(void);
uint16_t GetEmbracoInverterBusVoltageRead(void);
uint16_t GetEmbracoInverterTemperatureX10Read(void);
uint16_t GetEmbracoInverterPowerLimitationRead(void);

#ifdef EMBRACO_INVERTER_GET_LAST_OTHER_DATA	// cf. "EmbracoInverterConf.h"
	uint8_t  GetEmbracoInverterLastOtherDataType(void);
	uint16_t GetEmbracoInverterLastOtherDataValue(void);
#endif // EMBRACO_INVERTER_GET_LAST_OTHER_DATA

/******************************************************************************/
// Elements de Configuration pour "mUartComInitParam" (dans "UartComUser.c")
//  à intégrer dans la zone des "USER CODE (...) Static_InitParams" :

#define EMBRACO_INVERTER_RX_FN_HANDLER  	EmbracoInverterRxHandler

#define EMBRACO_INVERTER_EOF_RX 		28		// 28ms pour sabEndOfRxFrame (base = IT @ 1ms)
#define EMBRACO_INVERTER_EOB_RX 		56		// 56ms pour sabEndOfRxFrame lorsqu'on n'a reçu qu'un Bloc de la Trame (base = IT @ 1ms)
#define EMBRACO_INVERTER_REPLY_TO	 	(1000)	// 1s pour sabTimeOut4Reply (base = IT @ 1ms)
#define EMBRACO_INVERTER_NO_TX_INIT 	(300)	// 300ms pour sabReady4Tx (base = IT @ 1ms)
#define EMBRACO_INVERTER_NO_TX_FRAME	140		// 140ms pour sabReady4Tx (base = IT @ 1ms)
#define EMBRACO_INVERTER_NO_TX_BYTE 	0		// 0ms pour sabMayTxNextByte (base = IT @ 1ms)
#define EMBRACO_INVERTER_NO_RX_TO		(60 *10) // 60s pour sabReSetRxBufPtr (base = IT @ 100ms)

int EMBRACO_INVERTER_RX_FN_HANDLER(tRxTxBufInfo* pRxTxBI, void* pVoidParam);

/******************************************************************************/
// Elements de Configuration pour "mInitRegTx" (dans "UartComUser.c")
//  à intégrer dans la zone des "USER CODE (...) Register_RegularTx" :

#define EMBRACO_INVERTER_TX_REGULAR_FN  	EmbracoInverterRequestFactory

#define EMBRACO_INVERTER_TX_FIRST_DELAY 	(2) 	// After 300ms @ MST (Base 100ms)
#define EMBRACO_INVERTER_TX_NORMAL_DELAY 	(3) 	// 1 FrameTx / 500ms (Base 100ms)
#define EMBRACO_INVERTER_TX_DEF_FRAME_SIZE	EMBRACO_INVERTER_BUF_DEF_TX_SIZE

uint16_t EMBRACO_INVERTER_TX_REGULAR_FN(tComFrameParams* pFI, void* pVoidParam);

#endif /* EMBRACOINVERTER_EMBRACOINVERTER_H_ */
