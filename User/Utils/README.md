# Utils Lib v1.0


## History Usage 
* 25/02/2022 : Added by AB to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)
* 03/02/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


# Procédure pour intégrer cette Librairie

 1) Ajouter le Dossier "Utils" à l' "IncludePath", pour toutes les Configs de Build.   
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)    
-> il est souvent préférable d'utiliser la formulation "../User/Utils" (sans les guillemets)
   
Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings".
   
 2) Vérifier que le Dossier "Utils" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")   
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".
   
Félicitations, c'est prêt :-) !

