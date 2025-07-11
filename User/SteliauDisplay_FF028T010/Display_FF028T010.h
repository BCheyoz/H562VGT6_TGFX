/*
 * Display_FF028T010.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#ifndef STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_H_
#define STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "Display_FF028T010_conf.h"
#include "ST7789.h"

 /* Defines *******************************************************************/

#ifndef BSP_ERROR_NONE
#define BSP_ERROR_NONE                      0     /*!< \hideinitializer No error occured */
#define BSP_ERROR_NO_INIT                   -1    /*!< \hideinitializer Initialization function is not defined */
#define BSP_ERROR_WRONG_PARAM               -2    /*!< \hideinitializer Wrong parameters */
#define BSP_ERROR_BUSY                      -3    /*!< \hideinitializer Driver is busy */
#define BSP_ERROR_PERIPH_FAILURE            -4    /*!< \hideinitializer Peripheral error */
#define BSP_ERROR_COMPONENT_FAILURE         -5    /*!< \hideinitializer Componenet failure */
#define BSP_ERROR_UNKNOWN_FAILURE           -6    /*!< \hideinitializer Unknown failure */
#define BSP_ERROR_UNKNOWN_COMPONENT         -7    /*!< \hideinitializer Unknown component */
#define BSP_ERROR_BUS_FAILURE               -8    /*!< \hideinitializer Bus failure */
#define BSP_ERROR_CLOCK_FAILURE             -9    /*!< \hideinitializer Clock failure */
#define BSP_ERROR_MSP_FAILURE               -10   /*!< \hideinitializer MSP failure */
#define BSP_ERROR_FEATURE_NOT_SUPPORTED     -11   /*!< \hideinitializer Feature not supported */
#endif /* BSP_ERROR_NONE */

#define LCD_OS_TIMEOUT_BUSY                 ((uint32_t)  1) /*LCD OS Busy Timeout value in Milliseconds, default is 1ms */

typedef union{
	struct {
		unsigned b0: 1;
		unsigned b1: 1;
		unsigned b2: 1;
		unsigned b3: 1;
		unsigned b4: 1;
		unsigned g0: 1;
		unsigned g1: 1;
		unsigned g2: 1;
		unsigned g3: 1;
		unsigned g4: 1;
		unsigned g5: 1;
		unsigned r0: 1;
		unsigned r1: 1;
		unsigned r2: 1;
		unsigned r3: 1;
		unsigned r4: 1;
	};
	struct {
		uint8_t B0;
		uint8_t B1;
	};
	uint16_t color;
} rgb565;

typedef enum
{
  LCD_OS_ERROR_NONE = 0
, LCD_OS_ERROR_WRONG_PARAM
, LCD_OS_ERROR_INIT
, LCD_OS_ERROR_BUSY
, LCD_OS_ERROR_WAIT
, LCD_OS_ERROR_LOCK
, LCD_OS_ERROR_UNLOCK
} LCD_OS_Error_t;

/* prototypes *******************************************************************/
void    Display_FF028T010_Init();
int16_t Display_FF028T010_Status();
int16_t Display_FF028T010_isAlive();
void    Display_FF028T010_setBackLightLevel(uint8_t lvl);
uint8_t Display_FF028T010_backLightLevel();

int16_t BSP_LCD_DeInit();
int16_t BSP_LCD_SetOrientation(uint32_t Orientation);
int16_t BSP_LCD_GetOrientation(uint32_t *pOrientation);
int16_t BSP_LCD_GetXSize(uint32_t *pXSize);
int16_t BSP_LCD_GetYSize(uint32_t *pYSize);
int16_t BSP_LCD_DisplayOn();
int16_t BSP_LCD_DisplayOff();
int16_t BSP_LCD_WriteData(const uint8_t *pData, uint32_t Length);
int16_t BSP_LCD_SetDisplayWindow(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
uint8_t BSP_LCD_GetTransferStatus();
uint8_t BSP_LCD_GetPixelDepth();
void    BSP_LCD_WaitForTransferToBeDone();
//void    BSP_LCD_SignalTransferDone();
//void    BSP_LCD_SignalTearingEffectEvent(uint8_t State, uint16_t Line);

#ifdef __cplusplus
}
#endif
#endif /* STELIAU_DISPLAY_FF028T010_DISPLAY_FF028T010_H_ */
