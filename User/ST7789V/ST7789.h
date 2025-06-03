/*
 *  ST7789.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#ifndef ST7789V_ST7789V_H
#define ST7789V_ST7789V_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ST7789Conf.h>


/*** prototype du pointer de fonction */
typedef int32_t (*ST7789V_Init_Func)     (void);
typedef int32_t (*ST7789V_DeInit_Func)   (void);
typedef int32_t (*ST7789V_GetTick_Func)  (void);
typedef int32_t (*ST7789V_Delay_Func)    (uint32_t);
typedef int32_t (*ST7789V_WriteReg_Func) (volatile uint8_t*, uint32_t);
typedef int32_t (*ST7789V_ReadReg_Func)  (volatile uint8_t*, uint32_t);
typedef int32_t (*ST7789V_SendData_Func) (uint8_t*, uint32_t);
typedef int32_t (*ST7789V_RecvData_Func) (uint8_t*, uint32_t);

typedef enum
{
	ST7789V_ENDIAN_BIG = 0,
	ST7789V_ENDIAN_LITTLE
} ST7789V_Endian_t;

typedef enum
{
	ST7789V_TE_DISABLED = 0,
	ST7789V_TE_MODE_1,
	ST7789V_TE_MODE_2
} ST7789V_TE_Mode_t;

typedef enum
{
	ST7789V_119_Hz = 0,
	ST7789V_111_Hz,
	ST7789V_105_Hz,
	ST7789V_99_Hz,
	ST7789V_94_Hz,
	ST7789V_90_Hz,
	ST7789V_86_Hz,
	ST7789V_82_Hz,
	ST7789V_78_Hz,
	ST7789V_75_Hz,
	ST7789V_72_Hz,
	ST7789V_69_Hz,
	ST7789V_67_Hz,
	ST7789V_64_Hz,
	ST7789V_62_Hz,
	ST7789V_60_Hz,
	ST7789V_58_Hz,
	ST7789V_57_Hz,
	ST7789V_55_Hz,
	ST7789V_53_Hz,
	ST7789V_52_Hz,
	ST7789V_50_Hz,
	ST7789V_49_Hz,
	ST7789V_48_Hz,
	ST7789V_46_Hz,
	ST7789V_45_Hz,
	ST7789V_44_Hz,
	ST7789V_43_Hz,
	ST7789V_42_Hz,
	ST7789V_41_Hz,
	ST7789V_40_Hz,
	ST7789V_39_Hz
} ST7789V_FrameRate_t;

typedef struct
{
	ST7789V_Init_Func          Init;
	ST7789V_DeInit_Func        DeInit;
	uint16_t                   Address;
	ST7789V_WriteReg_Func      WriteReg;
	ST7789V_ReadReg_Func       ReadReg;
	ST7789V_SendData_Func      SendData;
	ST7789V_SendData_Func      SendDataDMA;
	ST7789V_RecvData_Func      RecvData;
	ST7789V_RecvData_Func      RecvDataDMA;
	ST7789V_GetTick_Func       GetTick;
	ST7789V_Delay_Func         Delay;
} ST7789V_IO_t;

typedef struct
{
	ST7789V_IO_t               IO;
	uint8_t                    IsInitialized;
	uint32_t                   XSize;
	uint32_t                   YSize;
} ST7789V_Object_t;

typedef struct
{
	uint32_t                   hsync;   /* Horizontal synchronization */
	uint32_t                   hbp;     /* Horizontal back porch      */
	uint32_t                   hfp;     /* Horizontal front porch     */
	uint32_t                   vsync;   /* Vertical synchronization   */
	uint32_t                   vbp;     /* Vertical back porch        */
	uint32_t                   vfp;     /* Vertical front porch       */
} ST7789V_TimingParams_t;

typedef struct
{
	ST7789V_Endian_t           Endian;
	uint8_t                    SwapRB;
	uint8_t                    ColorCoding;
	uint8_t					 InvertColor;
	uint8_t                    Orientation;
	uint32_t                   TEScanline;
	ST7789V_TE_Mode_t          TEMode;
	ST7789V_TimingParams_t     Timings;
	ST7789V_FrameRate_t		 FrameRate;
} ST7789V_InitParams_t;


int32_t ST7789V_Init(ST7789V_Object_t *pObj, ST7789V_InitParams_t* pInitParams);
int32_t ST7789V_DeInit(ST7789V_Object_t *pObj);
int32_t ST7789V_ReadID(ST7789V_Object_t *pObj, uint32_t *Id);
int32_t ST7789V_DisplayOn(ST7789V_Object_t *pObj);
int32_t ST7789V_DisplayOff(ST7789V_Object_t *pObj);
int32_t ST7789V_SetDisplayWindow(ST7789V_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
int32_t ST7789V_SetOrientation(ST7789V_Object_t *pObj, uint32_t Orientation);
int32_t ST7789V_GetOrientation(ST7789V_Object_t *pObj, uint32_t *Orientation);
int32_t ST7789V_GetXSize(ST7789V_Object_t *pObj, uint32_t *XSize);
int32_t ST7789V_GetYSize(ST7789V_Object_t *pObj, uint32_t *YSize);


#ifdef __cplusplus
}
#endif

#endif /* ST7789V_ST7789V_H */
