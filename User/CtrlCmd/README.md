# CtrlCmd Code Generer 


## Code Generer a partir du model TFlow4_Ctrl N0 version CPP   
[TFlow4 Ctrl N0](http://srv-balma-app-02:8082/regul/projects/tflow4/-/releases/N0)     
 
# Procédure pour intégrer le code généré  
## Etape I : Intégrer les srouces dans le Projet 
**Dans le nouveau Projet**
1) Crée un dossier CtrlCmd  

2) Ajouter tous les dossiers se trouvant dans "slprj/ert" sauf "_htmllib"

3) Ajouter les fichiers commencant par "TFLOW4_Ctrl" à la racine du dossier CtrlCmd

4) Ajouter le Dossier "CtrlCmd" et ses sous-dossiers à l' "IncludePath", pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
-> il est souvent préférable d'utiliser la formulation "../User/CtrlCmd" (sans les guillemets)

5) Vérifier que le Dossier "CtrlCmd" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")   
Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".
   
## Etape II : Configurer 
**Dans le fichier FirmwareStateMachine.hpp/cpp**
   
1) Ajouter l'include principale dans le FirmwareStateMachine.hpp   
```  
	#include "TFLOW4_Ctrl.h"
```     
   
2) Ajouter l'objet TFLOW4_Ctrl en tant que membre de la class FwMng dans le fichier FirmwareStateMachine.hpp   
```  
/********************************************************************************************/
// User variable
	TFLOW4_Ctrl *CtrlCmd;
```     
   
3) Instancier et initialiser l'objet TFLOW4_Ctrl dans le constructeur de FwMng dans le fichier FirmwareStateMachine.cpp   
```  
/********************************************************************************************/
// User Init
	CtrlCmd = new TFLOW4_Ctrl;
	CtrlCmd->initialize();
```     
   
   
4) executer la régulation dans la méthode run de la classe FwMng   
```  
	if(regReset == TRUE){
		regReset = FALSE;
		CtrlCmd->initialize();
	}
	else {
		if(powerOnTimer >= POWER_ON_WAIT){
			CtrlCmd->step();
		}
	}
```
   
5) alimenter la structure d'entrée et cablé la structure de sortie au différent organes   


Félicitations, c'est prêt :-) !

