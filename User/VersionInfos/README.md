# VersionInfos Lib v1.1


## History Usage 
* 30/07/2020 : Added by SF to IHM_Easykub_Firmware (STM32F722RC : productprojects/ventilation/ihm/easykub/FW_Easykub)
* 06/08/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
* 21/10/2021 : Added by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
* 23/12/2021 : Added by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 10/01/2024 : Added by Jp to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
* 03/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans "ProjectManager -> Code Generator" s'assurer des Paramètres suivants :
	* "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" -> Coché
	* "Keep User Code when re-generating" -> Coché
	* Je recommande de Cocher également "Delete previously generated files when not re-generated"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus est modifié !


## Etape II : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier "VersionInfos" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/VersionInfos" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "VersionInfos" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*

3) Ajouter l'Include du .h dans le "main.c/.cpp" (par exemple parmi les "USER CODE * Includes") :
```
	#include "VersionInfos.h"
```

4) Ajouter l'appel d'Init dans le "main" du "main.c/.cpp" (par exemple parmi les "USER CODE * 2") :
```
	InitComputeInfos();
```

5) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c/.cpp" :   
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	ComputeMyInfos();
```

6) S'il faut gérer les Reboot temporisés vers un BootLoader (avec VI_SUPPORT_JUMP_BL) :
  * Ajouter l'Include du .h dans la zone "USER CODE * Includes" du .c de la Librairie "BaseDeTemps"
  * Ajouter le Handler @ 100ms dans une sous-partie de la Boucle Principale (RT) à 100ms :
(par exemple dans la zone @ 100ms "USER CODE * RT_100ms" de "GestionBaseDeTemps" ("BaseDeTemps.c")
```
/* USER CODE BEGIN Includes */
	#include "VersionInfos.h"
/* USER CODE END Includes */

...

    /* USER CODE BEGIN RT_100ms */
	Handle_Infos_RT_100ms();
	/* USER CODE END RT_100ms */
```

7) Configurer les Fonctionnalités de signature CRC (FW et/ou BL) ou de Versionning BootLoader
(avec VI_SUPPORT_FW_CRC, VI_SUPPORT_BL_VERSION ou VI_SUPPORT_BL_CRC) :
    
    1) Il faut vérifier manuellement "VECTORS_BLOC_SIZE", à partir du "startup_stm32*.s" :
    localiser "g_pfnVectors" et compter le nombre de ".word" jusqu'au ".weak" ou ".thumb_set" suivant
    (Attention, il ne faut compter que les ligne commençant par le mot clef ".word")
    
    2) Si le calcul n'est pas automatique : Vérifier que "FLASH_MAX_SIZE" (dans "FlashInfo.h") corresponde bien à la propriété "LENGTH" de la ligne "FLASH" du "STM32*_FLASH.ld" (partie "MEMORY")   
    exemple : 
    
```
STM32*_FLASH.ld :
FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 512K 

FlashInfo.h
#define FLASH_MAX_SIZE  	(512 *1024)
```

## Etape III : Configurer la version 
**Dans le fichier "VersionInfos.h"**

1) Renseigner la Version du Soft via "VERSION_FW_SOFT" (cf. plus bas) :
 Remarque : je recommande de toujours commencer par une version FW Alpha 0.00.01

```
#define VERSION_FW_SOFT_TYPE    (0xA0)  // Bin type
#define VERSION_FW_SOFT_MAJOR   (0x00)  // major version
#define VERSION_FW_SOFT_MINOR   (0x00)  // Minor version
#define VERSION_FW_SOFT_DEV     (0x01)  // Dev version
```

2) Décommenter (ci-dessous) les différentes fonctionalités "VI_SUPPORT_*" souhaitées
 ATTENTION : les 2 fonctionnalités "VI_SUPPORT_FW_CRC" & "VI_SUPPORT_BL_CRC" requièrent
la Librairie "CRC", ou équivalent, pour calculer un CRC32.


Félicitations, c'est prêt :-) !

