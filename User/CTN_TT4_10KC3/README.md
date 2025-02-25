# CTN_TT4_10KC3 Lib v1.0

## History Usage 
* 18/02/2025 : Added by Mf	to Tfl4 (STM32H562VGTX : /be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)

# Procédure pour intégrer cette Librairie
## Etape I : Avoir configuré la librairie "AnalogInputs" via le "analogInputsConf.h"


## Etape II : Ajout de la librairie
** Dans le dossier User **
1) Ajouter le dossier "CTN_TT4_10KC3"

2) Ajouter le Dossier "CTN_TT4_10KC3" à l' "IncludePath" pour toutes les Configs de Build
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
	* il est souvent préférable d'utiliser la formulation "../User/CTN_TT4_10KC3" (sans les guillemets)
	* Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

3) Vérifier que le Dossier "CTN_TT4_10KC3" ne soit "Exclude From Build" d'aucune des Configs.
 (y compris "Debug" & "Release")
	* Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".


## Etape III : connexion de la librairie avec AnalogInputs
** Dans le fichier User.h ("AnalogInputsUser.h") : **
1) Déclarer la structure suivante (si inexistante):
	```
	typedef struct {
	uint16_t nbPtADC;
	int16_t TempValue;
	} tAI_IntValue;
	```
	(déclaration en dessous de tAI_FloatValue)

** Dans le fichier User.c ("AnalogInputsUser.c") : **
1) ajouter en entete de fichier l'include :
	```
	#include <ctn.hpp>
	```
	
2) Déclarer dans la zone "// Prototypes des Fonctions de Conversion ... Résulat Final" la fonction suivante :
	```
	void AnalogInput_HandleNewFloat_CTN(void* pVar, float newValue);
	```

3) Déclarer dans la zone "// Fonctions User de Finalisation de la Conversion " la fonction suivante :
	```
	void AnalogInput_HandleNewFloat_CTN(void* pVar, float newValue)
	{
		tAI_IntValue* pData = pVar;
		pData->nbPtADC = (uint16_t)(newValue);
		pData->TempValue = convertADC_to_CTN_10K(pData->nbPtADC);
	}
	```
	
4) Dans la zone "// Variables finales pour le Stockage des Résultats ADC " 
	déclarer les variables qui vont stocker les données ctn
	
	Exemple 1 :
	```
	tAI_IntValue tAi0_T0 = {0};
	```

5) Dans la zone "// Tableau des Fonctions de Conversion à appeler & Variables Finales, en fonction du Channel considéré"
	ajouter la variable et la fonction de Handle associée dans l'adc concerné:
	
	Exemple 1 :
	```
	{ AnalogInput_HandleNewFloat_CTN, 	&tAi0_T0 }
	```


6) Ajouter d'éventuelles fonctions "Get" pour récupérer la/les valeur(s) utile(s) à partir des variables crées précédemment )

	Exemple 1 : Récupérer sur 16bits la valeur de "tAi0_T0"
	```
	int16_t getAi0_T0(void)
	{
		return tAi0_T0.TempValue);
	}
	```


7) Dans le fichier User.h ("AnalogInputsUser.h"),
	1) Ajouter éventuellement comme "extern" les variables crées précédemment)

		Exemple 1 : Autoriser l'accès externe à la Variable "tAi0_T0"
		```
		extern tAI_IntValue tAi0_T0;
		```

		
	2) Ajouter éventuellement "publiquement" le(s) prototype(s) de fonction(s) de "GET" crée(s) aprécédemment)
	
		Exemple 1 : Donner accès sur 16bits à la valeur de "tAi0_T0"
		```
		int16_t getAi1_T1(void);
		```

Compiler et,
Félicitations, c'est prêt :-) !

