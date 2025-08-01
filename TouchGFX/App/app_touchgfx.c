/**
  ******************************************************************************
  * File Name          : app_touchgfx.c
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.24.1. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t timer1ms;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
//void touchgfx_init(void);
//void touchgfx_components_init(void);
//void touchgfx_taskEntry(void);
__attribute__((weak)) void touchgfx_init(void); 			// Patch_Jp from "%UserProfile%\STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-TOUCHGFX\4.24.1\STM32CubeMX\templates\app_touchgfx_c.ftl", Line 77 : Always reset to this WEAK line, instead of the (default) line from CubeMX !
__attribute__((weak)) void touchgfx_components_init(void);	// Patch_Jp from "%UserProfile%\STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-TOUCHGFX\4.24.1\STM32CubeMX\templates\app_touchgfx_c.ftl", Line 78 : Always reset to this WEAK line, instead of the (default) line from CubeMX !
__attribute__((weak)) void touchgfx_taskEntry(void);		// Patch_Jp from "%UserProfile%\STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-TOUCHGFX\4.24.1\STM32CubeMX\templates\app_touchgfx_c.ftl", Line 79 : Always reset to this WEAK line, instead of the (default) line from CubeMX !
__attribute__((weak)) void touchgfxSignalVSync(void);

/* USER CODE BEGIN PFP */
void Handle_TouchGFX_RT_1ms(){
	timer1ms++;
}
/* USER CODE END PFP */

/**
 * Initialize TouchGFX application
 */
void MX_TouchGFX_Init(void)
{
	timer1ms = 0;
    // Calling forward to touchgfx_init in C++ domain
    touchgfx_components_init();
    touchgfx_init();
}

/**
 * TouchGFX application entry function
 */
void MX_TouchGFX_Process(void)
{
	if(timer1ms > 16){
		timer1ms = 0;
		touchgfxSignalVSync();
	}

    // Calling forward to touchgfx_taskEntry in C++ domain
    touchgfx_taskEntry();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
