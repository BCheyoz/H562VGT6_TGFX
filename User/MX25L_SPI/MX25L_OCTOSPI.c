/*
 * MX25L_OCTOSPI.c
 *
 *  Created on: Apr 25, 2025
 *      Author: m.faget
 */

// Le fichier "octospi.c" pour STM32H5652 généré par CubeMX utilise les fonctions HAL "XSPI*",
// => cette Librairie s'appuie dons également uniquement sur les HAL "XSPI*".

#ifndef MX25L_SPI_MX25L_OCTOSPI_C_
#define MX25L_SPI_MX25L_OCTOSPI_C_

#include "MX25L_OCTOSPI.h"


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
tJedecInfos mJedecInfos;
uint8_t byte2_info;
uint16_t byte2_info2;
uint16_t byte_info;
uint16_t byte_info2;


void MX25L_xspi_Init(void)
{
	byte_info = 0;
	byte2_info = 0;
	byte2_info2 = 0;
    MEM_MX25L_CS_INIT();
    MEM_MX25L_PERIF_INIT();
}


/* This function Enables writing to the memory: write enable cmd is sent in
single SPI mode */
uint8_t MX25L_xspi_WriteEnable(void)
{// testé ok
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

	/* Send Write Enable command in single SPI mode */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t MX25L_xspi_WriteDisable(void)
{// testé ok
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

	/* Send Write Enable command in single SPI mode */
	if (MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t MX25L_xspi_Wait4WriteNotBusy(void) // from "MX25L3233F" v1.9 du 19/04/2025 p21
{//non testé
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

	// HAL_QPSI_TIMEOUT_DEFAULT_VALUE
	if(MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand) != HAL_OK) { return MEM_MX25L_RETURN_FAILURE; }
	if(MEM_MX25L_PERIF_AUTO_XSPI_POLLING(&sConfig) != HAL_OK) { return MEM_MX25L_RETURN_FAILURE; }
    return MEM_MX25L_RETURN_SUCCESS;
}

/*PageProgram*/
uint32_t MX25_xspi_PageProgram(uint32_t baseAdr_24bits, void *pArray2Write, uint32_t nbBytes2Write)
{// non testé
#define MEM_MX25L_WRITE_PAGE_BOUNDARY   256 // Program Page = 256 bytes

    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Write) && (0 < nbBytes2Write) )
    {
        uint16_t thisBlocSize;
        uint8_t *pData = pArray2Write;
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
		sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
		//sCommand.AlternateBytes = ;
		sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
		sCommand.AlternateBytesWidth = HAL_XSPI_ALT_BYTES_8_BITS;
		//sCommand.AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;
		sCommand.DataMode = HAL_XSPI_DATA_1_LINE;
		//sCommand.DataLength = nbBytes2Send;//max : 0xFF;//256
		sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
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

            if(MEM_MX25L_RETURN_SUCCESS != MX25L_xspi_WriteEnable()) { mayStop = 1; break; } // Arrêt immédiat, mais on peut faire le Break parce que CS n'est pas encore actif !

#ifdef MEM_MX25L_CS_PIN
            MEM_MX25L_ACTIVATE_CS();
#endif // MEM_MX25L_CS_PIN

            if(MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand) != HAL_OK) { mayStop = 1; } // Envoi Commande + Adresse 24 bits
            if(0 == mayStop)
            {
            	if(MEM_MX25L_PERIF_SEND_XSPI_STREAM(pData) != HAL_OK) { mayStop = 1; } // Envoi DataBytes
            }

#ifdef MEM_MX25L_CS_PIN
            MEM_MX25L_DEACTIVATE_CS();
#endif // MEM_MX25L_CS_PIN

            if(0 != mayStop) { break; } // S'il y a 1 erreur : on peut quitter ici (CS n'est plus actif) !

            if(MEM_MX25L_RETURN_SUCCESS != MX25L_xspi_Wait4WriteNotBusy()) { mayStop = 1; break; } // Attente Fin d'exécution

            // Wrtite is OK :
            baseAdr_24bits  += thisBlocSize;
            pData           += thisBlocSize;
            nbBytes2Write   -= thisBlocSize;
        }

        // Boucle terminée :
        if( (0 == nbBytes2Write) && (0 == mayStop)) { returnValue = MEM_MX25L_RETURN_SUCCESS; }
    }

    return returnValue;
}

/**Read Data Bytes value*/
uint8_t MX25_ReadDataBytes(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// test ok
	MEM_MX25L_ACTIVATE_CS();

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

	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			MEM_MX25L_DEACTIVATE_CS();
			return HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return HAL_ERROR;
}

/**Quad Read Data Bytes value*/
uint8_t MX25_xspi_QuadReadData(uint32_t Address, void *Value, uint32_t nbBytes2Read)
{// test ok
	// a Quad Enable (QE) bi of status must be set to "1" before sending QREAD instr
	MEM_MX25L_ACTIVATE_CS();

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
	sCommand.DummyCycles = 8;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */

	/* Configure the command */
	if (HAL_OK ==  MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)Value))
		{
			MEM_MX25L_DEACTIVATE_CS();
			return HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return HAL_ERROR;
}


/**Read ID register value*/
uint8_t MX25_xspi_ReadIDReg(void *pID_24bits)
{//non testé
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
			MEM_MX25L_DEACTIVATE_CS();
			return HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return HAL_ERROR;
}

/**Read ID register value*/
uint8_t MX25_xspi_ReadStatusReg(void *pID_8bits)
{//non testé
	MEM_MX25L_ACTIVATE_CS();

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_READ_STATUS_REGISTER;//RDID
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

	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_8bits))
		{
			MEM_MX25L_DEACTIVATE_CS();
			return HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return HAL_ERROR;
}

/**Read Config mode register value*/
uint8_t MX25_xspi_ReadConfigReg(void *pID_16bits)
{//non testé
	MEM_MX25L_ACTIVATE_CS();

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
	sCommand.DataLength = 2;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_DISABLE;//disable = only falling edge data receiving/transmiting
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_XSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_XSPI_SIOO_INST_EVERY_CMD;
	/* end of config part */

	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_RECV_XSPI_STREAM((uint8_t *)pID_16bits))
		{
			MEM_MX25L_DEACTIVATE_CS();
			return HAL_OK;
		}
	}
	MEM_MX25L_DEACTIVATE_CS();
	return HAL_ERROR;
}

/**Read ID register value*/
uint8_t MX25_xspi_WriteStatusReg(void *value)
{//non testé

	// en cours d'implémentation

	uint8_t returnValue = MX25L_xspi_WriteEnable();
	MEM_MX25L_ACTIVATE_CS();

	XSPI_RegularCmdTypeDef sCommand;
	/* Initialize the read register command */

	/* config part */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	//sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;//The SIO[3:1] are don't care
	sCommand.Instruction = MEM_MX25L_CMD_WRITE_STATUS_CFG_REG;//RDID
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

	/* Configure the command */
	if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_COMMAND(&sCommand))
	{
		/* Reception of the data */
		if (HAL_OK == MEM_MX25L_PERIF_SEND_XSPI_STREAM((uint8_t *)value))
		{
			returnValue = HAL_OK;
		}
		else{returnValue = HAL_ERROR;}
	}
	else{returnValue = HAL_ERROR;}
	MEM_MX25L_DEACTIVATE_CS();


    if(HAL_OK == returnValue)
    {
        returnValue = MX25L_xspi_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }
    returnValue = MX25L_xspi_WriteDisable();

	return returnValue;
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

/********* TEST ZONE *********/

void Test_memory_init(void)
{
	MX25L_xspi_Init();
}

void Test_memory_in_init(void)
{
	uint8_t returnValue = MX25_ReadDataBytes(0x00,&byte_info,sizeof(byte_info));

	returnValue = MX25_xspi_ReadIDReg(&mJedecInfos);
	returnValue = MX25L_xspi_WriteEnable();
	returnValue = MX25_xspi_ReadStatusReg(&byte2_info);
	returnValue = MX25L_xspi_WriteDisable();
	returnValue = MX25_xspi_ReadStatusReg(&byte2_info);
	returnValue = MX25_xspi_ReadConfigReg(&byte2_info2);


}

void Test_memory(void){
	//uint8_t returnValue = WriteEnable();
	//byte_info = 0;
	//uint8_t returnValue2 = MX25_ReadDataBytes(0x100000,&byte_info2);

}

#endif /* MX25L_SPI_MX25L_OCTOSPI_C_ */
