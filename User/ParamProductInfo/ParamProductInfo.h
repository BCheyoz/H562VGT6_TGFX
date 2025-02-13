#ifndef PARAMPRODUCTINFO_H
#define PARAMPRODUCTINFO_H

#include <stdint.h>

typedef enum {
E_FRANCE = 0,
E_BELGIQUE,
E_ESPAGNE,
E_ITALIE,
E_ALLEMAGNE,
E_DANEMARK,
E_NORVEGE,
E_SUEDE,
E_EUROPE,
E_CANADA,
E_CHINA,
E_NB_TARGET_ID
}E_TARGET_ID;

#ifdef __cplusplus
extern "C" {
#endif
void HandleWorkingTimeRT_1s(void);
void ParamProductInit();

uint8_t isCodeBoardCompliant();
uint8_t isCodeIdCompliant();
uint8_t isSAVProduct();

uint32_t getCurrentDateTimeCounter();
uint32_t getventilSecondCounter();

uint8_t isCommissionningDone(void);
void setCommissionningState(uint8_t state);
void resetParamProduct();
void resetCodeProduct();

#define GET_SET_DECLARATION(a, d)	d Get##a(void);\
		void Write##a(d value);

GET_SET_DECLARATION(SapProduct,	uint32_t)
GET_SET_DECLARATION(SnProduct,	uint64_t)
GET_SET_DECLARATION(TestDate,	uint16_t)
GET_SET_DECLARATION(SapBoard, 	uint32_t)
GET_SET_DECLARATION(SnBoard,	uint64_t)
GET_SET_DECLARATION(ProductId,	uint16_t)
GET_SET_DECLARATION(SoftTargetId,uint8_t)

#ifdef __cplusplus
}
#endif

#endif // PARAMPRODUCTINFO_H
