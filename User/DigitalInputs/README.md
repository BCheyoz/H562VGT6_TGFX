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

## Etape II : Configurer le Projet :

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

5) Ajouter l'appel de Gestion dans la Boucle Principale du "main.cpp" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	GestionDigitalInputs();				// A appeler dans la Boucle Principale (main.c)
```

## Etape III : Configuration de la librairie DigitalInput
** Dans le fichier "DigitalInputsInterface.h" **

2) Activer (ou Désactiver) le support des entrées Groupées "DigitalGroupedInputs" :
	* utiliser le define "DI_ENABLE_DIGITAL_GROUPED_INPUTS" pour autoriser (ou non) les entrées Groupées

3) Configurer la plage des Anti-Rebonds :
	a) les paramètres "DI_MAX_CT_ON" et "DI_MIN_CT_ON" définissent la dynamique de détection de la position "working"
	b) les paramètres "DI_SEUIL_ON" et "DI_SEUIL_OFF" définissent les seuils d'Hystérésis de l'anti-rebond

Remarque : il est recommandé de laisser la Configuration des "délais" avec les valeurs par défaut,
 c'est à dire "10" pour 1s, "30" pour 3s, "100" pour 10s, aini que "15" et "2" pour les délai d' "autoFire"

## Etape IV : Interfaçage de la librairie DigitalInput

1) Pour chaque "DigitalInput" (entrée digitale) configuré dans CubeMx,
	* dans le "main.h" ajouter les "#define" des pins et ports associés au nom de l'entrée
	
Exemple 1 : pour l'entrée "Anode" :
```
	/* Private defines -----*/
	#define Anode_Pin	GPIO_PIN_5
	#define Anode_GPIO_Port GPIOA
```
2) Pour chaque "DigitalInput" (entrée digitale) configuré dans CubeMx, 
	* créer une instance dans la partie du MainStateMachine ou du main.c
	* initialiser les arguments du constructeur:
		* le port utilisé 
		* le pin utilisé
		* "état" attendu du signal en position "travail" (NO ou NF)
	
Exemple 1 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Ouvert" ('1' = position active) :
```
	Di_Anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NO_WORKING_STATE_IS_1);
```

Exemple 2 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Fermé" ('0' = position active) :
```
	Di_Anode = new DigitalInputs(Anode_GPIO_Port, Anode_Pin,DI_NF_WORKING_STATE_IS_0);
```

	* ne pas oublier de déclarer l'entité dans le .h avec l'include associé
En entete du fichier:
```
	#include "DigitalInputs.hpp"
```
déclaration de l'instance :
```
		DigitalInputs *Di_Anode;
```

=> Félicitations, c'est prêt :-) !