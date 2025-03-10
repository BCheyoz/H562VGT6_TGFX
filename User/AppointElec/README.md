# AppointElec cpp Lib v1.0

## History Usage 
* 10/03/2025 : Added by MF to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Configurer la (ou les) pin(s) concernée(s) en "GPIO_Output"

2) Dans "System Core" -> "GPIO" -> "GPIO Mode and Configuration" -> Onglet "GPIO", Pour chaque pin concernée, configurer les options ainsi :
	* GPIO output level = "Low"
    * GPIO mode = "Output Push Pull"
    * GPIO Pull-up/Pull-down = "No pull-up and no pull-down"
    * Maximum output speed = "Low" ("Low" est généralement suffisant pour notre usage)
	* User Label : Indiquer le nom de la fonction associée, 

3) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
    * s'assurer que GPIO soit bien configuré de type "HAL"

## Etape II : Configurer le Projet 

**Dans le nouveau Projet**

1) Ajouter le Dossier "AppointElec" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/AppointElec" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "LED_cpp" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*

3) Ajouter l'Include du .h dans le "main.cpp" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans la zone "USER CODE * Includes" du .c
 de la Librairie "BaseDeTemps") :
```
	#include "AppointElec.hpp"
```

4) Crée l'instance d'AppointElec  
Initialiser le construction dans la partie "init" du MainStateMachine ou du main.c  
Exemple:
```
  AppointElec* appointElec = new AppointElec(DO_Appoint_GPIO_Port, DO_Appoint_Pin);
```
  	* par défaut le pin est initialisé à E_APPOINT_ELEC_OFF
  
Acceder aux commande via l'instance de l'objet crée :
	* changement de l'état du pin :
Exemple activation :
```
	appointElec->SetMode(E_APPOINT_ELEC_ON);//E_APPOINT_ELEC_ON:1
```

Exemple désactivation :
```
	appointElec->SetMode(E_APPOINT_ELEC_OFF);//E_APPOINT_ELEC_OFF:0
```

	* récupération de l'état du pin :
Exemple :
```
	appointElec->GetMode();
```

Félicitations, c'est prêt :-) !

