/*
 * MX25L_XSPI.h
 *
 *  Created on: May 13, 2025
 *  Original Author: j.proux
 *
 *  Updated on: 16 May 2025
 *  Updated by: j.proux
 *  Copyright © ALDES 2025
 *  LibVersion: v1.0.0
 *
 *  Pour intégrer facilement cette Librairie "MX25L_XSPI" dans un nouveau Projet :
 *   -> Suivre la Procédure décrite dans "MX25L_XSPI\README.md"
 *
 */

#pragma once
#ifndef MX25L_XSPI_MX25L_XSPI_H_
#define MX25L_XSPI_MX25L_XSPI_H_

// Activation des Fonctions XSPI autorisées :
//#define MEM_MX25L_XSPI_SUPPORT_2_LINES  		// Pour activer le support des fonctions en DualMode
#define MEM_MX25L_XSPI_SUPPORT_4_LINES  		// Pour activer le support des fonctions en QuadMode
//#define MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS 	// Pour activer le support des fonctions Read "Signature" (RES) & "Manufacturer & Device ID" (REMS)
//#define MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE		// Pour activer le support du ChipErase
//#define MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND // Pour activer le support des fonctions Suspend & Resume Program
//#define MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND	// Pour activer le support des fonctions Suspend & Resume Erase
//#define MEM_MX25L_XSPI_SUPPORT_DEEP_POWER		// Pour activer le support des fonctions de Deep Power
//#define MEM_MX25L_XSPI_SUPPORT_SECURED_OTP	// Pour activer le support des fonctions de Secure OTP
//#define MEM_MX25L_XSPI_SUPPORT_DISCOVER_PARAMS // Pour activer le support de la fonction de DiscoverParameter
//#define MEM_MX25L_XSPI_SUPPORT_WRSCUR			//!\ WARNING : Writing SecurityRegister locks down the Secured OTP (LDSO) /!\ !

// Opérations de Configuration à effectuer lors de l'Init :
#define MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT		1	// Vérif_Jp = OK sur MX25L6433F le 15/05/2025 pour Forçage à 0 ou 1.
#define MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT  	0	// Vérif_Jp = OK sur MX25L6433F le 15/05/2025 pour Forçage à 0 ou 1.
//#define MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT		0	// Default is 0 /!\ WARNING : This bit is OTP if set to '1' /!\ !
//#define MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT 			0	// "Output Driver Strength" default's value is 0.

// Config du Périphérique XSPI à utiliser :
#define MEM_MX25L_XSPI_PERIF_HANDLE 	&hospi1
#define MEM_MX25L_XSPI_PERIF_INIT() 	MX_OCTOSPI1_Init()				// OCTOSPI1
#define MEM_MX25L_XSPI_CS_INIT()		MEM_MX25L_XSPI_DEACTIVATE_CS()	// Force IDLE @ Init

//// Si le ChipSelect n'était pas géré en automatique par le Périphérique :
//#define MEM_MX25L_XSPI_CS_PORT  		Flash_Qspi_CS_GPIO_Port	// GPIOE
//#define MEM_MX25L_XSPI_CS_PIN			Flash_Qspi_CS_Pin		// P11

#define MEM_MX25L_XSPI_CS_ACTIVE		GPIO_PIN_RESET	// Memory is Active on Low
#define MEM_MX25L_XSPI_CS_INACTIVE		GPIO_PIN_SET	// Memory is IDLE when High

// Config des TimeOut pour les fonctions XSPI :
#define MEM_MX25L_XSPI_SEND_TO			100U            // Send TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_XSPI_RECV_TO			100U            // Receive TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_XSPI_POLL_TO			1500U			// Polling TimeOut : 1500ms = 1.5s

// Config des valeurs de retour XSPI :
#define MEM_MX25L_XSPI_RETURN_SUCCESS	HAL_OK
#define MEM_MX25L_XSPI_RETURN_FAILURE	HAL_ERROR

// Config des valeurs de réponses XSPI :
#define MEM_MX25L_XSPI_RETURN_READY		HAL_OK
#define MEM_MX25L_XSPI_RETURN_BUSY		HAL_BUSY
#define MEM_MX25L_XSPI_RETURN_ERROR		HAL_ERROR

// Config des réponses de Test XSPI :
#define MEM_MX25L_XSPI_RETURN_TRUE		(uint8_t)UINT8_MAX	// 0xFF
#define MEM_MX25L_XSPI_RETURN_FALSE 	(uint8_t)INT8_MIN	// 0x80

// Config des Fonctions HAL pour XSPI :
#define MEM_MX25L_XSPI_PERIF_SEND_COMMAND(pCmd) 	HAL_XSPI_Command(MEM_MX25L_XSPI_PERIF_HANDLE, pCmd, MEM_MX25L_XSPI_SEND_TO)
#define MEM_MX25L_XSPI_PERIF_SEND_STREAM(pTxBuf)	HAL_XSPI_Transmit(MEM_MX25L_XSPI_PERIF_HANDLE, pTxBuf, MEM_MX25L_XSPI_SEND_TO)
#define MEM_MX25L_XSPI_PERIF_RECV_STREAM(pRxBuf)	HAL_XSPI_Receive( MEM_MX25L_XSPI_PERIF_HANDLE, pRxBuf, MEM_MX25L_XSPI_RECV_TO)
#define MEM_MX25L_XSPI_PERIF_AUTO_POLLING(pCfg) 	HAL_XSPI_AutoPolling(MEM_MX25L_XSPI_PERIF_HANDLE, pCfg, MEM_MX25L_XSPI_POLL_TO)

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

#include <stdint.h>				// Pour les types "int*_t" & "uint*_t"

// Prototypes Publics :
void Mem_MX25L_XSPI_Init(void);

// Read the Status & Config Register :
uint8_t Mem_MX25L_XSPI_ReadStatusRegister(uint8_t *pStatusRegister);	// RDSR
uint8_t Mem_MX25L_XSPI_IsWriteBusy();
uint8_t Mem_MX25L_XSPI_IsWriteEnabled();
uint8_t Mem_MX25L_XSPI_Wait4WriteNotBusy();
uint8_t Mem_MX25L_XSPI_ReadConfigRegister(uint8_t *pConfigRegister);	// RDCR

// Write the Status & Config Register :
uint8_t Mem_MX25L_XSPI_WriteStatusRegister(uint8_t newStatusRegister);	// WRSR
uint8_t Mem_MX25L_XSPI_WriteStatusConfigRegister(uint8_t newStatusRegister, uint8_t newConfigRegister);
uint8_t Mem_MX25L_XSPI_WriteStatusConfigRegisterArray(uint8_t* pNewStatusConfigRegister, uint8_t newStatusConfigRegisterSize);

// Read the Device & Manufacturer IDs :
uint8_t Mem_MX25L_XSPI_ReadIdRegister(void *pID_24bits); // RDID
uint8_t Mem_MX25L_XSPI_ReadElectronicID(uint8_t *pElectronicID); // RES
uint8_t Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(uint8_t Adr, uint8_t *pManufacturerAndDeviceID); // REMS

// Read & Write the Security Register :
uint8_t Mem_MX25L_XSPI_ReadSecurityRegister(uint8_t *pSecurityRegister); // RDSCUR
//uint8_t Mem_MX25L_XSPI_WriteSecurityRegister(void);	// WRSCUR (Voluntarily disabled by Jp on 15/05/2025)

// Read Datas from the Memory :
uint8_t Mem_MX25L_XSPI_ReadDataBytes(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf);	// READ
uint8_t Mem_MX25L_XSPI_ReadDataBytes_HighSpeed(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf);	// FAST_READ
uint8_t Mem_MX25L_XSPI_ReadDataBytes_DualRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf); // DREAD
uint8_t Mem_MX25L_XSPI_ReadDataBytes_TwoRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf); // 2READ
uint8_t Mem_MX25L_XSPI_ReadDataBytes_QuadRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf); // QREAD
uint8_t Mem_MX25L_XSPI_ReadDataBytes_FourRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf); // 4READ

// Erase Sector, Bloc or Chip :
uint8_t Mem_MX25L_XSPI_SectorErase4K(uint32_t baseAdr_24bits);	// SE
uint8_t Mem_MX25L_XSPI_BlockErase64K(uint32_t baseAdr_24bits);	// BE
uint8_t Mem_MX25L_XSPI_BlocErase32K(uint32_t baseAdr_24bits);	// BE32K
uint8_t Mem_MX25L_XSPI_ChipErase(void);	// CE

// Write Data to the Memory :
uint8_t Mem_MX25L_XSPI_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write);	// PP
uint8_t Mem_MX25L_XSPI_WriteArray_QuadWrite(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write); // 4PP
uint8_t Mem_MX25L_XSPI_WriteEnable(void);	// WREN
uint8_t Mem_MX25L_XSPI_WriteDisable(void);	// WRDI

// Suspend & Resume Program :
uint8_t Mem_MX25L_XSPI_SuspendProgram(void);	// PGM
uint8_t Mem_MX25L_XSPI_ResumeProgram(void); 	// PGM

// Suspend & Resume Erase :
uint8_t Mem_MX25L_XSPI_SuspendErase(void);	// ERS
uint8_t Mem_MX25L_XSPI_ResumeErase(void);	// ERS

// Deep Power :
uint8_t Mem_MX25L_XSPI_EnterDeepPowerDown(void);	// DP
uint8_t Mem_MX25L_XSPI_ReleaseFromDeepPower(void);	// RDP

// Secure OTP :
uint8_t Mem_MX25L_XSPI_EnterSecuredOTP(void);	// ENSO
uint8_t Mem_MX25L_XSPI_ExitSecuredOTP(void);	// EXSO

// Misc :
uint8_t Mem_MX25L_XSPI_NoOperation(void); // NOP
uint8_t Mem_MX25L_XSPI_DoSoftwareReset(void); // RSTEN & RST
uint8_t MemMX25L_XSPI_SetBurstLength(uint8_t newBurstLength); // SBL
uint8_t Mem_MX25L_XSPI_ReadDiscoverableParameter(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf); // RDSFDP
#define Mem_MX25L_XSPI_WriteSecurityRegister	Mem_MX25L_XSPI_NoOperation // (Voluntarily discarded by Jp on 15/05/2025)

// Sélection du ChipSelect (Manuel ou Auto) :
#if defined(MEM_MX25L_XSPI_CS_PORT) && defined(MEM_MX25L_XSPI_CS_PIN)
	#define MEM_MX25L_XSPI_ACTIVATE_CS()     HAL_GPIO_WritePin(MEM_MX25L_XSPI_CS_PORT, MEM_MX25L_XSPI_CS_PIN, MEM_MX25L_XSPI_CS_ACTIVE)
	#define MEM_MX25L_XSPI_DEACTIVATE_CS()   HAL_GPIO_WritePin(MEM_MX25L_XSPI_CS_PORT, MEM_MX25L_XSPI_CS_PIN, MEM_MX25L_XSPI_CS_INACTIVE)
#else // ! MEM_MX25L_XSPI_CS_PORT || ! MEM_MX25L_XSPI_CS_PIN
	#define MEM_MX25L_XSPI_ACTIVATE_CS()     // Nothing to Do !
	#define MEM_MX25L_XSPI_DEACTIVATE_CS()   // Nothing to Do !
#endif // MEM_MX25L_XSPI_CS_PORT & MEM_MX25L_XSPI_CS_PIN

// Redirections de Disponibilité 4_Lines -> Read 2_Line & Write 1_Line :
#ifndef MEM_MX25L_XSPI_SUPPORT_4_LINES
	#define Mem_MX25L_XSPI_ReadDataBytes_QuadRead	Mem_MX25L_XSPI_ReadDataBytes_DualRead
	#define Mem_MX25L_XSPI_ReadDataBytes_FourRead	Mem_MX25L_XSPI_ReadDataBytes_TwoRead
	#define Mem_MX25L_XSPI_WriteArray_QuadWrite 	Mem_MX25L_XSPI_WriteArray
#endif // MEM_MX25L_XSPI_SUPPORT_4_LINES

// Redirections de Disponibilité 2_Lines -> 1_Line :
#ifndef MEM_MX25L_XSPI_SUPPORT_2_LINES
	#define Mem_MX25L_XSPI_ReadDataBytes_DualRead	Mem_MX25L_XSPI_ReadDataBytes_HighSpeed
	#define Mem_MX25L_XSPI_ReadDataBytes_TwoRead	Mem_MX25L_XSPI_ReadDataBytes_HighSpeed
#endif // MEM_MX25L_XSPI_SUPPORT_2_LINES

#ifdef __cplusplus
}
#endif

#endif /* MX25L_XSPI_MX25L_XSPI_H_ */
