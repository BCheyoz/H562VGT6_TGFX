/*
 * crc.h
 *
 *  Created on: 4 avr. 2019
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 */

#ifndef CRC_H_
#define CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************/

// Paramètres User à Activer, si nécessaire :
#define CRC_SUPPORT_CRC8	// Nécessaire pour SHT_30, SDP_6xx
#define CRC_SUPPORT_CRC16	// Nécessaire pour Trames Modbus
#define CRC_SUPPORT_CRC32	// Nécessaire pour Calcul signature du Soft

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC8	//	Support du CRC8 :

#define INIT_CRC_8			0xFF
uint8_t CRC8(uint8_t* ptr, uint8_t nbBytes, uint8_t CrcInit);
//uint8_t CRC8_Poly(uint8_t* ptr, uint8_t NumBytes, uint8_t CrcInit, uint8_t Polynomial);
uint8_t isValidMultiU16BlocsCRC8(uint8_t* pBuf, uint8_t nbU16Blocs, uint8_t CrcInit);

#endif // CRC_SUPPORT_CRC8

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC16	// Support du CRC16 :

#define POLYNOME_16			0xA001  // Polynôme CRC16 : 2^15 + 2^13 + 2^0 = 0xA001.
#define INIT_CRC_16			0xFFFF
uint16_t CRC16(uint8_t* ptr, uint16_t nbBytes, uint16_t Crc);

#endif // CRC_SUPPORT_CRC16

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32	// Support du CRC32 :

#define POLYNOME_32			0xEDB88320L     	// Standard CRC-32 Polynomial
#define INIT_CRC_32			0xFFFFFFFFUL
uint32_t CRC32(uint32_t crc_accum, uint8_t* ptr, uint32_t nbBytes);
void CreateCRC32_Table(void);
void FreeCRC32_Table(void);

#endif // CRC_SUPPORT_CRC32


#ifdef __cplusplus
}
#endif

#endif /* CRC_H_ */
