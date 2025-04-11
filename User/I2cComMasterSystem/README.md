# I2cComMasterSys Lib v1.0


## History Usage 
* 08/11/2022 : Merge by Jp to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 10/02/2025 : Added by Mf  to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie
## Etape I : Configurer le "Gestionnaire I2C" 

* Pour rappel, un "Gestionnaire I2C" est l'association des fichiers .c & .h qui permet la Gestion d'un I2C en particulier, via l'appel des fonctions appropriées de la Librairie "I2cComMaster".  
Exemple d'un Gestionnaire I2C appelé "Internal" : "I2cComMasterInternal.c" & "I2cComMasterInternal.h"  
Exemple d'un Gestionnaire I2C appelé "External" : "I2cComMasterExternal.c"	& "I2cComMasterExternal.h"  
Exemple d'un Gestionnaire I2C appelé "System"	 : "I2cComMasterSystem.c"	& "I2cComMasterSystem.h"  
Exemple d'un Gestionnaire I2C appelé "SoftPort" : "I2cComMasterSoftPort.c"	& "I2cComMasterSoftPort.h"  

Remarque : Ce fichier, dit de "Configuration", permet de faire le lien entre ce Gestionnaire I2C et les Variables & Fonctions globales spécifiques qui doivent lui être associées, notamment via les HAL.

* Il y a normalement 1 fichier de Configuration spécifique par Gestionnaire I2C.  
Exemple de fichier de Configuration pour un Gestionnaire I2C appelé "Internal"	: "I2cComMasterIntConf.h"  
Exemple de fichier de Configuration pour un Gestionnaire I2C appelé "External"	: "I2cComMasterExtConf.h"  
Exemple de fichier de Configuration pour un Gestionnaire I2C appelé "System"	: "I2cComMasterSysConf.h"

1) Inclure ci-dessous (cf. plus bas) le Header où sont déclarés Handle & Fonction d'Init du matériel requis :
    ```
	#include "i2c.h"	// Pour accès aux Variables & Fonctions d'Init de la HAL I2C
    ```
    
2) Définir comme "I2CCM_HANDLE" le Handle de la Structure de référence du matériel requis :
    ```
	#define I2CCM_HANDLE		&hi2c1			// Pour utiliser le Périphrique matériel I2C1
	#define I2CCM_HANDLE		&hi2c2			// Pour utiliser le Périphrique matériel I2C2
	#define I2CCM_HANDLE		&hi2c4			// Pour utiliser le Périphrique matériel I2C4
    ```
3) Définir comme "I2CCM_FN_INIT" la Fonction d'Init du Périphrique matériel I2C sélectionné :
    ```
	#define I2CCM_FN_INIT		MX_I2C1_Init	// Pour utiliser le Périphrique matériel I2C1
	#define I2CCM_FN_INIT		MX_I2C2_Init	// Pour utiliser le Périphrique matériel I2C2
	#define I2CCM_FN_INIT		MX_I2C4_Init	// Pour utiliser le Périphrique matériel I2C4
    ```
4) Définir "I2CCM_RESET_PIN" & "I2CCM_RESET_PORT" comme éventuelle Pin de "Reset" I2C (cf. CubeMx / main.h)

5) Définir dans "I2CCM_MAX_DEV_SIM" le nombre max de Devices (ou de "Groupes") à supporter simultanément

6) Configurer les fichiers .c & .h de ce Gestionnaire I2C suivant les recommandation de chaque Capteur I2C ciblé


## Etape II : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier de ce "Gestionnaire I2C" à l' "IncludePath" pour toutes les Configs de Build.  
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)  
 Exemple pour un Gestionnaire I2C appelé "Internal"	: "../User/I2cComMasterInternal" (sans les guillemets)  
 Exemple pour un Gestionnaire I2C appelé "External"	: "../User/I2cComMasterExternal" (sans les guillemets)  
 Exemple pour un Gestionnaire I2C appelé "System"	: "../User/I2cComMasterSystem" (sans les guillemets)  
 Exemple pour un Gestionnaire I2C appelé "SoftPort"	: "../User/I2cComMasterSoftPort" (sans les guillemets)  

Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings".

2) Vérifier que le Dossier de ce "Gestionnaire I2C" ne soit "Exclude From Build" d'aucune des Configs.  
  (y compris "Debug" & "Release")  
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".  



Félicitations, c'est prêt :-) !

