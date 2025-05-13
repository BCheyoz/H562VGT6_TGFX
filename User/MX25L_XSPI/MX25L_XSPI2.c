/*
 * MX25L_XSPI.c
 *
 *  Created on: May 13, 2025
 *      Author: j.proux
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
//#define MEM_MX25L_BUILD_XSPI_CMD_DUAL_RATE(XSPI_Cmd,Mode,Val)		XSPI_Cmd.DdrMode = Mode; XSPI_Cmd.DdrHoldHalfCycle = Val
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
//#define MEM_MX25L_BUILD_XSPI_CMD_NO_DDR(XSPI_Cmd)			MEM_MX25L_BUILD_XSPI_CMD_DUAL_RATE(XSPI_Cmd, XSPI_DDR_MODE_DISABLE, XSPI_DDR_HHC_ANALOG_DELAY)
#define MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(XSPI_Cmd)							MEM_MX25L_BUILD_XSPI_CMD_DQS(XSPI_Cmd, HAL_XSPI_DQS_DISABLE)
#define MEM_MX25L_BUILD_XSPI_CMD_SIOO_EACH_CMD(XSPI_Cmd)					MEM_MX25L_BUILD_XSPI_CMD_SIOO(XSPI_Cmd, HAL_XSPI_SIOO_INST_EVERY_CMD)
#define MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(XSPI_Cmd)					MEM_MX25L_BUILD_XSPI_CMD_SIOO(XSPI_Cmd, HAL_XSPI_SIOO_INST_ONLY_FIRST_CMD)

// Macros de renseignement de la structure XSPI_AutoPollingTypeDef :
#define MEM_MX25L_BUILD_XSPI_CMP_PARAMS(XSPI_AtPol,Mode,Msk,Val)			XSPI_AtPol.MatchMode = Mode; XSPI_AtPol.MatchMask = Msk; XSPI_AtPol.MatchValue = Val
#define MEM_MX25L_BUILD_XSPI_CMP_RETRY(XSPI_AtPol,Delay,AutoStop)			XSPI_AtPol.AutomaticStop = AutoStop; XSPI_AtPol.IntervalTime = Delay

uint8_t Mem_MX25L_XSPI_Init_Config(void);
uint8_t Mem_MX25L_XSPI_Wait4WriteNotBusy();


#ifdef __cplusplus
extern "C" {
#endif

void Mem_MX25L_XSPI_Init(void)
{
	// Init Hardware :
    MEM_MX25L_XSPI_CS_INIT();
    HAL_GPIO_WritePin(GPIOA, Flash_Qspi_IO3_Pin|Flash_Qspi_IO2_Pin, GPIO_PIN_SET); // Tant qu'on est en mode DUAL uniquement !
    MEM_MX25L_XSPI_PERIF_INIT();

    // Init Software :
	Mem_MX25L_XSPI_Init_Config();
//	Mem_MX25L_XSPI_DoSoftwareReset(); // Temporaire

	MX25L_XSPI_HALT_IF_DEBUG();
}

//******************************************************************************

uint8_t Mem_MX25L_XSPI_Init_Config(void)
{
	uint8_t retVal = Mem_MX25L_XSPI_Wait4WriteNotBusy();

	return retVal;
}



//******************************************************************************

uint8_t Mem_MX25L_XSPI_Wait4WriteNotBusy() // from "MX25L6433F" v1.9 du 09/04/2025 p16 & 21 :
{
    uint8_t returnValue = MEM_MX25L_XSPI_RETURN_FAILURE;
	XSPI_RegularCmdTypeDef sCommand;
	XSPI_AutoPollingTypeDef sConfig;
	MEM_MX25L_XSPI_CLEAR_STRUCT(sCommand);
	MEM_MX25L_XSPI_CLEAR_STRUCT(sConfig);

	MEM_MX25L_BUILD_XSPI_CMD_OP_TYPE_COMMON(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_INSTR_1_LINE(sCommand, MEM_MX25L_CMD_READ_STATUS_REGISTER);	// Instruction is ALWAYS 1 line / 8 bits / No DTR
	MEM_MX25L_BUILD_XSPI_CMD_NO_ADDRESS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_ALT_BYTES(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_DATA_1_LINE(sCommand, 0);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DUMMY(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_NO_DQS(sCommand);
	MEM_MX25L_BUILD_XSPI_CMD_SIOO_FIRST_CMD(sCommand);

	MEM_MX25L_BUILD_XSPI_CMP_PARAMS(sConfig, HAL_XSPI_MATCH_MODE_AND, (1 << 0), 0x00); // OK si bit0 "WIP" ("Write in Progress") = 0
	MEM_MX25L_BUILD_XSPI_CMP_RETRY(sConfig, 0, HAL_XSPI_AUTOMATIC_STOP_ENABLE); // TestDelay à 0 (précédemment à 16)

	MEM_MX25L_XSPI_ACTIVATE_CS();
	if(HAL_OK == MEM_MX25L_XSPI_PERIF_SEND_COMMAND(&sCommand))
	{
		if(HAL_OK == MEM_MX25L_XSPI_PERIF_AUTO_POLLING(&sConfig))
		{
			returnValue = MEM_MX25L_XSPI_RETURN_SUCCESS;
		}
	}
	MEM_MX25L_XSPI_DEACTIVATE_CS();

    return returnValue;
}

#ifdef __cplusplus
}
#endif
