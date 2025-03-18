/*
 * UartComConf.h
 *
 *  Created on: Dec 15, 2020
 *  Original Author: j.proux
 *
 *  Updated on: 26 Feb. 2025
 *  Updated by: j.proux
 *
 *  Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 *	-> il faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  History Usage :
 *-> 16/12/2020 : Added by Jp	to TestUART_Com (STM32F732VETx : )
 *-> 21/05/2021 : Added by Jp	to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
 *-> 30/07/2021 : Added by Jp	to TstMotPaP_Firmware (STM32F103RBTx : toolsprojects/moteurpap)
 *-> 12/08/2021 : Added by Jp	to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
 *-> 22/10/2021 : Added by Jp	to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
 *-> 04/01/2022 : Added by Jp	to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
 *-> 25/02/2022 : Added by AB	to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
 *-> 08/04/2022 : Added by Jp	to SensorsAcquisition_G071RB (STM32G071RBT6 : innoprojects/sensors-acquisition/firmware-kit-stm32g071rb)
 *-> 30/11/2023 : Added by Ab	to MV_By_ALDES (STM32G030C8T6 : productprojects/ventilation/individuel/mvbyaldes/mv-by-aldes-app)
 *-> 21/02/2025 : Added by Jp	to TFlow4_CarteMere_App (STM32H562VGTX : be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)
 *
 */

#ifndef UART_COM_UART_COM_CONF_H_
#define UART_COM_UART_COM_CONF_H_

// Remarque_Jp le 22/10/2021 : Merci de conserver les commentaires, pour Exemple, svp.

/****************************************************************************************
* Procédure pour intégrer facilement cette Librairie "UartCom" dans un nouveau projet : *
*****************************************************************************************

 +-----------------------------+
 | Etape I : Configurer CubeMX |
 +-----------------------------+
===> Dans CubeMX :

I.1) Dans CubeMX, configurer toutes les entrées & sorties concernées et leur attribuer
 un Nom en fonction de leur usage prévu d'après le schéma.

I.2) Configurer CubeMX pour Communiquer en iBus :
-------------------------------------------------

I.2.1) Pour chaque U(S)ART# prévu pour communiquer en iBus, configurer le "Mode" suivant :
 -> Mode = "Asynchronous"
 -> Hardware Flow Control (RS232) = "Disable"
 -> Hardware Flow Control (RS485) = ne PAS cocher

I.2.2) Pour chaque U(S)ART# pour iBus, indiquer les "Parameter Settings -> Basic Parameters" suivants :
 -> Baud Rate = "2400 Bits/s"
 -> Word Length = "9 Bits (including Parity)
 -> Parity = "Even"
 -> Stop Bits = "1"

I.2.3) Pour chaque U(S)ART# pour iBus, indiquer les "Parameter Settings -> Advanced Parameters" suivants :
 -> Data Direction = "Receive and Transmit"
 -> Over Sampling = "16 samples"
 -> Single Sample = "Disable"
 -> ClockPrescaler (si demandé) = "1"
 -> Fifo Mode (si demandé) = "Disable"
 -> Txfifo Threshold (si demandé) = "1 eighth full configuration"
 -> Rxfifo Threshold (si demandé) = "1 eighth full configuration"

I.2.4) Pour chaque U(S)ART# pour iBus, indiquer les "Parameter Settings -> Advanced Features" suivants :
 -> Auto Baudrate = "Disable"
 -> TX Pin Activate Level Inversion = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> RX Pin Activate Level Inversion = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> Data Inversion = "Disable"
 -> TX and RX Pins Swapping = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> Overrun = "Enable"
 -> DMA on RX Error = "Enable"
 -> MSB First = "Disable"

I.2.5) Pour chaque U(S)ART# pour iBus, configurer "NVIC Settings" tel que :
 -> "U(S)ART* interrupt" soit bien Coché "Enabled"

I.2.6) Dans la Catégorie "System Core" -> "NVIC" -> Panel "Configuration" -> onglet "NVIC",
 s'assurer que, pour chaque U(S)ART# pour iBus soient bien configurés :
 -> "U(S)ART* interrupt" ou "U(S)ART* global interrupt" : Coché "Enabled"
  + si proposé : laisser "Preemption Priority" à 0,
  + si proposé : laisser "Sub Priority" à 0.

I.2.7) Dans la Catégorie "System Core" -> "NVIC" -> Panel "Configuration" -> onglet "Code generation",
 s'assurer que, pour chaque U(S)ART# pour iBus soient bien configurés :
 -> "U(S)ART* interrupt" ou "U(S)ART* global interrupt" : Cochés "Generate IRQ handler" & "Call HAL handler"


I.3) Configurer CubeMX pour Communiquer en ModBus :
---------------------------------------------------

I.3.1) Pour chaque U(S)ART# prévu pour communiquer en Modbus, configurer le "Mode" suivant :
 -> Mode = "Asynchronous"
 -> Hardware Flow Control (RS232) = "Disable"
 -> Hardware Flow Control (RS485) = Coché, si nécessaire ou utile

I.3.2) Pour chaque U(S)ART# pour Modbus, indiquer les "Parameter Settings -> Basic Parameters" suivants :
 -> Baud Rate = suivant ce qui est nécessaire, par exemple "115200 Bits/s"
 -> Word Length = "8 Bits" ou "9 Bits (including Parity), suivant ce qui est nécessaire
 -> Parity = "None" (ou "Odd" ou "Even", suivant ce qui est nécessaire)
 -> Stop Bits = "1" (ou "2", "0.5" ou "1.5", suivant ce qui est nécessaire)

I.3.3) Pour chaque U(S)ART# pour ModBus, indiquer les "Parameter Settings -> Advanced Parameters" suivants :
 -> Data Direction = "Receive and Transmit"
 -> Over Sampling = "16 samples"
 -> Single Sample = "Disable"
 -> ClockPrescaler (si demandé) = "1"
 -> Fifo Mode (si demandé) = "Disable"
 -> Txfifo Threshold (si demandé) = "1 eighth full configuration"
 -> Rxfifo Threshold (si demandé) = "1 eighth full configuration"
 -> Polarity (si demandé) = "High" (ou "Low" si nécessaire -> cf. schéma)
 -> Assertion Time (si demandé) = "0 Sample Time Unit"
 -> Dessertion Time (si demandé) = "0 Sample Time Unit"

I.3.4) Pour chaque U(S)ART# pour ModBus, indiquer les "Parameter Settings -> Advanced Features" suivants :
 -> Auto Baudrate = "Disable"
 -> TX Pin Activate Level Inversion = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> RX Pin Activate Level Inversion = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> Data Inversion = "Disable"
 -> TX and RX Pins Swapping = "Disable" (ou "Enable" si nécessaire -> cf. schéma)
 -> Overrun = "Enable"
 -> DMA on RX Error = "Enable"
 -> MSB First = "Disable"

I.3.5) Configurer un DMA_TX pour chaque U(S)ART# vers Modbus :
  Si le µC est équipé d'un (ou plusieurs) "GPDMA", CubeMX proposera certainement / obligera pê à l'utiliser.
 -> Dans ce cas, se reporter à la section I.3.5b
 -> Sinon, procéder comme indiqué dans la section I.3.5a

I.3.5a) Si le µC n'est PAS équipé de "GPDMA" (ou qu'il n'y en a plus de disponible), configurer un DMA ...
  Dans "Configuration -> DMA Settings", ajouter puis configurer une 'DMA Request' de type "U(S)ART#_TX" telle que :
-> Mode = "Normal"
-> Increment Address -> Peripheral = ne PAS cocher
-> Increment Address -> Memory = Cocher
-> Use Fifo (si proposé) = ne PAS cocher
-> Data Width -> Peripheral = "Byte"
-> Data Width -> Memory = "Byte"
-> Enable synchronisation (si proposé) : à priori pas besoin de cocher
-> Enable event (si proposé) : à priori pas besoin de cocher

I.3.5b) Si le µC est équipé d'un (ou plusieurs) "GPDMA" ...
  Sélectioner l'un des 'GPDMA' disponibles, puis configurer l'un des "Channel" disponibles (par exemple Channel 0) :
-> s'assurer au préalable qu'il soit bien de type "2 Words Internal FIFO",
-> puis le configurer en "Standard Request Mode"

  Dans le paneau "Configuration", cliquer sur l'onglet correspondant au channel sélectionné (par exemple "CH0") :
-> Renseigner la "Circular configuration" telle que :
  >> Circular Mode = "Disable",

-> Renseigner la "Request Configuration" telle que :
  >> Request -> sélectionner "U(S)ART#_TX" (en remplaçant '#' par le numéro d'UART souhaité), "ADC1" ou "ADC2"
  >> DMA Handle in IP Structure -> sélectionner "hdmatx" si disponible,
  >> Block HW request protocol = "Single/Burst Level", "Single/Burst Level"

-> Renseigner la "Channel Configuration" telle que :
  >> Priority = "Low",
  >> Transaction Mode = "Normal"
  >> Direction = "Memory To Peripheral", ("Peripheral To Memory" for ADC)

-> Renseigner "Source Data Setting" tels que :
  >> Source Address Increment After Transfer = "Enabled" ("Disabled" for ADC)
  >> Data Width = "Byte" ("Half Word" for ADC)
  >> Burst Length = "1" ("1" for ADC)
  >> Allocated Port for Transfer = "Port0" ou "Port1"

-> Renseigner "Destination Data Setting" tels que :
  >> Destination Address Increment After Transfer = "Disabled", ("Enabled" for ADC)
  >> Data Width = "Byte" ("Half Word" for ADC)
  >> Burst Length = "1" ("1" for ADC)
  >> Allocated Port for Transfer = "Port0" ou "Port1"

-> Renseigner "Data Handling" tel que :
  >> Data Handling Configuration = "Disable", ("Disable" for ADC)

-> Renseigner "Transfer Event Configuration" tel que :
  >> Transfer Event Generation = "The TC (and HT) event is generated at the (respectively half) end of each block"

  Dans le paneau "Configuration" -> onglet "SECURITY" :
-> Vérifier que tous les Channels concernés pour le(s) U(S)ARTs souhaités soient indiqués tels que :
  >> "Enable Channel as Priviledged" = "NON PRIVILEDGED"

  Dans le paneau "Configuration" -> onglet "All Channels" :
-> Vérifier que tous les Channels concernés pour le(s) U(S)ARTs souhaités soient indiqués tels que :
  >> "Request" = "GPDMA*_REQUEST_UART#_TX"

I.3.6) Pour chaque U(S)ART#  pour ModBus, configurer les "NVIC Settings" tels que :
 -> "U(S)ART* interrupt" ou "U(S)ART* global interrupt" soit bien coché "Enabled"
 -> "GPDMAx Channel * global interrupt" (si GPDMA sélectionné) soit bien coché "Enabled"
 -> "DMAx ch* interrupt" (si DMA classique sélectionné) soit bien coché "Enabled"

I.3.7) Dans la Catégorie "System Core" -> "NVIC" -> Panel "Configuration" -> onglet "NVIC",
 s'assurer que, pour chaque U(S)ART# pour ModBus, soient bien configurés :

 -> "U(S)ART* interrupt" ou "U(S)ART* global interrupt" : coché "Enabled"
  + si proposé : laisser "Preemption Priority" à 0,
  + si proposé : laisser "Sub Priority" à 0.

 -> "GPDMAx Channel * global interrupt" (si GPDMA sélectionné) : coché "Enabled"
  + si proposé : laisser "Preemption Priority" à 0,
  + si proposé : laisser "Sub Priority" à 0.

 -> "DMAx channel * interrupt" (si DMA classique sélectionné) : coché "Enabled"
  + si proposé : laisser "Preemption Priority" à 0,
  + si proposé : laisser "Sub Priority" à 0.

I.3.8) Dans la Catégorie "System Core" -> "NVIC" -> Panel "Configuration" -> onglet "Code generation",
 s'assurer que, pour chaque U(S)ART# pour ModBus soient bien configurés :

 -> "U(S)ART* interrupt" ou "U(S)ART* global interrupt" :
  >> "Generate IRQ handler" : coché
  >> "Call HAL handler" : coché
  >> "Generate Enable in Init" (si la colonne est proposée) : coché

 -> "GPDMAx Channel * global interrupt" (si GPDMA sélectionné) :
  >> "Generate IRQ handler" : coché
  >> "Call HAL handler" : coché
  >> "Generate Enable in Init" (si la colonne est proposée) : coché

 -> "DMAx channel * interrupt" (si DMA classique sélectionné) :
  >> "Generate IRQ handler" : coché
  >> "Call HAL handler" : coché
  >> "Generate Enable in Init" (si la colonne est proposée) : coché


I.4) Pour toutes les Méthodes de Communication :
------------------------------------------------

I.4.1) Dans l'onglet général "Clock Configuration", s'assurer que la Clock vers chaque U(S)ART# soit à une
 fréquence suffisante, sans pour autant être en rouge. Dans la plupart des cas, 16 ou 32 MHz peut être suffisant.

Notes_Jp le 12/04/2022 sur la Clock : avec 2*UART @ 115200 => HCLK >= 25.6 MHz (21.33MHz insuffisant)
HCLK @ 25.6MHz + USART2 @ 16MHz + LPUART1 @ 16 MHz = presque OK pour UART2 + pb réguliers pour UART3
Essai identique avec "Preemption Priority" à 1 sur USART3 : semble identique essai précédent
Essai avec "Preemption Priority" à 0 sur USART3 et à 1 sur USART2 : semble OK sur UART3 + pb réguliers sur UART2
-> problème résolu avec HCLK @ 32MHz, même si UART1 & UART2 @ 16MHz avec "Preemption Priority" à 0 !

Remarque_Jp le 12/04/2022 : sur STM32G071RB, UART3 n'a pas de "Clock Source" configurable !
 -> il s'agit peut-être directement de PCLK, qui semble être la Clock par défaut(?) pour les autres UARTs...

I.4.2) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
 -> "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" -> coché
 -> "Keep User Code when re-generating" -> coché
 -> Je recommande de cocher également "Delete previously generated files when not re-generated".

I.4.3) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
 -> s'assurer que tous les Périphériques U(S)ART# pour cette Librairie soient de type "HAL",
 -> s'assurer que tous les GPDMAx (le cas échéant) pour cette Librairie soient de type "HAL"

I.4.4) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Generated Function Calls" :
 -> Pour chaque Périphérique U(S)ART# à utiliser, s'assurer que soient bien Cochés :
 "Generate Code" (à gauche) + "Do Not Generate Function Call" + "Visibility (Static)"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX.

Note_Jp : la Clock des DMA & GPDMA semble provenir directement de HCLK.

 +---------------------------------+
 | Etape II : Configurer "UartCom" |
 +---------------------------------+
===> Dans ce fichier "UartComConf.h" (cf. plus bas) :

II.1) Activer ou Désactiver "UART_COM_ENABLE_IBUS" pour iBus, si nécessaire
 Remarque : la fonctionnalité "iBus" requiert une version de la Librairie "iBus" compatible "UartCom".
 -> se reporter à "iBusConf.h" pour Configurer le module iBus.

II.2) Activer ou Désactiver "UART_COM_ENABLE_MODBUS_SLAVE" pour ModbusSlave, si nécessaire
 Remarque : la fonctionnalité "ModBusSlave" requiert une version de la Librairie "ModBusSlave" compatible "UartCom".
 -> se reporter à "ModbusSlaveConf.h" pour Configurer le module "Modbus Slave".

II.3) Activer ou Désactiver "UART_COM_ENABLE_MODBUS_MASTER" pour ModbusMaster, si nécessaire
 Remarque : la fonctionnalité "ModBusMaster" requiert une version de la Librairie "ModBusMaster" compatible "UartCom".
 -> se reporter à "ModbusMasterConf.h" pour Configurer le module "Modbus Master".

II.4) Activer ou Désactiver "UART_COM_SUPPORT_TX_RX_PIN" pour qu'une Pin soit basculée pendant le Tx

II.5) Activer ou Désactiver "UART_COM_SUPPORT_STATS" pour que des Compteurs de Statistiques soient mis à disposition

II.6) Adapter "UART_COM_MAX_REG_TX" au nombre maximal de Trames Tx différentes à gérer régulièrement ou sur demande.

II.7) ToDo : Poursuivre la description de la Configuration UartCom ...


 +---------------------------------------+
 | Etape III : Configurer la partie User |
 +---------------------------------------+
===> Dans le fichier User.h ("UartComUser.h") :

III.1) Activer (ou Ajouter), parmi les "USER CODE * Includes", l'Include des .h correspondant à
 chaque Type de Périphérique qui va être associé (par ex UART, USART, USB_Device ou USB_Host) :

Exemple 1 : en cas d'UART et/ou USART :
#include "usart.h"  		// Pour accès aux Variables & Fonctions d'Init UART & USART

Exemple 2 : en cas d'USB_Device :
#include "usb_device.h" 	// Pour accès aux Variables & Fonctions d'Init USB_DEVICE

Exemple 3 : en cas d'USB_Host :
#include "usb_host.h"		// Pour accès aux Variables & Fonctions d'Init USB_HOST

III.2) Décommenter & Configurer, dans la zone "USER CODE * LinkToDevices", les liens vers chaque
 Périphérique qui sera utilisé dans UartCom :

Exemple 1 : iBus Interne sur Uart1 :
#define UART_IBUS_INT		&huart1

Exemple 2 : iBus Externe sur Uart 3 :
#define UART_IBUS_EXT		&huart3

Exemple 3 : Modbus IHM sur Uart 2 :
#define UART_MODBUS_IHM 	&huart2

Exemple 4 : Modbus User sur Uart 5 :
#define UART_MODBUS_USER	&huart5

Exemple 5 : Modbus Externe sur Uart8 :
#define UART_MODBUS_EXT 	&huart8


 +---------------------------------+
 | Etape IV : Configurer le Projet |
 +---------------------------------+
===> Dans le nouveau Projet :

IV.1) Ajouter l'Include du .h dans le "main.c" (par exemple parmi les "USER CODE * Includes"),
  ainsi qu'une Base de Temps appropriée (par exemple dans "USER CODE * Includes" de "stm32*_it.c")
	#include "UartComCore.h"

IV.2) Ajouter l'appel d'Init Hardware dans le "main" du "main.c" (par exemple parmi les "USER CODE * 2") :
	UartCom_Devices_Init();				// A appeler dans la partie Init Hardware (main.c)

IV.3) Ajouter l'appel d'Init Logiciel dans le "main" du "main.c" (par exemple parmi les "USER CODE * 2") :
	UartCom_RunTime_Init();				// A appeler dans la partie Init Logiciel (main.c)

IV.4) Ajouter le Handler @ 1ms dans une Interruption (IT) à 1ms :
  (par exemple dans la zone "USER CODE * SysTick_IRQn 1" du "SysTick_Handler", dans "stm32*_it.c")
	UartCom_Handle_IT_1ms();			// A appeler dans l'Interruption @ 1ms

IV.5) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c" :
  (Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
	Gestion_UartCom();					// A appeler dans la Boucle Principale (main.c)

IV.6) Renseigner les Devices à supporter dans la zone "USER CODE * LinkToDevices" de "UartComUser.h",
  ainsi que leur répartition dans la zone "USER CODE * DevicesByUsage" de "UartComDevices.h".

IV.7) Pour intégrer le support iBus, ajouter une version compatible de la Librairie "iBusCom",
 par exemple celle du projet "HII_CarteMere_App", et la configurer selon les besoins.
-> se reporter à "iBusConf.h" pour plus de détails.

IV.8) Pour intégrer le support du "ModBus Slave", ajouter une version compatible de la Librairie
 "ModbusSlave", par exemple celle du projet "HII_CarteMere_App", et la configurer selon les besoins.
-> se reporter à "ModbusSlaveConf.h" pour plus de détails.

IV.9) Dans "UartComUser.c", configurer la variable "mUartComInitParam" et suivantes
  en fonction des éléments statiques ou dynamiques à charger l'Init ou ultérieurement

IV.10) Ajouter le Dossier "UartCom" à l' "IncludePath" pour toutes les Configs de Build.
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/UartCom" (sans les guillemets)
Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

IV.11) Vérifier que le Dossier "UartCom" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

IV.12) Ajouter au projet l'une des Librairie "Utils" compatibles, comme celle de "RMD_Firmware",
 "HII_Manta_App" ou "HII_CarteMere_App", et suivre les indications données dans "utils.h" pour réussir
  plus facilement son intégration dans votre Projet.

Info : Si besoin, il est possible de demander à cette Librairie "UartCom" la Dé-Initialisation
  de tous les Périphériques matériels en cours d'usage, via l'appel à la fonction :
// UartCom_Devices_DeInit()

Remarques sur les CallBacks Uart réquisitionnés par "UartCom" :
------------------------------------------------------------

	Cette Librairie "UartCom" réquisitionne le CallBack "HAL_UART_RxCpltCallback",
  mais il est possible d'ajouter un lien vers une autre fonction User via "HAL_UART_RxCpltCallback2"

	Le CallBack "HAL_UART_TxCpltCallback" est aussi réquisitionné, mais il est également
  possible d'ajouter un lien vers une autre fonction User via "HAL_UART_TxCpltCallback2"

	De même, le CallBack "HAL_UART_ErrorCallback" est également réquisitionné, mais il est
  possible d'ajouter un lien vers une autre fonction User via "HAL_UART_ErrorCallback2"


 +-------------------------------------------+
 | Etape V : Configurer le "build-job" du CI |
 +-------------------------------------------+
===> Dans le MakeFile :

V.1) Ajouter aux "C_SOURCES" le chemin relatif vers les fichiers "UartComCore.c" & "UartComUser.c",
 très probablement : "User/UartCom/UartComCore.c \" (sans les guillemets mais avec '\' en fin de ligne)
 ET : "User/UartCom/UartComUser.c \" (toujours sans les guillemets mais aussi avec '\' en fin de ligne)

Remarque : pour le chemin, il faut bien utiliser la bare oblique de la division '/' à la place du '\' habituel de Windows.

V.2) Ajouter aux "C_INCLUDES" le chemin relatif vers notre dossier "UartCom" avec le préfixe "-I" devant,
 très probablement : "-IUser/UartCom \" (sans les guillemets mais également avec '\' en fin de ligne)

Même Remarque : pour le chemin, utiliser la bare obliques de la division '/' à la place du '\' habituel de Windows.

=> Félicitations, c'est prêt :-) !

*************************************************************************************************/

// Paramètres User à Activer / Configurer :

//#define UART_COM_ENABLE_IBUS    			// Pour activer la liaison avec la Librairie "iBus"
#define UART_COM_ENABLE_MODBUS_SLAVE		// Pour activer la liaison avec la Librairie "ModBusSlave"
//#define UART_COM_ENABLE_MODBUS_MASTER		// Pour activer la liaison avec la Librairie "ModBusMaster"
#define UART_COM_ENABLE_EMBRACO_INVERTER	// Pour activer la liaison avec la Librairie "EmbracoInverter"

#define UART_COM_DISABLE_CHECK_ECHO			// Pour désactiver la gestion de l'écho *Rx = *Tx
#define UART_COM_IGNORE_EXCEDENT_INIT_PARAMS// Pour désactiver la Vérification des Inits excédentaires/mauvaise Config au RunTime

//#define UART_COM_HANDLE_RX_STD_CALLBACK 	// Pour activer le CallBack Rx classique "HAL_UART_RxCpltCallback" (via HAL_UART_Receive_IT)
#define UART_COM_HANDLE_RX_EVENT_CALLBACK	// Pour activer le CallBack Rx en DMA ou ReceiveToIdle => "HAL_UARTEx_RxEventCallback"
//#define UART_COM_HANDLE_RX_EXTERNAL_BLOCS	// Pour activer la Réception de Blocs depuis d'autres Périphériques (par ex : USB_D ou USB_H)

//#define UART_COM_SUPPORT_TX_RX_PIN  		// Pour activer la prise en charge d'une Pin de TxRx
//#define UART_COM_SUPPORT_STATS  			// Pour activer les compteurs de Stats dans UartCom
//#define UART_COM_START_SAB_EOF_AT_HT		// Pour activer le lancement du sabEndOfRxFrame au "Half Transfer" event

#define UART_COM_MAX_REG_TX 		1		// nb Max de RegularTx à Gérer (1 par Trame iBus Auto ou Callback )

/********************************************************************************************/

// Fonctions d'appel en Cascade si l'un de ces évènements concerne également un U(s)art non géré par UartCom :

//#define NEXT_HAL_UART_RxCpltCallback		USER_HAL_UART_RxCpltCallback2	// Remplacer par un nom User RxCpltCallback approprié
//#define NEXT_HAL_UART_TxCpltCallback		USER_HAL_UART_TxCpltCallback2	// Remplacer par un nom User TxCpltCallback approprié
//#define NEXT_HAL_UART_ErrorCallback 		USER_HAL_UART_ErrorCallback2	// Remplacer par un nom User ErrorCallback  approprié
//#define NEXT_HAL_UARTEx_RxEventCallback 	USER_HAL_UARTEx_RxEventCallback	// Remplacer par un nom User RxEventCallback approprié

/********************************************************************************************/

#include "UartComDevices.h"	// Pour accès à Répartition des Usages prévus
#include "UartComUtils.h"	// Pour accès aux Outils d'aide à la Configuration

#ifdef UART_COM_ENABLE_IBUS
	#include "iBusConf.h"		// Pour accès à la Config iBus
#endif // UART_COM_ENABLE_IBUS

#ifdef UART_COM_ENABLE_MODBUS_SLAVE
	#include "ModbusSlaveConf.h" // Pour accès à la Config Modbus Slave
#endif // UART_COM_ENABLE_MODBUS_SLAVE

#ifdef UART_COM_ENABLE_MODBUS_MASTER
	#include "ModbusMasterConf.h" // Pour accès à la Config Modbus Master
#endif // UART_COM_ENABLE_MODBUS_MASTER

//#include "UsbDeviceCdcConf.h"	// Pour accès à la Config USB Modbus

#ifdef UART_COM_ENABLE_EMBRACO_INVERTER
	#include "EmbracoInverterConf.h" // Pour accès à la Config de l'Inverter Embraco
#endif // UART_COM_ENABLE_EMBRACO_INVERTER

/********************************************************************************************/

// Synthèse (automatique) pour "UartComCore.c" des différentes Sources & Applications :
#define NB_OF_COM_INIT_PARAMS	UART_COM_NB_OF_DEVICES_USED	// Synthèse automatique issu de la Configuration dans "UartComDevices.h"
//#define NB_OF_UART_COM_IBUS 	NB_OF_IBUS				// None
//#define NB_OF_UART_COM_MODBUS	NB_OF_MODBUS_SLAVE_UART	// None
//#define NB_OF_USB_DEVICE_MODBUS	NB_OF_MODBUS_SLAVE_USB	// None

// Tableau des Liens à charger automatiquement à la Mise sous Tension (pendant UartCom_Devices_Init) :
UART_COM_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(tUartComInitParams, UC_BaseInitParam, UC_EndInitParams);
#define FIRST_COM_INIT_PARAM	UC_BaseInitParam
#define AFTER_COM_INIT_PARAM	UC_EndInitParams

// Fonction d'Init User appelé pendant UartCom_RunTime_Init :
#define UART_COM_INIT_USER_MST	UartCom_InitUserMST

// Dimensionnement de quelques Emplacements de travail internes à UartCom :
#define NB_FRAME_PARAMS_PER_COM_MANAGER		2	// En moyenne 2 FrameInfos par UART peuvent suffire
#define NB_SHARED_UART_COM_FRAME_PARAMS		5	// Habituellement 5 de plus sont largement suffisants

#define UART_COM_SHARED_BUF_TX_SIZE		500	// Taille du Buffer Interne partagé en cas de besoin exceptionnel

#define UART_COM_SUPPORT_FRAME_TTL			// Nécessaire : Activer le support de la durée de Vie d'une Trame

/********************************************************************************************/

// Paramètres pour Réinit Uart avec de nouveaux BaudRate/Parity/Stops :
#define UART_COM_SUPPORT_REINIT
#define UART_COM_SAB_APPLY_AT_MST	50	// Délai Apply = 5s, par pas de 100ms
#define UART_COM_SAB_APPLY_REINIT	50	// Délai Apply = 5s, par pas de 100ms
#define UART_COM_SAB_VALID_REINIT	900 // Délai pour Valider le changement = 90s, par pas de 100ms
#define UART_COM_SAB_RETRY_REINIT	50	// Délai Retry = 5s, par pas de 100ms
#define UART_COM_REINIT_VALID_KEY	0x5a5a // = 23130 = 2313 *10
#define UART_COM_FORCE_REINIT_NOW	0x500d // = 20493

extern UartReInitUserParams UartModbusUser_ReInitUserParams;
extern UartReInitUserParams UartModbusUser_ReInitRealParams;
extern UartReInitCoreVars	UartModbusUser_ReInitCoreVars;

UART_COM_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(UartReInitItem, UC_BaseUartReInitItems, UC_EndUartReInitItems);
#define FIRST_COM_REINIT_ITEMS	UC_BaseUartReInitItems
#define AFTER_COM_REINIT_ITEMS	UC_EndUartReInitItems

/********************************************************************************************/

// Exported functions prototypes :

#ifdef UART_COM_INIT_USER_MST
	void UART_COM_INIT_USER_MST(void);	// Pour que le Prototype de la fonction d'Init User soit toujours connu !
#endif /// UART_COM_INIT_USER_MST

#ifdef NEXT_HAL_UART_RxCpltCallback
	void NEXT_HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUart);	// Pour que le Prototype de la fonction User de remplacement soit toujours connu !
#endif // NEXT_HAL_UART_RxCpltCallback

#ifdef NEXT_HAL_UART_TxCpltCallback
	void NEXT_HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUart);	// Pour que le Prototype de la fonction User de remplacement soit toujours connu !
#endif // NEXT_HAL_UART_TxCpltCallback

#ifdef NEXT_HAL_UART_ErrorCallback
	void NEXT_HAL_UART_ErrorCallback(UART_HandleTypeDef *hUart);	// Pour que le Prototype de la fonction User de remplacement soit toujours connu !
#endif // NEXT_HAL_UART_ErrorCallback

#endif /* UART_COM_UART_COM_CONF_H_ */
