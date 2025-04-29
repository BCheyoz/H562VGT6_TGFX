/*
 * AN5050_OctoSPI_4.c
 *
 *  Created on: Apr 29, 2025
 *      Author: j.proux
 */


#define __IO

// from https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/dd/af/52/e1/d3/48/8e/DM00407776/files/DM00407776.pdf
// Exemple "IV. HyperFlash and HyperRAM memories with Multiplexed mode example" p77 from "OCTOSPI application examples AN5050 Rev 12" :


/* USER CODE BEGIN Private defines */
#define BUFFERSIZE 0x4000
#define DLYB_BUFFERSIZE (COUNTOF(Cal_buffer) - 1)
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END Private defines */


/* USER CODE BEGIN PV */
/*define a 64Kbyte buffer for HyperRam data read with CPU*/
#pragma location = 0x20020000
uint32_t RxHyperRAM[BUFFERSIZE];
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
void EnableMemMapped(void);
void DelayBlock_Calibration(void);
/* USER CODE END PFP */

void Main_Custom_4(void)
{
	/* USER CODE BEGIN 1 */
	/*pointer on OCTOSPI1 memory mapped address region*/
	__IO uint32_t *OCTOSPI1_MEMMAPPED_ADD = (__IO uint32_t *)(OCTOSPI1_BASE);
	/* USER CODE END 1 */

	/* USER CODE BEGIN 2 */
	/*Configure the MAXTRAN feature for 241 clock cycles for OCTOSPI1 and
	OCTOSPI2 (4μs of max transaction period)*/
	MAXTRAN_Configuration();

	/*Configure and Enable the Memory Mapped mode for both OCTOSPI1 and OCTOSPI2
	respectively at address 0x90000000 and 0x70000000*/
	EnableMemMapped();

	/*Delay block Calibration*/
	DelayBlock_Calibration();

	/*Start Data read (64Kbyte) with DMA1 from the HyperFlash (0x70000000) to
	the internal SRAM3 (0x20030000)*/
	if(HAL_DMA_Start(&hdma_memtomem_dma1_channel1,OCTOSPI2_BASE, SRAM3_BASE,
	BUFFERSIZE) != HAL_OK)
	{
		Error_Handler();
	}

	/*Start Data read (64Kbyte) with CPU from the HyperRAM (0x90000000) to
	the internal SRAM2 (0x20020000) while the DMA is reading from HyperFLASH*/
	for (index = 0; index < BUFFERSIZE; index++)
	{
		RxHyperRAM[index] = *OCTOSPI1_MEMMAPPED_ADD++;
	}
	/* USER CODE END 2 */
}


/* USER CODE BEGIN 4 */
/* Memory-mapped mode configuration for OCTOSPI1 and OCTOSPI2--------- */
void EnableMemMapped(void)
{
	OSPI_HyperbusCmdTypeDef sCommand;
	OSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Memory-mapped mode configuration ---------------------------------- */
	sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	sCommand.Address = 0;
	sCommand.NbData = 1;
	if (HAL_OSPI_HyperbusCmd(&hospi1, &sCommand,
	HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_OSPI_HyperbusCmd(&hospi2, &sCommand,
	HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_ENABLE;
	sMemMappedCfg.TimeOutPeriod = 0x1;
	if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	}
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_ENABLE;
	sMemMappedCfg.TimeOutPeriod = 0x1;
	if (HAL_OSPI_MemoryMapped(&hospi2, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	}
}

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
					Memory*/
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
				Memory */
					Error_Handler();
				}
			}
			else
			{
				Max_Window = delay;
				Max_found=1;
			}
		}

		/* min and max delay window found , configure the delay block with the
		middle window value and exit calibration */
		else
		{
			Mid_window = (Max_Window+Min_Window)/2;
			HAL_RCCEx_OCTOSPIDelayConfig(Mid_window, 0);
			/* Exit calibration */
			calibration_ongoing = 0;
		}
	}
}

/* MAXTRAN configuration function for OCTOSPI1 and OCTOSPI2 */
void MAXTRAN_Configuration(void)
{
	/*Maximum transaction configured for 4us*/
	MODIFY_REG(hospi1.Instance->DCR3, OCTOSPI_DCR3_MAXTRAN, 0x000000F1);
	MODIFY_REG(hospi2.Instance->DCR3, OCTOSPI_DCR3_MAXTRAN, 0x000000F1);
}
/* USER CODE END 4 */

