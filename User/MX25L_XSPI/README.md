# Librairie `MX25L_XSPI.c` v1.0.0
Copyright © ALDES 2025 -
Updated on 23 May 2025, by j.proux (@JoelP)


## History Usage :
* 13/05/2025 : Added by @JoelP to [TFL4_CarteMere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app) (STM32H562VGT6)


# Procédure pour intégrer cette Librairie "**MX25L_XSPI**"
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans CubeMX, configurer toutes les entrées & sorties concernées et leur attribuer  
 un Nom en fonction de leur usage prévu d'après le schéma.

2) Pour chaque OCTOSPI# prévu pour communiquer en XSPI "Quad SPI" avec une Mémoire de type MX25L,
 Configurer le `Mode` suivant :
- Mode = `Quad SPI`
- HyperBus(TM) 1.8V Inverted Clock : PAS coché
- Data = `IO[3:0]`

3) Pour chaque OCTOSPI# prévu pour communiquer en XSPI "Quad SPI" avec une Mémoire de type MX25L,
 Configurer les `Parameter Settings` > `Generic` suivants :
- Fifo Threshold = `1`
- MemoryMode = `Single`
- Memory Type = `Macronix`
- Memory Size : selon la taille de la Mémoire qui sera physiquement associée,  
 Par exemple : `64 MBits` pour une `MX25L6433` ou `32 MBits` pour une `MX25L3233`
- Device Type = `Flash`
- Chip Select High Time : `3` semble bien fonctionner
- Free Running Clock = `Disable`
- Clock Mode : `Low` semble bien fonctionner
- Wrap size = `Not Supported`
- Clcok Prescaler : `2` semble bien fonctionner
- Sample Shifting : `None` semble bien fonctionner
- Delay Hold Quarter Cycle : `Enable` semble bien fonctionner
- Chip Select Boundary : `Disable` semble bien fonctionner
- Refresh Rate : `0` semble bien fonctionner

3) Pour chaque OCTOSPI# prévu pour communiquer en XSPI "Quad SPI" avec une Mémoire de type MX25L,
 Configurer ainsi les `GPIO Settings` des Pins CLK, IO0, IO1, IO2, IO0 & NCS :
- GPIO mode = `Alternate Function Push Pull`
- GPIO Pull-up/Pull-down = `No pull-up and no pull-down`
- Maximum output speed = `Very High`

4) Dans l'onglet général `Project Manager` > `Code Generator` > `Generated files` s'assurer des Paramètres suivants :
[x] `Generate peripheral initialization as a pair of '.c/.h' files per peripheral` = Coché
[x] `Keep User Code when re-generating` = Coché
[x] `Delete previously generated files when not re-generated` = Coché.

10) Dans l'onglet général `Project Manager` > `Advanded Settings` > `Driver Selector` :
- s'assurer que tous les Périphériques OCTOSPI# pour cette Librairie soient de type `HAL`.

11) Dans l'onglet général `Project Manager` > `Advanded Settings` > `Generated Function Calls`,
- Pour chaque Périphérique OCTOSPI# à utiliser, s'assurer que soient bien Cochés :  
[x] `Generate Code` (à gauche) = Coché
[x] `Do Not Generate Function Call` = Coché
[x] `Visibility (Static)` = Coché


## Etape II : Dans le fichier `MX25L_XSPI.h` :

1) Configurer les Fonctions XSPI autorisées (permet de réduire la taille de celles pas utilisées) :
* `MEM_MX25L_XSPI_SUPPORT_2_LINES`			<=> Pour activer le support des fonctions en DualMode
* `MEM_MX25L_XSPI_SUPPORT_4_LINES`			<=> Pour activer le support des fonctions en QuadMode
* `MEM_MX25L_XSPI_SUPPORT_READ_RES_REMS`	<=> Pour activer le support des fonctions Read "Signature" (RES) & "Manufacturer & Device ID" (REMS)
* `MEM_MX25L_XSPI_SUPPORT_CHIP_ERASE`		<=> Pour activer le support du ChipErase
* `MEM_MX25L_XSPI_SUPPORT_PROGRAM_SUSPEND`	<=> Pour activer le support des fonctions Suspend & Resume Program
* `MEM_MX25L_XSPI_SUPPORT_ERASE_SUSPEND`	<=> Pour activer le support des fonctions Suspend & Resume Erase
* `MEM_MX25L_XSPI_SUPPORT_DEEP_POWER`		<=> Pour activer le support des fonctions de Deep Power
* `MEM_MX25L_XSPI_SUPPORT_SECURED_OTP`		<=> Pour activer le support des fonctions de Secure OTP
* `MEM_MX25L_XSPI_SUPPORT_DISCOVER_PARAMS`	<=> Pour activer le support de la fonction de DiscoverParameter

2) Configurer les éventuelles Opérations de Configuration à effectuer lors de l'Init :
* `MEM_MX25L_XSPI_USE_CONFIG_DUMMY_CYCLES`		<=> Pour utiliser ConfigRegister.DC comme DummyCycle Automatique en TwoRead & QuadRead
* `MEM_MX25L_XSPI_CONFIG_QUAD_ENABLE_AT_INIT`	<=> Pour activer le mode Quad de la Mémoire
* `MEM_MX25L_XSPI_CONFIG_DUMMY_CYCLES_AT_INIT`	<=> Pour activer les DummyCycles correspondants aux Vitesses élevées
* `MEM_MX25L_XSPI_CONFIG_ODS_AT_INIT`			<=> Pour configurer la valeur du "Output Driver Strength"

3) Configurer les liens vers le Périphérique XSPI à utiliser :
* `MEM_MX25L_XSPI_PERIF_HANDLE` : Adresse du XSPI_HandleTypeDef du Périphérique correspondant
* `MEM_MX25L_XSPI_PERIF_INIT` : Nom de la fonction d'Init de la XSPI générée par CubeMX
* `MEM_MX25L_XSPI_CS_INIT`	: Eventuelles Opérations complémentaires à effectuer après l'Init
* `MEM_MX25L_XSPI_CS_PORT`	: *_GPIO_Port du ChipSelect, d'après le `main.h` généré par CubeMX
* `MEM_MX25L_XSPI_CS_PIN`	: *_Pin du ChipSelect, d'après le `main.h` généré par CubeMX

4) Configurer les `*_RDID_*` attendus selon les infos du Fabricant :
* `MEM_MX25L_XSPI_RDID_MFG` <=> Manufacturer ID ($C2 = Macronix)
* `MEM_MX25L_XSPI_RDID_MMT` <=> Memory Type
* `MEM_MX25L_XSPI_RDID_MMD` <=> Memory Density (0x16 = 4Mo/32Mbits, 0x17 = 8Mo/64Mbits)


## Etape III : Configurer le projet

**Dans le nouveau Projet**

1) Ajouter l'Include du .h dans le `main.c/.cpp` (par exemple parmi les `USER CODE * Includes`),  
#include "MX25L_XSPI.h"			// Pour accès aux fonctions d'Init & d'Accès XSPI à la Mémoire MX25L


2) Ajouter l'appel d'Init dans le `main` du `main.c/.cpp` (par exemple parmi les `USER CODE * 2`) :
Mem_MX25L_XSPI_Init();

3) Ajouter le Dossier "Mem_MX25L_XSPI" à l' "IncludePath" pour toutes les Configs de Build.

>[!warning] Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier !  
> => il est souvent préférable d'utiliser la formulation `../User/MX25L_XSPI`.

>[!info] Info : Pour vérifier, sélectionner les `Properties` du Projet > `C/C++ Build` > `Settings`, puis pour chaque Config :  
> Dans l'onglet `Tool Settings`, aller à `MCU GCC Compiler` (éventuellement `MCU/MPU GCC Compiler`) > `Include paths`,  
> Dans l'onglet `Tool Settings`, aller à `MCU GCC Assembler` (éventuellement `MCU/MPU GCC Assembler`) > `Include paths`,  
> Dans l'onglet `Tool Settings`, aller à `MCU G++ Compiler` (éventuellement `MCU/MPU G++ Compiler`) > `Include paths`.

>[!tip] Ne pas hésiter à replacer les `Includes` dans l'ordre Alphabétique des chemins, pour éviter des ajouts inutiles.

4) Vérifier que le Dossier `MX25L_XSPI` ne soit `Exclude From Build` d'aucune des Configs.  
  (y compris `Debug` & `Release`)

>[!info] Info : Pour vérifier, sélectionner les `Properties` du Dossier > `C/C++ Build` > `Settings`.


## Etape IV : Configurer le "build-job" du CI

**Dans le MakeFile :**

1) Ajouter aux `C_SOURCES` le chemin relatif vers le fichier `MX25L_XSPI.c`,  
 très probablement : `User/MX25L_XSPI/MX25L_XSPI.c \` (avec '\' en fin de ligne)

Remarque : pour le chemin, il faut bien utiliser la bare oblique "slash" de la division '/', comme sur Linux.

2) Ajouter aux `C_INCLUDES` le chemin relatif vers notre dossier `UartCom` avec le préfixe `-I` devant,  
 très probablement : `-IUser/MX25L_XSPI \` (également avec '\' en fin de ligne)

Même Remarque : pour le chemin, utiliser la bare oblique "slash" de la division '/', comme sur Linux.


>[!success] Félicitations, 
 c'est prêt :-) !
