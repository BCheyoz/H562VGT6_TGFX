/*
 * GestionInputSensor.c
 *
 *  Created on: 14 nov. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 28 Feb. 2025
 *  Updated by: b.chhay
 *
 *  Version 1.1
 */

#include "GestionInputSensor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void *pDevice;
	E_SENSOR_ID id;
	uint8_t isTake;
} gis_device;

typedef struct {
	uint8_t id;
	uint8_t saveId;
	uint16_t byPasseTimer;
	uint8_t deviceIdx;
} gis_BaseValue;

typedef struct {
	gis_BaseValue base;
	int16_t value;
} gis_IntValue;

typedef struct {
	gis_BaseValue base;
	uint16_t value;
} gis_UIntValue;


/*** private vars *******************************/
static uint8_t gis_timer_100ms = 0;
static uint16_t bypassDuration = 30 * 60;   // 30min par default

#if NB_PRESSURE_SENSOR_USED > 0
static gis_IntValue Pressure[NB_PRESSURE_SENSOR_USED];
#define I2C_NB_PRESS_DEVICE 4

static gis_device i2cDevicePressure[I2C_NB_PRESS_DEVICE] = {
		{&mPresHSC, SENSOR_HSC, 0},
		{&mPresABP2, SENSOR_ABP2, 0},
		//{&mPresSDP6, SENSOR_SDP6, 0},
		{&mPresSDP8, SENSOR_SDP8, 0},
		{&mPresLMI, SENSOR_LMI, 0},
};
#endif

#if NB_COV_SENSOR_USED > 0
static gis_UIntValue Cov[NB_COV_SENSOR_USED];
#define I2C_NB_COV_DEVICE 2

static gis_device i2cDeviceCov[I2C_NB_COV_DEVICE] = {
		{&mCovSGP40, SENSOR_SGP40, 0},
		{&mCovENS160, SENSOR_ENS160, 0},
};
#endif

#if NB_CO2_SENSOR_USED > 0
static gis_UIntValue Co2[NB_CO2_SENSOR_USED];
#define I2C_NB_CO2_DEVICE 3

static gis_device i2cDeviceCo2[I2C_NB_CO2_DEVICE] = {
		{&mCo2SCD3, SENSOR_SCD3x, 0},
		{&mCo2SCD4, SENSOR_SCD4x, 0},
		{&mCo2EE895, SENSOR_EE895, 0},
};
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
static gis_UIntValue Hr[NB_HR_TEMP_SENSOR_USED];
static gis_IntValue Temp[NB_HR_TEMP_SENSOR_USED];
#define I2C_NB_HR_TEMP_DEVICE 2

static gis_device i2cDeviceHr[I2C_NB_HR_TEMP_DEVICE] = {
		{&mHrSHT3, SENSOR_SHT3x, 0},
		{&mHrSHT4, SENSOR_SHT4x, 0},
};

static gis_device i2cDeviceTemp[I2C_NB_HR_TEMP_DEVICE] = {
		{&mHrSHT3, SENSOR_SHT3x, 0},
		{&mHrSHT4, SENSOR_SHT4x, 0},
};
#endif

#if NB_CTN_TT4_10KC3_USE > 0
static gis_IntValue Ctn[NB_CTN_TT4_10KC3_USE];
#define ADC_NB_CTN_DEVICE 5

static gis_device i2cDeviceCtn[ADC_NB_CTN_DEVICE] = {
		{&tAi_CTN[0], SENSOR_CTN_TT4_10KC3, 0},
		{&tAi_CTN[1], SENSOR_CTN_TT4_10KC3, 0},
		{&tAi_CTN[2], SENSOR_CTN_TT4_10KC3, 0},
		{&tAi_CTN[3], SENSOR_CTN_TT4_10KC3, 0},
		{&tAi_CTN[4], SENSOR_CTN_TT4_10KC3, 0},
};
#endif
// Add above this line others private vars

/*** private prototype functions *******************************/
static uint8_t isI2cDeviceLoaded(void* pDevice, uint8_t CustomDevId);
uint8_t updateI2cDeviceId(gis_BaseValue *pData, gis_device *pDevice, uint8_t nbDevice);

void updateTempValueInt(gis_IntValue *pData, gis_device *pDevice, uint8_t nbDevice);
void updatePressureValueInt(gis_IntValue *pData, gis_device *pDevice, uint8_t nbDevice);
void updateHrValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice);
void updateCOVValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice);
void updateCO2ValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice);

#define GET_SET_ARRAY_DEFINITION_UINT16(a, b)	uint16_t get##a(uint8_t idx){ \
												if(idx > b-1) return UINT16_MAX; \
												return a[idx].value; \
											} \
											uint8_t getID##a(uint8_t idx){ \
												if(idx > b-1) return NO_SENSOR; \
												return a[idx].base.id; \
											} \
											void byPass##a(uint8_t idx, uint16_t val){ \
												if(idx > b-1) return ; \
												a[idx].base.id = SENSOR_BENCHTEST; a[idx].value = val; a[idx].base.byPasseTimer = bypassDuration; \
											}

#define GET_SET_ARRAY_DEFINITION_INT16(a, b)	int16_t get##a(uint8_t idx){ \
												if(idx > b-1) return INT16_MIN; \
												return a[idx].value; \
											} \
											uint8_t getID##a(uint8_t idx){ \
												if(idx > b-1) return NO_SENSOR; \
												return a[idx].base.id; \
											} \
											void byPass##a(uint8_t idx, int16_t val){ \
												if(idx > b-1) return ; \
												a[idx].base.id = SENSOR_BENCHTEST; a[idx].value = val; a[idx].base.byPasseTimer = bypassDuration; \
											}
// Add above this line others private prototype functions

/*** public functions *******************************/
void Handle_InputSensor_RT_100ms(){
	gis_timer_100ms++;
}

void InitInputSensor()
{
	gis_timer_100ms = 0;

#if NB_PRESSURE_SENSOR_USED > 0
	for(uint8_t i = 0; i < NB_PRESSURE_SENSOR_USED; i++){
		Pressure[i].base.byPasseTimer = 0;
		Pressure[i].base.id = NO_SENSOR;
		Pressure[i].base.saveId = NO_SENSOR;
		Pressure[i].value = 0;
	}
#endif

#if NB_COV_SENSOR_USED > 0
	for(uint8_t i = 0; i < NB_COV_SENSOR_USED; i++){
		Cov[i].base.byPasseTimer = 0;
		Cov[i].base.id = NO_SENSOR;
		Cov[i].base.saveId = NO_SENSOR;
		Cov[i].value = 0;
	}
#endif

#if NB_CO2_SENSOR_USED > 0
	for(uint8_t i = 0; i < NB_CO2_SENSOR_USED; i++){
		Co2[i].base.byPasseTimer = 0;
		Co2[i].base.id = NO_SENSOR;
		Co2[i].base.saveId = NO_SENSOR;
		Co2[i].value = 0;
	}
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
	for(uint8_t i = 0; i < NB_HR_TEMP_SENSOR_USED; i++){
		Hr[i].base.byPasseTimer = 0;
		Hr[i].base.id = NO_SENSOR;
		Hr[i].base.saveId = NO_SENSOR;
		Hr[i].value = 0;
		Temp[i].byPasseTimer = 0;
		Temp[i].id = NO_SENSOR;
		Temp[i].saveId = NO_SENSOR;
		Temp[i].value = 0;
	}
#endif

#if NB_CTN_TT4_10KC3_USE > 0
	for(uint8_t i = 0; i < NB_CTN_TT4_10KC3_USE; i++){
		Ctn[i].base.byPasseTimer = 0;
		Ctn[i].base.id = NO_SENSOR;
		Ctn[i].base.saveId = NO_SENSOR;
		Ctn[i].value = 0;
	}
#endif
}

void GestionInputSensor()
{
	if(gis_timer_100ms < 2) return;
	gis_timer_100ms = 0;

#if NB_PRESSURE_SENSOR_USED > 0
	for(uint8_t idx = 0; idx < NB_PRESSURE_SENSOR_USED; idx++){
		updatePressureValueInt(&(Pressure[idx]), i2cDevicePressure, I2C_NB_PRESS_DEVICE);
	}
#endif

#if NB_COV_SENSOR_USED > 0
	for(uint8_t idx = 0; idx < NB_COV_SENSOR_USED; idx++){
		updateCOVValueUint(&(Cov[idx]), i2cDeviceCov, I2C_NB_COV_DEVICE);
	}
#endif

#if NB_CO2_SENSOR_USED > 0
	for(uint8_t idx = 0; idx < NB_CO2_SENSOR_USED; idx++){
		updateCO2ValueUint(&(Co2[idx]), i2cDeviceCo2, I2C_NB_CO2_DEVICE);
	}
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
	for(uint8_t idx = 0; idx < NB_HR_TEMP_SENSOR_USED; idx++){
		updateHrValueUint(&(Hr[idx]), i2cDeviceHr, I2C_NB_HR_TEMP_DEVICE);
		updateTempValueInt(&(Temp[idx]), i2cDeviceTemp, I2C_NB_HR_TEMP_DEVICE);
	}
#endif

#if NB_CTN_TT4_10KC3_USE > 0
	for(uint8_t idx = 0; idx < NB_CTN_TT4_10KC3_USE; idx++){
		updateTempValueInt(&(Ctn[idx]), i2cDeviceCtn, ADC_NB_CTN_DEVICE);
	}
#endif

}


uint16_t getBypassDuration(){
	return bypassDuration;
}
void setBypassDuration(uint16_t duration){
	bypassDuration = duration;
}

#if NB_PRESSURE_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION_INT16(Pressure, NB_PRESSURE_SENSOR_USED)
#endif

#if NB_COV_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION_UINT16(Cov, NB_COV_SENSOR_USED)
#endif

#if NB_CO2_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION_UINT16(Co2, NB_CO2_SENSOR_USED)
#endif

#if NB_HR_TEMP_SENSOR_USED > 0
GET_SET_ARRAY_DEFINITION_UINT16(Hr, NB_HR_TEMP_SENSOR_USED)
GET_SET_ARRAY_DEFINITION_INT16(Temp, NB_HR_TEMP_SENSOR_USED)
#endif

#if NB_CTN_TT4_10KC3_USE > 0
GET_SET_ARRAY_DEFINITION_INT16(Ctn, NB_CTN_TT4_10KC3_USE)
#endif

// Add above this line others public functions

/*** private functions *******************************/

uint8_t isI2cDeviceLoaded(void* pDevice, uint8_t CustomDevId)
{
	if(0 == pDevice) return NO_SENSOR;

	// La base du Device I2C pointe sur la structure par défaut I2CCM_ExternalBaseData :
	I2CCM_MAKE_TYPED_PTR(I2CCM_ExternalBaseData, pI2cDevExtBaseData, pDevice);

	// Vérifie si le Device est (ou non) actuellement chargé :
	if(0 == pI2cDevExtBaseData->idOfDevice) return NO_SENSOR;	// le Device n'est pas actuellement chargé.

	// OK, le Device est bien actuellement chargé :
	uint8_t id = SENSOR_INIT_PENDING;// le Device doit être en INIT !
	if(0 == (pI2cDevExtBaseData->idOfDevice & I2CCM_DEVICE_RECENTLY_LOADED_MSK)) // Vérifie si InitDevice Terminé :
	{
		id = CustomDevId;	// OK, le Device est opérationnel :-) ...
	}

	return id;
}

uint8_t updateI2cDeviceId(gis_BaseValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = 0;
	if(pData->id == SENSOR_BENCHTEST){
		pData->byPasseTimer--;
	}

	if(pData->byPasseTimer == 0 && pData->id == SENSOR_BENCHTEST){
		/* permet de lancer la recherche de capteur s'il etait en NO_SENSOR
		 * sinon reprend le capteur déjà selectioné */
		pData->id = pData->saveId;
	}

	while(pData->id == NO_SENSOR && deviceIdx < nbDevice){
		// Recherche du Device actuellement chargé, par ordre de probabilité :

		if (pDevice[deviceIdx].isTake == 1) {
			deviceIdx++;
			continue;
		}

		uint8_t id = isI2cDeviceLoaded(pDevice[deviceIdx].pDevice, pDevice[deviceIdx].id);

		if (id <= SENSOR_INIT_PENDING) { // on attent que le capteur ai fini de s'initialiser pour le selectionner
			deviceIdx++; // check next
		}
		else {
			pDevice[deviceIdx].isTake = 1; // signal que ce device est géré
			pData->deviceIdx = deviceIdx; // conserve l'adresse
			pData->saveId = id; // conserve l'ID capteur en cas de bypass
			pData->id = id;
		}
	}

	if(pData->id > SENSOR_INIT_PENDING){ // les enums au dessus
		deviceIdx = pData->deviceIdx;
	}

	return deviceIdx;
}

void updateTempValueInt(gis_IntValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = updateI2cDeviceId(&(pData->base), pDevice, nbDevice);
	// Récupération de la valeur à la bonne source :
	switch(pData->base.id)
	{
#if I2CCM_NB_MAX_DEV_PRESS_HSC
	case SENSOR_HSC:
		I2CCM_Pres_HSC_ExtData *pHSC = (I2CCM_Pres_HSC_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pHSC->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_SDP6
	case SENSOR_SDP6:
		I2CCM_Pres_SDP6_ExtData *pSDP6 = (I2CCM_Pres_SDP6_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pSDP6->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_SDP8
	case SENSOR_SDP8:
		I2CCM_Pres_SDP8_ExtData *pSDP8 = (I2CCM_Pres_SDP8_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pSDP8->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_LMI
	case SENSOR_LMI:
		I2CCM_Pres_LMI_ExtData *pLMI = (I2CCM_Pres_LMI_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pLMI->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_ABP2
	case SENSOR_ABP2:
		I2CCM_Pres_ABP2_ExtData *pABP2 = (I2CCM_Pres_ABP2_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pABP2->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_HR_SHT3x
	case SENSOR_SHT3x:
		I2CCM_Hr_SHT3x_ExtData *pSHT3x = (I2CCM_Hr_SHT3x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pSHT3x->Temperature * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_HR_SHT4x
	case SENSOR_SHT4x:
		I2CCM_Hr_SHT4x_ExtData *pSHT4x = (I2CCM_Hr_SHT4x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t(pSHT4x->Temperature * 10);
		break;
#endif
#if NB_CTN_TT4_10KC3_USE
	case SENSOR_CTN_TT4_10KC3:
		tAI_IntValue *pTT4_10KC3 = (tAI_IntValue*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)pTT4_10KC3->value;
		break;
#endif
	case SENSOR_BENCHTEST:
		// ne rien faire
		break;
	default:
		pData->value = (int16_t)INT16_MIN;
		break;
	}
}

void updatePressureValueInt(gis_IntValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = updateI2cDeviceId(&(pData->base), pDevice, nbDevice);
	// Récupération de la valeur à la bonne source :
	switch(pData->base.id)
	{
#if I2CCM_NB_MAX_DEV_PRESS_HSC
	case SENSOR_HSC:
		I2CCM_Pres_HSC_ExtData *pHSC = (I2CCM_Pres_HSC_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pHSC->Pressure * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_SDP6
	case SENSOR_SDP6:
		I2CCM_Pres_SDP6_ExtData *pSDP6 = (I2CCM_Pres_SDP6_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pSDP6->Pressure * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_SDP8
	case SENSOR_SDP8:
		I2CCM_Pres_SDP8_ExtData *pSDP8 = (I2CCM_Pres_SDP8_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pSDP8->Pressure * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_LMI
	case SENSOR_LMI:
		I2CCM_Pres_LMI_ExtData *pLMI = (I2CCM_Pres_LMI_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pLMI->Pressure * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_PRESS_ABP2
	case SENSOR_ABP2:
		I2CCM_Pres_ABP2_ExtData *pABP2 = (I2CCM_Pres_ABP2_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (int16_t)(pABP2->Pressure * 10);
		break;
#endif
	case SENSOR_BENCHTEST:
		// ne rien faire
		break;
	default:
		pData->value = INT16_MIN;
		break;
	}
}

void updateHrValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = updateI2cDeviceId(&(pData->base), pDevice, nbDevice);
	// Récupération de la valeur à la bonne source :
	switch(pData->base.id)
	{
#if I2CCM_NB_MAX_DEV_HR_SHT3x
	case SENSOR_SHT3x:
		I2CCM_Hr_SHT3x_ExtData *pSHT3x = (I2CCM_Hr_SHT3x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)(pSHT3x->HygroRelative * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_HR_SHT4x
	case SENSOR_SHT4x:
		I2CCM_Hr_SHT4x_ExtData *pSHT4x = (I2CCM_Hr_SHT4x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t(pSHT4x->HygroRelative * 10);
		break;
#endif
#if I2CCM_NB_MAX_DEV_HR_HTU31
	case SENSOR_HTU31: // TODO
		//I2CCM_Co2_EE895_ExtData *p = (I2CCM_Co2_EE895_ExtData*)pDevice[deviceIdx].pDevice;
		//pData->value = (uint16_t)p->Co2Value;
		break;
#endif
	case SENSOR_BENCHTEST:
		// ne rien faire
		break;
	default:
		pData->value = UINT16_MAX;
		break;
	}
}

void updateCOVValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = updateI2cDeviceId(&(pData->base), pDevice, nbDevice);
	// Récupération de la valeur à la bonne source :
	switch(pData->base.id)
	{
#if I2CCM_NB_MAX_DEV_COV_CCS811
	case SENSOR_CCS811:  // TODO
		//I2CCM_Co2_EE895_ExtData *pCCS811 = (I2CCM_Co2_EE895_ExtData*)pDevice[deviceIdx].pDevice;
		//pData->value = (uint16_t)pCCS811->Cov;
		break;
#endif
#if I2CCM_NB_MAX_DEV_COV_SGP40
	case SENSOR_SGP40:
		I2CCM_COV_SGP40_ExtData *pSGP40 = (I2CCM_COV_SGP40_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pSGP40->VOC_Index;
		break;
#endif
#if I2CCM_NB_MAX_DEV_COV_ENS160
	case SENSOR_ENS160:
		I2CCM_COV_ENS160_ExtData *pENS160 = (I2CCM_COV_ENS160_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pENS160->Data_TVOC;
		break;
#endif
	case SENSOR_BENCHTEST:
		// ne rien faire
		break;
	default:
		pData->value = UINT16_MAX;
		break;
	}
}

void updateCO2ValueUint(gis_UIntValue *pData, gis_device *pDevice, uint8_t nbDevice){
	uint8_t deviceIdx = updateI2cDeviceId(&(pData->base), pDevice, nbDevice);
	// Récupération de la valeur à la bonne source :
	switch(pData->base.id)
	{
#if I2CCM_NB_MAX_DEV_CO2_EE894
	case SENSOR_EE894:
		I2CCM_Co2_EE894_ExtData *pEE894 = (I2CCM_Co2_EE894_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pEE894->Co2Value;
		break;
#endif
#if I2CCM_NB_MAX_DEV_CO2_EE895
	case SENSOR_EE895:
		I2CCM_Co2_EE895_ExtData *pEE895 = (I2CCM_Co2_EE895_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pEE895->Co2Value;
		break;
#endif
#if I2CCM_NB_MAX_DEV_CO2_SCD3x
	case SENSOR_SCD3x:
		I2CCM_Co2_SCD3x_ExtData *pSCD3x = (I2CCM_Co2_SCD3x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pSCD3x->Co2;	// Info le Float est volontairement "tronqué"
		break;
#endif
#if I2CCM_NB_MAX_DEV_CO2_SCD4x
	case SENSOR_SCD4x:
		I2CCM_Co2_SCD4x_ExtData *pSCD4x = (I2CCM_Co2_SCD4x_ExtData*)pDevice[deviceIdx].pDevice;
		pData->value = (uint16_t)pSCD4x->Co2;
		break;
#endif
	case SENSOR_BENCHTEST:
		// ne rien faire
		break;
	default:
		pData->value = UINT16_MAX;
		break;
	}
}

// Add above this line others private locale functions

#ifdef __cplusplus
}
#endif
