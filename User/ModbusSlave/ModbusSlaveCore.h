/*
 * ModbusSlaveCore.h
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 25 Feb. 2025
 *  Updated by: j.proux
 *
 * Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 * -> il te faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#ifndef UART_COM_MODBUSSLAVECORE_H_
#define UART_COM_MODBUSSLAVECORE_H_

#include "ModbusSlaveConf.h"	// Pour accès à la Configuration ModbusSlave du User
#include "UartComUtils.h"		// Pour pouvoir s'appuyer sur la Librairie UART_COM

/******************************************************************************/

typedef struct _tModbusSlaveParams
{
	// Pour l'identification :
	uint16_t SrcId;     // Pour identifier le Périphérique Source de l'Evènement
	uint16_t SlaveAdr;	// Adresse(s) à laquelle doit réagir le Décodage (LowByte = Adr1, HighByte = Adr2)
	// Pour la Gestion du Niveau d'accès :
	uint16_t AccessLevel; // Niveau d'accès actuellement autorisé
	uint16_t nbPswdTries;// Pour éviter les attaques en brute force
	// Infos complémentaires :
	uint16_t lastFrameResult; // Pour Mémoire du résultat de traitement de la dernière Réception Modbus

	// Statistiques ModbusSlave :
#ifdef MODBUS_SLAVE_SUPPORT_STATS
	uint32_t nbFramesRx;
	uint32_t nbFramesRxLowSz;
	uint32_t nbFramesRxCrcOk;
	uint32_t nbFramesRxCrcErr;
	uint32_t nbFramesRxAdrOk;
	uint32_t nbFramesRxAdrErr;
	uint32_t NbTramesTx;
	// Répartition par Handler :
	uint32_t nbFramesRead03;
	uint32_t nbFramesWrit16;
	uint32_t nbFramesNoFnHand;
	uint32_t nbFramesTxExcept;
#endif // MODBUS_SLAVE_SUPPORT_STATS

} tModbusSlaveParams;

/******************************************************************************/

typedef enum {
    TVarUnknown,

    // Type Signed Char (1 Byte = 8 bits) :
    TVarSCharGetVarSetVar,
    TVarSCharGetVarSetFct,
    TVarSCharGetFctSetVar,
    TVarSCharGetFctSetFct,

    // Type Unsigned Char (1 Byte = 8 bits) :
    TVarUCharGetVarSetVar,
    TVarUCharGetVarSetFct,
    TVarUCharGetFctSetVar,
    TVarUCharGetFctSetFct,

    // Type Signed Int (2 Bytes = 1x 16 bits) :
    TVarSIntGetVarSetVar,
    TVarSIntGetVarSetFct,
    TVarSIntGetFctSetVar,
    TVarSIntGetFctSetFct,

    // Type Unsigned Int (2 Bytes = 1x 16 bits) :
    TVarUIntGetVarSetVar,
    TVarUIntGetVarSetFct,
    TVarUIntGetFctSetVar,
    TVarUIntGetFctSetFct,

    // Type Signed Long (4 Bytes = 2x 16 bits) :
    TVarSLongGetVarSetVar,
    TVarSLongGetVarSetFct,
    TVarSLongGetFctSetVar,
    TVarSLongGetFctSetFct,

    // Type Unsigned Long (4 Bytes = 2x 16 bits) :
    TVarULongGetVarSetVar,
    TVarULongGetVarSetFct,
    TVarULongGetFctSetVar,
    TVarULongGetFctSetFct,

    // Type Signed LongLong (8 Bytes = 4x 16 bits) :
    TVarSLongLongGetVarSetVar,
    TVarSLongLongGetVarSetFct,
    TVarSLongLongGetFctSetVar,
    TVarSLongLongGetFctSetFct,

    // Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
    TVarULongLongGetVarSetVar,
    TVarULongLongGetVarSetFct,
    TVarULongLongGetFctSetVar,
    TVarULongLongGetFctSetFct,

    // Type Float Int x1 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX1GetVarSetVar,
    TVarFloatIntX1GetVarSetFct,
    TVarFloatIntX1GetFctSetVar,
    TVarFloatIntX1GetFctSetFct,

    // Type Float Int x10 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX10GetVarSetVar,
    TVarFloatIntX10GetVarSetFct,
    TVarFloatIntX10GetFctSetVar,
    TVarFloatIntX10GetFctSetFct,

    // Type Float Int x100 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX100GetVarSetVar,
    TVarFloatIntX100GetVarSetFct,
    TVarFloatIntX100GetFctSetVar,
    TVarFloatIntX100GetFctSetFct,

    // Type Float Int x1000 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX1000GetVarSetVar,
    TVarFloatIntX1000GetVarSetFct,
    TVarFloatIntX1000GetFctSetVar,
    TVarFloatIntX1000GetFctSetFct,

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG
	//----------------------------------------
	// Type Float Long x10 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX10GetVarSetVar,
	TVarFloatLongX10GetVarSetFct,
	TVarFloatLongX10GetFctSetVar,
	TVarFloatLongX10GetFctSetFct,

	// Type Float Long x100 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX100GetVarSetVar,
	TVarFloatLongX100GetVarSetFct,
	TVarFloatLongX100GetFctSetVar,
	TVarFloatLongX100GetFctSetFct,

	// Type Float Long x1000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX1KGetVarSetVar,
    TVarFloatLongX1KGetVarSetFct,
    TVarFloatLongX1KGetFctSetVar,
    TVarFloatLongX1KGetFctSetFct,

	// Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX10KGetVarSetVar,
    TVarFloatLongX10KGetVarSetFct,
    TVarFloatLongX10KGetFctSetVar,
    TVarFloatLongX10KGetFctSetFct,

	// Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX100KGetVarSetVar,
    TVarFloatLongX100KGetVarSetFct,
    TVarFloatLongX100KGetFctSetVar,
    TVarFloatLongX100KGetFctSetFct,

	// Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX1MGetVarSetVar,
    TVarFloatLongX1MGetVarSetFct,
    TVarFloatLongX1MGetFctSetVar,
    TVarFloatLongX1MGetFctSetFct,

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW
	//----------------------------------------
	// Type Float Long Raw (4 Bytes = 2x 16 bits) :
	TVarFloatLongRawGetVarSetVar,
    TVarFloatLongRawGetVarSetFct,
    TVarFloatLongRawGetFctSetVar,
    TVarFloatLongRawGetFctSetFct,

	// Type Double LongLong Raw (8 Bytes = 4x 16 bits) :
	TVarDoubleLongLongRawGetVarSetVar,
    TVarDoubleLongLongRawGetVarSetFct,
    TVarDoubleLongLongRawGetFctSetVar,
    TVarDoubleLongLongRawGetFctSetFct,

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

} eTVarGetSet;

/******************************************************************************/

void InitModbusSlaveStruct(tModbusSlaveParams* pStruct, uint16_t srcId, uint16_t slaveAdr);
int ModbusSlaveRxHandler(tRxTxBufInfo* pRxTxBI, void* pVoidParam);	// Nécessite "UartComUtils.h"

extern tModbusSlaveParams	ModbusSlaveParam[UART_COM_NB_OF_MODBUS_SLAVE];

//#define MODBUS_SLAVE_UART_IHM_PARAMS	&ModbusSlaveParam[0]
//#define MODBUS_SLAVE_UART_USER_PARAMS	&ModbusSlaveParam[1]
//#define MODBUS_SLAVE_UART_EXT_PARAMS	&ModbusSlaveParam[2]
//#define MODBUS_SLAVE_USBD_CDC_PARAMS	&ModbusSlaveParam[3]
//#define MODBUS_SLAVE_USBH_CDC_PARAMS	&ModbusSlaveParam[3]	// UsbHost_ModbusSlave partage les même Params que UsbDevice_ModbusSlave

/******************************************************************************/
// Prototypes des fonctions Internes de ModbusSlave autorisées dans la Table Modbus :

void RegisterNewAccessPswd(uint16_t NewPswd);
uint16_t getCurAccessLevel(void);
uint16_t GetNullU16(void);
uint32_t GetNullU32(void);
void DiscardWriteU16(uint16_t NewValue);
void RequestRunMode4ThisModbus(uint8_t newRunMode);

/******************************************************************************/

#endif /* UART_COM_MODBUSSLAVECORE_H_ */
