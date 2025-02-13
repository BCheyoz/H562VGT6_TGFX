/*
 * I2cComMasterConf.h
 *
 *  Created on: 28 sept. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 *  Version 1.0
 */

#ifndef USER_I2CCOMMASTERCORE_I2CCOMMASTERCONF_H_
#define USER_I2CCOMMASTERCORE_I2CCOMMASTERCONF_H_

/******************************************************************************************/

// Choix du Driver I2C : soit les HAL I2C standard, soit un Driver Custom (un seul choix possible à la fois)
#define I2CCM_USE_I2C_HAL_DRIVER			// (par défaut) Pour utiliser les Librairies & Drivers système de type HAL
//#define I2CCM_USE_I2C_CUSTOM_DRIVER 		// Pour utiliser le Driver custom ALDES "I2cSoftComPort" à la place des HAL

// Capteurs de Pression supportés :
#define I2CCM_NB_MAX_DEV_PRESS_HSC  	0	// Aucun Capteur de Pression HSC à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_PRESS_SDP6 	0	// Aucun Capteur de Pression SDP6 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_PRESS_SDP8 	0	// Aucun Capteur de Pression SDP8 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_PRESS_LMI  	0	// Aucun Capteur de Pression LMI à gérer simultanément sur l'ensemble des I2C

// Capteurs de Co2 supportés :
#define I2CCM_NB_MAX_DEV_CO2_EE894  	0	// Aucun Capteur de Co2 EE894 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_CO2_EE895  	1	// 1 seul Capteur de Co2 EE895 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_CO2_SCD3x  	1	// 1 seul Capteur de Co2 SCD3x à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_CO2_SCD4x  	0	// Aucun Capteur de Co2 SCD4x à gérer simultanément sur l'ensemble des I2C

// Capteurs de COV supportés :
#define I2CCM_NB_MAX_DEV_COV_CCS811		0	// Aucun Capteur de COV CCS811 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_COV_SGP40		0	// Aucun Capteur de COV SGP40 à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_COV_ENS160		0	// Aucun Capteur de COV ENS160 à gérer simultanément sur l'ensemble des I2C

// Capteurs de T° & HR supportés :
#define I2CCM_NB_MAX_DEV_HR_SHT3x		0	// Aucun Capteur de T° & HR SHT3x à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_HR_SHT4x		0	// Aucun Capteur de T° & HR SHT4x à gérer simultanément sur l'ensemble des I2C
#define I2CCM_NB_MAX_DEV_HR_HTU31		0	// Aucun Capteur de T° & HR HTU31D à gérer simultanément sur l'ensemble des I2C

// Périphériques RTC supportés :
#define I2CCM_NB_MAX_DEV_RTC_RV3028 	1	// 1 seul Périphérique RTC "RV3028" à gérer simultanément sur l'ensemble des I2C

//#define I2CCM_ENABLE_I2C_DEBUG				// Pour activer la mise à disposition générale d'Infos I2c Interne
//#define I2CCM_ENABLE_I2C_IBUS_DEBUG		// Pour activer la capture d'Infos I2c Interne vers l'iBus
//#define I2CCM_ENABLE_DEBUG_MULTI_EXECUTION	// Pour activer les envois multiples de DEBUG I2C_RTC

#endif /* USER_I2CCOMMASTERCORE_I2CCOMMASTERCONF_H_ */
