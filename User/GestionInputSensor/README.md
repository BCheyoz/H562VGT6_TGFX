# GestionInputSensor Lib v1.0  

**ATTENTION** : cette librairie peux utiliser plusieurs Librairie (I2cComMasterSystem, AnalogInputsUser, etc.), pensez a intégrer la librairie dépendante.

## History Usage 
* 16/12/2021 : Added by BC to [H2_Manta_App](http://srv-balma-app-02:8082/productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app) (STM32G070CBTX)
* 14/11/2022 : Added by JP to [H2_Cartemere_App](http://srv-balma-app-02:8082/productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app/-/tree/develop) (STM32F732VETX)
* 10/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie   
## Etape I : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier "GestionInputSensor" à l' "IncludePath" pour toutes les Configs de Build.  
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)    
 il est souvent préférable d'utiliser la formulation "../User/GestionInputSensor" (sans les guillemets)

Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

2) Vérifier que le Dossier "GestionInputSensor" ne soit "Exclude From Build" d'aucune des Configs.  
  (y compris "Debug" & "Release")  
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".  

3) Ajouter l'Include du .h dans le "main.c/.cpp" (par exemple parmi les "USER CODE * Includes"),
 ainsi qu'une Base de Temps appropriée (par exemple dans la zone "USER CODE * Includes" du .c
 de la Librairie "BaseDeTemps") :
```
	#include "GestionInputSensor.h"
```

4) Ajouter l'appel d'Init dans le "main" du "main.c/.cpp" (par exemple parmi les "USER CODE * 2") :
```
	InitInputSensor();
```

5) Ajouter l'appel de Gestion dans la Boucle Principale du "main.c/.cpp" :   
(Attention à bien l'insérer entre l'accolade ouvrante du "while (1)" et "USER CODE END WHILE")
```
	GestionInputSensor();
```

4) Ajouter le Handler @ 100ms dans une sous-partie de la Boucle Principale (RT) à 100ms :
(par exemple dans la zone @ 100ms "USER CODE * RT_100ms" de "GestionBaseDeTemps" ("BaseDeTemps.c")
```
	Handle_InputSensor_RT_100ms();
```


## Etape II : Configurer les capteurs a utiliser
**Dans GestionInputSensor.h**

1) activer la ou les sources d'entrées des capteurs 
    ```
    #define USE_I2C_SENSOR TRUE
    #define USE_ADC_SENSOR TRUE
    ```
    
    1) Pour les capteurs sur l'i2c, paramétrer le nombre et le type de capteurs qui sera actif en parallèle 
        ```
        #define NB_PRESSURE_SENSOR_USED 1
        #define NB_COV_SENSOR_USED 0
        #define NB_CO2_SENSOR_USED 0
        #define NB_HR_TEMP_SENSOR_USED 0
        ```

**Dans GestionInputSensor.c**  
1) Pour chaques type de capteur :   
    1) Définir le nombre de référence HW du capteur   
    ```
    #define I2C_NB_PRESS_DEVICE 4
    ```
    2) Remplir la list de référence des capteurs qui peuvent être monté.    
    1er paramètre : le pointeur sur la structures du capteurs   
    2eme paramètre : L'enum du capteur définit dans le .h  
    3eme paramètre : Toujour à 0
    ```
    static gis_i2cDevice i2cDevicePressure[I2C_NB_PRESS_DEVICE] = {
		{&mPresHSC, SENSOR_HSC, 0},
		{&mPresABP2, SENSOR_ABP2, 0},
		//{&mPresSDP6, SENSOR_SDP6, 0},
		{&mPresSDP8, SENSOR_SDP8, 0},
		{&mPresLMI, SENSOR_LMI, 0},
    };
    ```



Félicitations, c'est prêt :-) !

