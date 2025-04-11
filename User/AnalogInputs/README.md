# Analog Inputs Lib v1.0

legacy from AnalogInputsConf.h j.proux for ALDES © 2025!

## History Usage 
* 08/09/2021 : Added by Jp	to RMD_Firmware (STM32G0B1CETx : productprojects/ventilation/tertiaire/rmd/rmd_firmware)
* 23/12/2021 : Added by AM	to HII_Manta_App (STM32G070CBTx : productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app)
* 07/01/2022 : Added by Jp	to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 30/11/2023 : Added by AB	to MV_By_Aldes (STM32G030K8Tx : innoprojects/mv_by_aldes)
* 18/02/2025 : Added by Mf	to TFL4_CarteMere_App (STM32H562VGT6 : be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)

# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**
  
1) Utiliser la "Pinout view" pour Configurer toutes les entrées Analogiques concernées comme ADC#_IN*
	et leur attribuer un Nom commençant par "Ai_" en fonction de leur usage prévu sur le schéma.

2) Dans la Catégorie "System Core" -> "GPIO", Configurer chaque Pin analogique concernée de l'onglet "ADC" telle que :
	* GPIO mode = "Analog mode"
	* GPIO Pull-up/Pull-down = "No pull-up and no pull-down"

3) Pour chaque Périphérique ADC# concerné de la Catégorie "Analog", Vérifier & Configurer le panneau "Mode" :
	* Vérifier (et corriger le cas échéant) que soient bien cochées (ou configurées) toutes les entrées souhaitées.
	* Certains Périphériques ADC# proposent des "Channels" complémentaires, de type "Temperature Sensor" / "Vrefint" / "Vbat" ou "Vddcore",
	 -> Activer ceux qui sont nécessaires (il faudra adapter la suite de la Configuration en conséquence).
	* Certains "Channels" peuvent parfois fonctionner en plusieurs Modes, comme "Single-ended" ou "Differential",
	 -> Dans ce cas, Configurer celui qui correspond au besoin.
	* Certains Périphériques ADC# proposent un "EXTI Conversion Trigger",
	 -> A priori, laisser "Disable" pour conserver le séquencement logiciel natif de la Librairie "AnalogInputs".

4) Pour chaque Périphérique ADC# concerné de la Catégorie "Analog", Configurer l'onglet "Parameter Settings" :
	a) Groupe de Paramètres "ADCs_Common_Settings" (si proposé) :
		* Mode = "Independent mode"

	b) Groupe de Paramètres "ADC_Settings" :
		* Clock Prescaler = (par exemple) "PCLK2 divided by 4" (ou 8) OU "Synchronous clock mode divided by 2" (ou 4)
		* Resolution = 12bits (ou mieux si possible ?)
		* Scan Conversion Mode (si proposé) : généralement "Forward" est suffisant (à défaut prendre "Enabled"),
		 -> mais "Backward" peut être utile si, par exemple, on veut mettre "Vbat" ou "Vrefint" avant les autres Channels.
		* Data Alignment = "Right alignment"
		* Sequencer (si proposé) : généralement "Not fully configurable" est suffisant,
		 -> mais "Fully configurable" fonctionne également, il y a juste un peu plus de Configuration manuelle à faire.
		* Continuous Conversion Mode = "Enabled", pour remplir plusieurs lots d'échantillons d'1 coup
		* Discontinuous Conversion Mode = "Disabled"
		* DMA Continuous Requests = "Disabled", pour que le DMA s'arrête une fois le Buffer rempli
		* End Of Conversion Selection = "End of sequence of conversion" OU "EOC flag at the end of all conversions"
		* Overrun behaviour (si proposé) : "Overrun data overwritten" marche habituellement bien
		* Low Power Auto Wait (si proposé) : "Disabled" marche habituellement bien
		* Auto Off (si proposé) : "Disabled" marche habituellement bien

	c) Groupe de Paramètres "ADC_Regular_ConversionMode" :
		* SamplingTime Common 1 (si demandé) = (par exemple) "160.5 Cycles", voire "480 Cycles"
		* Enable Regular Conversions (si demandé) = "Enable"
		* Enable Regular Oversampling (si demandé) = "Disable"
		* Number Of Conversion (si demandé) : indiquer le nb total de "Channels" à Convertir via ce Périphérique ADC
		* External Trigger Conversion Source = "Regular Conversion launched by software"
		* External Trigger Conversion Edge = "None"
		* Trigger Frequency (si proposé) : "High frequency" marche habituellement bien
		* Sampling Mode = "Normal"

	d) Sous-groupes de Paramètres "ADC_Regular_ConversionMode" -> "Rank" (si proposé) :
		Remarque : il est parfois possible de configurer l'ordre des Conversions, par exemple si "Sequencer" est "Fully configurable".
		Dans ce cas, pour chacun des RANK, renseigner :
			* Channel : Nom ou Numéro du Channel correspondant à l'entrée ADC à convertir, selon l'ordre souhaité
			* Sampling Time : Durée de cet échantilonnage, par exemple "112 Cycles" OU "480 Cycles" OU "Sampling time commun 1"
			* Offset Number (si proposé) = "No offset"
			* Monitored by (si proposé) = "None"

	e) Groupe de Paramètres "ADC_Injected_ConversionMode" (si proposé) :
		* Enable Injected Conversions = "Disable"

	f) Groupes de Paramètres "Analog Watchdog *" (si proposé) :
		* Enable Analog WatchDog* Mode : n'activer AUCUN "Analog Watchdog Mode"

5) Pour chaque Périphérique ADC# concerné de la Catégorie "Analog", Configurer l'onglet "DMA Settings" ...
	Remarque : Si le µC est équipé d'un (ou plusieurs) "GPDMA", CubeMX proposera certainement / obligera pê à l'utiliser.
	 -> Dans ce cas, se reporter à la section 5b (cf. plus bas),
	 -> Sinon, procéder comme indiqué dans la section 5a ci-dessous.

	a) Si le µC n'est PAS équipé de "GPDMA" (ou qu'il n'y en a plus de disponible), configurer un DMA "standard" ...
		Dans "Configuration -> DMA Settings", ajouter puis configurer une 'DMA Request' telle que :
		* Mode = "Normal" ("Circular" pourrait convenir, mais c'est inutile)
		* Increment Address -> Peripheral = ne PAS cocher
		* Increment Address -> Memory = Cocher
		* Use Fifo (si proposé) = ne PAS cocher
		* Data Width -> Peripheral = "Half Word"
		* Data Width -> Memory = "Half Word"
		* Enable synchronisation (si proposé) : à priori pas besoin de cocher
		* Enable event (si proposé) : à priori pas besoin de cocher

	b) Si le µC est équipé d'un (ou plusieurs) "GPDMA" : Sélectioner l'un des 'GPDMA' disponibles,
		Puis configurer dans le panneau "Mode" l'un des "Channel" disponibles (par exemple Channel 0) :
		* s'assurer au préalable qu'il soit bien de type "2 Words Internal FIFO",
		* puis le configurer en "Standard Request Mode"

		Dans l'onglet correspondant au channel sélectionné (par exemple "CH0") du panneau "Configuration",
		 -> Renseigner le Groupe de Paramètres "Circular configuration" tel que :
		 	* Circular Mode = "Disable"

		 -> Renseigner le Groupe de Paramètres "Request Configuration" tel que :
			* Request = "ADC#" (en remplaçant '#' par le numéro d'ADC souhaité)
			* DMA Handle in IP Structure = "DMA Handle"
			* Block HW request protocol = "Single/Burst Level"

		 -> Renseigner le Groupe de Paramètres "Channel Configuration" tel que :
			* Priority = "Low"
			* Transaction Mode = "Normal"
			* Direction = "Peripheral To Memory"

		 -> Renseigner le Groupe de Paramètres "Source Data Setting" tel que :
			* Source Address Increment After Transfer = "Disabled"
			* Data Width = "Half Word"
			* Burst Length = "1"
			* Allocated Port for Transfer = "Port0" ou "Port1"

		 -> Renseigner le Groupe de Paramètres "Destination Data Setting" tel que :
			* Destination Address Increment After Transfer = "Enabled"
			* Data Width = "Half Word"
			* Burst Length = "1"
			* Allocated Port for Transfer = "Port0" ou "Port1"

		 -> Renseigner le Groupe de Paramètres "Data Handling" tel que :
			* Data Handling Configuration = "Disable"

		 -> Renseigner le Groupe de Paramètres "Trigger" tel que :
			* Trigger Configuration = "Disable"

		 -> Renseigner "Transfer Event Configuration" tel que :
			* Transfer Event Generation = "The TC (and HT) event is generated at the (respectively half) end of each block"

		Dans l'onglet "SECURITY" de ce même panneau "Configuration" :
		 -> Vérifier que tous les Channels concernés pour le(s) ADCs souhaités soient indiqués tels que :
			* "Enable Channel as Priviledged" = "NON PRIVILEDGED"

		Dans l'onglet "All Channels" de ce même panneau "Configuration" :
		 -> Vérifier que tous les Channels concernés pour le(s) ADCs souhaités soient indiqués tels que :
			* "Request" = "GPDMA*_REQUEST_ADC#"

6) Pour chaque Périphérique ADC# concerné de la Catégorie "Analog", Configurer l'onglet "NVIC Settings" tel que :
	* si GPDMA sélectionné, "GPDMAx Channel * global interrupt" soit bien coché "Enabled"
    	+ si proposé : laisser "Preemption Priority" à 0,
 		+ si proposé : laisser "Sub Priority" à 0.
    * si DMA classique sélectionné, "DMAx ch* interrupt" (ou "DMAx stream* global interrupt") soit bien coché "Enabled"
    	+ si proposé : laisser "Preemption Priority" à 0,
 		+ si proposé : laisser "Sub Priority" à 0.
    * "ADC* interrupt" : à priori, ça peut rester 'not Enabled'.

7) Dans la Catégorie "System Core" -> "NVIC" -> Panneau "Configuration" -> onglet "NVIC",
	* s'assurer que, pour chaque ADC associé à cette Librairie "AnalogInputs", soient bien configurés :

	 -> "GPDMAx Channel * global interrupt" (si GPDMA sélectionné) : coché "Enabled"
	  + si proposé : laisser "Preemption Priority" à 0,
	  + si proposé : laisser "Sub Priority" à 0.

	 -> "DMAx channel * interrupt" (si DMA classique sélectionné) : coché "Enabled"
	  + si proposé : laisser "Preemption Priority" à 0,
	  + si proposé : laisser "Sub Priority" à 0.
  
8) Dans la Catégorie "System Core" -> "NVIC" -> Panneau "Configuration" -> onglet "Code generation",
	* s'assurer que, pour chaque ADC associé à cette Librairie "AnalogInputs", soient bien configurés :

	 -> "GPDMAx Channel * global interrupt" (si GPDMA sélectionné) :
		* "Generate IRQ handler" : coché
		* "Call HAL handler" : coché
		* "Generate Enable in Init" (si la colonne est proposée) : coché

	 -> "DMAx channel * interrupt" (si DMA classique sélectionné) :
		* "Generate IRQ handler" : coché
		* "Call HAL handler" : coché
		* "Generate Enable in Init" (si la colonne est proposée) : coché

9) Dans l'onglet général "Clock Configuration" :
	* s'assurer que la Clock "to ADCx" (probablement PCLK2) soit configurée à une fréquence suffisante,
	 -> sans pour autant être en rouge. 
	Dans la plupart des cas, 30 MHz peut être suffisant.

10) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
    * "Generate peripheral initialization as a pair of '.c/.h' files per peripheral" -> coché
    * "Keep User Code when re-generating" -> coché
    * "Delete previously generated files when not re-generated" -> coché.

11) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> bloc "Driver Selector" :
    * s'assurer que chaque Périphérique ADC associé à cette Librairie "AnalogInputs" soit bien de type "HAL".

12) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> bloc "Generated Function Calls" :
	-> s'assurer que, pour chaque Périphérique ADC associé à cette Librairie "AnalogInputs",
		* "Generate Code" (à gauche) -> coché
		* "Do Not Generate Function Call" -> coché
		* "Visibility (Static)" -> coché

Remarque : Ne pas oublier de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX !

Note_Jp : la Clock des DMA & GPDMA semble provenir directement de HCLK.


## Etape II : Configurer "AnalogInputs"

** Dans le fichier "AnalogInputsConf.h" : **

1) Indiquer dans "AI_NB_MAX_OF_INIT_PARAMS" le nombre total de Périphériques ADC que devra gérer cette Librairie

2) Décommenter ou Commenter "AI_DISCARD_MIN_MAX_VALUE" pour ignorer les valeurs Min & Max des échantillons ADC_DMA

3) Décommenter ou Commenter "AI_REQUEST_CALIB_AT_MST" pour demander (ou pas) une Calibration de l'ADC à l'Init
	* Si possible, cette fonctionalité est recommandée, mais elle n'est pas présente sur tous les µC !

Remarque : il est recommandé de laisser "AI_FIRST_INIT_PARAM" & "AI_AFTER_INIT_PARAM" à leurs Paramètres par défaut,
 respectivement "AI_BaseInitParam" et "AI_EndInitParams".


## Etape III : Configurer la partie User

** Dans le fichier User.c ("AnalogInputsUser.c") : **

1) Pour chaque Périphérique ADC qui sera associé à cette Librairie "AnalogInputs", définir :
	* ADC#_HANDLE	: Indiquer le hadc* correspondant, d'après "adc.h", par exemple "&hadc1". Ne pas oublier le '&' devant.
	* ADC#_INIT_FN 	: Indiquer le nom de la fonction d'Init CubeMx, d'après "adc.h", par exemple "MX_ADC1_Init",
	* ADC#_DMA_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée par le DMA pour le stockage des échantillons de l'ADC, par exemple "AdcDmaBuf1"
	* ADC#_RAW_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée pour le stockage de chaque dernière valeur RAW, par exemple "AdcRawBuf1"
	* ADC#_ACCU_RAW_BUF_NAME : Indiquer un Nom de Variable, qui sera utilisée pour la gestion Interne des échantillons RAW, par exemple "AdcAccuRawBuf1"
	* ADC#_MOY_FN_HANDLERS : Indiquer le Nom du Tableau, qui contiendra, pour chaque entrée ADC, la fonction à appeler pour finaliser la Conversion
	* ADC#_NB_OF_CHANNELS : Indiquer le nombre de Channels configurés (= nb de "HAL_ADC_ConfigChannel") pour cet ADC dans "adc.c"
	* ADC#_SAMPLES_PER_CH : Indiquer le nombre d'échantillons successifs à prendre en rafale pour obtenir 1 valeur ADC
	* ADC#_MOY_NB_VALUES : Indiquer le nombre de valeurs ADC à intégrer pour le calcul de la Moyenne finale

2) Pour chaque Périphérique ADC qui sera associé à cette Librairie "AnalogInputs", définir les Variables nécessaires à la Librairie,
 via les macros "AI_MAKE_DMA_RESULTS_BUF" & "AI_MAKE_ADC_ACCU_RAW_BUF" proposées (et éventuellement "AI_MAKE_ADC_RAW_BUFFER")

	Exemple 1, d'après les Paramètres "ADC1*" configurés ci-dessus :
	``` C
	AI_MAKE_DMA_RESULTS_BUF(ADC1_DMA_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH);
	AI_MAKE_ADC_RAW_BUFFER (ADC1_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS);
	AI_MAKE_ADC_ACCU_RAW_BUF(ADC1_ACCU_RAW_BUF_NAME, ADC1_NB_OF_CHANNELS, ADC1_MOY_NB_VALUES);
	```

	Exemple 2, d'après les Paramètres "ADC2*" configurés ci-dessus :
	``` C
	AI_MAKE_DMA_RESULTS_BUF(ADC2_DMA_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_SAMPLES_PER_CH);
	AI_MAKE_ADC_RAW_BUFFER (ADC2_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS);
	AI_MAKE_ADC_ACCU_RAW_BUF(ADC2_ACCU_RAW_BUF_NAME, ADC2_NB_OF_CHANNELS, ADC2_MOY_NB_VALUES);
	```

3) Définir chaque Variable finale nécessaire pour le Stockage du Résultat en RAM

	Exemple 1, une Variable "tAi0_0_10V" mémorise la mesure de l'entrée 0-10V sur Ai0 :
	``` C
	tAI_FloatValue tAi0_0_10V = {0};
	```
	
	Exemple 2, une Variable "tAi1_12vAlim" mémorise la mesure d'une alimentation 12v sur Ai1 :
	``` C
	tAI_FloatValue tAi1_12vAlim = {0};
	```

4) Si besoin, Ajouter / Corriger ou Implémenter la(les) Fonctions User de Conversion Finales appropriées,
 en respectant le prototype "pAiFnHandleNewFloatValue" (cf. "AnalogInputsUtils.h" & exemples fournis)

5) Pour chaque Périphérique ADC qui sera associé à cette Librairie "AnalogInputs", Remplir / Configurer le Tableau des Fonctions de
 Conversion & Variables Finales associées, en respectant la structuration "tAiFnNewFloatValueHandler"
  (1 ligne par Channel et 1 Tableau par Périphérique ADC)

	Exemple :
	``` C
		{ AnalogInput_HandleNewFloat_0_10V,	&tAi0_0_10V },
	```
	
	6) Pour chaque Périphérique ADC qui sera utilisé, Ajouter / Configurer la ligne correspondante dans le Tableau
	 "mAdcInitParam", en respectant la structuration "tAdcInitParams" (1 ligne d'init par Périphérique ADC)
	
	Exemple :
	``` C
		{ ADC1_HANDLE, ADC1_INIT_FN, ADC1_DMA_BUF_NAME, &ADC1_ACCU_RAW_BUF_NAME, ADC1_MOY_FN_HANDLERS, AnalogInput_HandleEndOfConv, ADC1_RAW_BUF_NAME, \
			ADC1_NB_OF_CHANNELS, ADC1_SAMPLES_PER_CH, ADC1_MOY_NB_VALUES, ADC1_CONV_DELAY, ADC1_ERROR_DELAY, ADC1_REF_INT_CHANNEL, ADC1_REF_INT_PT_CONV },
	```

7) Ajouter d'éventuelles fonctions "Get" pour récupérer la/les valeur(s) utile(s) à partir des variables crées au III.3)

	Exemple 1 : Récupérer sur 8bits la valeur de "tAi0_0_10V"
	``` C
	uint8_t getA0_0_10V(void)
	{
		return (uint8_t)(tAi0_0_10V.value);
	}
	```
	
	Exemple 2 : Récupérer sur 8bits la valeur de "tAi0_0_10V" * 10 
	``` C
	uint8_t getA1_0_10V_x10(void)
	{
		return (uint8_t)(tAi1_0_10V.value * 10.0f);
	}
	```
	
	Exemple 3 : Récupérer sur 16bits la valeur de "tAi2_0_10V" * 10
	``` C
	uint16_t getA2_0_10V_x10(void)
	{
		return (uint16_t)(tAi2_0_10V.value * 10.0f);
	}
	```

8) Dans le fichier User.h ("AnalogInputsUser.h"),
	1) Ajouter éventuellement comme "extern" les variables crées au  III.3)

		Exemple 1 : Autoriser l'accès externe à la Variable "tAi0_0_10V"
		``` C
		extern tAI_FloatValue tAi0_0_10V;
		```
		
		Exemple 2 : Autoriser l'accès externe à la Variable "tAi1_0_10V"
		``` C
		extern tAI_FloatValue tAi1_0_10V;
		```
		
	2) Ajouter éventuellement "publiquement" le(s) prototype(s) de fonction(s) de "GET" crée(s) au III.7)
	
		Exemple 1 : Donner accès sur 8bits à la valeur de "A0_0_10V"
		``` C
		uint8_t getA0_0_10V(void);
		```
		
		Exemple 2 : Donner accès sur 8bits à la valeur de "A1_0_10V" * 10
		``` C
		uint8_t getA1_0_10V_x10(void);
		```
		
		Exemple 3 : Donner accès sur 16bits à la valeur de "A2_0_10V" * 10
		``` C
		uint16_t getA2_0_10V_x10(void);
		```

## Etape IV : Configurer le Projet
### Dans le nouveau Projet :

1) Ajouter l'Include du .h dans le "main.c" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans "USER CODE * Includes" de "BaseDeTemps.c")
	``` C
	#include "AnalogInputsCore.h"		// Pour Gestion des Entrées ADC
	```

2) Ajouter l'appel d'Init dans le "main" du "main.c" (par exemple parmi les "USER CODE * 2") :
	``` C
	InitAnalogInputs();					// A appeler dans la partie Init Software (main.c)
	```

3) Ajouter le Handler @ 10ms dans une sous-partie de la Boucle Principale (RT) à 10ms :
 (par exemple dans la zone @ 10ms "USER CODE * RT_10ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
	``` C
	Handle_AnalogInputs_RT_10ms();		// A appeler depuis une Base de Temps RT @ 10ms
	```

4) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c" :
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
	``` C
	Gestion_AnalogInputs();				// A appeler dans la Boucle Principale (main.c)
	```

5) Ajouter le Dossier "AnalogInputs" à l' "IncludePath" pour toutes les Configs de Build
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/AnalogInputs" (sans les guillemets)
	* Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

6) Vérifier que le Dossier "AnalogInputs" ne soit "Exclude From Build" d'aucune des Configs.
 (y compris "Debug" & "Release")
	* Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

7) Ajouter au projet l'une des Librairie "Utils" compatibles, comme celle de "RMD_Firmware" ou "HII_Manta_App".
 	* se reporter à "utils.h" pour intégrer plus facilement votre Librairie "Utils".


## Etape V : Configurer le "build-job" du CI
### Dans le MakeFile :

1) Ajouter aux "C_SOURCES" le chemin relatif vers les fichiers "AnalogInputsCore.c" & "AnalogInputsUser.c",
 très probablement : `User/AnalogInputs/AnalogInputsCore.c \` (sans les guillemets mais avec '\' en fin de ligne)
 ET : `User/AnalogInputs/AnalogInputsUser.c \` (toujours sans les guillemets mais aussi avec '\' en fin de ligne)

	* Remarque : pour le chemin, il faut bien utiliser la bare oblique de la division '/' à la place du '\' habituel de Windows.

2) Ajouter aux "C_INCLUDES" le chemin relatif vers notre dossier "AnalogInputs" avec le préfixe "-I" devant,
 très probablement : `-IUser/AnalogInputs \` (sans les guillemets mais également avec '\' en fin de ligne)

	* Même Remarque : pour le chemin, utiliser la bare obliques de la division '/' à la place du '\' habituel de Windows.


Félicitations, c'est prêt 😉 !

