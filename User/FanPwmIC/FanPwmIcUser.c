/*
 * FanPwmIcUser.c
 *
 *  Created on: 16 févr. 2022
 *  Original Author: j.proux
 *
 *  Updated on: 26 Mars 2025
 *  Updated by: m.faget
 *
 *  Version 1.0
 */
#include "tim.h"			// Pour accès aux Variables & Fonctions des Timers
#include "FanPwmIcUser.h"	// Pour accès à la Configuration & Déclarations User

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#if defined(FPIC_NB_FANS_WITH_FEEDBACK) && (FPIC_NB_FANS_WITH_FEEDBACK > 0)
	tFanPwmIc mFanData[FPIC_NB_FANS_WITH_FEEDBACK] = {0};
#endif // FPIC_NB_FANS_WITH_FEEDBACK

#if defined(FPIC_NB_FANS_ONLY_COMMAND) && (FPIC_NB_FANS_ONLY_COMMAND > 0)
	tFanPwmData mFanCmdOnlyData[FPIC_NB_FANS_ONLY_COMMAND] = {0};
#endif // FPIC_NB_FANS_ONLY_COMMAND

#if defined(FPIC_NB_FANS_ONLY_FEEDBACK) && (FPIC_NB_FANS_ONLY_FEEDBACK > 0)
	tFanIcData mFanIcOnlyData[FPIC_NB_FANS_ONLY_FEEDBACK] = {0};
#endif // FPIC_NB_FANS_ONLY_FEEDBACK

#if defined(FPIC_NB_TOTAL_INPUT_CAPTURES) && (FPIC_NB_TOTAL_INPUT_CAPTURES > 0)
	tFanIcSrcDatas mFanIcDatas[FPIC_NB_TOTAL_INPUT_CAPTURES] = {0};
#endif // FPIC_NB_TOTAL_INPUT_CAPTURES

/******************************************************************************/
// Définitions pour la Gestion du Ventilateur n°1 (TIM4_CH3 avec FeedBack sur TIM1_CH2) :

#define FAN1_DATA_POS			0				// Position 0 dans mFanData (PWM & IC)

#define FAN1_PWM_HANDLE 		&htim4
#define FAN1_PWM_CHANNEL_ID 	TIM_CHANNEL_3	// TIM4_CH3 sur APB1
#define FAN1_PWM_DATAS  		&mFanData[FAN1_DATA_POS].PwmDatas
//#define FAN1_PWM_DATAS  		&mFanCmdOnlyData[FAN1_DATA_POS]	// suffisant en cas de PWM sans FeedBack
#define FAN1_PWM_K_FACTOR		0.990099		// Correspondance User 1mv => PWM = 1
#define FAN1_PWM_MAX_VALUE  	10000*FAN1_PWM_K_FACTOR		// Max = 10.0V

#define FAN1_IC_SRC_DATA_POS	0				// Position 0 dans mFanIcDatas
#define FAN1_IC_HANDLE  		&htim1
#define FAN1_IC_CHANNEL_ID  	TIM_CHANNEL_2	// TIM1_CH2
#define FAN1_IC_CHANNEL_FLAG	FPIC_MAKE_CHANNEL_FLAG(FAN1_IC_CHANNEL_ID) // <=> HAL_TIM_ACTIVE_CHANNEL_1 // Equivalent (1 << (ChannelId >> 2))
#define FAN1_IC_SRC_DATAS		&mFanIcDatas[FAN1_IC_SRC_DATA_POS]
#define FAN1_IC_GET_DATAS		&mFanData[FAN1_DATA_POS].IcDatas

#define FAN1_IC_TIME_OUT		FPIC_MAKE_TIME_OUT_ms(1100)	// TimeOut = 1.1s (pour RPM > 55)
#define FAN1_IC_FAPB			FPIC_MAKE_FAPB_MHz(240)	// Tim1 sur APB1 @ 240MHz
#define FAN1_IC_PSC 			4028	// Valeur du Prescaler PSC de CubeMx (= htim#.Init.Prescaler dans "tim.c")
#define FAN1_IC_PPT 			1	// Nb of "Pulse Per Turn" (PPT) from the fan feedback, generally only 1
#define FAN1_IC_K_UNIT  		60	// Coefficient de FeedBack, pour convertir l'unité Hz -> RPM
#define FAN1_IC_K_FEED_BACK 	FPIC_MAKE_K_FEED_BACK(FAN1_IC_FAPB, FAN1_IC_PSC, FAN1_IC_PPT, FAN1_IC_K_UNIT)
#define FAN1_IC_K_FREQ			FPIC_MAKE_K_FREQ(FAN1_IC_FAPB, FAN1_IC_PSC, FAN1_IC_PPT)// pour avoir la fréquence du signal d'entrée
/******************************************************************************/
// Définitions pour la Gestion du Ventilateur n°2 (TIM9_CH2 avec FeedBack sur TIM12_CH2) :

//#define FAN2_DATA_POS			1				// Position 1 dans mFanData (PWM & IC)

//#define FAN2_PWM_HANDLE 		&htim9
//#define FAN2_PWM_CHANNEL_ID 	TIM_CHANNEL_2	// TIM9_CH2
//#define FAN2_PWM_DATAS  		&mFanData[FAN2_DATA_POS].PwmDatas
//#define FAN2_PWM_K_FACTOR		1				// Correspondance User 1mv => PWM = 1
//#define FAN2_PWM_MAX_VALUE  	10000			// Max = 10.0V

//#define FAN2_IC_SRC_DATA_POS	1				// Position 1 dans mFanIcDatas
//#define FAN2_IC_HANDLE  		&htim12
//#define FAN2_IC_CHANNEL_ID  	TIM_CHANNEL_2	// TIM12_CH2
//#define FAN2_IC_CHANNEL_FLAG	FPIC_MAKE_CHANNEL_FLAG(FAN2_IC_CHANNEL_ID) // <=> HAL_TIM_ACTIVE_CHANNEL_2 // Equivalent (1 << (ChannelId >> 2))
//#define FAN2_IC_SRC_DATAS		&mFanIcDatas[FAN2_IC_SRC_DATA_POS]
//#define FAN2_IC_GET_DATAS		&mFanData[FAN2_DATA_POS].IcDatas

//#define FAN2_IC_TIME_OUT		FPIC_MAKE_TIME_OUT_ms(1100)	// TimeOut = 1.1s (pour RPM > 55)
//#define FAN2_IC_FAPB			FPIC_MAKE_FAPB_MHz(60)	// Tim12 sur APB1 @ 60MHz
//#define FAN2_IC_PSC 			1007	// Valeur du Prescaler PSC de CubeMx (= htim#.Init.Prescaler dans "tim.c")
//#define FAN2_IC_PPT 			1	// Nb of "Pulse Per Turn" (PPT) from the fan feedback, generally only 1
//#define FAN2_IC_K_UNIT  		60	// Coefficient de FeedBack, pour convertir l'unité Hz -> RPM
//#define FAN2_IC_K_FEED_BACK 	FPIC_MAKE_K_FEED_BACK(FAN2_IC_FAPB, FAN2_IC_PSC, FAN2_IC_PPT, FAN2_IC_K_UNIT)
//#define FAN2_IC_K_FREQ		FPIC_MAKE_K_FREQ(FAN2_IC_FAPB, FAN2_IC_PSC, FAN2_IC_PPT)// pour avoir la fréquence du signal d'entrée

/******************************************************************************/
// Définitions pour la Gestion du Ventilateur n°3 (TIM8_CH6 sans FeedBack) :

//#define FAN3_DATA_POS			0				// Position 0 dans mFanCmdOnlyData (only PWM)

//#define FAN3_PWM_HANDLE 		&htim8
//#define FAN3_PWM_CHANNEL_ID 	TIM_CHANNEL_6	// TIM8_CH6
//#define FAN3_PWM_DATAS  		&mFanCmdOnlyData[FAN3_DATA_POS].PwmDatas
//#define FAN3_PWM_K_FACTOR		1				// Correspondance User 1mv => PWM = 1
//#define FAN3_PWM_MAX_VALUE  	10000			// Max = 10.0V

//#define FAN3_IC_SRC_DATA_POS	0				// no Feedback !
//#define FAN3_IC_HANDLE  		0				// no Feedback !
//#define FAN3_IC_CHANNEL_ID  	0				// no Feedback !
//#define FAN3_IC_CHANNEL_FLAG	0				// no Feedback !
//#define FAN3_IC_SRC_DATAS		0				// no Feedback !
//#define FAN3_IC_GET_DATAS		0				// no Feedback !

//#define FAN3_IC_TIME_OUT		FPIC_MAKE_TIME_OUT_ms(100)	// TimeOut = 100ms
//#define FAN3_IC_K_FEED_BACK 	0				// no Feedback !
//#define FAN3_IC_K_FREQ		0 				// no Freq !

/******************************************************************************/
// Définitions pour la Gestion du Ventilateur n°4 (Pas de PWM mais FeedBack sur TIM5_CH1) :

//#define FAN4_DATA_POS			0				// Position 0 dans mFanIcOnlyData (only InputCapture)

//#define FAN4_PWM_HANDLE  		0				// no PWM !
//#define FAN4_PWM_CHANNEL_ID 	0				// no PWM !
//#define FAN4_PWM_DATAS  		0				// no PWM !
//#define FAN4_PWM_K_FACTOR		1				// Correspondance User 1mv => PWM = 1
//#define FAN4_PWM_MAX_VALUE  	10000			// Max = 10.0V

//#define FAN4_IC_SRC_DATA_POS	2				// Position 2 dans mFanIcDatas
//#define FAN4_IC_HANDLE  		&htim5
//#define FAN4_IC_CHANNEL_ID  	TIM_CHANNEL_1	// TIM5_CH1
//#define FAN4_IC_CHANNEL_FLAG	FPIC_MAKE_CHANNEL_FLAG(FAN4_IC_CHANNEL_ID) // <=> HAL_TIM_ACTIVE_CHANNEL_1 // Equivalent (1 << (ChannelId >> 2))
//#define FAN4_IC_SRC_DATAS		&mFanIcDatas[FAN4_IC_SRC_DATA_POS]
//#define FAN4_IC_GET_DATAS		&mFanIcOnlyData[FAN4_DATA_POS].IcDatas

//#define FAN4_IC_TIME_OUT		FPIC_MAKE_TIME_OUT_ms(100)	// TimeOut = 1.1s (pour RPM > 600)
//#define FAN4_IC_FAPB			FPIC_MAKE_FAPB_MHz(60)	// Tim5 sur APB1 @ 60MHz
//#define FAN4_IC_PSC 			91	// Valeur du Prescaler PSC de CubeMx (= htim#.Init.Prescaler dans "tim.c")
//#define FAN4_IC_PPT 			1	// Nb of "Pulse Per Turn" (PPT) from the fan feedback, generally only 1
//#define FAN4_IC_K_UNIT  		60	// Coefficient de FeedBack, pour convertir l'unité Hz -> RPM
//#define FAN4_IC_K_FEED_BACK 	FPIC_MAKE_K_FEED_BACK(FAN4_IC_FAPB, FAN4_IC_PSC, FAN4_IC_PPT, FAN4_IC_K_UNIT)
//#define FAN4_IC_K_FREQ		FPIC_MAKE_K_FREQ(FAN4_IC_FAPB, FAN4_IC_PSC, FAN4_IC_PPT)// pour avoir la fréquence du signal d'entrée
/******************************************************************************/

tFanPwmIcInitParams mFanInitParam[] = {

#if defined(FAN1_PWM_HANDLE) && defined(FAN1_IC_HANDLE)
	{ { FAN1_PWM_HANDLE, FAN1_PWM_CHANNEL_ID, FAN1_PWM_MAX_VALUE, FAN1_PWM_DATAS }, { FAN1_IC_HANDLE, FAN1_IC_CHANNEL_ID, FAN1_IC_CHANNEL_FLAG, FAN1_IC_TIME_OUT, FAN1_IC_K_FEED_BACK, FAN1_IC_K_FREQ, FAN1_IC_SRC_DATAS, FAN1_IC_GET_DATAS } },
#endif // FAN1_PWM_HANDLE & FAN1_IC_HANDLE

#if defined(FAN2_PWM_HANDLE) && defined(FAN2_IC_HANDLE)
	{ { FAN2_PWM_HANDLE, FAN2_PWM_CHANNEL_ID, FAN2_PWM_MAX_VALUE, FAN2_PWM_DATAS }, { FAN2_IC_HANDLE, FAN2_IC_CHANNEL_ID, FAN2_IC_CHANNEL_FLAG, FAN2_IC_TIME_OUT, FAN2_IC_K_FEED_BACK, FAN2_IC_K_FREQ, FAN2_IC_SRC_DATAS, FAN2_IC_GET_DATAS } },
#endif // FAN2_PWM_HANDLE & FAN2_IC_HANDLE

#if defined(FAN3_PWM_HANDLE) && defined(FAN3_IC_HANDLE)
	{ { FAN3_PWM_HANDLE, FAN3_PWM_CHANNEL_ID, FAN3_PWM_MAX_VALUE, FAN3_PWM_DATAS }, { FAN3_IC_HANDLE, FAN3_IC_CHANNEL_ID, FAN3_IC_CHANNEL_FLAG, FAN3_IC_TIME_OUT, FAN3_IC_K_FEED_BACK, FAN3_IC_K_FREQ, FAN3_IC_SRC_DATAS, FAN3_IC_GET_DATAS } },
#endif // FAN3_PWM_HANDLE & FAN3_IC_HANDLE

#if defined(FAN4_PWM_HANDLE) && defined(FAN4_IC_HANDLE)
	{ { FAN4_PWM_HANDLE, FAN4_PWM_CHANNEL_ID, FAN4_PWM_MAX_VALUE, FAN4_PWM_DATAS }, { FAN4_IC_HANDLE, FAN4_IC_CHANNEL_ID, FAN4_IC_CHANNEL_FLAG, FAN4_IC_TIME_OUT, FAN4_IC_K_FEED_BACK, FAN4_IC_K_FREQ, FAN4_IC_SRC_DATAS, FAN4_IC_GET_DATAS } },
#endif // FAN4_PWM_HANDLE & FAN4_IC_HANDLE

};
FPIC_MAKE_CONST_BASE_AND_END_PTR_OF_TABLE(tFanPwmIcInitParams, FPIC_FIRST_INIT_PARAM, FPIC_AFTER_INIT_PARAM, mFanInitParam);

/******************************************************************************/

void setFanExhaustVoltage_mV(uint16_t newVoltage)
{

#ifdef FAN1_PWM_DATAS
	FanPwmIC_UpdatePwmValue(FAN1_PWM_DATAS, FAN1_PWM_K_FACTOR * newVoltage);
#endif // FAN1_PWM_DATAS

}

void setFanExhaustVoltage_percent(uint16_t  motorSetPoint)
{
#ifdef FAN1_PWM_DATAS
	uint16_t coef = FAN1_PWM_MAX_VALUE / 100;  // conversion
	uint16_t newVoltage = motorSetPoint * coef / 10; // Mise dans la bonne unité.  valeur recu en % x10
	setFanExhaustVoltage_mV( newVoltage );
#endif // FAN1_PWM_DATAS
}

uint16_t getFanExhaustVoltage_mV(void)
{

#ifdef FAN1_PWM_DATAS
	return (uint16_t)(FanPwmIC_getFanVoltage_mV(FAN1_PWM_DATAS) / FAN1_PWM_K_FACTOR);
#else // !FAN1_PWM_DATAS
	return 0;
#endif // FAN1_PWM_DATAS

}

/******************************************************************************/

uint16_t getFanExhaustFeedbackSpeed(void)
{

#ifdef FAN1_IC_GET_DATAS
	return FanPwmIC_getFanSpeed(FAN1_IC_GET_DATAS);
#else // !FAN1_IC_GET_DATAS
	return 0;
#endif // FAN1_IC_GET_DATAS

}

uint16_t getFanExhaustLastFeedbackSpeed(void)
{

#if defined(FAN1_IC_GET_DATAS) && defined(FPIC_GET_LAST_FEED_BACK)
	return FanPwmIC_getLastFanSpeed(FAN1_IC_GET_DATAS);
#else // !FAN1_IC_GET_DATAS || !FPIC_GET_LAST_FEED_BACK
	return 0;
#endif // FAN1_IC_GET_DATAS

}

uint16_t getFanExhaustLastDeltaTime(void)
{

#if defined(FAN1_IC_GET_DATAS) && defined(FPIC_GET_LAST_DELTA_TIME)
	return FanPwmIC_getLastDeltaTime(FAN1_IC_GET_DATAS);
#else // !FAN1_IC_GET_DATAS || !FPIC_GET_LAST_DELTA_TIME
	return 0;
#endif // FAN1_IC_GET_DATAS

}

uint32_t getFanExhaustLastFrequency(void)
{

#if defined(FAN1_IC_GET_DATAS) && defined(FPIC_GET_LAST_FREQUENCY)
	return FanPwmIC_getLastFrequency(FAN1_IC_GET_DATAS);
#else // !FAN1_IC_GET_DATAS || !FPIC_GET_LAST_FREQUENCY
	return 0;
#endif // FAN1_IC_GET_DATAS

}

/******************************************************************************/

void setFanSupplyVoltage_mV(uint16_t newVoltage)
{

#ifdef FAN2_PWM_DATAS
	FanPwmIC_UpdatePwmValue(FAN2_PWM_DATAS, FAN2_PWM_K_FACTOR * newVoltage);
#endif // FAN2_PWM_DATAS

}

uint16_t getFanSupplyVoltage_mV(void)
{

#ifdef FAN2_PWM_DATAS
	return (uint16_t)(FanPwmIC_getFanVoltage_mV(FAN2_PWM_DATAS) / FAN2_PWM_K_FACTOR);
#else // !FAN2_PWM_DATAS
	return 0;
#endif // FAN2_PWM_DATAS

}

/******************************************************************************/

uint16_t getFanSupplyFeedbackSpeed(void)
{

#ifdef FAN2_IC_GET_DATAS
	return FanPwmIC_getFanSpeed(FAN2_IC_GET_DATAS);
#else // !FAN2_IC_GET_DATAS
	return 0;
#endif // FAN2_IC_GET_DATAS

}

uint16_t getFanSupplyLastFeedbackSpeed(void)
{

#if defined(FAN2_IC_GET_DATAS) && defined(FPIC_GET_LAST_FEED_BACK)
	return FanPwmIC_getLastFanSpeed(FAN2_IC_GET_DATAS);
#else // !FAN2_IC_GET_DATAS || !FPIC_GET_LAST_FEED_BACK
	return 0;
#endif // FAN2_IC_GET_DATAS & FPIC_GET_LAST_FEED_BACK

}

uint16_t getFanSupplyLastDeltaTime(void)
{

#if defined(FAN2_IC_GET_DATAS) && defined(FPIC_GET_LAST_DELTA_TIME)
	return FanPwmIC_getLastDeltaTime(FAN2_IC_GET_DATAS);
#else // !FAN2_IC_GET_DATAS || !FPIC_GET_LAST_DELTA_TIME
	return 0;
#endif // FAN2_IC_GET_DATAS & FPIC_GET_LAST_DELTA_TIME

}

uint32_t getFanSupplyLastFrequency(void)
{

#if defined(FAN2_IC_GET_DATAS) && defined(FPIC_GET_LAST_FREQUENCY)
	return FanPwmIC_getLastFrequency(FAN2_IC_GET_DATAS);
#else // !FAN2_IC_GET_DATAS || !FPIC_GET_LAST_FREQUENCY
	return 0;
#endif // FAN2_IC_GET_DATAS & FPIC_GET_LAST_FREQUENCY

}

/******************************************************************************/

void ApplyFanSwap_Exhaust_Supply(uint8_t swapAction)
{

#if defined(FAN1_PWM_DATAS) && defined(FAN2_PWM_DATAS) && defined(FAN1_IC_GET_DATAS) && defined(FAN2_IC_GET_DATAS)
	FanPwmIC_SwapPwmIc(FAN1_PWM_DATAS, FAN1_IC_GET_DATAS, FAN2_PWM_DATAS, FAN2_IC_GET_DATAS, swapAction);
#endif // FAN1_PWM_DATAS && FAN2_PWM_DATAS && FAN1_IC_GET_DATAS && FAN2_IC_GET_DATAS

//#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS
//	#if defined(FAN1_PWM_DATAS) && defined(FAN2_PWM_DATAS)
//		FanPwmIC_SwapPwmDataPtr(FAN1_PWM_DATAS, FAN2_PWM_DATAS, swapAction);
//	#endif // FAN1_PWM_DATAS && FAN2_PWM_DATAS
//#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS

//#ifdef FPIC_ENABLE_SWAP_IC_FEED_BACK
//	#if defined(FAN1_IC_GET_DATAS) && defined(FAN2_IC_GET_DATAS)
//		FanPwmIC_SwapIcDataOutPtr(FAN1_IC_GET_DATAS, FAN2_IC_GET_DATAS, swapAction);
//	#endif // FAN1_IC_DATAS_OUT && FAN2_IC_DATAS_OUT
//#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK

}

uint8_t AreFansSwapped_Exhaust_Supply(void)
{

#ifdef FPIC_ENABLE_SWAP_PWM_COMMANDS
	#if defined(FAN1_PWM_DATAS) && defined(FAN2_PWM_DATAS)
		return FanPwmIC_ArePwmSwapped(FAN1_PWM_DATAS, FAN2_PWM_DATAS);
	#endif // FAN1_PWM_DATAS && FAN2_PWM_DATAS
//#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS
#elif defined(FPIC_ENABLE_SWAP_IC_FEED_BACK)
	#if defined(FAN1_IC_GET_DATAS) && defined(FAN2_IC_GET_DATAS)
		return FanPwmIC_AreIcSwapped(FAN1_IC_GET_DATAS, FAN2_IC_GET_DATAS);
	#endif // FAN1_IC_GET_DATAS && FAN2_IC_GET_DATAS
//#endif // FPIC_ENABLE_SWAP_IC_FEED_BACK
//#else // !FPIC_ENABLE_SWAP_PWM_COMMANDS && !FPIC_ENABLE_SWAP_IC_FEED_BACK
#endif // FPIC_ENABLE_SWAP_PWM_COMMANDS && FPIC_ENABLE_SWAP_IC_FEED_BACK
	return 0;
//#endif

}

/******************************************************************************/
#ifdef __cplusplus
}
#endif
