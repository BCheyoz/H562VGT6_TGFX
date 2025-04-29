/*
 * AN5050_OctoSPI_3.c
 *
 *  Created on: Apr 29, 2025
 *      Author: j.proux
 */


#define __IO

// from https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/dd/af/52/e1/d3/48/8e/DM00407776/files/DM00407776.pdf
// Exemple "III. Quad-SPI PSRAM in Regular-command protocol example" p72 from "OCTOSPI application examples AN5050 Rev 12" :


/* USER CODE BEGIN Private defines */
/*APS1604M-3SQR PSRAM APmemory*/
#define FAST_READ_QUAD				0xEB
#define QUAD_WRITE					0x38
#define FAST_READ_QUAD_DUMMY_CYCLES	6
#define WRITE_QUAD_DUMMY_CYCLES		0
#define ENTER_QUAD_DUMMY_CYCLES		0
#define QUAD_WRITE					0x38
#define ENTER_QUAD_MODE				0x35
#define EXIT_QUAD_MODE				0xF5

/* Exported macro -----------------------------------------------------*/
#define BUFFERSIZE (COUNTOF(aTxBuffer) - 1)
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define EXTENDEDBUFFERSIZE (1048576)
/* USER CODE END Private defines */




/* USER CODE BEGIN PV */
/*buffer that we will write n times to the external memory , user can modify
the content to write his desired data */
uint8_t aTxBuffer[] = " **OCTOSPI/Quad-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Quad-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Quad-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Quad-spi PSRAM Memory-mapped \
communication example** ";
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
void EnterQuadMode(void);
void EnableMemMappedQuadMode(void);
/* USER CODE END PFP */

void Main_Custom_3(void)
{
	/* USER CODE BEGIN 1 */
	__IO uint8_t *mem_addr;
	uint32_t address = 0;
	uint16_t index1; /*index1 counter of bytes used when reading/writing 256
	bytes buffer */
	uint16_t index2; /*index2 counter of 256 bytes buffer used when
	reading/writing the 1Mbytes extended buffer */
	/* USER CODE END 1 */

	/* USER CODE BEGIN 2 */
	/* Enter Quad Mode 4-4-4 --------------------------------------------- */
	EnterQuadMode();

	/* Enable Memory mapped in Quad mode -------------------------------- */
	EnableMemMappedQuadMode();

	/* Writing Sequence of 1Mbyte --------------------------------------- */
	mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE + address);
	for (index2 = 0; index2 < EXTENDEDBUFFERSIZE/BUFFERSIZE; index2++)
	/*Writing 1Mbyte (256Byte BUFFERSIZE x 4096 times) */
	{
		for (index1 = 0; index1 < BUFFERSIZE; index1++)
		{
			*mem_addr = aTxBuffer[index1];
			mem_addr++;
		}
	}

	/* Reading Sequence of 1Mbyte ---------------------------------------- */
	mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE + address);
	for (index2 = 0; index2 < EXTENDEDBUFFERSIZE/BUFFERSIZE; index2++)
	/*Reading 1Mbyte (256Byte BUFFERSIZE x 4096 times)*/
	{
		for (index1 = 0; index1 < BUFFERSIZE; index1++)
		{
			if (*mem_addr != aTxBuffer[index1])
			{
				/*can toggle led here*/
			}
			mem_addr++;
		}
	}
	/*can toggle led here*/
	/* USER CODE END 2 */
}

/* USER CODE BEGIN 4 */
/*Function to Enable Memory mapped mode in Quad mode 4-4-4*/
void EnableMemMappedQuadMode(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_MemoryMappedTypeDef sMemMappedCfg;

	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_4_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_4_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_4_LINES;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.Address = 0;
	sCommand.NbData = 1;

	/* Memory-mapped mode configuration for Quad Read mode 4-4-4*/
	sCommand.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
	sCommand.Instruction = FAST_READ_QUAD;
	sCommand.DummyCycles = FAST_READ_QUAD_DUMMY_CYCLES;
	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
	/* Memory-mapped mode configuration for Quad Write mode 4-4-4*/
	sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
	sCommand.Instruction = QUAD_WRITE;
	sCommand.DummyCycles = WRITE_QUAD_DUMMY_CYCLES;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/*Disable timeout counter for memory mapped mode*/
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

	/*Enable memory mapped mode*/
	if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	}
}

/*Function to configure the external memory in Quad mode 4-4-4*/
void EnterQuadMode(void)
{
	OSPI_RegularCmdTypeDef sCommand;

	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = ENTER_QUAD_MODE;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_NONE;
	sCommand.DummyCycles = ENTER_QUAD_DUMMY_CYCLES;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

	/*Enter QUAD mode*/
	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
}
/* USER CODE END 4 */
