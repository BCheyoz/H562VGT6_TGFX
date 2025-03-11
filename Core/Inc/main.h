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
#define AFF_SPI4_SCK_Pin GPIO_PIN_2
#define AFF_SPI4_SCK_GPIO_Port GPIOE
#define AFF_D_C_Pin GPIO_PIN_3
#define AFF_D_C_GPIO_Port GPIOE
#define AFF_SPI4_CS_Pin GPIO_PIN_4
#define AFF_SPI4_CS_GPIO_Port GPIOE
#define AFF_SPI4_SDO_Pin GPIO_PIN_5
#define AFF_SPI4_SDO_GPIO_Port GPIOE
#define AFF_SPI4_SDA_Pin GPIO_PIN_6
#define AFF_SPI4_SDA_GPIO_Port GPIOE
#define TouchScreen_Y__Pin GPIO_PIN_0
#define TouchScreen_Y__GPIO_Port GPIOC
#define TouchScreen_Y_C1_Pin GPIO_PIN_1
#define TouchScreen_Y_C1_GPIO_Port GPIOC
#define TouchScreen_X__Pin GPIO_PIN_2
#define TouchScreen_X__GPIO_Port GPIOC
#define TouchScreen_X_C3_Pin GPIO_PIN_3
#define TouchScreen_X_C3_GPIO_Port GPIOC
#define AI_T2_Pin GPIO_PIN_0
#define AI_T2_GPIO_Port GPIOA
#define AI_T1_Pin GPIO_PIN_1
#define AI_T1_GPIO_Port GPIOA
#define AI_T5_Pin GPIO_PIN_2
#define AI_T5_GPIO_Port GPIOA
#define AI_T4_Pin GPIO_PIN_3
#define AI_T4_GPIO_Port GPIOA
#define AI_T3_Pin GPIO_PIN_4
#define AI_T3_GPIO_Port GPIOA
#define DI_Anode_Pin GPIO_PIN_5
#define DI_Anode_GPIO_Port GPIOA
#define Flash_Qspi_IO3_Pin GPIO_PIN_6
#define Flash_Qspi_IO3_GPIO_Port GPIOA
#define Flash_Qspi_IO2_Pin GPIO_PIN_7
#define Flash_Qspi_IO2_GPIO_Port GPIOA
#define AFF_RESET_Pin GPIO_PIN_4
#define AFF_RESET_GPIO_Port GPIOC
#define Flash_Qspi_IO1_Pin GPIO_PIN_0
#define Flash_Qspi_IO1_GPIO_Port GPIOB
#define Flash_Qspi_IO0_Pin GPIO_PIN_1
#define Flash_Qspi_IO0_GPIO_Port GPIOB
#define Flash_Qspi_Clk_Pin GPIO_PIN_2
#define Flash_Qspi_Clk_GPIO_Port GPIOB
#define Flash_Qspi_CS_Pin GPIO_PIN_11
#define Flash_Qspi_CS_GPIO_Port GPIOE
#define LED_ALIVE_Pin GPIO_PIN_15
#define LED_ALIVE_GPIO_Port GPIOE
#define INVERTER_TX_Pin GPIO_PIN_10
#define INVERTER_TX_GPIO_Port GPIOB
#define BLE_SPI_CS_Pin GPIO_PIN_12
#define BLE_SPI_CS_GPIO_Port GPIOB
#define BLE_SPI_SCK_Pin GPIO_PIN_13
#define BLE_SPI_SCK_GPIO_Port GPIOB
#define BLE_SPI_MISO_Pin GPIO_PIN_14
#define BLE_SPI_MISO_GPIO_Port GPIOB
#define BLE_SPI_MOSI_Pin GPIO_PIN_15
#define BLE_SPI_MOSI_GPIO_Port GPIOB
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
#define DO_PWM_Exhaust_Pin GPIO_PIN_14
#define DO_PWM_Exhaust_GPIO_Port GPIOD
#define ModBus_User_DE_Pin GPIO_PIN_8
#define ModBus_User_DE_GPIO_Port GPIOC
#define Sensor_IC2_SDA_Pin GPIO_PIN_9
#define Sensor_IC2_SDA_GPIO_Port GPIOC
#define Sensor_IC2_SCL_Pin GPIO_PIN_8
#define Sensor_IC2_SCL_GPIO_Port GPIOA
#define DI_Tachy_Exhaust_Pin GPIO_PIN_9
#define DI_Tachy_Exhaust_GPIO_Port GPIOA
#define Flash_SPI_CS_Pin GPIO_PIN_15
#define Flash_SPI_CS_GPIO_Port GPIOA
#define Flash_SPI_SCK_Pin GPIO_PIN_10
#define Flash_SPI_SCK_GPIO_Port GPIOC
#define Flash_SPI_MISO_Pin GPIO_PIN_11
#define Flash_SPI_MISO_GPIO_Port GPIOC
#define Flash_SPI_MOSI_Pin GPIO_PIN_12
#define Flash_SPI_MOSI_GPIO_Port GPIOC
#define DI_J_N_Pin GPIO_PIN_3
#define DI_J_N_GPIO_Port GPIOD
#define DI_SMART_Pin GPIO_PIN_4
#define DI_SMART_GPIO_Port GPIOD
#define ModBus_User_Rx_Pin GPIO_PIN_5
#define ModBus_User_Rx_GPIO_Port GPIOB
#define ModBus_User_Tx_Pin GPIO_PIN_6
#define ModBus_User_Tx_GPIO_Port GPIOB
#define TouchScreen_I2C_SDA_Pin GPIO_PIN_7
#define TouchScreen_I2C_SDA_GPIO_Port GPIOB
#define TouchScreen_I2C_SCL_Pin GPIO_PIN_8
#define TouchScreen_I2C_SCL_GPIO_Port GPIOB
#define Anode_Pin	GPIO_PIN_5
#define Anode_GPIO_Port GPIOA
#define PWM_BackLight_Pin GPIO_PIN_9
#define PWM_BackLight_GPIO_Port GPIOB
#define DO_Appoint_Pin GPIO_PIN_0
#define DO_Appoint_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
