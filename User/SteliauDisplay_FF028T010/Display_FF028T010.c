/*
 * Display_FF028T010.c
 *
 *  Created on: 27 mai 2025
 *  Original Author: b.chhay
 *
 *
 *  Version 1.0
 */

#include "Display_FF028T010.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_RST_LOW()		WRITE_REG(LCD_RESET_GPIO_PORT->BRR, LCD_RESET_GPIO_PIN)
#define LCD_RST_HIGH()		WRITE_REG(LCD_RESET_GPIO_PORT->BSRR, LCD_RESET_GPIO_PIN)

#define LCD_CS_LOW()		WRITE_REG(LCD_CS_GPIO_PORT->BRR, LCD_CS_GPIO_PIN)
#define LCD_CS_HIGH()		WRITE_REG(LCD_CS_GPIO_PORT->BSRR, LCD_CS_GPIO_PIN)

#define LCD_DC_LOW()		WRITE_REG(LCD_DCX_GPIO_PORT->BSRR, LCD_DCX_GPIO_PIN)
#define LCD_DC_HIGH()		WRITE_REG(LCD_DCX_GPIO_PORT->BRR, LCD_DCX_GPIO_PIN)

/*** Variables globals ****************************************************************/
static ST7789V_IO_t     IOCtx = { 0 };
static ST7789V_Object_t ObjCtx = { 0 };
static int32_t display_status = BSP_ERROR_NONE;
static uint8_t DisplayInit = 0;
static volatile uint8_t displayLock = 0;

/*** Prototypes privées ***************************************************************/
static int32_t LCD_IO_GetTick(void);
static int32_t LCD_IO_Delay(uint32_t Delay);
static int32_t LCD_IO_Init(void);
static int32_t LCD_IO_DeInit(void);
static int32_t LCD_IO_WriteReg(volatile uint8_t *Reg, uint32_t Length);
static int32_t LCD_IO_ReadReg(volatile uint8_t *Reg, uint32_t Length);
static int32_t LCD_IO_SendData(uint8_t *pData, uint32_t Length);
static int32_t LCD_IO_RecvData(uint8_t *pData, uint32_t Length);
uint8_t LCD_TryLock(uint32_t Timeout);
uint8_t LCD_Unlock();

void Display_FF028T010_Init(){
	int32_t ret = BSP_ERROR_NONE;
	ST7789V_InitParams_t ST7789V_InitParams;
	uint32_t UserBaudRatePrescaler = 0;
	uint32_t id = 0;

	if(LCD_TryLock(LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
		return;
	}

	// Active le backlight à 100%
	__HAL_TIM_SET_COMPARE(LCD_BACKLIGHT_HANDLE, LCD_BACKLIGHT_CHANNEL_ID, 100);
	HAL_TIM_PWM_Start(LCD_BACKLIGHT_HANDLE, LCD_BACKLIGHT_CHANNEL_ID);

	/* Configure le driver ST7789 pour utiliser les requetes SPI*/
	IOCtx.Init             = LCD_IO_Init;
	IOCtx.DeInit           = LCD_IO_DeInit;
	IOCtx.ReadReg          = LCD_IO_ReadReg;
	IOCtx.WriteReg         = LCD_IO_WriteReg;
	IOCtx.SendData         = LCD_IO_SendData;
	IOCtx.SendDataDMA      = NULL;
	IOCtx.RecvData         = LCD_IO_RecvData;
	IOCtx.RecvDataDMA      = NULL;
	IOCtx.GetTick          = LCD_IO_GetTick;
	IOCtx.Delay            = LCD_IO_Delay;

	ObjCtx.IO = IOCtx;

	if(LCD_IO_Init() != BSP_ERROR_NONE){ // init la SPI
		ret = BSP_ERROR_UNKNOWN_COMPONENT;
	}
	else {
		UserBaudRatePrescaler = hLCDSPI.Init.BaudRatePrescaler;
		hLCDSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; // Ralenti au maximum la clock pour etre sur de lire l'ID
		if (HAL_SPI_Init(&hLCDSPI) != HAL_OK){
			ret = BSP_ERROR_BUS_FAILURE;
		}

		if(ret == BSP_ERROR_NONE)
		{
			if((ST7789V_ReadID(&ObjCtx, &id) == ST7789V_OK) && (id == ST7789V_ID))
			{
				hLCDSPI.Init.BaudRatePrescaler = UserBaudRatePrescaler;
				if (HAL_SPI_Init(&hLCDSPI) != HAL_OK){
					ret = BSP_ERROR_BUS_FAILURE;
				}

				if(ret == BSP_ERROR_NONE){
					/* LCD Initialization */
					ObjCtx.IsInitialized = 0;

					ST7789V_InitParams.Endian         = ST7789V_ENDIAN_BIG;
					ST7789V_InitParams.SwapRB         = 0;
					ST7789V_InitParams.InvertColor    = 1;
					ST7789V_InitParams.ColorCoding    = LCD_COLOR_FORMAT;
					ST7789V_InitParams.Orientation    = LCD_ORIENTATION;
					ST7789V_InitParams.FrameRate      = ST7789V_60_Hz;
					ST7789V_InitParams.TEScanline     = 0;
					ST7789V_InitParams.TEMode         = ST7789V_TE_DISABLED;
					ST7789V_InitParams.Timings.hsync  = ST7789V_HSYNC;
					ST7789V_InitParams.Timings.hbp    = ST7789V_HBP;
					ST7789V_InitParams.Timings.hfp    = ST7789V_HFP;
					ST7789V_InitParams.Timings.vsync  = ST7789V_VSYNC;
					ST7789V_InitParams.Timings.vbp    = ST7789V_VBP;
					ST7789V_InitParams.Timings.vfp    = ST7789V_VFP;
					if(ST7789V_Init(&ObjCtx, &ST7789V_InitParams) != ST7789V_OK){
						ret = BSP_ERROR_COMPONENT_FAILURE;
					}
				}
			}
			else
			{
				ret = BSP_ERROR_UNKNOWN_COMPONENT;
			}
		}
	}

	LCD_Unlock();

	if(ret == BSP_ERROR_NONE){
		ret = BSP_LCD_DisplayOn();
	}

	display_status = ret;
}

/**
 * @brief  De-Initializes the LCD resources.

 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_DeInit()
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_IO_DeInit < 0)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	else
	{
		ret = BSP_ERROR_NONE;
	}


	return ret;
}

/**
 * @brief  Sets the LCD Orientation.

 * @param  Orientation:  New LCD Orientation.
 *         This parameter can be one of the following values:
 *          @arg @ref LCD_ORIENTATION_PORTRAIT           Portrait orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_LANDSCAPE          Landscape orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_PORTRAIT_ROT180    Portrait rotated 180° orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_LANDSCAPE_ROT180   Landscape rotated 180° orientation choice of LCD screen.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_SetOrientation(uint32_t Orientation)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if(ST7789V_SetOrientation(&ObjCtx, Orientation) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Gets the LCD Orientation.

 * @param  pOrientation: Pointer to Current LCD Orientation.
 *         This parameter will be set to one of the following values:
 *          @arg @ref LCD_ORIENTATION_PORTRAIT           Portrait orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_LANDSCAPE          Landscape orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_PORTRAIT_ROT180    Portrait rotated 180° orientation choice of LCD screen.
 *          @arg @ref LCD_ORIENTATION_LANDSCAPE_ROT180   Landscape rotated 180° orientation choice of LCD screen.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_GetOrientation(uint32_t *pOrientation)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{

		if(ST7789V_GetOrientation(&ObjCtx, pOrientation) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}


		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Gets the LCD X size.

 * @param  pXSize:       Pointer to Used LCD X size.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_GetXSize(uint32_t *pXSize)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if(ST7789V_GetXSize(&ObjCtx, pXSize) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}


		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Gets the LCD Y size.

 * @param  pYSize:       Pointer to Used LCD Y size.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_GetYSize(uint32_t *pYSize)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if(ST7789V_GetYSize(&ObjCtx, pYSize) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Switch On the display.

 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_DisplayOn()
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if(ST7789V_DisplayOn(&ObjCtx) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Switch Off the display.

 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_DisplayOff()
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if(ST7789V_DisplayOff(&ObjCtx) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Send data to the selected the LCD GRAM.

 * @param  pData:        Pointer to data to write to LCD GRAM.
 * @param  Length:       Length of data to write to LCD GRAM.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_WriteData(uint8_t *pData, uint32_t Length)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		/* Set the SPI in 16-bit mode to match endianess */
		hLCDSPI.Init.DataSize = SPI_DATASIZE_16BIT;
		HAL_SPI_Init(&hLCDSPI);
		if(LCD_IO_SendData(pData, (Length / 2)) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		/* Go back to 8-bit mode */
		hLCDSPI.Init.DataSize = SPI_DATASIZE_8BIT;
		HAL_SPI_Init(&hLCDSPI);

		LCD_Unlock();
	}

	return ret;
}

/**
 * @brief  Sets a display window.

 * @param  Xpos:         Specifies the X position.
 * @param  Ypos:         Specifies the Y position.
 * @param  Height:       Specifies the height of the rectangle to fill.
 * @param  Width:        Specifies the width of the rectangle to fill.
 * @retval int32_t:      BSP status.
 */
int32_t BSP_LCD_SetDisplayWindow(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
	int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

	if(LCD_TryLock( LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
	{
		ret = BSP_ERROR_BUSY;
	}
	else
	{
		if (ST7789V_SetDisplayWindow(&ObjCtx, Xpos, Ypos, Width, Height) < 0)
		{
			ret = BSP_ERROR_COMPONENT_FAILURE;
		}
		else
		{
			ret = BSP_ERROR_NONE;
		}

		LCD_Unlock();
	}

	return ret;
}


/**
 * @brief  Get the status of the LCD Transfer.

 * @retval uint8_t:      Zero if no Transfer, Transfer Operation code otherwise.
 */
uint8_t BSP_LCD_GetTransferStatus()
{
	return displayLock;
}

uint8_t BSP_LCD_GetPixelDepth(){
	uint8_t depth;
	switch(LCD_COLOR_FORMAT){
	case ST7789V_FORMAT_RBG565:
		depth = 2;
		break;
	case ST7789V_FORMAT_RBG666:
		depth = 3;
		break;
	default :
		depth = 1;
	}

	return depth;
}

/**
 * @brief  Wait for until complete LCD Transfer.

 */
void BSP_LCD_WaitForTransferToBeDone()
{
	while(displayLock == 1){
		HAL_Delay(1);
	}
}


/**
 * @brief  Provide a tick value in millisecond.
 * @retval int32_t:  Tick value.
 */
static int32_t LCD_IO_GetTick(void)
{
	uint32_t ret;
	ret = HAL_GetTick();
	return (int32_t)ret;
}

/**
 * @brief  LCD IO delay
 * @param  Delay:    Delay in ms
 * @retval int32_t:  Error status
 */
static int32_t LCD_IO_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
	return BSP_ERROR_NONE;
}

static int32_t LCD_IO_Init(void)
{
	int32_t ret = BSP_ERROR_NONE;

	// reset SW
	LCD_RST_LOW();
	HAL_Delay(100);
	LCD_RST_HIGH();
	HAL_Delay(100);

	// Mise en attente de l'afficheur (active low)
	LCD_CS_HIGH();
	LCD_DC_HIGH();

	if(DisplayInit++ == 0){
		if (HAL_SPI_GetState(&hLCDSPI) == HAL_SPI_STATE_RESET){
			HAL_SPI_MspInit(&hLCDSPI);
			/* Dans la demo la fonction est redéfinie en interne mais s'il y a une erreur
			 * il retourne une erreur au lieu de partir dans la fonction Error_Handler
			 * A faire de même ? */
			LCD_SPI_INIT();
		}
	}

	if(ret == BSP_ERROR_NONE){
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
		/* Register SPI Tx Complete Callback */
		HAL_SPI_RegisterCallback(&hLCDSPI, HAL_SPI_TX_COMPLETE_CB_ID, SPI_TxCpltCallback);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
	}
	return ret;
}

static int32_t LCD_IO_DeInit(void)
{
	int32_t ret = BSP_ERROR_BUS_FAILURE;

	if(DisplayInit > 0){
		HAL_SPI_MspDeInit(&hLCDSPI);
		if (HAL_SPI_DeInit(&hLCDSPI) == HAL_OK)
		{
			ret = BSP_ERROR_NONE;
		}
	}

	return ret;
}

/**
 * @brief  Writes register on LCD register.
 * @param  Reg    Register to be written
 * @param  Length length of data be read from the LCD GRAM
 * @retval BSP status
 */
static int32_t LCD_IO_WriteReg(volatile uint8_t *Reg, uint32_t Length)
{
	int32_t ret = BSP_ERROR_NONE;

	LCD_CS_LOW();
	LCD_DC_HIGH();

	if(HAL_SPI_Transmit(&hLCDSPI, (uint8_t *)Reg, Length, LCD_SPI_POLL_TIMEOUT) != HAL_OK){
		ret = BSP_ERROR_UNKNOWN_FAILURE;
	}

	LCD_DC_LOW();
	LCD_CS_HIGH();

	return ret;
}

/**
 * @brief  Read register on LCD register.
 * @param  Reg    Register to be read
 * @param  Length length of data be read from the LCD GRAM
 * @retval BSP status
 */
static int32_t LCD_IO_ReadReg(volatile uint8_t *Reg, uint32_t Length)
{
	/* USER CODE BEGIN LCD_IO_ReadReg */

	return BSP_ERROR_FEATURE_NOT_SUPPORTED;

	/* USER CODE END LCD_IO_ReadReg */
}

/**
 * @brief  Send data to select the LCD GRAM.
 * @param  pData  pointer to data to write to LCD GRAM.
 * @param  Length length of data to write to LCD GRAM
 * @retval Error status
 */
static int32_t LCD_IO_SendData(uint8_t *pData, uint32_t Length)
{
	int32_t ret = BSP_ERROR_NONE;

	LCD_CS_LOW();
	LCD_DC_LOW();

	if(HAL_SPI_Transmit(&hLCDSPI, pData, Length, LCD_SPI_POLL_TIMEOUT) != HAL_OK){
		ret = BSP_ERROR_UNKNOWN_FAILURE;
	}

	LCD_CS_HIGH();

	return ret;
}

/**
 * @brief  Receive data from selected LCD GRAM.
 * @param  pData  pointer to data to read to from selected LCD GRAM.
 * @param  Length length of data to read from selected LCD GRAM
 * @retval Error status
 */
static int32_t LCD_IO_RecvData(uint8_t *pData, uint32_t Length)
{
	int32_t ret = BSP_ERROR_NONE;

	LCD_CS_LOW();
	LCD_DC_HIGH();

	/* Write Register to read */
	if(HAL_SPI_Transmit(&hLCDSPI, (uint8_t *)pData, 1, LCD_SPI_POLL_TIMEOUT) != HAL_OK){
		ret = BSP_ERROR_UNKNOWN_FAILURE;
	}

	LCD_DC_LOW();

	/* Receive the Data */
	if(HAL_SPI_Receive(&hLCDSPI, (uint8_t *)pData, Length, LCD_SPI_POLL_TIMEOUT) != HAL_OK){
		ret = BSP_ERROR_UNKNOWN_FAILURE;
	}

	LCD_CS_HIGH();

	return ret;
}

uint8_t LCD_TryLock(uint32_t Timeout){
	uint32_t tickstart = HAL_GetTick();
	if(displayLock == 0)
	{
		displayLock = 1;
		return LCD_OS_ERROR_NONE;
	}
	else if(Timeout == 0)
	{
		return LCD_OS_ERROR_BUSY;
	}
	else
	{
		do
		{
			if(displayLock == 0)
			{
				displayLock = 1;
				return LCD_OS_ERROR_NONE;
			}
		} while ((HAL_GetTick() - tickstart) < Timeout);
		return LCD_OS_ERROR_BUSY;
	}
}

uint8_t LCD_Unlock(){
	displayLock = 0;
	return LCD_OS_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif
