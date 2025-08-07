/*
 * ModbusSlaveUser.c
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 07 Mars 2025
 *  Updated by: b.chhay
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

#include "AnalogInputsUser.h"		// Pour accès aux Variables AnalogInputs pour le debug/PdV
#include "EmbracoInverter.h"		// Pour accès aux Infos & Commandes manuelles Inverter Embraco
#include "I2cComMasterSystem.h" 	// Pour accès à tous les Capteurs sur I2C_System pour le debug/PdV
#include "FirmwareCInterface.h"		// pour accès aux fonctions logiciel
#include "FirmwareGateway.h"		// pour accès controlé aux différents composant système
//#include "memoireNonVolatile.h" 	// Pour accès à la Mémoire non-volatile
//#include "ERR_ErrorManager.h"		// pour la remontee des erreurs
//#include "DateTime32.h" 			// Pour accès à la Gestion de la DateTime
#include "ParamProductInfo.h"		// pour la lecture de l'etat logiciel
#include "Display_FF028T010.h"		// Pour retourner l'etat de l'afficheur
//#include "iBusDevTesteur.h" 		// Pour accès aux Commandes de Test des iBus
//#include "UpgradeFirmware.h"		// Pour accès aux Infos & Commandes de Mise à Jour Firmware

/* USER CODE END Includes */

/******************************************************************************/
// Variables locales & externes, suivant les besoins :

/* USER CODE BEGIN Variables */

uint16_t InstallCodePin = 405;	// For IHM

/* USER CODE END Variables */

/******************************************************************************/
// Prototypes de Fonctions locales & externes, suivant les besoins :

uint16_t GetVersionSoft16(void);
uint32_t GetVersionSoft32(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

/******************************************************************************/
// ProductInfos : Name = "TFlow4 Product" // "TFL4 Carte Mere App Product"
// BusConfig : Type = "rtu", Baudrate = "115200", Data = "8", Parity = "none", Stop = "1"
// EquipmentCfg : Name = "TFL4_CarteMere_App", Slave = "2"
// XmlConfig : varPrefix = "modbus_TFlow4_App", getPrefix = "get", setPrefix = "set", fnRead = "3", fnWrite = "16"

#pragma GCC diagnostic ignored "-Wcomment" // Pour ignorer les Multi-Line dans les commentaires (from "https://stackoverflow.com/questions/925179/selectively-remove-warning-message-gcc#3125889")

static const tModbusSlaveItem TableModbusSlave[] = {
//	Adresse,		RdMinLevel,			WrMinLevel,				VarType and Get/Set Method,		RdPtr,				WrPtr :

#ifndef DISABLE_MODBUS_SLAVE_SUPPORT	// EXPORT = 1

	// Identification :
	{ 0x01,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetFctSetFct},		&GetSapProduct,	&WriteSapProduct},	// Code SAP du Produit complet (Name = "Code SAP Produit")
	{ 0x03,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongLongGetFctSetFct},	&GetSnProduct,	&WriteSnProduct},	// SN du Produit ALDES (Name = "SN Produit complet")
	{ 0x07,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},		&GetTestDate,	&WriteTestDate},			// Date dernier test sous traitant (Name = "Date Test Carte")
	{ 0x08,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetFctSetFct},		&GetSapBoard,	&WriteSapBoard},			// Code SAP de la Carte Principale (Name = "Code SAP Carte", Enum = "11029476:Classic/11029475:Pression")
	{ 0x0A,		{{{ ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_5}},	TVarULongGetFctSetFct},		&GetSnBoard,	&WriteSnBoard},			// S/N de la Carte Pincipale (Name = "SN Carte")

	{ 0x0C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetVar},		GetVersionSoft16,		0},	// Version du Firmware sur 16bits (SHOW_HEX", DefVal = "B150")
// Remarque_Jp le 20/03/2025 : tu as défini "ZoneTargetId" comme "uint8_t" dans la Macro mais en UInt ci-dessous ...
	{ 0x0D,		{{{	ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetVar},		GetZoneTargetId,		0},	// Target du Soft (Name = "TargetId", Enum = \
	"0:France/1:Belgique/2:Espagne/3:Italie/4:Allemagne/5:Danemark/6:Norvege/7:Suede/8:Europe/9:Canada/10:China")
#if defined(VERSION_INFOS_VERSION_INFOS_H_) && defined(VI_SUPPORT_FW_CRC)	// EXPORT = 1
	{ 0x0E,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},		&FwComputedCRC32,		0},	// CRC du Soft (Name = "CRC_Soft", SHOW_HEX)
#endif // VERSION_INFOS_VERSION_INFOS_H_ && VI_SUPPORT_FW_CRC

	{ 0x10,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getCurAccessLevel,		RegisterNewAccessPswd},	// Code d'accès aux Niveaux Proteges (Name = "UserLevel", DefVal = "0", Enum = "0:Normal/1:Level 1/2:Level 2/3:Level 3/4:Level 4/5:Level 5/9781:Accès 1/5476:Configurator/32184:IHM Produit/2794:Banc ALDES/941:Carte nue")
//	{ 0x11,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetFct},		&Fab_MdbUserSlaveId,	SetModbusUserSlaveAdr},	// Nouvelle Adresse ModBus Client (Name = "ID Modbus User", DefVal = "2")
//	{ 0x12,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetFctSetVar},		0,						0},	// fonctionnalitées, type de fonctions disponibles
	{ 0x14,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		0,						SaveParamsOnOrder},	// Ordre Sauvegarde param (Name = "SaveParams", Enum = "0:Idle/22577:Product Params/31028:Factory Params")
	{ 0x15,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetVar},		GetProductVersionId,	0},			// Name = "Model Size", Enum = "0:Inconnu/8:Individual 105/9:Individual 180/16:Collectif 105/17:Collectif 180/65535:Indéterminé"
// $16 -> $1F = non affectés (au 24/07/2020).


	// Réglages Usine :
	{ 0x20,		{{{	ACCESS_MIN_LEVEL_MAX,ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		0,							requestToSwitchToFactoryState},	// Factory license : l'écriture de 147 provoque un passage en FACTORY_STATE (Enum = "0:NoChange/147:FACTORY_STATE")
	{ 0x21,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		firmwareState,				0},	// Etat logiciel : Enum = "0:Boot/1:Board ready/2:Board SAV ready/3:Product ready/4:Product complete/5:Bench state/6:Factory state"
	{ 0x22,		{{{	ACCESS_MIN_LEVEL_MAX,ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		0,							requestProductReset},	// Product reset: écrire 242 en FactoryState efface en mémoire les codes produit. Au prochain démarrage, reinit + retour en BOARD_READY (Enum = "0:NoChange/242:Reset Product")
	{ 0x23,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		GetProductId,				WriteProductId},	// product ID. Ecriture possible uniquement en mode factory state ou board SAV ready
	{ 0x24,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		0,							requestToInitRegulation},

#ifdef RTC_DATETIME32_SUPPORT_ENABLED	// EXPORT = 0
	// DateTime RTC :
	{ 0xF0,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarULongGetFctSetFct},		getCurDateTime32,			HandleNewDateTime32FromModbus}, // Name = "DateTime32", Unit = "s"
	{ 0xF2,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetVarSetVar},		&curRTC_DateTime.Year,		&curRTC_DateTime.Year},		// Name = "Date_Year", MinVal = "2020"
	{ 0xF3,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Month,		&curRTC_DateTime.Month},	// Name = "Date_Month", MinVal = "1", MaxVal = "12", DefVal = "1", Enum = "1:Janvier/2:Février/3:Mars/4:Avril/5:Mai/6:Juin/7:Juillet/8:Août/9:Septembre/10:Octobre/11:Novembre/12:Décembre"
	{ 0xF4,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Date,		&curRTC_DateTime.Date},		// Name = "Date_Day", MinVal = "1", MaxVal = "31", DefVal = "1"
	{ 0xF5,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Weekday,	&curRTC_DateTime.Weekday},	// Name = "Date_WeekDay", Enum = "0:Lundi/1:Mardi/2:Mercredi/3:Jeudi/4:Vendredi/5:Samedi/6:Dimanche", MinVal = "0", MaxVal = "6", DefVal = "0"
	{ 0xF6,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Hours,		&curRTC_DateTime.Hours},	// Name = "Time_Hours", Unit = "h", MinVal = "0", MaxVal = "23", DefVal = "0"
	{ 0xF7,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetVar},		&curRTC_DateTime.Minutes,	&curRTC_DateTime.Minutes},	// Name = "Time_Minuts" Unit = "m", MinVal = "0", MaxVal = "59", DefVal = "0"
	{ 0xF8,		{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_3}},	TVarUCharGetVarSetFct},		&curRTC_DateTime.Seconds,	HandleNewDateTimeSeconds},	// Name = "Time_Seconds", Unit = "s", MinVal = "0", MaxVal = "59", DefVal = "0"
	{ 0xF9, 	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},		&mRtcRv3028.base.idOfDevice,	0},	// Name = "RTC.RV3028", Enum = "0:Not Loaded/17:RV3028/32785:Try RV3028"
#endif // RTC_DATETIME32_SUPPORT_ENABLED
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

	// IHM :
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

	// Paramètres de Config France :
	{ 0x128,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.TypeLogement,	&HRVK_ConfigFR.TypeLogement},	// MinVal = "2", MaxVal = "6", DefVal = "2", Type de Logement : Name = "Type Logement"
	{ 0x129,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbBain,			&HRVK_ConfigFR.nbBain},			// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Bains : Name ="nb SDB"
	{ 0x12A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbWC,			&HRVK_ConfigFR.nbWC},			// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Wc : Name = "nb WC"
	{ 0x12B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_2}},	TVarSIntGetVarSetVar},		&HRVK_ConfigFR.nbCellier,		&HRVK_ConfigFR.nbCellier},		// MinVal = "1", MaxVal = "6", DefVal = "1", Nb de Cellier : Name = "nb Pièces Eau"
// $12C -> $12F = non affectés (au 24/07/2020).
*/

	// Commande Ventilateurs :
	{ 0x140,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		fanVoltage_mV,		requestFanVoltage_mV},	// Unit = "V", Coef = "1000", MinVal = "0", MaxVal = "10"
//	{ 0x141,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},		getFanSupplyVoltage_mV, 		setFanSupplyVoltage_mV},	// Unit = "V", Coef = "1000", MinVal = "0", MaxVal = "10"

	// Retours Ventilateurs :
	{ 0x162,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanFeedbackSpeed,			0},	// Vitesse Extraction (Unit = "RPM")
	{ 0x163,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanLastFeedbackSpeed,		0},	// Temporaire non moyennée Extraction (Unit = "RPM")
	{ 0x164,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetVar},		fanLastDeltaTime,			0},	// Temporaire deltaTime Extraction
	{ 0x165,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_3}},	TVarULongGetFctSetVar},		fanLastFrequency,			0},	// Temporaire frequence Extraction, (Unit = "Hz", Coef = "10", Name = "FreqFanIC")

//	{ 0x16A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyFeedbackSpeed,			0},	// Vitesse Extraction (Unit = "RPM")
//	{ 0x16B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyLastFeedbackSpeed,		0},	// Temporaire non moyennée Extraction (Unit = "RPM")
//	{ 0x16C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},		getFanSupplyLastDeltaTime,			0},	// Temporaire deltaTime Extraction
//	{ 0x16D,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarULongGetFctSetVar},		getFanSupplyLastFreq,				0},	// Temporaire frequence Extraction, (Unit = "Hz", Coef = "10", Name = "FreqFanSupplyIC")

	/*
	// Codes Erreur :
	{ 0x180,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getError,	0},	// erreur la plus  prioritaire
	{ 0x181,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},		getMantaError, 0},  // 1ere Manta en erreur trouvé
	 */

	// Gestion Banc de test
	{ 0x1200,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		blinkMode,					requestBlinkMode},	// Name = "BlinkMode", Enum = "0:Led Off/1:Led Fixe/2:Slow Blink/3:Fast Blink/4:Very Fast Blink/5:Heart Beat Blink"
	{ 0x1201,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUCharGetFctSetFct},		isAppointEnable,			setAppointEnable},	// Name = "AppointEnable", Enum = "0:Appoint Elec Off/1:Appoint Elec On"
	{ 0x1202,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUCharGetFctSetFct},		isAnodeState,				0},
	{ 0x1203,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUCharGetFctSetFct},		isAnodeFlags,				0},
	{ 0x1204,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		Display_FF028T010_Status,	0},
	{ 0x1205,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetFctSetFct},		Display_FF028T010_backLightLevel, Display_FF028T010_setBackLightLevel},
	{ 0x1206,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetFctSetFct},		requestBleSpiId,			0},
	{ 0x1208,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		0,							setBLE},
	{ 0x1209,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		BleIrqStatus,				0},

	// Gestion bypass
	{ 0x1210,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getBypassDuration,			setBypassDuration},
	{ 0x1211,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getPressure0,				byPassPressure0},
	{ 0x1212,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDPressure0,				0},
	{ 0x1213,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getCtn0,					byPassCtn0},
	{ 0x1214,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDCtn0,					0},
	{ 0x1215,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getCtn1,					byPassCtn1},
	{ 0x1216,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDCtn1,					0},
	{ 0x1217,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getCtn2,					byPassCtn2},
	{ 0x1218,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDCtn2,					0},
	{ 0x1219,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getCtn3,					byPassCtn3},
	{ 0x121A,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDCtn3,					0},
	{ 0x121B,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarSIntGetFctSetFct},		getCtn4,					byPassCtn4},
	{ 0x121C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarUIntGetFctSetFct},		getIDCtn4,					0},

	/*
    // Push Button :
// Voir si nécessaire :	{   0x1250,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getStartTimerPushButton,                        0},
// Voir si nécessaire :	{   0x1251,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getPushButtonClosedLongTime,                    0},
// Voir si nécessaire :	{   0x1252,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getPushButtonConsigneMode,                      0},
// Voir si nécessaire :	{   0x1253,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getPushButtonConsigneVitesse_MVE,               0},
// Voir si nécessaire :	{   0x1254,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUIntGetFctSetVar},         getPushButtonConsigneVitesse_MVI,               0},
    {   0x1255,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},        getCurrentPushButtonPressed,                    0},	// Pour Watts : Enum = "0:NotPressed/1:Pressed"

    {   0x1300,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getGestionModeVentilateur,                      setGestionModeVentilateur},		// A partir de A10F : Name = "Mode Régul", Enum = "0:Débit/1:Hygro/2:Vitesse"
    {   0x1300,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},        getGestionModeVentilateur,                      setGestionModeVentilateur},		// Enum = "0:DEBIT/1:VITESSE/2:HYGRO"
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

    // Gestion bypass
    {   0x1350,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetFct},         getBypassCurrentSaison,                        setBypassCurrentSaison},	// A partir A10F : Enum = "0:Inconnu/1:Hiver/2:Eté"
    {   0x1350,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getBypassCurrentSaison,                        0},	// Enum = "0:SPRING/1:SUMMER/2:AUTOMN/3:WINTER"
    {   0x1351,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getCurrentModeBypass,                          0},	// A partir A10F : Enum = "0:Manuel/1:Auto"
    {   0x1351,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_0}},  TVarUCharGetFctSetVar},         getCurrentModeBypass,                          0},	// Enum = "0:Auto/1:Manuel"
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



    // Gestion Filtre :
	{ 0x3000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetVar},	getBesoinChangementFiltreActif,				0},
	{ 0x3001,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},	getLimiteTimeChangementFiltre,				setLimiteTimeChangementFiltre},
	{ 0x3002,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},	getTimeRegulationFiltre1h,					setTimeRegulationFiltre1h},
	{ 0x3003,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetFct},	0,											setUserActionFiltreChange},

*/


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

	// Input Control Command
	{ 0x4F00,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_err_dtct_shnt_ena, setBs_err_dtct_shnt_ena},
	{ 0x4F01,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_ctry, setSs_ctry},
	{ 0x4F02,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_tech_mode, setSs_tech_mode},
	{ 0x4F03,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getNs_anti_lgn_day, setNs_anti_lgn_day},
	{ 0x4F04,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_heat_pump_test_rqst, setSs_heat_pump_test_rqst},
	{ 0x4F05,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_tank_size, setSs_tank_size},
	{ 0x4F06,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_sys_ver, setSs_sys_ver},
	{ 0x4F07,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_min, setCs_vent_pres_min},
	{ 0x4F08,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_sys, setCs_vent_pres_sys},
	{ 0x4F09,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getNs_hldy_nb, setNs_hldy_nb},
	{ 0x4F0A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getNs_pers_nb, setNs_pers_nb},
	{ 0x4F0B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_user_mode, setSs_user_mode},
	{ 0x4F0C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_anti_lgn_ena, setSs_anti_lgn_ena},
	{ 0x4F0D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_heat_wtr_cnsp_rst, setSs_heat_wtr_cnsp_rst},
	{ 0x4F0E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_vent_cnsp_rst, setSs_vent_cnsp_rst},
	{ 0x4F0F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_tot_cnsp_rst, setSs_tot_cnsp_rst},
	{ 0x4F00,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_sg_mode_ena, setSs_sg_mode_ena},
	{ 0x4F01,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_oph_mode_ena, setSs_oph_mode_ena},
	{ 0x4F02,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_hldy_rqst, setSs_hldy_rqst},
	{ 0x4F03,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_bst_rqst, setSs_bst_rqst},
	{ 0x4F04,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_down_temp_raw, setCs_tank_down_temp_raw},
	{ 0x4F05,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_up_temp_raw, setCs_tank_up_temp_raw},
	{ 0x4F06,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_xhst_temp_raw, setCs_pump_xhst_temp_raw},
	{ 0x4F07,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_evap_temp_raw, setCs_pump_evap_temp_raw},
	{ 0x4F08,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_temp_raw, setCs_vent_temp_raw},
	{ 0x4F09,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_raw, setCs_vent_pres_raw},
	{ 0x4F0A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_rot_spd_raw, setCs_vent_rot_spd_raw},
	{ 0x4F0B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_heat_pump_pwr, setCs_heat_pump_pwr},

	// Bypass Control Command
	{ 0x5000,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_heat_mode_simu,	setSs_heat_mode_simu},
	{ 0x5001,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_v40_sp_simu,	setCs_v40_sp_simu},
	{ 0x5002,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_temp_sp_simu,	setCs_temp_sp_simu},
	{ 0x5003,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_heat_pump_rot_spd_sp_simu,	setCs_heat_pump_rot_spd_sp_simu},
	{ 0x5004,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_elec_bstr_htr_sp_simu,	setSs_elec_bstr_htr_sp_simu},
	{ 0x5005,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_sp_simu,	setCs_vent_pres_sp_simu},
	{ 0x5006,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_flow_sp_simu,	setCs_vent_flow_sp_simu},
	{ 0x5007,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_vltg_sp_simu,	setCs_vent_vltg_sp_simu},
	{ 0x5008,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_down_temp_simu,	setCs_tank_down_temp_simu},
	{ 0x5009,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_up_temp_simu,	setCs_tank_up_temp_simu},
	{ 0x500A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_xhst_temp_simu,	setCs_pump_xhst_temp_simu},
	{ 0x500B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_evap_temp_simu,	setCs_pump_evap_temp_simu},
	{ 0x500C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_temp_simu,	setCs_vent_temp_simu},
	{ 0x500D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_simu,	setCs_vent_pres_simu},
	{ 0x500E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_down_temp_err_simu,	setBs_tank_down_temp_err_simu},
	{ 0x500F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_up_temp_err_simu,	setBs_tank_up_temp_err_simu},
	{ 0x5010,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_xhst_temp_err_simu,	setBs_pump_xhst_temp_err_simu},
	{ 0x5011,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_evap_temp_err_simu,	setBs_pump_evap_temp_err_simu},
	{ 0x5012,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_temp_err_simu,	setBs_vent_temp_err_simu},
	{ 0x5013,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_pres_err_simu,	setBs_vent_pres_err_simu},
	{ 0x5014,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_xhst_pres_simu,	setCs_pump_xhst_pres_simu},
	{ 0x5015,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_evap_pres_simu,	setCs_pump_evap_pres_simu},
	{ 0x5016,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_v40_min_simu,	setCs_v40_min_simu},
	{ 0x5017,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_rot_spd_simu,	setCs_vent_rot_spd_simu},
	{ 0x5018,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_heat_mode_simu_ena,	setBs_heat_mode_simu_ena},
	{ 0x5019,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_v40_sp_simu_ena,	setBs_v40_sp_simu_ena},
	{ 0x501A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_temp_sp_simu_ena,	setBs_temp_sp_simu_ena},
	{ 0x501B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_heat_pump_freq_sp_simu_ena,	setBs_heat_pump_freq_sp_simu_ena},
	{ 0x501C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_elec_bstr_htr_sp_simu_ena,	setBs_elec_bstr_htr_sp_simu_ena},
	{ 0x501D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_pres_sp_simu_ena,	setBs_vent_pres_sp_simu_ena},
	{ 0x501E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_flow_sp_simu_ena,	setBs_vent_flow_sp_simu_ena},
	{ 0x501F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_vltg_sp_simu_ena,	setBs_vent_vltg_sp_simu_ena},
	{ 0x5020,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_down_temp_simu_ena,	setBs_tank_down_temp_simu_ena},
	{ 0x5021,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_up_temp_simu_ena,	setBs_tank_up_temp_simu_ena},
	{ 0x5022,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_xhst_temp_simu_ena,	setBs_pump_xhst_temp_simu_ena},
	{ 0x5023,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_evap_temp_simu_ena,	setBs_pump_evap_temp_simu_ena},
	{ 0x5024,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_temp_simu_ena,	setBs_vent_temp_simu_ena},
	{ 0x5025,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_pres_simu_ena,	setBs_vent_pres_simu_ena},
	{ 0x5026,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_down_temp_err_simu_ena,	setBs_tank_down_temp_err_simu_ena},
	{ 0x5027,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_tank_up_temp_err_simu_ena,	setBs_tank_up_temp_err_simu_ena},
	{ 0x5028,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_xhst_temp_err_simu_ena,	setBs_pump_xhst_temp_err_simu_ena},
	{ 0x5029,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_evap_temp_err_simu_ena,	setBs_pump_evap_temp_err_simu_ena},
	{ 0x502A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_temp_err_simu_ena,	setBs_vent_temp_err_simu_ena},
	{ 0x502B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_pres_err_simu_ena,	setBs_vent_pres_err_simu_ena},
	{ 0x502C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_xhst_pres_simu_ena,	setBs_pump_xhst_pres_simu_ena},
	{ 0x502D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_pump_evap_pres_simu_ena,	setBs_pump_evap_pres_simu_ena},
	{ 0x502E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_v40_min_simu_ena,	setBs_v40_min_simu_ena},
	{ 0x502F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getBs_vent_rot_spd_simu_ena,	setBs_vent_rot_spd_simu_ena},

	// Output Control Command
	{ 0x5030,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_v40_lvl, setCs_v40_lvl},
	{ 0x5031,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_op_mode, setSs_op_mode},
	{ 0x5032,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_sg_stt, setSs_sg_stt},
	{ 0x5033,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_oph_stt, setSs_oph_stt},
	{ 0x5034,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_down_temp, setCs_tank_down_temp},
	{ 0x5035,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tank_up_temp, setCs_tank_up_temp},
	{ 0x5036,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_xhst_temp, setCs_pump_xhst_temp},
	{ 0x5037,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pump_evap_temp, setCs_pump_evap_temp},
	{ 0x5038,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_temp, setCs_vent_temp},
	{ 0x5039,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getSs_elec_bstr_htr_sp, setSs_elec_bstr_htr_sp},
	{ 0x503A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_heat_pump_rot_spd_sp, setCs_heat_pump_rot_spd_sp},
	{ 0x503B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_rot_spd, setCs_vent_rot_spd},
	{ 0x503C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_cnsp, setCs_vent_cnsp},
	{ 0x503D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_heat_wtr_cnsp, setCs_heat_wtr_cnsp},
	{ 0x503E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_tot_cnsp, setCs_tot_cnsp},
	{ 0x503F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_v40_sp, setCs_v40_sp},
	{ 0x5040,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_temp_sp, setCs_temp_sp},
	{ 0x5041,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres_sp, setCs_vent_pres_sp},
	{ 0x5042,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_flow_sp, setCs_vent_flow_sp},
	{ 0x5043,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_vltg_sp, setCs_vent_vltg_sp},
	{ 0x5044,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pres, setCs_vent_pres},
	{ 0x5045,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_pres_mes_filt, setCs_pres_mes_filt},
	{ 0x5046,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_vent_pwr, setCs_vent_pwr},


	// Calibration Control Command
	{ 0x6000,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetFctSetFct},	getControlTick,	setControlTick},
	{ 0x6001,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetFctSetFct},	getWaterHeatCtrlTick,	setWaterHeatCtrlTick},
	{ 0x6002,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetFctSetFct},	getInputMngTick,	setInputMngTick},
	{ 0x6003,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpressSpfilt_K,	setpressSpfilt_K},
	{ 0x6004,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getfirstOpressSpFilt_K,	setfirstOpressSpFilt_K},
	{ 0x6005,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpresMesfilt_K,	setpresMesfilt_K},
	{ 0x6006,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpresFilter_K,	setpresFilter_K},
	{ 0x6007,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpressSpfilt_Tau,	setpressSpfilt_Tau},
	{ 0x6008,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getfirstOpressSpFilt_Tau,	setfirstOpressSpFilt_Tau},
	{ 0x6009,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpresMesfilt_Tau,	setpresMesfilt_Tau},
	{ 0x600A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getpresFilter_Tau,	setpresFilter_Tau},
	{ 0x600B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpressSpfilt_initVal,	setpressSpfilt_initVal},
	{ 0x600C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getfirstOpressSpFilt_initVal,	setfirstOpressSpFilt_initVal},
	{ 0x600D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpresMesfilt_initVal,	setpresMesfilt_initVal},
	{ 0x600E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpresFilter_initVal,	setpresFilter_initVal},
	{ 0x600F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpressSpfilt_sampleTime,	setpressSpfilt_sampleTime},
	{ 0x6010,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getfirstOpressSpFilt_sampleTime,	setfirstOpressSpFilt_sampleTime},
	{ 0x6011,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpresMesfilt_sampleTime,	setpresMesfilt_sampleTime},
	{ 0x6012,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpresFilter_sampleTime,	setpresFilter_sampleTime},
	{ 0x6013,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpress_max_Value,	setpress_max_Value},
	{ 0x6014,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getpress_min_Value,	setpress_min_Value},
	{ 0x6015,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getflow_max_Value,	setflow_max_Value},
	{ 0x6016,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getflow_min_Value,	setflow_min_Value},
	{ 0x6017,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getPressureRegulator_InitVal,	setPressureRegulator_InitVal},
	{ 0x6018,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getPressureRegulator_Kawu,	setPressureRegulator_Kawu},
	{ 0x6019,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getPressureRegulator_Kd,	setPressureRegulator_Kd},
	{ 0x601A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getPressureRegulator_Ki,	setPressureRegulator_Ki},
	{ 0x601B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getPressureRegulator_Kp,	setPressureRegulator_Kp},
	{ 0x601C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1GetFctSetFct},	getPressureRegulator_SampleTime,	setPressureRegulator_SampleTime},
	{ 0x601D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getPressureRegulator_Tau_f,	setPressureRegulator_Tau_f},
	{ 0x601E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getCs_flow_fan_sp_C_Value,	setCs_flow_fan_sp_C_Value},
	{ 0x601F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getNullFlowConstant_Value,	setNullFlowConstant_Value},
	{ 0x6020,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getflowEsti_InitialCondition,	setflowEsti_InitialCondition},
	{ 0x6021,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX100GetFctSetFct},	getFPresVent_Tau,	setFPresVent_Tau},


//	// Code Installation pour l'IHM :
//	{ 0x9C54,	{{{	ACCESS_MIN_LEVEL_3,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetVar},	&InstallCodePin,				0},				// Code Installateur

	// Debug I2C :
#ifdef I2CCM_ENABLE_I2C_DEBUG	// EXPORT = 1
	{ 0xA000,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2cSystemNbRestart,	0},
	{ 0xA001,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2cDeviceErrorsCtmPresHSC,	0},
	{ 0xA002,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2cDeviceErrorsCtmPresABP2,	0},
	{ 0xA003,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2cDeviceErrorsCtmPresSDP8,	0},
	{ 0xA004,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2cDeviceErrorsCtmPresLMI,	0},

	{ 0xA005,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresHSC.Pressure,	&mPresHSC.Pressure},		// Unit = "Pa"
	{ 0xA006,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresHSC.Temperature,	&mPresHSC.Temperature},	// Unit = "°C"
	{ 0xA007,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2C_PresHSC_BridgeOffset,	setI2C_PresHSC_BridgeOffset},
	{ 0xA008,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetFctSetFct},	getI2C_PresHSC_BrdgOfstOpId,	setI2C_PresHSC_BrdgOfstOpId},
	{ 0xA009,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&mPresHSC.newFlags,	&mPresHSC.newFlags},

	{ 0xA00A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresABP2.Pressure,	&mPresABP2.Pressure},	// Unit = "Pa"
	{ 0xA00B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresABP2.Temperature,	&mPresABP2.Temperature},// Unit = "°C"
	{ 0xA00C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mPresABP2.status,	&mPresABP2.status},
	{ 0xA00D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&mPresABP2.newFlags,	&mPresABP2.newFlags},

	{ 0xA00E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresSDP8.Pressure,	&mPresSDP8.Pressure},	// Unit = "Pa"
	{ 0xA00F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresSDP8.Temperature,	&mPresSDP8.Temperature},// Unit = "°C"
	{ 0xA010,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&mPresSDP8.newFlags,	&mPresSDP8.newFlags},

	{ 0xA011,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresLMI.Pressure,	&mPresLMI.Pressure},		// Unit = "Pa"
	{ 0xA012,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX10GetVarSetVar},	&mPresLMI.Temperature,	&mPresLMI.Temperature},	// Unit = "°C"
	{ 0xA013,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&mPresLMI.newFlags,	&mPresLMI.newFlags},
#endif // I2CCM_ENABLE_I2C_DEBUG

	// ADC Debug
	{ 0xA020,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarSIntGetVarSetVar},	&tAi_CTN[0].value,		0},	// Unit = "°C", Coef = "100"
	{ 0xA021,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAi_CTN[0].nbPtADC,	0},
	{ 0xA022,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarSIntGetVarSetVar},	&tAi_CTN[1].value,		0},	// Unit = "°C", Coef = "100"
	{ 0xA023,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAi_CTN[1].nbPtADC,	0},
	{ 0xA024,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarSIntGetVarSetVar},	&tAi_CTN[2].value,		0},	// Unit = "°C", Coef = "100"
	{ 0xA025,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAi_CTN[2].nbPtADC,	0},
	{ 0xA026,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarSIntGetVarSetVar},	&tAi_CTN[3].value,		0},	// Unit = "°C", Coef = "100"
	{ 0xA027,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAi_CTN[3].nbPtADC,	0},
	{ 0xA028,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarSIntGetVarSetVar},	&tAi_CTN[4].value,		0},	// Unit = "°C", Coef = "100"
	{ 0xA029,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAi_CTN[4].nbPtADC,	0},
	{ 0xA02A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarFloatIntX1000GetVarSetVar},	&tAiRefAlim.value,	0},	// Unit = "V", Coef = "1000"
	{ 0xA02B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUIntGetVarSetVar},	&tAiRefAlim.nbPtADC,	0},

	// Infos & Commandes EmbracoInverter :
#ifdef EMBRACOINVERTER_EMBRACOINVERTER_H_	// EXPORT = 1
	{ 0xE000,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUCharGetFctSetFct},	GetEmbracoManagerFlags, 		0},			// SHOW_HEX
	{ 0xE001,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},	GetEmbracoInverterNbNoReplies,	SetEmbracoInverterNbNoReplies},
	{ 0xE002,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUCharGetFctSetFct},	GetEmbracoInverterComError, 	SetEmbracoInverterComError},	// SHOW_HEX

	{ 0xE008,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},	GetEmbracoInverterSpeedConsRead,	SetEmbracoInverterSpeedConsRPM}, // Name = "EmbracoInverterSpeedCons", Unit = "RPM"
	{ 0xE009,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_3}},	TVarUIntGetFctSetFct},	GetEmbracoInverterStatus16, 		SetEmbracoInverterStatus16},	// SHOW_HEX
	{ 0xE00A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetEmbracoInverterPowerRead,		0},		// Unit = "W"
	{ 0xE00B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetEmbracoInverterNbOfTrialsRead,	0},
	{ 0xE00C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetEmbracoInverterBusVoltageRead,	0}, 	// Unit = "V"
	{ 0xE00D,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetEmbracoInverterTemperatureX10Read, 0},	// Unit = "°c", Coef = "10"
	{ 0xE00E,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetEmbracoInverterPowerLimitationRead, 0},	// Unit = "W"
#endif // EMBRACOINVERTER_EMBRACOINVERTER_H_

	// RTC spy :
#ifdef RV3028_RTC_ENABLE_MANUAL_RW	// EXPORT = 0
	{ 0xF020,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.opType,	&rtcRV3028_ManualRW.opType},	// Name = "RV3028.opType", Enum = "0:Idle/1:Read/2:Write"
	{ 0xF021,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.startAdr,	&rtcRV3028_ManualRW.startAdr},	// Name = "RV3028.startAdr"
	{ 0xF022,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.nbAdr,	&rtcRV3028_ManualRW.nbAdr}, 	// Name = "RV3028.nbAdr"
	{ 0xF023,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcRV3028_ManualRW.nbTries,	&rtcRV3028_ManualRW.nbTries},	// Name = "RV3028.Execute
	{ 0xF024,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mRtcRv3028.manualStatus,	&mRtcRv3028.manualStatus},		// Name = "RV3028.manualStatus"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

#ifdef RV3028_RTC_ENABLE_MANUAL_RW	// EXPORT = 0
	{ 0xF030,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[0],	&rtcManualBuffer[0]},	// Name = "rtcManualBuffer_0"
	{ 0xF031,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[1],	&rtcManualBuffer[1]},	// Name = "rtcManualBuffer_1"
	{ 0xF032,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[2],	&rtcManualBuffer[2]},	// Name = "rtcManualBuffer_2"
	{ 0xF033,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[3],	&rtcManualBuffer[3]},	// Name = "rtcManualBuffer_3"
	{ 0xF034,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[4],	&rtcManualBuffer[4]},	// Name = "rtcManualBuffer_4"
	{ 0xF035,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[5],	&rtcManualBuffer[5]},	// Name = "rtcManualBuffer_5"
	{ 0xF036,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[6],	&rtcManualBuffer[6]},	// Name = "rtcManualBuffer_6"
	{ 0xF037,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[7],	&rtcManualBuffer[7]},	// Name = "rtcManualBuffer_7"
	{ 0xF038,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[8],	&rtcManualBuffer[8]},	// Name = "rtcManualBuffer_8"
	{ 0xF039,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[9],	&rtcManualBuffer[9]},	// Name = "rtcManualBuffer_9"
	{ 0xF03A,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[10],	&rtcManualBuffer[10]},	// Name = "rtcManualBuffer_10"
	{ 0xF03B,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[11],	&rtcManualBuffer[11]},	// Name = "rtcManualBuffer_11"
	{ 0xF03C,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[12],	&rtcManualBuffer[12]},	// Name = "rtcManualBuffer_12"
	{ 0xF03D,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[13],	&rtcManualBuffer[13]},	// Name = "rtcManualBuffer_13"
	{ 0xF03E,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[14],	&rtcManualBuffer[14]},	// Name = "rtcManualBuffer_14"
	{ 0xF03F,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&rtcManualBuffer[15],	&rtcManualBuffer[15]},	// Name = "rtcManualBuffer_15"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

#ifdef RV3028_RTC_ENABLE_MANUAL_RW	// EXPORT = 0
	{ 0xF040,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[0],	&mI2CCM_Rtc_RV3028_RxBuf[0]},	// Name = "rtcRv3028RxBuffer_0"
	{ 0xF041,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[1],	&mI2CCM_Rtc_RV3028_RxBuf[1]},	// Name = "rtcRv3028RxBuffer_1"
	{ 0xF042,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[2],	&mI2CCM_Rtc_RV3028_RxBuf[2]},	// Name = "rtcRv3028RxBuffer_2"
	{ 0xF043,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[3],	&mI2CCM_Rtc_RV3028_RxBuf[3]},	// Name = "rtcRv3028RxBuffer_3"
	{ 0xF044,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[4],	&mI2CCM_Rtc_RV3028_RxBuf[4]},	// Name = "rtcRv3028RxBuffer_4"
	{ 0xF045,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[5],	&mI2CCM_Rtc_RV3028_RxBuf[5]},	// Name = "rtcRv3028RxBuffer_5"
	{ 0xF046,	{{{	ACCESS_MIN_LEVEL_5,	ACCESS_MIN_LEVEL_5}},	TVarUCharGetVarSetVar},	&mI2CCM_Rtc_RV3028_RxBuf[6],	&mI2CCM_Rtc_RV3028_RxBuf[6]},	// Name = "rtcRv3028RxBuffer_6"
#endif // RV3028_RTC_ENABLE_MANUAL_RW

//	{ 0xF100,   {{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_5}},   TVarUIntGetFctSetFct},  resetMemoriesState,     requestResetMemories},   // Name = "reset memories" code 3854

	// [ Début de Zone spéciale avec emplacements communs Applicatif / BootLoader / Outils ]

	// Infos & Commandes spéciales BootLoader & Applicatif (Base Nükub) :
	{ 0xFF00,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getVersion16BootLoader,	0}, 						// Version du BootLoader, si présent, recherché & détecté (SHOW_HEX)
	{ 0xFF01,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getAliveRunTime1s,		0}, 						// Temps de RunTime @ 1s (Name = "AliveRunTime", Unit = "s")
	{ 0xFF03,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUCharGetVarSetFct},	(void*)&curRunMode,		RequestRunMode4ThisModbus},	// Name = "RunMode", Enum = "1:BootLoader/2:Applicatif", MinVal = "1", MaxVal = "2", DefVal = "2"
	{ 0xFF04,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getRunStatus,			0},							// Name = "RunStatus", Enum = "1:Ready for Cmd/2:Update Pending/257:Ready 4 App/513:On Jump 2 BL"

#if defined(VI_SUPPORT_JUMP_BL) || defined(FIRMWARE_IS_BOOTLOADER)
  #ifdef FIRMWARE_IS_APPLICATIF	// EXPORT = 1
	{ 0xFF05,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&sabJumpToBL,			0},							// Temps avant try to Jump BL (Unit = "s", Coef = "10")
  #elif defined(FIRMWARE_IS_BOOTLOADER) // !FIRMWARE_IS_APPLICATIF && FIRMWARE_IS_BOOTLOADER (EXPORT = 0) :
	{ 0xFF05,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&sabKeepInBL,			0}, 						// Temps de maintient en BL (Unit = "s", Coef = "10")
  #endif // FIRMWARE_IS_APPLICATIF / FIRMWARE_IS_BOOTLOADER
	{ 0xFF06,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetFct},	&BL_Requesters,			0},							// Enum = "0:None/4:Modbus/32768:System"/2:iBus1/8:ModbusUser" (cf. Flags Source @ UartComDevices.h::L107)
#endif // VI_SUPPORT_JUMP_BL || FIRMWARE_IS_BOOTLOADER

#ifdef UPGRADE_FIRMWARE_UPGRADE_FIRMWARE_H_	// EXPORT = 0
	{ 0xFF07,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},	&CurUpgradeAdress,		0},							// SHOW_HEX
	{ 0xFF09,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetVarSetVar},	&CurUpgradeTimeOut, 	0},							// Temps restant pour envoyer la suite d'une MàJ (Unit = "s", Coef = "10")
	{ 0xFF0A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetVarSetFct},	&CurUpgradeSrcId,		UnLockFlashProgramSrc},		// Enum = "0:None/2:Modbus/15:System"/1:iBusInt/3:iBusExt" (Cf. Id Source @ UartComDevices.h::L97)
  #ifdef FIRMWARE_IS_BOOTLOADER	// EXPORT = 0
	{ 0xFF0B,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getUpdateStatus,	 	0},							//
	{ 0xFF0C,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getUpdateError, 	 	0},							//
  #endif // FIRMWARE_IS_BOOTLOADER
#endif // UPGRADE_FIRMWARE_UPGRADE_FIRMWARE_H_

	// Modbus Infos :
	{ 0xFF0D,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusSrcId,		0}, 						// Name = "ModbusId", Enum = "3:ModbusUser"/1:iBusInt/7:iBusExt" (Cf. Id Source @ UartComDevices.h::L97)
	{ 0xFF0E,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusRxBufSize,	0}, 						// Name = "ModbusRxBufSize", Unit = "bytes"
	{ 0xFF0F,	{{{ ACCESS_MIN_LEVEL_0, ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	getThisModbusTxBufSize,	0}, 						// Name = "ModbusTxBufSize", Unit = "bytes"

	// Infos & Commandes spéciales UserApp :
	{ 0xFF20,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetVarSetVar},	(void*)&ProductKey,		0}, 						// Name = "ProductKey", SHOW_HEX, Enum = "1093944916:TFlow 4 App/1110722132:TFlow 4 BootLoader" (<=> 0x41344654 = TF4A, 0x42344654 = TF4B)
	{ 0xFF22,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetVersionSoft,			0},							// Version du Firmware Applicatif (SHOW_HEX, Name = "VersionSoft32")
	{ 0xFF24,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_MaxFwSize,	0}, 						// Max allowed size for the embedded Application (SHOW_HEX, Name = "MaxUserAppSize")
#ifdef FIRMWARE_IS_BOOTLOADER	// EXPORT = 0
	{ 0xFF26,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_EndOfStack,	0}, 						// End Of Stack for the new embedded Application (SHOW_HEX, Name = "AppEndOfStack")
	{ 0xFF28,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getUserApp_RstVector,	0}, 						// Reset Vector for the new embedded Application (SHOW_HEX, Name = "AppResetVector")
#endif // FIRMWARE_IS_BOOTLOADER

	// Infos spéciales ST (emplacements identiques Applicatif / BootLoader) :
#ifdef UID_BASE	// EXPORT = 1
	{ 0xFF40,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_0,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF42,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_1,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF44,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_2,			0},	// SHOW_HEX, ONLY_DBG
	{ 0xFF46,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	GetUID_3,			0},	// SHOW_HEX, ONLY_DBG
#endif // UID_BASE
#ifdef FLASHSIZE_BASE	// EXPORT = 1
	{ 0xFF48,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetFlashSize,		0},	// Unit = "KB Flash", ONLY_DBG
#endif // FLASHSIZE_BASE
#ifdef PACKAGE_BASE	// EXPORT = 1
	{ 0xFF49,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetFct},	GetPackageData, 	0},	// SHOW_HEX, ONLY_DBG
#endif // PACKAGE_BASE
#ifdef DBGMCU	// EXPORT = 1
	{ 0xFF4A,	{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarULongGetFctSetFct},	getMcuDeviceId,		0},	// SHOW_HEX, ONLY_DBG
#endif // DBGMCU

	// Infos spéciales Linker :
#ifdef VI_SUPPORT_LINKER_INFOS	// EXPORT = 0
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
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_USER_PARAMS,	COM_UART_MODBUS_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);	// Init nécessaire ici, puis ID éventuellement reconfiguré à la Lecture de MemHisto.
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

void SaveParamsOnOrder(uint16_t Order2Save)
{
#define SAVE_FACT_PARAMS_NOW	0x7934	// Save Factory Params now (0x7934 = 31028)
#define SAVE_USER_PARAMS_NOW	0x5831	// Save User Params Now (0x5831 = 22577)
//*
	int targetFlags = 0;
	switch(Order2Save)
	{
	case SAVE_FACT_PARAMS_NOW:
		if( (0 != isModbusWriteHandlerInFactoryLevel()) || (0 == isInModbusHandlerContext()) ) // FactoryLevel OR InternalRequest :
		{
			targetFlags = 1;
		}
		if(0 != (targetFlags & 1))
		{
//			ForceSaveSystemParams(MEM_HISTO_FORCE_CAPTURE_NOW);
		} // ToDo: Ajouter l'info comme quoi l'opération se serait bien passée (ou pas) !
		break;
	case SAVE_USER_PARAMS_NOW:
//		ForceCaptureStats2(MEM_HISTO_FORCE_CAPTURE_NOW | MEM_HISTO_RESYNC_AUTO_CAPTURE);
		targetFlags |= 2;
		break;
	default:
		break;
	}
	UNUSED(targetFlags); // ToDo : Transférer le résultat "targetFlags" vers une variable RAM pour Modbus
//*/
}

/******************************************************************************/

void handleQueryReInitFactory(uint16_t pswd)
{
#define CONFIG_RESET	0x4725	// RESET_TO_PRODUCT_READY
#define PRODUCT_RESET	0x9653	// RESET_TO_BOARD_READY
	int tmpFlags = (pswd == CONFIG_RESET) ? 1 : 0; // Uniquement le Niveau 1
	if(0 != isModbusWriteHandlerInFactoryLevel())	// FactoryLevel4 requis !
//	if(pCurModbus->AccessLevel >= ACCESS_MIN_LEVEL_4) // Niveau 4 requis !
	{
		if(pswd == PRODUCT_RESET) { tmpFlags = 3; } // RéInitialiser les Niveaux 1 & 2
	}
	if(tmpFlags != 0)
	{
//		doReInitFactory(tmpFlags);
//		ForceCaptureStats2(MEM_HISTO_FORCE_CAPTURE_NOW | MEM_HISTO_RESYNC_AUTO_CAPTURE);
	}
}

/******************************************************************************/
