/*
 * I2cComMasterSystem.h
 *
 *  Created on: 8 nov. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 24 Fév. 2023
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 08/11/2022 : Added by Jp to HII_CarteMere_App
 *
 */

#ifndef I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSTEM_H_
#define I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSTEM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN Includes */

// Includes des Capteurs de Pression :
//#include "I2cDevPressureSDP8.h" 		// Pour accès au Capteur de Pression SDP8
//#include "I2cDevPressureLMI.h"  		// Pour accès au Capteur de Pression LMI
//#include "I2cDevPressureSDP6.h" 		// Pour accès au Capteur de Pression SDP6

// Includes des Capteurs de Co2 :
//#include "I2cDevCo2_SCD3x.h"			// Pour accès au Capteur de Co2 SCD3x
//#include "I2cDevCo2_SCD4x.h"			// Pour accès au Capteur de Co2 SCD4x
//#include "I2cDevCo2_EE895.h"			// Pour accès au Capteur de Co2 EEE895

// Includes des Capteurs de COV :
//#include "I2cDevCov_SGP40.h"			// Pour accès au Capteur de COV SGP40
//#include "I2cDevCov_ENS160.h"			// Pour accès au Capteur de COV ENS160

// Includes des Capteurs de T° & HR :
//#include "I2cDevHr_SHT3x.h" 			// Pour accès au Capteur T° & HR SHT3x
//#include "I2cDevHr_SHT4x.h" 			// Pour accès au Capteur T° & HR SHT4x

// Includes des Périphériques RTC :
#ifdef I2C_USE_RTC_RV3028
#include "I2cDevRtc_RV3028.h" 			// Pour accès au Périphérique RTC RV3028
#endif
/* USER CODE END Includes */

/******************************************************************************/
// Variables Publiques, par exemple pour accès Modbus :

/* USER CODE BEGIN Public_Vars */

// Variables de Pression :
//extern I2CCM_Pres_HSC_ExtData 	mPresHSC;
//extern I2CCM_Pres_SDP6_ExtData	mPresSDP6;
//extern I2CCM_Pres_SDP8_ExtData	mPresSDP8;
//extern I2CCM_Pres_LMI_ExtData 	mPresLMI;

// Variables de Co2 :
//extern I2CCM_Co2_SCD3x_ExtData	mCo2SCD3;
//extern I2CCM_Co2_SCD4_ExtData 	mCo2SCD4;
//extern I2CCM_Co2_EE895_ExtData	mCo2EE895;

// Variables de COV :
//extern I2CCM_COV_SGP40_ExtData	mCovSGP40;
//extern I2CCM_COV_ENS160_ExtData	mCovENS160;

// Variables de HR :
//extern I2CCM_Hr_SHT3x_ExtData	mHrSHT3;
//extern I2CCM_Hr_SHT4x_ExtData	mHrSHT4;

// Variables de RTC :
#ifdef I2C_USE_RTC_RV3028
extern I2CCM_Rtc_RV3028_ExtData	mRtcRv3028;

extern I2CCM_RTC_RV3028_ManualRW rtcRV3028_ManualRW;
#define RTC_RV3028_MANUAL_BUF_SIZE	16
extern uint8_t rtcManualBuffer[RTC_RV3028_MANUAL_BUF_SIZE];
#endif
/* USER CODE END Public_Vars */

/******************************************************************************/
// Fonctions Publiques à appeler dans le "main.c" :
void I2cComMaster_Init_System(void);	// A appeler dans la partie Init (main.c)
void GestionI2cSystem(void);			// A appeler dans la Boucle Principale (main.c)

#ifdef I2C_USE_RTC_RV3028
uint16_t I2cSystem_StartWriteNewDateTime2RTC(void* pExtVar, RTC_CommonBase_DateTime* pNewDateTime);
void I2cSystem_HandleQueryReadRtcRegisters(uint8_t nb2Read);
void I2cSystem_HandleRtcManualMultiExecute_10ms(void);
#endif

#ifdef I2CCM_ENABLE_I2C_DEBUG	// cf. "I2cComMasterConf.h"
uint16_t getI2cSystemNbRestart(void);
uint16_t getI2cSystemCo2EE895ErrorsCt(void);
uint16_t getI2cSystemCo2SCD3xErrorsCt(void);
#endif // I2CCM_ENABLE_I2C_DEBUG

#ifdef __cplusplus
}
#endif

#endif /* I2CCOMMASTERSYSTEM_I2CCOMMASTERSYSTEM_H_ */
