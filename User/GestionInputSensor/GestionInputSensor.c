/*
 * GestionInputSensor.c
 *
 *  Created on: 14 nov. 2022
 *  Original Author: j.proux
 *
 */

#include "GestionInputSensor.h"
#include "I2cComMasterSystem.h"

/*** private vars *******************************/
#define CO2_ID_BYPASS_CODE 44
static uint8_t co2Id         = NO_SENSOR;
static uint16_t co2BypassValue = 0;
// Add above this line others private vars

/*** private functions *******************************/

static uint8_t isI2cDeviceLoaded(void* pI2cDevice, uint8_t CustomDevId, uint8_t* pCustomDevId);
// Add above this line others private functions

/*** public functions *******************************/

void InitInputSensor()
{
	// nothing to Do !
}

void GestionInputSensor()
{
	// nothing to Do !
}

// Add above this line others public functions

/*** Synthetic fuctions *******************************/

uint16_t getCo2(){
	uint16_t currentCo2;

    if(co2Id != SENSOR_BENCHTEST){
        co2Id = NO_SENSOR;
        // Recherche du Device actuellement chargé, par ordre de probabilité :
        if(NO_SENSOR == isI2cDeviceLoaded(&mCo2SCD3, SENSOR_SCD30, &co2Id))
            if(NO_SENSOR == isI2cDeviceLoaded(&mCo2EE895, SENSOR_EE895, &co2Id))
                co2Id = NO_SENSOR;
    }

	// Récupération de la valeur à la bonne source :
	switch(co2Id)
	{
	case SENSOR_SCD30:
		currentCo2 = (uint16_t)(mCo2SCD3.Co2);	// Remarque_Jp le 14/11/222 : le Float est "tronqué" au lieu d'être arrondi.
		break;
	case SENSOR_EE895:
		currentCo2 = (uint16_t)(mCo2EE895.Co2Value);
		break;
	case SENSOR_BENCHTEST:
        currentCo2 = co2BypassValue;
        break;
	default:
		currentCo2 = (uint16_t)UINT16_MAX;
		break;
	}

	return currentCo2;
}

// Add above this line others public synthetic functions

/*** locale functions *******************************/

uint8_t isI2cDeviceLoaded(void* pI2cDevice, uint8_t CustomDevId, uint8_t* pCustomDevId)
{
// à priori, déjà fait par la fonction appelante :	*pCustomDevId = NO_SENSOR;
	if(0 == pI2cDevice) return NO_SENSOR;

	// La base du Device I2C pointe sur la structure par défaut I2CCM_ExternalBaseData :
	I2CCM_MAKE_TYPED_PTR(I2CCM_ExternalBaseData, pI2cDevExtBaseData, pI2cDevice);

	// Vérifie si le Device est (ou non) actuellement chargé :
	if(0 == pI2cDevExtBaseData->idOfDevice) return NO_SENSOR;	// le Device n'est pas actuellement chargé.

	// OK, le Device est bien actuellement chargé :
	if(0 == (pI2cDevExtBaseData->idOfDevice & I2CCM_DEVICE_RECENTLY_LOADED_MSK)) // Vérifie si InitDevice Terminé :
	{
		*pCustomDevId = CustomDevId;	// OK, le Device est opérationnel :-) ...
	} else
	{
		*pCustomDevId = SENSOR_INIT_PENDING; // le Device est toujours en INIT !
	}

	return CustomDevId;
}

// Add above this line others private locale functions

/*** public Getters *******************************/

uint8_t getCo2Id(){ return co2Id; }

/*** public Setter for benchtest *******************************/
void byPassCo2Value(uint16_t val){
    co2BypassValue = val;
}

void byPassCo2Id(uint16_t code){
    if(code == CO2_ID_BYPASS_CODE) {
        co2Id = SENSOR_BENCHTEST;
    }
    else {
        co2Id = NO_SENSOR; // la fonction getCo2 cherche automatiquement le capteur
    }

}

// Add above this line others public Getters
