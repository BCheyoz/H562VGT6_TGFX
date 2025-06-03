/*
 * Display_FF028T010_conf.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#ifndef STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_CONF_H_
#define STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32h5xx.h"
#include <ST7789.h>
#include "spi.h"

/*** BSP / HW configuration ***********************************************/
#define hLCDSPI                         hspi4
#define LCD_SPI_INIT					MX_SPI4_Init

/* CS Pin mapping */
#define LCD_CS_GPIO_PORT                GPIOE
#define LCD_CS_GPIO_PIN                 GPIO_PIN_4

/* DCX Pin mapping */
#define LCD_DCX_GPIO_PORT               GPIOE
#define LCD_DCX_GPIO_PIN                GPIO_PIN_3

/* RESET Pin mapping */
#define LCD_RESET_GPIO_PORT             GPIOC
#define LCD_RESET_GPIO_PIN              GPIO_PIN_4

/* BackLight PWM pin */
#define LCD_BACKLIGHT_HANDLE  			&htim17
#define LCD_BACKLIGHT_CHANNEL_ID  		TIM_CHANNEL_1	// TIM17_CH1

#define LCD_SPI_POLL_TIMEOUT            0x50U // ms

/*** Display configuration ***********************************************/
#define LCD_ORIENTATION            		ST7789V_ORIENTATION_LANDSCAPE
#define LCD_HEIGHT 						240
#define LCD_WIDTH  						320
#define LCD_COLOR_FORMAT  				ST7789V_FORMAT_RBG565


#ifdef __cplusplus
}
#endif
#endif /* STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_CONF_H_ */
