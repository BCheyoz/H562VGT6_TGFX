/*
 * utils.c
 *
 *  Created on: 22 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 29 Oct. 2021
 *  Updated by: j.proux
 *
 *      Fonctions de calcul utiles a divers modules
 *
 */

#include "utils.h"	// Pour nos propres déclarations publiques

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

void FillMemoryWithBuffer(uint8_t* pDest, const uint8_t *pSrc, uint16_t valueSize, uint16_t BytesCt){
    while(BytesCt > 0)
    {
        for(uint16_t idx = 0; idx < valueSize; idx++){
            *pDest = pSrc[idx];
            pDest++;
        }
        BytesCt--;
    }
}

/******************************************************************************/

inline void FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt)
{
	while(BytesCt > 0)
	{
		*pDest = Value;
		pDest++;
		BytesCt--;
	}
}

/******************************************************************************/

inline void CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt)
{
	while(BytesCt > 0)
	{
		*pDest = *pSrc;
		pDest++;
		pSrc++;
		BytesCt--;
	}
}

/******************************************************************************/

uint8_t areMemoryAreasTheSame(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt)
{
	while(BytesCt > 0)
	{
		if(*pDest != *pSrc)
			return 0;

		pDest++;
		pSrc++;
		BytesCt--;
	}
	return 1;
}


#ifdef __cplusplus
}
#endif
