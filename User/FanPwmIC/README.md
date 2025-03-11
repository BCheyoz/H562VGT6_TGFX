# FanPwmIc Lib v1.0

Export Original from ConfFormatter by j.proux on 13/03/2024 @ 14:30:17.608 for ALDES © 2024 !

## History Usage 
* 15/02/2022 : Added by Jp	to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 30/11/2023 : Added by Ab	to MV_By_Aldes (STM32G030K8Tx : innoprojects/mv_by_aldes)
* 10/02/2025 : Added by Mf  to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1. Configurer la/les sortie(s) PWM :  
    1. Configurer toutes les sorties de commande PWM sur des pins TIM#_CH* et leur attribuer un Nom commençant par "PWM_" en fonction de leur usage prévu sur le schéma. Généralement, la Configuration associée à chacune de ces Pin PWM (dans "GPIO -> TIM") est :     
        * GPIO mode = "Alternate Function Push Pull"  
        * GPIO Pull-up/Pull-down = "Pull-down"
        * Maximum output speed : "Low" est généralement suffisant
        * Fast Mode (si proposé) : "Disable" est généralement suffisant

    Remarque : le choix d'une Pin plutôt qu'une autre est à bien considérer parce la fréquence de la source APBx du Timer associé peut rendre facile / possible ou difficile, voire impossible la configuration et l'opérabilité de la fonction attendue.

    2. Pour chacun des Périphériques TIM# concernés, configurer tout d'abord la partie "Mode" :   
        * Slave Mode = "Disable"
        * Trigger Source = "Disable"
        * Clock Source (si proposé) = "Internal Clock"
        * Internal Clock (si proposé) = Coché
        * "Channel*" concerné = "PWM Generation CH*
        * Combined Channels = "Disable"
        * Use ETR as Clearing Source (si proposé) = Décoché
        * XOR activation (si proposé) = Décoché
        * One Pulse Mode = Décoché

    3. Pour chacun des Périphériques TIM# concernés, configurer les "Parameter Settings" :
        1. Repérer à quel APBx est connecté le Timer en question   
            -> La méthode la plus rapide que j'ai trouvée est de rechercher "ENR_TIMxEN" dans tout le projet, en remplaçant 'x' par le numéro du Timer concerné.   
        Exemple 1 : Rechercher "ENR_TIM9EN" pour le Timer9 permet de trouver "RCC_APB2ENR_TIM9EN" -> APB2   
        Exemple 2 : Rechercher "ENR_TIM12EN" pour le Timer12 permet de trouver "RCC_APB1ENR_TIM12EN" -> APB1

        2. A l'aide de l'onlget "Clock Configuration", déterminer la fréquence nominale de l' "APB# timer clocks" concerné. La valeur en Hz est désignée dans les calculs ci-dessous "FAPB".
        3. Les "Counter Settings" sont communs à tous les Channels d'un même Timer ...
            * Pour faciliter la commande directe en mV, la "Période" recommandée est "10 000"
                * Le "Counter Period" sera configuré sur 10000 -1, soit "9999"

            * La fréquence "Cible" de PWM recommandée pour la commande d'un ventilateur est de l'orde de 12KHz.  
            (Pour info, à ce jour, 6KHz pourrait également être suffisant)
                * Le "Prescaler PSC" se calcule ainsi : PSC = (((FAPB / Période) / Cible) -1)   
            Exemple 1 : FAPB = 180MHz, Période = 10000, Cible = 6KHz => PSC = (((18*10e7 / 10e4) / 6*10e3) -1 = 2   
            Exemple 2 : FAPB =  60MHz, Période = 10000, Cible = 6KHz => PSC = ((( 6*10e7 / 10e4) / 6*10e3) -1 = 0  
            * Counter Mode = "Up"
            * Internal Clock Division CKD = "No Division"
            * auto-reload preload = "Disable"

        4. Configurer (si proposé) les "Trigger Output TRGO Parameters" :
            * Master/Slave Mode MSM =  "Disable" (Trigger input effect not delayed)
            * Trigger Event Selection TRGO = "Reset" (UG bit from TIMx_EGR)

        5. Configurer chaque "PWM Generation Channel *" concerné tel que :
            * Mode = "PWM mode 1"
            * Pulse = "0"
            * Output compare preload = "Enable"
            * Fast Mode = "Disable"
            * CH Polarity = "High"


2. Configurer la/les entrée(s) InputCapture des retours Tachy :

    1. Configurer toutes les entrées de retour Tachy sur des pins TIM#_CH* et leur attribuer un Nom commençant par "Tachy_" en fonction de leur usage prévu sur le schéma.   
    Généralement, la Configuration associée à chacune de ces Pin IC (dans "GPIO" -> "TIM") est :
        * GPIO mode = "Alternate Function Push Pull"
        * GPIO Pull-up/Pull-down = "No pull-up and no pull-down"
        * Maximum output speed : "Low" est généralement suffisant
        * Fast Mode (si proposé) : "Disable" est généralement suffisant

    2. Pour chacun des Périphériques TIM# concernés, configurer tout d'abord la partie "Mode" :
        * Slave Mode = "Disable"
        * Trigger Source = "Disable"
        * Clock Source (si proposé) = "Internal Clock"
        * Internal Clock (si proposé) = Coché
        * "Channel*" concerné = "Input Capture direct mode*
        * Combined Channels = "Disable"
        * Use ETR as Clearing Source (si proposé) = Décoché
        * XOR activation (si proposé) = Décoché
        * One Pulse Mode = Décoché

    3. Pour chacun des Périphériques TIM# concernés, configurer les "Parameter Settings" :
        1. Repérer à quel APBx est connecté le Timer en question (cf. I.1.3.1)

        2. A l'aide de l'onglet "Clock Configuration", déterminer la fréquence nominale de l' "APB# timer clocks" concerné. La valeur en Hz est désignée dans les calculs ci-dessous "FAPB".

        3. Les "Counter Settings" sont communs à tous les Channels d'un même Timer ...
            * Cette Librairie est basée sur un deltaMax de 1.1s (=1100ms) entre 2 impulsions de signal, soit 0.9Hz / 55 RPM Min
            * Sur la base du "Counter Period" maximal autorisé (sur 16bits) de 65535 (ci-dessous "Period"), PSC = "Prescaler -1" avec Prescaler minimum = (int)((FAPB * deltaMax)/(Period +1))   
            Remarque : plus le Prescaler est petit, plus précise sera la mesure.   
            Exemple 1 : pour deltaMax = 100ms et FAPB @ 60MHz => Prescaler = 91.55 => PSC = 92 -1 = 91   
            Exemple 2 : pour deltaMax = 1.10s et FAPB @ 60MHz => Prescaler = 1007.08 => PSC = 1007   
            * Counter Mode = "Up"
            * Internal Clock Division CKD = "No Division"
            * auto-reload preload = "Disable"

        4. Configurer chaque "Input Capture Channel *" concerné tel que :
            * Polarity Selection = "Rising Edge"
            * IC Selection = "Direct"
            * Prescaler Division Ratio = "No division"
            * Input Filter = "0"

    4. Pour chacun des Périphériques TIM# concernés, configurer "NVIC Settings" tel que :   
        "TIM# global interrupt" soit bien coché "Enabled"   
        si proposé : laisser "Preemption Priority" à 0,   
        si proposé : laisser "Sub Priority" à 0.   

    5) Dans la Catégorie "System Core" -> "NVIC" -> "Code generation", s'assurer que, pour chaque "TIM# global interrupt" concerné, soient cochés "Generate IRQ handler" & "Call HAL handler"

3) Configurer le Projet CubeMx :
    1) Dans l'onglet général "Project Manager" -> "Code Generator" s'assurer des Paramètres suivants :
        * "Generate peripheral initialization as a pair of '.c/h' files per peripheral" -> coché
        * "Keep User Code when re-generating" -> coché
        * Je recommande de cocher également "Delete previously generated files when not re-generated".

    2) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Driver Selector" :
        * s'assurer que tous les Timers pour cette Librairie soient de type "HAL".

    3) Dans l'onglet général "Project Manager" -> "Advanded Settings" -> "Generated Function Calls" :
        * Pour chaque Timer à utiliser, s'assurer que soient bien cochés : "Generate Code" (à gauche) & "Visibility (Static)"
        * Pour chaque Timer à utiliser, s'assurer que soit bien décoché "Do Not Generate Function Call".

Remarque : je recommande de re- "GENERATE CODE" si l'un des paramètres ci-dessus a été modifié dans CubeMX !

## Etape II : Configurer la lib 
**Dans le fichier "FanPwmIcConf.h"**

1) Indiquer dans "FPIC_NB_FANS_WITH_FEEDBACK" (cf. plus bas) le nombre de Ventilateurs AVEC retour Feedback
 que devra gérer cette Librairie

2) Indiquer dans "FPIC_NB_FANS_ONLY_COMMAND" (cf. plus bas) le nombre de Ventilateurs SANS retour Feedback
 que devra gérer cette Librairie

3) Indiquer dans "FPIC_NB_FANS_ONLY_FEEDBACK" (cf. plus bas) le nombre d'entrées Feedback, SANS commande,
 que devra gérer cette Librairie

4) Indiquer dans "FPIC_NB_MAX_OF_INIT_PARAMS" (cf. plus bas) le nombre total d'éléments que devra gérer cette Librairie

5) Décommenter et Configurer "FPIC_MOY_FEED_BACK_CONV" (cf. plus bas) pour le nombre d'intervalles de temps à intégrer dans la Moyenne
 glissante interne
-> Mettre en commentaire pour désactiver la Moyenne glissante automatique intégrée.

6) Décommenter ou Commenter "FPIC_GET_LAST_DELTA_TIME" (cf. plus bas) pour demander à récupérer le dernier deltaTime calculé

7) Décommenter ou Commenter "FPIC_GET_LAST_FEED_BACK" (cf. plus bas) pour demander à récupérer le dernier résultat de calcul Feedback

8) Décommenter "FPIC_FEED_BACK_IS_FLOAT" (cf. plus bas) pour que le résultat final de Feedback soit moyenné puis stocké en Float
    * Remarque : le Float permet d'être plus précis sur les calculs, au détriment d'un impact plus fort sur la RAM
    * Mettre en commentaire pour que la moyenne puis le résultat final soient calculés & stocké en UINT16 (2x plus petit)

Remarque : il est recommandé de laisser "FPIC_FIRST_INIT_PARAM" & "FPIC_AFTER_INIT_PARAM" à leurs Paramètres par défaut,
 respectivement "FPIC_BaseInitParam" et "FPIC_EndInitParams".

## Etape III : Configurer la version 
**Dans le fichier "FanPwmIcUser.c"**

1) Pour chaque élément AVEC Commande PWM & Retour Tachy :

    1) Configurer "FAN?_DATA_POS" comme indice dans le tableau "mFanData"
    ```
    //Exemple 1 : Gestion PWM du Ventilateur n°1 via l'index 0 dans mFanData (PWM & IC)
    #define FAN1_DATA_POS			0				// Position 0 dans mFanData (PWM & IC)

    //Exemple 2 : Gestion PWM du Ventilateur n°2 via l'index 1 dans mFanData (PWM & IC)
    #define FAN2_DATA_POS			1				// Position 1 dans mFanData (PWM & IC)
    ```
    Remarque : "FAN?_DATA_POS" doit être inférieur à FPIC_NB_FANS_WITH_FEEDBACK.

    2) Configurer "FAN?_PWM_HANDLE" et "FAN?_PWM_CHANNEL_ID" sur le Timer & Channel qui gère la sortie physique PWM
    ```
    //Exemple 1 : Sortie PWM du Ventilateur n°1 sur Timer 9 Channel 1
    #define FAN1_PWM_HANDLE 		&htim9
    #define FAN1_PWM_CHANNEL_ID 	TIM_CHANNEL_1	// TIM9_CH1
    
    //Exemple 2 : Sortie PWM du Ventilateur n°2 sur Timer 9 Channel 2
    #define FAN2_PWM_HANDLE 		&htim9
    #define FAN2_PWM_CHANNEL_ID 	TIM_CHANNEL_2	// TIM9_CH2
    ```
    Remarque 1 : "FAN?_PWM_HANDLE" doit toujours être l'un des "extern TIM_HandleTypeDef" répertoriés dans "tim.h"
    -> Rappel : ne pas oublier le '&' devant "htim*"
    
    Remarque 2 : "FAN?_PWM_CHANNEL_ID" doit toujours être l'une des valeurs
     du groupe "TIM_Channel TIM Channel" (cf. "stm*_hal_tim.h").

    3) Configurer la précision, ainsi que la dynamique, à l'aide via "FAN?_PWM_K_FACTOR" et "FAN?_PWM_MAX_VALUE"
    ```
    //Exemple de Correspondance PWM @ 1mv sur 10V :
    #define FAN1_PWM_K_FACTOR		1				// Correspondance User 1mv => PWM = 1
    #define FAN1_PWM_MAX_VALUE  	10000			// Max = 10.0V
    ```
    4) Configurer "FAN?_IC_SRC_DATA_POS" comme indice dans le tableau "mFanIcDatas"
    ```
    //Exemple 1 : Retour Tachy du Ventilateur n°1 via l'index 0 dans mFanIcDatas
    #define FAN1_IC_SRC_DATA_POS	0				// Position 0 dans mFanIcDatas
    
    //Exemple 2 : Retour Tachy du Ventilateur n°2 via l'index 1 dans mFanIcDatas
    #define FAN2_IC_SRC_DATA_POS	1				// Position 1 dans mFanIcDatas
    ```
    5) Configurer "FAN?_IC_HANDLE" et "FAN?_IC_CHANNEL_ID" sur le Timer & Channel qui Capture le retour Tachy
    ```
    //Exemple 1 : Retour Tachy du Ventilateur n°1 sur Timer 12 Channel 1
    #define FAN1_IC_HANDLE  		&htim12
    #define FAN1_IC_CHANNEL_ID  	TIM_CHANNEL_1	// TIM12_CH1
    
    //Exemple 2 : Retour Tachy du Ventilateur n°2 sur Timer 9 Channel 2
    #define FAN2_PWM_HANDLE 		&htim9
    #define FAN2_PWM_CHANNEL_ID 	TIM_CHANNEL_2	// TIM9_CH2
    ```
    6) Configurer le TimeOut Tachy sur "FAN?_IC_TIME_OUT", à l'aide de la macro "FPIC_MAKE_TIME_OUT_ms"
    ```
    //Exemple : Habituellement 1100ms, soit 1.1s
    #define FAN1_IC_TIME_OUT		FPIC_MAKE_TIME_OUT_ms(1100)	// TimeOut = 1.1s (pour RPM > 55)
    ```
    7) Configurer la fréquence APB du Timer de Tachy sur "FAN?_IC_FAPB", à l'aide de la macro "FPIC_MAKE_FAPB_MHz"
    ```
    //Exemple : Retour Tachy du Ventilateur n°1 (Tim12) sur APB1 @ 60MHz
    #define FAN1_IC_FAPB			FPIC_MAKE_FAPB_MHz(60)	// Tim12 sur APB1 @ 60MHz
    ```
    8) Renseigner "FAN?_IC_PSC" avec la valeur du Prescaler PSC de CubeMx (= htim#.Init.Prescaler dans "tim.c")
    ```
    //Exemple pour le Retour Tachy du Ventilateur 1 :
    #define FAN1_IC_PSC 			1007
    ```
    9) Configurer dans "FAN?_IC_PPT" le nombre de "Pulse Per Turn" (PPT) pour le Retour Tachy du Ventilateur considéré
    ```
    //Exemple pour un Retour Tachy d'1 Impulsion par Tour pour le Ventilateur 1 :
    #define FAN1_IC_PPT 			1	// Nb of "Pulse Per Turn" (PPT) from the fan feedback, generally only 1
    ```


2) Pour chaque élément AVEC Commande PWM mais SANS Retour Tachy :

    1) Configurer "FAN?_DATA_POS" comme indice dans le tableau "mFanCmdOnlyData"
    ```
    //Exemple : Gestion PWM du Ventilateur n°3 via l'index 0 dans mFanCmdOnlyData (only PWM)
    #define FAN3_DATA_POS			0				// Position 0 dans mFanCmdOnlyData (only PWM)
    ```
    Remarque : "FAN?_DATA_POS" doit alors être inférieur à FPIC_NB_FANS_ONLY_COMMAND.

    2) Configurer "FAN?_PWM_HANDLE", "FAN?_PWM_CHANNEL_ID", "FAN?_PWM_K_FACTOR" et "FAN?_PWM_MAX_VALUE" comme pour une Commande PWM AVEC Retour Tachy (cf. plus haut)

    3) Configurer "FAN?_IC_SRC_DATA_POS", "FAN?_IC_HANDLE", "FAN?_IC_CHANNEL_ID", "FAN?_IC_CHANNEL_FLAG", "FAN?_IC_SRC_DATAS, "FAN?_IC_GET_DATAS", ainsi que "FAN?_IC_K_FEED_BACK" à "0"

3) Pour chaque élément avec Retour Tachy mais SANS Commande PWM :

    1) Configurer "FAN?_DATA_POS" comme indice dans le tableau "mFanIcOnlyData"
    ```
    //Exemple : Traitement Retour Tachy du Ventilateur n°4 via l'index 0 dans mFanIcOnlyData (only InputCapture)
    #define FAN4_DATA_POS			0				// Position 0 dans mFanIcOnlyData (only InputCapture)
    ```
    2) Configurer "FAN?_PWM_HANDLE", "FAN4_PWM_CHANNEL_ID",     "FAN4_PWM_DATAS" à "0"
    
    3) Configurer "FAN?_IC_SRC_DATA_POS", "FAN?_IC_HANDLE", "FAN?_IC_CHANNEL_ID", "FAN?_IC_TIME_OUT", "FAN?_IC_FAPB", ainsi que "FAN?_IC_PSC" et "FAN?_IC_PPT" comme pour un Retour Tachy AVEC Commande PWM (cf. plus haut)

4) Pour chaque élément avec Commande PWM, implémenter les fonctions "Get" & "Set" appropriées
```
//Exemple 1 pour Commande d'un Ventilateur "Exhaust" sur "FAN1" :
void setFanExhaustVoltage_mV(uint16_t newVoltage)
{
	FanPwmIC_UpdatePwmValue(FAN1_PWM_DATAS, FAN1_PWM_K_FACTOR * newVoltage);
}
uint16_t getFanExhaustVoltage_mV(void)
{
	return (uint16_t)(FanPwmIC_getFanVoltage_mV(FAN1_PWM_DATAS) / FAN1_PWM_K_FACTOR);
}

//Exemple 2 pour Commande d'un Ventilateur "Supply" sur "FAN2" :
void setFanSupplyVoltage_mV(uint16_t newVoltage)
{
	FanPwmIC_UpdatePwmValue(FAN2_PWM_DATAS, FAN2_PWM_K_FACTOR * newVoltage);
}
uint16_t getFanSupplyVoltage_mV(void)
{
	return (uint16_t)(FanPwmIC_getFanVoltage_mV(FAN2_PWM_DATAS) / FAN2_PWM_K_FACTOR);
}
```
5) Pour chaque élément avec Retour Tachy, implémenter la fonction "Get" appropriée
```
//Exemple 1 pour Retour Tachy d'un Ventilateur "Exhaust" sur "FAN1" :
uint16_t getFanExhaustFeedbackSpeed(void)
{
	return FanPwmIC_getFanSpeed(FAN1_IC_GET_DATAS);
}

//Exemple 2 pour Retour Tachy d'un Ventilateur "Supply" sur "FAN2" :
uint16_t getFanSupplyFeedbackSpeed(void)
{
	return FanPwmIC_getFanSpeed(FAN2_IC_GET_DATAS);
}
```

## Etape IV : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter l'Include du .h dans le "main.c" (par exemple parmi les "USER CODE * Includes"), ainsi qu'une Base de Temps appropriée (par exemple dans "USER CODE * Includes" de "BaseDeTemps.c")
```
#include "FanPwmIcCore.h"			// Pour accès à la Gestion du Ventilateur
```
2) Ajouter l'appel d'Init dans le "main" du "main.cpp" (par exemple parmi les "USER CODE * 2") :
```
InitFanPwmIC();					// A appeler dans la partie Init Software (main.cpp)
```
3) Ajouter le Handler @ 10ms dans une sous-partie de la Boucle Principale (RT) à 10ms :  
(par exemple dans la zone @ 10ms "USER CODE * RT_10ms" de "GestionBaseDeTemps", dans "BaseDeTemps.c")
```	
Handle_FanPwmIC_RT_10ms();		// A appeler depuis une Base de Temps RT @ 10ms
```
4) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c" :   
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
Gestion_FanPwmIC();				// A appeler dans la Boucle Principale (main.c)
```
5) Ajouter le Dossier "FanPwmIC" à l' "IncludePath" pour toutes les Configs de Build  
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)  
-> il est souvent préférable d'utiliser la formulation "../User/FanPwmIC" (sans les guillemets)  
Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

6) Vérifier que le Dossier "FanPwmIC" ne soit pas "Exclude From Build" des Config.
 (y compris "Debug" & "Release")  
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

7) Ajouter au projet l'une des Librairie "Utils" compatibles, comme celle de "HII_Manta_App" ou "HII_CarteMere_App".  
    -> se reporter à "utils.h" pour intégrer plus facilement votre Librairie "Utils".

**remarques :**  

    Ventilateur CEBI P14011 :  
		commande ventilateur :  
		FAPB = 240 Mhz, Counter period = 10000, Freq cible PWM = 12khz  
		PSC = (((FAPB / Période) / Cible) -1)  
		PSC = (((240*10^(6))/(10000))/(12*10^3))-1 = 1  

		retour tachy :  
		FAPB = 240 Mhz, Counter period = 65535, deltaMax = 1100ms entre 2 impulsions de signal, soit 0.9Hz / 54 RPM Min  
		PSC = "Prescaler -1" avec Prescaler minimum = (int)((FAPB * deltaMax)/(Period +1))  
		PSC = (((240*10^(6))*(1100*10^(-3)))/(65535+1)) = 4028.3 = 4029 -1 = 4028  


Félicitations, c'est prêt :-) !

