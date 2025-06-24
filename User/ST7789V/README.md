# ST7789 Lib v1.0


## History Usage 
* 27/05/2025 : Added by BC to [TFL4_Cartemere_App](https://git-ext.aldes.com/be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app/-/tree/develop) (STM32H562VGTX)


# Procédure pour intégrer cette Librairie
**Vérifier que l'afficheur ciblé utilise le driver ST7789 ou un équivalent**

## Etape I : Configurer le Projet 
**Dans le nouveau Projet**
1) Ajouter le Dossier "ST7789" à l' "IncludePath" pour toutes les Configs de Build    
(Attention : CubeIDE a l'habitude de stocker les chemins relatifs au Workspace, et non au Dossier)   
il est souvent préférable d'utiliser la formulation "../User/ST7789" (sans les guillemets) 

*Info : Pour vérifier, sélectionner "Properties" du Projet -> "C/C++ Build" -> "Settings" -> "Tool Settings" -> "MCU GCC Compiler" -> "Include paths".*

2) Vérifier que le Dossier "ST7789" ne soit pas "Exclude From Build" des Config. (y compris "Debug" & "Release")  

*Info : Pour vérifier, sélectionner "Properties" du Dossier -> "C/C++ Build" -> "Settings".*

3) Ajouter l'Include du .h dans le(s) fichier(s) du driver de l'afficheur :
```
	#include "ST7789.h"
```

4) définir les pointeurs de fonction pour initialiser et envoyer des commandes en fonction du type de Com de l'afficheur (spi 3/4 lignes, parallèles, rgb).    
Les fonctions doivent respecter les signatures des fonctions décrite dans "ST7789.h"
```
	// Exemple
	ST7789_IO_t IOCtx = { 0 };
	
	/* Configure le driver ST7789 pour utiliser les requetes SPI*/
	IOCtx.Init             = LCD_IO_Init;
	IOCtx.DeInit           = LCD_IO_DeInit;
	IOCtx.ReadReg          = LCD_IO_ReadReg;
	IOCtx.WriteReg         = LCD_IO_WriteReg;
	IOCtx.SendData         = LCD_IO_SendData;
	IOCtx.SendDataDMA      = NULL;
	IOCtx.RecvData         = LCD_IO_RecvData;
	IOCtx.RecvDataDMA      = NULL;
	IOCtx.GetTick          = LCD_IO_GetTick;
	IOCtx.Delay            = LCD_IO_Delay;
	
```

5) Configurer les parametres en fonction des caractéristiques de l'afficheur :   
```
	// Exemple
	ST7789_InitParams_t ST7789_InitParams;
	ST7789_InitParams.Endian         = ST7789_ENDIAN_BIG;
	ST7789_InitParams.SwapRB         = 0;	// false
	ST7789_InitParams.InvertColor    = 1;	// true
	ST7789_InitParams.ColorCoding    = ST7789_FORMAT_RBG565;
	ST7789_InitParams.Orientation    = ST7789_ORIENTATION_LANDSCAPE;
	ST7789_InitParams.FrameRate      = ST7789_60_Hz;
	ST7789_InitParams.TEScanline     = 0;
	ST7789_InitParams.TEMode         = ST7789_TE_DISABLED;
	ST7789_InitParams.Timings.hsync  = ST7789_HSYNC;
	ST7789_InitParams.Timings.hbp    = ST7789_HBP;
	ST7789_InitParams.Timings.hfp    = ST7789_HFP;
	ST7789_InitParams.Timings.vsync  = ST7789_VSYNC;
	ST7789_InitParams.Timings.vbp    = ST7789_VBP;
	ST7789_InitParams.Timings.vfp    = ST7789_VFP;
```

6) Initialiser le driver de l'afficheur avec les parametres précédente via la fonction "ST7789_Init" :
```
	// Exemple
	ObjCtx.IO = IOCtx;
	ObjCtx.IsInitialized = 0;
	int32_t ret = ST7789_Init(&ObjCtx, &ST7789_InitParams);
```

Ci-dessous un exemple de commandes pour afficher une image :
    

    
```
uint8_t buffer[SCREEN_SIZE]; // width * height * depth
uint32_t XposStart = 0; 
uint32_t YposStart = 0;
uint32_t XposEnd = width; 
uint32_t YposEnd = height;
ST7789_SetDisplayWindow(&ObjCtx, XposStart, YposStart, XposEnd, YposEnd);
IOCtx.SendData(buffer, SCREEN_SIZE);
ST7789_DisplayOn(&ObjCtx);
```

Félicitations, c'est prêt :-) !

