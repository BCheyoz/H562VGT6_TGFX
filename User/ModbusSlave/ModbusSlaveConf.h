/*
 * ModbusSlaveConf.h
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 27 Feb. 2025
 *  Updated by: j.proux
 *
 * Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 * -> il faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  History Usage :
 *-> 27/04/2021 : Added by Jp	to TestUART_Com (STM32F732VETx)
 *-> 21/05/2021 : Added by Jp	to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
 *-> 30/07/2021 : Added by Jp	to TstMotPaP_Firmware (STM32F103RBTx : toolsprojects/moteurpap)
 *-> 12/08/2021 : Added by Jp	to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
 *-> 05/01/2022 : Added by Jp	to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 25/02/2022 : Added by AB	to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *-> 08/04/2022 : Added by Jp	to SensorsAcquisition_G071RB (STM32G071RBT6 : innoprojects/sensors-acquisition/firmware-kit-stm32g071rb)
 *-> 06/12/2023 : Added by Ab	to MV_By_ALDES (STM32G030C8T6 : productprojects/ventilation/individuel/mvbyaldes/mv-by-aldes-app)
 *-> 27/11/2024 : Added by Jp	to Msp430i2021_Test1 (MSP430i2021 : sandbox/teststi/msp-exp430fr2433_test1)
 *-> 25/02/2025 : Added by Jp	to TFlow4_CarteMere_App (STM32H562VGTX : be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)
 *
 */

#ifndef MODBUS_SLAVE_MODBUS_SLAVE_CONF_H_
#define MODBUS_SLAVE_MODBUS_SLAVE_CONF_H_

// Remarque_Jp le 22/10/2021 : Merci de conserver les commentaires, pour Exemple, svp.

/********************************************************************************************
* Procédure pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau projet : *
*********************************************************************************************

 +--------------------------------+
 | Etape I : Configurer "UartCom" |
 +--------------------------------+
===> Cette Librairie s'appuie sur la Librairie "UartCom" :

I.1) Suivre tout d'abord la Procédure décrite dans "UartComConf.h", et surtout les étapes suivantes :
 -> I.3) Configurer CubeMX pour Communiquer en ModBus
 -> II.2) Activer "UART_COM_ENABLE_MODBUS_SLAVE" pour ModbusSlave
 -> III.10) Ajouter au projet l'une des Librairie "Utils" compatibles

I.2) Configurer dans "UartComDevices.h" les define correspondant à votre usage en ModbusSlave,
 en particulier "UART_COM_NB_OF_UART_MODBUS_SLAVE_ONLY", "UART_COM_NB_OF_UART_MODBUS_SLAVE_FTFL",
 "UART_COM_NB_OF_USBD_MODBUS_SLAVE_ONLY", "UART_COM_NB_OF_USBD_MODBUS_SLAVE_FTFL",
 "UART_COM_NB_OF_USBH_MODBUS_SLAVE_ONLY", "UART_COM_NB_OF_USBH_MODBUS_SLAVE_FTFL",
 ainsi que "UART_COM_NB_OF_USB_MODBUS_SLAVE_FTFL" (= "UART_COM_NB_OF_*_MODBUS_SLAVE_*").


 +-------------------------------------+
 | Etape II : Configurer "ModbusSlave" |
 +-------------------------------------+
===> Dans ce fichier "ModbusSlaveConf.h" (cf. plus bas) :

II.1) Renseigner "NB_MODBUS_SLAVE_BUF_DEF_SIZE" du nombre de périphériques vers ModbusSlave
 qui vont utiliser des Buffers de la taille par défaut

II.2) Le cas échéant, activer et renseigner "NB_MODBUS_SLAVE_BUF_SIZE_2" et/ou
 "NB_MODBUS_SLAVE_BUF_SIZE_3" du nombre de périphériques vers ModbusSlave qui vont utiliser,
 respectivement, des Buffers de taille personnalisée n°2 et/ou 3

II.3) Adapter "MODBUS_SLAVE_BUF_DEF_RX_SIZE" et "MODBUS_SLAVE_BUF_DEF_TX_SIZE" aux besoins,
 respectivement, en réception et transmission, de taille par défaut de Buffer.

Exemple 1 : 270 bytes en Réception et en Réponse par défaut
#define MODBUS_SLAVE_BUF_DEF_RX_SIZE	270	// Taille du Buffer par défaut pour la Réception ModbusSlave (min = 266 bytes)
#define MODBUS_SLAVE_BUF_DEF_TX_SIZE	270		// Taille du Buffer par défaut pour un Envoi ModbusSlave (min = 266 bytes)

Exemple 2 : 1100 bytes en Réception et 500 en Réponse par défaut
#define MODBUS_SLAVE_BUF_DEF_RX_SIZE	1100	// Taille du Buffer par défaut pour la Réception ModbusSlave (min = 266 bytes)
#define MODBUS_SLAVE_BUF_DEF_TX_SIZE	500		// Taille du Buffer par défaut pour un Envoi ModbusSlave (min = 266 bytes)

Exemple 3 : 1270 bytes en Réception et 270 bytes en Réponse par défaut
#define MODBUS_SLAVE_BUF_DEF_RX_SIZE	1270	// Taille du Buffer par défaut pour la Réception ModbusSlave (min = 266 bytes)
#define MODBUS_SLAVE_BUF_DEF_TX_SIZE	270		// Taille du Buffer par défaut pour un Envoi ModbusSlave (min = 266 bytes)

II.4) Le cas échéant, adapter "MODBUS_SLAVE_BUF_SIZE_2_RX" & "MODBUS_SLAVE_BUF_SIZE_2_TX" et/ou
 "MODBUS_SLAVE_BUF_SIZE_3_RX" & "MODBUS_SLAVE_BUF_SIZE_3_TX" aux besoins, respectivement, pour
 les Tailles de Buffer personnalisée n°2 et/ou 3 (en réception & transmission)

Exemple 1 : 50 bytes en Réception et en Réponse pour la Taille 2 :
#define MODBUS_SLAVE_BUF_SIZE_2_RX		50		// Pour la Réception
#define MODBUS_SLAVE_BUF_SIZE_2_TX		50		// Pour la Transmission

Exemple 2 : 1050 bytes en Réception et en Réponse pour la Taille 3 :
#define MODBUS_SLAVE_BUF_SIZE_3_RX		1050	// Pour la Réception
#define MODBUS_SLAVE_BUF_SIZE_3_TX		1050	// Pour la Transmission

II.5) Activer ou Désactiver les fonctions de ModbusSlave qui devront être supportées,
 par exemple : "MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS",
		 et/ou "MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS"
		 et/ou "MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER"

II.6) Activer ou Désactiver les spécificités "long" & "float" qui devront être supportées (ou pas), parmi
 "MODBUS_SLAVE_SUPPORT_LONG_INT32", "MODBUS_SLAVE_SUPPORT_LONG_LONG", "#define MODBUS_SLAVE_SUPPORT_FLOAT_INT",
 "MODBUS_SLAVE_SUPPORT_FLOAT_LONG" et/ou "MODBUS_SLAVE_SUPPORT_FLOAT_RAW"

II.7) Activer ou Désactiver le support des Statistiques ModbusSlave, ainsi que leur éventuel
 accès dans la Table Modbus : "MODBUS_SLAVE_SUPPORT_STATS" et "MODBUS_SLAVE_ENABLE_STATS_ACCESS"

II.8) Configurer l'Adresse Esclave Modbus par défaut pour chaque périphérique Source

Exemples :
#define MODBUS_SLAVE_IHM_DEF_SLAVE_ID	2	// Répondre au Modbus IHM  sur Adr_Esclave = 2
#define MODBUS_SLAVE_USER_DEF_SLAVE_ID	2	// Répondre au Modbus USER sur Adr_Esclave = 2
#define MODBUS_SLAVE_EXT_DEF_SLAVE_ID	2	// Répondre au Modbus EXT  sur Adr_Esclave = 2
#define MODBUS_SLAVE_USBD_DEF_SLAVE_ID	2	// Répondre au Modbus USBD sur Adr_Esclave = 2


 +---------------------------------------------------------+
 | Etape III : Configurer la 1° partie User de ModbusSlave |
 +---------------------------------------------------------+
===> Dans le fichier User.h de "ModbusSlaveUser.h" :

III) Activer ou Commenter, dans la zone "USER CODE * ManageParams" de "ModbusSlaveUser.h", les différentes
 Affectations des Paramètres de Gestion des sources de ModbusSlave

ATTENTION : Le nombre d'emplacements disponibles dépend des informations préalablement indiquées dans "UartComDevices.h".
 -> La définition "UART_COM_NB_OF_MODBUS_SLAVE" n'en est que la synthèse
 => Si besoin, se reporter en particulier à l'étape I.2 décrite ci-dessus.

Exemple 1 : Les Paramètres de Gestion ModbusSlave sur l'Uart IHM seront stockés à l'emplacement #0 de "ModbusSlaveParam" :
#define MODBUS_SLAVE_UART_IHM_PARAMS	&ModbusSlaveParam[0]

Exemple 2 : Les Paramètres de Gestion ModbusSlave sur l'Uart USER seront stockés à l'emplacement #1 de "ModbusSlaveParam" :
#define MODBUS_SLAVE_UART_USER_PARAMS	&ModbusSlaveParam[1]

Exemple 3 : Les Paramètres de Gestion ModbusSlave sur l'Uart EXT seront stockés à l'emplacement #2 de "ModbusSlaveParam" :
#define MODBUS_SLAVE_UART_EXT_PARAMS	&ModbusSlaveParam[2]

Exemple 4 : Les Paramètres de Gestion ModbusSlave sur l'USB_Device seront stockés à l'emplacement #3 de "ModbusSlaveParam" :
#define MODBUS_SLAVE_USBD_CDC_PARAMS	&ModbusSlaveParam[3]


 +-----------------------------------+
 | Etape IV : Configurer UartComUser |
 +-----------------------------------+
===> Dans la partie USER de "UartComUser.c" :

INFO : Si "UartComDevices.h" a suffisemment bien été configuré à l'étape I.2 ci-dessus,
 => Le fichier "ModbusSlaveUser.h" est automatiquement inclu dans "UartComUser.c".

IV.1) Dans la partie "USER CODE * Static_InitParams", insérer une ligne par Périphérique à associer
 à une gestion permanente en ModbusSlave, en respectant la structuration d'Init (de type "tUartComInitParams"),
 et en indiquant "ModbusSlaveRxHandler" comme fonction de traitement de Réception (paramètre "pFnRxHandler")

Exemple 1 : Modbus IHM sur Uart2, avec Buffer #0 de la taille par défaut, Réception par Interruption et Réponse par DMA :
{ UartComModbus,	UART_MODBUS_IHM,	MX_USART2_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),
	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,
	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,
	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,
	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,
	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave IHM sur Uart2

Exemple 2 : Modbus USER sur Uart3, avec Buffer #1 de la taille par défaut, Réception par Interruption et Réponse par DMA :
{ UartComModbus,	UART_MODBUS_USER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[1]),
	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_USER_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,
	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,
	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,
	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,
	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave User sur Uart3

Exemple 3 : Modbus EXT sur Uart8, avec Buffer #0 de Taille Custom 2, Réception par Interruption et Réponse par DMA :
{ UartComModbus,	UART_MODBUS_EXT,	MX_UART8_Init,			UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf2[0]),
	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_EXT_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,
	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,
	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,
	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_REPLY_SIZE_2,	MODBUS_SLAVE_ERR_RX_FRAME2,	MODBUS_SLAVE_ERR_TX_RETRY2,
	MODBUS_SLAVE_ERR_TX_FRAME2,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT },	// ModbusSlave Ext sur Uart8

IV.2) Si nécessaire (en cas de ModbusSlave intermittent) : dans la partie "USER CODE * Dynamic_InitParams",
 décommenter ou ajouter une constante de type "tUartComInitParams" pour chaque Périphérique à associer
 à une gestion intermittente en ModbusSlave, en respectant la structuration d'Init appropriée (de type "tUartComInitParams"),
 et en indiquant bien "ModbusSlaveRxHandler" comme fonction de traitement de Réception (c'est le paramètre "pFnRxHandler")

Exemple 1 : Modbus IHM sur Uart2, avec Buffer #0 de la taille par défaut, Réception par Interruption et Réponse par DMA :
const tUartComInitParams mUartComInitModbusIhmSlave =
	{ UartComModbus,	UART_MODBUS_IHM,	MX_USART2_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[0]),
	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_IHM_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,
	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,
	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,
	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,
	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusSlave IHM sur Uart2

Exemple 2 : Modbus USER sur Uart3, avec Buffer #1 de la taille par défaut, Réception par Interruption et Réponse par DMA :
const tUartComInitParams mUartComInitModbusUserSlave =
	{ UartComModbus,	UART_MODBUS_USER,	MX_USART3_UART_Init,	UART_COM_STRUCT_BUF_INFOS(ModbusSlaveBuf[1]),
	ModbusSlaveRxHandler,	MODBUS_SLAVE_UART_USER_PARAMS,	UART_COM_GPIO_TX_NONE,	MODBUS_SLAVE_EOF_RX,
	MODBUS_SLAVE_EOB_RX,	MODBUS_SLAVE_REPLY_TO,	MODBUS_SLAVE_NO_TX_INIT,	MODBUS_SLAVE_NO_TX_FRAME,
	MODBUS_SLAVE_NO_TX_BYTE,	MODBUS_SLAVE_NO_RX_TO,	MODBUS_SLAVE_MIN_RX_SIZE,	MODBUS_SLAVE_MIN_TX_SIZE,
	MODBUS_SLAVE_MAX_RX_BLOC_SZ,	MODBUS_SLAVE_DEF_REPLY_SZ,	MODBUS_SLAVE_ERR_RX_FRAME,	MODBUS_SLAVE_ERR_TX_RETRY,
	MODBUS_SLAVE_ERR_TX_FRAME,	MODBUS_SLAVE_INIT_DO_LOAD_FLAGS,	UART_COM_CLASS_TX_DMA_RX_IT };	// ModbusSlave User sur Uart3

IV.3) En cas de ModbusSlave intermittent, Décommenter et Adapter les fonctions appropriées parmi celles existantes :
 -> "ManageModbusIhmMode" : pour basculer Automatiquement ModbusSlave <-> ModbusMaster, par exemple sur UART_IHM
 -> "LoadUartIhmModbusSlave" & "UnloadUartIhmModbusSlave" : pour Activer/Désactiver Manuellement ModbusSlave sur UART_IHM
 -> "ManageModbusUserMode" pour basculer Automatiquement ModbusSlave <-> ModbusMaster, par exemple sur UART_USER
 -> "LoadUsbDeviceModbusSlave" & "UnloadUsbDeviceModbusSlave" : pour Activer/Désactiver Manuellement ModbusSlave sur USB_DEVICE
(ou en créer / ajouter d'autres similaires suivants les besoins)


 +-------------------------------------------------------+
 | Etape V : Configurer la 2° partie User de ModbusSlave |
 +-------------------------------------------------------+
===> Dans le fichier User.c de "ModbusSlaveUser.c" :

V.1) Ajouter vos Includes utiles dans la zone des "USER CODE * Includes" de "ModbusSlaveUser.c",

V.2) Ajouter vos Variables nécessaires dans la zone des "USER CODE * Variables" de "ModbusSlaveUser.c",

V.3) Ajouter vos Prototypes appropriés dans la zone des "USER CODE * Prototypes" de "ModbusSlaveUser.c",

V.4) Ajouter dans la Table ModbusSlave un élément de type tModbusSlaveItem par adresse Modbus à implémenter.
 Remarque : il faut toujours veiller à respecter l'ordre croissant des adresses.
 + Il faut aussi toujours veiller à ce qu'aucun élément ne fasse "d'ombre" au(x) suivant(s) en raison de sa taille.

Exemple 1 : En adresse $1, niveau d'accès 0 en lecture et niveau 4 requis en écriture, la variable "Fab_CodeSapProduct"
 (de type "ULong", c'est à dire 32bits non-signé) est retournée par lecture directe depuis son pointeur RAM ("GetVar") :
	{ 0x01,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_4}},	TVarULongGetVarSetVar},		&Fab_CodeSapProduct,	&Fab_CodeSapProduct},

Exemple 2 : En adresse 12 (= $C), niveau d'accès 0 en lecture et inaccessible en écriture ("LEVEL_MAX"), la valeur
 (de type "UInt", c'est à dire 16bits non-signé) est retournée à partir de la fonction "GetVersionSoft" :
	{ 0x0C,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_MAX}},	TVarUIntGetFctSetVar},		GetVersionSoft,			0},
Remarque : le temps d'exécution de la fonction de "lecture" indiquée doit être le plus bref possible, idéalement retourner directement
 une constante, ou une valeur déjà préalablement calculée ailleurs et stockée en RAM.

Exemple 3 : En adresse 16 (=$10), niveau d'accès 0 en lecture et écriture, la valeur (de type "UInt", c'est à dire 16bits non-signé)
 sera transmise à la fonction "RegisterNewAccessPswd" pour qu'elle effectue le traitement approprié :
	{ 0x10,		{{{	ACCESS_MIN_LEVEL_0,	ACCESS_MIN_LEVEL_0}},	TVarUIntGetFctSetFct},		getCurAccessLevel,		RegisterNewAccessPswd},
Remarque : le temps d'exécution de la fonction appelée pour traiter une "écriture" doit être le plus bref possible, idéalement
 se limiter à stocker la valeur et/ou activer un Flag en vue d'un traitement ultérieur éventuellement plus conséquent.

V.5) Décommenter ou Ajouter une ligne d'Initialisation de la Structure de gestion correspondante dans "ModbusSlaveInitUserMST"
 pour chaque ModbusSlave "statique" (c'est à dire en gestion "ModbusSlave" permanente) sur le Périphérique source.

Exemple 1 : Initialisation de la Structure de Gestion ModbusSlave sur l'Uart pour IHM, avec ses propres paramètres & Identifiants "IHM" :
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_IHM_PARAMS, 	COM_UART_MODBUS_IHM_SRC_ID,	MODBUS_SLAVE_IHM_DEF_SLAVE_ID);

Exemple 2 : Initialisation de la Structure de Gestion ModbusSlave sur l'Uart pour USER, avec ses propres paramètres & Identifiants "USER" :
	InitModbusSlaveStruct(MODBUS_SLAVE_UART_USER_PARAMS,	COM_UART_MODBUS_USER_SRC_ID,	MODBUS_SLAVE_USER_DEF_SLAVE_ID);

Info : La fonction "ModbusSlaveInitUserMST" est appelée par la Librairie "UartCom" (à partir de "UartCom_InitUserMST") juste après que
 tous les Périphériques matériel "statiques" associés aient été initialisés.


 +---------------------------------+
 | Etape VI : Configurer le Projet |
 +---------------------------------+
===> Dans le nouveau Projet :

VI.1) Ajouter le Dossier "ModbusSlave" à l' "IncludePath" pour toutes les Configs de Build
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/ModbusSlave" (sans les guillemets)
Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings"
 -> "MCU GCC Compiler" (si proposé) -> "Include paths",
 -> "MCU/MPU GCC Assembler" (si proposé) -> "Include paths",
 -> "MCU/MPU GCC Compiler" (si proposé) -> "Include paths",
 -> "MCU/MPU G++ Compiler" (si proposé) -> "Include paths".

VI.2) Vérifier que le Dossier "ModbusSlave" ne soit "Exclude From Build" d'aucune des Configs.
 (y compris "Debug" & "Release")
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

VI.3) Ajouter au projet l'une des Librairie "Utils" compatibles, comme celle de "HII_Manta_App" ou "HII_CarteMere_App".
 -> se reporter à "utils.h" pour intégrer plus facilement votre Librairie "Utils".


 +---------------------------------------------+
 | Etape VII : Configurer le "build-job" du CI |
 +---------------------------------------------+
===> Dans le MakeFile :

VII.1) Ajouter aux "C_SOURCES" le chemin relatif vers les fichiers "ModbusSlaveCore.c" & "ModbusSlaveUser.c",
 très probablement : "User/ModbusSlave/ModbusSlaveCore.c \" (sans les guillemets mais avec '\' en fin de ligne)
 ET : "User/ModbusSlave/ModbusSlaveUser.c \" (toujours sans les guillemets mais aussi avec '\' en fin de ligne)

Remarque : pour le chemin, il faut bien utiliser la bare oblique de la division '/' à la place du '\' habituel de Windows.

VII.2) Ajouter aux "C_INCLUDES" le chemin relatif vers notre dossier "ModbusSlave" avec le préfixe "-I" devant,
 très probablement : "-IUser/ModbusSlave \" (sans les guillemets mais également avec '\' en fin de ligne)

Même Remarque : pour le chemin, utiliser la bare obliques de la division '/' à la place du '\' habituel de Windows.

=> Félicitations, c'est prêt :-) !

Remarque : La table ModbusSlave est maintenant délocalisée dans "ModbusSlaveUser.c".

*******************************************************************************************/

#include "ModbusSlaveUtils.h"	// Pour accès aux outils de base spécifiques à ModbusSlave
#include "UartComDepUtils.h"	// Pour accès aux outils de base spécifiques aux dépendants de UartCom
#include "UartComDevices.h"		// Pour accès à la description des Périphériques utilisés

/******************************************************************************/
// Pour "UartComUser.c" (à adapter selon les besoins) :

#define NB_MODBUS_SLAVE_BUF_DEF_SIZE	1		// UART5 de taille Standard
//#define NB_MODBUS_SLAVE_BUF_SIZE_2  	1		// UART8 sera de taile réduite
//#define NB_MODBUS_SLAVE_BUF_SIZE_3  	1		// USBD_MODBUS_SLAVE & USBH_MODBUS_SLAVE sur le même Buffer

// ModbusSlave de Taille par défaut :
#define MODBUS_SLAVE_BUF_DEF_RX_SIZE	1270	// Taille du Buffer par défaut pour la Réception ModbusSlave (min = 266 bytes)
#define MODBUS_SLAVE_BUF_DEF_TX_SIZE	270		// Taille du Buffer par défaut pour un Envoi ModbusSlave (min = 266 bytes)

// ModbusSlave de Taille 2 :
#define MODBUS_SLAVE_BUF_SIZE_2_RX  	50		// Taille n°2 du Buffer pour la Réception
#define MODBUS_SLAVE_BUF_SIZE_2_TX  	50		// Taille n°2 du Buffer pour la Transmission

// ModbusSlave de Taille 3 :
#define MODBUS_SLAVE_BUF_SIZE_3_RX  	1050	// Taille n°3 du Buffer pour la Réception
#define MODBUS_SLAVE_BUF_SIZE_3_TX  	1050	// Taille n°3 du Buffer pour la Transmission

#ifndef NB_MODBUS_SLAVE_BUF_DEF_SIZE	// S'il n'y a pas de contre-ordre :
#define NB_MODBUS_SLAVE_BUF_DEF_SIZE	MAX(UART_COM_NB_MAX_OF_MODBUS_SLAVE, 0)	// Tous les ModbusSlave sont de la Taille par défaut
#endif // ! NB_MODBUS_SLAVE_BUF_DEF_SIZE

/******************************************************************************/
// Pour "ModbusSlaveCore.c" :

//#define DISABLE_MODBUS_SLAVE_SUPPORT
//#define DISABLE_MAX_READ_REGISTERS_ONCE 	// Désactiver la Limitation officielle (à 125 Registres max) sur ReadMultipleRegisters pour rentrer dans 256 Bytes
//#define DISABLE_MAX_WRITE_REGISTERS_ONCE	// Désactiver la Limitation officielle (à 124 Registres max) sur WriteMultipleRegisters pour rentrer dans 256 Bytes

//#define MODBUS_SLAVE_DEF_LVL_ACCESS_IHM 	ACCESS_MIN_LEVEL_MAX	// Full Access for Internal usage only
#define MODBUS_SLAVE_DEF_LVL_ACCESS_USER 	ACCESS_MIN_LEVEL_MAX	// Full Access for Internal usage only

// Activation des Fonctions Modbus Standard supportées :
#define MODBUS_SLAVE_SUPPORT_READ_HOLDING_REGISTERS 	// Enable support for 0x03 "Read Holding Registers"
#define MODBUS_SLAVE_SUPPORT_WRITE_MULTIPLE_REGISTERS	// Enable support for 0x10 "Write Multiple Registers"
#define MODBUS_SLAVE_SUPPORT_WRITE_SINGLE_REGISTER  	// Enable support for 0x06 "Write Single Register"

// Activation des "User Defined Function codes" supportés sur la Mémoire Externe :
//#define MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE	// Enable support for 0x74 "Write Custom Resources to External Memory"
//#define MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_MEMORY  	// Enable support for 0x75 "Write External Memory Bloc"
//#define MODBUS_SLAVE_SUPPORT_ERASE_EXTERNAL_MEMORY  	// Enable support for 0x76 "Erase External Memory Bloc"
//#define MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY		// Enable support for 0x77 "Read External Memory Bloc"

// Activation des "User Defined Function codes" supportés sur l'EEPROM Externe :
//#define MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_EEPROM		// Enable support for 0x78 "Read External EEPROM Bloc"

// Activation des "User Defined Function codes" supportés sur la Flash Interne :
//#define MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC		// Enable support for 0x69 "Write Firmware Program Bloc"
//#define MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE 		// Enable support for 0x68 "Check & Validate Firmware Program"
//#define MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM	// Enable support for 0x6A "Read an Internal Firmware Program Bloc"

/******************************************************************************/
// Pour "ModbusSlaveCore.h/c" :

// Activation des spécificités supportées :
#define MODBUS_SLAVE_SUPPORT_LONG_INT32  	// Pour supporter le transfert des "Long 32bits" en 2*Int16
#define MODBUS_SLAVE_SUPPORT_LONG_LONG  	// Pour supporter le transfert des "LongLong 64bits" en 4*Int16
#define MODBUS_SLAVE_SUPPORT_FLOAT_INT  	// Pour le transfert de Float en Int16, avec coefficient de multiplication
#define MODBUS_SLAVE_SUPPORT_FLOAT_LONG	// Pour le transfert de Float de très petite valeur, à fort coefficient de multiplication nécessaire
#define MODBUS_SLAVE_SUPPORT_FLOAT_RAW	// Pour le transfert de Float & Doubles en mode RAW

#define MODBUS_SLAVE_DEFAULT_UINT16_VALUE 0xFFFF

/******************************************************************************/
// Pour "ModbusSlaveCore.h/.c" & "ModbusSlaveUser.c" :

//#define MODBUS_SLAVE_SUPPORT_STATS		// Pour activer le support des Variables de Stat ModbusSlave
//#define MODBUS_SLAVE_ENABLE_STATS_ACCESS	// Pour activer l'accès, dans le Modbus, aux Stats de DEBUG
//#define MODBUS_SLAVE_HAS_LAST_FRAM_RESULT	// Pour activer la récupération du dernier FrameResult
#define MODBUS_SLAVE_USE_COMMON_WR_REG_FN	// Pour activer l'utilisation de la fonction commune de WriteRegister

/******************************************************************************/
// Pour "ModbusSlaveUser.c" :

#define MODBUS_SLAVE_IHM_DEF_SLAVE_ID	2	// Répondre au Modbus IHM  sur Adr_Esclave = 2
#define MODBUS_SLAVE_USER_DEF_SLAVE_ID	2	// Répondre au Modbus USER sur Adr_Esclave = 2
#define MODBUS_SLAVE_EXT_DEF_SLAVE_ID	2	// Répondre au Modbus EXT  sur Adr_Esclave = 2
#define MODBUS_SLAVE_USBD_DEF_SLAVE_ID	2	// Répondre au Modbus USBD sur Adr_Esclave = 2

/******************************************************************************/
// Pour "ModbusSlaveCore.c" -> HandleModbusFunctionDumpExternalMemory:

#ifdef MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY
	#define MODBUS_SLAVE_GETTER_EXT_MEM_MAX_ADR 	MEM_DEV_MAX_ADR	// from "MemHistoCore.h"
	#define MODBUS_SLAVE_CALLER_ReadArrayFromExternalMemory(a,n,b)	MemoryReadArrayHighSpeed(a,b,n)	// Requires Library "MemoryBridge"
#endif // MODBUS_SLAVE_SUPPORT_DUMP_EXTERNAL_MEMORY

/******************************************************************************/
// Pour "ModbusSlaveCore.c" -> HandleModbusFunctionWriteExternalRessource :

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE
	#define MODBUS_SLAVE_GETTER_EXT_RES_MAX_SIZE	MEM_HISTO_RES_MAX_SIZE	// from "MemHistoConf.h"
#endif // MODBUS_SLAVE_SUPPORT_WRITE_EXTERNAL_RESSOURCE

/******************************************************************************/
// Pour "ModbusSlaveCore.c" -> HandleModbusFunctionReadInternalProgram :

#ifdef MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM
	#define MODBUS_SLAVE_GETTER_FIRMWARE_BASE_ADR	FIRMWARE_BASE_ADR	// from "VersionInfos.h"
#endif // MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

/******************************************************************************/
// Pour "ModbusSlaveCore.c" -> HandleModbusFunctionWriteFirmwareBloc :

#ifdef MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC
	#define MODBUS_SLAVE_CALLER_LockThisFlashProgramSrc 	LockThisFlashProgramSrc	// from "UpgradeFirmware.h"

	#define MODBUS_SLAVE_CALLER_WriteFirmwareBlocFromBuf	WriteFirmwareBloc2ExtMemFromBuf 	// Pour stockage en Mémoire Externe
//	#define MODBUS_SLAVE_CALLER_WriteFirmwareBlocFromBuf	WriteFirmwareBloc2IntFlashFromBuf	// Pour écriture directe en Flash Interne
#endif // MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC

/******************************************************************************/
// Pour "ModbusSlaveCore.c" -> HandleModbusFunctionValidateFirmwareProgram :

#ifdef MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE
	#define MODBUS_SLAVE_GETTER_CHECK_FIMWARE_MIN_BLOC_SIZE	CHECK_FIMWARE_MIN_BLOC_SIZE 	// from "UpgradeFirmware.h"
	#define MODBUS_SLAVE_GETTER_CHECK_FIMWARE_VALUE_SIZE	CHECK_FIMWARE_VALUE_SIZE 		// from "UpgradeFirmware.h"

	#define MODBUS_SLAVE_CALLER_ValidateFirmwareProgram 	CheckFirmwareBlocFromExtMem 	// Si stockage en Mémoire Externe
//	#define MODBUS_SLAVE_CALLER_ValidateFirmwareProgram 	CheckFirmwareBlocFromIntFlash	// Si écriture directe en Flash Interne
#endif // MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE

/******************************************************************************/
// Pour HandleModbusFunctionReadInternalProgram & HandleModbusFunctionWriteFirmwareBloc :

#if defined(MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM) || defined(MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC)
	#define MODBUS_SLAVE_GETTER_FIRMWARE_MAX_SIZE	FIRMWARE_MAX_SIZE	// from "VersionInfos.h"
#endif // MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM || MODBUS_SLAVE_SUPPORT_READ_INTERNAL_FLASH_PRGM

/******************************************************************************/
// Pour HandleModbusFunctionWriteFirmwareBloc & HandleModbusFunctionValidateFirmwareProgram :

#if defined(MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC) || defined(MODBUS_SLAVE_SUPPORT_VALIDATE_FW_UPDATE)
	#define MODBUS_SLAVE_CALLER_CanFlashProgramFromSrc	CanFlashProgramFromSrc	// from "UpgradeFirmware.h"
	#define MODBUS_SLAVE_GETTER_FLASH_SRC_LOCKED		FLASH_SRC_LOCKED		// from "UpgradeFirmware.h"

	#define MODBUS_SLAVE_PROGRAM_SUCCESS	0x01
	#define MODBUS_SLAVE_PROGRAM_FAILURE	0xFF
#endif // MODBUS_SLAVE_SUPPORT_WRITE_FIRMWARE_BLOC

/******************************************************************************/
// Pour "ModbusSlaveCore.c" & "ModbusSlaveUser.c" :

#define MODBUS_SLAVE_BASE_OF_TABLE	BaseOfTableModbusSlave
#define MODBUS_SLAVE_END_OF_TABLE	EndOfTableModbusSlave
UART_COM_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(tModbusSlaveItem, MODBUS_SLAVE_BASE_OF_TABLE, MODBUS_SLAVE_END_OF_TABLE);

#define MODBUS_SLAVE_BASE_OF_PSWD	BaseOfPswdModbusSlave
MODBUS_SLAVE_MAKE_XTERN_CONST_BASE_OF_TABLE(uint16_t, MODBUS_SLAVE_BASE_OF_PSWD);

/******************************************************************************/
// Pour "UartComUser.c" (ne pas modifier ces réglages) :

#define MODBUS_SLAVE_EOF_RX 		5		// 5ms pour sabEndOfRxFrame (base = IT @ 1ms)
#define MODBUS_SLAVE_EOB_RX 		60		// 10ms pour sabEndOfRxFrame lorsqu'on n'a reçu qu'un Bloc de la Trame (base = IT @ 1ms)
#define MODBUS_SLAVE_REPLY_TO	 	(1000)	// 1s pour sabTimeOut4Reply (base = IT @ 1ms)
//#define MODBUS_SLAVE_NO_TX_INIT 	(10 *1000) // 10s pour sabReady4Tx (base = IT @ 1ms)
#define MODBUS_SLAVE_NO_TX_INIT 	(1 *1000) // 1s pour sabReady4Tx (base = IT @ 1ms)
#define MODBUS_SLAVE_NO_TX_FRAME	50		// 50ms pour sabReady4Tx (base = IT @ 1ms)
#define MODBUS_SLAVE_NO_TX_BYTE 	0		// 0ms pour sabMayTxNextByte (base = IT @ 1ms)
#define MODBUS_SLAVE_NO_RX_TO		(60 *10) // 60s pour sabReSetRxBufPtr (base = IT @ 100ms)

// Tailles par défaut allouées pour Répondre à une trame (< MODBUS_TX_SIZE) :
#define MODBUS_SLAVE_DEF_REPLY_SZ	MODBUS_SLAVE_BUF_DEF_TX_SIZE // Par défaut : tout l'espace TX disponible
#define MODBUS_SLAVE_REPLY_SIZE_2	MODBUS_SLAVE_BUF_SIZE_2_TX	 // Par défaut : tout l'espace TX_2 disponible
#define MODBUS_SLAVE_REPLY_SIZE_3	MODBUS_SLAVE_BUF_SIZE_3_TX	 // Par défaut : tout l'espace TX_3 disponible

#define MODBUS_SLAVE_ERR_RX_FRAME	15		// Tempo pour sabEndOfRxFrame en cas d'erreur de Réception (base = IT @ 1ms)
#define MODBUS_SLAVE_ERR_TX_RETRY	15		// Tempo pour sabMayTxNextByte : Retry en cas d'erreur d'Envoi (base = IT @ 1ms)
#define MODBUS_SLAVE_ERR_TX_FRAME	20		// Tempo pour sabMayTxNextByte : Silence après erreur d'Envoi (base = IT @ 1ms)

#define MODBUS_SLAVE_ERR_RX_FRAME2	500		// Tempo pour sabEndOfRxFrame en cas d'erreur de Réception (base = IT @ 1ms)
#define MODBUS_SLAVE_ERR_TX_RETRY2	100		// Tempo pour sabMayTxNextByte : Retry en cas d'erreur d'Envoi (base = IT @ 1ms)
#define MODBUS_SLAVE_ERR_TX_FRAME2	200		// Tempo pour sabMayTxNextByte : Silence après erreur d'Envoi (base = IT @ 1ms)

#define MODBUS_SLAVE_CHECK_ECHO 		0	// S'il faut Contrôler la Réception d'un Echo RxTX
#define MODBUS_SLAVE_MAY_SYNC_TX 		0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Requête Modbus
#define MODBUS_SLAVE_MAY_SYNC_REPLY 	0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Réponse Modbus
#define MODBUS_SLAVE_ON_TX_STATE		1	// Etat de la Pin GPIOx lors d'une Transmission Active de Trame
#define MODBUS_SLAVE_INIT_DO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_DO_LOAD_AT_MST, MODBUS_SLAVE_CHECK_ECHO, MODBUS_SLAVE_MAY_SYNC_TX, MODBUS_SLAVE_MAY_SYNC_REPLY, MODBUS_SLAVE_ON_TX_STATE) // Synthèse des Flags UartComInitFlags
#define MODBUS_SLAVE_INIT_NO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_NO_LOAD_AT_MST, MODBUS_SLAVE_CHECK_ECHO, MODBUS_SLAVE_MAY_SYNC_TX, MODBUS_SLAVE_MAY_SYNC_REPLY, MODBUS_SLAVE_ON_TX_STATE) // Synthèse des Flags UartComInitFlags

#endif /* MODBUS_SLAVE_MODBUS_SLAVE_CONF_H_ */
