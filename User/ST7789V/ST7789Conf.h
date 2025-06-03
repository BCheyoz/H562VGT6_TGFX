/*
 *  ST7789Conf.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#ifndef ST7789V_ST7789V_DEFINE_H
#define ST7789V_ST7789V_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


#define ST7789V_OK                (0)
#define ST7789V_ERROR             (-1)

#define  ST7789V_ID              0x858552U

/**
 * @brief  ST7789V Timing
 */
/* Timing configuration  (Typical configuration from ST7789V datasheet)
HSYNC=10 (9+1)
HBP=20 (29-10+1)
ActiveW=240 (269-20-10+1)
HFP=10 (279-240-20-10+1)

VSYNC=2 (1+1)
VBP=2 (3-2+1)
ActiveH=320 (323-2-2+1)
VFP=4 (327-320-2-2+1)
 */
#define  ST7789V_HSYNC            ((uint32_t)9)     /* Horizontal synchronization */
#define  ST7789V_HBP              ((uint32_t)29)    /* Horizontal back porch      */
#define  ST7789V_HFP              ((uint32_t)2)     /* Horizontal front porch     */
#define  ST7789V_VSYNC            ((uint32_t)1)     /* Vertical synchronization   */
#define  ST7789V_VBP              ((uint32_t)3)     /* Vertical back porch        */
#define  ST7789V_VFP              ((uint32_t)2)     /* Vertical front porch       */

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define ST7789V_ORIENTATION_PORTRAIT         0x00U /* Portrait orientation choice of LCD screen               */
#define ST7789V_ORIENTATION_PORTRAIT_ROT180  0x01U /* Portrait rotated 180° orientation choice of LCD screen  */
#define ST7789V_ORIENTATION_LANDSCAPE        0x02U /* Landscape orientation choice of LCD screen              */
#define ST7789V_ORIENTATION_LANDSCAPE_ROT180 0x03U /* Landscape rotated 180° orientation choice of LCD screen */

/**
 *  @brief  Possible values of pixel data format (ie color coding)
 */
#define ST7789V_FORMAT_RBG565                0x55U /* Pixel format chosen is RGB565 : 16 bpp */
#define ST7789V_FORMAT_RBG666                0x66U /* Pixel format chosen is RGB666 : 18 bpp */

/*** ST7789V Registers *********************************************************************/
/* Level 1 Commands */
#define ST7789V_NOP                       0x00U   /* No operation */
#define ST7789V_SWRESET                   0x01U   /* Software Reset */
#define ST7789V_READ_DISPLAY_ID           0x04U   /* Read display identification information */
#define ST7789V_RDDST                     0x09U   /* Read Display Status */
#define ST7789V_RDDPM                     0x0AU   /* Read Display Power Mode */
#define ST7789V_RDDMADCTL                 0x0BU   /* Read Display MADCTL */
#define ST7789V_RDDCOLMOD                 0x0CU   /* Read Display Pixel Format */
#define ST7789V_RDDIM                     0x0DU   /* Read Display Image Format */
#define ST7789V_RDDSM                     0x0EU   /* Read Display Signal Mode */
#define ST7789V_RDDSDR                    0x0FU   /* Read Display Self-Diagnostic Result */
#define ST7789V_SPLIN                     0x10U   /* Enter Sleep Mode */
#define ST7789V_SLEEP_OUT                 0x11U   /* Sleep out register */
#define ST7789V_PTLON                     0x12U   /* Partial Mode ON */
#define ST7789V_NORMAL_MODE_ON            0x13U   /* Normal Display Mode ON */
#define ST7789V_DINVOFF                   0x20U   /* Display Inversion OFF */
#define ST7789V_DINVON                    0x21U   /* Display Inversion ON */
#define ST7789V_GAMMA                     0x26U   /* Gamma register */
#define ST7789V_DISPLAY_OFF               0x28U   /* Display off register */
#define ST7789V_DISPLAY_ON                0x29U   /* Display on register */
#define ST7789V_CASET                     0x2AU   /* Colomn address register */
#define ST7789V_RASET                     0x2BU   /* Raw address register */
#define ST7789V_GRAM                      0x2CU   /* GRAM register */
#define ST7789V_RGBSET                    0x2DU   /* Color SET */
#define ST7789V_RAMRD                     0x2EU   /* Memory Read */
#define ST7789V_PLTAR                     0x30U   /* Partial Area */
#define ST7789V_VSCRDEF                   0x33U   /* Vertical Scrolling Definition */
#define ST7789V_TEOFF                     0x34U   /* Tearing Effect Line OFF */
#define ST7789V_TEON                      0x35U   /* Tearing Effect Line ON */
#define ST7789V_MAC                       0x36U   /* Memory Access Control register*/
#define ST7789V_VSCRSADD                  0x37U   /* Vertical Scrolling Start Address */
#define ST7789V_IDMOFF                    0x38U   /* Idle Mode OFF */
#define ST7789V_IDMON                     0x39U   /* Idle Mode ON */
#define ST7789V_PIXEL_FORMAT              0x3AU   /* Pixel Format register */
#define ST7789V_WRITE_MEM_CONTINUE        0x3CU   /* Write Memory Continue */
#define ST7789V_READ_MEM_CONTINUE         0x3EU   /* Read Memory Continue */
#define ST7789V_SET_TEAR_SCANLINE         0x44U   /* Set Tear Scanline */
#define ST7789V_GET_SCANLINE              0x45U   /* Get Scanline */
#define ST7789V_WDB                       0x51U   /* Write Brightness Display register */
#define ST7789V_RDDISBV                   0x52U   /* Read Display Brightness */
#define ST7789V_WCD                       0x53U   /* Write Control Display register*/
#define ST7789V_RDCTRLD                   0x54U   /* Read CTRL Display */
#define ST7789V_WRCABC                    0x55U   /* Write Content Adaptive Brightness Control */
#define ST7789V_RDCABC                    0x56U   /* Read Content Adaptive Brightness Control */
#define ST7789V_WRITE_CABC                0x5EU   /* Write CABC Minimum Brightness */
#define ST7789V_READ_CABC                 0x5FU   /* Read CABC Minimum Brightness */
#define ST7789V_READ_ABCSDR               0x68U   /* Read Automatic Brightness Control Self-Diagnostic Result */
#define ST7789V_READ_ID1                  0xDAU   /* Read ID1 */
#define ST7789V_READ_ID2                  0xDBU   /* Read ID2 */
#define ST7789V_READ_ID3                  0xDCU   /* Read ID3 */

/* Level 2 Commands */
#define ST7789V_RAM_CTRL                  0xB0U   /* RAM Control */
#define ST7789V_RGB_INTERFACE_CTRL        0xB1U   /* Porch Setting */
#define ST7789V_PORCH_CTRL                0xB2U   /* RGB Interface Signal Control */
#define ST7789V_FRAME_RATE_CTRL1          0xB3U   /* Frame Rate Control 1 (In partial mode/ idle colors)) */
#define ST7789V_PARTIAL_CTRL              0xB5U   /* N/A */
#define ST7789V_GATE_CTRL                 0xB7U   /* Gate Control */
#define ST7789V_GATE_TIMING_ADJUSTMENT    0xB8U   /* Timing Adjustement */
#define ST7789V_DIGITAL_GAMMA_ENABLE      0xBAU   /* Digital Gamma Enable */
#define ST7789V_VCOM_SET                  0xBBU   /* VCOM Setting */
#define ST7789V_PWR_SAVING_MODE           0xBCU   /* LCM Control */
#define ST7789V_DISPLAY_OFF_PWR_SAVE      0xBDU   /* N/A */
#define ST7789V_LCM_CTRL                  0xC0U   /* N/A */
#define ST7789V_ID_CODE_SETTING           0xC1U   /* ID Code Setting */
#define ST7789V_VDV_VRH_EN                0xC2U   /* VDV and VRH Command Enable */
#define ST7789V_VRH_SET                   0xC3U   /* VRH Set */
#define ST7789V_VDV_SET                   0xC4U   /* VDV Set */
#define ST7789V_VCOMH_OFFSET_SET          0xC5U   /* VCOM Offset Set */
#define ST7789V_FRAME_RATE_CTRL2          0xC6U   /* Frame Rate Control 2 (In Normal Mode) */
#define ST7789V_CABC_CTRL                 0xC7U   /* CABC Control */
#define ST7789V_REG_VALUE_SELECTION1      0xC8U   /* Register Value Selection 1 */
#define ST7789V_REG_VALUE_SELECTION2      0xCAU   /* Register Value Selection 2 */
#define ST7789V_PWM_FREQ_SELECTION        0xCCU   /* PWM Frequency Selection */
#define ST7789V_POWER_CTRL                0xD0U   /* Power Control 1 */
#define ST7789V_EN_VAP_VAN_SIGNAL_OUTPUT  0xD2U   /* Enable VAP/VAN signal output */
#define ST7789V_COMMAND2_ENABLE           0xDFU   /* Command 2 Enable */
#define ST7789V_PV_GAMMA_CTRL             0xE0U   /* Positive Voltage Gamma Control */
#define ST7789V_NV_GAMMA_CTRL             0xE1U   /* Negative Voltage Gamma Control */
#define ST7789V_GAMMA_RED_TABLE           0xE2U   /* Digital Gamma Look-up Table for Red */
#define ST7789V_GAMMA_BLUE_TABLE          0xE3U   /* Digital Gamma Look-up Table for Blue */
#define ST7789V_GATE_CTRL2                0xE4U   /* Gate Control */
#define ST7789V_SPI2_ENABLE               0xE7U   /* SPI2 Enable */
#define ST7789V_PWR_CTRL2                 0xE8U   /* Power Control 2 */
#define ST7789V_EQUALIZE_TIME_CTRL        0xE9U   /* Equalize time control */
#define ST7789V_PROGRAM_MODE_CTRL         0xECU   /* Program Mode Control */
#define ST7789V_PROGRAM_MODE_ENABLE       0xFAU   /* Program Mode Enable */
#define ST7789V_NVM_SETTING               0xFCU   /* NVM Setting */
#define ST7789V_PROGRAM_ACTION            0xFEU   /* Program action */

#ifdef __cplusplus
}
#endif

#endif /* ST7789V_ST7789V_DEFINE_H */

