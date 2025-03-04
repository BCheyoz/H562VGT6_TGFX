/*
 * UartComDepUtils.h
 *
 *  Created on: 7 mai 2021
 *  Original Author: j.proux
 *
 *  Updated on: 21 Feb. 2025
 *  Updated by: j.proux
 *
 *  Remarque_Jp le 19/04/2024 : Ce Fichier ayant été converti en UTF-8 pour GitLab,
 *	-> il te faudra peut-être forcer manuellement l'affichage de cette "Ressource"
 * en "Text file encoding" = "Other: UTF-8" (clic-droit sur le Fichier -> "Properties").
 *
 *  Pour intégrer facilement cette Librairie "UartCom" dans un nouveau Projet :
 *   -> Suivre les indications dans "UartComConf.h"
 *
 */

#ifndef UART_COM_UARTCOMDEPUTILS_H_
#define UART_COM_UARTCOMDEPUTILS_H_

/******************************************************************************/

#define UART_COM_MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define UART_COM_MAX(a, b)	(((a) > (b)) ? (a) : (b))

/******************************************************************************/

#define UART_COM_DO_LOAD_AT_MST					1	// S'il faut charger cet élément à la Mise sous Tension
#define UART_COM_NO_LOAD_AT_MST					0	// S'il ne faut pas charger cet élément à la Mise sous Tension
#define UART_COM_MAKE_INIT_FLAGS(l,e,s,r,p) 	{{ l, e, s, r, p }} // Pour conformité à tUartComInitFlags

/******************************************************************************/

#define UART_COM_MAKE_CONST_END_OF_TABLE(t,n,b)					const t* n = ((void*)b) + (sizeof(b))
#define UART_COM_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(t,b,e,a)	const t* b = a; const t* e = ((void*)a) + (sizeof(a))
#define UART_COM_MAKE_XTERN_CONST_BASE_AND_END_OF_TABLE(t,b,e)	extern const t* b; extern const t* e

/******************************************************************************/

#endif /* UART_COM_UARTCOMDEPUTILS_H_ */
