# I2cDevPressureSDP8 Lib v1.0


## History Usage 
* 05/10/2021 : Added by Jp to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
* 10/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie
## Etape I : Configurer le "Gestionnaire I2C" 

* Pour rappel, un "Gestionnaire I2C" est l'association des fichiers .c & .h qui permet la Gestion d'un I2C en particulier, via l'appel des fonctions appropriées de la Librairie "I2cComMaster".  
Exemple d'un Gestionnaire I2C appelé "Internal" : "I2cComMasterInternal.c" & "I2cComMasterInternal.h"  
Exemple d'un Gestionnaire I2C appelé "External" : "I2cComMasterExternal.c"	& "I2cComMasterExternal.h"  
Exemple d'un Gestionnaire I2C appelé "System"	 : "I2cComMasterSystem.c"	& "I2cComMasterSystem.h"  
Exemple d'un Gestionnaire I2C appelé "SoftPort" : "I2cComMasterSoftPort.c"	& "I2cComMasterSoftPort.h"  

1) Ajouter l'Include de ce fichier dans la zone "USER CODE Includes", du .h de votre Gestionnaire I2C :
    ```
	#include "I2cDevPressureSDP8.h"		// Pour accès au Capteur de Pression SDP_8xx
    ```
    
2) Ajouter la référence à votre/vos Variable(s) Publique(s) de Sortie dans la zone "USER CODE Public_Vars" du même .h :
    ```
	extern I2CCM_Pres_SDP8_ExtData	mPresSDP8;	// Exemple d'une Variable Publique "mPresSDP8"
    ```
3) Ajouter votre/vos Variable(s) Publique(s) de Sortie dans la zone "USER CODE Public_Vars" du .c de votre Gestionnaire :
    ```
	I2CCM_Pres_SDP8_ExtData mPresSDP8 = {0};	// Exemple d'une Variable Publique "mPresSDP8"
    ```
4) Ajouter la ligne de Configuration à votre "InitTable",  dans la zone "USER CODE InitTable" de ce même .c :  
    * le 1° Paramètre est l'identifiant de Groupe, parmi "I2CCM_*DeviceGroupId", dans lequel s'inscrit le fonctionnement de ce Capteur,
       par exemple "I2cIntDevGrpPressure", "I2cExtDevGrpPressure" ou "I2cSysDevGrpPressure"
    * le 2° Paramètre est le "chemin" d'accès au Capteur, en cas de Multiplexeur I2C de type "PCA9546AD".
       La valeur "I2CCM_CHANNEL_NONE" permet d'ignorer cette fonctionnalité.
    * le 3° Paramètre identifie le Type de Capteur à charger. Il y a 2 méthodes possibles :
       - Soit demander le chargement d'un Type en particulier, en spécifiant son Indice, par exemple "I2cDevPresType_SDP8_8x0", ou "I2cDevPresType_SDP8_8x1",
       - Soit demander à la Librairie de charger par ordre de suggestion, à l'aide de "I2CCM_LOAD_DEFAULT_DEVICE" ou "I2CCM_LOAD_DEFAULT_DEVICE2" ...
    * le 4° Paramètre est le nom de la Fonction à appeler pour Charger notre module : "i2cCM_PressureSDP8_Init"
    * le 5° Paramètre est un Identifiant de Type dans la Famille "I2CCM_DevicePressureFamily", par exemple "I2cDevPresFamilySDP8"
    * le 6° Paramètre est un Index d'accès (base 0) dans la Structure des Variables Interne, en cas de multiple Devices identiques à gérer en parallèle
    * le 7° Paramètre est le Pointeur vers notre Variable Publique de sortie, de type "I2CCM_Pres_SDP8_ExtData"  
    
    ```
    // Exemple 1 : Groupe "Internal Pressure", pas de Multiplexeur I2C, Capteur par défaut, Famille SDP8, VariableInterne #0, Sortie sur "mPresSDP8" :
	 { I2cIntDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureSDP8_Init, I2cDevPresFamilySDP8,	0, &mPresSDP8 },

    // Exemple 2 : Groupe "External Pressure", pas de Multiplexeur I2C, Capteur par défaut, Famille SDP8, VariableInterne #0, Sortie sur "mXtPresSDP8" :
 	{ I2cExtDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureSDP8_Init, I2cDevPresFamilySDP8,	0, &mXtPresSDP8 },

    // Exemple 3 : Groupe "System Pressure", pas de Multiplexeur I2C, Capteur par défaut, Famille SDP8, VariableInterne #1, Sortie sur "mSysPresSDP8" :
 	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureSDP8_Init, I2cDevPresFamilySDP8,	1, &mSysPresSDP8 },
    ```

## Etape II : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier "I2c_Press_SDP8" à l' "IncludePath" pour toutes les Configs de Build.  
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)    
 il est souvent préférable d'utiliser la formulation "../User/I2c_Press_SDP8" (sans les guillemets)

Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

2) Vérifier que le Dossier "I2c_Press_SDP8" ne soit "Exclude From Build" d'aucune des Configs.  
  (y compris "Debug" & "Release")  
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".  



Félicitations, c'est prêt :-) !

