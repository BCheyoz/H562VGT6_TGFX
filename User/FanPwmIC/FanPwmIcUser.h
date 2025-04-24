/*
 * FanPwmIcUser.h
 *
 *  Created on: 16 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 26 Mars 2025
 *  Updated by: m.faget
 *
 *  Pour intégrer facilement cette Librairie "FanPwmIC" dans un nouveau Projet :
 *   -> Suivre les indications dans "FanPwmIcConf.h"
 *
 */

#ifndef FANPWMIC_FANPWMICUSER_H_
#define FANPWMIC_FANPWMICUSER_H_

#include "FanPwmIcUtils.h"			// Pour accès aux Outils d'aide à la Configuration

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************************/
// Fonctions publiques se rapportant au Ventilateur 1 (Exhaust) :

uint16_t getFanExhaustVoltage_mV(void);
void setFanExhaustVoltage_mV(uint16_t newVoltage);
void setFanExhaustVoltage_percent(uint16_t  motorSetPoint);

uint16_t getFanExhaustFeedbackSpeed(void);
uint16_t getFanExhaustLastFeedbackSpeed(void);
uint16_t getFanExhaustLastDeltaTime(void);
uint32_t getFanExhaustLastFrequency(void);

/********************************************************************************************/
// Fonctions publiques se rapportant au Ventilateur 2 (Supply) :

uint16_t getFanSupplyVoltage_mV(void);
void setFanSupplyVoltage_mV(uint16_t newVoltage);

uint16_t getFanSupplyFeedbackSpeed(void);
uint16_t getFanSupplyLastFeedbackSpeed(void);
uint16_t getFanSupplyLastDeltaTime(void);
uint32_t getFanSupplyLastFrequency(void);

/********************************************************************************************/
// Fonctions publiques se rapportant à l'inversion des Ventilateurs 1 & 2 (Exhaust / Supply) :

uint8_t AreFansSwapped_Exhaust_Supply(void);
void ApplyFanSwap_Exhaust_Supply(uint8_t swapAction);

#ifdef __cplusplus
}
#endif

#endif /* FANPWMIC_FANPWMICUSER_H_ */
