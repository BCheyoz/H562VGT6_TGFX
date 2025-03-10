# DigitalInputs v1.0
## lib cpp

héritage de la librairie DigitalInputs en c de joel.p

## History Usage 
* 06/02/2025 : Added by MF to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie 
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans CubeMX, configurer toutes les entrées Numériques concernées comme GPIO_Input
 et leur attribuer un Nom commençant par "Di_" en fonction de leur usage prévu sur le schéma.

2) Configurer chaque Pin numérique concernée de "GPIO -> GPIO" telle que :
	* GPIO mode = "Input mode"
 	* GPIO Pull-up/Pull-down = "No pull-up and no pull-down"

3) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
	* "Generate peripheral initialization as a pair of '.c//h' files per peripheral" -> coché
	* "Keep User Code when re-generating" -> coché

4) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
	* s'assurer que "GPIO" soit de type "HAL"

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX

## Attention au code généré pour un projet en c++ !!!!! il faut trier ce qui est utile ou pas

## Etape II : Configurer le projet

**Dans le nouveau Projet**

1) Ajouter le Dossier "DigitalInputs" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/DigitalInputs" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "DigitalInputs" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*


3) Ajouter l'Include du .h dans le "main.cpp" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans "USER CODE * Includes" du .c
 de la librairie "BaseDeTemps") :
```
	#include "DigitalInputsInterface.h"
```

4) Ajouter le Handler @ 10ms dans une sous-partie de la Boucle Principale (RT) à 10ms :
 (par exemple dans la zone @ 10ms "USER CODE * RT_10ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
```
	Handle_DigitalInputs_RT_10ms();		// A appeler depuis une Base de Temps RT @ 10ms
```

5) Ajouter le Handler @ 100ms dans une sous-partie de la Boucle Principale (RT) à 100ms :
 (par exemple dans la zone @ 100ms "USER CODE * RT_100ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
```	
	Handle_DigitalInputs_RT_100ms();	// A appeler depuis une Base de Temps RT @ 100ms
``` 

6) Ajouter l'appel de Gestion dans la Boucle Principale du "main.cpp" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	GestionDigitalInputs();				// A appeler dans la Boucle Principale (main.c)
```

** Dans le fichier "DigitalInputsInterface.h" **

7) Configurer la plage des Anti-Rebonds :
	a) les paramètres "DI_MAX_CT_ON" et "DI_MIN_CT_ON" définissent la dynamique de détection de la position "working"
	b) les paramètres "DI_SEUIL_ON" et "DI_SEUIL_OFF" définissent les seuils d'Hystérésis de l'anti-rebond

Remarque : il est recommandé de laisser la Configuration des "délais" avec les valeurs par défaut,
 c'est à dire "10" pour 1s, "30" pour 3s, "100" pour 10s, aini que "15" et "2" pour les délai d' "autoFire"

8) Dans la partie "Configuration des Evènements :"
	Pour l'utilisation des évènements, laisser la ligne suivante commentée ://#define DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS
	

## Etape III : Création et initialisation de DigitalInput

1) Pour chaque "DigitalInput" (entrée digitale) configuré dans CubeMx, 
	* créer une instance dans la partie du MainStateMachine ou du main.c
	* initialiser les arguments du constructeur:
		* le port utilisé 
		* le pin utilisé
		* "état" attendu du signal en position "travail" (NO ou NF)
		* le type d'entrée ( single, grouped)
	
Exemple 1 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Ouvert" ('1' = position active) :
```
	Di_Anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NO_WORKING_STATE_IS_1,E_SINGLE_INPUT);
```

Exemple 2 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Fermé" ('0' = position active) :
```
	Di_Anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NF_WORKING_STATE_IS_0,E_SINGLE_INPUT);
```

	* ne pas oublier de déclarer l'entité dans le .h
	
2) Pour une entrée groupée de type "Rotocom" à 4 positions, configurer de la façon suivante:
Exemple :
```
	#define ROTOCOM1_SIZE 4
	std::vector<DigitalInputs*> Di_Rotocom1;
	for(int i = 0; i < ROTOCOM1_SIZE; i++)
	{
		Di_Rotocom1[i] = new DigitalInputs(Rotocom1_GPIO_Port, Rotocom1_Pin,DI_NO_WORKING_STATE_IS_1,E_GROUPED_INPUT,i);
	}
```

Remarque : le support des Evènements "DigitalInputs" ne doit pas avoir été désactivé (DISABLE_DIGITAL_INPUTS_EVENTS_HANDLERS)



## Etape IV : Initialisation des fonctions Handle de DigitalInput

1) pour chaque entrée de type single, configurer la fonction "RegisterDigitalInput2EventFnHandler"
	à la suite de la déclaration de l'instance :
	* en paramètre on a :
		* le /les évènement(s) que l'on souhaite capturer 
		* l'instance de l'entrée 
		* la fonction de callback

Exemple 1 : entrée Anode avec l'évènement "DI_EVENT_NEW_STATE"
```
  RegisterDigitalInput2EventFnHandler(DI_EVENT_NEW_STATE,Di_Anode, HandleDI_Event);
```

Exemple 2 : entrée Anode avec plusieurs évènements
```
  RegisterDigitalInput2EventFnHandler(DI_EVENT_NEW_STATE | DI_EVENT_WORK_STATE_1S | DI_EVENT_IDLE_STATE_1S |
		  DI_EVENT_WORK_STATE_3S | DI_EVENT_IDLE_STATE_3S | DI_EVENT_WORK_STATE_10S | DI_EVENT_IDLE_STATE_10S,Di_Anode, HandleDI_Event);
```

2) pour chaque entrée de type grouped, configurer la fonction "RegisterDigitalInputGroupedEventFnHandler"
	à la suite de la déclaration de l'instance :
	* en paramètre on a :
		* le /les évènement(s) que l'on souhaite capturer 
		* le vecteur d'instances d'entrées 
		* la fonction de callback
		* la taille du vecteur (nombre d'entrées)
		
Exemple 1 : groupe d'entrée Rotocom (4 positions) avec l'évènement "DI_EVENT_NEW_STATE"
```
  RegisterDigitalInputGroupedEventFnHandler(DI_EVENT_NEW_STATE, Di_Rotocom1, HandleDI_Event,ROTOCOM1_SIZE);
```

Exemple 2 : groupe d'entrée Rotocom (4 positions) avec plusieurs évènement.
```
  RegisterDigitalInputGroupedEventFnHandler(DI_EVENT_NEW_STATE | DI_EVENT_WORK_STATE_1S | DI_EVENT_IDLE_STATE_1S |
		  DI_EVENT_WORK_STATE_3S | DI_EVENT_IDLE_STATE_3S | DI_EVENT_WORK_STATE_10S | DI_EVENT_IDLE_STATE_10S,Di_Rotocom1, HandleDI_Event,ROTOCOM1_SIZE);
```

=> Félicitations, c'est prêt :-) !