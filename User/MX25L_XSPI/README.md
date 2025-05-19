# Librairie `MX25L_XSPI.c` v1.0.0
Copyright © ALDES 2025 -
Updated on 19 May 2025, by j.proux (@JoelP)


## History Usage :
* 13/05/2025 : Added by @JoelP to [TFL4_CarteMere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app) (STM32H562VGT6)

>[!ToDo] Poursuivre la Mise à Jour de ce ReadMe.md.

# Procédure pour intégrer cette Librairie
## Etape I : Configurer CubeMX 

**Dans CubeMX :**

1) Dans CubeMx, configurer les HAL.

## Etape II : Dans le fichier "MX25L_SPI.h", Configurer :

1) Configurer les RDID selon les infos du Fabricant :
* SPI_MEM_MX25L_RDID_MFG <=> Manufacturer ID
* SPI_MEM_MX25L_RDID_MMT <=> Memory Type
* SPI_MEM_MX25L_RDID_MMD <=> Memory Density

## Etape III : Configurer le projet

**Dans le nouveau Projet**

1) Ajouter le Dossier "Mem_MX25L_XSPI" à l' "IncludePath" pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
il est souvent préférable d'utiliser la formulation "../User/Mem_MX25L_XSPI" (sans les guillemets)

2) Vérifier que le Dossier "Mem_MX25L_XSPI" ne soit "Exclude From Build" d'aucune des Configs.
  (y compris "Debug" & "Release")
*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".

   
Félicitations, c'est prêt :-) !



/*
#define Flash_Qspi_IO3_Pin GPIO_PIN_6
#define Flash_Qspi_IO3_GPIO_Port GPIOA
#define Flash_Qspi_IO2_Pin GPIO_PIN_7
#define Flash_Qspi_IO2_GPIO_Port GPIOA
#define Flash_Qspi_IO1_Pin GPIO_PIN_0
#define Flash_Qspi_IO1_GPIO_Port GPIOB
#define Flash_Qspi_IO0_Pin GPIO_PIN_1
#define Flash_Qspi_IO0_GPIO_Port GPIOB
#define Flash_Qspi_Clk_Pin GPIO_PIN_2
#define Flash_Qspi_Clk_GPIO_Port GPIOB
*/