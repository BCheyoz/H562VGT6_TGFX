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
	* voir Boris Chay pour + d'infos

## Etape II : Configuration de la librairie
** Dans le fichier "DigitalInputsInterface.h" **

2) Activer (ou Désactiver) le support des entrées Groupées "DigitalGroupedInputs" :
	* utiliser le define "DI_ENABLE_DIGITAL_GROUPED_INPUTS" pour autoriser (ou non) les entrées Groupées

3) Configurer la plage des Anti-Rebonds :
	a) les paramètres "DI_MAX_CT_ON" et "DI_MIN_CT_ON" définissent la dynamique de détection de la position "working"
	b) les paramètres "DI_SEUIL_ON" et "DI_SEUIL_OFF" définissent les seuils d'Hystérésis de l'anti-rebond

Remarque : il est recommandé de laisser la Configuration des "délais" avec les valeurs par défaut,
 c'est à dire "10" pour 1s, "30" pour 3s, "100" pour 10s, aini que "15" et "2" pour les délai d' "autoFire"

## Etape III : Création et initialisation de DigitalInput

1) Pour chaque "DigitalInput" (entrée digitale) configuré dans CubeMx, 
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

	* ne pas oublier de déclarer l'entité dans le .h

4) Pour les entrées où c'est utile, enregistrer, dans la partie "USER CODE * Init_Vars",
 les éventuels Handlers custom pour les évènements souhaités, parmi les "DI_EVENT_*" disponibles.
// TODO gestion evenements

5) Ajouter également, dans cette même partie "USER CODE * Init_Vars", l'Initialisation
 de chaque variable de Groupe, de type "DigitalGroupedInput" (cf. III.2).
Remarque : le support pour les variables de Groupe doit être activé à l'aide du mot-clef "DI_ENABLE_DIGITAL_GROUPED_INPUTS".
// TODO a voir gestion des entrées groupées

6) Pour les Groupes où c'est utile, enregistrer, dans la partie "USER CODE * Init_Vars",
 les éventuels Handlers custom pour les évènements souhaités, parmi les "DI_EVENT_*" disponibles.
Remarque 1 : le support pour les variables de Groupe doit être activé à l'aide du mot-clef "DI_ENABLE_DIGITAL_GROUPED_INPUTS" ci-dessous.
// TODO a voir gestion des entrées groupées

8) Dans la partie "USER CODE * DispatchDI", appeler la fonction "DispatchDigitalInputEvents" pour chaque
 "DigitalInput" à gérer.
Remarque : la fonction "DispatchDigitalInputArrayEvents", qui prend en argument supplémentaire le nb d'éléments à
 surveiller, permet, par exemple, de dispatcher les évènements de tous les éléments d'un même tableau.
// TODO gestion evenements

9) Dans la partie "USER CODE * DispatchDG", appeler la fonction "DispatchDigitalGroupEvents" pour chaque
 Groupe à gérer, le cas échéant.
// TODO a voir gestion des entrées groupées
// TODO gestion evenements

10) Dans la partie "USER CODE * RegDI_Event" de "RegisterDigitalInput2EventFnHandler", ajouter, pour chaque
 "DigitalInput" à gérer, un appel à la macro "REGISTER_DIGITAL_INPUT_EVENT_FN_HANDLER_IF_FLAG_PRESENT".
Remarque : utiliser la macro "REGISTER_DIGITAL_INPUT_ARRAY_EVENT_FN_HANDLER_IF_FLAG_PRESENT" à la place de l'autre
 lorsqu'il s'agit d'un tableau d'entrées associées vers un même Groupe.
// TODO a voir gestion des entrées groupées
// TODO gestion evenements

III.12) Dans la partie "USER CODE * HandleDI_State" de "Handle_DigitalInputs_RT_100ms", ajouter, pour chaque
 "DigitalInput" à gérer, un appel à la fonction "ManageDigitalInputStateMaintained".
// TODO gestion evenements


## Etape IV : Configurer le Projet :

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
// TODO gestion evenements 

6) Ajouter l'appel de Gestion dans la Boucle Principale du "main.cpp" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	GestionDigitalInputs();				// A appeler dans la Boucle Principale (main.c)
```

=> Félicitations, c'est prêt :-) !