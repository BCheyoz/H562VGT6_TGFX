/*
 * MX25L_XSPI.c
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

#include "MX25L_XSPI2.h"

#include "octospi.h"
#include "main.h"
#include <string.h> // pour MemSet


#if defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_MX25L_XSPI)
	#define MX25L_XSPI_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
//	#warning "DEBUG_MX25L_XSPI is Active !!!"
#else // (! __DEBUG) && (! DEBUG_MX25L_XSPI) :
	#define MX25L_XSPI_HALT_IF_DEBUG()	// Nop();
#endif // __DEBUG ; DEBUG_MX25L_XSPI

#define MEM_MX25L_XSPI_CLEAR_STRUCT(Struct)	memset(&Struct, 0, sizeof(Struct))

// Flags du StatusRegister d'après la Table 5 du "MX25L6433F" v1.9 du 09/04/2025 p23 :
#define MEM_MX25L_XSPI_RDSR_WIP 	(1 << 0)	// bit0 : WIP "Write in Progress" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_WEL 	(1 << 1)	// bit1 : WEL "Write Enable Latch" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_BP0 	(1 << 2)	// bit2 : BP0 "Block Protect 0" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_BP1 	(1 << 3)	// bit3 : BP1 "Block Protect 1" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_BP2 	(1 << 4)	// bit4 : BP2 "Block Protect 2" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_BP3 	(1 << 5)	// bit5 : BP3 "Block Protect 3" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_BPS 	(MEM_MX25L_XSPI_RDSR_BP0 | MEM_MX25L_XSPI_RDSR_BP1 | MEM_MX25L_XSPI_RDSR_BP2 | MEM_MX25L_XSPI_RDSR_BP3)	// bits [5:2] : "Block Protect" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_QE  	(1 << 6)	// bit6 : QE "Quad Enable" du StatusRegister
#define MEM_MX25L_XSPI_RDSR_SRWD	(1 << 7)	// bit7 : SRWD "Status Register Write Disable" du StatusRegister

// Flags du ConfigRegister d'après la Table 6 du "MX25L6433F" v1.9 du 09/04/2025 p24 :
#define MEM_MX25L_XSPI_RDCR_ODS 	(1 << 0)	// bit0 : ODS "OutputDriver Strength" du ConfigRegister
#define MEM_MX25L_XSPI_RDCR_TB	 	(1 << 3)	// bit3 : TB "Top/Bottom selected" du ConfigRegister
#define MEM_MX25L_XSPI_RDCR_DC  	(1 << 6)	// bit6 : DC "Dummy Cycle" du ConfigRegister

// Macros principales de renseignement de la structure XSPI_RegularCmdTypeDef :
#define MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE(XSPI_Cmd,OpType)					XSPI_Cmd.OperationType = OpType;
#define MEM_MX25L_BUILD_XSPI_CMD_INSTRUCTION(XSPI_Cmd,Mode,Width,DTR,Val)	XSPI_Cmd.InstructionMode = Mode; XSPI_Cmd.InstructionWidth = Width; XSPI_Cmd.InstructionDTRMode = DTR; XSPI_Cmd.Instruction = Val
#define MEM_MX25L_BUILD_XSPI_CMD_INSTR_8_BITS_NO_DTR(XSPI_Cmd,Mode,Val)		XSPI_Cmd.InstructionMode = Mode; XSPI_Cmd.Instruction = Val
#define MEM_MX25L_BUILD_XSPI_CMD_ADDRESS(XSPI_Cmd,Mode,Width,DTR,Val)		XSPI_Cmd.AddressMode = Mode; XSPI_Cmd.AddressWidth = Width; XSPI_Cmd.AddressDTRMode = DTR;XSPI_Cmd.Address = Val
#define MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(XSPI_Cmd,Mode,Width,Val)		XSPI_Cmd.AddressMode = Mode; XSPI_Cmd.AddressWidth = Width; XSPI_Cmd.Address = Val
#define MEM_MX25L_BUILD_XSPI_CMD_ALT_BYTES(XSPI_Cmd,Mode,Width,DTR,Val) 	XSPI_Cmd.AlternateBytesMode = Mode; XSPI_Cmd.AlternateBytesWidth = Width; XSPI_Cmd.AlternateBytesDTRMode = DTR; XSPI_Cmd.AlternateBytes = Val
#define MEM_MX25L_BUILD_XSPI_CMD_ALT_BYTES_NO_DTR(XSPI_Cmd,Mode,Width,Val)	XSPI_Cmd.AlternateBytesMode = Mode; XSPI_Cmd.AlternateBytesWidth = Width; XSPI_Cmd.AlternateBytes = Val
#define MEM_MX25L_BUILD_XSPI_CMD_DATA(XSPI_Cmd,Mode,Length, DTR)			XSPI_Cmd.DataMode = Mode; XSPI_Cmd.DataLength = Length; XSPI_Cmd.DataDTRMode = DTR
#define MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(XSPI_Cmd,Mode,Length)			XSPI_Cmd.DataMode = Mode; XSPI_Cmd.DataLength = Length
#define MEM_MX25L_BUILD_XSPI_CMD_DUMMY(XSPI_Cmd,Val)						XSPI_Cmd.DummyCycles = Val
#define MEM_MX25L_BUILD_XSPI_CMD_DQS(XSPI_Cmd,Val)							XSPI_Cmd.DQSMode = Val
#define MEM_MX25L_BUILD_XSPI_CMD_SIOO(XSPI_Cmd,Mode)						XSPI_Cmd.SIOOMode = Mode

// Macros dérivées pour le renseignement de la structure XSPI_RegularCmdTypeDef :
#define MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(XSPI_Cmd)					MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE(XSPI_Cmd, HAL_XSPI_OPTYPE_COMMON_CFG)
#define MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(XSPI_Cmd,Val)					MEM_MX25L_BUILD_XSPI_CMD_INSTR_8_BITS_NO_DTR(XSPI_Cmd, HAL_XSPI_INSTRUCTION_1_LINE, Val)
//#define MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE_8_BITS_NO_DTR(XSPI_Cmd,Val)	MEM_MX25L_BUILD_XSPI_CMD_INSTRUCTION(XSPI_Cmd, HAL_XSPI_INSTRUCTION_1_LINE, HAL_XSPI_INSTRUCTION_8_BITS)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(XSPI_Cmd)						MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(XSPI_Cmd, HAL_XSPI_ADDRESS_NONE, HAL_XSPI_ADDRESS_8_BITS, 0)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(XSPI_Cmd)						MEM_MX25L_BUILD_XSPI_CMD_ALT_BYTES_NO_DTR(XSPI_Cmd, HAL_XSPI_ALT_BYTES_NONE, HAL_XSPI_ALT_BYTES_8_BITS, 0)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_DATA(XSPI_Cmd)							MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(XSPI_Cmd, HAL_XSPI_DATA_NONE, 0)
#define MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(XSPI_Cmd,Length)				MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(XSPI_Cmd, HAL_XSPI_DATA_1_LINE, Length)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(XSPI_Cmd)							MEM_MX25L_BUILD_XSPI_CMD_DUMMY(XSPI_Cmd, 0)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(XSPI_Cmd)							MEM_MX25L_BUILD_XSPI_CMD_DQS(XSPI_Cmd, HAL_XSPI_DQS_DISABLE)
#define MEM_MX25L_BUILD_XSPI_CMD_SIOO_EACH_CMD(XSPI_Cmd)					MEM_MX25L_BUILD_XSPI_CMD_SIOO(XSPI_Cmd, HAL_XSPI_SIOO_INST_EVERY_CMD)
#define MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(XSPI_Cmd)					MEM_MX25L_BUILD_XSPI_CMD_SIOO(XSPI_Cmd, HAL_XSPI_SIOO_INST_ONLY_FIRST_CMD)

// Macros de renseignement de la structure XSPI_AutoPollingTypeDef :
#define MEM_MX25L_BUILD_XSPI_CMP_PARAMS(XSPI_AtPol,Mode,Msk,Val)			XSPI_AtPol.MatchMode = Mode; XSPI_AtPol.MatchMask = Msk; XSPI_AtPol.MatchValue = Val
#define MEM_MX25L_BUILD_XSPI_CMP_RETRY(XSPI_AtPol,Delay,AutoStop)			XSPI_AtPol.AutomaticStop = AutoStop; XSPI_AtPol.IntervalTime = Delay

#ifdef __cplusplus
extern "C" {
#endif

// Prototypes Privés :
static uint8_t Mem_MX25L_XSPI_UpdateConfigMemory(void);
static uint8_t Mem_MX25L_XSPI_SendThisCommand(uint8_t Cmd2Send);
static uint8_t Mem_MX25L_XSPI_SendCmdAndArray(uint8_t Cmd2Send, void* pArray2Send, uint16_t nbBytes2Send);
static uint8_t Mem_MX25L_XSPI_SendCmdReceiveArray(uint8_t Cmd2Send, uint16_t nbBytes2Read, void* pArray2Receive);
static uint8_t Mem_MX25L_XSPI_SendWriteCmdAndAddress(uint8_t WriteCmd2Send, uint32_t baseAdr_24bits);

void Mem_MX25L_XSPI_Init(void)
{
	// Init Hardware :
	MEM_MX25L_XSPI_CS_INIT();
//	HAL_GPIO_WritePin(GPIOA, Flash_Qspi_IO3_Pin|Flash_Qspi_IO2_Pin, GPIO_PIN_SET); // Tant qu'on est en mode DUAL uniquement !
	MEM_MX25L_XSPI_PERIF_INIT();

    // Init Software :
	Mem_MX25L_XSPI_UpdateConfigMemory();
//	Mem_MX25L_XSPI_DoSoftwareReset();	// Si besoin de DEBUG de la Mémoire XSPI !
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_UpdateConfigMemory(void)
{
#if defined(MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT) \
	|| defined(MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT)
  #if defined(MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT)
	#define STATUS_CONFIG_REGISTER_SIZE	2
  #else // ! MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT && ! MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT && ! MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT
	#define STATUS_CONFIG_REGISTER_SIZE	1
  #endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT & MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT & MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT

	uint8_t StatusConfigRegister[STATUS_CONFIG_REGISTER_SIZE] = {0};
	uint8_t retVal = Mem_MX25L_XSPI_ReadStatusRegister(&StatusConfigRegister[0]); // Read Status RDSR
	uint8_t nb2Write = 0;

  #if defined(MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT) || defined(MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT)
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
	{
		retVal = Mem_MX25L_XSPI_ReadConfigRegister(&StatusConfigRegister[1]); // Read Config RDCR
	}
  #endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT & MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT

	if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
	{
		// QuadEnable sur StatusRegister :
  #ifdef MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT
		if((0 != (StatusConfigRegister[0] & MEM_MX25L_XSPI_RDSR_QE)) ^ (0 != MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT))
		{
	#if 0 == MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT // Désactiver QuadEnable :
			StatusConfigRegister[0] &= ~MEM_MX25L_XSPI_RDSR_QE;
	#else // Activer QuadEnable :
			StatusConfigRegister[0] |= MEM_MX25L_XSPI_RDSR_QE;
	#endif //
			nb2Write = 1; // At least the StatusRegister
		}
  #endif // MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT

		// ODS sur ConfigRegister :
  #ifdef MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT
		if((0 != (StatusConfigRegister[1] & MEM_MX25L_XSPI_RDCR_ODS)) ^ (0 != MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT))
		{
	#if 0 == MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT	// ReInit à 0 (valeur par défaut)
			StatusConfigRegister[1] &= ~MEM_MX25L_XSPI_RDCR_ODS;
	#else // Activer ODS :
			StatusConfigRegister[1] |= MEM_MX25L_XSPI_RDCR_ODS;
	#endif // MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT
			nb2Write = 2;	// StatusRegister & ConfigRegister
		}
  #endif // MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT

		// TopBottom is OTP "OneTimeProgramming" sur ConfigRegister :
  #ifdef MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT
		if((0 != (StatusConfigRegister[1] & MEM_MX25L_XSPI_RDCR_TB)) ^ (0 != MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT))
		{
	#if 0 == MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT	// ReInit à 0 "Top Area Protected" (valeur par défaut)
			StatusConfigRegister[1] &= ~MEM_MX25L_XSPI_RDCR_TB;
	#else // Activer Bottom Area Protetecd (OTP) :
			StatusConfigRegister[1] |= MEM_MX25L_XSPI_RDCR_TB;
	#endif // MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT
			nb2Write = 2;	// StatusRegister & ConfigRegister
		}
  #endif // MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT

		// DummyCycle sur ConfigRegister :
  #ifdef MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
		if((0 != (StatusConfigRegister[1] & MEM_MX25L_XSPI_RDCR_DC)) ^ (0 != MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT))
		{
	#if 0 == MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT	// ReInit à 0 (valeur par défaut)
			StatusConfigRegister[1] &= ~MEM_MX25L_XSPI_RDCR_DC;
	#else // Activer DummyCycle :
			StatusConfigRegister[1] |= MEM_MX25L_XSPI_RDCR_DC;
	#endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
			nb2Write = 2;	// StatusRegister & ConfigRegister
		}
  #endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT

		if(nb2Write > 0)
		{
			retVal = Mem_MX25L_XSPI_WriteStatusConfigRegisterArray(StatusConfigRegister, nb2Write);
		}
	}

	return retVal;
#else // ! MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT && ! MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT && ! MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT && ! MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT
	return MEM_MX25L_XSPI_RETURN_SUCCESS;
#endif // MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT & MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT & MEM_MX25L_XSPI_CONFIG_TOP_BOTTOM_AT_INIT & MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadStatusRegister(uint8_t *pStatusRegister) // RDSR from "MX25L6433F" v1.9 du 09/04/2025 p16, 21 & 23
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendCmdReceiveArray(MEM_MX25L_CMD_READ_STATUS_REGISTER, sizeof(uint8_t), pStatusRegister);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_IsWriteBusy()
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.
	uint8_t tmpStatusReg;
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == Mem_MX25L_XSPI_ReadStatusRegister(&tmpStatusReg))
	{
		return (tmpStatusReg & MEM_MX25L_XSPI_RDSR_WIP) ? MEM_MX25L_XSPI_RETURN_BUSY : MEM_MX25L_XSPI_RETURN_READY; // bit0 = WIP "Write in Progress"
	}
	return MEM_MX25L_XSPI_RETURN_ERROR; // Error
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_IsWriteEnabled()
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	uint8_t tmpStatusReg;
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == Mem_MX25L_XSPI_ReadStatusRegister(&tmpStatusReg))
	{
		return (tmpStatusReg & MEM_MX25L_XSPI_RDSR_WEL) ? MEM_MX25L_XSPI_RETURN_TRUE : MEM_MX25L_XSPI_RETURN_FALSE; // bit1 = WEL "Write Enable Latch"
	}
	return MEM_MX25L_XSPI_RETURN_ERROR; // Error
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_Wait4WriteNotBusy() // from "MX25L6433F" v1.9 du 09/04/2025 p16 & 21 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;
	XSPI_AutoPollingTypeDef sConfig;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_STATUS_REGISTER);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, 0);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_EACH_CMD(sCommand);	// Ne marche pas en MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD !

	MEM_MX25L_XSPI_CLEAR_STRUCT(sConfig);
	MEM_MX25L_BUILD_XSPI_CMP_PARAMS(sConfig, HAL_XSPI_MATCH_MODE_AND, MEM_MX25L_XSPI_RDSR_WIP, 0x00); // OK si bit0 "WIP" ("Write in Progress") = 0
	MEM_MX25L_BUILD_XSPI_CMP_RETRY(sConfig, 0, HAL_XSPI_AUTOMATIC_STOP_ENABLE); // TestDelay à 0 (précédemment à 16)

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		switch(MEM_MX25L_XSPI_PERIF_AUTO_POLLING(&sConfig))
		{
		case HAL_OK:
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
			break;
#ifdef MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE	// ChipErase is too long for standard polling TimeOut !
		case HAL_TIMEOUT:
			returnValue = MEM_MX25L_XSPI_RETURN_BUSY;	// Memory is still Busy pending, but it's NOT an error !
			break;
#endif // MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE
		case HAL_ERROR:	// Device has Error => Failure ...
		case HAL_BUSY:	// Device was Busy => Failure ...
		default:
			break;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadConfigRegister(uint8_t *pConfigRegister) // RDCR from "MX25L6433F" v1.9 du 09/04/2025 p16, 22 & 24 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendCmdReceiveArray(MEM_MX25L_CMD_READ_CONFIG_REGISTER, sizeof(uint8_t), pConfigRegister);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteStatusRegister(uint8_t newStatusRegister) // WRSR from "MX25L6433F" v1.9 du 09/04/2025 p16 & 25 :
{ // NonTesté_Jp au 16/05/2025.
	uint8_t tmpNewStatusRegister = newStatusRegister; // Variable intermédiaire pour accès au pointeur
    return Mem_MX25L_XSPI_WriteStatusConfigRegisterArray(&tmpNewStatusRegister, sizeof(tmpNewStatusRegister));
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteStatusConfigRegister(uint8_t newStatusRegister, uint8_t newConfigRegister) // WRSR from "MX25L6433F" v1.9 du 09/04/2025 p16 & 25 :
{ // NonTesté_Jp au 16/05/2025.
    uint8_t TxArray[] = { newStatusRegister, newConfigRegister }; // StatusRegister puis ConfigRegister
    return Mem_MX25L_XSPI_WriteStatusConfigRegisterArray(TxArray, sizeof(TxArray));
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteStatusConfigRegisterArray(uint8_t* pNewStatusConfigRegister, uint8_t newStatusConfigRegisterSize) // WRSR from "MX25L6433F" v1.9 du 09/04/2025 p16 & 25 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
#define MIN_STATUS_CONFIG_REGISTER_SIZE	1
#define MAX_STATUS_CONFIG_REGISTER_SIZE	2
	if(newStatusConfigRegisterSize > MAX_STATUS_CONFIG_REGISTER_SIZE) { newStatusConfigRegisterSize = MAX_STATUS_CONFIG_REGISTER_SIZE; }

	if(newStatusConfigRegisterSize >= MIN_STATUS_CONFIG_REGISTER_SIZE)
	{
		uint8_t returnValue = Mem_MX25L_XSPI_Wait4WriteNotBusy(); // Attente Fin d'exécution déjà en cours
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
		{
			returnValue = Mem_MX25L_XSPI_WriteEnable(); // Enable Write First !
		}
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
		{
			returnValue = Mem_MX25L_XSPI_SendCmdAndArray(MEM_MX25L_CMD_WRITE_STATUS_CFG_REG, pNewStatusConfigRegister, newStatusConfigRegisterSize);
		}
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
		{
			returnValue = Mem_MX25L_XSPI_Wait4WriteNotBusy(); // Attente Fin d'exécution
		}
		return returnValue;
	}
	return MEM_MX25L_XSPI_RETURN_FAILURE;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadIdRegister(void *pID_24bits) // RDID from "MX25L6433F" v1.9 du 09/04/2025 p16 & 20 + Table 9 p49 :
{ // Vérif_Jp = OK sur MX25L6433F le 14/05/2025 : [ 0xC2 ; 0x20 ; 0x17 ]
	// Note: The RDID instruction is for reading the 1-byte manufacturer ID and the 2-byte Device ID that follows.
	return Mem_MX25L_XSPI_SendCmdReceiveArray(MEM_MX25L_CMD_READ_IDENTIFICATION, 3, pID_24bits);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadElectronicID(uint8_t *pElectronicID) // RES from "MX25L6433F" v1.9 du 09/04/2025 p16 & 46 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025 : 0x16

#ifdef MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS
	uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_ELECTRONIC_ID);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, 8*3);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, 1);	// HAL_XSPI_DATA_1_LINE pour RES
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pElectronicID))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

	return returnValue;
#else // ! MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS :
	return MEM_MX25L_XSPI_RETURN_FAILURE;
#endif // MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS

}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(uint8_t Adr, uint8_t *pManufacturerAndDeviceID) // REMS from "MX25L6433F" v1.9 du 09/04/2025 p16, 48 & Table 9 p49 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025 : [ 0xC2 ; 0x16 ] si Adr = 0 ; [ 0x16 ; 0xC2 ] si Adr = 1 => OK.

#ifdef MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS
	uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_MFG_DEV_ID);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, Adr); // Remarque_Jp : (ADR_24 + NoAlt + NoDummy) remplace (2 Dummy Bytes + Add_8) !
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, 2);	// HAL_XSPI_DATA_1_LINE pour REMS
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pManufacturerAndDeviceID))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

	return returnValue;
#else // ! MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS :
	return MEM_MX25L_XSPI_RETURN_FAILURE;
#endif // MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS

}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadSecurityRegister(uint8_t *pSecurityRegister) // RDSCUR from "MX25L6433F" v1.9 du 09/04/2025 p17, 50 & 51 :
{ // Vu 0 le 14/05/2025 !
	return Mem_MX25L_XSPI_SendCmdReceiveArray(MEM_MX25L_CMD_READ_SECURITY_REG, sizeof(uint8_t), pSecurityRegister);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadDataBytes(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // READ from "MX25L6433F" v1.9 du 09/04/2025 p15 & 28 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Read);	// HAL_XSPI_DATA_1_LINE pour READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ReadDataBytes_HighSpeed(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf)	// FAST_READ from "MX25L6433F" v1.9 du 09/04/2025 p15 & 29 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_FAST_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, 8);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Read);	// HAL_XSPI_DATA_1_LINE pour FAST_READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_2_LINES

uint8_t Mem_MX25L_XSPI_ReadDataBytes_DualRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // DREAD from "MX25L6433F" v1.9 du 09/04/2025 p15 & 30 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_DUAL_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits); // HAL_XSPI_ADDRESS_1_LINE pour DREAD
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, 8);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(sCommand, HAL_XSPI_DATA_2_LINES, nbBytes2Read);	// HAL_XSPI_DATA_2_LINES pour DREAD
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_2_LINES

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_2_LINES

uint8_t Mem_MX25L_XSPI_ReadDataBytes_TwoRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // 2READ from "MX25L6433F" v1.9 du 09/04/2025 p15, 31 & Table 7 p24 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025 avec DUMMY = { 4 ; 8 } si ConfigRegister.DC = { 0 ; 1 }.

#if 1 == MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
	#define TWO_READ_DUMMY	8	// Vérif_Jp = OK sur MX25L6433F le 15/05/2025 avec DUMMY = 8 si ConfigRegister.DC = 1.
#else // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
	#define TWO_READ_DUMMY	4	// Vérif_Jp = OK sur MX25L6433F le 15/05/2025 avec DUMMY = 4 si ConfigRegister.DC = 0.
#endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT

    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_TWO_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_2_LINES, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits); // HAL_XSPI_ADDRESS_2_LINES pour 2READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, TWO_READ_DUMMY);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(sCommand, HAL_XSPI_DATA_2_LINES, nbBytes2Read);	// HAL_XSPI_DATA_2_LINES pour 2READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_2_LINES

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_4_LINES

uint8_t Mem_MX25L_XSPI_ReadDataBytes_QuadRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // QREAD from "MX25L6433F" v1.9 du 09/04/2025 p15 & 32 :
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_QUAD_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits); // HAL_XSPI_ADDRESS_1_LINE pour DREAD
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, 8);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(sCommand, HAL_XSPI_DATA_4_LINES, nbBytes2Read); // HAL_XSPI_DATA_4_LINES pour QREAD
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_4_LINES

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_4_LINES

uint8_t Mem_MX25L_XSPI_ReadDataBytes_FourRead(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // 4READ from "MX25L6433F" v1.9 du 09/04/2025 p15, 33 & Table 7 p24 :
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.

#if 1 == MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
	#define FOUR_READ_DUMMY	10
#else // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT
	#define FOUR_READ_DUMMY	6
#endif // MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT

    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_FOUR_READ_DATA_BYTES);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_4_LINES, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits); // HAL_XSPI_ADDRESS_4_LINES pour 4READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, FOUR_READ_DUMMY);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(sCommand, HAL_XSPI_DATA_4_LINES, nbBytes2Read);	// HAL_XSPI_DATA_4_LINES pour 4READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_4_LINES

//******************************************************************************

uint8_t Mem_MX25L_XSPI_SectorErase4K(uint32_t baseAdr_24bits)	// SE from "MX25L6433F" v1.9 du 09/04/2025 p16 & 37 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendWriteCmdAndAddress(MEM_MX25L_CMD_SECTOR_ERASE_4K, baseAdr_24bits);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_BlockErase64K(uint32_t baseAdr_24bits)	// BE from "MX25L6433F" v1.9 du 09/04/2025 p16 & 38 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendWriteCmdAndAddress(MEM_MX25L_CMD_BLOC_ERASE_64K, baseAdr_24bits);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_BlocErase32K(uint32_t baseAdr_24bits)	// BE32K from "MX25L6433F" v1.9 du 09/04/2025 p16 & 39 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendWriteCmdAndAddress(MEM_MX25L_CMD_BLOC_ERASE_32K, baseAdr_24bits);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_ChipErase(void)	// CE from "MX25L6433F" v1.9 du 09/04/2025 p16 & 40 :
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.
#ifdef MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE
	uint8_t returnValue = Mem_MX25L_XSPI_WriteEnable(); // Enable Write First !

	if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
	{
		returnValue = Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_CHIP_ERASE); // or MEM_MX25L_CMD_CHIP_ERASE_ALT
	}

	if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
	{
		returnValue = Mem_MX25L_XSPI_Wait4WriteNotBusy(); // Attente Fin d'exécution
	}

	return returnValue;
#else // ! MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE
	return MEM_MX25L_XSPI_RETURN_FAILURE;
#endif // MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write)	// PP from "MX25L6433F" v1.9 du 09/04/2025 p15 & 41 :
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.
#define MEM_MX25L_XSPI_WRITE_PAGE_BOUNDARY   256 // Program Page = 256 bytes

	uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;

	if( (0 != pArray2Write) && (0 < nbBytes2Write) )
	{
        uint16_t thisBlocSize;
        uint8_t *pData = pArray2Write;
        uint8_t mayStop = 0;

    	XSPI_RegularCmdTypeDef sCommand;

    	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_PAGE_PROGRAM);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
    	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, 0); // Adresse vide pour le moment, HAL_XSPI_ADDRESS_1_LINE pour PP
    	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Write);	// HAL_XSPI_DATA_1_LINE pour PP
    	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

    	while(0 < nbBytes2Write)
    	{ // Calcule le Max autorisé en Ecriture à partir de cette Adresse :
    		thisBlocSize = (MEM_MX25L_XSPI_WRITE_PAGE_BOUNDARY) - (baseAdr_24bits & ((MEM_MX25L_XSPI_WRITE_PAGE_BOUNDARY) -1));
    		if(thisBlocSize > nbBytes2Write)    { thisBlocSize = nbBytes2Write; } // Ramène au nb de Bytes demandés / disponibles
            sCommand.Address = baseAdr_24bits;
            sCommand.DataLength = thisBlocSize;

            if(MEM_MX25L_XSPI_RETURN_SUCCESS != Mem_MX25L_XSPI_WriteEnable()) { mayStop = 1; break; } // Arrêt immédiat, mais on peut faire le Break parce que CS n'est pas encore actif !

            MEM_MX25L_XSPI_ACTIVATE_CS();
            if(HAL_OK != MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand)) { mayStop = 1; } // Envoi Commande + Adresse 24 bits
            if(0 == mayStop)
            {
            	if(HAL_OK != MEM_MX25L_XSPI_PERIF_SEND_STREAM(pData)) { mayStop = 1; } // Envoi DataBytes
            }
            MEM_MX25L_XSPI_DEACTIVATE_CS();

            if(0 != mayStop) { break; } // S'il y a 1 erreur : on peut quitter ici (CS n'est plus actif) !

            if(MEM_MX25L_XSPI_RETURN_SUCCESS != Mem_MX25L_XSPI_Wait4WriteNotBusy()) { mayStop = 1; break; } // Attente Fin d'exécution

            // Write is OK :
            baseAdr_24bits  += thisBlocSize;
            pData           += thisBlocSize;
            nbBytes2Write   -= thisBlocSize;
    	}

        // Boucle terminée :
        if( (0 == nbBytes2Write) && (0 == mayStop)) { returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS; }
	}

	return returnValue;
}

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_4_LINES

uint8_t Mem_MX25L_XSPI_WriteArray_QuadWrite(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write) // 4PP from "MX25L6433F" v1.9 du 09/04/2025 p16 & 42 :
{ // Vérif_Jp = OK sur MX25L6433F le 16/05/2025.
	uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;

#ifdef MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT
	if( (0 != pArray2Write) && (0 < nbBytes2Write) )
	{
        uint16_t thisBlocSize;
        uint8_t *pData = pArray2Write;
        uint8_t mayStop = 0;

    	XSPI_RegularCmdTypeDef sCommand;

    	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_QUAD_PAGE_PROG);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
    	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_4_LINES, HAL_XSPI_ADDRESS_24_BITS, 0); // Adresse vide pour le moment, HAL_XSPI_ADDRESS_4_LINES pour 4PP
    	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_DATA_NO_DTR(sCommand, HAL_XSPI_DATA_4_LINES, nbBytes2Write);	// HAL_XSPI_DATA_4_LINES pour 4PP
    	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
    	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

    	while(0 < nbBytes2Write)
    	{ // Calcule le Max autorisé en Ecriture à partir de cette Adresse :
    		thisBlocSize = (MEM_MX25L_XSPI_WRITE_PAGE_BOUNDARY) - (baseAdr_24bits & ((MEM_MX25L_XSPI_WRITE_PAGE_BOUNDARY) -1));
    		if(thisBlocSize > nbBytes2Write)    { thisBlocSize = nbBytes2Write; } // Ramène au nb de Bytes demandés / disponibles
            sCommand.Address = baseAdr_24bits;
            sCommand.DataLength = thisBlocSize;

            if(MEM_MX25L_XSPI_RETURN_SUCCESS != Mem_MX25L_XSPI_WriteEnable()) { mayStop = 1; break; } // Arrêt immédiat, mais on peut faire le Break parce que CS n'est pas encore actif !

            MEM_MX25L_XSPI_ACTIVATE_CS();
            if(HAL_OK != MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand)) { mayStop = 1; } // Envoi Commande + Adresse 24 bits
            if(0 == mayStop)
            {
            	if(HAL_OK != MEM_MX25L_XSPI_PERIF_SEND_STREAM(pData)) { mayStop = 1; } // Envoi DataBytes
            }
            MEM_MX25L_XSPI_DEACTIVATE_CS();

            if(0 != mayStop) { break; } // S'il y a 1 erreur : on peut quitter ici (CS n'est plus actif) !

            if(MEM_MX25L_XSPI_RETURN_SUCCESS != Mem_MX25L_XSPI_Wait4WriteNotBusy()) { mayStop = 1; break; } // Attente Fin d'exécution

            // Write is OK :
            baseAdr_24bits  += thisBlocSize;
            pData           += thisBlocSize;
            nbBytes2Write   -= thisBlocSize;
    	}

        // Boucle terminée :
        if( (0 == nbBytes2Write) && (0 == mayStop)) { returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS; }
	}
#endif // MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT

	return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_4_LINES

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteEnable(void)	// WREN from "MX25L6433F" v1.9 du 09/04/2025 p16 & 18 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_WRITE_ENABLE);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_WriteDisable(void)	// WRDI from "MX25L6433F" v1.9 du 09/04/2025 p16 & 19 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_WRITE_DISABLE);
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_NoOperation(void) // NOP from "MX25L6433F" v1.9 du 09/04/2025 p17 & 56
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_NO_OPERATION);
}

//******************************************************************************

uint8_t MemMX25L_XSPI_SetBurstLength(uint8_t newBurstLength) // SBL from "MX25L6433F" v1.9 du 09/04/2025 p17 & 36 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	uint8_t tmpBurstLength = newBurstLength; // Variable intermédiaire pour accès au pointeur
	return Mem_MX25L_XSPI_SendCmdAndArray(MEM_MX25L_CMD_SET_BURST_LENGTH, &tmpBurstLength, sizeof(uint8_t));
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_DoSoftwareReset(void) // RSTEN & RST from "MX25L6433F" v1.9 du 09/04/2025 p17 & 56 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_RESET_ENABLE);	// RSTEN

    if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_DO_SOFT_RESET);	// RST
    }

    return returnValue;
}

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_DISCOVER_PARAMS

uint8_t Mem_MX25L_XSPI_ReadDiscoverableParameter(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf) // RDSFDP from "MX25L6433F" v1.9 du 09/04/2025 p17 & 57 :
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025 : 0x50444653 "SFDP Signature" en Adr 0.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_DISCOV_PARAMS);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DUMMY(sCommand, 8);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Read);	// HAL_XSPI_DATA_1_LINE pour FAST_READ
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pReadBuf))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#endif // MEM_MX25L_XSPI_SUPPORT_DISCOVER_PARAMS

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND

uint8_t Mem_MX25L_XSPI_SuspendProgram(void) // PGM from "MX25L6433F" v1.9 du 09/04/2025 p16 & 53 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_PROGRAM_SUSPEND);
}

#endif // MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND

uint8_t Mem_MX25L_XSPI_ResumeProgram(void) // PGM from "MX25L6433F" v1.9 du 09/04/2025 p16 & 55 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_PROGRAM_RESUME);
}

#endif // MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND

uint8_t Mem_MX25L_XSPI_SuspendErase(void) // ERS from "MX25L6433F" v1.9 du 09/04/2025 p16 & 55 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_ERASE_SUSPEND);
}

#endif // MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND

uint8_t Mem_MX25L_XSPI_ResumeErase(void) // ERS from "MX25L6433F" v1.9 du 09/04/2025 p16 & 55 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_ERASE_RESUME);
}

#endif // MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_DEEP_POWER

uint8_t Mem_MX25L_XSPI_EnterDeepPowerDown(void)	// DP from "MX25L6433F" v1.9 du 09/04/2025 p16 & 45 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_ENTER_DEEP_POWER);
}

#endif // MEM_MX25L_XSPI_SUPPORT_DEEP_POWER

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_DEEP_POWER

uint8_t Mem_MX25L_XSPI_ReleaseFromDeepPower(void)	// RDP from "MX25L6433F" v1.9 du 09/04/2025 p16, 46 & Figure 28 p47 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_RELEASE_DEEP_POWER);
}

#endif // MEM_MX25L_XSPI_SUPPORT_DEEP_POWER

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_SECURED_OTP

uint8_t Mem_MX25L_XSPI_EnterSecuredOTP(void)	// ENSO from "MX25L6433F" v1.9 du 09/04/2025 p16 & 49 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_ENTER_SECURE_OTP);
}

#endif // MEM_MX25L_XSPI_SUPPORT_SECURED_OTP

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_SECURED_OTP

uint8_t Mem_MX25L_XSPI_ExitSecuredOTP(void) 	// EXSO from "MX25L6433F" v1.9 du 09/04/2025 p16 & 49 :
{ // Envoi_Jp = OK sur MX25L6433F le 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_EXIT_SECURE_OTP);
}

#endif // MEM_MX25L_XSPI_SUPPORT_SECURED_OTP

//******************************************************************************

#ifdef MEM_MX25L_XSPI_SUPPORT_WRSCUR

uint8_t Mem_MX25L_XSPI_WriteSecurityRegister(void)	// WRSCUR from "MX25L6433F" v1.9 du 09/04/2025 p16 & 52 :
{ // NotTested_Jp on 15/05/2025.
	return Mem_MX25L_XSPI_SendThisCommand(MEM_MX25L_CMD_WRITE_SECURITY_REG);
}

#endif // MEM_MX25L_XSPI_SUPPORT_WRSCUR

//******************************************************************************
// Low level functions :

uint8_t Mem_MX25L_XSPI_SendThisCommand(uint8_t Cmd2Send)
{ // Vérif_Jp = OK sur MX25L6433F le 15/05/2025.
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, Cmd2Send);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DATA(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_SendCmdAndArray(uint8_t Cmd2Send, void* pArray2Send, uint16_t nbBytes2Send)
{
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, Cmd2Send);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Send);	// HAL_XSPI_DATA_1_LINE pour
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_STREAM(pArray2Send))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_SendCmdReceiveArray(uint8_t Cmd2Send, uint16_t nbBytes2Read, void* pArray2Receive)
{

    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;

	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, Cmd2Send);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, nbBytes2Read);	// HAL_XSPI_DATA_1_LINE pour
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if (HAL_OK == MEM_MX25L_XSPI_PERIF_RECV_STREAM(pArray2Receive))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_SendWriteCmdAndAddress(uint8_t WriteCmd2Send, uint32_t baseAdr_24bits)
{
	uint8_t returnValue = Mem_MX25L_XSPI_WriteEnable(); // Enable Write First !

	// Send Command :
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
	{
		XSPI_RegularCmdTypeDef sCommand;

		MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, WriteCmd2Send);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
		MEM_MX25L_BUILD_XSPI_CMD_ADDR_NO_DTR(sCommand, HAL_XSPI_ADDRESS_1_LINE, HAL_XSPI_ADDRESS_24_BITS, baseAdr_24bits);
		MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_NO_DATA(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
		MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

		MEM_MX25L_XSPI_ACTIVATE_CS();
		if(HAL_OK != MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
		}
		MEM_MX25L_XSPI_DEACTIVATE_CS();
	}

	// Wait for Operation complete :
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == returnValue)
	{
		returnValue = Mem_MX25L_XSPI_Wait4WriteNotBusy();
	}

	return returnValue;
}

//******************************************************************************

/* Variables Spéciales & Temporaires pour Test de la Mémoire XSPI :
typedef union {
	uint8_t U8[4];
	uint16_t U16[2];
	uint32_t U32;
} tU8_16_32;

#define TestBuf_Size	4096
tU8_16_32 TestBuf[TestBuf_Size] = {0};

uint8_t tmpU24[3];
uint8_t retVal;
uint8_t tmpU8;

void Mem_MX25L_XSPI_DoTestMemory(void)
{
	MX25L_XSPI_HALT_IF_DEBUG();

	retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_ReadConfigRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_ReadSecurityRegister(&tmpU8);

	retVal = Mem_MX25L_XSPI_ReadIdRegister(tmpU24); // OK
	retVal = Mem_MX25L_XSPI_ReadElectronicID(&tmpU8); // OK
	retVal = Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(0, tmpU24);// OK
	retVal = Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(1, tmpU24);// OK
	retVal = Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(2, tmpU24);// OK
	retVal = Mem_MX25L_XSPI_ReadManufacturerAndDeviceID(3, tmpU24);// OK

	retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_WriteEnable();
	retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_IsWriteEnabled();

	retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_WriteDisable();
	retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_XSPI_IsWriteEnabled();

	MX25L_XSPI_HALT_IF_DEBUG();

	// Test de Read Memory :
	uint32_t adr = 0; uint16_t nb2Read = TestBuf_Size * sizeof(tU8_16_32); uint16_t readMode = 0;
	uint32_t adrMax = 8UL * 1024UL * 1024UL; uint32_t nbU32Virgin = 0; uint32_t nbU32NotVirgin = 0; uint32_t nbU32AdrOK = 0;
	uint8_t curReadMode = 0;
	for(; adr < adrMax;)
	{
		switch(readMode)
		{
		case 1:
			retVal = Mem_MX25L_XSPI_ReadDataBytes_HighSpeed(adr, nb2Read, TestBuf);
			break;
		case 2:
			retVal = Mem_MX25L_XSPI_ReadDataBytes_DualRead(adr, nb2Read, TestBuf);
			break;
		case 3:
			retVal = Mem_MX25L_XSPI_ReadDataBytes_TwoRead(adr, nb2Read, TestBuf);
			break;
		case 4:
			retVal = Mem_MX25L_XSPI_ReadDataBytes_QuadRead(adr, nb2Read, TestBuf);
			break;
		case 5:
			retVal = Mem_MX25L_XSPI_ReadDataBytes_FourRead(adr, nb2Read, TestBuf);
			break;
		default:
		case 0:
			retVal = Mem_MX25L_XSPI_ReadDataBytes(adr, nb2Read, TestBuf);
			break;
		}
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
		{
			for(int i = 0; i < (nb2Read/4); i++)
			{
				if(UINT32_MAX == TestBuf[i].U32)
				{
					if(1 != curReadMode)
					{
						curReadMode = 1;
						nbU32Virgin = 0;
					}
					nbU32Virgin++;
				} else if((adr + i) == TestBuf[i].U32)
				{
					if(2 != curReadMode)
					{
						curReadMode = 2;
						nbU32AdrOK = 0;
					}
					nbU32AdrOK++;
				} else {
					MX25L_XSPI_HALT_IF_DEBUG();
					nbU32NotVirgin++;
				}
			}
		} else {
			MX25L_XSPI_HALT_IF_DEBUG();
		}
		adr += nb2Read;
	}

	MX25L_XSPI_HALT_IF_DEBUG();

	// Test de Erase Sector :
	uint32_t eraseStep = 4096;
	adr = 0; uint32_t nbU32Erased = 0; uint32_t nbU32NotErased = 0; uint16_t eraseMode = 0;
	for(; adr < adrMax;)
	{
		switch(eraseMode)
		{
		case 1:
			retVal = Mem_MX25L_XSPI_BlocErase32K(adr);
			break;
		case 2:
			retVal = Mem_MX25L_XSPI_BlockErase64K(adr);
			break;
		case 0:
		default:
			retVal = Mem_MX25L_XSPI_SectorErase4K(adr);
			break;
		}
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
		{
			nbU32Erased++;
		} else {
			MX25L_XSPI_HALT_IF_DEBUG();
			nbU32NotErased++;
		}
		adr += eraseStep;
	}

	MX25L_XSPI_HALT_IF_DEBUG();

	// Test du Chip Erase :
	retVal = Mem_MX25L_XSPI_ChipErase();
	if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
	{
		MX25L_XSPI_HALT_IF_DEBUG();
	} else if(MEM_MX25L_XSPI_RETURN_BUSY == retVal)
	{
		retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
		retVal = Mem_MX25L_XSPI_IsWriteBusy();
		retVal = Mem_MX25L_XSPI_Wait4WriteNotBusy(); // Attente Fin d'exécution
	} else {
		MX25L_XSPI_HALT_IF_DEBUG();
	}

	MX25L_XSPI_HALT_IF_DEBUG();

	// Test de Write Memory :
	uint16_t nb2Write = TestBuf_Size * sizeof(tU8_16_32); uint16_t writeMode = 0;
	adr = 0; uint32_t nbU32Written = 0; uint32_t nbU32NotWritten = 0;
	for(; adr < adrMax;)
	{
		for(int i = 0; i < (nb2Write/4); i++)
		{
			TestBuf[i].U32 = adr + i;
		}
		switch(writeMode)
		{
		case 1:
			retVal = Mem_MX25L_XSPI_WriteArray_QuadWrite(adr, TestBuf, nb2Write);
			break;
		default:
		case 0:
			retVal = Mem_MX25L_XSPI_WriteArray(adr, TestBuf, nb2Write);
			break;
		}
		if(MEM_MX25L_XSPI_RETURN_SUCCESS == retVal)
		{
			nbU32Written++;
		} else {
			MX25L_XSPI_HALT_IF_DEBUG();
			nbU32NotWritten++;
		}
		adr += nb2Write;
	}

	MX25L_XSPI_HALT_IF_DEBUG();

	// Test Suspend & Resume Program :
#ifdef MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND
	retVal = Mem_MX25L_XSPI_SuspendProgram();
	retVal = Mem_MX25L_XSPI_ResumeProgram();
#endif // MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND

	// Test Suspend & Resume Erase :
#ifdef MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND
	retVal = Mem_MX25L_XSPI_SuspendErase();
	retVal = Mem_MX25L_XSPI_ResumeErase();
#endif // MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND

	// Test Deep Power :
	retVal = Mem_MX25L_XSPI_EnterDeepPowerDown();	// DP
	retVal = Mem_MX25L_XSPI_ReleaseFromDeepPower();	// RDP

	// Test Secure OTP :
	retVal = Mem_MX25L_XSPI_EnterSecuredOTP();	// ENSO
	retVal = Mem_MX25L_XSPI_ExitSecuredOTP();	// EXSO

	// Test Misc :
	retVal = Mem_MX25L_XSPI_NoOperation(); // NOP
	tmpU8 = 0;	// 0 => Wrap 8-byte
	retVal = MemMX25L_XSPI_SetBurstLength(tmpU8); // SBL
	tmpU8 = 3;	// 3 => Wrap around 64-byte
	retVal = MemMX25L_XSPI_SetBurstLength(tmpU8); // SBL
	tmpU8 = 0x10; // 1xh => Disable Wrap
	retVal = MemMX25L_XSPI_SetBurstLength(tmpU8); // SBL

	adr = 0;
	retVal = Mem_MX25L_XSPI_ReadDiscoverableParameter(adr, nb2Read, TestBuf); // RDSFDP

	MX25L_XSPI_HALT_IF_DEBUG();
}
//*/

//******************************************************************************

#ifdef __cplusplus
}
#endif
