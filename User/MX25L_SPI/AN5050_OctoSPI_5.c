/*
 * AN5050_OctoSPI_5.c
 *
 *  Created on: Apr 29, 2025
 *      Author: j.proux
 */


#define __IO

// from https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/dd/af/52/e1/d3/48/8e/DM00407776/files/DM00407776.pdf
// Exemple "XSPI application example configuration with STM32CubeMX" p87 from "OCTOSPI application examples AN5050 Rev 12" :


/* USER CODE BEGIN EC */
/* Aps256xx APMemory memory */
/* Read Operations */
#define READ_CMD				0x00
#define READ_LINEAR_BURST_CMD	0x20

/* Write Operations */
#define WRITE_CMD				0x80
#define WRITE_LINEAR_BURST_CMD	0xA0

/* Registers definition */
#define MR0						0x00000000
#define MR8						0x00000008

/* Register Operations */
#define READ_REG_CMD			0x40
#define WRITE_REG_CMD			0xC0

/* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ 	4
#define DUMMY_CLOCK_CYCLES_WRITE	4

/* Size of buffers */
#define BUFFERSIZE 10240
#define KByte 1024
/* USER CODE END EC */



/* USER CODE BEGIN PV */
/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFERSIZE];
__IO uint8_t *mem_addr;
uint8_t CmdCplt, TxCplt , StatusMatch , RxCplt;
XSPI_MemoryMappedTypeDef sMemMappedCfg;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
uint32_t APS6408_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address,
uint8_t *Value);
uint32_t APS6408_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t
*Value, uint32_t LatencyCode);
static void Configure_APMemory(void);
/* USER CODE END PFP */

void Main_Custom_5(void)
{
	/* USER CODE BEGIN 1 */
	XSPI_RegularCmdTypeDef sCommand = {0};
	uint16_t errorBuffer = 0;
	uint32_t index, index_K;
	/* USER CODE END 1 */

	/* USER CODE BEGIN 2 */
	Configure_APMemory();

	/*Configure Memory Mapped mode*/
	sCommand.OperationType = HAL_XSPI_OPTYPE_WRITE_CFG;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Instruction = WRITE_CMD;
	sCommand.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
	sCommand.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
	sCommand.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand.Address = 0x0;
	sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand.DataMode = HAL_XSPI_DATA_16_LINES;
	sCommand.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand.DataLength = BUFFERSIZE;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_WRITE;
	sCommand.DQSMode = HAL_XSPI_DQS_ENABLE;
	if (HAL_XSPI_Command(&hxspi1, &sCommand,
	HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	sCommand.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
	sCommand.Instruction = READ_CMD;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ;
	sCommand.DQSMode = HAL_XSPI_DQS_ENABLE;
	if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_ENABLE;
	sMemMappedCfg.TimeoutPeriodClock = 0x34;
	if (HAL_XSPI_MemoryMapped(&hxspi1, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	}

	/*fill aTxBuffer */
	for (index_K = 0; index_K < 10; index_K++)
	{
		for (index = (index_K * KByte); index < ((index_K +1) * KByte);
		index++)
		{
			aTxBuffer[index]=index + index_K;
		}
	}

	/*Writing Sequence ----------------------------------------------- */
	index_K=0;
	for (index_K = 0; index_K < 10; index_K++)
	{
		mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
		for (index = (index_K * KByte); index < ((index_K +1) * KByte);
		index++)
		{
			*mem_addr = aTxBuffer[index];
			mem_addr++;
		}
		/* In memory-mapped mode, not possible to check if the memory is ready
		after the programming. So a delay corresponding to max page programming
		time is added */
		HAL_Delay(1);
	}

	/* Reading Sequence ----------------------------------------------- */
	index_K=0;
	for (index_K = 0; index_K < 2; index_K++)
	{
		mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
		for (index = (index_K * KByte); index < ((index_K +1) * KByte);
		index++)
		{
			if (*mem_addr != aTxBuffer[index])
			{
				/* can toggle led here*/
				errorBuffer++;
			}
			mem_addr++;
		}
		/* In memory-mapped mode, not possible to check if the memory is ready
		after the programming. So a delay corresponding to max page programming
		time is added */
		HAL_Delay(1);
	}
	if (errorBuffer == 0)
	{
		/* can toggle led here*/
	}

	/* Abort XSPI driver to stop the memory-mapped mode ------------ */
	if (HAL_XSPI_Abort(&hxspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END 2 */

}


/* USER CODE BEGIN 4 */
/***Write mode register*/
uint32_t APS6408_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address,
uint8_t *Value)
{
	XSPI_RegularCmdTypeDef sCommand1={0};

	/*Initialize the write register command */
	sCommand1.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand1.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
	sCommand1.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand1.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand1.Instruction = WRITE_REG_CMD;
	sCommand1.AddressMode = HAL_XSPI_ADDRESS_8_LINES;
	sCommand1.AddressWidth = HAL_XSPI_ADDRESS_32_BITS;
	sCommand1.AddressDTRMode = HAL_XSPI_ADDRESS_DTR_ENABLE;
	sCommand1.Address = Address;
	sCommand1.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
	sCommand1.DataMode = HAL_XSPI_DATA_8_LINES;
	sCommand1.DataDTRMode = HAL_XSPI_DATA_DTR_ENABLE;
	sCommand1.DataLength = 2;
	sCommand1.DummyCycles = 0;
	sCommand1.DQSMode = HAL_XSPI_DQS_DISABLE;

	/* Configure the command*/
	if (HAL_XSPI_Command(Ctx, &sCommand1, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		return HAL_ERROR;
	}

	/* Transmission of the data */
	if (HAL_XSPI_Transmit(Ctx, (uint8_t *)(Value),
	HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/**Read mode register value*/
uint32_t APS6408_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t
*Value, uint32_t LatencyCode)
{
	XSPI_RegularCmdTypeDef sCommand;

	/* Initialize the read register command */
	sCommand.OperationType = HAL_XSPI_OPTYPE_COMMON_CFG;
	sCommand.InstructionMode = HAL_XSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionWidth = HAL_XSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Instruction = READ_REG_CMD;
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
	if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		return HAL_ERROR;
	}
	/* Reception of the data */
	if (HAL_XSPI_Receive(Ctx, (uint8_t *)Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)
	!= HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/** Switch from Octal Mode to Hexa Mode on the memory*/
static void Configure_APMemory(void)
{
	/* MR0 register for read and write */
	uint8_t regW_MR0[2]={0x24,0x8D}; /* To configure AP memory Latency Type
	and drive Strength */
	uint8_t regR_MR0[2]={0};
	/* MR8 register for read and write */
	uint8_t regW_MR8[2]={0x4B,0x08}; /* To configure AP memory Burst Type */
	uint8_t regR_MR8[2]={0};
	/*Read Latency */
	uint8_t latency=6;

	/*Configure Read Latency and drive Strength */
	if (APS6408_WriteReg(&hxspi1, MR0, regW_MR0) != HAL_OK)
	{
		Error_Handler();
	}
	/* Check MR0 configuration */
	if (APS6408_ReadReg(&hxspi1, MR0, regR_MR0, latency ) != HAL_OK)
	{
		Error_Handler();
	}
	/* Check MR0 configuration */
	if (regR_MR0 [0] != regW_MR0 [0])
	{
		Error_Handler() ;
	}

	/* Configure Burst Length */
	if (APS6408_WriteReg(&hxspi1, MR8, regW_MR8) != HAL_OK)
	{
		Error_Handler();
	}
	/* Check MR8 configuration */
	if (APS6408_ReadReg(&hxspi1, MR8, regR_MR8, 6) != HAL_OK)
	{
		Error_Handler();
	}
	if (regR_MR8[0] != regW_MR8[0])
	{
		Error_Handler() ;
	}
}
/* USER CODE END 4 */
