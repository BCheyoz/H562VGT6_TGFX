# LedBlinker cpp Lib v1.1

Basé sur la libraire LedBlinker.c anciennement LedAlive.c

## History Usage 
* 06/12/2024 : Added by BC to Test_Aff_Nucleo_H563ZI (STM32H563ZI)
* 03/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


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

1) Ajouter le Dossier "LED_cpp" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/LED_cpp" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "LED_cpp" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*

3) Ajouter l'Include du .h dans le "main.cpp" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans la zone "USER CODE * Includes" du .c
 de la Librairie "BaseDeTemps") :
```
	#include "LedBlinkerInterface.h"
```

4) Ajouter le Handler @ 100ms dans une sous-partie de la Boucle Principale (RT) à 100ms :
(par exemple dans la zone @ 100ms "USER CODE * RT_100ms" de "GestionBaseDeTemps" ("BaseDeTemps.c")
```
	Handle_Led_RT_100ms();
```

5) Crée les instances par LED   
Appeler le construction dans la partie "init" du MainStateMachine ou du main.c    
Acceder aux commande via l'instance de l'objet crée
```
	LedBlinker(Status_Led_GPIO_Port, Status_Led_Pin, E_LED_SLOW_BLINK);
```


   
Félicitations, c'est prêt :-) !

