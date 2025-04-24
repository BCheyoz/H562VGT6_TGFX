/*
 * utils.h
 *
 *  Created on: 22 oct. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 03 Fev. 2025
 *  Updated by: b.chhay
 *
 * version 1.0
 *  History Usage :
 *-> 25/02/2022 : Added by AB to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************/

#define MAX2(a,b)			(((a) >= (b)) ? (a) : (b))
#define MAX3(a,b,c) 		(((MAX2(a,b)) >= (c)) ? (MAX2(a,b)) : (c))
#define MAX4(a,b,c,d)		(((MAX2(a,b)) >= (MAX2(c,d))) ? (MAX2(a,b)) : (MAX2(c,d)))
#define MAX5(a,b,c,d,e)     (((MAX2(a,b)) >= (MAX3(c,d,e))) ? (MAX2(a,b)) : (MAX3(c,d,e)))

#define MIN2(a,b)			(((a) <= (b)) ? (a) : (b))

#define ABS(x)				(((x)<0)?-(x):(x))

#define UINT8T_MAX			0xFF
#define UINT16T_MAX			0xFFFF
#define UINT32T_MAX			0xFFFFFFFF
#define UINT64T_MAX			0xFFFFFFFFFFFFFFFF

/******************************************************************************/
// Macros utiles à la Librairie "I2cComMaster" :

#define JOIN_32_32_BE(a,b)	( ((((uint64_t)a) & 0xFFFFFFFF) << 32) | ((((uint64_t)b) & 0xFFFFFFFF) << 0) )
#define GET_LOW_32(a)		(((a) >>  0) & 0xFFFFFFFF)
#define GET_HIGH_32(a)		(((a) >> 32) & 0xFFFFFFFF)

#define JOIN_16_16_BE(a,b)	( (((a) & 0xFFFF) << 16) | (((b) & 0xFFFF) << 0) )
#define GET_LOW_16(a)		(((a) >>  0) & 0xFFFF)
#define GET_HIGH_16(a)		(((a) >> 16) & 0xFFFF)

#define JOIN_8_8_BE(a,b)	( (((a) & 0xFF) << 8) | (((b) & 0xFF) << 0) )
#define JOIN_8_8_LE(a,b)	( (((b) & 0xFF) << 8) | (((a) & 0xFF) << 0) )	// LittleEndian : a = Byte0; b = Byte1
#define GET_BYTE_0(a)		(((a) >>  0) & 0xFF)
#define GET_BYTE_1(a)		(((a) >>  8) & 0xFF)
#define GET_BYTE_2(a)		(((a) >> 16) & 0xFF)
#define GET_BYTE_3(a)		(((a) >> 24) & 0xFF)
#define GET_BYTE_N(a,b) 	(((a) >> ((b)*8)) & 0xFF) // Récupère le ByteN (n = [0, 3] sur 32 bits)

/******************************************************************************/

#define TRUE 1
#define FALSE 0

/******************************************************************************/
// Macros utiles à la Librairie "AnalogInputs" :
#ifndef IS_IN_RANGE
	#define IS_IN_RANGE(val,min,max)	(((val) >= (min)) && ((val) <= (max)))
#endif // IS_IN_RANGE

#define ROUND_TO_UINT_CAST(t,f)	(t)((f) + 0.5f)	// (float, type) -> (type)

/******************************************************************************/
// Macros utiles à la Librairie "DigitalInputs" :

#define FLAG_FOR_BIT_ID(b)  		(1 << (b))

/******************************************************************************/
// Macros utiles à la Librairie "PwmFan" :

#define REFRAME_TO_MAX_OF_RANGE(val,min,max)	if( ((val) < (min)) || ((val) > (max)) ) (val) = (max)

/******************************************************************************/
// Macros utiles à la Librairie "UpgradeFirmware" :

#define MAKE_DWORD_BE(hh,hl,lh,ll)	(uint32_t)((((hh) & 0xFF)<<24) | (((hl) & 0xFF) << 16) | (((lh) & 0xFF)<<8) | (((ll) & 0xFF) << 0)) // hh = Byte3 ... ll = Byte0
#define MAKE_DWORD_LE(ll,lh,hl,hh)	(uint32_t)((((hh) & 0xFF)<<24) | (((hl) & 0xFF) << 16) | (((lh) & 0xFF)<<8) | (((ll) & 0xFF) << 0)) // hh = Byte3 ... ll = Byte0

/******************************************************************************/

#include <stdint.h>		// Pour les types "int*_t" & "uint*_t"

void FillMemoryWithBuffer(uint8_t* pDest, const uint8_t *pSrc, uint16_t valueSize, uint16_t BytesCt);
void FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void CopyMemory(uint8_t* pDest, const uint8_t *pSrc, uint16_t BytesCt);

#define REG_GDC_CLEAR_STRUCT(p)             FillMemory((void*)&p, 0, sizeof(p))
#define REG_GDC_COPY_STRUCT_PTR_PTR(d,p)	CopyMemory((void*)d, (void*)p, sizeof(d))

#define DONT_WARN_ME_UNUSED_PARAMETER(var)	(void)var

/******************************************************************************/
// Définitions & Structures partagés entre plusieurs Modules RTC :

typedef struct _RTC_CommonBase_DateTime
{
    uint16_t Year;
    uint8_t  Month;
    uint8_t  Date;
    uint8_t  Weekday;
    uint8_t  Hours;
    uint8_t  Minutes;
    uint8_t  Seconds;
} __attribute__ ((__packed__)) RTC_CommonBase_DateTime;

/******************************************************************************/

/* compare les premiers octets pointes par pDest avec ceux pointes par pSrc. Retourne 1 si les deux zones
 * memoires sont les memes, sinon 0.
 */
uint8_t areMemoryAreasTheSame(uint8_t* pDest, uint8_t* pSrc, uint16_t BytesCt);


#ifdef __cplusplus
}
#endif

#endif /* UTILS_UTILS_H_ */
