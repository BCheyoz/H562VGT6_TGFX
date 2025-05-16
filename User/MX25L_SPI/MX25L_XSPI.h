/*
 * MX25L_XSPI.h
 *
 *  Created on: Apr 29, 2025
 *      Author: m.faget
 */

#ifndef MX25L_SPI_MX25L_XSPI_H_
#define MX25L_SPI_MX25L_XSPI_H_

#include "MX25L_SPI.h"

#include "spi.h"
#include "main.h"

#define MEM_MX25L_PERIF_SEND_XSPI_COMMAND(pCmdStruct)	HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, pCmdStruct, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)// MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_SEND_XSPI_STREAM(pTxBuf)		HAL_XSPI_Transmit(MEM_MX25L_PERIF_HANDLE, pTxBuf, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_RECV_XSPI_STREAM(pRxBuf)		HAL_XSPI_Receive(MEM_MX25L_PERIF_HANDLE, pRxBuf, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_RECV_TO)
#define MEM_MX25L_PERIF_AUTO_XSPI_POLLING(pCfg)	HAL_XSPI_AutoPolling(MEM_MX25L_PERIF_HANDLE, pCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)//MEM_MX25L_POLL_TO)

//static void Configure_APMemory(void);
void MX25L_xspi_Init(void);

uint8_t MX25L_xspi_WriteEnable(void);// WREN
uint8_t MX25L_xspi_WriteDisable(void);// WRDI
uint8_t MX25L_xspi_Wait4WriteNotBusy(void);

uint8_t MX25_xspi_ReadIDReg(void *pID_24bits);// RDID
uint8_t MX25_xspi_ReadStatusReg(void *pID_8bits);// RDSR
uint8_t MX25_xspi_ReadConfigReg(void *pID_8bits);// RDCR
/*
 * Table : Status Register
 * +------+-------+------+------+------+------+------+------+
 * | bit7 | bit6  | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
 * +------+-------+------+------+------+------+------+------+
 * |SRWD  | QE    | BP3  | BP2  | BP1  | BP0  | WEL  | WIP  |
 * +------+-------+------+------+------+------+------+------+
 * | (1:0)| (1:0) |	     |      |      |      | (1:0)| (1:0)|
 * +------+-------+------+------+------+------+------+------+
 *
 * SRWD -> Status Register Write Disable
 * QE -> Quad Enable
 * BPx -> level of protect block -> voir page 10 MX25L6433F datasheet
 * WEL -> Write enable latch
 * WIP -> Write in progress bit
 * NVB -> Non volatile bit
 *
 *  Table : Configuration Register
 * +------+-------+------+------+------+------+------+------+
 * | bit7 | bit6  | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
 * +------+-------+------+------+------+------+------+------+
 * |  X   | DC    | X    | X    | TB   | X    | X    | ODS  |
 * +------+-------+------+------+------+------+------+------+
 * |  X   | vl    | X    | X    | OTP  | X    | X    | vl   |
 * +------+-------+------+------+------+------+------+------+
 *
 * DC -> Dummy cycle
 * TB -> top/bottom selected for protect area
 * ODS -> Output driver strength
 * OTP ->
 * vl -> volatile
 * X -> Reserved
 *
 */

uint8_t MX25_xspi_WriteStatusReg(uint8_t StatusRegisterValue);// WRSR 01h

uint8_t MX25_xspi_ReadDataBytes(uint32_t Address, void *Value, uint32_t nbBytes2Read);// READ 03h
uint8_t MX25_xspi_FastReadDataBytes(uint32_t Address, void *Value, uint32_t nbBytes2Read);// FAST_READ 0Bh
uint8_t MX25_xspi_DualReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read);// DREAD 3Bh
uint8_t MX25_xspi_TwoReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read);// 2READ BBh
uint8_t MX25_xspi_QuadReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read);// QREAD 6Bh
uint8_t MX25_xspi_FourReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read);// 4READ EBh

uint8_t MX25_xspi_BurstRead(void *Value, uint32_t nbBytes2Read);//todo SBL Read or write ? C0h or 77h

uint8_t MX25L_xspi_SectorErase4K(uint32_t baseAdr_24bits);// SE 20h
uint8_t MX25L_xspi_BlocErase64K(uint32_t baseAdr_24bits);// BE D8h
uint8_t MX25L_xspi_BlocErase32K(uint32_t baseAdr_24bits);// BE32K 52h
uint8_t MX25L_xspi_ChipErase(void);// CE 60h or C7h

uint8_t MX25_xspi_PageProgram(uint32_t baseAdr_24bits, void *pArray2Write, uint32_t nbBytes2Write);// PP 02h
uint8_t MX25_xspi_FourPageProgram(uint32_t baseAdr_24bits, void *pArray2Write, uint32_t nbBytes2Write);// 4PP 38h

uint8_t MX25L_xspi_DeepPowerDown(void);//todo DP B9h
uint8_t MX25L_xspi_ReleaseDeepPowerDown(void);//todo RDP ABh
uint8_t MX25_xspi_ReadElecSign(void *pID_8bits);// RES ABh
uint8_t MX25_xspi_ReadElecManufacturerIdDeviceId(void *pID_16bits);// REMS 90h

/*
 *  Table 9 ID Definitions : Configuration Register
 * +--------------+------------------------------------------------+
 * | Command Type |              MX25L6433F                        |
 * +--------------+-----------------+-------------+----------------+
 * |     RDID     | Manufacturer ID | Memory Type | Memory Density |
 * |              |-----------------+-------------+----------------+
 * |              |        C2       |     20      |       17       |
 * +--------------+-----------------+-------------+----------------+
 * |     RES      |             Electronic Signature ID            |
 * |              |------------------------------------------------+
 * |              |                       16                       |
 * +--------------+-----------------+-------------+----------------+
 * |     REMS     | Manufacturer ID | Device ID   |                |
 * |              |-----------------+-------------+----------------+
 * |              |        C2       |     16      |                |
 * +--------------+-----------------+-------------+----------------+
 */

uint8_t MX25L_xspi_EnterSecuredOTP(void);//todo ENSO B1h
uint8_t MX25L_xspi_ExitSecuredOTP(void);//todo EXSO C1h

uint8_t MX25_xspi_ReadSecurityReg(void *pID_16bits);// RDSCUR 2Bh

/*
*  Table 10 : Security Register Definition
* +------+--------+--------+--------+--------+--------+--------+--------+
* | bit7 |  bit6  |  bit5  |  bit4  |  bit3  |  bit2  |  bit1  |  bit0  |
* +------+--------+--------+--------+--------+--------+--------+--------+
* |  X   | E_FAIl | P_FAIL |   X    |   ESB  |   PSB  |   LDSO | OTP    |
* +------+--------+--------+--------+--------+--------+--------+--------+
* |  X   |  (0:1) |  (0:1) |   X    |  (0:1) |  (0:1) |  (0:1) |  (0:1) |
* +------+--------+--------+--------+--------+--------+--------+--------+
* |  nvb |  vb    |  vb    |  vb    |  vb    |  vb    | nvb    |  nvb   |
* +------+--------+--------+--------+--------+--------+--------+--------+
* |  X   |  RO    |  RO    |   X    |  RO    |  RO    | OTP    |  RO    |
* +------+--------+--------+--------+--------+--------+--------+--------+
* ESB -> Erase Suspend status
* PSB -> Program Suspend Status
* LDSO -> Lock Down 1st 4kbit Secured OTP
* OTP -> Secured OTP indicator
* X -> Reserved
* nvb -> non-volatile bit
* vb -> volatile bit
* RO -> Read Only
*/

uint8_t MX25L_xspi_WriteSecurityReg(void);//todo WRSCUR 2Fh

uint8_t MX25L_xspi_ProgramSuspend(void);//todo 75h
uint8_t MX25L_xspi_EraseSuspend(void);//todo B0h

uint8_t MX25L_xspi_ProgramResume(void);//todo 7Ah
uint8_t MX25L_xspi_EraseResume(void);//todo 30h

uint8_t MX25L_xspi_NoOperation(void);//todo NOP 30h
uint8_t MX25L_xspi_SoftReset(void);//todo RSTEN 66h & RST 99h -> 2 commands : RSTEN & RST

uint8_t MX25_xspi_ReadSerialFlashDiscovParameter(void *pID_8bits);//todo RDSFDP 5Ah


// added functions
uint8_t MX25_xspi_WriteStatusAndConfigReg(uint8_t StatusRegisterValue, uint8_t ConfigRegisterValue);

uint8_t MX25L_xspi_Enable_QuadMode(void);
uint8_t MX25L_xspi_Disable_QuadMode(void);

void MX25L_xspi_Exit_HPM(void);// hardware protection mode
void MX25L_xspi_DisableAllBlockProtection(void);// d blocks protection



void Test_memory_init(void);
void Test_memory_in_init(void);
void Test_memory(void);


#endif /* MX25L_SPI_MX25L_XSPI_H_ */
