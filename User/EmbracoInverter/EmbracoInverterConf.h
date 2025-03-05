/*
 * EmbracoInverterConf.h
 *
 *  Created on: Mar 4, 2025
 *  Original Author: j.proux
 *
 *  Updated on: 4 Mar. 2025
 *  Updated by: j.proux
 *
 *  History Usage :
 *-> 04/03/2025 : Added by Jp	to TFlow4_CarteMere_App (STM32H562VGTX : be-eec/productprojects/confortthermique/chauffe-eau-air/tflow4/tfl4_cartemere_app)
 *
 */

#ifndef EMBRACO_INVERTER_EMBRACO_INVERTER_CONF_H_
#define EMBRACO_INVERTER_EMBRACO_INVERTER_CONF_H_

#include "UartComDepUtils.h"	// Pour accès aux outils de base spécifiques aux dépendants de UartCom

/******************************************************************************/
// Pour "UartComUser.c" (à priori pas besoin de modifier) :

#define NB_EMBRACO_INVERTER_BUF_DEF_SIZE	1	// 1 seul Embraco Inverter, sur USART3

#define EMBRACO_INVERTER_EXT_PARAMS			&EmbracoInverterDatas[0] //0	// Pour le moment, pas de Params pour EmbracoInverter

#define EMBRACO_INVERTER_MIN_RX_SIZE		5	// 5 Bytes en Rx
#define EMBRACO_INVERTER_MIN_TX_SIZE		5	// 5 Bytes en Tx

// EmbracoInverter de Taille par défaut :
#define EMBRACO_INVERTER_BUF_DEF_RX_SIZE	(EMBRACO_INVERTER_MIN_RX_SIZE +1)	// 5 Bytes +1 vide nécessaire en Rx
#define EMBRACO_INVERTER_BUF_DEF_TX_SIZE	EMBRACO_INVERTER_MIN_TX_SIZE		// 5 Bytes uniquement requis en Tx

/******************************************************************************/
// Pour "EmbracoInverter.c" :

//#define MIN_SPEED_START_RPM 	1600	// Default Minimum Speed is 1600 RPM (cf. Table 16 page 37 and Figure 4.2 page 38)
#define MIN_SPEED_START_RPM 	1200	// Custom compressor Model has Minimum Speed of 1200 RPM (cf. note below Table 16 page 37)

//#define EMBRACO_INVERTER_CAN_START_ANY_SPEED
#define EMBRACO_INVERTER_CAN_STOP_ANY_SPEED

#define EMBRACO_INVERTER_GET_LAST_OTHER_DATA	// for EXPLORATION only !

/******************************************************************************/
// Pour "UartComUser.c" (ne pas modifier ces réglages) :

#define EMBRACO_INVERTER_MAX_RX_BLOC_SZ 	1 // ToDo : vérifier déjà avec 1 en RX_IT !EMBRACO_INVERTER_MIN_RX_SIZE		// Taille Maximale à recevoir en une seule fois : Bytes 1 par 1 sur UART

#define EMBRACO_INVERTER_DEF_REPLY_SZ		0 // Master => pas de réponse à retourner ! EMBRACO_INVERTER_BUF_DEF_TX_SIZE // Par défaut : tout l'espace TX disponible
#define EMBRACO_INVERTER_ERR_RX_FRAME		15	// Tempo pour sabEndOfRxFrame en cas d'erreur de Réception (base = IT @ 1ms)
#define EMBRACO_INVERTER_ERR_TX_RETRY		15	// Tempo pour sabMayTxNextByte : Retry en cas d'erreur d'Envoi (base = IT @ 1ms)
#define EMBRACO_INVERTER_ERR_TX_FRAME		20	// Tempo pour sabMayTxNextByte : Silence après erreur d'Envoi (base = IT @ 1ms)

#define EMBRACO_INVERTER_CHECK_ECHO 		0	// S'il faut Contrôler la Réception d'un Echo RxTX
#define EMBRACO_INVERTER_MAY_SYNC_TX 		0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Requête Modbus
#define EMBRACO_INVERTER_MAY_SYNC_REPLY 	0	// S'il faut attendre une Synchro avant de commencer l'envoi d'une Réponse Modbus
#define EMBRACO_INVERTER_ON_TX_STATE		1	// Etat de la Pin GPIOx lors d'une Transmission Active de Trame
#define EMBRACO_INVERTER_INIT_DO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_DO_LOAD_AT_MST, MODBUS_SLAVE_CHECK_ECHO, MODBUS_SLAVE_MAY_SYNC_TX, MODBUS_SLAVE_MAY_SYNC_REPLY, MODBUS_SLAVE_ON_TX_STATE) // Synthèse des Flags UartComInitFlags
#define EMBRACO_INVERTER_INIT_NO_LOAD_FLAGS UART_COM_MAKE_INIT_FLAGS(UART_COM_NO_LOAD_AT_MST, MODBUS_SLAVE_CHECK_ECHO, MODBUS_SLAVE_MAY_SYNC_TX, MODBUS_SLAVE_MAY_SYNC_REPLY, MODBUS_SLAVE_ON_TX_STATE) // Synthèse des Flags UartComInitFlags

#endif /* EMBRACO_INVERTER_EMBRACO_INVERTER_CONF_H_ */
