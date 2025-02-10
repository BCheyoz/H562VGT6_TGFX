#ifndef FIRMWARE_MANAGER_GATEWAY_H
#define FIRMWARE_MANAGER_GATEWAY_H

#include <stdint.h>


void requestFanVoltage_mV(uint16_t newVoltage);
uint16_t fanVoltage_mV();
uint16_t fanFeedbackSpeed();
uint16_t fanLastFeedbackSpeed();
uint16_t fanLastDeltaTime();
uint8_t fanVoltage_V_x10();

#endif // FIRMWARE_MANAGER_GATEWAY_H
