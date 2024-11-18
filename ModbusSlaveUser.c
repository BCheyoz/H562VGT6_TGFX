/*
 * ModbusSlaveUser.c
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 14 Fév. 2024
 *  Updated by: a.bardant
 *
 *  Updated on: 8 Avr. 2024
 *  Updated by: m.faget
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#include "ModbusSlaveUser.h"	// Pour nos propres déclarations publiques

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

typedef enum {
    TVarUnknown,

    // Type Signed Char (1 Byte = 8 bits) :
    TVarSCharGetVarSetVar,
    TVarSCharGetVarSetFct,
    TVarSCharGetFctSetVar,
    TVarSCharGetFctSetFct,

    // Type Unsigned Char (1 Byte = 8 bits) :
    TVarUCharGetVarSetVar,
    TVarUCharGetVarSetFct,
    TVarUCharGetFctSetVar,
    TVarUCharGetFctSetFct,

    // Type Signed Int (2 Bytes = 1x 16 bits) :
    TVarSIntGetVarSetVar,
    TVarSIntGetVarSetFct,
    TVarSIntGetFctSetVar,
    TVarSIntGetFctSetFct,

    // Type Unsigned Int (2 Bytes = 1x 16 bits) :
    TVarUIntGetVarSetVar,
    TVarUIntGetVarSetFct,
    TVarUIntGetFctSetVar,
    TVarUIntGetFctSetFct,

    // Type Signed Long (4 Bytes = 2x 16 bits) :
    TVarSLongGetVarSetVar,
    TVarSLongGetVarSetFct,
    TVarSLongGetFctSetVar,
    TVarSLongGetFctSetFct,

    // Type Unsigned Long (4 Bytes = 2x 16 bits) :
    TVarULongGetVarSetVar,
    TVarULongGetVarSetFct,
    TVarULongGetFctSetVar,
    TVarULongGetFctSetFct,

    // Type Signed LongLong (8 Bytes = 4x 16 bits) :
    TVarSLongLongGetVarSetVar,
    TVarSLongLongGetVarSetFct,
    TVarSLongLongGetFctSetVar,
    TVarSLongLongGetFctSetFct,

    // Type Unsigned LongLong (8 Bytes = 4x 16 bits) :
    TVarULongLongGetVarSetVar,
    TVarULongLongGetVarSetFct,
    TVarULongLongGetFctSetVar,
    TVarULongLongGetFctSetFct,

    // Type Float Int x1 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX1GetVarSetVar,
    TVarFloatIntX1GetVarSetFct,
    TVarFloatIntX1GetFctSetVar,
    TVarFloatIntX1GetFctSetFct,

    // Type Float Int x10 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX10GetVarSetVar,
    TVarFloatIntX10GetVarSetFct,
    TVarFloatIntX10GetFctSetVar,
    TVarFloatIntX10GetFctSetFct,

    // Type Float Int x100 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX100GetVarSetVar,
    TVarFloatIntX100GetVarSetFct,
    TVarFloatIntX100GetFctSetVar,
    TVarFloatIntX100GetFctSetFct,

    // Type Float Int x1000 (2 Bytes = 1x 16 bits) :
    TVarFloatIntX1000GetVarSetVar,
    TVarFloatIntX1000GetVarSetFct,
    TVarFloatIntX1000GetFctSetVar,
    TVarFloatIntX1000GetFctSetFct,

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_LONG
	//----------------------------------------
	// Type Float Long x10 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX10GetVarSetVar,
    TVarFloatLongX10GetVarSetFct,
    TVarFloatLongX10GetFctSetVar,
    TVarFloatLongX10GetFctSetFct,

	// Type Float Long x100 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX100GetVarSetVar,
    TVarFloatLongX100GetVarSetFct,
    TVarFloatLongX100GetFctSetVar,
    TVarFloatLongX100GetFctSetFct,

	// Type Float Long x1000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX1KGetVarSetVar,
    TVarFloatLongX1KGetVarSetFct,
    TVarFloatLongX1KGetFctSetVar,
    TVarFloatLongX1KGetFctSetFct,

	// Type Float Long x10.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX10KGetVarSetVar,
    TVarFloatLongX10KGetVarSetFct,
    TVarFloatLongX10KGetFctSetVar,
    TVarFloatLongX10KGetFctSetFct,

	// Type Float Long x100.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX100KGetVarSetVar,
    TVarFloatLongX100KGetVarSetFct,
    TVarFloatLongX100KGetFctSetVar,
    TVarFloatLongX100KGetFctSetFct,

	// Type Float Long x1.000.000 (4 Bytes = 2x 16 bits) :
	TVarFloatLongX1MGetVarSetVar,
    TVarFloatLongX1MGetVarSetFct,
    TVarFloatLongX1MGetFctSetVar,
    TVarFloatLongX1MGetFctSetFct,

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_LONG

#ifdef MODBUS_SLAVE_SUPPORT_FLOAT_RAW
	//----------------------------------------
	// Type Float Long Raw (4 Bytes = 2x 16 bits) :
	TVarFloatLongRawGetVarSetVar,
    TVarFloatLongRawGetVarSetFct,
    TVarFloatLongRawGetFctSetVar,
    TVarFloatLongRawGetFctSetFct,

	// Type Double LongLong Raw (8 Bytes = 4x 16 bits) :
	TVarDoubleLongLongRawGetVarSetVar,
    TVarDoubleLongLongRawGetVarSetFct,
    TVarDoubleLongLongRawGetFctSetVar,
    TVarDoubleLongLongRawGetFctSetFct,

#endif // MODBUS_SLAVE_SUPPORT_FLOAT_RAW

} eTVarGetSet;

unsigned int TestDate;
unsigned long SapProduct, SapBoard;
unsigned long long TestDate, SnProduct, SnBoard;

// Table Modbus en Référence @ RMD-321 (7.1.1.3 "Liste des registres adressables") :
const tModbusSlaveItem TableModbusSlave[] = {
//   Adresse,       RdMinLevel,         WrMinLevel,             VarType and Get/Set Method,     RdPtr,                                      WrPtr :

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT
	// Identification :

	{ 0x01,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetFctSetFct},		&SapProduct,		&SapProduct}, // Code SAP du Produit complet (Name = "Code SAP Produit")
	{ 0x03,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongLongGetFctSetFct},	&SnProduct,		&SnProduct},	// SN du Produit ALDES (Name = "SN du Produit")
	{ 0x07,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},		&TestDate,		&TestDate},	// Date dernier test sous traitant (Name = "Date Test Carte")
	{ 0x08,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetFctSetFct},		&SapBoard,		&SapBoard},	// Code SAP de la Carte Principale (Name = "Code SAP Carte")
	{ 0x0A,		{{{ ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongLongGetFctSetFct},		&SnBoard,			&SnBoard},	// S/N de la Carte Pincipale (Name = "SN Carte Principale")
	{ 0x0C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		GetVersionSoft16,		0},	// Version du Firmware (SHOW_HEX) Modif_Jp le 03/08/2023 pour Ticket #148

};
//MODBUS_SLAVE_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tModbusSlaveItem, BaseOfTableModbusSlave, EndOfTableModbusSlave, TableModbusSlave);

/******************************************************************************/

inline void ModbusSlaveInitUserMST(void)
{
}

/******************************************************************************/

void SetModbusUserSlaveAdr(uint16_t newSlaveAdr)
{
/*
//	ModbusUser.mySlaveAdr = newSlaveAdr;
	tModbusSlaveParams* pStruct = MODBUS_SLAVE_UART_USER_PARAMS;
	pStruct->SlaveAdr = newSlaveAdr;
	Fab_MdbUserSlaveId = newSlaveAdr;
*/
}

/******************************************************************************/

void checkModbusParamsFromMemHisto(uint16_t newSlaveAdr) // Ajout_Jp le 10/06/2020 pour Contrôle des Paramètres après Importation par MemHisto
{
/*
#define DEF_MODBUS_USER_SLAVE_ADR	MODBUS_SLAVE_USER_DEF_SLAVE_ID
//	if(UINT16_MAX == newSlaveAdr) { newSlaveAdr = MODBUS_DEFAULT_SLAVE_ADR; } // Si vierge => Valeur par défaut
	if( (UINT16_MAX == newSlaveAdr) || (0 == newSlaveAdr) ) { newSlaveAdr = DEF_MODBUS_USER_SLAVE_ADR; } // Correction_Jp le 22/06/2020 : Si vierge (ou non configuré) => Valeur par défaut
	SetModbusUserSlaveAdr(newSlaveAdr & 0x7F); // Adresses autorisées jusqu'à 127 maximum
*/
}

/******************************************************************************/

void initModbusUserParamsFromMemHisto(void) // Ajout_Jp le 22/06/2020 : en cas de Démarrage du Soft avec Paramètres vierges
{
//	SetModbusUserSlaveAdr(DEF_MODBUS_USER_SLAVE_ADR);
}

/******************************************************************************/

__attribute__((weak)) uint32_t GetVersionSoft(void) // Pour proposer un Pseudo Versionning en l'absence de la Librairie "VersionInfos"
{
	return 0xA0000001; // Alpha v0.00.01 sur 32bits
}

/******************************************************************************/
