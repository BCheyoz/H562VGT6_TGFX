# MX25L_SPI c Lib v1.0

Basé sur la libraire Mem_MX25L_SPI.c crée par j.proux

## History Usage :
* 14/01/2020 : Added by AA to Nükub732_Firmware (STM32F732VE : productprojects/ventilation/double-flux/nukub/Nukub_firmware)
* 14/06/2021 : Added by BC to CarteAqui_BestCC (STM32F732VE : innoprojects/carte-acquisition/banc-de-test-statique/best-cc)
* 04/08/2023 : Added by AB to HII_CarteMere_App (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_app)
* 10/01/2024 : Added by Jp to HII_CarteMere_Bootloader (STM32F732VETx : productprojects/ventilation/individuel/himalaya2/carte-mere/h2_cartemere_bootloader)

-> Merci d'utiliser une version récente fournée par ST.

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

1) Ajouter le Dossier "Mem_MX25L_SPI" à l' "IncludePath" pour toutes les Configs de Build.
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)
il est souvent préférable d'utiliser la formulation "../User/Mem_MX25L_SPI" (sans les guillemets)

2) Vérifier que le Dossier "Mem_MX25L_SPI" ne soit "Exclude From Build" d'aucune des Configs.
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