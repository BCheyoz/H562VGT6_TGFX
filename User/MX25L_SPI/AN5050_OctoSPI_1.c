/*
 * AN5050_OctoSPI_1.c
 *
 *  Created on: Apr 29, 2025
 *      Author: j.proux
 */

#define __IO

// from https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/dd/af/52/e1/d3/48/8e/DM00407776/files/DM00407776.pdf
// Exemple "I. Octo-SPI PSRAM in Regular-command protocol example" p55 from "OCTOSPI application examples AN5050 Rev 12" :


/* USER CODE BEGIN Private defines */
/*APS6408L-3OB PSRAM APmemory*/
#define LINEAR_BURST_READ 0x20
#define LINEAR_BURST_WRITE 0xA0
#define DUMMY_CLOCK_CYCLES_SRAM_READ 5
#define DUMMY_CLOCK_CYCLES_SRAM_WRITE 4
/* Exported macro -----------------------------------------------------*/
#define BUFFERSIZE (COUNTOF(aTxBuffer) - 1)
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define DLYB_BUFFERSIZE (COUNTOF(Cal_buffer) - 1)
#define EXTENDEDBUFFERSIZE (1048576)
/* USER CODE END Private defines */


/* USER CODE BEGIN PV */
/*buffer that we will write n times to the external memory, user can modify
the content to write his desired data*/
/* Private variables -------------------------------------------------*/
uint8_t aTxBuffer[] = " **OCTOSPI/Octal-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped \
communication example** **OCTOSPI/Octal-spi PSRAM Memory-mapped \
communication example**";
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes ----------------------------------------*/
void EnableMemMapped(void);
void DelayBlock_Calibration(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 1 */
__IO uint8_t *mem_addr;
uint32_t address = 0;
uint16_t index1;/*index1 counter of bytes used when reading/
writing 256 bytes buffer */
uint16_t index2;/*index2 counter of 256 bytes buffer used when reading/
writing the 1Mbytes extended buffer */
/* USER CODE END 1 */

void Main_Custom_1(void)
{

/* USER CODE BEGIN 2 */
/*----------------------------------------------------------------------*/

	/*Enable Memory Mapped Mode*/
	EnableMemMapped();
/*----------------------------------------------------------------------*/
	/*Enable the Delay Block Calibration*/
	DelayBlock_Calibration();
/*----------------------------------------------------------------------*/
	/* Writing Sequence of 1Mbyte */
	mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE + address);

	/*Writing 1Mbyte (256Byte BUFFERSIZE x 4096 times) */
	for (index2 = 0; index2 < EXTENDEDBUFFERSIZE/BUFFERSIZE; index2++)
	{
		for (index1 = 0; index1 < BUFFERSIZE; index1++)
		{
			*mem_addr = aTxBuffer[index1];
			mem_addr++;
		}
	}
/*----------------------------------------------------------------------*/
	/* Reading Sequence of 1Mbyte */
	mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE + address);

	/*Reading 1Mbyte (256Byte BUFFERSIZE x 4096 times)*/
	for (index2 = 0; index2 < EXTENDEDBUFFERSIZE/BUFFERSIZE; index2++) {
		for (index1 = 0; index1 < BUFFERSIZE; index1++)
		{
			if (*mem_addr != aTxBuffer[index1])
			{
				/*if data read is corrupted we can toggle a led here: example blue led*/
			}
			mem_addr++;
		}
	}
/*if data read is correct we can toggle a led here: example green led*/
/* USER CODE END 2 */
}


/* USER CODE BEGIN 4 */
/*----------------------------------------------------------------------*/
/* This function enables memory-mapped mode for Read and Write operations */
void EnableMemMapped(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_MemoryMappedTypeDef sMemMappedCfg;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.Address = 0;
	sCommand.NbData = 1;
	/* Memory-mapped mode configuration for Linear burst write operations */
	sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
	sCommand.Instruction = LINEAR_BURST_WRITE;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_WRITE;

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Memory-mapped mode configuration for Linear burst read operations */
	sCommand.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
	sCommand.Instruction = LINEAR_BURST_READ;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_READ;

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

/*----------------------------------------------------------------------*/
/*This function is used to calibrate the Delayblock before initiating
USER's application read/write transactions*/
void DelayBlock_Calibration(void)
{
/*buffer used for calibration*/
uint8_t Cal_buffer[] = " ****Delay Block Calibration Buffer**** ****Delay \
Block Calibration Buffer**** ****Delay Block Calibration Buffer**** \
****Delay Block Calibration Buffer**** ****Delay Block Calibration \
Buffer**** ****Delay Block Calibration Buffer**** ";
uint16_t index;
__IO uint8_t *mem_addr;
uint8_t test_failed;
uint8_t delay = 0x0;
uint8_t Min_found = 0;
uint8_t Max_found = 0;
uint8_t Min_Window = 0x0;
uint8_t Max_Window = 0xF;
uint8_t Mid_window = 0;
uint8_t calibration_ongoing = 1;

	/* Write the Cal_buffer to the memory*/
	mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE);
	for (index = 0; index < DLYB_BUFFERSIZE; index++)
	{
		*mem_addr = Cal_buffer[index];
		mem_addr++;
	}

	while (calibration_ongoing)
	{
		/* update the Delayblock calibration */
		HAL_RCCEx_OCTOSPIDelayConfig(delay, 0);
		test_failed = 0;
		mem_addr = (__IO uint8_t *)(OCTOSPI1_BASE);

		for (index = 0; index < DLYB_BUFFERSIZE; index++)
		{
			/* Read the Cal_buffer from the memory*/
			if (*mem_addr != Cal_buffer[index])
			{
				/*incorrect data read*/
				test_failed = 1;
			}
			mem_addr++;
		}

		/* search for the Min window */
		if (Min_found!=1)
		{
			if (test_failed == 1)
			{
				if (delay < 15)
				{
					delay++;
				}
				else
				{
					/* If delay set to maximum and error still detected: can't use external
					PSRAM */
					Error_Handler();
				}
			}
			else
			{
				Min_Window = delay;
				Min_found=1;
				delay = 0xF;
			}
		}

		/* search for the Max window */
		else if (Max_found!=1)
		{
			if (test_failed == 1)
			{
				if (delay > 0)
				{
					delay--;
				}
				else
				{
					/* If delay set to minimum and error still detected: can't use external
					PSRAM */
					Error_Handler();
				}
			}
			else
			{
				Max_Window = delay;
				Max_found=1;
			}
		}

		/* min and max delay window found, configure the delay block with the middle
		window value and exit calibration */
		else
		{
			Mid_window = (Max_Window+Min_Window)/2;
			HAL_RCCEx_OCTOSPIDelayConfig(Mid_window, 0);
			/* exit calibration */
			calibration_ongoing = 0;
		}
	}
}
/* USER CODE END 4 */

