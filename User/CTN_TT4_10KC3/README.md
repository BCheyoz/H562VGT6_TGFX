# CTN_TT4_10KC3 Lib v1.0

## History Usage 
* 18/02/2025 : Added by Mf	to Tfl4 (STM32H562VGTX : /be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)

# Procédure pour intégrer cette Librairie
## Etape I : Avoir configuré la librairie "AnalogInputs" via le "analogInputsConf.h"


## Etape II : Ajout de la librairie
**Dans le dossier User**
1) Ajouter le dossier "CTN_TT4_10KC3"

2) Ajouter le Dossier "CTN_TT4_10KC3" à l' "IncludePath" pour toutes les Configs de Build
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
	* il est souvent préférable d'utiliser la formulation "../User/CTN_TT4_10KC3" (sans les guillemets)
	* Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

3) Vérifier que le Dossier "CTN_TT4_10KC3" ne soit "Exclude From Build" d'aucune des Configs.
 (y compris "Debug" & "Release")
	* Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".


## Etape III : connexion de la librairie avec AnalogInputs

**Dans le fichier AnalogInputsUser.h :**
1) ajouter en entete de fichier l'include :
	```
	#include "ctn_tt4_10kc3.h"
	```
	
2) parametrer le nombre de CTN utiliser 
	```
	#define NB_CTN_TT4_10KC3_USE 5
	```


**Dans le fichier AnalogInputsUser.c :**  
1) Dans la zone "// Tableau des Fonctions de Conversion à appeler & Variables Finales, en fonction du Channel considéré"
	ajouter la variable et la fonction de Handle associée dans l'adc concerné:
	
	Exemple :
	```
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi_CTN[0] }
	```

2) configuer la fonction "AnalogInput_HandleNewFloat_CTN" pour quelle appel la fonction de convertion "convertADC_to_CTN_10K"   

	Exemple :
	```
	tAI_IntValue* pData = pVar;
	pData->nbPtADC = (uint16_t)(newValue);
	pData->value = convertADC_to_CTN_10K(pData->nbPtADC);
	```
	
	
	
Compiler et,
Félicitations, c'est prêt :-) !

