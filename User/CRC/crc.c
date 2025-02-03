/*
 * crc.c
 *
 *  Created on: 4 avr. 2019
 *
 *  Updated on: 19 Déc. 2023
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement les Modules CRC dans un nouveau Projet :
 *   -> Suivre les indications dans "crc.h"
 *
 */

#include <stdint.h> // Pour les types "int*_t" & "uint*_t"
#include "crc.h"	// Pour nos propres déclarations publiques

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32

#ifdef TABLE_CRC32_DYNAMIQUE
	#include "stdlib.h"
#endif

// DEFINITIONS :
#define SIZE_TABLE_CRC32	256				// Non modifiable

// VARIABLES :
//unsigned long CRC32_Value;

#ifdef TABLE_CRC32_DYNAMIQUE
	uint32_t *crc_table = NULL;
#else
	uint32_t crc_table[SIZE_TABLE_CRC32];
#endif

#endif // CRC_SUPPORT_CRC32

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC8

uint8_t CRC8(uint8_t* ptr, uint8_t NumBytes, uint8_t CrcInit)
{
	const uint8_t Polynomial = 0x31;	// 0x31 = x^8 + x^5 + x^4 + 1
//	const uint8_t FinalXor = 0x00;		// Final XOR = 0x00
	// Reflect Input = FALSE ; Reflect Output = FALSE
	uint8_t Crc8, i;

	Crc8 = CrcInit;
	while(NumBytes-- > 0)
	{
		Crc8 ^= (*ptr++);
		for(i=1; i <= 8; i++)	// 8 tours car CRC sur 8 bits
		{
			if(Crc8 & (1 << (8-1)))
			{
				Crc8 = (uint8_t)((Crc8 << 1) ^  (Polynomial));
			} else {
				Crc8 = (Crc8 << 1);
			}
		}
	}
	return (Crc8 /*^ FinalXor*/);
}

#endif // CRC_SUPPORT_CRC8

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC8

/*
uint8_t CRC8_Poly(uint8_t* ptr, uint8_t NumBytes, uint8_t CrcInit, uint8_t Polynomial)
{
	uint8_t Crc8, i;

	Crc8 = CrcInit;
	while(NumBytes-- > 0)
	{
		Crc8 ^= (*ptr++);
		for(i=1; i <= 8; i++)   // 8 tours car CRC sur 8 bits
		{
			if(Crc8 & (1 << (8-1)))
			{
				Crc8 = (Crc8 << 1) ^  (Polynomial);
			} else {
				Crc8 = (Crc8 << 1);
			}
		}
	}
	return (Crc8);
}
*/

#endif // CRC_SUPPORT_CRC8

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC8

uint8_t isValidMultiU16BlocsCRC8(uint8_t* pBuf, uint8_t nbU16Blocs, uint8_t CrcInit)
{
#define U16_BLOC_SIZE sizeof(uint16_t)
#define U16_CRC8_BLOC_SIZE (U16_BLOC_SIZE + sizeof(uint8_t))

	for(int i = 0; i < nbU16Blocs; i++)
	{
		if(pBuf[i * U16_CRC8_BLOC_SIZE + U16_BLOC_SIZE] != CRC8(&pBuf[i * U16_CRC8_BLOC_SIZE], U16_BLOC_SIZE, CrcInit)) return 0;
	}
	return 1;
}

#endif // CRC_SUPPORT_CRC8

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC16

uint16_t CRC16(uint8_t* Adresse_tab, uint16_t Taille_max, uint16_t CrcInit)
{
	uint16_t Polynome = POLYNOME_16;
	uint16_t CptOctet = 0;
	uint8_t  CptBit = 0;
	uint8_t  Parity = 0;
	uint16_t Crc;

	Crc = CrcInit;

	for(CptOctet = 0; CptOctet < Taille_max; CptOctet++)
	{
		Crc ^= *(Adresse_tab + CptOctet);		// Ou eXclusif entre octet message et CRC

		for(CptBit = 0; CptBit <= 7; CptBit++)	// Mise à 0 du compteur nombre de bits
		{
			Parity = (uint8_t)Crc;
			Crc >>= 1;							// Décalage à droite du crc
			if(Parity % 2 == 1)					// Si impair -> Après décalage à droite il y aura une retenue
			{
				Crc ^= Polynome;				// "Ou eXclusif" entre le CRC et le polynôme générateur.
			}
		}
	}
	return (Crc);
}

#endif // CRC_SUPPORT_CRC16

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32

void CreateCRC32_Table(void)
{
	uint32_t crc, poly;
	int i, j;

#ifdef TABLE_CRC32_DYNAMIQUE
	if(crc_table == NULL)
	{
		crc_table = malloc(SIZE_TABLE_CRC32*sizeof(unsigned long));
	}
#endif

	poly = POLYNOME_32;

	for(i = 0; i < SIZE_TABLE_CRC32; i++)
	{
		crc = (uint32_t)i;
		for(j = 8; j > 0; j--)
		{
			if(crc & 1)
			{
				crc = (crc >> 1) ^ poly;
			} else {
				crc >>= 1;
			}
		}
		crc_table[i] = crc;
	}
}

#endif // CRC_SUPPORT_CRC32

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32

uint32_t CRC32(uint32_t crc_accum, uint8_t* data_blk_ptr, uint32_t data_blk_size)
{
	uint32_t j;

	for(j=0; j<data_blk_size; j++)
	{
		crc_accum = ((crc_accum >> 8) & 0x00FFFFFF) ^ crc_table[(crc_accum ^ *data_blk_ptr++) & 0xFF];
	}

	return crc_accum;
}

#endif // CRC_SUPPORT_CRC32

/******************************************************************************/

#ifdef CRC_SUPPORT_CRC32

void FreeCRC32_Table(void)
{

#ifdef TABLE_CRC32_DYNAMIQUE
	free(crc_table);
	crc_table = NULL;
#endif

}

#endif // CRC_SUPPORT_CRC32
