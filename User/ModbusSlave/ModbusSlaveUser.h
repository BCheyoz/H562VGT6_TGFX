/*
 * ModbusSlaveUser.h
 *
 *  Created on: 27 avr. 2021
 *  Original Author: j.proux
 *
 *  Updated on: 27 Feb. 2025
 *  Updated by: j.proux
 *
 * Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 * -> il faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "ModbusSlave" dans un nouveau Projet :
 *   -> Suivre les indications dans "ModbusSlaveConf.h"
 *
 */

#ifndef MODBUS_SLAVE_MODBUS_SLAVE_USER_H_
#define MODBUS_SLAVE_MODBUS_SLAVE_USER_H_

#include "ModbusSlaveCore.h"	// Pour accès aux Fonctions, Config & Structures du Core

/******************************************************************************/
// Affectation des Paramètres de Gestion des différentes sources de ModbusSlave :

/* USER CODE BEGIN ManageParams */

//#define MODBUS_SLAVE_UART_IHM_PARAMS	&ModbusSlaveParam[0]
#define MODBUS_SLAVE_UART_USER_PARAMS	&ModbusSlaveParam[0]
//#define MODBUS_SLAVE_UART_EXT_PARAMS	&ModbusSlaveParam[2]
//#define MODBUS_SLAVE_USBD_CDC_PARAMS	&ModbusSlaveParam[3]
//#define MODBUS_SLAVE_USBH_CDC_PARAMS	&ModbusSlaveParam[3]	// UsbHost_ModbusSlave partage les même Params que UsbDevice_ModbusSlave

/* USER CODE END ManageParams */

/******************************************************************************/
// Prototypes des Fonctions Publiques de "ModbusSlaveUser.c" :

void ModbusSlaveInitUserMST(void);

void initModbusUserParamsFromMemHisto(void);
void checkModbusParamsFromMemHisto(uint16_t newSlaveAdr);
void SetModbusUserSlaveAdr(uint16_t newSlaveAdr);

void SaveParamsOnOrder(uint16_t Order2Save);
void handleQueryReInitFactory(uint16_t pswd);

#define OBSCURE_U16(a)	( (( ((a^((a&0xF)<<12))&0xF000) | ((~(a^((a&0xF)<<8)))&0x0F00))>>8) | (( ((a^((a&0xF)<<4))&0xF0) | ((~a)&0xF))<<8) )	// 0xabcd -> (c^d)(~d)(a^d)~(b^d)
#define UNOBSCR_U16(a)	( (( ((a^(((~a)&0xF00)<<4))&0xF000) | ((~a)&0xF00))>>8) | (( ((a^(((~a)&0xF00)>>4))&0xF0) | ((~(a^(((~a)&0xF00)>>8)))&0xF))<<8) )	// 0xcdab -> (a^(~d))~(b^(~d))(c^(~d))(~d)

#endif /* MODBUS_SLAVE_MODBUS_SLAVE_USER_H_ */
