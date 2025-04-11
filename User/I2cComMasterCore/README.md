# I2cComMasterCore Lib v1.0


## History Usage 
* 05/10/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
* 26/10/2021 : Added by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
* 30/03/2022 : Added by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 13/04/2022 : Added by Jp to SensorsAcquisition_G071RB (STM32G071RBTx : innoprojects/sensors-acquisition/firmware-kit-stm32g071rb)
* 02/09/2022 : Added by Jp to Himalaya1_Carte6_App (STM32F051K8 : productprojects/ventilation/individuel/Himalaya1/Carte6/FirmwareV6)
* 20/10/2022 : Merge by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
* 08/11/2022 : Merge by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 10/02/2025 : Added by Mf  to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans CubeMX, configurer toutes les entrées & sorties I2C concernées (*SCL & *SDA) et
 leur attribuer un Nom commençant par "I2C_" en fonction de l'usage prévu sur le schéma.

2) Configurer chaque Pin concernée de "GPIO* I2C" telle que :
    * GPIO mode = "Alternate Function Open Drain"
    * GPIO Pull-up/Pull-down = "No pull-up and no pull-down" (OU "Pull-up" si les résistances SDA/SCL sont absentes)
    * Maximum output speed = "Low" devrait être suffisant

3) Pour chacun des Périphériques I2C# concernés, configurer les "Parameter Settings" -> "Timing configuration" :
    * Custom Timing (si proposé) = "Disabled"
    * I2C Speed Mode = "Standard Mode"
    * I2C Speed Frequency (KHz) = "100" // La plupart du temps, 100KHz est recommandé
    * Rise Time (ms) = 0
    * Fall Time (ms) = 0
    * Coefficient of Digital Filter = 0
    * Analog Filter = "Enabled"  
     (ignorer le paramètre "Timing" : s'il est affiché, il est probablement déjà grisé)

4) Pour chacun des Périphériques I2C# concernés, configurer les "Parameter Settings" -> "Slave Features" :
    * Clock No Stretch Mode = "Disabled"
    * General Call Address = "Disabled"
    * Primary Address Length selection = "7-bit"
    * Dual Address Acknowledged = "Disabled"
    * Primary slave address = "0"

5) Dans l'onglet général "Clock Configuration", s'assurer que la Clock "I2Cx" soit à une fréquence suffisante,
 par exemple entre 42 et 64 MHz, sans pour autant être indiqué en rouge  
 (remarque_Jp le 08/11/2022 : 30MHz pourrait être suffisant ... à vérifier)

6) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
    * "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" -> Coché
    * "Keep User Code when re-generating" -> Coché

7) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
    * s'assurer que tous les Périphériques I2C pour cette Librairie soient bien de type "HAL"

8) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Generated Function Calls" :
    * Pour chaque Périphérique I2C à utiliser, s'assurer que soient bien cochés :   
    "Generate Code" + "Do Not Generate Function Call" + "Visibility (Static)"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX


## Etape II : Configurer la lib 
**Dans le fichier "I2cComMasterConf.h"**

1) Indiquer ci-dessous (cf. plus bas), pour chaque type de Capteur supporté, le nombre max
  de Capteurs à gérer simultanément sur l'ensemble des I2C associés.

## Etape III : Configurer la partie User pour votre "Gestionnaire I2C"   
J'appele "Gestionnaire I2C" l'association des fichiers .c & .h qui permettent la Gestion d'un I2C en particulier, via l'appel des fonctions appropriées de la Librairie "I2cComMaster".   
* Exemple d'un Gestionnaire I2C appelé "Internal"	: "I2cComMasterInternal.c"	& "I2cComMasterInternal.h"  
* Exemple d'un Gestionnaire I2C appelé "External"	: "I2cComMasterExternal.c"	& "I2cComMasterExternal.h"  
* Exemple d'un Gestionnaire I2C appelé "System"	: "I2cComMasterSystem.c"	& "I2cComMasterSystem.h"  
* Exemple d'un Gestionnaire I2C appelé "SoftPorts"	: "I2cComMasterSoftPorts.c"	& "I2cComMasterSoftPorts.h"  

1) Ajouter l'Include correspondant dans la zone "USER CODE * Includes" du "main.c" :
    ```
	#include "I2cComMasterInternal.h"	// Pour un Gestionnaire I2C appelé "Internal"
	#include "I2cComMasterExternal.h"	// Pour un Gestionnaire I2C appelé "External"
	#include "I2cComMasterSystem.h" 	// Pour un Gestionnaire I2C appelé "System"
	#include "I2cComMasterSoftPorts.h"	// Pour un Gestionnaire I2C appelé "SoftPorts"
    ```
2) Ajouter l'appel d'Init correspondant dans la zone "USER CODE * 2" du "main.c" :
    ```
	I2cComMaster_Init_Internal();	// Pour un Gestionnaire I2C appelé "Internal"
	I2cComMaster_Init_External();	// Pour un Gestionnaire I2C appelé "External"
	I2cComMaster_Init_System(); 	// Pour un Gestionnaire I2C appelé "System"
	I2cComMaster_Init_SoftPorts();	// Pour un Gestionnaire I2C appelé "SoftPorts"
    ```
3) Ajouter l'appel de Gestion correspondant dans la Boucle Principale du "main.c" :  
  (Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
    ```
	GestionI2cInternal();	// Pour un Gestionnaire I2C appelé "Internal"
	GestionI2cExternal();	// Pour un Gestionnaire I2C appelé "External"
	GestionI2cSystem(); 	// Pour un Gestionnaire I2C appelé "System"
	GestionI2cSoftPorts();	// Pour un Gestionnaire I2C appelé "SoftPorts"
    ```
## Etape IV : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier "I2cComMasterCore" à l' "IncludePath" pour toutes les Configs de Build.
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
    * il est souvent préférable d'utiliser la formulation "../User/I2cComMasterCore" (sans les guillemets)  
    Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings".

2) Vérifier que le Dossier "I2cComMasterCore" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")  
    Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

3) Vérifier que le Module "Utils" soit bien présent dans le Projet (cf. "utils.h"), que les macros "JOIN_32_32_BE", "JOIN_16_16_BE" et "JOIN_8_8_BE" soit bien définies dans "utils.h", et que les fonctions "FillMemory" & "CopyMemory" soient bien exposées dans "utils.h", et implémentées dans "utils.c".



Félicitations, c'est prêt :-) !

