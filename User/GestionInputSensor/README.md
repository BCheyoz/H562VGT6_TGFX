# GestionInputSensor Lib v1.0  

ATTENTION : cette librairie peux utiliser la Librairie "I2cComMasterSystem", pensez a intégrer la librairie dépendante.

## History Usage 
* 16/12/2021 : Added by BC to [H2_Manta_App](http://srv-balma-app-02:8082/productprojects/ventilation/individuel/himalaya2/manta/h2_manta_app) (STM32G070CBTX)
* 14/11/2022 : Added by JP to [H2_Cartemere_App](http://srv-balma-app-02:8082/productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app/-/tree/develop) (STM32F732VETX)
* 10/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)

# Procédure pour intégrer cette Librairie   
## Etape I : Configurer le Projet 
**Dans le nouveau Projet**

1) Ajouter le Dossier "I2c_Press_ABP2" à l' "IncludePath" pour toutes les Configs de Build.  
  (Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)    
 il est souvent préférable d'utiliser la formulation "../User/I2c_Press_ABP2" (sans les guillemets)

Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".

2) Vérifier que le Dossier "I2c_Press_ABP2" ne soit "Exclude From Build" d'aucune des Configs.  
  (y compris "Debug" & "Release")  
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".  

## Etape II : 
**TODO**


Félicitations, c'est prêt :-) !

