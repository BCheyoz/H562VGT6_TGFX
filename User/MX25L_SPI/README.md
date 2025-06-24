# Librairie `MX25L_SPI.c` v1.0.0
Copyright © ALDES 2025 -
Updated on 23 May 2025, by j.proux (@JoelP)


## History Usage :
* 14/01/2020 : Added by AA to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
* 14/06/2021 : Added by BC to CarteAqui_BestCC (STM32F732VE : innoprojects/carte-acquisition/banc-de-test-statique/best-cc)
* 04/08/2023 : Added by AB to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 10/01/2024 : Added by @JoelP to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
* 22/04/2025 : Added by @MatthieuF to [TFL4_CarteMere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app) (STM32H562VGT6)


# Procédure pour intégrer cette Librairie "**MX25L_SPI**"
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans CubeMX, configurer toutes les entrées & sorties concernées et leur attribuer  
 un Nom en fonction de leur usage prévu d'après le schéma.

Remarque : Sur le STM32H562VGT6 (et d'autres), le Hardware NSS ne fonctionne pas (toujours à `0`)  
 => Il faut Configurer la Pin de "ChipSelect" en tant que `GPIO Output` (cf. partie #)
https://community.st.com/t5/stm32-mcus-products/stm32-g4-spi-hardware-nss-with-nssp-diabled-does-not-work-only/td-p/127135

2) Pour chaque SPI# classique prévu pour communiquer avec une Mémoire de type MX25L, configurer le `Mode` suivant :
- Mode = `Full-Duplex Master`
- Hardware NSS Signal = `Disable` (-> sur le STM32H562VGT6, le Hardware NSS ne fonctionne pas !)
- Hardware RDY Signal = `Disable`

3) Pour chaque SPI# classique vers MX25L, Configurer les `Parameter Settings` > `Basic Parameters` suivants :
- Frame Format = `Motorola`
- Data Size = `8 Bits`
- First Bit = `MSB First`

4) Pour chaque SPI# classique vers MX25L, Configurer les `Parameter Settings` > `Clock Parameters` suivants :
- Prescaler (for Baud Rate) : Configurer pour que "BaudRate" soit d'environ `30.0 MBits/s` ou moins  
Remarque : Au besoin, ajuster la sélection du `Clock Mux` correspondant dans l'onglet général `Clock Configuration`.

- Clock Polarity (CPOL) = `Low`
- Clock Phase (CPHA) = `1 Edge`

5) Pour chaque SPI# classique vers MX25L, Configurer les `Parameter Settings` > `CRC Parameters` suivants :
- CRC Calculation = `Disabled`

6) Pour chaque SPI# classique vers MX25L, Configurer les `Parameter Settings` > `Advanced Parameters` suivants :
- NSSP Mode = `Disabled`
- NSS Signal Type = `Software`
- Fifo Threshold = `Fifo Threshold 01 Data`
- Nss Polarity = `Nss Polarity Low`
- Master Ss Idleness = `00 Cycle`
- Master Inter Data Idleness = `00 Cycle`
- Master Receiver Auto Susp = `Master Keep Io State Disable`
- IO Swap = `Disabled`
- Ready Master Management = `Internal`
- Ready Signal Polarity = `High`

7) Pour chaque SPI# classique vers MX25L, Configurer ainsi les `GPIO Settings` des Pins SCK, MISO & MOSI :
- GPIO mode = `Alternate Function Push Pull`
- GPIO Pull-up/Pull-down = `No pull-up and no pull-down`
- Maximum output speed = `Very High`

8) Dans la catégorie `GPIO` Configurer ainsi la `GPIO` de la Pin de ChipSelect :
- GPIO output level = `High`
- GPIO mode = `Output Push Pull`
- GPIO Pull-up/Pull-down = `No pull-up and no pull-down`
- Maximum output speed = `Very High`

9) Dans l'onglet général `Project Manager` > `Code Generator` > `Generated files` s'assurer des Paramètres suivants :
[x] `Generate peripheral initialization as a pair of '.c/.h' files per peripheral` = Coché
[x] `Keep User Code when re-generating` = Coché
[x] `Delete previously generated files when not re-generated` = Coché.

10) Dans l'onglet général `Project Manager` > `Advanded Settings` > `Driver Selector` :
- s'assurer que tous les Périphériques SPI# pour cette Librairie soient de type `HAL`.

11) Dans l'onglet général `Project Manager` > `Advanded Settings` > `Generated Function Calls`,
- Pour chaque Périphérique SPI# à utiliser, s'assurer que soient bien Cochés :  
[x] `Generate Code` (à gauche) = Coché
[x] `Do Not Generate Function Call` = Coché
[x] `Visibility (Static)` = Coché


## Etape II : Configurer le fichier `MX25L_SPI.h` :

1) Configurer les `*_RDID_*` attendus selon les infos du Fabricant :
* `SPI_MEM_MX25L_RDID_MFG` <=> Manufacturer ID ($C2 = Macronix)
* `SPI_MEM_MX25L_RDID_MMT` <=> Memory Type
* `SPI_MEM_MX25L_RDID_MMD` <=> Memory Density (0x16 = 4Mo/32Mbits, 0x17 = 8Mo/64Mbits)


## Etape III : Configurer le fichier `MX25L_SPI.c` :
* `MEM_MX25L_PERIF_INIT` : Nom de la fonction d'Init de la SPI générée par CubeMX
* `MEM_MX25L_PERIF_HANDLE` : Adresse du SPI_HandleTypeDef du Périphérique correspondant
* `MEM_MX25L_CS_PORT`	: *_GPIO_Port du ChipSelect, d'après le `main.h` généré par CubeMX
* `MEM_MX25L_CS_PIN`	: *_Pin du ChipSelect, d'après le `main.h` généré par CubeMX


## Etape III : Configurer le projet
**Dans le nouveau Projet**

1) Ajouter l'Include du .h dans le `main.c/.cpp` (par exemple parmi les `USER CODE * Includes`),  
#include "MX25L_SPI.h"			// Pour accès aux fonctions d'Init & d'Accès SPI à la Mémoire MX25L

2) Ajouter l'appel d'Init dans le `main` du `main.c/.cpp` (par exemple parmi les `USER CODE * 2`) :
Mem_MX25L_Init();

3) Ajouter le Dossier `MX25L_SPI` à l' `IncludePath` pour toutes les Configs de Build.

>[!warning] Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier !  
> => il est souvent préférable d'utiliser la formulation `../User/MX25L_SPI`.

>[!info] Info : Pour vérifier, sélectionner les `Properties` du Projet > `C/C++ Build` > `Settings`, puis pour chaque Config :  
> Dans l'onglet `Tool Settings`, aller à `MCU GCC Compiler` (éventuellement `MCU/MPU GCC Compiler`) > `Include paths`,  
> Dans l'onglet `Tool Settings`, aller à `MCU GCC Assembler` (éventuellement `MCU/MPU GCC Assembler`) > `Include paths`,  
> Dans l'onglet `Tool Settings`, aller à `MCU G++ Compiler` (éventuellement `MCU/MPU G++ Compiler`) > `Include paths`.

>[!tip] Ne pas hésiter à replacer les `Includes` dans l'ordre Alphabétique des chemins, pour éviter des ajouts inutiles.

4) Vérifier que le Dossier `MX25L_SPI` ne soit `Exclude From Build` d'aucune des Configs.  
  (y compris `Debug` & `Release`)

>[!info] Info : Pour vérifier, sélectionner les `Properties` du Dossier > `C/C++ Build` > `Settings`.


## Etape IV : Configurer le "build-job" du CI

**Dans le MakeFile :**

1) Ajouter aux `C_SOURCES` le chemin relatif vers le fichier `MX25L_SPI.c`,  
 très probablement : `User/MX25L_SPI/MX25L_SPI.c \` (avec '\' en fin de ligne)

Remarque : pour le chemin, il faut bien utiliser la bare oblique "slash" de la division '/', comme sur Linux.

2) Ajouter aux `C_INCLUDES` le chemin relatif vers notre dossier `MX25L_SPI` avec le préfixe `-I` devant,  
 très probablement : `-IUser/MX25L_SPI \` (également avec '\' en fin de ligne)

Même Remarque : pour le chemin, utiliser la bare oblique "slash" de la division '/', comme sur Linux.


>[!success] Félicitations, 
 c'est prêt :-) !
