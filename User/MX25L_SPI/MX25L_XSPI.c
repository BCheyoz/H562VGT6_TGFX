/*
 * MX25L_XSPI.c
 *
 *  Created on: Apr 25, 2025
 *      Author: m.faget
 */

// Le fichier "octospi.c" pour STM32H5652 généré par CubeMX utilise les fonctions HAL "XSPI*",
// => cette Librairie s'appuie dons également uniquement sur les HAL "XSPI*".

#ifndef MX25L_SPI_MX25L_XSPI_C_
#define MX25L_SPI_MX25L_XSPI_C_

#include <MX25L_XSPI.h>


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

tJedecInfos mJedecInfos;
uint8_t byte_status_info;
uint8_t byte_config_info;
uint8_t byte_secu_info;
uint16_t byte_info;
uint16_t byte_info2;
uint16_t word_info_rx = 4;
uint32_t test_read_adress = 0x10;
uint32_t test_write_adress = 0x10;
uint16_t word_info_tx = 0xEABC;
uint16_t word_info_dual_rx = 4;
uint16_t word_info_two_rx = 4;



void MX25L_xspi_Init(void)
{
	byte_info = 0;
	byte_status_info = 0;
	byte_config_info = 0;
	byte_secu_info = 0;
	//word_info_rx = 0;
    MEM_MX25L_CS_INIT();
	//MEM_MX25L_ACTIVATE_SIO2();
    MEM_MX25L_PERIF_INIT();
}


/* This function Enables writing to the memory: write enable cmd is sent in
single SPI mode */
uint8_t MX25L_xspi_WriteEnable(void)
{// testé ok
    uint8_t returnValue = HAL_ERROR;
	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the Write Enable cmd in single SPI mode */
	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_ENABLE;//WREN
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25L_xspi_WriteDisable(void)
{// testé ok
    uint8_t returnValue = HAL_ERROR;
	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_DISABLE;//WRD
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25L_xspi_Wait4WriteNotBusy(void) // from "MX25L3233F" v1.9 du 19/04/2025 p21
{//test ok
    uint8_t returnValue = HAL_ERROR;
	XSPI_RegularCmdTypeDef sCommand;
	XSPI_AutoPollingTypeDef sConfig;
	MEM_MX25L_CLEAR_STRUCT(sCommand);
	MEM_MX25L_CLEAR_STRUCT(sConfig);

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_READ_STATUS_REGISTER;//WRD
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	//sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand.AlternateBytes = 0;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;// no data to send
	sCommand.DataLength = 0;// nbData = 0 ?
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;

	sConfig.MatchValue = 0x00;
	sConfig.MatchMask = (1 << 0);
	sConfig.MatchMode = HAL_XSPI_MATCH_MODE_AND;
	sConfig.AutomaticStop = HAL_XSPI_AUTOMATIC_STOP_ENABLE;
	sConfig.IntervalTime = 0x10;
	/* end of config part */

	if(HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand)) {
		if(HAL_OK == MEM_MX25L_PERIF_AUTO_XSPI_POLLING(&sConfig)) {
			returnValue = HAL_OK;
		}
	}
    return returnValue;
}


/**Read ID register value*/
uint8_t MX25_xspi_ReadIDReg(void *pID_24bits)
{// test ok
	uint8_t returnValue = HAL_ERROR;
	MEM_MX25L_ACTIVATE_CS();

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_READ_IDENTIFICATION;//RDID
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 3;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */

	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_24bits))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

/**Read Status register value*/
uint8_t MX25_xspi_ReadStatusReg(void *pID_8bits)
{//test ok
	uint8_t returnValue = HAL_ERROR;

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_READ_STATUS_REGISTER;//RDSR
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 1;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_8bits))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

/**Read Config mode register value*/
uint8_t MX25_xspi_ReadConfigReg(void *pID_8bits)
{//non testé
	uint8_t returnValue = HAL_ERROR;

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_READ_CONFIG_REGISTER;//RDCR
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 1;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_8bits))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

/**Read Data Bytes value*/
uint8_t MX25_xspi_ReadDataBytes(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// test ok
	uint8_t returnValue = HAL_ERROR;

	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_READ_DATA_BYTES;//RDB
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = nbBytes2Read;//max : 0xFF;//256
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;// 6? page 24 MX25L6433F
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25_xspi_DualReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// test ok
	uint8_t returnValue = HAL_ERROR;

	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_DUAL_READ_DATA_BYTES;//DRD
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_2_LINES;
	sCommand.DataLength = nbBytes2Read;//max : 0xFF;//256
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 8;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25_xspi_TwoReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// test ok
	uint8_t returnValue = HAL_ERROR;
	uint32_t DummyCyclevar = 4;//default
	uint8_t read_config = 0;

	if(HAL_OK == MX25_xspi_ReadConfigReg(&read_config))// verification DummyCycle
	{
		if((read_config & (1<<6))){
			DummyCyclevar = 8;
		}
	}

	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_TWO_READ_DATA_BYTES;//DRD
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_2_LINES;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_2_LINES;
	sCommand.DataLength = nbBytes2Read;//max : 0xFF;//256
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = DummyCyclevar;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}


/**Quad Read Data Bytes value*/
uint8_t MX25_xspi_QuadReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// non testé

	// en cours de dev
	// a Quad Enable (QE) bi of status must be set to "1" before sending QREAD instr
	uint8_t returnValue = MX25L_xspi_Enable_QE_Bit();

	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_QUAD_READ_DATA_BYTES;//RDB
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_4_LINES;
	sCommand.DataLength = nbBytes2Read;//max : 0xFF;//256
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 6;// datasheet -> p24 MX25L6433F Table7
	sCommand.DQSMode = HAL_XSPI_DQS_ENABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

/**Four Read Data Bytes value*/
uint8_t MX25_xspi_FourReadMode(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// non testé

	// en cours de dev
	// a Quad Enable (QE) bi of status must be set to "1" before sending QREAD instr
	uint8_t returnValue = MX25L_xspi_Enable_QE_Bit();

	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_FOUR_READ_DATA_BYTES;//4READ
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_4_LINES;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_4_LINES;
	sCommand.DataLength = nbBytes2Read;//max : 0xFF;//256
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 6;// datasheet -> p24 MX25L6433F Table7
	sCommand.DQSMode = HAL_XSPI_DQS_ENABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

/**Write Status register value*/
uint8_t MX25_xspi_WriteStatusAndConfigReg(uint8_t StatusRegisterValue, uint8_t ConfigRegisterValue)
{//test ok

	uint8_t TxData[] = {StatusRegisterValue, ConfigRegisterValue }; // StatusRegister puis ConfigRegister
	uint8_t returnValue = MX25L_xspi_WriteEnable();

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_STATUS_CFG_REG;//WRSR
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 2;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_STREAM(TxData))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    returnValue = MX25L_xspi_WriteDisable();

	return returnValue;
}

/**Write Status register value*/
uint8_t MX25_xspi_WriteStatusReg(uint8_t StatusRegisterValue)
{//test ok
	uint8_t pData = StatusRegisterValue;
	uint8_t returnValue = MX25L_xspi_WriteEnable();

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_STATUS_CFG_REG;//WRSR
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 1;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_STREAM(&pData))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    returnValue = MX25L_xspi_WriteDisable();

	return returnValue;
}

uint8_t MX25L_xspi_Enable_QuadMode(void)
{// test ok
	uint8_t returnValue = HAL_ERROR;
	uint8_t byte_status_info = 0;
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	if(byte_status_info & QE_BIT_MASK)
	{
		return returnValue;
	}
	byte_status_info |= QE_BIT_MASK;
	return MX25_xspi_WriteStatusReg(byte_status_info);
}

uint8_t MX25L_xspi_Disable_QuadMode(void)
{// test ok
	uint8_t returnValue = HAL_ERROR;
	uint8_t byte_status_info = 0;
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	if(0 == (byte_status_info & QE_BIT_MASK))
	{
		return returnValue;
	}
	byte_status_info &= ~QE_BIT_MASK;

	return MX25_xspi_WriteStatusReg(byte_status_info);
}

uint8_t MX25L_xspi_SectorErase4K(uint32_t baseAdr_24bits)
{//test ok
	uint8_t returnValue = MX25L_xspi_WriteEnable(); // Enable Write First !

	XSPI_RegularCmdTypeDef sCommand;

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_SECTOR_ERASE_4K;//SE
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = baseAdr_24bits;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send xspi Sector erase cmd */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
    MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    return returnValue;
}

uint8_t MX25L_xspi_BlocErase64K(uint32_t baseAdr_24bits)
{//test ok
	MEM_MX25L_ACTIVATE_CS();
	uint8_t returnValue = MX25L_xspi_WriteEnable(); // Enable Write First !

	XSPI_RegularCmdTypeDef sCommand;

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_BLOC_ERASE_64K;//BE64
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = baseAdr_24bits;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;// 6? page 24 MX25L6433F
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */

	/* Send xspi Sector erase cmd */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
    MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    return returnValue;
}

uint8_t MX25L_xspi_BlocErase32K(uint32_t baseAdr_24bits)
{//test ok
	MEM_MX25L_ACTIVATE_CS();
	uint8_t returnValue = MX25L_xspi_WriteEnable(); // Enable Write First !

	XSPI_RegularCmdTypeDef sCommand;

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_BLOC_ERASE_32K;//BE32
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = baseAdr_24bits;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;// 6? page 24 MX25L6433F
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */

	/* Send xspi Sector erase cmd */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
    MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    return returnValue;
}

uint8_t MX25L_xspi_ChipErase(void)
{//non testé
	uint8_t returnValue = MX25L_xspi_WriteEnable(); // Enable Write First !

	XSPI_RegularCmdTypeDef sCommand;

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_CHIP_ERASE;//CE
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address = 0;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;
	sCommand.DataLength = 0;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;// 6? page 24 MX25L6433F
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send xspi Sector erase cmd */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
    MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    return returnValue;
}

/*PageProgram*/
uint8_t MX25_xspi_PageProgram(uint32_t baseAdr_24bits, void *pArray2Write, uint32_t nbBytes2Write)
{//test ok
#define MEM_MX25L_WRITE_PAGE_BOUNDARY   256 // Program Page = 256 bytes

    uint8_t returnValue = HAL_ERROR;

    if( (0 != pArray2Write) && (0 < nbBytes2Write) )
    {
        uint16_t thisBlocSize;
        uint8_t *pData2Write = pArray2Write;
        uint8_t mayStop = 0;

		XSPI_RegularCmdTypeDef sCommand;
		MEM_MX25L_CLEAR_STRUCT(sCommand);
		/*Initialize the write register command */

		/* config part */
		sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
		//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
		sCommand.Instruction = MEM_MX25L_CMD_PAGE_PROGRAM;//PP
		sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
		sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
		sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
		//sCommand.Address = Address;
		sCommand.AddressMode = HAL_XSPI_ADDRESS_1_LINE;
		sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
		sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
		//sCommand.AlternateBytes = ;
		sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
		sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
		//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
		sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
		//sCommand.DataLength = nbBytes2Send;//max : 0xFF;//256
		sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
		sCommand.DummyCycles = 0;
		sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
		sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
		/* end of config part */

        while(0 < nbBytes2Write)
        { // Calcule le Max autorisé en Ecriture à partir de cette Adresse :
            thisBlocSize = (MEM_MX25L_WRITE_PAGE_BOUNDARY) - (baseAdr_24bits & ((MEM_MX25L_WRITE_PAGE_BOUNDARY) -1));
            if(thisBlocSize > nbBytes2Write)    { thisBlocSize = nbBytes2Write; } // Ramène au nb de Bytes demandés / disponibles

            sCommand.Address = baseAdr_24bits;
            sCommand.DataLength = thisBlocSize;

            if(HAL_OK != MX25L_xspi_WriteEnable()) { mayStop = 1; break; } // Arrêt immédiat, mais on peut faire le Break parce que CS n'est pas encore actif !

#ifdef MEM_MX25L_CS_PIN
            MEM_MX25L_ACTIVATE_CS();
#endif // MEM_MX25L_CS_PIN

            if(MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand) != HAL_OK) { mayStop = 1; } // Envoi Commande + Adresse 24 bits
            if(0 == mayStop)
            {
            	if(MEM_MX25L_PERIF_SEND_XSPI_STREAM(pData2Write) != HAL_OK) { mayStop = 1; } // Envoi DataBytes
            }

#ifdef MEM_MX25L_CS_PIN
            MEM_MX25L_DEACTIVATE_CS();
#endif // MEM_MX25L_CS_PIN

            if(0 != mayStop) { break; } // S'il y a 1 erreur : on peut quitter ici (CS n'est plus actif) !

            if(HAL_OK != MX25L_xspi_Wait4WriteNotBusy()) { mayStop = 1; break; } // Attente Fin d'exécution

            // Write is OK :
            baseAdr_24bits  += thisBlocSize;
            pData2Write     += thisBlocSize;
            nbBytes2Write   -= thisBlocSize;
        }

        // Boucle terminée :
        if( (0 == nbBytes2Write) && (0 == mayStop)) { returnValue = HAL_OK; }
    }

    return returnValue;
}

uint8_t MX25_xspi_ReadSecurityReg(void *pID_8bits)
{//test nok
	uint8_t returnValue = HAL_ERROR;

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_READ_SECURITY_REG;//RDSR
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_24_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_DISABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	//sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
	sCommand.DataLength = 1;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_8bits))
		{
			returnValue = HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25L_xspi_WriteSecurityReg(void)
{// test nok
    uint8_t returnValue = HAL_ERROR;
	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* Initialize the Write Enable cmd in single SPI mode */
	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_SECURITY_REG;//WREN
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
	MEM_MX25L_DEACTIVATE_CS();
	return returnValue;
}

uint8_t MX25L_xspi_SoftReset(void)
{// test nok
    uint8_t returnValue = HAL_ERROR;
	XSPI_RegularCmdTypeDef sCommand;
	MEM_MX25L_CLEAR_STRUCT(sCommand);
	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_RESET_ENABLE;//RSTEN
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
	MEM_MX25L_DEACTIVATE_CS();

	MEM_MX25L_CLEAR_STRUCT(sCommand);

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;
	sCommand.Instruction = MEM_MX25L_CMD_DO_SOFT_RESET;//RST
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	//sCommand.Address = Address;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_NONE;// no adress to send
	//sCommand.AddressWidth = HAL_XSPI_ADDRESS_8_BITS;
	//sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	//sCommand.AlternateBytes = ;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
	//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
	sCommand.DataMode = HAL_XSPI_DATA_NONE;// no data to send
	sCommand.DataLength = 0;
	//sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */
	MEM_MX25L_ACTIVATE_CS();
	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		returnValue = HAL_OK;
	}
	MEM_MX25L_DEACTIVATE_CS();

    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
	return returnValue;
}

void MX25L_xspi_Exit_HPM(void)// hardware protection mode
{
	uint8_t readStatusData;
	uint8_t returnValue = MX25_xspi_ReadStatusReg(&readStatusData);
	if(readStatusData & 0x80)
	{
		MEM_MX25L_ACTIVATE_SIO2();
		MX25_xspi_WriteStatusReg(0x00);
	}

}

void MX25L_xspi_DisableAllBlockProtection(void)// d blocks protection
{
	MEM_MX25L_ACTIVATE_SIO2();
	MX25_xspi_WriteStatusReg(0x00);
}



/********* TEST ZONE *********/

void Test_memory_init(void)
{
	MX25L_xspi_Init();
}

void Test_memory_in_init(void)
{
	uint8_t returnValue;
	//MX25_xspi_WriteStatusReg(0x00);
	//MX25L_xspi_Exit_HPM();
	//MX25L_xspi_DisableAllBlockProtection();

	//****** test read Status Register, read ID register, read Config Register, read Security Register **********
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	returnValue = MX25_xspi_ReadIDReg(&mJedecInfos);
	returnValue = MX25_xspi_ReadConfigReg(&byte_config_info);
	returnValue = MX25_xspi_ReadSecurityReg(&byte_secu_info);
	//********************************************************************************************


	//********************* test Single Read , Dual Read Mode & Two Read mode **********************************
	/*
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_DualReadMode(test_read_adress,&word_info_dual_rx,sizeof(word_info_dual_rx));
	returnValue = MX25_xspi_TwoReadMode(test_read_adress,&word_info_two_rx,sizeof(word_info_two_rx));
	*/
	//********************************************************************************************

	//********************* test Quad enable mode  **********************************
	returnValue = MX25L_xspi_Enable_QuadMode();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	returnValue = MX25L_xspi_Disable_QuadMode();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	//********************************************************************************************



	//returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	//returnValue = HAL_GPIO_ReadPin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin);
	//HAL_Delay(1000);
	//MEM_MX25L_ACTIVATE_CS();
    //HAL_GPIO_WritePin(Flash_Qspi_IO2_GPIO_Port, Flash_Qspi_IO2_Pin, GPIO_PIN_RESET);
	//returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	//returnValue = HAL_GPIO_ReadPin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin);
    //HAL_Delay(1000);

    //HAL_GPIO_WritePin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin, GPIO_PIN_SET);
	//MEM_MX25L_DEACTIVATE_CS();
	//returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	//returnValue = HAL_GPIO_ReadPin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin);

	//returnValue = MX25L_xspi_WriteEnable();
	/*returnValue = MX25_xspi_WriteStatusReg(0xC0);//0xC0 192 ou 64 0x40
	//MEM_MX25L_DEACTIVATE_SIO2();
	returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25_xspi_WriteStatusReg(0xC0);
	returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	*/
	//returnValue = MX25L_xspi_SoftReset();
	//returnValue = MX25L_xspi_SectorErase4K(test_read_adress);
	//HAL_Delay(1000);
	// = MX25_xspi_ReadDataBytes(0x00,&byte_info,sizeof(byte_info));

	//returnValue = MX25_xspi_WriteStatusReg(0);
	//MEM_MX25L_DEACTIVATE_SIO2();
	//returnValue = MX25L_xspi_WriteEnable();

	//uint8_t wrsr_command[2] = {0x01,0x00};
	//returnValue = MX25_xspi_WriteStatusReg(0x00);//0xC0 192 ou 64 0x40
	//MEM_MX25L_DEACTIVATE_SIO2();
	//returnValue = MX25_xspi_WriteStatusReg(0xC0);

	//returnValue = MX25L_xspi_WriteEnable();

	//returnValue = MX25L_xspi_WriteDisable();
	//returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);

	//MEM_MX25L_ACTIVATE_SIO2();
	//MX25_xspi_WriteStatusReg(0);

	//returnValue = MX25L_xspi_Enable_QE_Bit();
	/*returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	returnValue = MX25L_xspi_Disable_QE_Bit();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);

	returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	returnValue = MX25L_xspi_Disable_QE_Bit();
	returnValue = MX25_xspi_ReadStatusReg(&byte_status_info);
	*/
	/*returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));*/

	/*returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));

	returnValue = MX25L_xspi_ChipErase();
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));

	//returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));

	//returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25L_xspi_SectorErase4K(test_read_adress);
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));

	returnValue = MX25L_xspi_BlocErase32K(test_read_adress);
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));

	returnValue = MX25L_xspi_BlocErase64K(test_read_adress);
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	returnValue = MX25_xspi_PageProgram(test_write_adress, &word_info_tx, sizeof(word_info_tx));
	returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));*/

	//returnValue = MX25_xspi_QuadReadData(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	//returnValue = MX25_xspi_QuadReadMode(test_read_adress,&word_info_four_rx,sizeof(word_info_four_rx));



}

void Test_memory(void){
	/*uint8_t returnValue;
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN))
	{
		returnValue = MX25L_xspi_WriteEnable();
		returnValue = MX25_xspi_WriteStatusReg(0xC0);//0xC0 192 ou 64 0x40
		MEM_MX25L_DEACTIVATE_SIO2();
		returnValue = MX25L_xspi_WriteEnable();
		returnValue = MX25_xspi_WriteStatusReg(0xC0);
		returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	}
    HAL_GPIO_WritePin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(2000);
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Flash_Qspi_CS_GPIO_Port, Flash_Qspi_CS_Pin))
	{
		returnValue = MX25L_xspi_WriteEnable();
		returnValue = MX25_xspi_WriteStatusReg(0xC0);//0xC0 192 ou 64 0x40
		MEM_MX25L_DEACTIVATE_SIO2();
		returnValue = MX25L_xspi_WriteEnable();
		returnValue = MX25_xspi_WriteStatusReg(0xC0);
		returnValue = HAL_GPIO_ReadPin(MEM_MX25l_IO2_PORT, MEM_MX25l_IO2_PIN);
	}*/


	//uint8_t returnValue;
	//returnValue = MX25_xspi_ReadDataBytes(test_read_adress,&word_info_rx,sizeof(word_info_rx));
	//uint8_t returnValue = WriteEnable();
	//byte_info = 0;
	//uint8_t returnValue2 = MX25_ReadDataBytes(0x100000,&byte_info2);

}

#endif /* MX25L_SPI_MX25L_XSPI_C_ */
