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

#ifndef MX25L_SPI_MX25L_SPI_H_
#define MX25L_SPI_MX25L_SPI_H_

#ifndef USE_HAL_DRIVER
	#error "SPI Memory MX25L Error : HAL Driver Required !"
#else // HAL_DRIVER :

#endif

#include <stdint.h>
#include "spi.h"

//#define SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE // Activer uniquement si plusieurs Mémoires à gérer en SPI !
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

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* MX25L_SPI_MX25L_SPI_H_ */
