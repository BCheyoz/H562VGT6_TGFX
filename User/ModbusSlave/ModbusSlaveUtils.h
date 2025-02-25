/*
 * ModbusSlaveUtils.h
 *
 *  Created on: 28 avr. 2021
 *      Author: j.proux
 *
 *  Updated on: 19 Déc. 2023
 *  Updated by: j.proux
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#ifndef MODBUSSLAVE_MODBUSSLAVEUTILS_H_
#define MODBUSSLAVE_MODBUSSLAVEUTILS_H_

#include <stdint.h>		// Pour les types "int*_t" & "uint*_t"
//#if (__has_include("utils.h"))	// Macro spéciale de 'Code Composer Studio' pour µC de 'Texas Instruments'
	#include "utils.h"  	// Pour accès aux éléments utiles génériques
//#endif

/******************************************************************************/

typedef struct _tModbusSlaveItem
{
    uint16_t Adr;
    struct {
        union {
            struct {
                unsigned readMinLevel :4; // Value in eTVarAccessMinLevel
                unsigned writeMinLevel:4; // Value in eTVarAccessMinLevel
            } __attribute__ ((__packed__));
            uint8_t accessMinLevels;
        } __attribute__ ((__packed__));
        uint8_t typeGetSet; // Value in eTVarGetSet
    } __attribute__ ((__packed__));
    void* getPtr;	// Pointer to a RAM address with "&" OR directly the "Get" functionName
    void* setPtr;	// Pointer to a RAM address with "&" OR directly the "Set" functionName
} tModbusSlaveItem;

/******************************************************************************/

// définition des Niveaux Minimum d'accès requis pour un élément de la Table Modbus :
typedef enum {
	ACCESS_MIN_LEVEL_0	= 0,
	ACCESS_MIN_LEVEL_1	= 1,
	ACCESS_MIN_LEVEL_2	= 2,
	ACCESS_MIN_LEVEL_3	= 3,
	ACCESS_MIN_LEVEL_4	= 4,
	ACCESS_MIN_LEVEL_5	= 5,
	ACCESS_MIN_LEVEL_6	= 6,
	ACCESS_MIN_LEVEL_7	= 7,
	ACCESS_MIN_LEVEL_8	= 8,
	ACCESS_MIN_LEVEL_9	= 9,
	ACCESS_MIN_LEVEL_10	= 10,
	ACCESS_MIN_LEVEL_11	= 11,
	ACCESS_MIN_LEVEL_12	= 12,
	ACCESS_MIN_LEVEL_13	= 13,
	ACCESS_MIN_LEVEL_14	= 14,
	ACCESS_MIN_LEVEL_15	= 15,
	ACCESS_MIN_LEVEL_MIN= ACCESS_MIN_LEVEL_0,
	ACCESS_MIN_LEVEL_MAX= ACCESS_MIN_LEVEL_15,
} eTVarAccessMinLevel;

/******************************************************************************/

// Type pour support QWORD 64bits :
typedef union {
    struct {
        union { // 32 bits DWord0
            struct {
                union { // 16 bits Word0 :
                    struct {
                        union { // 8 bits Byte0 :
                            int8_t  SChar0;
                            uint8_t UChar0;
                        };
                        union { // 8 bits Byte1
                            int8_t  SChar1;
                            uint8_t UChar1;
                        };
                    };
                    int16_t SInt0;
                    uint16_t UInt0;
                };
                union { // 16 bits Word1 :
                    struct {
                        union { // 8 bits Byte2 :
                            int8_t  SChar2;
                            uint8_t UChar2;
                        };
                        union { // 8 bits Byte3 :
                            int8_t  SChar3;
                            uint8_t UChar3;
                        };
                    };
                    int16_t SInt1;
                    uint16_t UInt1;
                };
            };
            int32_t SLong0;
            uint32_t ULong0;
            float Float0; // Ajout_Jp le 28/05/2020 : taille = 4 Bytes
        };
        union { // 32 bits DWord1 :
            struct {
                union { // 16 bits Word2 :
                    struct {
                        union { // 8 bits Byte4 :
                            int8_t  SChar4;
                            uint8_t UChar4;
                        };
                        union { // 8 bits Byte5 :
                            int8_t  SChar5;
                            uint8_t UChar5;
                        };
                    };
                    int16_t SInt2;
                    uint16_t UInt2;
                };
                union { // 16 bits Word3 :
                    struct {
                        union { // 8 bits Byte6 :
                            int8_t  SChar6;
                            uint8_t UChar6;
                        };
                        union { // 8 bits Byte7 :
                            int8_t  SChar7;
                            uint8_t UChar7;
                        };
                    };
                    int16_t SInt3;
                    uint16_t UInt3;
                };
            };
            int32_t SLong1;
            uint32_t ULong1;
        };
    };
    int64_t SLongLong;
    uint64_t ULongLong;
    double Double; // Ajout_Jp le 28/05/2020 : taille = 8 Bytes

   // Pour accès également en tableaux :
    uint8_t  Byte[8];  // Tableau de 8 Bytes 8bits
    uint16_t Word[4];  // Tableau de 4 Words 16bits
    uint32_t DWord[2]; // Tableau de 2 DWords 32bits
} tModbusSlaveQWordVar;

/******************************************************************************/

#define MODBUS_SLAVE_MAKE_CONST_BASE_PTR_OF_TABLE(t,b,a)	const t* b = a
#define MODBUS_SLAVE_MAKE_XTERN_CONST_BASE_OF_TABLE(t,b)	extern const t* b

#define MODBUS_SLAVE_MAKE_CONST_END_OF_TABLE(t,n,b)	const t* n = ((void*)b) + (sizeof(b))
//#define MODBUS_SLAVE_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = ((void*)a) + (sizeof(a))
#define MODBUS_SLAVE_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = (t*)(((void*)a) + (sizeof(a)))
#define MODBUS_SLAVE_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(t,b,e)	extern const t* b; extern const t* e

#define MODBUS_SLAVE_MAKE_ARRAY_PARAMS(name,count)	tModbusSlaveParams name[count]
#define MODBUS_SLAVE_MAKE_EXTRN_PARAMS(name,count)	extern MODBUS_SLAVE_MAKE_ARRAY_PARAMS(name,count)
#define MODBUS_SLAVE_GET_ITEM_ARRAY_PTR(name,id)	&name[id]

/******************************************************************************/

#define MODBUS_SLAVE_HEADER_SIZE	(2)			// Taille du Header = 1 (SlaveAdr) + 1 (CodeFonction)
#define MODBUS_SLAVE_FOOTER_SIZE	(2)			// Taille du Footer = 2 (CRC16)

#define MODBUS_SLAVE_MIN_RX_SIZE	(MODBUS_SLAVE_HEADER_SIZE +1 +MODBUS_SLAVE_FOOTER_SIZE)	// Taille Minimale d'une Requête Modbus
#define MODBUS_SLAVE_MIN_TX_SIZE	(MODBUS_SLAVE_HEADER_SIZE +1 +MODBUS_SLAVE_FOOTER_SIZE)	// Taille Minimale d'une Réponse ModbusSlave

#define MODBUS_SLAVE_MAX_RX_BLOC_SZ	1		// Taille Maximale à recevoir en une seule fois : Bytes 1 par 1 sur UART

/******************************************************************************/
// Prototypes des Fonctions Internes Modbus accessibles depuis une Table ModbusSlave externe :

void RegisterNewAccessPswd(uint16_t NewPswd);
void SaveParamsOnOrder(uint16_t Order2Save);
void handleQueryReInitFactory(uint16_t pswd);

uint16_t GetNullU16(void);
uint32_t GetNullU32(void);
void DiscardWriteU16(uint16_t NewValue);

uint32_t GetUID_0(void);
uint32_t GetUID_1(void);
uint32_t GetUID_2(void);
uint32_t GetUID_3(void);
uint16_t GetFlashSize(void);
uint16_t GetPackageData(void);
//uint16_t GetPackageSize_0(void);
//uint16_t GetPackageSize_1(void);
//uint16_t GetPackageSize_2(void);
//uint16_t GetPackageSize_3(void);

uint16_t getThisModbusSrcId(void);
uint16_t getThisModbusRxBufSize(void);
uint16_t getThisModbusTxBufSize(void);

/******************************************************************************/
// Links to utils.h :

#define ModbusSlave_FillMemory	FillMemory
#define ModbusSlave_CopyMemory	CopyMemory
void ModbusSlave_FillMemory(uint8_t* pDest, uint8_t Value, uint16_t BytesCt);
void ModbusSlave_CopyMemory(uint8_t* pDest, const uint8_t* pSrc, uint16_t BytesCt);

#endif /* MODBUSSLAVE_MODBUSSLAVEUTILS_H_ */
