/*
 * Mem_MX25L_SPI.c
 *
 *  Created on: 17 juin 2019
 *  Original Author: j.proux
 *
 *  Updated on: 04 Oct. 2023
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "Mem_MX25L_SPI" dans un nouveau Projet :
 *   -> Suivre les indications dans le README de la lib MX25L_SPI
 *
 */

#include "stm32h5xx_hal.h"

#include "MX25L_SPI.h"
#include "spi.h"
#include "main.h"

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    #define MEM_MX25L_PERIF_INIT()  MX_SPI4_Init(); // MX_SPI5_Init() // SPI4 + SPI5
    #define MEM_MX25L_CS_INIT()     ; // L'init du CS est à faire dans l'Init correspondant ci-dessus

    // La structure pSPI_CS est à fournir comme paramètre dans chaque fonction appelée
    #define MEM_MX25L_PERIF_HANDLE  pSPI_CS->hSPI
    #define MEM_MX25L_CS_PORT       pSPI_CS->csPort
    #define MEM_MX25L_CS_PIN        pSPI_CS->csPin
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    // Paramètre unique (statique) sur SPI4 + CS sur PE15 :
    #define MEM_MX25L_PERIF_INIT()  MX_SPI3_Init()  // SPI3
    #define MEM_MX25L_CS_INIT()     MEM_MX25L_DEACTIVATE_CS()

    #define MEM_MX25L_PERIF_HANDLE  &hspi3
    #define MEM_MX25L_CS_PORT       Flash_SPI_CS_GPIO_Port	// PA15
    #define MEM_MX25L_CS_PIN        Flash_SPI_CS_Pin		// PA15
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE


#define MEM_MX25L_CS_ACTIVE         GPIO_PIN_RESET  // Active Low
#define MEM_MX25L_CS_INACTIVE       GPIO_PIN_SET

#define MEM_MX25L_SEND_TO           100U            // Send TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_RECV_TO           100U            // Receive TimeOut : 100 x 1ms = 100ms

#define MEM_MX25L_ACTIVATE_CS()     HAL_GPIO_WritePin(MEM_MX25L_CS_PORT, MEM_MX25L_CS_PIN, MEM_MX25L_CS_ACTIVE)
#define MEM_MX25L_DEACTIVATE_CS()   HAL_GPIO_WritePin(MEM_MX25L_CS_PORT, MEM_MX25L_CS_PIN, MEM_MX25L_CS_INACTIVE)

#define MEM_MX25L_PERIF_SEND_STREAM(TxBuf,BufSize) HAL_SPI_Transmit(MEM_MX25L_PERIF_HANDLE, TxBuf, BufSize, MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_RECV_STREAM(RxBuf,nb2Read) HAL_SPI_Receive( MEM_MX25L_PERIF_HANDLE, RxBuf, nb2Read, MEM_MX25L_RECV_TO)

// MX25L Read Commands :
#define MEM_MX25L_CMD_READ_STATUS_REGISTER  0x05 // RDSR : Read Status Register
#define MEM_MX25L_CMD_READ_CONFIG_REGISTER  0x15 // RCSR : Read Configuration Register
#define MEM_MX25L_CMD_READ_IDENTIFICATION   0x9F // RDID : Read Identification
#define MEM_MX25L_CMD_READ_SECURITY_REG     0x2B // RDSCUR : Read Security Register
#define MEM_MX25L_CMD_READ_DATA_BYTES       0x03 // READ : Normal Read
#define MEM_MX25L_CMD_FAST_READ_DATA_BYTES  0x0B // FAST_READ : Fast Read
#define MEM_MX25L_CMD_READ_ELECTRONIC_ID    0xAB // RES : Read Electronic ID
#define MEM_MX25L_CMD_READ_MFG_DEV_ID       0x90 // REMS : Read Electronic Manufacturer & device ID

// MX25L Write Commands :
#define MEM_MX25L_CMD_WRITE_ENABLE          0x06 // WREN : Write Enable
#define MEM_MX25L_CMD_WRITE_DISABLE         0x04 // WRDI : Write Disable
#define MEM_MX25L_CMD_WRITE_STATUS_CFG_REG  0x01 // WRSR : Write Status and Config Register
#define MEM_MX25L_CMD_WRITE_SECURITY_REG    0x2F // WRSCUR : Write Security Register
#define MEM_MX25L_CMD_SECTOR_ERASE_4K       0x20 // SE : Sector Erase (4K)
#define MEM_MX25L_CMD_BLOC_ERASE_64K        0xD8 // BE : Bloc Erase (64K)
#define MEM_MX25L_CMD_BLOC_ERASE_32K        0x52 // BE32K : Bloc Erase spécial 32K
#define MEM_MX25L_CMD_CHIP_ERASE            0x60 // CE : Chip Erase
#define MEM_MX25L_CMD_PAGE_PROGRAM          0x02 // PP -> Attention Page Boundary = 256 Bytes !

// MX25L Misc Commands :
#define MEM_MX25L_CMD_NO_OPERATION          0x00 // NOP : No Operation
#define MEM_MX25L_CMD_ENTER_DEEP_POWER      0xB9 // DP : Enter Deep Power Down Mode
#define MEM_MX25L_CMD_RELEASE_DEEP_POWER    0xAB // RDP : Release from Deep Power Down Mode
#define MEM_MX25L_CMD_PROGRAM_SUSPEND       0x75 // PGM Suspend : Program operation is interrupted by suspend command
#define MEM_MX25L_CMD_ERASE_SUSPEND         0xB0 // ERS Suspend : Erase operation is interrupted by suspend command
#define MEM_MX25L_CMD_PROGRAM_RESUME        0x7A // PGM Resume : Continue performing Program operation
#define MEM_MX25L_CMD_ERASE_RESUME          0x30 // ERS Resume : Continue performing Erase operation
#define MEM_MX25L_CMD_SET_BURST_LENGTH      0xC0 // SBL : Set Burst Length
#define MEM_MX25L_CMD_ENTER_SECURE_OTP      0xB1 // ENSO : Enter Secured OTP
#define MEM_MX25L_CMD_EXIT_SECURE_OTP       0xC1 // EXSO : Exit Secured OTP
#define MEM_MX25L_CMD_RESET_ENABLE          0x66 // RSTEN : Reset Enable
#define MEM_MX25L_CMD_DO_SOFT_RESET         0x99 // RST : Do Software Reset

#define MEM_MX25L_PERIF_NO_ERROR    0x00

#define MEM_MX25L_RETURN_SUCCESS    0x01
#define MEM_MX25L_RETURN_FAILURE    0x00

#define MEM_MX25L_GET_BYTE_N(value,N)   (((value)>>(8*(N))) & 0xFF)

#if defined(__DEBUG) || defined(DEBUG) || defined(DEBUG_MX25L_SPI)
	#define MX25L_SPI_HALT_IF_DEBUG()	__BKPT(0) // { while(1) ClrWdt(); }
//	#warning "DEBUG_MX25L_SPI is Active !!!"
#else // (! __DEBUG) && (! DEBUG_MX25L_XSPI) :
	#define MX25L_SPI_HALT_IF_DEBUG()	// Nop();
#endif // __DEBUG ; DEBUG_MX25L_SPI

//******************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

// Internal only usage functions :
#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    uint8_t Mem_MX25L_WriteEnable(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_WriteDisable(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_SendThisByte(SPI_CS_TypeDef *pSPI_CS, uint8_t byte2Send);
    uint8_t Mem_MX25L_SendArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send);
    uint8_t Mem_MX25L_SendReceiveArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send,
                                       uint16_t nbBytes2Read, void* pArray2Receive);
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    uint8_t Mem_MX25L_WriteEnable(void);
    uint8_t Mem_MX25L_WriteDisable(void);
    uint8_t Mem_MX25L_SendThisByte(uint8_t byte2Send);
    uint8_t Mem_MX25L_SendArray(void* pArray2Send, uint16_t nbBytes2Send);
    uint8_t Mem_MX25L_SendReceiveArray(void* pArray2Send, uint16_t nbBytes2Send,
                                       uint16_t nbBytes2Read, void* pArray2Receive);
    uint8_t Mem_MX25L_StartSendArraySequence(void* pArray2Send, uint16_t nbBytes2Send);
    uint8_t Mem_MX25L_ReceiveArrayInSequence(uint16_t nbBytes2Read, void* pArray2Receive);
    uint8_t Mem_MX25L_SendArrayInSequence(void* pArray2Send, uint16_t nbBytes2Send);
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

typedef union {
	uint8_t U8[4];
	uint16_t U16[2];
	uint32_t U32;
} tU8_16_32;

#define TestBuf_Size	4096
tU8_16_32 TestBuf[TestBuf_Size] = {0};

uint8_t tmpU24[3] = {0};
uint8_t tmpU8;

// Problem with NSSP Hrdware : https://community.st.com/t5/stm32-mcus-products/stm32-g4-spi-hardware-nss-with-nssp-diabled-does-not-work-only/td-p/127135

void Mem_MX25L_Init(void)
{ // Vérif_Jp = OK sur IS25LP le 21/06/2019
    MEM_MX25L_CS_INIT();
    MEM_MX25L_PERIF_INIT();

    MX25L_SPI_HALT_IF_DEBUG();

    tmpU8 = 25;
    uint8_t retVal = Mem_MX25L_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_ReadConfigRegister(&tmpU8);
	retVal = Mem_MX25L_ReadSecurityRegister(&tmpU8);

	retVal = Mem_MX25L_ReadIdRegister(tmpU24);

	retVal = Mem_MX25L_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_WriteEnable();	// WEL in StatusRegister
	retVal = Mem_MX25L_ReadStatusRegister(&tmpU8);

	retVal = Mem_MX25L_ReadStatusRegister(&tmpU8);
	retVal = Mem_MX25L_WriteDisable();	// WEL in StatusRegister
	retVal = Mem_MX25L_ReadStatusRegister(&tmpU8);

    MX25L_SPI_HALT_IF_DEBUG();

	// Test de Read Memory :
	uint32_t adr = 0; uint16_t nb2Read = TestBuf_Size * sizeof(tU8_16_32); uint16_t readMode = 0;
	uint32_t adrMax = 8UL * 1024UL * 1024UL; uint32_t nbU32Virgin = 0; uint32_t nbU32NotVirgin = 0; uint32_t nbU32AdrOK = 0;
	uint8_t curReadMode = 0;
	for(; adr < adrMax;)
	{
		switch(readMode)
		{
		case 1:
			retVal = Mem_MX25L_ReadDataBytes_HighSpeed(adr, nb2Read, TestBuf);
			break;
		default:
		case 0:
			retVal = Mem_MX25L_ReadDataBytes(adr, nb2Read, TestBuf);
			break;
		}
		if(MEM_MX25L_RETURN_SUCCESS == retVal)
		{
			for(int i = 0; i < (nb2Read/4); i++)
			{
				if(UINT32_MAX == TestBuf[i].U32)
				{
					if(1 != curReadMode)
					{
						curReadMode = 1;
						nbU32Virgin = 0;
					}
					nbU32Virgin++;
				} else if((adr + i) == TestBuf[i].U32)
				{
					if(2 != curReadMode)
					{
						curReadMode = 2;
						nbU32AdrOK = 0;
					}
					nbU32AdrOK++;
				} else {
					MX25L_SPI_HALT_IF_DEBUG();
					nbU32NotVirgin++;
				}
			}
		} else {
			MX25L_SPI_HALT_IF_DEBUG();
		}
		adr += nb2Read;
	}

	MX25L_SPI_HALT_IF_DEBUG();

	// Test de Erase Sector :
	uint32_t eraseStep = 4096;
	adr = 0; uint32_t nbU32Erased = 0; uint32_t nbU32NotErased = 0; uint16_t eraseMode = 0;
	for(; adr < adrMax;)
	{
		switch(eraseMode)
		{
		case 1:
			retVal = Mem_MX25L_BlockErase32K(adr);
			break;
		case 2:
			retVal = Mem_MX25L_BlockErase64K(adr);
			break;
		case 0:
		default:
			retVal = Mem_MX25L_SectorErase4K(adr);
			break;
		}
		if(MEM_MX25L_RETURN_SUCCESS == retVal)
		{
			nbU32Erased++;
		} else {
			MX25L_SPI_HALT_IF_DEBUG();
			nbU32NotErased++;
		}
		adr += eraseStep;
	}

	MX25L_SPI_HALT_IF_DEBUG();

	// Test du Chip Erase :
	retVal = Mem_MX25L_ChipErase();
	if(MEM_MX25L_RETURN_SUCCESS == retVal)
	{
		MX25L_SPI_HALT_IF_DEBUG();
//	} else if(MEM_MX25L_RETURN_BUSY == retVal)
//	{
//		retVal = Mem_MX25L_XSPI_ReadStatusRegister(&tmpU8);
//		retVal = Mem_MX25L_XSPI_IsWriteBusy();
//		retVal = Mem_MX25L_XSPI_Wait4WriteNotBusy(); // Attente Fin d'exécution
	} else {
		MX25L_SPI_HALT_IF_DEBUG();
	}

	MX25L_SPI_HALT_IF_DEBUG();

	// Test de Write Memory :
	uint16_t nb2Write = TestBuf_Size * sizeof(tU8_16_32); uint16_t writeMode = 0;
	adr = 0; uint32_t nbU32Written = 0; uint32_t nbU32NotWritten = 0;
	for(; adr < adrMax;)
	{
		for(int i = 0; i < (nb2Write/4); i++)
		{
			TestBuf[i].U32 = adr + i;
		}
		switch(writeMode)
		{
		case 1:
//			retVal = Mem_MX25L_XSPI_WriteArray_QuadWrite(adr, TestBuf, nb2Write);
//			break;
		default:
		case 0:
			retVal = Mem_MX25L_WriteArray(adr, TestBuf, nb2Write);
			break;
		}
		if(MEM_MX25L_RETURN_SUCCESS == retVal)
		{
			nbU32Written++;
		} else {
			MX25L_SPI_HALT_IF_DEBUG();
			nbU32NotWritten++;
		}
		adr += nb2Write;
	}

	MX25L_SPI_HALT_IF_DEBUG();

	retVal = Mem_MX25L_NoOperation();
	retVal = Mem_MX25L_IsWriteBusy();

    MX25L_SPI_HALT_IF_DEBUG();
    tmpU8 = 0;
}

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_ReadStatusRegister(SPI_CS_TypeDef *pSPI_CS, uint8_t *pStatusRegister)
{
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_STATUS_REGISTER };
    return Mem_MX25L_SendReceiveArray(pSPI_CS, TxCmd, sizeof(TxCmd), sizeof(uint8_t), pStatusRegister);
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_ReadStatusRegister(uint8_t *pStatusRegister)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_STATUS_REGISTER };
    return Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd), sizeof(uint8_t), pStatusRegister);
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_IsWriteBusy(SPI_CS_TypeDef *pSPI_CS)
{
    uint8_t tmpStatusReg;
    if(MEM_MX25L_RETURN_SUCCESS == Mem_MX25L_ReadStatusRegister(pSPI_CS, &tmpStatusReg))
    {
        return (tmpStatusReg & (1 << 0)) ? 1 : 0; // bit0 = WIP "Write in Progress"
    }
    return -1; // Error
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_IsWriteBusy()
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t tmpStatusReg = 0;	// Modif_Jp le 12/03/2020 pour correction CodeSonar "Uninitialized Variable"
    if(MEM_MX25L_RETURN_SUCCESS == Mem_MX25L_ReadStatusRegister(&tmpStatusReg))
    {
        return (tmpStatusReg & (1 << 0)) ? 1 : 0; // bit0 = WIP "Write in Progress"
    }
    return UINT8_MAX; // -1; // -1 = Error; Modif_Jp le 18/06/2020 pour correction CodeSonar "Coercion Alters Value"
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_Wait4WriteNotBusy(SPI_CS_TypeDef *pSPI_CS)
{
    while(1)
    {
        switch(Mem_MX25L_IsWriteBusy(pSPI_CS))
        {
            case 0:  return MEM_MX25L_RETURN_SUCCESS; // Not Busy => OK
            case (uint8_t)-1: return MEM_MX25L_RETURN_FAILURE; // Error => Failure
            default: break; // Continue polling !
        }
    }
    return MEM_MX25L_RETURN_FAILURE; // Au cas où !
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_Wait4WriteNotBusy()
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    while(1)
    {
        switch(Mem_MX25L_IsWriteBusy())
        {
            case 0:  return MEM_MX25L_RETURN_SUCCESS; // Not Busy => OK
            case (uint8_t)-1: return MEM_MX25L_RETURN_FAILURE; // Error => Failure
            default: break; // Continue polling !
        }
    }
    return MEM_MX25L_RETURN_FAILURE; // Au cas où !
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_ReadConfigRegister(SPI_CS_TypeDef *pSPI_CS, uint8_t *pConfigRegister)
{ // Non testé !
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_CONFIG_REGISTER };
    return Mem_MX25L_SendReceiveArray(pSPI_CS, TxCmd, sizeof(TxCmd), sizeof(uint8_t), pConfigRegister);
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_ReadConfigRegister(uint8_t *pConfigRegister)
{ // Non testé !
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_CONFIG_REGISTER };
    return Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd), sizeof(uint8_t), pConfigRegister);
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

uint8_t Mem_MX25L_WriteStatusConfigRegister(uint8_t newStatusRegister, uint8_t newConfigRegister)
{ // Non testé !
    uint8_t returnValue = Mem_MX25L_WriteEnable(); // Enable Write First !

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_WRITE_STATUS_CFG_REG, newStatusRegister, newConfigRegister };
        returnValue = Mem_MX25L_SendArray(TxCmd, sizeof(TxCmd));
    }

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_ReadIdRegister(void *pID_24bits)
{ // Vérif_Jp = OK sur IS25LP le 21/06/2019
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_IDENTIFICATION };
    return Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd), 3, pID_24bits); // 24 bits => 3 Bytes
}

//******************************************************************************

uint8_t Mem_MX25L_ReadSecurityRegister(uint8_t *pSecurityRegister)
{ // Non testé !
    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_SECURITY_REG };
    return Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd), sizeof(uint8_t), pSecurityRegister);
}

//******************************************************************************

uint8_t Mem_MX25L_ReadDataBytes(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if(0 != pReadBuf)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_DATA_BYTES,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0)};   // Byte 0 : bits [07:00]
        returnValue = Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd),
                                                 nbBytes2Read, pReadBuf);
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_StartReadArraySequence(uint32_t baseAdr_24bits)
{ // Vérif_Jp = OK sur IS25LP le 15/07/2019

    uint8_t TxCmd[] = { MEM_MX25L_CMD_READ_DATA_BYTES,
                        MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                        MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                        MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0)};   // Byte 0 : bits [07:00]
    return Mem_MX25L_StartSendArraySequence(TxCmd, sizeof(TxCmd));
}

//******************************************************************************

uint8_t Mem_MX25L_ReadArrayInSequence(uint16_t nbBytes2Read, void *pReadBuf)
{ // Vérif_Jp = OK sur IS25LP le 15/07/2019
    return Mem_MX25L_ReceiveArrayInSequence(nbBytes2Read, pReadBuf);
}

//******************************************************************************

uint8_t Mem_MX25L_NoOperation(void)
{ // Non testé !
    return Mem_MX25L_SendThisByte(MEM_MX25L_CMD_NO_OPERATION);
}

//******************************************************************************

uint8_t Mem_MX25L_ReadDataBytes_HighSpeed(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if(0 != pReadBuf)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_FAST_READ_DATA_BYTES,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0),    // Byte 0 : bits [07:00]
                            0 };                                        // Dummy Byte
        returnValue = Mem_MX25L_SendReceiveArray(TxCmd, sizeof(TxCmd),
                                                 nbBytes2Read, pReadBuf);
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_SectorErase4K(uint32_t baseAdr_24bits)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = Mem_MX25L_WriteEnable(); // Enable Write First !

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_SECTOR_ERASE_4K,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0) };  // Byte 0 : bits [07:00]
        returnValue = Mem_MX25L_SendArray(TxCmd, sizeof(TxCmd));
    }

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_BlockErase64K(uint32_t baseAdr_24bits)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = Mem_MX25L_WriteEnable(); // Enable Write First !

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_BLOC_ERASE_64K,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0) };  // Byte 0 : bits [07:00]
        returnValue = Mem_MX25L_SendArray(TxCmd, sizeof(TxCmd));
    }

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_BlockErase32K(uint32_t baseAdr_24bits)
{ // Non testé !
    uint8_t returnValue = Mem_MX25L_WriteEnable(); // Enable Write First !

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_BLOC_ERASE_32K,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0) };  // Byte 0 : bits [07:00]
        returnValue = Mem_MX25L_SendArray(TxCmd, sizeof(TxCmd));
    }

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_ChipErase(void)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = Mem_MX25L_WriteEnable(); // Enable Write First !

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_SendThisByte(MEM_MX25L_CMD_CHIP_ERASE);
    }

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_Wait4WriteNotBusy(); // Attente Fin d'exécution
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
#define MEM_MX25L_WRITE_PAGE_BOUNDARY   256 // Program Page = 256 bytes

    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Write) && (0 < nbBytes2Write) )
    {
        uint16_t thisBlocSize;
        uint8_t *pData = pArray2Write;
        uint8_t mayStop = 0;

        while(0 < nbBytes2Write)
        { // Calcule le Max autorisé en Ecriture à partir de cette Adresse :
            thisBlocSize = (MEM_MX25L_WRITE_PAGE_BOUNDARY) - (baseAdr_24bits & ((MEM_MX25L_WRITE_PAGE_BOUNDARY) -1));
            if(thisBlocSize > nbBytes2Write)    { thisBlocSize = nbBytes2Write; } // Ramène au nb de Bytes demandés / disponibles

            uint8_t TxCmd[] = { MEM_MX25L_CMD_PAGE_PROGRAM,
                                MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                                MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                                MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0) };  // Byte 0 : bits [07:00]

            if(MEM_MX25L_RETURN_SUCCESS != Mem_MX25L_WriteEnable()) { mayStop = 1; break; } // Arrêt immédiat, mais on peut faire le Break parce que CS n'est pas encore actif !

            MEM_MX25L_ACTIVATE_CS();

            if(MEM_MX25L_PERIF_NO_ERROR != MEM_MX25L_PERIF_SEND_STREAM(TxCmd, sizeof(TxCmd))) { mayStop = 1; } // Envoi Commande + Adresse 24 bits
            if(0 == mayStop)
            {
                if(MEM_MX25L_PERIF_NO_ERROR != MEM_MX25L_PERIF_SEND_STREAM(pData, thisBlocSize)) { mayStop = 1; } // Envoi DataBytes
            }

            MEM_MX25L_DEACTIVATE_CS();

            if(0 != mayStop) { break; } // S'il y a 1 erreur : on peut quitter ici (CS n'est plus actif) !

            if(MEM_MX25L_RETURN_SUCCESS != Mem_MX25L_Wait4WriteNotBusy()) { mayStop = 1; break; } // Attente Fin d'exécution

            // Wrtite is OK :
            baseAdr_24bits  += thisBlocSize;
            pData           += thisBlocSize;
            nbBytes2Write   -= thisBlocSize;
        }

        // Boucle terminée :
        if( (0 == nbBytes2Write) && (0 == mayStop)) { returnValue = MEM_MX25L_RETURN_SUCCESS; }
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_StartWriteArraySequence(uint32_t baseAdr_24bits)
{ // Non testé au 10/07/2019 !

    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;


    if(MEM_MX25L_RETURN_SUCCESS == Mem_MX25L_WriteEnable())
    {
        uint8_t TxCmd[] = { MEM_MX25L_CMD_PAGE_PROGRAM,
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 2),    // Byte 2 : bits [23:16]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 1),    // Byte 1 : bits [15:08]
                            MEM_MX25L_GET_BYTE_N(baseAdr_24bits, 0) };  // Byte 0 : bits [07:00]

        returnValue = Mem_MX25L_StartSendArraySequence(TxCmd, sizeof(TxCmd)); // Envoi Commande + Adresse 24 bits
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_WriteArrayInSequence(void* pArray2Write, uint16_t nbBytes2Write)
{ // ATTENTION : Non testé au 15/07/2019
    return MEM_MX25L_PERIF_SEND_STREAM(pArray2Write, nbBytes2Write); // Envoi DataBytes
}

//******************************************************************************

uint8_t Mem_MX25L_StopWriteArraySequence(void)
{ // ATTENTION : Non testé au 15/07/2019

       MEM_MX25L_DEACTIVATE_CS();
    return Mem_MX25L_Wait4WriteNotBusy();
}

//******************************************************************************

uint8_t Mem_MX25L_DoSoftwareReset(void)
{ // Non testé !
    uint8_t returnValue = Mem_MX25L_SendThisByte(MEM_MX25L_CMD_RESET_ENABLE);

    if(MEM_MX25L_RETURN_SUCCESS == returnValue)
    {
        returnValue = Mem_MX25L_SendThisByte(MEM_MX25L_CMD_DO_SOFT_RESET);
    }

    return returnValue;
}

//******************************************************************************
// Basic useful functions :

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_WriteEnable(SPI_CS_TypeDef *pSPI_CS)
{ // Non testé !
    return Mem_MX25L_SendThisByte(pSPI_CS, MEM_MX25L_CMD_WRITE_ENABLE);
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_WriteEnable(void)
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    return Mem_MX25L_SendThisByte(MEM_MX25L_CMD_WRITE_ENABLE);
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_WriteDisable(SPI_CS_TypeDef *pSPI_CS)
{ // Non testé !
    return Mem_MX25L_SendThisByte(pSPI_CS, MEM_MX25L_CMD_WRITE_DISABLE);
}
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_WriteDisable(void)
{ // Non testé !
    return Mem_MX25L_SendThisByte(MEM_MX25L_CMD_WRITE_DISABLE);
}
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

//******************************************************************************
// Low level functions :

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendThisByte(SPI_CS_TypeDef *pSPI_CS, uint8_t byte2Send)
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendThisByte(uint8_t byte2Send)
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    MEM_MX25L_ACTIVATE_CS();

    if(MEM_MX25L_PERIF_NO_ERROR == MEM_MX25L_PERIF_SEND_STREAM(&byte2Send, sizeof(byte2Send)))
    {
        returnValue = MEM_MX25L_RETURN_SUCCESS;
    }

    MEM_MX25L_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send)
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendArray(void* pArray2Send, uint16_t nbBytes2Send)
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Send) && (0 < nbBytes2Send) )
    {
        MEM_MX25L_ACTIVATE_CS();

        if(MEM_MX25L_PERIF_NO_ERROR == MEM_MX25L_PERIF_SEND_STREAM(pArray2Send, nbBytes2Send))
        {
            returnValue = MEM_MX25L_RETURN_SUCCESS;
        }

        MEM_MX25L_DEACTIVATE_CS();
    }

    return returnValue;
}

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendReceiveArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send,
                                       uint16_t nbBytes2Read, void* pArray2Receive)
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendReceiveArray(void* pArray2Send, uint16_t nbBytes2Send,
                                   uint16_t nbBytes2Read, void* pArray2Receive)
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
{ // Vérif_Jp = OK sur IS25LP le 24/06/2019
    uint8_t returnValue = MEM_MX25L_RETURN_SUCCESS;

    MEM_MX25L_ACTIVATE_CS();

    // Partie Send :
    if( (0 != pArray2Send) && (0 < nbBytes2Send) )
    {
        if(MEM_MX25L_PERIF_NO_ERROR != MEM_MX25L_PERIF_SEND_STREAM(pArray2Send, nbBytes2Send))
        {
            returnValue = MEM_MX25L_RETURN_FAILURE;
        }
    }

    // Partie Receive :
    if( (0 != pArray2Receive) && (0 < nbBytes2Read) && (MEM_MX25L_RETURN_SUCCESS == returnValue) )
    {
        if(MEM_MX25L_PERIF_NO_ERROR != MEM_MX25L_PERIF_RECV_STREAM(pArray2Receive, nbBytes2Read))
        {
            returnValue = MEM_MX25L_RETURN_FAILURE;
        }
    }

    MEM_MX25L_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_StartSendArraySequence(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send)
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_StartSendArraySequence(void* pArray2Send, uint16_t nbBytes2Send)
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
{ // Vérif_Jp = OK sur IS25LP le 15/07/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Send) && (0 < nbBytes2Send) )
    {
        MEM_MX25L_ACTIVATE_CS();

        if(MEM_MX25L_PERIF_NO_ERROR == MEM_MX25L_PERIF_SEND_STREAM(pArray2Send, nbBytes2Send))
        {
            returnValue = MEM_MX25L_RETURN_SUCCESS;
        }

// Ne pas Désactiver pour conserver la séquence ouverte !       MEM_MX25L_DEACTIVATE_CS();
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_ReceiveArrayInSequence(uint16_t nbBytes2Read, void* pArray2Receive)
{ // Vérif_Jp = OK sur IS25LP le 15/07/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Receive) && (0 < nbBytes2Read) )
    {
        if(MEM_MX25L_PERIF_NO_ERROR == MEM_MX25L_PERIF_RECV_STREAM(pArray2Receive, nbBytes2Read))
        {
            returnValue = MEM_MX25L_RETURN_SUCCESS;
        }
    }

// Ne pas Désactiver pour conserver la séquence ouverte !      MEM_MX25L_DEACTIVATE_CS();

    return returnValue;
}

//******************************************************************************

#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendArrayInSequence(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send)
#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
uint8_t Mem_MX25L_SendArrayInSequence(void* pArray2Send, uint16_t nbBytes2Send)
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
{ // ATTENTION : Non testé au 10/07/2019
    uint8_t returnValue = MEM_MX25L_RETURN_FAILURE;

    if( (0 != pArray2Send) && (0 < nbBytes2Send) )
    {
// Ne pas activer pour modifier la séquence ouverte !      MEM_MX25L_ACTIVATE_CS();

        if(MEM_MX25L_PERIF_NO_ERROR == MEM_MX25L_PERIF_SEND_STREAM(pArray2Send, nbBytes2Send))
        {
            returnValue = MEM_MX25L_RETURN_SUCCESS;
        }

// Ne pas Désactiver pour conserver la séquence ouverte !      MEM_MX25L_DEACTIVATE_CS();
    }

    return returnValue;
}

//******************************************************************************

uint8_t Mem_MX25L_StopReadArraySequence(void)
{ // Vérif_Jp = OK sur IS25LP le 15/07/2019

    MEM_MX25L_DEACTIVATE_CS();

    return MEM_MX25L_RETURN_SUCCESS;
}

#ifdef __cplusplus
}
#endif
