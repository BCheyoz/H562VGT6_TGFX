/*
 * I2cComMasterSystem.c
 *
 *  Created on: 8 nov. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 30 Août 2023
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 08/11/2022 : Added by Jp to HII_CarteMere_App
 *
 */

#include "I2cComMasterSystem.h" 	// Pour accès à nos propres déclarations publiques
#include "I2cComMasterSysConf.h"	// Pour accès à la Configuration de l'I2C_System
#include "I2cComMasterUtils.h"  	// Pour accès aux Outils de support I2cComMaster

/******************************************************************************/

// Variables Spécifiques à l'I2C_System :
typedef enum
{
	I2cSysDevGrpNone = I2CCM_DEV_GROUP_UNKNOWN,
	I2cSysDevGrpPressure,	// Groupe System 1 : Capteurs de Pression (HSC, ABP2, SDP810 ou LMI2500)
	I2cSysDevGrpTempHR,		// Groupe System 2 : Capteurs de Température & Hygro (SHT3x, SHT4x ou HTU31D)
	I2cSysDevGrpCo2,		// Groupe System 3 : Capteurs de Co2 (EE894, EE895, SCD4x ou SCD3x)
	I2cSysDevGrpCOV,		// Groupe System 4 : Capteurs de COV (CCS811, SGP40 ou ENS160)
	I2cSysDevGrpRTC,		// Groupe System 5 : Périphériques RTC (RV3028)
} I2CCM_SystemDeviceGroupId;

typedef enum	// Identification des Familles de Capteurs de Pression :
{
	I2cDevPresFamilyNone	= I2CCM_DEV_FAMILY_UNKNOWN,
	I2cDevPresFamilyHSC 	= 0x0010,	// Capteur de Pression HSC, via "I2cDevPressureHSC" (.c & .h)
	I2cDevPresFamilySDP6	= 0x0020,	// Capteur de Pression SDP6, via "I2cDevPressureSDP6" (.c & .h)
	I2cDevPresFamilySDP8	= 0x0030,	// Capteur de Pression SDP8, via "I2cDevPressureSDP8" (.c & .h)
	I2cDevPresFamilyLMI 	= 0x0040,	// Capteur de Pression LMI, via "I2cDevPressureLMI" (.c & .h)
	I2cDevPresFamilyABP2 	= 0x0050,	// Capteur de Pression HSC, via "I2cDevPressureABP2" (.c & .h)
} I2CCM_DevicePressureFamily;

typedef enum	// Identification des Familles de Capteurs de Co2 :
{
	I2cDevCo2FamilyNone		= I2CCM_DEV_FAMILY_UNKNOWN,
	I2cDevCo2FamilySCD4 	= 0x0010,	// Capteur de Co2 SCD4x, via "I2cDevCo2_SCD4x" (.c & .h)
	I2cDevCo2FamilySCD3 	= 0x0020,	// Capteur de Co2 SCD3x, via "I2cDevCo2_SCD3x" (.c & .h)
	I2cDevCo2FamilyEE895 	= 0x0040,	// Capteur de Co2 EE895, via "I2cDevCo2_EE895" (.c & .h)
} I2CCM_DeviceCo2Family;

typedef enum	// Identification des Familles de Capteurs de COV :
{
	I2cDevCovFamilyNone		= I2CCM_DEV_FAMILY_UNKNOWN,
	I2cDevCovFamilyCCS8 	= 0x0010,	// Capteur de COV CCS8, via "I2cDevCov_CCS8" (.c & .h)
	I2cDevCovFamilySGP40 	= 0x0020,	// Capteur de COV SGP40, via "I2cDevCov_SGP40" (.c & .h)
	I2cDevCovFamilyENS160 	= 0x0040,	// Capteur de COV ENS160, via "I2cDevCov_ENS160" (.c & .h)
} I2CCM_DeviceCovFamily;

typedef enum	// Identification des Familles de Capteurs de T° & HR :
{
	I2cDevThrFamilyNone		= I2CCM_DEV_FAMILY_UNKNOWN,
	I2cDevThrFamilySHT3 	= 0x0010,	// Capteur de T°&HR SHT3x, via "I2cDevHr_SHT3x" (.c & .h)
	I2cDevThrFamilySHT4 	= 0x0020,	// Capteur de T°&HR SHT4x, via "I2cDevHr_SHT4x" (.c & .h)
	I2cDevThrFamilyHTU31 	= 0x0030,	// Capteur de T°&HR HTU31, via "I2cDevHr_HTU31" (.c & .h)
} I2CCM_DeviceTHrFamily;

typedef enum	// Identification des Familles de Périphériques RTC :
{
	I2cDevRtcFamilyNone		= I2CCM_DEV_FAMILY_UNKNOWN,
	I2cDevRtcFamilyRV3028 	= 0x0010,	// Périphérique RTC RV3028, via "I2cDevRtc_RV3028" (.c & .h)
} I2CCM_DeviceRtcFamily;

/* USER CODE BEGIN Public_Vars */

// Variables de Pression :
//I2CCM_Pres_SDP6_ExtData	mPresSDP6 = {0};
I2CCM_Pres_SDP8_ExtData 	mPresSDP8 = {0};
I2CCM_Pres_LMI_ExtData  	mPresLMI = {0};
I2CCM_Pres_HSC_ExtData 		mPresHSC = {0};
I2CCM_Pres_ABP2_ExtData		mPresABP2 = {0};

// Variables de Co2 :
//I2CCM_Co2_SCD3x_ExtData 	mCo2SCD3 = {0};
//I2CCM_Co2_SCD4_ExtData	mCo2SCD4 = {0};
//I2CCM_Co2_EE895_ExtData 	mCo2EE895 = {0};

// Variables de COV :
//I2CCM_COV_SGP40_ExtData 	mCovSGP40 = {0};
//I2CCM_COV_ENS160_ExtData	mCovENS160 = {0};

// Variables de HR :
//I2CCM_Hr_SHT3x_ExtData  	mHrSHT3 = {0};
//I2CCM_Hr_SHT4x_ExtData  	mHrSHT4 = {0};

// Variables de RTC :
#ifdef I2C_USE_RTC_RV3028
I2CCM_Rtc_RV3028_ExtData	mRtcRv3028 = {0};
#endif

#ifdef RV3028_RTC_ENABLE_MANUAL_RW
	I2CCM_RTC_RV3028_ManualRW	rtcRV3028_ManualRW = {0};
	uint8_t rtcManualBuffer[RTC_RV3028_MANUAL_BUF_SIZE];
#endif // RV3028_RTC_ENABLE_MANUAL_RW

/* USER CODE END Public_Vars */

/******************************************************************************/

const I2CCM_DevInitParams i2cSystemInitTable[] = {

/* USER CODE BEGIN InitTable */

	// Capteurs de T° & HR (SHT4x ou SHT3x) :
//	{ I2cSysDevGrpTempHR,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Hr_SHT4x_Init,		I2cDevThrFamilySHT4,	0, &mHrSHT4 },		// SHT4x est recherché en premier
//	{ I2cSysDevGrpTempHR,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Hr_SHT3x_Init,		I2cDevThrFamilySHT3,	0, &mHrSHT3 },  	// SHT3x sera chargé si SHT4x ne répond pas

	// Capteurs de COV (SGP40 ou ENS160) :
//	{ I2cSysDevGrpCOV,		I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Cov_SGP40_Init,		I2cDevCovFamilySGP40,	0, &mCovSGP40 },	// SGP40 est recherché en premier
//	{ I2cSysDevGrpCOV,		I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Cov_ENS160_Init,		I2cDevCovFamilyENS160,	0, &mCovENS160 },	// ENS160 sera chargé si SGP40 ne répond pas

	// Capteurs de Co2 (SCD3x ou EE895) :
//	{ I2cSysDevGrpCo2,		I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Co2_SCD3x_Init,		I2cDevCo2FamilySCD3,	0, &mCo2SCD3 },		// SCD3x est recherché en premier
//	{ I2cSysDevGrpCo2,		I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Co2_EE895_Init,		I2cDevCo2FamilyEE895,	0, &mCo2EE895 },	// EE895 sera chargé si SCD3 ne répond pas

	// Capteurs de Pression (LMI ou SDP8) :
	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureLMI_Init,		I2cDevPresFamilyLMI,	0, &mPresLMI },		// LMI est recherché en premier
	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureSDP8_Init,	I2cDevPresFamilySDP8,	0, &mPresSDP8 },	// SDP8 sera chargé si LMI ne répond pas
//	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureSDP6_Init,	I2cDevPresFamilySDP6,	0, &mPresSDP6 },
	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureHSC_Init,		I2cDevPresFamilyHSC,	0, &mPresHSC },
	{ I2cSysDevGrpPressure,	I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_PressureABP2_Init,	I2cDevPresFamilyABP2,	0, &mPresABP2 },

	// Périphérique RTC (RV3028) :
//	{ I2cSysDevGrpRTC,		I2CCM_CHANNEL_NONE, I2CCM_LOAD_DEFAULT_DEVICE,	i2cCM_Rtc_RV3028_Init,		I2cDevRtcFamilyRV3028,	0, &mRtcRv3028 },	// RV3028 est recherché en premier

/* USER CODE END InitTable */

};

I2CCM_Manage_Device	mI2cCmSystemDevice[I2CCM_MAX_DEV_SIM] = {0};

I2CCM_DevManager mI2cSystemManager = {0};
const I2CCM_DevManagerParams mI2cCmSystemManParams = {I2CCM_SIZED_OBJ_WITH_BASE_END(mI2cCmSystemDevice), I2CCM_SIZED_OBJ_WITH_BASE_END(i2cSystemInitTable)};

#define THIS_DEV_MANAGER_PTR	&mI2cSystemManager
#define THIS_DEV_MAN_PRMS_PTR	(I2CCM_DevManagerParams*)&mI2cCmSystemManParams

#ifdef I2CCM_ENABLE_I2C_DEBUG	// cf. "I2cComMasterConf.h"
	static I2CCM_Manage_Device* getI2cSystemManagedDeviceFromExtPtr(void* pExtStruct);
#endif // I2CCM_ENABLE_I2C_DEBUG

/******************************************************************************/

void I2cComMaster_Init_System(void)
{
	// Init de l'I2C :
	#ifdef I2CCM_FN_INIT
		I2CCM_FN_INIT();
	#endif // I2CCM_FN_INIT
}

/******************************************************************************/

void GestionI2cSystem(void)
{
	// Etape 1 : Gérer les Devices à charger / décharger / ré-Initialiser :
	if(0 != i2cComMasterHandleDeviceList(THIS_DEV_MANAGER_PTR, THIS_DEV_MAN_PRMS_PTR))
	{
		// Relance l'Init du Périphérique I2C :
		#ifdef I2CCM_FN_INIT
			I2CCM_FN_INIT();
		#endif // I2CCM_FN_INIT
	}

	// Etape 2 : Gérer les transactions I2c des Devices actuellement chargés :
	i2cComMasterHandleActiveDevice(THIS_DEV_MANAGER_PTR, I2CCM_HANDLE);
}

/******************************************************************************/
#ifdef I2C_USE_RTC_RV3028
uint16_t I2cSystem_StartWriteNewDateTime2RTC(void* pExtVar, RTC_CommonBase_DateTime* pNewDateTime)
{
	if(0 == pExtVar) return 0;

	I2CCM_ExternalAction tmpAction = {0};
	switch(I2CCM_DEV_FAMILY_MASK & ((I2CCM_ExternalBaseData*)pExtVar)->idOfDevice)	// Selon la Famille du Device concerné :
	{
#ifdef I2C_RTC_RV3028_I2CDEVRTC_RV3028_H_
	case I2cDevRtcFamilyRV3028:
		 tmpAction.ActionId = I2cCmRtcRv3028_XtrnAction_Write_DateTime;
		 break;
#endif // I2C_RTC_RV3028_I2CDEVRTC_RV3028_H_
	case I2CCM_DEV_FAMILY_UNKNOWN:
	default:
		 break;
	}
	if(0 != tmpAction.ActionId)
	{
		tmpAction.pParam = pNewDateTime;
		return i2cComMasterHandleExternalActionByExtVarPtr(THIS_DEV_MAN_PRMS_PTR, pExtVar, &tmpAction);
	}
	return 0;
}

/******************************************************************************/

uint16_t I2cSystem_StartReadRTC_Registers(void* pExtVar, I2CCM_RTC_RV3028_ManualRW* pManualRW)
{
	if(0 == pExtVar) return 0;

	I2CCM_ExternalAction tmpAction = {0};
	switch(I2CCM_DEV_FAMILY_MASK & ((I2CCM_ExternalBaseData*)pExtVar)->idOfDevice)	// Selon la Famille du Device concerné :
	{
#ifdef I2C_RTC_RV3028_I2CDEVRTC_RV3028_H_
	case I2cDevRtcFamilyRV3028:
		 tmpAction.ActionId = I2cCmRtcRv3028_XtrnAction_Read_Register;
		 if(0 != pManualRW)
		 {
			 pManualRW->pBuffer = rtcManualBuffer;
			 if(pManualRW->nbAdr > sizeof(rtcManualBuffer)) pManualRW->nbAdr = sizeof(rtcManualBuffer);
		 }
		 break;
#endif // I2C_RTC_RV3028_I2CDEVRTC_RV3028_H_
	case I2CCM_DEV_FAMILY_UNKNOWN:
	default:
		 break;
	}
	if(0 != tmpAction.ActionId)
	{
		tmpAction.pParam = pManualRW;
		return i2cComMasterHandleExternalActionByExtVarPtr(THIS_DEV_MAN_PRMS_PTR, pExtVar, &tmpAction);
	}
	return 0;
}

void I2cSystem_HandleQueryReadRtcRegisters(uint8_t nb2Read)
{
#ifdef RV3028_RTC_ENABLE_MANUAL_RW
	if(nb2Read > sizeof(rtcManualBuffer)) nb2Read = sizeof(rtcManualBuffer);
	rtcRV3028_ManualRW.nbAdr = nb2Read;
	rtcRV3028_ManualRW.pBuffer = rtcManualBuffer;
	I2cSystem_StartReadRTC_Registers(&mRtcRv3028, &rtcRV3028_ManualRW);
#endif // RV3028_RTC_ENABLE_MANUAL_RW
}

void I2cSystem_HandleRtcManualMultiExecute_10ms(void)
{
#ifdef RV3028_RTC_ENABLE_MANUAL_RW // from "I2cDevRtc_RV3028.h"
  #ifdef I2CCM_ENABLE_DEBUG_MULTI_EXECUTION // from "I2cComMasterConf.h"
	if(rtcRV3028_ManualRW.nbTries > 0)
	{
		rtcRV3028_ManualRW.nbTries--;
		I2cSystem_StartReadRTC_Registers(&mRtcRv3028, &rtcRV3028_ManualRW);
	}
  #endif // I2C_ENABLE_DEBUG_MULTI_EXECUTION
#endif // RV3028_RTC_ENABLE_MANUAL_RW
}
#endif

/******************************************************************************/

#ifdef I2CCM_ENABLE_I2C_DEBUG
uint16_t getI2cSystemNbRestart(void)
{
	return (uint16_t)mI2cSystemManager.nbRestart;
}
#endif // I2CCM_ENABLE_I2C_DEBUG

/******************************************************************************/

#ifdef I2CCM_ENABLE_I2C_DEBUG
I2CCM_Manage_Device* getI2cSystemManagedDeviceFromExtPtr(void* pExtStruct)
{
	for(I2CCM_Manage_Device* pManagedDevice = (I2CCM_Manage_Device*)mI2cCmSystemManParams.pDevListBase; pManagedDevice < (I2CCM_Manage_Device*)mI2cCmSystemManParams.pDevListEnd; pManagedDevice++)
	{
		if(pExtStruct == pManagedDevice->Device.pExtData) return pManagedDevice;
	}
	return 0;
}
#endif // I2CCM_ENABLE_I2C_DEBUG

/******************************************************************************/

#ifdef I2CCM_ENABLE_I2C_DEBUG
uint16_t getI2cSystemErrorsCtFromExtPtr(void* pExtStruct)
{
	I2CCM_Manage_Device* pManagedDevice = getI2cSystemManagedDeviceFromExtPtr(pExtStruct);
	if(0 == pManagedDevice) return 0;

	I2CCM_MAKE_TYPED_PTR(uint16_t, pIntData, pManagedDevice->Device.pIntData);
	if(0 == pIntData) return 0;

	return pIntData[2]; // ErrorsCt is generaly the 1st Uint16 after I2CCM_InternalBaseData
}
#endif // I2CCM_ENABLE_I2C_DEBUG

/******************************************************************************/

#ifdef I2CCM_ENABLE_I2C_DEBUG
uint16_t getI2cSystemCo2EE895ErrorsCt(void)
{
	return getI2cSystemErrorsCtFromExtPtr(&mCo2EE895);
}
#endif // I2CCM_ENABLE_I2C_DEBUG

/******************************************************************************/

#ifdef I2CCM_ENABLE_I2C_DEBUG
uint16_t getI2cSystemCo2SCD3xErrorsCt(void)
{
	return getI2cSystemErrorsCtFromExtPtr(&mCo2SCD3);
}
#endif // I2CCM_ENABLE_I2C_DEBUG

