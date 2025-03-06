# CRC Lib v1.0


## History Usage 
* 14/01/2020 : Added by AA to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
* 09/04/2020 : Added by AB to IHM_Easykub_Firmware (STM32F722RC : productprojects/ventilation/ihm/easykub/FW_Easykub)
* 30/07/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
* 21/10/2021 : Added by Jp to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
* 23/12/2021 : Added by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 25/02/2022 : Added by AB to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
* 03/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


# Procédure pour intégrer cette Librairie

1) Ajouter l'Include du .h parmi ceux des Fichiers.c qui ont besoin des Fonctions de CRC :   
  (par exemple parmi les "USER CODE * Includes")
```
	#include "crc.h"
```


2) Activer ou mettre en commentaire les define ci-dessous suivant le(s) module(s) CRC
  souhaité(s) :    
  CRC_SUPPORT_CRC8 / CRC_SUPPORT_CRC16 / CRC_SUPPORT_CRC32

 3) Pour le module CRC32, appeler les fonctions "CreateCRC32_Table" et "FreeCRC32_Table"
  à un moment approprié, surtout avant le 1° appel à la fonction de calcul "CRC32" :   
```  
	CreateCRC32_Table();	// Pour Initialiser la Table de CRC32 avec le Polynôme choisi
	FreeCRC32_Table();		// Pour Libérer la Table de CRC32 lorsqu'elle n'est plus utile
```

 4) Ajouter le Dossier "CRC" à l' "IncludePath", pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/CRC" (sans les guillemets)

 5) Vérifier que le Dossier "CRC" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")   
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".
   
Félicitations, c'est prêt :-) !

