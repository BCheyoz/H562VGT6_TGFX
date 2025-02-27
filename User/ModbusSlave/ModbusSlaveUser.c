/*
 * ModbusSlaveUser.c
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 25 Feb. 2025
 *  Updated by: j.proux
 *
 * Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 * -> il faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#include "ModbusSlaveUser.h"	// Pour nos propres déclarations publiques

/******************************************************************************/
// Includes Optionnels :

#include "VersionInfos.h"	// Pour les infos de Version / Run / CRC
#include "BaseDeTemps.h"	// Pour le Compteur de RunTime

/******************************************************************************/
// Includes personnalisés, suivant les besoins :

/* USER CODE BEGIN Includes */

//#include "GestionLedAlive.h"		// Pour accès aux Commandes de la Led de Vie
//#include "DigitalInputsUser.h"  	// Pour accès aux Infos des DigitalInputs
//#include "I2cComMasterSystem.h" 	// Pour accès à tous les Capteurs sur I2C_System
//#include "GestionInputSensor.h" 	// Pour accès à la synthèse des Capteurs d'environement
//#include "AnalogInputsUser.h"		// Pour accès aux Variables AnalogInputs
//#include "MSM_mainStateMachine.h"	// pour la lecture de l'etat logiciel
//#include "memoireNonVolatile.h" 	// Pour accès à la Mémoire non-volatile
//#include "ERR_ErrorManager.h"		// pour la remontee des erreurs
//#include "iBusMantaDatas.h" 		// Pour accès à la structure de stockage interne des Datas des Mantas
//#include "mainRegulation.h" 		// Pour accès aux données d'entrées et sortie du bloc de regulation
//#include "DateTime32.h" 			// Pour accès à la Gestion de la DateTime
//#include "INF_productInfo.h"		// pour la lecture de l'etat logiciel
//#include "iBusDevTesteur.h" 		// Pour accès aux Commandes de Test des iBus
//#include "UpgradeFirmware.h"		// Pour accès aux Infos & Commandes de Mise à Jour Firmware

/* USER CODE END Includes */

/******************************************************************************/
// Variables locales & externes, suivant les besoins :

/* USER CODE BEGIN Variables */

uint16_t InstallCodePin = 405;	// For IHM Himalaya2

/* USER CODE END Variables */

/******************************************************************************/
// Prototypes de Fonctions locales & externes, suivant les besoins :

uint16_t GetVersionSoft16(void);
uint32_t GetVersionSoft32(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

/******************************************************************************/
// ProductInfos : Name = "Himalaya2 Product" // "HII Carte Mere App Product"
// BusConfig : Type = "rtu", Baudrate = "115200", Data = "8", Parity = "none", Stop = "1", Port = "COM6"
// EquipmentCfg : Name = "HII_Mere_App_A035", Slave = "2"
// XmlConfig : varPrefix = "modbus_HII_Mere_App_", getPrefix = "get", setPrefix = "set", fnRead = "3", fnWrite = "16"

#pragma GCC diagnostic ignored "-Wcomment" // Pour ignorer les Multi-Line dans les commentaires (from "https://stackoverflow.com/questions/925179/selectively-remove-warning-message-gcc#3125889")

static const tModbusSlaveItem TableModbusSlave[] = {
//	Adresse,		RdMinLevel,			WrMinLevel,				VarType and Get/Set Method,		RdPtr,				WrPtr :

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT	// EXPORT = 1
/*
	// Identification :
	{ 0x01,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetVarSetVar},		&Fab_CodeSapProduct,	&Fab_CodeSapProduct},	// Code SAP du Produit complet (Name = "Code SAP Produit", \
	Enum = "11023471:VEX 40 T CLASSIC 3471/11023472:VEX 40 T PREMIUM 3472/11023473:INSPIRAIR TOP 300 CLASSIC 3473/11023474:INSPIRAIR TOP 300 PREMIUM 3474/11023475:INSPIRAIR TOP 450 CLASSIC 3475/\
			11023476:INSPIRAIR TOP 450 PREMIUM 3476/11023477:INSPIRAIR TOP 300 ERV 3477/11023478:INSPIRAIR TOP 450 ERV 3478/11027104:INSPIRAIR TOP 300 FRANCE 7104/11027106:INSPIRAIR TOP 450 PREMIUM ERV NA 7106/\
			11027107:INSPIRAIR TOP 40T CLASS ERV 7107/11027110:INSPIRAIR TOP 300 PREMIUM HY 7110/11027111:INSPIRAIR TOP 450 PREMIUM HY 7111/11027130:INSPIRAIR SIDE V2 S1 PRIMA 7130/\
			11027131:INSPIRAIR SIDE V2 S1 CLASSIC 7131/11027132:INSPIRAIR SIDE V2 S1 CLASSIC DHU 7132/11027133:INSPIRAIR SIDE V2 S1 CLASSIC ERV 7133/11027134:INSPIRAIR SIDE V2 S2 CLASSIC 7134/\
			11027135:INSPIRAIR SIDE V2 S2 CLASSIC DHU 7135/11027136:INSPIRAIR SIDE V2 S2 CLASSIC ERV 7136"/110236239:T.VEX 40 T CLASSIC/110236240:T.VEX 40 T PREMIUM/110236241:T.INSPIRAIR TOP 300 CLASSIC/\
			110236242:T.INSPIRAIR TOP 300 PREMIUM/110236243:T.INSPIRAIR TOP 450 CLASSIC/110236244:T.INSPIRAIR TOP 450 PREMIUM/110236245:T.INSPIRAIR TOP 300 ERV/110236246:T.INSPIRAIR TOP 450 ERV/65535:Non Configuré")
	{ 0x03,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongLongGetVarSetVar},	&Fab_SN_AldesProduct,	&Fab_SN_AldesProduct},	// SN du Produit ALDES (Name = "SN Produit complet")
	{ 0x07,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},		&Fab_DateTstMainCard,	&Fab_DateTstMainCard},	// Date dernier test sous traitant (Name = "Date Test Carte")
	{ 0x08,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetVarSetVar},		&Fab_CodeSapMainCard,	&Fab_CodeSapMainCard},	// Code SAP de la Carte Principale (Name = "Code SAP Carte", Enum = "11029476:Classic/11029475:Pression")
	{ 0x0A,		{{{ ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetVarSetVar},		&Fab_SN_ThisMainCard,	&Fab_SN_ThisMainCard},	// S/N de la Carte Pincipale (Name = "SN Carte Principale")
*/
	{ 0x0C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetVar},		GetVersionSoft16,		0},	// Version du Firmware sur 16bits (SHOW_HEX", DefVal = "B150")
//	{ 0x0D,		{{{	ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetVar},		GetSoftTargetId,		0},	// Target du Soft (Name = "TargetId", Enum = "0:Europe/1:Chine")

#if defined(VERSION_INFOS_VERSION_INFOS_H_) && defined(VI_SUPPORT_FW_CRC)	// EXPORT = 1
	{ 0x0E,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},		&FwComputedCRC32,		0},	// CRC du Soft (Name = "CRC Soft", SHOW_HEX)
#endif // VERSION_INFOS_VERSION_INFOS_H_ && VI_SUPPORT_FW_CRC

	{ 0x10,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getCurAccessLevel,		RegisterNewAccessPswd},	// Code d'accès aux Niveaux Proteges (Name = "UserLevel", DefVal = "0", \
	Enum = "0:Normal/1:Level 1/2:Level 2/3:Level 3/4:Level 4/5:Level 5/9781:Accès 1/5476:Configurator/32184:IHM Produit/2794:Banc ALDES/941:Carte nue")

//	{ 0x11,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetFct},		&Fab_MdbUserSlaveId,	SetModbusUserSlaveAdr},	// Nouvelle Adresse ModBus Client (Name = "ID Modbus User", DefVal = "2")
//	{ 0x12,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetFctSetVar},		0,						0},	// fonctionnalitées, type de fonctions disponibles
	{ 0x14,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		0,						SaveParamsOnOrder},	// Ordre Sauvegarde param (Name = "Save Params", Enum = "0:Idle/22577:Product Params/31028:Factory Params")
//	{ 0x15,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		GetNukubModelSizeFromParamsSets,	0},			// Name = "Model Size", Enum = "0:Inconnu/16:Nükub 300/17:Nükub 450/18:Nükub 150/19:Nükub 180/65535:Indéterminé", DefVal = "0"
// $16 -> $1F = non affectés (au 24/07/2020).

/*
	// Réglages Usine :
	{ 0x20,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetFctSetFct},		getProductConfigStatus,					handleNewProductConfig},	// Enum = "3:Product Complete/2:Config Ready/1:Product Ready/0:Board Ready/9545:Start Service" (9545 = 0x2549 = Start Service), DefVal = "0"
	{ 0x21,		{{{ ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetFctSetFct},		0,										handleQueryReInitFactory},	// Name = "ReInit Factory", Enum = "0:NoAction/18213:ConfigReset"Enum4Dbg="/38483:ProductReset"
	{ 0x22,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetVarSetFct},		&ProductId,								HandleParamsSetFromThisProductId},	// Réglage Type de Produit (=> Jeu de Paramètre, DefVal = "65535"), \
	Enum = "3471:VEX 40 T CLASSIC 11023471/3472:VEX 40 T PREMIUM 11023472/3473:INSPIRAIR TOP 300 CLASSIC 11023473/3474:INSPIRAIR TOP 300 PREMIUM 11023474/3475:INSPIRAIR TOP 450 CLASSIC 11023475/\
			3476:INSPIRAIR TOP 450 PREMIUM 11023476/3477:INSPIRAIR TOP 300 ERV 11023477/3478:INSPIRAIR TOP 450 ERV 11023478/7104:INSPIRAIR TOP 300 FRANCE 11027104/7106:INSPIRAIR TOP 450 PREMIUM ERV NA 11027106/\
			7107:INSPIRAIR TOP 40T CLASSIC ERV 11027107/7110:INSPIRAIR TOP 300 PREMIUM HY 11027110/7111:INSPIRAIR TOP 450 PREMIUM HY 11027111/7130:INSPIRAIR SIDE V2 S1 PRIMA 11027130/\
			7131:INSPIRAIR SIDE V2 S1 CLASSIC 11027131/7132:INSPIRAIR SIDE V2 S1 CLASSIC DHU 11027132/7133:INSPIRAIR SIDE V2 S1 CLASSIC ERV 11027133/7134:INSPIRAIR SIDE V2 S2 CLASSIC 11027134/\
			7135:INSPIRAIR SIDE V2 S2 CLASSIC DHU 11027135/7136:INSPIRAIR SIDE V2 S2 CLASSIC ERV 11027136"/36239:T.VEX 40 T CLASSIC/36240:T.VEX 40 T PREMIUM/36241:T.INSPIRAIR TOP 300 CLASSIC/\
			36242:T.INSPIRAIR TOP 300 PREMIUM/36243:T.INSPIRAIR TOP 450 CLASSIC/36244:T.INSPIRAIR TOP 450 PREMIUM/36245:T.INSPIRAIR TOP 300 ERV/36246:T.INSPIRAIR TOP 450 ERV/65535:Non Configuré"
	{ 0x23,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetFct}, 	&TableMV_Custom[0].idCmdReel,			SetNukubFanCmdTable4CurParamsSets}, // Name = "FanCmdTable", GetNukubFanCmdTableFromParamsSets \
	Enum = "1:Table 300_1/2:Table 450/3:Table 150/4:Table 180/5:Table 300_5/0:Table Custom/129:Internal 300_1/130:Internal 450/131:Internal 150/132:Internal 180/133:Internal 300_5"Enum4Dbg="/161:RstPrdId" (161 = Reset ProductId)
	{ 0x24,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultFilterInsFromParamsSets,	0},			// Name = "DefaultFilterInsufflation", Enum = "1:Poussières/2:Pollens/3:Particules/4:Particules Fines/5:Bactéries/6:COV/0:Indéterminé"
	{ 0x25,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultFlags1FromParamsSets,	0},			// Name = "HR_LED_IHM_BYP Flags", \
	Enum = "1:HR Requise/2:Led RGB Autorisée/4:IHM Requise/3:HR + Led RGB/5:HR + IHM requises/6:Led + IHM/7:HR + Led RGB + IHM/8:Bypass inversé/0:Aucun Flag/\
			9:HR Req+Byp Inv/10:Led RGB+Byp Inv/11:HR+Led RGB+Byp Inv/12:IHM Req+Byp Inv/13:HR+IHM+Byp Inv/14:Led+IHM+Byp Inv/15:HR+Led+IHM+Byp Inv"
	{ 0x26,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDebitMinFromParamsSets,			0},			// Name = "Débit Min", Unit = "m³/h"
	{ 0x27,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDebitMaxFromParamsSets,			0},			// Name = "Débit Max", Unit = "m³/h"
	{ 0x28,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDefaultDebit0FromParamsSets,	0},			// Name = "Débit L0 par défaut", Unit = "m³/h"
	{ 0x29,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDefaultDebit1FromParamsSets,	0},			// Name = "Débit L1 par défaut", Unit = "m³/h"
	{ 0x2A,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDefaultDebit2FromParamsSets,	0},			// Name = "Débit L2 par défaut", Unit = "m³/h"
	{ 0x2B,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getNukubDefaultDebit3FromParamsSets,	0},			// Name = "Débit L3 par défaut", Unit = "m³/h"
	{ 0x2C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultLngIdFromParamsSets,		0},			// Name = "Default Language", Enum = "1:Français/2:Anglais/3:Allemand/4:Espagnol/5:Italien/6:Néerlandais/7:Danois/0:Inconnu"
	{ 0x2D,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultCountryIdFromParamsSets,	0},			// Name = "Default Country", Enum = "1:France/2:Angleterre/3:Allemagne/4:Espagne/5:Italie/6:Benelux/7:Danemark/10:Nord-Amérique/15:Chine/0:Inconnu"
	// 0x2E ... 0x2F = RESERVED
	{ 0x30,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultTimer0FromParamsSets,	0},			// Name = "Default Timer L0", Unit = "jours"
	{ 0x31,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultTimer2FromParamsSets,	0},			// Name = "Default Timer L2", Unit = "mn"
	{ 0x32,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultTimer3FromParamsSets,	0},			// Name = "Default Timer L3", Unit = "heures"
	// $33 = non affecté (au 08/11/2023).
	{ 0x34,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultIhmUnitTemperature,  	0},			// Name = "Default IHM Unit Temperature", Enum = "0:°C/1:°F" (Ajout_Jp le 08/11/2023 pour Ticket #27)
	{ 0x35,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultIhmUnitFlow, 			0},			// Name = "Default IHM Unit Flow", Enum = "0:m³_h/1:L_s/2:CFM" (Ajout_Jp le 08/11/2023 pour Ticket #27)
	{ 0x36,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultIhmUnitPressure, 		0},			// Name = "Default IHM Unit Pressure", Enum = "0:Pa/1:Po.H2O" (Ajout_Jp le 08/11/2023 pour cohérence avec le Ticket #27)
	{ 0x37,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		getNukubDefaultIhmUnitSpeed, 			0},			// Name = "Default IHM Unit Speed", Enum = "0:en %/1:RPM" (Ajout_Jp le 08/11/2023 pour cohérence avec le Ticket #27)
	// $38 -> $FF = non affectés (au 08/11/2023).

	// ModbusUser Params : // Ajout_Jp le 16/04/2024 pour Ticket #33 :
	{ 0x40, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetVarSetVar},		&Uart3ReInitUserParams.BaudRateDiv100,	&Uart3ReInitUserParams.BaudRateDiv100},	// Parmi "eUartReInitBaudRate", Name = "UserBaudRate", Unit = "bauds", Coef = "x100", \
	Enum = "96:9600 bauds/0:ResetDefault/48:4800 bauds/192:19200 bauds/384:38400 bauds/576:57600 bauds/1152:115200 bauds",2304:230400 bauds/4608:460800 bauds", DefVal = "96:9600 bauds"
	{ 0x41, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUCharGetVarSetVar},		&Uart3ReInitUserParams.Parity,			&Uart3ReInitUserParams.Parity},	// Parmi "eUartReInitParity", Name = "UserParity", Enum = "0:None/1:Odd/2:Even/255:ResetDefault", DefVal = "0:None"
	{ 0x42, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUCharGetVarSetFct},		&Uart3ReInitUserParams.nbStops,		 	HandleNewUart3Stops}, // Parmi "eUartReInitStop", Name = "UserStops", Enum = "1:One/2:Two/0:ResetDefault", DefVal = "1:One"
	{ 0x43, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetVarSetFct},		&Uart3ReInitCoreVars.SabApply,			HandleValidateNewUart3Params},	// Name = "SabApplyUartChange", Unit = "s", Coef = "10" (1 <=> 0.1s), Enum = "0:NoChange/23130:ValidateChange"

	{ 0x01,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetFctSetFct},		INF_GetSapProduct,		INF_WriteSapProduct}, // Code SAP du Produit complet (Name = "Code SAP Produit")
	{ 0x03,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongLongGetFctSetFct},	INF_GetSnProduct,		INF_WriteSnProduct},	// SN du Produit ALDES (Name = "SN du Produit")
	{ 0x07,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},		INF_GetTestDate,		INF_WriteTestDate},	// Date dernier test sous traitant (Name = "Date Test Carte")
	{ 0x08,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetFctSetFct},		INF_GetSapBoard,		INF_WriteSapBoard},	// Code SAP de la Carte Principale (Name = "Code SAP Carte")
	{ 0x0A,		{{{ ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetFctSetFct},		INF_GetSnBoard,			INF_WriteSnBoard},	// S/N de la Carte Pincipale (Name = "SN Carte Principale")
	{ 0x0C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		GetVersionSoft16,		0},	// Version du Firmware (SHOW_HEX) Modif_Jp le 03/08/2023 pour Ticket #148
*/

/*
	// Fonctionnement GTB :
	{ 0x100,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getGestionModeVentilateur,				setGestionModeVentilateur},					// A partir de A10F : Name = "Mode Régul", Enum = "0:Débit/1:Hygro/2:Vitesse", MinVal = "0", MaxVal = "2", DefVal = "0"
	{ 0x101,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarSCharGetFctSetFct},		getActionUser,							setActionUser},								// Name = "Demande User", Enum = "1:Quotidien/2:Boost/3:Invités/0:Vacances/255:Ignorer/4:MaxSpeed(DK)/65535:Transparent", MinVal = "0", MaxVal = "4", DefVal = "1"
//	{ 0x102,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getSystemModeConsigneGestionRegulation,	setSystemModeConsigneGestionRegulation},	// Name = "Demande Programmateur", Enum = "1:Quotidien/2:Boost/3:Invités/0:Vacances/4:MaxSpeed(DK)", MinVal = "0", MaxVal = "4", DefVal = "1"
	{ 0x102,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getAirProgLevel,						HandleNewAirProgLevel},						// Name = "Demande Programmateur", Enum = "1:Quotidien/2:Boost/3:Invités/0:Vacances/4:MaxSpeed(DK)/65535:Transparent", MinVal = "0", MaxVal = "4", DefVal = "1"
	{ 0x103,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},	TVarUCharGetFctSetVar},		getCurrentModeBypass,					0},	// A partir de A10F : Name = "Mode Bypass", Enum = "0:Manuel/1:Auto"
//	{ 0x104,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUIntGetFctSetFct},		0,										0},	// Méthode de Régulation : Europe / France -> non configurable sur Nükub !
	{ 0x105,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUCharGetVarSetFct},		&gestion_A0_0_10v.currentCapteurActif,	setCurrentCapteurActifA0_0_10V}, // Type de Capteur 0-10V sur AI0 : Name = "Type 0-10V AI0", Enum = "0:Co2 Aldes/2:Générique Proportionnel/1:PM 2.5 VOC China", MinVal = "0", MaxVal = "2", DefVal = "2"
	{ 0x106,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarUCharGetVarSetFct},		&gestion_A1_0_10v.currentCapteurActif,	setCurrentCapteurActifA1_0_10V}, // Type de Capteur 0-10V sur AI1 : Name = "Type 0-10V AI1", Enum = "0:Co2 Aldes/2:Générique Proportionnel/1:PM 2.5 VOC China", MinVal = "0", MaxVal = "2", DefVal = "2"

	// Réglages Usine :
	{ 0x20,		{{{	ACCESS_MIN_LEVEL_MAX,ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		0,								requestToSwitchToFactoryState},	// Factory license : l'écriture de 147 provoque un passage en FACTORY_STATE (Enum = "0:NoChange/147:FACTORY_STATE")
	{ 0x21,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		MSM_readSoftState,				0},	// Etat logiciel : Enum = "0:Boot/1:Board ready/2:Board SAV ready/3:Product ready/4:Product complete/5:Factory state"
	{ 0x22,		{{{	ACCESS_MIN_LEVEL_MAX,ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		0,								requestProductReset},	// Product reset: écrire 242 en FactoryState efface en mémoire les codes produit. Au prochain démarrage, reinit + retour en BOARD_READY (Enum = "0:NoChange/242:Reset Product")
	{ 0x23,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		INF_GetProductId,				INF_WriteProductId},	// product ID. Ecriture possible uniquement en mode factory state ou board SAV ready
	{ 0x24,		{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetFctSetFct},		INF_GetisFirstCommissionning,	INF_WriteisFirstCommissionning},	// Booléen pour savoir si c'est la première mise en service du produit. Ecriture possible uniquement en mode factory state, board ready ou board complete  (Enum = "0:False/1:True")

#ifdef RTC_DATETIME32_SUPPORT_ENABLED
	// DateTime RTC :
	{ 0xF0,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarULongGetFctSetFct},		getCurDateTime32,			HandleNewDateTime32FromModbus}, // Name = "DateTime32", Unit = "s"
	{ 0xF2,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetVarSetVar},		&curRTC_DateTime.Year,		&curRTC_DateTime.Year},		// Name = "Date_Year", MinVal = "2020"
	{ 0xF3,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Month,		&curRTC_DateTime.Month},	// Name = "Date_Month", MinVal = "1", MaxVal = "12", DefVal = "1", Enum = "1:Janvier/2:Février/3:Mars/4:Avril/5:Mai/6:Juin/7:Juillet/8:Août/9:Septembre/10:Octobre/11:Novembre/12:Décembre"
	{ 0xF4,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Date,		&curRTC_DateTime.Date},		// Name = "Date_Day", MinVal = "1", MaxVal = "31", DefVal = "1"
	{ 0xF5,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Weekday,	&curRTC_DateTime.Weekday},	// Name = "Date_WeekDay", Enum = "0:Lundi/1:Mardi/2:Mercredi/3:Jeudi/4:Vendredi/5:Samedi/6:Dimanche", MinVal = "0", MaxVal = "6", DefVal = "0"
	{ 0xF6,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Hours,		&curRTC_DateTime.Hours},	// Name = "Time_Hours", Unit = "h", MinVal = "0", MaxVal = "23", DefVal = "0"
	{ 0xF7,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Minutes,	&curRTC_DateTime.Minutes},	// Name = "Time_Minuts" Unit = "m", MinVal = "0", MaxVal = "59", DefVal = "0"
	{ 0xF8,		{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetFct},		&curRTC_DateTime.Seconds,	HandleNewDateTimeSeconds},	// Name = "Time_Seconds", Unit = "s", MinVal = "0", MaxVal = "59", DefVal = "0"
#endif // RTC_DATETIME32_SUPPORT_ENABLED
	{ 0xF9, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},		&mRtcRv3028.base.idOfDevice,	0},	// Name = "RTC.RV3028", Enum = "0:Not Loaded/17:RV3028/32785:Try RV3028"

	// IHM :
	{ 0x100,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		0,								requestConfigReset},	// config reset: écrire 41 pour passer de PRODUCT COMPLETE a PRODUCT READY  (Enum = "0:NoChange/41:PRODUCT_READY")
	{ 0x101,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		0,								requestSAVreset},		// Enum = "0:NoChange/186:SAV_Ready"
	{ 0x102,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_bst_ktch_req_stt,	&inData.Ss_bst_ktch_req_stt},	// Enum = "0:no Request/1:boost required/2:boost stop"
	{ 0x103,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_fnc_rcpt_san_req,	&inData.Ss_fnc_rcpt_san_req},	// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x104,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_fnc_rcpt_ktch_req,	&inData.Ss_fnc_rcpt_ktch_req},	// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x105,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_netw_leak_req,		&inData.Ss_netw_leak_req},		// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x106,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_flw_brch_req,		&inData.Ss_flw_brch_req},		// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x107,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_chk_max_flow_req,	&inData.Ss_chk_max_flow_req},	// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x108,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ns_mnta_flw_brch_act,	&inData.Ns_mnta_flw_brch_act},	// Numero de la manta. Par defaut à 1
	//{ 0x109,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getBs_from_hmi_rst,				setBs_from_hmi_rst},					// Enum = "0:no Request/1:RequestOn/2:RequestOff"
	{ 0x10A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		INF_GetnightCoolingEnable,	INF_WritenightCoolingEnable},	// Enum = "10:RequestOn/20:RequestOff"
	{ 0x10B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_init_test_mode_req,	&inData.Ss_init_test_mode_req},	// Enum = "0:no Request/1:RequestOn"
	{ 0x10C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&inData.Ss_chk_test_mode_req,	&inData.Ss_chk_test_mode_req},	// Enum = "0:no Request/1:RequestOn"
	{ 0x10D,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.RcptFnc.Ss_rcpt_san_flow_ctrl_stt, 0}, // Enum = "0:empty/10:Transitoire/20:Steady"
	{ 0x10E,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.RcptFnc.Ss_rcpt_ktch_flow_ctrl_stt, 0}, // Enum = "0:empty/10:Transitoire/20:Steady"
	{ 0x10F,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.Ss_netw_leak_reg_stt, 0}, // Enum = "0:empty/10:Transitoire/20:Steady"
	{ 0x110,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[0], 0}, // Enum = "1:OK/2:NOK"
	{ 0x111,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[1], 0}, // Enum = "1:OK/2:NOK"
	{ 0x112,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[2], 0}, // Enum = "1:OK/2:NOK"
	{ 0x113,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[3], 0}, // Enum = "1:OK/2:NOK"
	{ 0x114,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[4], 0}, // Enum = "1:OK/2:NOK"
	{ 0x115,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[5], 0}, // Enum = "1:OK/2:NOK"
	{ 0x116,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[6], 0}, // Enum = "1:OK/2:NOK"
	{ 0x117,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.flowBranchVerification.Ss_flw_brch_reg_stt, 0}, // Enum = "0:empty/10:Transitoire/20:Steady"
	{ 0x118,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.flowBranchVerification.Ss_flw_brch_cfrm, 0}, // Enum = "1:OK/2:NOK"
	{ 0x119,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.checkingFanMaxFlowRate.Ss_chk_max_flow_reg_stt, 0}, // Enum = "0:empty/10:Transitoire/20:Steady"
	{ 0x11A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.AutoDiag.checkingFanMaxFlowRate.Ss_chk_max_flow_cfrm, 0}, // Enum = "1:OK/2:NOK"
	{ 0x11B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.TestMode.Ss_test_mode_cfrm, 0}, // Enum = "1:OK/2:NOK"
	{ 0x11C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getInputSecuBs_orng_led, 0}, // Enum = "0:Off/1:On"
	{ 0x11D,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getInputSecuBs_red_led, 0}, // Enum = "0:Off/1:On"
	{ 0x11E,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		getRemote1IAQLevel, 0}, // Enum = 0:E_GOOD_IAQ / 1:E_MEDIUM_IAQ / 2:E_BAD_IAQ
	{ 0x11F,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		getRemote2IAQLevel, 0}, // Enum = 0:E_GOOD_IAQ / 1:E_MEDIUM_IAQ / 2:E_BAD_IAQ
	{ 0x120,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		getRemote3IAQLevel, 0}, // Enum = 0:E_GOOD_IAQ / 1:E_MEDIUM_IAQ / 2:E_BAD_IAQ
	{ 0x121,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getInputSecuBs_maint_tout, 0}, // Maintenance à faire
	{ 0x122,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&outData.FcnArb.Ss_hmi_op_mod, 0}, // Enum = "1:Ss_hmi_op_mod_qai_C/2:Ss_hmi_op_mod_boost_C/3:Ss_hmi_op_mod_nc_C/4:Ss_hmi_op_mod_diag_C"
	{ 0x123,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getFlwSpCalcBs_noc_cool_ena, setFlwSpCalcBs_noc_cool_ena}, //
	{ 0x124,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		INF_GetlastMaintenanceDate0, 0}, //
	{ 0x125,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		INF_GetlastMaintenanceDate1, 0}, //
	{ 0x126,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		INF_GetlastMaintenanceDate2, 0}, //
	{ 0x127,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},		getBs_maint_done, setBs_maint_done}, // Correction_Jp le 29/11/2023 : macro returns uint8_t; Maintenance effectuée
	{ 0x128,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[7], 0}, // Enum = "1:OK/2:NOK"
	{ 0x129,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[8], 0}, // Enum = "1:OK/2:NOK"
	{ 0x12A,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[9], 0}, // Enum = "1:OK/2:NOK"
	{ 0x12B,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[10], 0}, // Enum = "1:OK/2:NOK"
	{ 0x12C,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[11], 0}, // Enum = "1:OK/2:NOK"
	{ 0x12D,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUCharGetVarSetVar},     &outData.AutoDiag.testNetworkLeakage.St_netw_leak_cfrm[12], 0}, // Enum = "1:OK/2:NOK"
	{ 0x12E,    {{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},   TVarUIntGetFctSetFct},      getGlobalIAQLevel, 0}, // Enum = 0:E_GOOD_IAQ / 1:E_MEDIUM_IAQ / 2:E_BAD_IAQ

	// Paramètres de Config France :
	{ 0x128,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.TypeLogement,	&HRVK_ConfigFR.TypeLogement},	// MinVal = "2", MaxVal = "6", DefVal = "2", Type de Logement : Name = "Type Logement"
	{ 0x129,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbBain,			&HRVK_ConfigFR.nbBain},			// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Bains : Name ="nb SDB"
	{ 0x12A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbWC,			&HRVK_ConfigFR.nbWC},			// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Wc : Name = "nb WC"
	{ 0x12B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbCellier,		&HRVK_ConfigFR.nbCellier},		// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Cellier : Name = "nb Pièces Eau"
// $12C -> $12F = non affectés (au 24/07/2020).


	// Commande Ventilateurs :
	{ 0x140,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		fanVoltage_mV,		requestFanVoltage_mV},	// Unit = "V", Coef = "1000", MinVal = "0", MaxVal = "10"
//	{ 0x141,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		getFanSupplyVoltage_mV, 		setFanSupplyVoltage_mV},	// Unit = "V", Coef = "1000", MinVal = "0", MaxVal = "10"

	// Retours Ventilateurs :
	{ 0x162,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanFeedbackSpeed,			0},	// Vitesse Extraction (Unit = "RPM")
	{ 0x163,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanLastFeedbackSpeed,		0},	// Temporaire non moyennée Extraction (Unit = "RPM")
	{ 0x164,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanLastDeltaTime,			0},	// Temporaire deltaTime Extraction

//	{ 0x16A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyFeedbackSpeed,			0},	// Vitesse Extraction (Unit = "RPM")
//	{ 0x16B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyLastFeedbackSpeed,		0},	// Temporaire non moyennée Extraction (Unit = "RPM")
//	{ 0x16C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyLastDeltaTime,			0},	// Temporaire deltaTime Extraction

	// Codes Erreur :
	{ 0x180,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getError,	0},	// erreur la plus  prioritaire
	{ 0x181,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getMantaError, 0},  // 1ere Manta en erreur trouvé

	// Données pour banc de test :
	{ 0x190,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int0, byPassMantaCo2Int0},  // force la valeur de co2 de la manta
	{ 0x191,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int1, byPassMantaCo2Int1},  // force la valeur de co2 de la manta
	{ 0x192,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int2, byPassMantaCo2Int2},  // force la valeur de co2 de la manta
	{ 0x193,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int3, byPassMantaCo2Int3},  // force la valeur de co2 de la manta
	{ 0x194,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int4, byPassMantaCo2Int4},  // force la valeur de co2 de la manta
	{ 0x195,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int5, byPassMantaCo2Int5},  // force la valeur de co2 de la manta
	{ 0x196,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Int6, byPassMantaCo2Int6},  // force la valeur de co2 de la manta
	{ 0x197,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext0, byPassMantaCo2Ext0},  // force la valeur de co2 de la manta
    { 0x198,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext1, byPassMantaCo2Ext1},  // force la valeur de co2 de la manta
    { 0x199,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext2, byPassMantaCo2Ext2},  // force la valeur de co2 de la manta
    { 0x19A,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext3, byPassMantaCo2Ext3},  // force la valeur de co2 de la manta
    { 0x19B,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext4, byPassMantaCo2Ext4},  // force la valeur de co2 de la manta
    { 0x19C,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext5, byPassMantaCo2Ext5},  // force la valeur de co2 de la manta
    { 0x19D,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},      read_byPassMantaCo2Ext6, byPassMantaCo2Ext6},  // force la valeur de co2 de la manta
    { 0x19E,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[0].raw, &bypassMantaErrorInt[0].raw},  // force les flag d'erreur de la manta
    { 0x19F,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[1].raw, &bypassMantaErrorInt[1].raw},  // force les flag d'erreur de la manta
    { 0x1A0,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[2].raw, &bypassMantaErrorInt[2].raw},  // force les flag d'erreur de la manta
    { 0x1A1,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[3].raw, &bypassMantaErrorInt[3].raw},  // force les flag d'erreur de la manta
    { 0x1A2,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[4].raw, &bypassMantaErrorInt[4].raw},  // force les flag d'erreur de la manta
    { 0x1A3,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[5].raw, &bypassMantaErrorInt[5].raw},  // force les flag d'erreur de la manta
    { 0x1A4,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorInt[6].raw, &bypassMantaErrorInt[6].raw},  // force les flag d'erreur de la manta
    { 0x1A5,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[0].raw, &bypassMantaErrorExt[0].raw},  // force les flag d'erreur de la manta
    { 0x1A6,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[1].raw, &bypassMantaErrorExt[1].raw},  // force les flag d'erreur de la manta
    { 0x1A7,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[2].raw, &bypassMantaErrorExt[2].raw},  // force les flag d'erreur de la manta
    { 0x1A8,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[3].raw, &bypassMantaErrorExt[3].raw},  // force les flag d'erreur de la manta
    { 0x1A9,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[4].raw, &bypassMantaErrorExt[4].raw},  // force les flag d'erreur de la manta
    { 0x1AA,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[5].raw, &bypassMantaErrorExt[5].raw},  // force les flag d'erreur de la manta
    { 0x1AB,    {{{ ACCESS_MIN_LEVEL_5, ACCESS_MIN_LEVEL_5}},   TVarUIntGetVarSetVar},      &bypassMantaErrorExt[6].raw, &bypassMantaErrorExt[6].raw},  // force les flag d'erreur de la manta

    {   0x120E,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getCurrentCapteurActifA0_0_10V,                         setCurrentCapteurActifA0_0_10V}, // Type de Capteur 0-10V n°1 : Name = "Type 0-10V #1", Enum = "0:Aldes Co2/2:Générique Proportionnel/1:PM 2.5 VOC China"
    {   0x120F,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getCurrentCapteurActifA1_0_10V,                         setCurrentCapteurActifA1_0_10V}, // Type de Capteur 0-10V n°2 : Name = "Type 0-10V #2", Enum = "0:Aldes Co2/2:Générique Proportionnel/1:PM 2.5 VOC China"


    // Push Button :
// Voir si nécessaire :	{   0x1250,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getStartTimerPushButton,                        0},
// Voir si nécessaire :	{   0x1251,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getPushButtonClosedLongTime,                    0},
// Voir si nécessaire :	{   0x1252,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getPushButtonConsigneMode,                      0},
// Voir si nécessaire :	{   0x1253,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getPushButtonConsigneVitesse_MVE,               0},
// Voir si nécessaire :	{   0x1254,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getPushButtonConsigneVitesse_MVI,               0},
    {   0x1255,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getCurrentPushButtonPressed,                    0},	// Pour Watts : Enum = "0:NotPressed/1:Pressed"

// A priori pas nécessaire ici ...
    //gestion ventilateur
    {   0x1300,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getGestionModeVentilateur,                      setGestionModeVentilateur},		// A partir de A10F : Name = "Mode Régul", Enum = "0:Débit/1:Hygro/2:Vitesse"
//    {   0x1300,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getGestionModeVentilateur,                      setGestionModeVentilateur},		// Enum = "0:DEBIT/1:VITESSE/2:HYGRO"
    {   0x1301,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},         getUnbalanceCoefMVI,                            setGestionParamExtUnbalanceCoef},

    {   0x1302,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitTimeStepRegulMV,                    setModeDebitTimeStepRegulMV},	// Unit = "ms"
    {   0x1303,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitBornePrecision,                     setModeDebitBornePrecision},
    {   0x1304,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitInfHysteresisBorne,                 setModeDebitInfHysteresisBorne}, // Unit = "m³/h"
    {   0x1305,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitSupHysteresisBorne,                 setModeDebitSupHysteresisBorne}, // Unit = "m³/h"
    {   0x1306,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitKI_low,                             setModeDebitKI_low},			 // Unit = "m³/h"
    {   0x1307,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeDebitKI_high,                            setModeDebitKI_high},			 // Unit = "m³/h"

    {   0x1308,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseTimeStepRegulMV,                  setModeVitesseTimeStepRegulMV},	// Unit = "ms"
    {   0x1309,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseBornePrecision,                   setModeVitesseBornePrecision},
    {   0x130A,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseKI_low,                           setModeVitesseKI_low},			// Unit = "mV"
    {   0x130B,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseKI_high,                          setModeVitesseKI_high},			// Unit = "mV"
    {   0x130C,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseSupHysteresisBorne,               setModeVitesseSupHysteresisBorne}, // Unit = "RPM"
    {   0x130D,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeVitesseInfHysteresisBorne,               setModeVitesseInfHysteresisBorne}, // Unit = "RPM"

    {   0x130E,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroTimeStepRegulMV,                    setModeHygroTimeStepRegulMV},	// Unit = "ms"
    {   0x130F,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroBornePrecision,                     setModeHygroBornePrecision},
    {   0x1310,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroKI_low,                             setModeHygroKI_low},			// Unit = "m³/h"
    {   0x1311,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroKI_high,                            setModeHygroKI_high},			// Unit = "m³/h"

    {   0x1312,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getTensionFinalMVE,                             0},	// Unit = "V" Coef = "1000"
    {   0x1313,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getTensionFinalMVI,                             0},	// Unit = "V" Coef = "1000"
    {   0x1314,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},         getUnbalanceCoefMVI,                            setUnbalanceCoefMVI}, // Unit = "%"
    {   0x1315,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getCurrentConsigneMVE,                          0},
    {   0x1316,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getCurrentConsigneMVI,                          0},

    {   0x1317,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetVar},         getErrorDeltaMVE,                               0},
    {   0x1318,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetVar},         getErrorDeltaMVI,                               0},
    {   0x1319,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getHystRegulMVE,                                0},
    {   0x131A,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getHystRegulMVI,                                0},
    {   0x131B,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroSupHysteresisBorne,                 setModeHygroSupHysteresisBorne},
    {   0x131C,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getModeHygroInfHysteresisBorne,                 setModeHygroInfHysteresisBorne},

    //gestion bypass
    {   0x1350,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getBypassCurrentSaison,                        setBypassCurrentSaison},	// A partir A10F : Enum = "0:Inconnu/1:Hiver/2:Eté"
//    {   0x1350,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getBypassCurrentSaison,                        0},	// Enum = "0:SPRING/1:SUMMER/2:AUTOMN/3:WINTER"
    {   0x1351,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getCurrentModeBypass,                          0},	// A partir A10F : Enum = "0:Manuel/1:Auto"
//    {   0x1351,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getCurrentModeBypass,                          0},	// Enum = "0:Auto/1:Manuel"
    {   0x1352,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},          getBypassTSummerComfort,                       setBypassTSummerComfort},	// Unit = "°c" Coef = "100"

    {   0x1353,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getTimeRegulationBypass45min,                  setTimeRegulationBypass45min}, // Unit = "min"
    {   0x1354,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetVar},          getSaisonMinAvailableValue,                    0},	// Unit = "°c" Coef = "100"
    {   0x1355,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetVar},          getSaisonMaxAvailableValue,                    0},	// Unit = "°c" Coef = "100"
    {   0x1356,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetVar},          getSaisonMeanAvailableValue,                   0},	// Unit = "°c" Coef = "100"
    {   0x1357,	  {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},			 getBypassCurrentPosition,						0}, // Enum = "0:Indéfini/1:Ouvert/2:45°/3:Fermé"
    {   0x1358,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getActionBypassPosition,                       setActionBypassPosition}, // Enum = "0:Indéfini/1:Ouvert/2:45°/3:Fermé"
	{   0x1359,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getBypassConsigneTrajetAir,					setBypassConsigneTrajetAir}, // Enum = "0:PassageEchangeur/1:PassageBypass"
	{   0x135A,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getBypassCurMoveStepId,						setBypassCurMoveStepId}, // Enum = "0:WaitMonitoring/1:RequestPreavis/2:WaitingPreavis/3:ActionBypass/4:Bypass_45/5:WaitingFinish/6:WaitingStable/7:ActionFinish"
	{   0x135B,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},			 getBypassCurSabMoveDelay,						setBypassCurSabMoveDelay}, // Unit = "s"


// Temporairement réactivé le 12/09/2020 pour les essais de FB
	// -> Attention : GestionDegivrageModeDIBTAllowed "Certification" (0x1400) est maintenant nécessaire !

	// Commandes vers Bus[1] -> Broadcast Manta :
	{0x8C2,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mMantaHub[1].Manta[0].hwAdress,	&mMantaHub[1].Manta[0].hwAdress},
	{0x8C3,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mMantaHub[1].Manta[0].initReq,		&mMantaHub[1].Manta[0].initReq},
	{0x8C4,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mMantaHub[1].Manta[0].mode,		&mMantaHub[1].Manta[0].mode},
	{0x8C5,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&mMantaHub[1].Manta[0].consigne,	&mMantaHub[1].Manta[0].consigne},

    {   0x1407,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarUCharGetFctSetFct},        getGestionDegivrageHystPreheater,                      setGestionDegivrageHystPreheater},	// Enum = "0:Repos/1:Activé", DefVal = "0"
    {   0x1408,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getGestionDegivrageErreurPreheater,                    setGestionDegivrageErreurPreheater},	// DefVal = "0"
    {   0x1409,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getGestionDegivrageSommeErreurPreheater,               setGestionDegivrageSommeErreurPreheater}, // DefVal = "0"
    {   0x140A,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getGestionDegivrageKpPreheater,                        setGestionDegivrageKpPreheater},		// Coef = "100", DefVal = "-0.01"
    {   0x140B,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getGestionDegivrageKiPreheater,                        setGestionDegivrageKiPreheater},		// Coef = "100", DefVal = "-0.01"
    {   0x140C,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getGestionDegivrageConsignePreheater,                  setGestionDegivrageConsignePreheater}, // Unit = "°c", Coef = "100", DefVal = "-3"
    {   0x140D,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarUIntGetFctSetVar},         getGestionDegivrageSablierDegivrage,                   0},									// Unit = "s", DefVal = "0"
    {   0x140E,   {{{ ACCESS_MIN_LEVEL_3, ACCESS_MIN_LEVEL_4}},  TVarSIntGetFctSetFct},         getConsignePreheater_BCAIBus,                          setConsignePreheater_BCAIBus},		// Unit = "°c", Coef = "100"

    {   0x140F,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getNumSoft_BCA_Ibus,                                0},
    {   0x1410,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getBcaFlagsStatus,                                  0},
    {   0x1411,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getBca_SWB,                                         0},
    {   0x1412,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getMesureCtn,                                       0},
    {   0x1413,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getPuissanceSortie,                                 0},
    {   0x1414,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getFault,                                           0},

    {   0x1415,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getDefrostConsigneMVI_Finale,                       0},
    {   0x1416,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getDefrostConsigneMVE_Finale,                       0},

    {   0x1417,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},          get_limit_temp_3,                           set_limit_temp_3},
    {   0x1418,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarSIntGetFctSetFct},          get_limit_temp_minus_2,                     set_limit_temp_minus_2},

    {   0x1419,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetFct},         getDefrostStateDatas,                       0},// ajout MF le 15/04/24 -> ticket #34, \
	Champ de bits : Enum = "1:stateIntBcaPreheating/2:stateIntBcaPostheating/4:stateExtBcaPretheating/8:stateExtBcaPostheating/16:modeSafeActif/32:cycleDegivrageActif/0:None", DefVal = "0"

    // Output PWM :
    {   0x1500,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_4}},  TVarUIntGetFctSetFct},          getOutputPwmValue,                         setOutputPwmValue}, // Pour Watts : Unit = "V", Coef = "100", MinVal = "0", MaxVal = "10", DefVal = "0"


    // Gestion Filtre :
	{ 0x3000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},	getBesoinChangementFiltreActif,				0},
	{ 0x3001,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},	getLimiteTimeChangementFiltre,				setLimiteTimeChangementFiltre},
	{ 0x3002,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},	getTimeRegulationFiltre1h,					setTimeRegulationFiltre1h},
	{ 0x3003,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetFct},	0,											setUserActionFiltreChange},

*/

	// code installation lue par l'ihm
	{ 0x9C54,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetVar},	&InstallCodePin,				0},				// Code Installateur

	// Commandes Test ModbusUserMode :
#ifdef MODBUS_SLAVE_ENABLE_STATS_ACCESS	// EXPORT = 0
#warning "Commandes Test & Stats ModbusSlave actives !!!"
//	{ 14000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},		&ModbusUserCurMode,		&ModbusUserCurMode},
//	{ 14001,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},		&ModbusUserNewMode,		&ModbusUserNewMode},

    // Statistiques Modbus 0 :
#if defined(MODBUS_SLAVE_SUPPORT_STATS) && defined(MODBUS_SLAVE_UART_IHM_PARAMS)	// EXPORT = 0
	{ 15000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRx,		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRx},
	{ 15004,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxCrcOk,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxCrcOk},
	{ 15008,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxCrcErr,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxCrcErr},
	{ 15012,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxAdrOk,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxAdrOk},
	{ 15016,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxAdrErr,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRxAdrErr},
	{ 15020,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbTramesTx,		MODBUS_SLAVE_UART_IHM_PARAMS.nbTramesTx},
	// Répartition par Handler :
	{ 15024,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRead03,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesRead03},
	{ 15028,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesWrit16,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesWrit16},
	{ 15032,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesNoFnHand,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesNoFnHand},
	{ 15036,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesTxExcept,	MODBUS_SLAVE_UART_IHM_PARAMS.nbFramesTxExcept},
#endif // MODBUS_SLAVE_SUPPORT_STATS && MODBUS_SLAVE_UART_IHM_PARAMS

    // Statistiques Modbus 1 :
#if defined(MODBUS_SLAVE_SUPPORT_STATS) && defined(MODBUS_SLAVE_UART_USER_PARAMS)	// EXPORT = 0
	{ 15100,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRx,		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRx},
	{ 15104,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxCrcOk,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxCrcOk},
	{ 15108,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxCrcErr,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxCrcErr},
	{ 15112,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxAdrOk,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxAdrOk},
	{ 15116,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxAdrErr,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRxAdrErr},
	{ 15120,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbTramesTx,		MODBUS_SLAVE_UART_USER_PARAMS.nbTramesTx},
	// Répartition par Handler :
	{ 15124,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRead03,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesRead03},
	{ 15128,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesWrit16,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesWrit16},
	{ 15132,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesNoFnHand,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesNoFnHand},
	{ 15136,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_UART_USER_PARAMS.nbFramesTxExcept,	MODBUS_SLAVE_UART_USER_PARAMS.nbFramesTxExcept},
#endif // MODBUS_SLAVE_SUPPORT_STATS && MODBUS_SLAVE_UART_USER_PARAMS

    // Statistiques Modbus USBD_CDC 0 :
#if defined(MODBUS_SLAVE_SUPPORT_STATS) && defined(MODBUS_SLAVE_USBD_CDC_PARAMS)	// EXPORT = 0
	{ 15400,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRx,		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRx},
	{ 15404,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxCrcOk,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxCrcOk},
	{ 15408,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxCrcErr,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxCrcErr},
	{ 15412,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxAdrOk,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxAdrOk},
	{ 15416,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxAdrErr,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRxAdrErr},
	{ 15420,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbTramesTx,		MODBUS_SLAVE_USBD_CDC_PARAMS.nbTramesTx},
	// Répartition par Handler :
	{ 15424,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRead03,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesRead03},
	{ 15428,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesWrit16,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesWrit16},
	{ 15432,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesNoFnHand,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesNoFnHand},
	{ 15436,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesTxExcept,	MODBUS_SLAVE_USBD_CDC_PARAMS.nbFramesTxExcept},
#endif // MODBUS_SLAVE_SUPPORT_STATS

    // Statistiques Modbus USBH_CDC 0 :
#if defined(MODBUS_SLAVE_SUPPORT_STATS) && defined(MODBUS_SLAVE_USBH_CDC_PARAMS)	// EXPORT = 0
	{ 15500,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRx,		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRx},
	{ 15504,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxCrcOk,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxCrcOk},
	{ 15508,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxCrcErr,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxCrcErr},
	{ 15512,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxAdrOk,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxAdrOk},
	{ 15516,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxAdrErr,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRxAdrErr},
	{ 15520,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbTramesTx,		MODBUS_SLAVE_USBH_CDC_PARAMS.nbTramesTx},
	// Répartition par Handler :
	{ 15524,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRead03,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesRead03},
	{ 15528,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesWrit16,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesWrit16},
	{ 15532,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesNoFnHand,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesNoFnHand},
	{ 15536,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesTxExcept,	MODBUS_SLAVE_USBH_CDC_PARAMS.nbFramesTxExcept},
#endif // MODBUS_SLAVE_SUPPORT_STATS

#endif // MODBUS_SLAVE_ENABLE_STATS_ACCESS

	// Statistiques iBus :
#ifdef IBUS_SUPPORT_STATS	// EXPORT = 0
	{ 16000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesRx,			&iBusInfos[0].nbFramesRx},
	{ 16004,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesRxAck,		&iBusInfos[0].nbFramesRxAck},
	{ 16008,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesErrSz,		&iBusInfos[0].nbFramesErrSz},
	{ 16012,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesErrTbl,		&iBusInfos[0].nbFramesErrTbl},
	{ 16016,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesErrCrc,		&iBusInfos[0].nbFramesErrCrc},
	{ 16020,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesValid,		&iBusInfos[0].nbFramesValid},
	{ 16024,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesLowSz,		&iBusInfos[0].nbFramesLowSz},
	{ 16028,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesLowBuf,		&iBusInfos[0].nbFramesLowBuf},
	{ 16032,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesTxAck,		&iBusInfos[0].nbFramesTxAck},
	// Répartition par Handler :
	{ 16036,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesFireDamper,	&iBusInfos[0].nbFramesFireDamper},
	{ 16040,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesBcaPreHeat,	&iBusInfos[0].nbFramesBcaPreHeat},
	{ 16044,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesBcaPostHeat,	&iBusInfos[0].nbFramesBcaPostHeat},
	{ 16048,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesClavGroupe,	&iBusInfos[0].nbFramesClavGroupe},
	{ 16052,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesClavSystem,	&iBusInfos[0].nbFramesClavSystem},
	{ 16056,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetVarSetVar},		&iBusInfos[0].nbFramesNoRxHandler,	&iBusInfos[0].nbFramesNoRxHandler},
#endif // IBUS_SUPPORT_STATS

	// RTC spy :
#ifdef RV3028_RTC_ENABLE_MANUAL_RW
	{ 0xF020,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.opType,	&rtcRV3028_ManualRW.opType},	// Name = "RV3028.opType", Enum = "0:Idle/1:Read/2:Write"
	{ 0xF021,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.startAdr,	&rtcRV3028_ManualRW.startAdr},	// Name = "RV3028.startAdr"
	{ 0xF022,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.nbAdr,	&rtcRV3028_ManualRW.nbAdr}, 	// Name = "RV3028.nbAdr"
	{ 0xF023,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.nbTries,	&rtcRV3028_ManualRW.nbTries},	// Name = "RV3028.Execute
	{ 0xF024,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mRtcRv3028.manualStatus,	&mRtcRv3028.manualStatus},		// Name = "RV3028.manualStatus"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

#ifdef RV3028_RTC_ENABLE_MANUAL_RW
	{ 0xF030,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[0],	&rtcManualBuffer[0]},	// Name = "rtcManualBuffer_0"
	{ 0xF031,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[1],	&rtcManualBuffer[1]},	// Name = "rtcManualBuffer_1"
	{ 0xF032,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[2],	&rtcManualBuffer[2]},	// Name = "rtcManualBuffer_2"
	{ 0xF033,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[3],	&rtcManualBuffer[3]},	// Name = "rtcManualBuffer_3"
	{ 0xF034,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[4],	&rtcManualBuffer[4]},	// Name = "rtcManualBuffer_4"
	{ 0xF035,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[5],	&rtcManualBuffer[5]},	// Name = "rtcManualBuffer_5"
	{ 0xF036,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[6],	&rtcManualBuffer[6]},	// Name = "rtcManualBuffer_6"
	{ 0xF037,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[7],	&rtcManualBuffer[7]},	// Name = "rtcManualBuffer_7"
	{ 0xF038,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[8],	&rtcManualBuffer[8]},	// Name = "rtcManualBuffer_8"
	{ 0xF039,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[9],	&rtcManualBuffer[9]},	// Name = "rtcManualBuffer_9"
	{ 0xF03A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[10],	&rtcManualBuffer[10]},	// Name = "rtcManualBuffer_10"
	{ 0xF03B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[11],	&rtcManualBuffer[11]},	// Name = "rtcManualBuffer_11"
	{ 0xF03C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[12],	&rtcManualBuffer[12]},	// Name = "rtcManualBuffer_12"
	{ 0xF03D,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[13],	&rtcManualBuffer[13]},	// Name = "rtcManualBuffer_13"
	{ 0xF03E,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[14],	&rtcManualBuffer[14]},	// Name = "rtcManualBuffer_14"
	{ 0xF03F,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[15],	&rtcManualBuffer[15]},	// Name = "rtcManualBuffer_15"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

#ifdef RV3028_RTC_ENABLE_MANUAL_RW
	{ 0xF040,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[0],	&mI2CCM_Rtc_RV3028_RxBuf[0]},	// Name = "rtcRv3028RxBuffer_0"
	{ 0xF041,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[1],	&mI2CCM_Rtc_RV3028_RxBuf[1]},	// Name = "rtcRv3028RxBuffer_1"
	{ 0xF042,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[2],	&mI2CCM_Rtc_RV3028_RxBuf[2]},	// Name = "rtcRv3028RxBuffer_2"
	{ 0xF043,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[3],	&mI2CCM_Rtc_RV3028_RxBuf[3]},	// Name = "rtcRv3028RxBuffer_3"
	{ 0xF044,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[4],	&mI2CCM_Rtc_RV3028_RxBuf[4]},	// Name = "rtcRv3028RxBuffer_4"
	{ 0xF045,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[5],	&mI2CCM_Rtc_RV3028_RxBuf[5]},	// Name = "rtcRv3028RxBuffer_5"
	{ 0xF046,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[6],	&mI2CCM_Rtc_RV3028_RxBuf[6]},	// Name = "rtcRv3028RxBuffer_6"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

//	{ 0xF100,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},  resetMemoriesState,     requestResetMemories},   // Name = "reset memories" code 3854

	// [ Début de Zone spéciale avec emplacements communs Applicatif / BootLoader / Outils ]

	// Infos & Commandes spéciales BootLoader & Applicatif (Base Nükub) :
	{ 0xFF00,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getVersion16BootLoader,	0}, 						// Version du BootLoader, si présent, recherché & détecté (SHOW_HEX)
	{ 0xFF01,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getAliveRunTime1s,		0}, 						// Temps de RunTime @ 1s (Name = "Alive_RunTime", Unit = "s")
	{ 0xFF03,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetFct},	(void*)&curRunMode,		RequestRunMode4ThisModbus},	// Name = "RunMode", Enum = "1:BootLoader/2:Applicatif", MinVal = "1", MaxVal = "2", DefVal = "2"
	{ 0xFF04,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getRunStatus,			0},							// Name = "RunStatus", Enum = "1:Ready for Cmd/2:Update Pending/257:Ready 4 App/513:On Jump 2 BL"

#if defined(VI_SUPPORT_JUMP_BL) || defined(FIRMWARE_IS_BOOTLOADER)
  #ifdef FIRMWARE_IS_APPLICATIF
	{ 0xFF05,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&sabJumpToBL,			0},							// Temps avant try to Jump BL (Unit = "s", Coef = "10")
  #elif defined(FIRMWARE_IS_BOOTLOADER) // !FIRMWARE_IS_APPLICATIF && FIRMWARE_IS_BOOTLOADER :
	{ 0xFF05,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&sabKeepInBL,			0}, 						// Temps de maintient en BL (Unit = "s", Coef = "10")
  #endif // FIRMWARE_IS_APPLICATIF / FIRMWARE_IS_BOOTLOADER
	{ 0xFF06,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetFct},	&BL_Requesters,			0},							// Enum = "0:None/4:Modbus/32768:System"/2:iBus1/8:ModbusUser" (cf. Flags Source @ UartComDevices.h::L107)
#endif // VI_SUPPORT_JUMP_BL || FIRMWARE_IS_BOOTLOADER

#ifdef UPGRADE_FIRMWARE_UPGRADE_FIRMWARE_H_
	{ 0xFF07,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},	&CurUpgradeAdress,		0},							// SHOW_HEX
	{ 0xFF09,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&CurUpgradeTimeOut, 	0},							// Temps restant pour envoyer la suite d'une MàJ (Unit = "s", Coef = "10")
	{ 0xFF0A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetFct},	&CurUpgradeSrcId,		UnLockFlashProgramSrc},		// Enum = "0:None/2:Modbus/15:System"/1:iBusInt/3:iBusExt" (Cf. Id Source @ UartComDevices.h::L97)
  #ifdef FIRMWARE_IS_BOOTLOADER
	{ 0xFF0B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getUpdateStatus,	 	0},							//
	{ 0xFF0C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getUpdateError, 	 	0},							//
  #endif // FIRMWARE_IS_BOOTLOADER
#endif // UPGRADE_FIRMWARE_UPGRADE_FIRMWARE_H_
	{ 0xFF0D,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusSrcId,		0}, 						// Name = "ModbusId", Enum = "2:Modbus"/1:iBusInt/7:iBusExt" (Cf. Id Source @ UartComDevices.h::L97)
	{ 0xFF0E,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusRxBufSize,	0}, 						// Name = "ModbusRxBufSize", Unit = "bytes"
	{ 0xFF0F,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusTxBufSize,	0}, 						// Name = "ModbusTxBufSize", Unit = "bytes"

	// Infos & Commandes spéciales UserApp :
	{ 0xFF20,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},	(void*)&ProductKey,		0}, 						// Name = "ProductKey", SHOW_HEX, Enum = "1095321928:HimalayaMB" (<=> 0x41494948 = HIIA)
	{ 0xFF22,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetVersionSoft,			0},							// Version du Firmware Applicatif (SHOW_HEX, Name = "VersionSoft32")
	{ 0xFF24,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_MaxFwSize,	0}, 						// Max allowed size for the embedded Application (SHOW_HEX, Name = "MaxUserAppSize")
#ifdef FIRMWARE_IS_BOOTLOADER
	{ 0xFF26,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_EndOfStack,	0}, 						// End Of Stack for the new embedded Application (SHOW_HEX, Name = "AppEndOfStack")
	{ 0xFF28,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_RstVector,	0}, 						// Reset Vector for the new embedded Application (SHOW_HEX, Name = "AppResetVector")
#endif // FIRMWARE_IS_BOOTLOADER

	// Infos spéciales ST (emplacements identiques Applicatif / BootLoader) :
	{ 0xFF40,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_0,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF42,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_1,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF44,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_2,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF46,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_3,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF48,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetFlashSize,		0},	// Unit = "KB Flash", ONLY_DBG
	{ 0xFF49,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageData, 	0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF4A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getMcuDeviceId,		0},	// SHOW_HEX, ONLY_DBG
//	{ 0xFF49,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageSize_0,	0},	// SHOW_HEX, ONLY_DBG
//	{ 0xFF4A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageSize_1,	0},	// SHOW_HEX, ONLY_DBG
//	{ 0xFF4B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageSize_2,	0},	// SHOW_HEX, ONLY_DBG
//	{ 0xFF4C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageSize_3,	0},	// SHOW_HEX, ONLY_DBG

	// Infos spéciales Linker :
#ifdef VI_SUPPORT_LINKER_INFOS
	{ 0xFF60,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_EndOfStack,		0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF62,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_OriginOfFlash,	0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF64,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_EndOfFlash,		0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF66,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_EndOfText,		0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF68,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_EndOfFW,  		0},	// SHOW_HEX, ONLY_DBG

	{ 0xFF6A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_StartOfData,	0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF6C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_EndOfData, 	0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF6E,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getLinker_SiData, 		0},	// SHOW_HEX, ONLY_DBG
#endif // VI_SUPPORT_LINKER_INFOS

#endif // DISABLE_MODBUS_SLAVE_SUPPORT

};
//MODBUS_SLAVE_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tModbusSlaveItem, BaseOfTableModbusSlave, EndOfTableModbusSlave, TableModbusSlave);
UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tModbusSlaveItem, MODBUS_SLAVE_BASE_OF_TABLE, MODBUS_SLAVE_END_OF_TABLE, TableModbusSlave);

/******************************************************************************/

const uint16_t accessPswdTable[] =
{
	9781,	// Level 1 : (not used yet)
	5476,	// Level 2 : Outils de Configuration
	32184,	// Level 3 : IHM Produit
	2794,	// Level 4 : Banc de Test Produit complet
	941,	// Level 5 : Banc de Test Carte nue
	0		// End of Levels List (0 is required to close the List).
};
MODBUS_SLAVE_MAKE_CONST_BASE_PTR_OF_TABLE(uint16_t, MODBUS_SLAVE_BASE_OF_PSWD, accessPswdTable);

/******************************************************************************/

inline void ModbusSlaveInitUserMST(void)
{
#if defined(UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY) && (UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY > 0)
	// Initialise l'Identité & Adresse Esclave de chaque ModbusSlave :

  #ifdef MODBUS_SLAVE_UART_IHM_PARAMS
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_IHM_PARAMS, 	COM_UART_MODBUS_IHM_SRC_ID, 	MODBUS_SLAVE_IHM_DEF_SLAVE_ID);
  #endif // MODBUS_SLAVE_UART_IHM_PARAMS

  #ifdef MODBUS_SLAVE_UART_USER_PARAMS
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_USER_PARAMS,	COM_UART_MODBUS_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);	// Init nécessaire ici, ID éventuellement reconfiguré à la Lecture de MemHisto.
  #endif // MODBUS_SLAVE_UART_USER_PARAMS

  #ifdef MODBUS_SLAVE_UART_EXT_PARAMS
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_EXT_PARAMS, 	COM_UART_MODBUS_EXT_SRC_ID, 	MODBUS_SLAVE_EXT_DEF_SLAVE_ID);
  #endif // MODBUS_SLAVE_UART_EXT_PARAMS

#endif // UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY
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
	SetModbusUserSlaveAdr(newSlaveAdr & 0x7F7F); // Adresses autorisées jusqu'à 127 maximum
*/
}

/******************************************************************************/

void initModbusUserParamsFromMemHisto(void) // Ajout_Jp le 22/06/2020 : en cas de Démarrage du Soft avec Paramètres vierges
{
//	SetModbusUserSlaveAdr(DEF_MODBUS_USER_SLAVE_ADR);
}

/******************************************************************************/

__attribute__((weak)) uint16_t GetVersionSoft16(void) // Pour proposer un Pseudo Versionning en l'absence de la Librairie "VersionInfos"
{
	return 0xA001; // Alpha A001 sur 16bits
}
__attribute__((weak)) uint32_t GetVersionSoft32(void) // Pour proposer un Pseudo Versionning en l'absence de la Librairie "VersionInfos"
{
	return 0xA0000001; // Alpha v0.00.01 sur 32bits
}

/******************************************************************************/
