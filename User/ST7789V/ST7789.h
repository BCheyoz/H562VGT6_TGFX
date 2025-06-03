/*
 *  ST7789.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#ifndef ST7789_ST7789_H
#define ST7789_ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ST7789Conf.h>


/*** prototype du pointer de fonction */
typedef int32_t (*ST7789_Init_Func)     (void);
typedef int32_t (*ST7789_DeInit_Func)   (void);
typedef int32_t (*ST7789_GetTick_Func)  (void);
typedef int32_t (*ST7789_Delay_Func)    (uint32_t);
typedef int32_t (*ST7789_WriteReg_Func) (volatile uint8_t*, uint32_t);
typedef int32_t (*ST7789_ReadReg_Func)  (volatile uint8_t*, uint32_t);
typedef int32_t (*ST7789_SendData_Func) (uint8_t*, uint32_t);
typedef int32_t (*ST7789_RecvData_Func) (uint8_t*, uint32_t);

typedef enum
{
	ST7789_ENDIAN_BIG = 0,
	ST7789_ENDIAN_LITTLE
} ST7789_Endian_t;

typedef enum
{
	ST7789_TE_DISABLED = 0,
	ST7789_TE_MODE_1,
	ST7789_TE_MODE_2
} ST7789_TE_Mode_t;

typedef enum
{
	ST7789_119_Hz = 0,
	ST7789_111_Hz,
	ST7789_105_Hz,
	ST7789_99_Hz,
	ST7789_94_Hz,
	ST7789_90_Hz,
	ST7789_86_Hz,
	ST7789_82_Hz,
	ST7789_78_Hz,
	ST7789_75_Hz,
	ST7789_72_Hz,
	ST7789_69_Hz,
	ST7789_67_Hz,
	ST7789_64_Hz,
	ST7789_62_Hz,
	ST7789_60_Hz,
	ST7789_58_Hz,
	ST7789_57_Hz,
	ST7789_55_Hz,
	ST7789_53_Hz,
	ST7789_52_Hz,
	ST7789_50_Hz,
	ST7789_49_Hz,
	ST7789_48_Hz,
	ST7789_46_Hz,
	ST7789_45_Hz,
	ST7789_44_Hz,
	ST7789_43_Hz,
	ST7789_42_Hz,
	ST7789_41_Hz,
	ST7789_40_Hz,
	ST7789_39_Hz
} ST7789_FrameRate_t;

typedef struct
{
	ST7789_Init_Func          Init;
	ST7789_DeInit_Func        DeInit;
	uint16_t                   Address;
	ST7789_WriteReg_Func      WriteReg;
	ST7789_ReadReg_Func       ReadReg;
	ST7789_SendData_Func      SendData;
	ST7789_SendData_Func      SendDataDMA;
	ST7789_RecvData_Func      RecvData;
	ST7789_RecvData_Func      RecvDataDMA;
	ST7789_GetTick_Func       GetTick;
	ST7789_Delay_Func         Delay;
} ST7789_IO_t;

typedef struct
{
	ST7789_IO_t               IO;
	uint8_t                    IsInitialized;
	uint32_t                   XSize;
	uint32_t                   YSize;
} ST7789_Object_t;

typedef struct
{
	uint32_t                   hsync;   /* Horizontal synchronization */
	uint32_t                   hbp;     /* Horizontal back porch      */
	uint32_t                   hfp;     /* Horizontal front porch     */
	uint32_t                   vsync;   /* Vertical synchronization   */
	uint32_t                   vbp;     /* Vertical back porch        */
	uint32_t                   vfp;     /* Vertical front porch       */
} ST7789_TimingParams_t;

typedef struct
{
	ST7789_Endian_t           Endian;
	uint8_t                    SwapRB;
	uint8_t                    ColorCoding;
	uint8_t					 InvertColor;
	uint8_t                    Orientation;
	uint32_t                   TEScanline;
	ST7789_TE_Mode_t          TEMode;
	ST7789_TimingParams_t     Timings;
	ST7789_FrameRate_t		 FrameRate;
} ST7789_InitParams_t;


int32_t ST7789_Init(ST7789_Object_t *pObj, ST7789_InitParams_t* pInitParams);
int32_t ST7789_DeInit(ST7789_Object_t *pObj);
int32_t ST7789_ReadID(ST7789_Object_t *pObj, uint32_t *Id);
int32_t ST7789_DisplayOn(ST7789_Object_t *pObj);
int32_t ST7789_DisplayOff(ST7789_Object_t *pObj);
int32_t ST7789_SetDisplayWindow(ST7789_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
int32_t ST7789_SetOrientation(ST7789_Object_t *pObj, uint32_t Orientation);
int32_t ST7789_GetOrientation(ST7789_Object_t *pObj, uint32_t *Orientation);
int32_t ST7789_GetXSize(ST7789_Object_t *pObj, uint32_t *XSize);
int32_t ST7789_GetYSize(ST7789_Object_t *pObj, uint32_t *YSize);


#ifdef __cplusplus
}
#endif

#endif /* ST7789_ST7789_H */
