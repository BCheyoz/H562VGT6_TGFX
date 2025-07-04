# DigitalInputs_cpp v1.1

## Historique de déploiement  
* 06/02/2025 : Added by MF to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

## Information complémentaire
Lors des essais sur la carte Tflow4 avec un échantillonnage à 1ms, le compteur peut avoir un offset allant jusqu'a +20.     
Ce décalage est du à l'interruption du systick générer par le µC qui peut être dephasé par rapport a l'horloge HW. (déclanche un état haut sur le front montant et/ou descendant)    
Avec un signal Carré à 50% de la fréquence, échantillonnage à 1ms sur 10ms on peut voir le compteur avec un décallage de +1 ou +2.   
Ce qui entraine un décalage montant à +20 sur une période de 100ms. 

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

1) Ajouter le Dossier "DigitalInputsCore_cpp" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/DigitalInputsCore_cpp" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "DigitalInputs" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*


3) Ajouter l'Include du .hpp dans le "main.cpp" (par exemple parmi les "USER CODE * Includes"),
```
	#include "DigitalInputs.hpp"
```

4) Ajouter l'interface dans la lib "BaseDeTemps.c" appropriée (par exemple dans "USER CODE * Includes" du .c
 de la librairie "BaseDeTemps") :
```
	#include "DigitalInputsInterface.hpp"
```

5) Ajouter le Handler @ 1ms dans la fonction HandleBaseDeTemps_IT_1ms :
 (par exemple dans la zone "USER CODE BEGIN IT_1ms" dans "BaseDeTemps.c")
```
	Handle_DigitalInputs_RT_1ms();		// A appeler depuis une Base de Temps RT @ 1ms
```

6) Ajouter le Handler @ 10ms dans une sous-partie de la Boucle Principale (RT) à 10ms :
 (par exemple dans la zone @ 10ms "USER CODE * RT_10ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
```
	Handle_DigitalInputs_RT_10ms();		// A appeler depuis une Base de Temps RT @ 10ms
```

7) Ajouter le Handler @ 100ms dans une sous-partie de la Boucle Principale (RT) à 100ms :
 (par exemple dans la zone @ 100ms "USER CODE * RT_100ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
```	
	Handle_DigitalInputs_RT_100ms();	// A appeler depuis une Base de Temps RT @ 100ms
``` 

8) Ajouter l'appel de Gestion dans la Boucle Principale du "main.cpp" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	GestionDigitalInputs();				// A appeler dans la Boucle Principale (main.c)
```

** Dans le fichier "DigitalInputsInterface.h" **

9) Configurer la plage des Anti-Rebonds :
	a) les paramètres "DI_DEFAULT_SEUIL_ON" et "DI_DEFAULT_SEUIL_OFF" définissent les seuils d'Hystérésis par defaut de l'anti-rebond

## Etape III : Utilisation 

1) Pour chaque "DigitalInput" (entrée digitale) configuré dans CubeMx, créer une instance dans la partie du MainStateMachine ou du main.cpp    
	* Les arguments du constructeur simplifié sont :   
		* GPIO_TypeDef *GPIOx, Le Port du GPIO   
		* uint16_t GPIO_Pin, Le Numero du GPIO    
		* GPIO_PinState WorkState, L'etat de fonctionnement : position Haut ou Bas   
		* E_DI_SAMPLE_FREQ freq, La fréquence d'échantillonnage (1ms ou 10ms)    
	* Les arguments du constructeur avancé sont :    
		* GPIO_TypeDef *GPIOx, Le Port du GPIO   
		* uint16_t GPIO_Pin, Le Numero du GPIO   
		* GPIO_PinState WorkState, L'etat de fonctionnement : position Haut ou Bas   
		* uint16_t thresoldOn, Le seuil pour considérer le Pin comme Actif.   
		* uint16_t thresoldOff, Le seuil pour considérer le Pin comme Inactif après une activation.    
		* E_DI_SAMPLE_FREQ freq, La fréquence d'échantillonnage (1ms ou 10ms)   
	
	N'oublier pas de prendre en compte la fréquence d'échantillonnage lors de la définition des seuils (par defaut ou non)    
	La librairire consulte le compteur tous les 100ms et le remet à zéro apres l'opération. 	
	
Exemple 1 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Ouvert" ('1' = position active) :
```
	di_Anode = new DigitalInputs(DI_Anode_GPIO_Port, DI_Anode_Pin, DI_NO_WORKING_STATE_IS_1); // utilisation des valeurs par defaut 
```

Exemple 2 : Une instance "Di_Anode" est initialisée en fonctionnement "Normalement Fermé" ('0' = position active) :
```
	di_Anode = new DigitalInputs(DI_Anode_GPIO_Port, DI_Anode_Pin, DI_NF_WORKING_STATE_IS_0, 75, 50, E_DI_SAMPLE_1ms);
```
	
2) Récupérer l'etat du Digital Input en appelant la fonction `uint8_t getcurState()`



## Etape IV : Déclinaison  
La librairie Digital Input est une classe/objet possédant des fonctions virtuelles pour la décliner sur des usages spécifiques.    


1) Commencer par créer une class s'appelant "DigitalInputsXXX" dans un dossier nommé "DigitalInputsXXX_cpp"   
	Redéfinisser les fonctions virtuels seulement s'il doit avoir un comportement différent 

**Exemple du fichier .hpp** 
```
#pragma once

#include "DigitalInputs.hpp"

class DigitalInputsXXX : public DigitalInputs {
public:
	DigitalInputsXXX(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, E_DI_SAMPLE_FREQ freq = E_DI_SAMPLE_10ms);
	DigitalInputsXXX(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, uint16_t thresoldOn, uint16_t thresoldOff, E_DI_SAMPLE_FREQ freq = E_DI_SAMPLE_10ms);

	virtual uint8_t getcurState(void) final;
	
protected :
	virtual void newStateHandler() final;
	virtual void newWorkHandler() final;
	virtual void newIdleHandler() final;
	virtual void work1sHandler() final;
	virtual void idle1sHandler() final;
	virtual void work3sHandler() final;
	virtual void idle3sHandler() final;
	virtual void work10sHandler() final;
	virtual void idle10sHandler() final;
	virtual void autoFireHandler() final;
};
```


**Exemple du fichier .cpp**  
```
#include "DigitalInputsXXXXXX.hpp"

DigitalInputsXXX::DigitalInputsXXX(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,GPIO_PinState WorkState, E_DI_SAMPLE_FREQ freq)
: DigitalInputsXXX(GPIOx, GPIO_Pin, WorkState, freq) {}

DigitalInputsXXX::DigitalInputsXXX(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState WorkState, uint16_t thresoldOn, uint16_t thresoldOff, E_DI_SAMPLE_FREQ freq) 
: DigitalInputsXXX(GPIOx, GPIO_Pin, WorkState, thresoldOn, thresoldOff, freq) {}

uint8_t DigitalInputsXXX::getcurState(void) {return (uint8_t)_curState;};

void DigitalInputsXXX::newStateHandler(){
	// Action à définir 
}

void DigitalInputsXXX::newWorkHandler(){
	// Action à définir
}

void DigitalInputsXXX::newIdleHandler(){
	// Action à définir 
}

void DigitalInputsXXX::work1sHandler(){
	// Action à définir 
}

void DigitalInputsXXX::idle1sHandler(){
	// Action à définir
}

void DigitalInputsXXX::work3sHandler(){
	// Action à définir
}

void DigitalInputsXXX::idle3sHandler(){
	// Action à définir
}

void DigitalInputsXXX::work10sHandler(){
	// Action à définir
}

void DigitalInputsXXX::idle10sHandler(){
	// Action à définir
}

void DigitalInputsXXX::autoFireHandler(){
	// Action à définir
}

```

