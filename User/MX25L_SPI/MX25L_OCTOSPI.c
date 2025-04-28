/*
 * MX25L_OCTOSPI.c
 *
 *  Created on: Apr 25, 2025
 *      Author: m.faget
 */

#ifndef MX25L_SPI_MX25L_OCTOSPI_C_
#define MX25L_SPI_MX25L_OCTOSPI_C_

#include "MX25L_SPI.h"

#include "spi.h"
#include "main.h"

/*
 *
uint8_t aTxBuffer[] = " **OCTOSPI/Octal-spi PSRAM Memory-mapped
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped
communication example**";
 *
 *
 */

void Mem_MX25L_Init(void)
{
    MEM_MX25L_CS_INIT();
    MEM_MX25L_PERIF_INIT();
}


uint32_t MX25_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address,uint8_t *Value);
uint32_t MX25_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode);
//static void Configure_APMemory(void);
uint8_t WriteEnable(void);
uint8_t WriteDisable(void);


/* This function Enables writing to the memory: write enable cmd is sent in
single SPI mode */
uint8_t WriteEnable(void)
{
	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the Write Enable cmd in single SPI mode */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_ENABLE;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* Send Write Enable command in single SPI mode */
	if (HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		//Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
	/* Transmission of the data/command ?/ already done previously with "HAL_XSPI_Command" ? */
	/*if (HAL_XSPI_Transmit(Ctx, (uint8_t *)(Value),HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
	*/
}

uint8_t WriteDisable(void)
{
	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the Write Enable cmd in single SPI mode */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_DISABLE;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* Send Write Enable command in single SPI mode */
	if (HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		//Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint32_t MX25_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value)
{


	XSPI_RegularCmdTypeDef sCommand1={0};
	/*Initialize the write register command */
	sCommand1.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand1.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand1.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand1.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand1.Instruction = MEM_MX25L_CMD_PAGE_PROGRAM;//WRITE_REG_CMD;//
	sCommand1.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand1.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand1.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand1.Address = Address;
	sCommand1.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand1.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand1.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand1.DataLength = 0xFF;//256
	sCommand1.DummyCycles = 0;
	sCommand1.DQSMode = HAL_XSPI_DQS_DISABLE;
	/* Configure the command*/
	if (HAL_XSPI_Command(Ctx, &sCommand1, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	/* Transmission of the data */
	if (HAL_XSPI_Transmit(Ctx, (uint8_t *)(Value), HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**Read mode register value*/
uint32_t MX25_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode)
{
	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Instruction = MEM_MX25L_CMD_READ_DATA_BYTES;//READ_REG_CMD;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand.Address = Address;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_8_LINES;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand.DataLength = 2;
	sCommand.DummyCycles = (LatencyCode - 1U);
	sCommand.DQSMode = HAL_XSPI_DQS_ENABLE;
	/* Configure the command */
	if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	/* Reception of the data */
	if (HAL_XSPI_Receive(Ctx, (uint8_t *)Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)	!= HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**Read Config mode register value*/
uint32_t MX25_ReadConfigReg(uint8_t *Value)
{
	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;//HAL_XSPI_OPTYPE_READ_CFG
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Instruction = MEM_MX25L_CMD_READ_CONFIG_REGISTER;//READ_REG_CMD;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* Configure the command */
	if (HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	/* Reception of the data */
	if (HAL_XSPI_Receive(MEM_MX25L_PERIF_HANDLE, (uint8_t *)Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)	!= HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

void SectorErase4K(uint32_t baseAdr_24bits)
{
	// enable Write First

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize Indirect write mode to erase the first sector */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.Instruction = MEM_MX25L_CMD_SECTOR_ERASE_4K;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.Address = baseAdr_24bits;
	/* Send Octal Sector erase cmd */
	if (HAL_XSPI_Command(MEM_MX25L_PERIF_HANDLE, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

#endif /* MX25L_SPI_MX25L_OCTOSPI_C_ */
