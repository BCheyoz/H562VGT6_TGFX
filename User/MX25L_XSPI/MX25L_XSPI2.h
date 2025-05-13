/*
 * MX25L_XSPI.h
 *
 *  Created on: May 13, 2025
 *      Author: j.proux
 */

#pragma once
#ifndef MX25L_XSPI_MX25L_XSPI_H_
#define MX25L_XSPI_MX25L_XSPI_H_


#define MEM_MX25L_XSPI_PERIF_HANDLE 	&hospi1
#define MEM_MX25L_XSPI_PERIF_INIT() 	MX_OCTOSPI1_Init()				// OCTOSPI1
#define MEM_MX25L_XSPI_CS_INIT()		MEM_MX25L_XSPI_DEACTIVATE_CS()	// Force IDLE @ Init

#define MEM_MX25L_XSPI_CS_PORT			Flash_Qspi_CS_GPIO_Port	// GPIOE
#define MEM_MX25L_XSPI_CS_PIN			Flash_Qspi_CS_Pin		// P11

#define MEM_MX25L_XSPI_CS_ACTIVE		GPIO_PIN_RESET	// Active on Low
#define MEM_MX25L_XSPI_CS_INACTIVE		GPIO_PIN_SET	// IDLE when High

#define MEM_MX25L_XSPI_SEND_TO			100U            // Send TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_XSPI_RECV_TO			100U            // Receive TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_XSPI_POLL_TO			1500U			// Polling TimeOut : 1500ms


#define MEM_MX25L_XSPI_RETURN_SUCCESS    HAL_OK
#define MEM_MX25L_XSPI_RETURN_FAILURE    HAL_ERROR

#define MEM_MX25L_XSPI_ACTIVATE_CS()     HAL_GPIO_WritePin(MEM_MX25L_XSPI_CS_PORT, MEM_MX25L_XSPI_CS_PIN, MEM_MX25L_XSPI_CS_ACTIVE)
#define MEM_MX25L_XSPI_DEACTIVATE_CS()   HAL_GPIO_WritePin(MEM_MX25L_XSPI_CS_PORT, MEM_MX25L_XSPI_CS_PIN, MEM_MX25L_XSPI_CS_INACTIVE)

#define MEM_MX25L_XSPI_PERIF_SEND_COMMAND(pCmdStruct)	HAL_XSPI_Command(MEM_MX25L_XSPI_PERIF_HANDLE, pCmdStruct, MEM_MX25L_XSPI_SEND_TO)
#define MEM_MX25L_XSPI_PERIF_SEND_STREAM(pTxBuf)		HAL_XSPI_Transmit(MEM_MX25L_XSPI_PERIF_HANDLE, pTxBuf, MEM_MX25L_XSPI_SEND_TO)
#define MEM_MX25L_XSPI_PERIF_RECV_STREAM(pRxBuf)		HAL_XSPI_Receive( MEM_MX25L_XSPI_PERIF_HANDLE, pRxBuf, MEM_MX25L_XSPI_RECV_TO)
//#define MEM_MX25L_XSPI_PERIF_AUTO_POLLING(pCmdStr,pCfg)	HAL_XSPI_AutoPolling(MEM_MX25L_XSPI_PERIF_HANDLE, pCfg, MEM_MX25L_XSPI_POLL_TO)
#define MEM_MX25L_XSPI_PERIF_AUTO_POLLING(pCfg) 		HAL_XSPI_AutoPolling(MEM_MX25L_XSPI_PERIF_HANDLE, pCfg, MEM_MX25L_XSPI_POLL_TO)

// MX25L Read Commands (from "MX25L6433F" v1.9 du 09/04/2025 p15) :
#define MEM_MX25L_CMD_READ_DATA_BYTES       0x03 // READ : Normal Read (+3 bytes for 24bits Address)
#define MEM_MX25L_CMD_FAST_READ_DATA_BYTES  0x0B // FAST_READ : Fast Read (requires 8 Dummy cycles = 5th byte)
#define MEM_MX25L_CMD_TWO_READ_DATA_BYTES	0xBB // 2READ : 2x I/O read (requires 4 or 8 Dummy cycles) -> n bytes read out by 2 x I/O
#define MEM_MX25L_CMD_DUAL_READ_DATA_BYTES	0x3B // DREAD : 1I / 2O read (requires 8 Dummy cycles = 5th byte) -> n bytes read out by Dual Output
#define MEM_MX25L_CMD_FOUR_READ_DATA_BYTES	0xEB // 4READ : 4x I/O read (requires 6 or 10 Dummy cycles) -> Quad I/O read with configurable dummy cycles
#define MEM_MX25L_CMD_QUAD_READ_DATA_BYTES	0x6B // QREAD : 1I / 4O read

// MX25L Other Commands (from "MX25L6433F" v1.9 du 09/04/2025 p16) :
#define MEM_MX25L_CMD_WRITE_ENABLE          0x06 // WREN : Write Enable -> sets the (WEL) write enable latch bit
#define MEM_MX25L_CMD_WRITE_DISABLE         0x04 // WRDI : Write Disable -> resets the (WEL) write enable latch bit
#define MEM_MX25L_CMD_READ_STATUS_REGISTER  0x05 // RDSR : Read Status Register
#define MEM_MX25L_CMD_READ_CONFIG_REGISTER  0x15 // RDCR : Read Configuration Register
#define MEM_MX25L_CMD_WRITE_STATUS_CFG_REG  0x01 // WRSR : Write Status and Config Register (+2 bytes values)
#define MEM_MX25L_CMD_QUAD_PAGE_PROG		0x38 // 4PP : Quad Page Program (+3 bytes for 24bits Address)
#define MEM_MX25L_CMD_SECTOR_ERASE_4K       0x20 // SE4K : Sector Erase 4K (+3 bytes for 24bits Address)

#define MEM_MX25L_CMD_BLOC_ERASE_32K        0x52 // BE32K : Bloc Erase 32K (+3 bytes for 24bits Address)
#define MEM_MX25L_CMD_BLOC_ERASE_64K        0xD8 // BE64K : Bloc Erase 64K (+3 bytes for 24bits Address)
#define MEM_MX25L_CMD_CHIP_ERASE            0x60 // CE : Chip Erase
#define MEM_MX25L_CMD_CHIP_ERASE_ALT        0xC7 // CE : (other) Chip Erase alternative command
#define MEM_MX25L_CMD_PAGE_PROGRAM          0x02 // PP : Page Program (+3 bytes for 24bits Address) -> Warning : Page Boundary = 256 Bytes !
#define MEM_MX25L_CMD_ENTER_DEEP_POWER      0xB9 // DP : Enter Deep Power Down Mode
#define MEM_MX25L_CMD_RELEASE_DEEP_POWER    0xAB // RDP : Release from Deep Power Down Mode
#define MEM_MX25L_CMD_PROGRAM_SUSPEND       0x75 // PGM Suspend : Program operation is interrupted by suspend command
#define MEM_MX25L_CMD_ERASE_SUSPEND         0xB0 // ERS Suspend : Erase operation is interrupted by suspend command

#define MEM_MX25L_CMD_PROGRAM_RESUME        0x7A // PGM Resume : Continue performing Program operation
#define MEM_MX25L_CMD_ERASE_RESUME          0x30 // ERS Resume : Continue performing Erase operation
#define MEM_MX25L_CMD_READ_IDENTIFICATION   0x9F // RDID : Read Identification -> JEDEC ID : 1 byte "Manufacturer ID" + 2 bytes "Device ID"
#define MEM_MX25L_CMD_READ_ELECTRONIC_ID    0xAB // RES : Read Electronic ID -> 1 byte "Device ID"
#define MEM_MX25L_CMD_READ_MFG_DEV_ID       0x90 // REMS : Read Electronic Manufacturer & device ID
#define MEM_MX25L_CMD_ENTER_SECURE_OTP      0xB1 // ENSO : Enter Secured OTP
#define MEM_MX25L_CMD_EXIT_SECURE_OTP       0xC1 // EXSO : Exit Secured OTP
#define MEM_MX25L_CMD_WRITE_SECURITY_REG    0x2F // WRSCUR : Write Security Register -> Set the lockdown bit as "1" (once lockdown, cannot be update)

// MX25L Other Commands (from "MX25L6433F" v1.9 du 09/04/2025 p17) :
#define MEM_MX25L_CMD_READ_SECURITY_REG     0x2B // RDSCUR : Read Security Register
#define MEM_MX25L_CMD_RESET_ENABLE          0x66 // RSTEN : Reset Enable
#define MEM_MX25L_CMD_DO_SOFT_RESET         0x99 // RST : Do Software Reset
#define MEM_MX25L_CMD_READ_DISCOV_PARAMS	0x5A // RDSFDP : Read Serial Flash Discoverable Parameter -> n bytes read out (requires 3 bytes for 24bits Address + 8 Dummy cycles = 5th byte)
#define MEM_MX25L_CMD_SET_BURST_LENGTH      0xC0 // SBL : Set Burst Length
#define MEM_MX25L_CMD_SET_BURST_LENGTH_ALT  0x77 // SBL : (other)Set Burst Length alternative command
#define MEM_MX25L_CMD_NO_OPERATION          0x00 // NOP : No Operation


#ifdef __cplusplus
extern "C" {
#endif

// Prototypes Publics :
void Mem_MX25L_XSPI_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* MX25L_XSPI_MX25L_XSPI_H_ */
