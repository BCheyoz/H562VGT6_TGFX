/*
 * MX25L_OCTOSPI.h
 *
 *  Created on: Apr 29, 2025
 *      Author: m.faget
 */

#ifndef MX25L_SPI_MX25L_OCTOSPI_H_
#define MX25L_SPI_MX25L_OCTOSPI_H_

#include "MX25L_SPI.h"

#include "spi.h"
#include "main.h"

#define MEM_MX25L_PERIF_SEND_XSPI_COMMAND(pCmdStruct)	HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, pCmdStruct, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)// MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_SEND_XSPI_STREAM(pTxBuf)		HAL_XSPI_Transmit(MEM_MX25L_PERIF_HANDLE, pTxBuf, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_RECV_XSPI_STREAM(pRxBuf)		HAL_XSPI_Receive(MEM_MX25L_PERIF_HANDLE, pRxBuf, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_RECV_TO)
#define MEM_MX25L_PERIF_AUTO_XSPI_POLLING(pCfg)	HAL_XSPI_AutoPolling(MEM_MX25L_PERIF_HANDLE, pCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_POLL_TO)

// tests
typedef union _tJedecInfos
{
    uint32_t U24bits;
    struct {
        uint8_t ManufacturerID;
        uint8_t DeviceType;
        uint8_t DeviceID;
        uint8_t FoundMemType;
    };
} tJedecInfos;


uint8_t MX25_xspi_ReadIDReg(void *pID_24bits);
uint8_t MX25_xspi_ReadStatusReg(void *pID_8bits);
uint8_t MX25_xspi_ReadConfigReg(void *pID_16bits);


//static void Configure_APMemory(void);
void MX25L_xspi_Init(void);
uint8_t MX25L_xspi_WriteEnable(void);
uint8_t MX25L_xspi_WriteDisable(void);
uint8_t MX25L_xspi_Wait4WriteNotBusy(void);
uint32_t MX25_xspi_PageProgram(uint32_t baseAdr_24bits,void *pArray2Write, uint32_t nbBytes2Write);
uint8_t MX25_ReadDataBytes(uint32_t Address, void *Value, uint32_t nbBytes2Read);

void Test_memory_init(void);
void Test_memory_in_init(void);
void Test_memory(void);


#endif /* MX25L_SPI_MX25L_OCTOSPI_H_ */
