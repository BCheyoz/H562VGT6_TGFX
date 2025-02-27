/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AFF_D_C_Pin GPIO_PIN_3
#define AFF_D_C_GPIO_Port GPIOE
#define DI_Anode_Pin GPIO_PIN_5
#define DI_Anode_GPIO_Port GPIOA
#define AFF_RESET_Pin GPIO_PIN_4
#define AFF_RESET_GPIO_Port GPIOC
#define LED_ALIVE_Pin GPIO_PIN_15
#define LED_ALIVE_GPIO_Port GPIOE
#define INVERTER_TX_Pin GPIO_PIN_10
#define INVERTER_TX_GPIO_Port GPIOB
#define BLE_EN_Pin GPIO_PIN_8
#define BLE_EN_GPIO_Port GPIOD
#define INVERTER_RX_Pin GPIO_PIN_9
#define INVERTER_RX_GPIO_Port GPIOD
#define BLE_IRQ_Pin GPIO_PIN_10
#define BLE_IRQ_GPIO_Port GPIOD
#define BLE_RX_Pin GPIO_PIN_11
#define BLE_RX_GPIO_Port GPIOD
#define BLE_TX_Pin GPIO_PIN_12
#define BLE_TX_GPIO_Port GPIOD
#define ModBus_User_DE_Pin GPIO_PIN_8
#define ModBus_User_DE_GPIO_Port GPIOC
#define DI_J_N_Pin GPIO_PIN_3
#define DI_J_N_GPIO_Port GPIOD
#define Sensor_IC2_SDA_Pin GPIO_PIN_9
#define Sensor_IC2_SDA_GPIO_Port GPIOC
#define Sensor_IC2_SCL_Pin GPIO_PIN_8
#define Sensor_IC2_SCL_GPIO_Port GPIOA
#define TouchScreen_I2C_SDA_Pin GPIO_PIN_7
#define TouchScreen_I2C_SDA_GPIO_Port GPIOB
#define TouchScreen_I2C_SCL_Pin GPIO_PIN_8
#define TouchScreen_I2C_SCL_GPIO_Port GPIOB
#define DI_SMART_Pin GPIO_PIN_4
#define DI_SMART_GPIO_Port GPIOD
#define ModBus_User_Rx_Pin GPIO_PIN_5
#define ModBus_User_Rx_GPIO_Port GPIOB
#define ModBus_User_Tx_Pin GPIO_PIN_6
#define ModBus_User_Tx_GPIO_Port GPIOB
#define DO_Appoint_Pin GPIO_PIN_0
#define DO_Appoint_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
