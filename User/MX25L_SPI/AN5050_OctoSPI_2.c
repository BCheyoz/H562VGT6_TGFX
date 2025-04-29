/*
 * AN5050_OctoSPI_2.c
 *
 *  Created on: Apr 29, 2025
 *      Author: j.proux
 */

#define __IO

// from https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/dd/af/52/e1/d3/48/8e/DM00407776/files/DM00407776.pdf
// Exemple "II. Octo-SPI FLASH in Regular-command protocol example" p61 from "OCTOSPI application examples AN5050 Rev 12" :


/* USER CODE BEGIN Private defines */
/* MX25LM512ABA1G12 Macronix memory */
/* Flash commands */
#define OCTAL_IO_DTR_READ_CMD		0xEE11
#define OCTAL_IO_READ_CMD			0xEC13
#define OCTAL_PAGE_PROG_CMD			0x12ED
#define OCTAL_READ_STATUS_REG_CMD	0x05FA
#define OCTAL_SECTOR_ERASE_CMD		0x21DE
#define OCTAL_WRITE_ENABLE_CMD		0x06F9
#define READ_STATUS_REG_CMD			0x05
#define WRITE_CFG_REG_2_CMD			0x72
#define WRITE_ENABLE_CMD			0x06

/* Dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ		6
#define DUMMY_CLOCK_CYCLES_READ_REG	4

/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE	0x02
#define WRITE_ENABLE_MASK_VALUE		0x02
#define MEMORY_READY_MATCH_VALUE	0x00
#define MEMORY_READY_MASK_VALUE		0x01
#define AUTO_POLLING_INTERVAL		0x10

/* Memory registers address */
#define CONFIG_REG2_ADDR1			0x0000000
#define CR2_STR_OPI_ENABLE			0x01
#define CR2_DTR_OPI_ENABLE			0x02
#define CONFIG_REG2_ADDR3			0x00000300
#define CR2_DUMMY_CYCLES_66MHZ		0x07

/* Exported macro ------------------------------------------------------*/
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__)/sizeof(*(__BUFFER__)))

/* Size of buffers */
#define BUFFERSIZE (COUNTOF(aTxBuffer) - 1)

/* USER CODE END Private defines */


/* USER CODE BEGIN PV */
/* Private variables --------------------------------------------------*/
uint8_t aTxBuffer[]=" Programming in indirect mode - Reading in memory- \
mapped mode ";
__IO uint8_t *nor_memaddr = (__IO uint8_t *)(OCTOSPI2_BASE);
__IO uint8_t aRxBuffer[BUFFERSIZE] ="";
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes ---------------------------------------*/
void WriteEnable(void);
void OctalWriteEnable(void);
void OctalDTR_MemoryCfg(void);
void OctalSectorErase(void);
void OctalDTR_MemoryWrite(void);
void AutoPollingWIP(void);
void OctalPollingWEL(void);
void OctalPollingWIP(void);
void EnableMemMapped(void);
/* USER CODE END PFP */

void Main_Custom_2(void)
{

/* USER CODE BEGIN 1 */
uint16_t index1;
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
/*----------------------------------------------------------------------*/
/*-------------- MX25LM51245G memory configuration --------------*/
	/* Configure MX25LM51245G memory to DTR Octal I/O mode */
	OctalDTR_MemoryCfg();
/*----------------------------------------------------------------------*/

/*----------------------- Erasing the first sector ----------------------*/
	/* Enable writing to memory using Octal Write Enable cmd */
	OctalWriteEnable();
	/* Enable Octal Software Polling to wait until WEL=1 */
	OctalPollingWEL ();
	/* Erasing first sector using Octal erase cmd */
	OctalSectorErase();
	/* Enable Octal Software Polling to wait until memory is ready WIP=0*/
	OctalPollingWIP();
/*----------------------------------------------------------------------*/

/*--------------------- Programming operation ----------------------*/
	/* Enable writing to memory using Octal Write Enable cmd */
	OctalWriteEnable();
	/* Enable Octal Software Polling to wait until WEL=1 */
	OctalPollingWEL();
	/* Writing (using CPU) the aTxBuffer to the memory */
	OctalDTR_MemoryWrite();
	/* Enable Octal Software Polling to wait until memory is ready WIP=0*/
	OctalPollingWIP();
/*---------------------------------------------------------------------*/

/*------ Configure memory-mapped Octal SDR Read/write ------*/
EnableMemMapped();
/*----------------------------------------------------------------------*/

/*---------------- Reading from the NOR memory ------------------*/
for(index = 0; index < BUFFERSIZE; index++)
{
	/* Reading back the written aTxBuffer in memory-mapped mode */
	aRxBuffer[index] = *nor_memaddr;
	if(aRxBuffer[index] != aTxBuffer[index])
	{
		/* Can add code to toggle a LED when data doesn't match */
	}
	nor_memaddr++;
}
/*----------------------------------------------------------------------*/
/* USER CODE END 2 */

}

/* USER CODE BEGIN 4 */
/* This function Enables writing to the memory: write enable cmd is sent in
single SPI mode */
void WriteEnable(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_AutoPollingTypeDef sConfig;

	/* Initialize the Write Enable cmd in single SPI mode */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = WRITE_ENABLE_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

	/* Send Write Enable command in single SPI mode */
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Initialize Automatic-Polling mode to wait until WEL=1 */
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.DataMode = HAL_OSPI_DATA_1_LINE;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.NbData = 1;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Set the mask to 0x02 to mask all Status REG bits except WEL */
	/* Set the match to 0x02 to check if the WEL bit is set */
	sConfig.Match = WRITE_ENABLE_MATCH_VALUE;
	sConfig.Mask = WRITE_ENABLE_MASK_VALUE;
	sConfig.MatchMode = HAL_OSPI_MATCH_MODE_AND;
	sConfig.Interval = AUTO_POLLING_INTERVAL;
	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

	/* Start Automatic-Polling mode to wait until WEL=1 */
	if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)
	!= HAL_OK)
	{
		Error_Handler();
	}
}

/* This functions Enables writing to the memory: write enable cmd is sent in
Octal SPI mode */
void OctalWriteEnable(void)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Initialize the Write Enable cmd */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = OCTAL_WRITE_ENABLE_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

	/* Send Write Enable command in Octal mode */
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
}

/* This function Configures Software polling to wait until WEL=1 */
void OctalPollingWEL(void)
	{
	OSPI_AutoPollingTypeDef sConfig;
	OSPI_RegularCmdTypeDef sCommand;

	/* Initialize Indirect read mode for Software Polling to wait until WEL=1 */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = OCTAL_READ_STATUS_REG_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.Address = 0x0;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.NbData = 2;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_REG;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

	/* Set the mask to 0x02 to mask all Status REG bits except WEL */
	/* Set the match to 0x02 to check if the WEL bit is Set */
	sConfig.Match = WRITE_ENABLE_MATCH_VALUE;
	sConfig.Mask = WRITE_ENABLE_MASK_VALUE;
	sConfig.MatchMode = HAL_OSPI_MATCH_MODE_AND;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Start Automatic-Polling mode to wait until the memory is ready WEL=1 */
	if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)
	!= HAL_OK)
	{
		Error_Handler();
	}
}

/* This function Configures Automatic-polling mode to wait until WIP=0 */
void AutoPollingWIP(void)
	{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_AutoPollingTypeDef sConfig;

	/* Initialize Automatic-Polling mode to wait until WIP=0 */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = READ_STATUS_REG_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.DataMode = HAL_OSPI_DATA_1_LINE;
	sCommand.NbData = 1;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;

	/* Set the mask to 0x01 to mask all Status REG bits except WIP */
	/* Set the match to 0x00 to check if the WIP bit is Reset */
	sConfig.Match = MEMORY_READY_MATCH_VALUE;
	sConfig.Mask = MEMORY_READY_MASK_VALUE;
	sConfig.MatchMode = HAL_OSPI_MATCH_MODE_AND;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Start Automatic-Polling mode to wait until the memory is ready WIP=0 */
	if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)
	!= HAL_OK)
	{
		Error_Handler();
	}
}

/* This function Configures Software polling mode to wait the memory is
ready WIP=0 */
void OctalPollingWIP(void)
	{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_AutoPollingTypeDef sConfig;

	/* Initialize Automatic-Polling mode to wait until WIP=0 */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = OCTAL_READ_STATUS_REG_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.Address = 0x0;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.NbData = 2;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_REG;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;

	/* Set the mask to 0x01 to mask all Status REG bits except WIP */
	/* Set the match to 0x00 to check if the WIP bit is Reset */
	sConfig.Match = MEMORY_READY_MATCH_VALUE;
	sConfig.Mask = MEMORY_READY_MASK_VALUE;
	sConfig.MatchMode = HAL_OSPI_MATCH_MODE_AND;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Start Automatic-Polling mode to wait until the memory is ready WIP=0 */
	if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)
	!= HAL_OK)
	{
		Error_Handler();
	}
}

/*** This function configures the MX25LM51245G memory ***/
void OctalDTR_MemoryCfg(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	uint8_t tmp;

	/* Enable writing to memory in order to set Dummy */
	WriteEnable();

	/* Initialize Indirect write mode to configure Dummy */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Instruction = WRITE_CFG_REG_2_CMD;
	sCommand.Address = CONFIG_REG2_ADDR3;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_1_LINE;
	sCommand.DataDtrMode= HAL_OSPI_DATA_DTR_DISABLE;
	sCommand.NbData = 1;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Write Configuration register 2 with new dummy cycles */
	tmp = CR2_DUMMY_CYCLES_66MHZ;
	if (HAL_OSPI_Transmit(&hospi2, &tmp, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
	AutoPollingWIP();

	/* Enable writing to memory in order to set Octal DTR mode */
	WriteEnable();

	/* Initialize OCTOSPI1 to Indirect write mode to configure Octal mode */
	sCommand.Instruction = WRITE_CFG_REG_2_CMD;
	sCommand.Address = CONFIG_REG2_ADDR1;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Write Configuration register 2 with with Octal mode */
	tmp = CR2_DTR_OPI_ENABLE;
	if (HAL_OSPI_Transmit(&hospi2, &tmp, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
}

/* This function erases the first memory sector */
void OctalSectorErase(void)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Initialize Indirect write mode to erase the first sector */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = OCTAL_SECTOR_ERASE_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_NONE;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.DummyCycles = 0;
	sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.Address = 0;

	/* Send Octal Sector erase cmd */
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}
}

/* This function writes the memory */
void OctalDTR_MemoryWrite(void)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Initialize Indirect write mode for memory programming */
	sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.Instruction = OCTAL_PAGE_PROG_CMD;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.Address = 0x00000000;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
	sCommand.NbData = BUFFERSIZE;
	sCommand.DummyCycles = 0;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Memory Page programming */
	if (HAL_OSPI_Transmit(&hospi2, aTxBuffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)!=
	HAL_OK)
	{
		Error_Handler();
	}
}

/* This function enables memory-mapped mode for Read and Write */
void EnableMemMapped(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	OSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Initialize memory-mapped mode for read operations */
	sCommand.OperationType = HAL_OSPI_OPTYPE_READ_CFG;
	sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
	sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
	sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
	sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
	sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
	sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ;
	sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.Instruction = OCTAL_IO_DTR_READ_CMD;
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_ENABLE;
	sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_ENABLE;
	sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_ENABLE;
	sCommand.DQSMode = HAL_OSPI_DQS_ENABLE;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Initialize memory-mapped mode for write operations */
	sCommand.OperationType = HAL_OSPI_OPTYPE_WRITE_CFG;
	sCommand.Instruction = OCTAL_PAGE_PROG_CMD;
	sCommand.DummyCycles = 0;
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) !=
	HAL_OK)
	{
		Error_Handler();
	}

	/* Configure the memory mapped mode with TimeoutCounter Disabled*/
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
	if (HAL_OSPI_MemoryMapped(&hospi2, &sMemMappedCfg) != HAL_OK)
	{
		Error_Handler();
	}
}
/* USER CODE END 4 */
