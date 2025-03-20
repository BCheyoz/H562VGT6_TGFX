#ifndef FIRMWARE_MANAGER_GATEWAY_H
#define FIRMWARE_MANAGER_GATEWAY_H

#include <stdint.h>
#include "GestionInputSensor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GET_SET_ARRAY_PROTOTYPE(a, b, c)	c get##a##b(void); uint8_t getID##a##b(void); void byPass##a##b(c val);

void requestFanVoltage_mV(uint16_t newVoltage);
uint16_t fanVoltage_mV();
uint16_t fanFeedbackSpeed();
uint16_t fanLastFeedbackSpeed();
uint16_t fanLastDeltaTime();
uint8_t fanVoltage_V_x10();

void setAppointEnable(uint8_t enable);
uint8_t isAppointEnable();

// interface modbus pour Gestion
#if NB_PRESSURE_SENSOR_USED > 0
GET_SET_ARRAY_PROTOTYPE(Pressure, 0, int16_t)
#endif

#if NB_COV_SENSOR_USED > 0
GET_SET_ARRAY_PROTOTYPE(Cov, 0, uint16_t)
#endif

#if NB_CO2_SENSOR_USED > 0
GET_SET_ARRAY_PROTOTYPE(Co2, 0, uint16_t)
#endif

#if NB_HR_TEMP_SENSOR_USED > 0

GET_SET_ARRAY_PROTOTYPE(Hr, 0, uint16_t)
GET_SET_ARRAY_PROTOTYPE(Temp, 0, int16_t)
#endif

#if NB_CTN_TT4_10KC3_USE > 0
GET_SET_ARRAY_PROTOTYPE(Ctn, 0, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 1, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 2, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 3, int16_t)
GET_SET_ARRAY_PROTOTYPE(Ctn, 4, int16_t)
#endif

#ifdef __cplusplus
}
#endif

#endif // FIRMWARE_MANAGER_GATEWAY_H
