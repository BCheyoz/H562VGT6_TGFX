/*
 * ParamProductInfo.c
 *
 *  Created on: 7 Feb 2025
 *  Original Author: b.chhay
 *
 *	Passerelle pour la lecture des parametres en mémoire extern
 *
 */

#include "ParamProductInfo.h"
#include "utils.h"
#include "FirmwareGateway.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NB_TRY_READ_PARAMETERS		5	//5 tentative de lecture du parametre avant de declencher un probleme
#define NB_HOUR_EACH_DATETIME_SAVE	3
#define NB_1s_IN_1h					3600	// 3600 = 60 * 60
#define RECORD_TIMER				NB_HOUR_EACH_DATETIME_SAVE * NB_1s_IN_1h


uint32_t SapProduct;
uint64_t SnProduct;
uint16_t TestDate;
uint32_t SapBoard;
uint32_t SnBoard;
uint16_t ProductId;
uint8_t commissionningDone;
E_TARGET_ID ZoneTargetId;
E_PRODUCT_VERSION ProductVersionId;


static uint32_t DateTimeCounter_sec;
static uint32_t WorkingFanCounter_sec;
static uint16_t recordTimer_sec;

uint32_t sapBoardCodeDatabase[] = {1101234};
uint32_t sapProductCodeDatabase[] = {1100001, 1100002, 1100003};
uint16_t idCodeDatabase[] = {0001, 0002, 0003};

inline void HandleWorkingTimeRT_1s(void)
{
	DateTimeCounter_sec++;
	recordTimer_sec++;
	if(0 < fanFeedbackSpeed())
	{
		WorkingFanCounter_sec++;
	}

	// Enregister toute les 3h les compteur
	if(recordTimer_sec >= RECORD_TIMER)
	{
		recordTimer_sec -= RECORD_TIMER;
		// TODO
	}
}

void ParamProductInit(void){
	//init du flag d'erreur de lecture des parametres
	DateTimeCounter_sec = 0;
	WorkingFanCounter_sec = 0;
	recordTimer_sec = 0;

	//Lecture des parametres dans la mémoire
//	for(int i=0;i < NB_PARAM_USE; i++){
//		MultiTryReadAndResetIfError(&Parametres[i], 1);
//	}

}

uint8_t isCodeBoardCompliant()
{
    uint16_t tabSize = sizeof(sapBoardCodeDatabase) / sizeof(uint32_t);
    for(int i = 0; i < tabSize; ++i){
        if(sapBoardCodeDatabase[i] == SapBoard){
            return TRUE;
        }
    }

    return FALSE;
}

uint8_t isCodeIdCompliant()
{
	 uint16_t tabSize = sizeof(idCodeDatabase) / sizeof(uint16_t);
	 for(int i = 0; i < tabSize; ++i){
		 if(idCodeDatabase[i] == ProductId){
		 	return TRUE;
		 }
	 }

	return FALSE;
}

uint8_t isSAVProduct()
{
	if((SapProduct >= 11100000) && (SapProduct <= 11199999))
		return TRUE;
	else
		return FALSE;
}

uint32_t getCurrentDateTimeCounter(){
	return DateTimeCounter_sec;
}

uint32_t getventilSecondCounter(){
	return WorkingFanCounter_sec;
}

uint8_t isCommissionningDone()
{
	return commissionningDone;
}

void setCommissionningState(uint8_t state){
	// TODO
}

void resetParamProduct(){
	// TODO
}

void resetCodeProduct(){
	// TODO
}

// TODO
#define GET_SET_DEFINITION(a, d)	d Get##a(void)\
		{\
		return 0;\
		}\
		void Write##a(d value)\
		{\
		}


GET_SET_DEFINITION(SapProduct, 		uint32_t)
GET_SET_DEFINITION(SnProduct, 		uint64_t)
GET_SET_DEFINITION(TestDate, 		uint16_t)
GET_SET_DEFINITION(SapBoard, 		uint32_t)
GET_SET_DEFINITION(SnBoard, 		uint32_t)
GET_SET_DEFINITION(ProductId, 		uint16_t)
GET_SET_DEFINITION(ZoneTargetId, 	uint8_t)
GET_SET_DEFINITION(ProductVersionId,uint8_t)

#ifdef __cplusplus
}
#endif
