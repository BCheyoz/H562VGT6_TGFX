/*
 *  ST7789.h
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#include <ST7789.h>

typedef struct
{
	uint32_t        Width;
	uint32_t        Height;
	uint32_t        Orientation;
} ST7789_Ctx_t;

/* The below table handle the different values to be set to Memory Data Access Control
   depending on the orientation and pbm image writing where the data order is inverted
 */
static uint32_t OrientationTab[4] =
{
		0x00U , /* Portrait orientation choice of LCD screen               */
		0xC0U , /* Portrait rotated 180° orientation choice of LCD screen  */
		0xA0U , /* Landscape orientation choice of LCD screen              */
		0x60U , /* Landscape rotated 180° orientation choice of LCD screen */
};

static ST7789_Ctx_t ST7789Ctx;

/*** Private Prototypes Functions ***************************************************************/

static int32_t ST7789_write_reg(ST7789_Object_t *pObj, volatile uint8_t *Reg, uint32_t Length);
static int32_t ST7789_send_data(ST7789_Object_t *pObj, uint8_t *pData, uint32_t Length);
static int32_t ST7789_recv_data(ST7789_Object_t *pObj, uint8_t *pData, uint32_t Length);
static int32_t ST7789_Delay(ST7789_Object_t *pObj, uint32_t Delay);


/*************************************************************************************************/

/**
 * @brief  Initialize the ST7789 LCD Component.
 * @param  pObj Component object
 * @param  pInitParams Initialization Parameters
 * @retval Component status
 */
int32_t ST7789_Init(ST7789_Object_t *pObj, ST7789_InitParams_t* pInitParams)
{
	int32_t ret = ST7789_OK;
	uint8_t volatile parameter[15];
	uint8_t volatile reg;
	uint8_t i;

	if((pObj == NULL) || (pInitParams->Orientation > ST7789_ORIENTATION_LANDSCAPE_ROT180))
	{
		ret = ST7789_ERROR;
	}
	else if (pObj->IsInitialized == 0)
	{
		/* Sleep In Command */
		reg = ST7789_SPLIN;
		ret += ST7789_write_reg(pObj, &reg, 1);
		/* Wait for 10ms */
		ST7789_Delay(pObj, 10);

		/* SW Reset Command */
		reg = ST7789_SWRESET;
		ret += ST7789_write_reg(pObj, &reg, 1);
		/* Wait for 200ms */
		ST7789_Delay(pObj, 200);

		/* Sleep Out Command */
		reg = ST7789_SLEEP_OUT;
		ret += ST7789_write_reg(pObj, &reg, 1);
		/* Wait for 120ms */
		ST7789_Delay(pObj, 120);

		/* Display Normal mode Command */
		reg = ST7789_NORMAL_MODE_ON;
		ret += ST7789_write_reg(pObj, &reg, 1);
		/* Wait for 100ms */
		ST7789_Delay(pObj, 100);

		/* RAM Control Command */
		reg = ST7789_RAM_CTRL;
		parameter[0] = 0x00;
		if(pInitParams->Endian == ST7789_ENDIAN_LITTLE)
		{
			parameter[1] = 0xF8;
		}
		else
		{
			parameter[1] = 0xF0;
		}
		ret += ST7789_write_reg(pObj, &reg, 1);
		ret += ST7789_send_data(pObj, (uint8_t *)parameter, 2);

		if(pInitParams->SwapRB)
		{
			for(i = 0; i < 4; i++)
			{
				OrientationTab[i] |= 0x08;
			}

			/* LCM Control Command */
			reg = ST7789_LCM_CTRL;
			parameter[0] = 0x0C;
			ret += ST7789_write_reg(pObj, &reg, 1);
			ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);
		}

		/* Set Display Orientation */
		ret += ST7789_SetOrientation(pObj, pInitParams->Orientation);

		/* Set Pixel Format */
		reg = ST7789_PIXEL_FORMAT;
		parameter[0] = pInitParams->ColorCoding;
		ret += ST7789_write_reg(pObj, &reg, 1);
		ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);

		/* Invert Color Command */
		if(pInitParams->InvertColor){
			reg = ST7789_DINVON;
			ret += ST7789_write_reg(pObj, &reg, 1);
		}

		/* Frame Rate Control in normal mode */
		reg = ST7789_FRAME_RATE_CTRL2;
		parameter[0] = pInitParams->FrameRate; /* Default is 60 Hz */
		ret += ST7789_write_reg(pObj, &reg, 1);
		ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);

		/* PORCH control setting */
		reg = ST7789_PORCH_CTRL;
		parameter[0] = 0x0C;
		parameter[1] = 0x0C;
		parameter[2] = 0x00;
		parameter[3] = 0x33;
		parameter[4] = 0x33;
		ret += ST7789_write_reg(pObj, &reg, 1);
		ret += ST7789_send_data(pObj, (uint8_t *)parameter, 5);

		/* GATE control setting */
		reg = ST7789_GATE_CTRL;
		parameter[0] = 0x35;
		ret += ST7789_write_reg(pObj, &reg, 1);
		ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);

		if(pInitParams->TEMode == ST7789_TE_MODE_1)
		{
			// Set TE scan line
			reg = ST7789_SET_TEAR_SCANLINE;
			ret += ST7789_write_reg(pObj, &reg, 1);
			parameter[0] = (pInitParams->TEScanline & 0x100);
			parameter[1] = (pInitParams->TEScanline & 0x0FF);
			ret += ST7789_send_data(pObj, (uint8_t *)parameter, 2);

			reg = ST7789_TEON;
			parameter[0] = 0x00; /* VSYNC Only */
			ret += ST7789_write_reg(pObj, &reg, 1);
			ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);
		}
		else if(pInitParams->TEMode == ST7789_TE_MODE_2)
		{
			// Reset TE scan line
			reg = ST7789_SET_TEAR_SCANLINE;
			ret += ST7789_write_reg(pObj, &reg, 1);
			parameter[0] = 0;
			parameter[1] = 0;
			ret += ST7789_send_data(pObj, (uint8_t *)parameter, 2);

			// Enable TE
			reg = ST7789_TEON;
			parameter[0] = 0x01; /* VSYNC + HSYNC */
			ret += ST7789_write_reg(pObj, &reg, 1);
			ret += ST7789_send_data(pObj, (uint8_t *)parameter, 1);
		}

		pObj->IsInitialized = 1U;
	}

	if(ret != ST7789_OK)
	{
		ret = ST7789_ERROR;
	}

	return ret;
}

/**
 * @brief  De-Initialize the ST7789 LCD Component.
 * @param  pObj Component object
 * @retval Component status
 */
int32_t ST7789_DeInit(ST7789_Object_t *pObj)
{
	return pObj->IO.DeInit();
}

/**
 * @brief  Get the ST7789 ID.
 * @param  pObj Component object
 * @param  Id Component ID
 * @retval The component status
 */
int32_t ST7789_ReadID(ST7789_Object_t *pObj, uint32_t *Id)
{
	int32_t ret;
	uint8_t volatile data[4] = {ST7789_READ_DISPLAY_ID, 0, 0, 0 };

	if(ST7789_recv_data(pObj, (uint8_t *)data, 4) != ST7789_OK)
	{
		ret = ST7789_ERROR;
	}
	else
	{
#if __CORTEX_M == 0x07
		if((SCB->CCR) & (uint32_t)SCB_CCR_DC_Msk)
		{
			SCB_CleanInvalidateDCache();
		}
#endif
		*Id = ((*(uint8_t *)&data[1] << 16) | (*(uint8_t *)&data[2] << 8) | *(uint8_t *)&data[3]);
		if(*Id != ST7789_ID)
		{
			data[0] <<= 1;
			if(data[1] & 0x80)
				data[0] |= 1;
			data[1] <<= 1;
			if(data[2] & 0x80)
				data[1] |= 1;
			data[2] <<= 1;
			if(data[3] & 0x80)
				data[2] |= 1;

			*Id = ((*(uint8_t *)&data[0] << 16) | (*(uint8_t *)&data[1] << 8) | *(uint8_t *)&data[2]);
		}
		ret = ST7789_OK;
	}

	return ret;
}

/**
 * @brief  Enables the Display.
 * @param  pObj Component object
 * @retval The component status
 */
int32_t ST7789_DisplayOn(ST7789_Object_t *pObj)
{
	uint8_t volatile reg = ST7789_DISPLAY_ON;

	return ST7789_write_reg(pObj, &reg, 1);
}

/**
 * @brief  Disables the Display.
 * @param  pObj Component object
 * @retval The component status
 */
int32_t ST7789_DisplayOff(ST7789_Object_t *pObj)
{
	uint8_t volatile reg = ST7789_DISPLAY_OFF;

	return ST7789_write_reg(pObj, &reg, 1);
}

/**
 * @brief  Set the Display Orientation.
 * @param  pObj Component object
 * @param  Orientation ST7789_ORIENTATION_PORTRAIT, ST7789_ORIENTATION_PORTRAIT_ROT180
 *                     ST7789_ORIENTATION_LANDSCAPE or ST7789_ORIENTATION_LANDSCAPE_ROT180
 * @retval The component status
 */
int32_t ST7789_SetOrientation(ST7789_Object_t *pObj, uint32_t Orientation)
{
	int32_t ret;
	uint8_t volatile reg;
	uint8_t volatile parameter;

	parameter = (uint8_t)OrientationTab[Orientation];
	reg = ST7789_MAC;
	ret = ST7789_write_reg(pObj, &reg, 1);
	ret += ST7789_send_data(pObj, (uint8_t *)&parameter, 1);

	if(ret == ST7789_OK)
	{
		ST7789Ctx.Orientation = Orientation;

		switch (Orientation)
		{
		case ST7789_ORIENTATION_LANDSCAPE:
		case ST7789_ORIENTATION_LANDSCAPE_ROT180:
			ST7789Ctx.Width  = pObj->YSize;
			ST7789Ctx.Height = pObj->XSize;
			break;
		default:
		case ST7789_ORIENTATION_PORTRAIT_ROT180:
		case ST7789_ORIENTATION_PORTRAIT:
			ST7789Ctx.Width  = pObj->XSize;
			ST7789Ctx.Height = pObj->YSize;
			break;
		}

		pObj->XSize = ST7789Ctx.Width;
		pObj->YSize = ST7789Ctx.Height;
	}
	else
	{
		ret = ST7789_ERROR;
	}

	return ret;
}

/**
 * @brief  Set the Display Orientation.
 * @param  pObj Component object
 * @param  Orientation pointer to the LCD orientation
 * @retval The component status
 */
int32_t ST7789_GetOrientation(ST7789_Object_t *pObj, uint32_t *Orientation)
{
	(void)pObj;

	*Orientation = ST7789Ctx.Orientation;

	return ST7789_OK;
}

/**
 * @brief  Get the LCD pixel Width.
 * @param  pObj Component object
 * @retval The Lcd Pixel Width
 */
int32_t ST7789_GetXSize(ST7789_Object_t *pObj, uint32_t *XSize)
{
	(void)pObj;

	*XSize = ST7789Ctx.Width;

	return ST7789_OK;
}

/**
 * @brief  Get the LCD pixel Height.
 * @param  pObj Component object
 * @retval The Lcd Pixel Height
 */
int32_t ST7789_GetYSize(ST7789_Object_t *pObj, uint32_t *YSize)
{
	(void)pObj;

	*YSize = ST7789Ctx.Height;

	return ST7789_OK;
}

/**
 * @brief  Sets a display window
 * @param  Xpos   specifies the X bottom left position.
 * @param  Ypos   specifies the Y bottom left position.
 * @param  Height display window height.
 * @param  Width  display window width.
 * @retval Component status
 */
int32_t ST7789_SetDisplayWindow(ST7789_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
	int32_t ret;
	uint8_t volatile parameter[4];
	uint8_t volatile reg = ST7789_CASET;

	/* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width - 1) */
	ret = ST7789_write_reg(pObj, &reg, 1);
	parameter [0] = (uint8_t)(Xpos >> 8U);
	parameter [1] = (uint8_t)(Xpos & 0xFFU);
	parameter [2] = (uint8_t)((Xpos + Width - 1U) >> 8U);
	parameter [3] = (uint8_t)((Xpos + Width - 1U) & 0xFFU);
	ret += ST7789_send_data(pObj, (uint8_t *)parameter, 4);

	/* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1) */
	reg = ST7789_RASET;
	ret += ST7789_write_reg(pObj, &reg, 1);
	parameter [0] = (uint8_t)(Ypos >> 8U);
	parameter [1] = (uint8_t)(Ypos & 0xFFU);
	parameter [2] = (uint8_t)((Ypos + Height - 1U) >> 8U);
	parameter [3] = (uint8_t)((Ypos + Height - 1U) & 0xFFU);
	ret += ST7789_send_data(pObj, (uint8_t *)parameter, 4);

	reg = ST7789_GRAM;
	ret += ST7789_write_reg(pObj, &reg, 1);

	if(ret != ST7789_OK)
	{
		ret = ST7789_ERROR;
	}

	return ret;
}


static int32_t ST7789_write_reg(ST7789_Object_t *pObj, volatile uint8_t *Reg, uint32_t Length){
	if(pObj == NULL || pObj->IO.WriteReg == NULL){
		return ST7789_ERROR;
	}

	return pObj->IO.WriteReg(Reg, Length);
}

static int32_t ST7789_send_data(ST7789_Object_t *pObj, uint8_t *pData, uint32_t Length){
	if(pObj == NULL || pObj->IO.SendData == NULL){
		return ST7789_ERROR;
	}

	return pObj->IO.SendData(pData, Length);
}

static int32_t ST7789_recv_data(ST7789_Object_t *pObj, uint8_t *pData, uint32_t Length){
	if(pObj == NULL || pObj->IO.RecvData == NULL){
		return ST7789_ERROR;
	}

	return pObj->IO.RecvData(pData, Length);
}

static int32_t ST7789_Delay(ST7789_Object_t *pObj, uint32_t Delay){
	uint32_t tickstart;
	if(pObj == NULL || pObj->IO.GetTick == NULL){
		return ST7789_ERROR;
	}

	tickstart = pObj->IO.GetTick();
	while((pObj->IO.GetTick() - tickstart) < Delay)
	{
	}
	return ST7789_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
