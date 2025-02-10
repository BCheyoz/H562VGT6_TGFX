#ifndef MSM_MAINSTATEMACHINE_H
#define MSM_MAINSTATEMACHINE_H

#include <stdint.h>

//#define USE_SAV_STATE
//#define USE_COMMISIONNING_STATE
//#define USE_ALIVE_LED

typedef enum
{
	E_BOOT_STATE = 0, // Etat réserver au Bootlaoder
	E_BOARD_READY_STATE, // Etat initial de la partie applicatif
#ifdef USE_SAV_STATE
	E_BOARD_SAV_READY_STATE = 2,
#endif
#ifdef USE_COMMISIONNING_STATE
	E_PRODUCT_READY_STATE = 3,
#endif
	E_PRODUCT_COMPLETE_STATE = 4,
	E_FACTORY_BENCH_STATE,
	E_FACTORY_STATE,
}e_softState;

void handleMainStateMachineRT_100ms();
void MSM_Init();
void MSM_Mgt();

e_softState firmwareState();
void requestToSwitchToFactoryState(uint16_t value);

void requestProductReset(uint16_t value);
void requestResetMemories(uint16_t code);
uint16_t resetMemoriesState();

void requestToInitRegulation(uint16_t value);

#ifdef USE_COMMISIONNING_STATE
void resetCommissionningState(uint8_t code);
void requestEndOfCommissionning(uint8_t code);
void requestConfigReset(uint8_t code);
#endif

#ifdef USE_SAV_STATE
void requestSAVreset(uint8_t code);
#endif

#ifdef USE_ALIVE_LED
void requestBlinkMode(uint16_t newBlinkMode);
uint16_t blinkMode();
#endif

#endif // MSM_MAINSTATEMACHINE_H
