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
 *   -> Suivre les indications dans "Mem_MX25L_SPI.h"
 *
 */

#include "stm32h5xx_hal.h"

#include "MX25L_SPI.h"
#include "spi.h"
#include "main.h"


//******************************************************************************

// Internal only usage functions :
#ifdef SPI_MEM_MX25L_SUPPORT_MULTI_INSTANCE
    uint8_t Mem_MX25L_WriteEnable(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_WriteDisable(SPI_CS_TypeDef *pSPI_CS);
    uint8_t Mem_MX25L_SendThisByte(SPI_CS_TypeDef *pSPI_CS, uint8_t byte2Send);
    uint8_t Mem_MX25L_SendArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send);
    uint8_t Mem_MX25L_SendReceiveArray(SPI_CS_TypeDef *pSPI_CS, void* pArray2Send, uint16_t nbBytes2Send,
                                       uint16_t nbBytes2Read, void* pArray2Receive);
#elif defined(OCTOSPI)

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
#ifdef OCTOSPI

#else
void Mem_MX25L_Init(void)
{ // Vérif_Jp = OK sur IS25LP le 21/06/2019
    MEM_MX25L_CS_INIT();
    MEM_MX25L_PERIF_INIT();
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

uint8_t Mem_MX25L_BlocErase32K(uint32_t baseAdr_24bits)
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

#endif //OCTOSPI
