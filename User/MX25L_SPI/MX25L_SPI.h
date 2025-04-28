/*
 * Mem_MX25L_SPI.h
 *
 *  Created on: 17 juin 2019
 *  Original Author: j.proux
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 14/01/2020 : Added by AA to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
 *-> 14/06/2021 : Added by BC to CarteAqui_BestCC (STM32F732VE : innoprojects/carte-acquisition/banc-de-test-statique/best-cc)
 *-> 04/08/2023 : Added by AB to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 10/01/2024 : Added by Jp to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *
 */

#ifndef MEM_MX25L_SPI_MEM_MX25L_SPI_H_
#define MEM_MX25L_SPI_MEM_MX25L_SPI_H_

#ifndef USE_HAL_DRIVER
    #error "SPI Memory MX25L Error : HAL Driver Required !"
#else // HAL_DRIVER :

#endif // HAL_DRIVER :


#define OCTOSPI
//#define SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE // Activer uniquement si plusieurs Mémoires à gérer en SPI !


#include <stdint.h>
#ifndef OCTOSPI
#include "spi.h"
#else
#include "octospi.h"
#endif


#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    #define MEM_MX25L_PERIF_INIT()  MX_SPI4_Init(); // MX_SPI5_Init() // SPI4 + SPI5
    #define MEM_MX25L_CS_INIT()     ; // L'init du CS est à faire dans l'Init correspondant ci-dessus

    // La structure pSPI_CS est à fournir comme paramètre dans chaque fonction appelée
    #define MEM_MX25L_PERIF_HANDLE  pSPI_CS->hSPI
    #define MEM_MX25L_CS_PORT       pSPI_CS->csPort
    #define MEM_MX25L_CS_PIN        pSPI_CS->csPin
#elif defined(OCTOSPI)
    #define MEM_MX25L_PERIF_INIT()  MX_OCTOSPI1_Init()  // OCTOSPI1
    #define MEM_MX25L_CS_INIT()     MEM_MX25L_DEACTIVATE_CS() //etat bas -> activité

    #define MEM_MX25L_PERIF_HANDLE  &hospi1
    #define MEM_MX25L_CS_PORT       Flash_Qspi_CS_GPIO_Port // GPIOE
    #define MEM_MX25L_CS_PIN        Flash_Qspi_CS_Pin       // P11
	#define MEM_MX25l_CLK_PIN  Flash_Qspi_Clk_Pin
	#define MEM_MX25l_CLK_PORT Flash_Qspi_Clk_GPIO_Port
	#define MEM_MX25l_IO0_PORT Flash_Qspi_IO0_GPIO_Port
	#define MEM_MX25l_IO0_PIN  Flash_Qspi_IO0_Pin
	#define MEM_MX25l_IO1_PORT Flash_Qspi_IO1_GPIO_Port
	#define MEM_MX25l_IO1_PIN  Flash_Qspi_IO1_Pin
	#define MEM_MX25l_IO2_PORT Flash_Qspi_IO2_GPIO_Port
	#define MEM_MX25l_IO2_PIN  Flash_Qspi_IO2_Pin
	#define MEM_MX25l_IO3_PORT Flash_Qspi_IO3_GPIO_Port
	#define MEM_MX25l_IO3_PIN  Flash_Qspi_IO3_Pin

#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    // Paramètre unique (statique) sur SPI4 + CS sur PE15 :
    #define MEM_MX25L_PERIF_INIT()  MX_SPI1_Init()  // SPI1
    #define MEM_MX25L_CS_INIT()     MEM_MX25L_DEACTIVATE_CS()

    #define MEM_MX25L_PERIF_HANDLE  &hspi1
    #define MEM_MX25L_CS_PORT       Memory_CS_GPIO_Port // PC12
    #define MEM_MX25L_CS_PIN        Memory_CS_Pin       // PC12
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

#define MEM_MX25L_CS_ACTIVE         GPIO_PIN_RESET  // Active Low
#define MEM_MX25L_CS_INACTIVE       GPIO_PIN_SET

#define MEM_MX25L_SEND_TO           100U            // Send TimeOut : 100 x 1ms = 100ms
#define MEM_MX25L_RECV_TO           100U            // Receive TimeOut : 100 x 1ms = 100ms

#define MEM_MX25L_ACTIVATE_CS()     HAL_GPIO_WritePin(MEM_MX25L_CS_PORT, MEM_MX25L_CS_PIN, MEM_MX25L_CS_ACTIVE)
#define MEM_MX25L_DEACTIVATE_CS()   HAL_GPIO_WritePin(MEM_MX25L_CS_PORT, MEM_MX25L_CS_PIN, MEM_MX25L_CS_INACTIVE)

#ifdef OCTOSPI

#define MEM_MX25L_PERIF_SEND_STREAM(TxBuf) HAL_XSPI_Transmit(MEM_MX25L_PERIF_HANDLE, TxBuf, MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_RECV_STREAM(RxBuf) HAL_XSPI_Receive( MEM_MX25L_PERIF_HANDLE, RxBuf, MEM_MX25L_RECV_TO)
#else
#define MEM_MX25L_PERIF_SEND_STREAM(TxBuf,BufSize) HAL_SPI_Transmit(MEM_MX25L_PERIF_HANDLE, TxBuf, BufSize, MEM_MX25L_SEND_TO)
#define MEM_MX25L_PERIF_RECV_STREAM(RxBuf,nb2Read) HAL_SPI_Receive( MEM_MX25L_PERIF_HANDLE, RxBuf, nb2Read, MEM_MX25L_RECV_TO)
#endif //OCTOSPI

// MX25L Read Commands :
#define MEM_MX25L_CMD_READ_STATUS_REGISTER  0x05 // RDSR : Read Status Register
#define MEM_MX25L_CMD_READ_CONFIG_REGISTER  0x15 // RCSR : Read Configuration Register
#define MEM_MX25L_CMD_READ_IDENTIFICATION   0x9F // RDID : Read Identification
#define MEM_MX25L_CMD_READ_SECURITY_REG     0x2B // RDSCUR : Read Security Register
#define MEM_MX25L_CMD_READ_DATA_BYTES       0x03 // READ : Normal Read
#define MEM_MX25L_CMD_FAST_READ_DATA_BYTES  0x0B // FAST_READ : Fast Read
#define MEM_MX25L_CMD_READ_ELECTRONIC_ID    0xAB // RES : Read Electronic ID
#define MEM_MX25L_CMD_READ_MFG_DEV_ID       0x90 // REMS : Read Electronic Manufacturer & device ID
#define MEM_MX25L_CMD_QUAD_READ_DATA_BYTES  0x6B // QUADREAD : Normal Read

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
#define MEM_MX25L_CMD_QUADPAGE_PROGRAM      0x38 // QPP -> Attention Page Boundary = 256 Bytes !

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

#define SPI_MEM_MX25L_MAKE_DWORD_4B(b3,b2,b1,b0)   (((b3) << 24) | ((b2) << 16) |((b1) << 8) | ((b0) << 0))

// MX25L3233F ID Definitions (from Table 9 p50) :
#define SPI_MEM_MX25L_RDID_MFG 0xC2 // Manufacturer ID : $C2 = Macronix
#define SPI_MEM_MX25L_RDID_MMT 0x20 // Memory Type
#define SPI_MEM_MX25L_RDID_MMD 0x16 // Memory Density
#define SPI_MEM_MX25L_RDID_VAL SPI_MEM_MX25L_MAKE_DWORD_4B(0, \
                                                        SPI_MEM_MX25L_RDID_MMD, \
                                                        SPI_MEM_MX25L_RDID_MMT, \
                                                        SPI_MEM_MX25L_RDID_MFG) // 0x001620C2
#define SPI_MEM_MX25L_RDID_MSK 0x00FFFFFF


typedef struct _SPI_CS_TypeDef
{
    SPI_HandleTypeDef  *hSPI;
    GPIO_TypeDef       *csPort;
    uint32_t            csPin;
} SPI_CS_TypeDef;



// Public functions :
    void Mem_MX25L_Init(void);
#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    uint8_t Mem_MX25L_ReadStatusRegister(SPI_CS_TypeDef *pSPI_CS, uint8_t *pStatusRegister);
    uint8_t Mem_MX25L_IsWriteBusy(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_Wait4WriteNotBusy(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_ReadConfigRegister(SPI_CS_TypeDef *pSPI_CS, uint8_t *pConfigRegister);
    // (autres fonctions publiques manquantes)
    #warning "Support MULTI_INSTANCE incomplet !!!"
#elif defined(OCTOSPI)
    HAL_StatusTypeDef HAL_XSPI_Transmit_lf(XSPI_HandleTypeDef *hxspi,const uint8_t *pData, uint32_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_XSPI_Receive_lf(XSPI_HandleTypeDef *hxspi, uint8_t *const pData, uint32_t Size, uint32_t Timeout);
    uint8_t Mem_MX25L_ReadStatusRegister(uint8_t *pStatusRegister);
    uint8_t Mem_MX25L_IsWriteBusy();
    uint8_t Mem_MX25L_Wait4WriteNotBusy();
    uint8_t Mem_MX25L_ReadConfigRegister(uint8_t *pConfigRegister);
    uint8_t Mem_MX25L_WriteStatusConfigRegister(uint8_t newStatusRegister, uint8_t newConfigRegister);
    uint8_t Mem_MX25L_ReadIdRegister(void *pID_24bits);
    uint8_t Mem_MX25L_ReadSecurityRegister(uint8_t *pSecurityRegister);
    uint8_t Mem_MX25L_ReadDataBytes(uint32_t baseAdr_24bits, uint32_t nbBytes2Read, void *pReadBuf);
    uint8_t Mem_MX25L_ReadDataBytes_HighSpeed(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf);


    uint8_t Mem_MX25L_StartReadArraySequence(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_ReadArrayInSequence(uint32_t nbBytes2Read, void *pReadBuf);
    uint8_t Mem_MX25L_StopReadArraySequence(void);
    uint8_t Mem_MX25L_NoOperation(void);
    uint8_t Mem_MX25L_SectorErase4K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_BlockErase64K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_BlockErase32K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_ChipErase(void);
    uint8_t Mem_MX25L_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint32_t nbBytes2Write);

    uint8_t Mem_MX25L_StartWriteArraySequence(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_WriteArrayInSequence(void* pArray2Write, uint32_t nbBytes2Write);
    uint8_t Mem_MX25L_StopWriteArraySequence(void);

    uint8_t Mem_MX25L_QuadPageProgram_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint32_t nbBytes2Write);
    uint8_t Mem_MX25L_QuadRead_Sequence(uint32_t baseAdr_24bits, uint32_t nbBytes2Read, void *pReadBuf);


#else // ! SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    uint8_t Mem_MX25L_ReadStatusRegister(uint8_t *pStatusRegister);
    uint8_t Mem_MX25L_IsWriteBusy();
    uint8_t Mem_MX25L_Wait4WriteNotBusy();
    uint8_t Mem_MX25L_ReadConfigRegister(uint8_t *pConfigRegister);
    uint8_t Mem_MX25L_WriteStatusConfigRegister(uint8_t newStatusRegister, uint8_t newConfigRegister);
    uint8_t Mem_MX25L_ReadIdRegister(void *pID_24bits);
    uint8_t Mem_MX25L_ReadSecurityRegister(uint8_t *pSecurityRegister);
    uint8_t Mem_MX25L_ReadDataBytes(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf);
    uint8_t Mem_MX25L_ReadDataBytes_HighSpeed(uint32_t baseAdr_24bits, uint16_t nbBytes2Read, void *pReadBuf);
    uint8_t Mem_MX25L_StartReadArraySequence(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_ReadArrayInSequence(uint16_t nbBytes2Read, void *pReadBuf);
    uint8_t Mem_MX25L_StopReadArraySequence(void);
    uint8_t Mem_MX25L_NoOperation(void);
    uint8_t Mem_MX25L_SectorErase4K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_BlockErase64K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_BlockErase32K(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_ChipErase(void);
    uint8_t Mem_MX25L_WriteArray(uint32_t baseAdr_24bits, void* pArray2Write, uint16_t nbBytes2Write);
    uint8_t Mem_MX25L_StartWriteArraySequence(uint32_t baseAdr_24bits);
    uint8_t Mem_MX25L_WriteArrayInSequence(void* pArray2Write, uint16_t nbBytes2Write);
    uint8_t Mem_MX25L_StopWriteArraySequence(void);
    uint8_t Mem_MX25L_DoSoftwareReset(void);
#endif // SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE

#endif /* MEM_MX25L_SPI_MEM_MX25L_SPI_H_ */
