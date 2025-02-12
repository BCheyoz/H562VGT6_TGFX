/*
 * FlashInfos.h
 *
 *  Created on: 28 nov. 2023
 *  Original Author: j.proux
 *
 *  Updated on: 31 Janv. 2024
 *  Updated by: j.proux
 *
 */

#ifndef VERSIONINFOS_FLASHINFOS_H_
#define VERSIONINFOS_FLASHINFOS_H_

#include "main.h"			// Pour accès aux bons includes des HAL

// Tentative de détermination automatique FlashBase & MaxSize :
#ifdef FLASHAXI_BASE
	#define FLASH_BASE_ADR		FLASHAXI_BASE	// 0x08000000UL = FLASHAXI_BASE : (up to 512 KB) embedded FLASH memory accessible over AXI
	#ifdef FLASH_END
		#define FLASH_MAX_SIZE	(FLASH_END +1 - FLASH_BASE_ADR)	// Calcul automatique si possible
	#else // ! FLASH_END
	#endif //FLASH_END
#elif defined(FLASH_BASE)// ! FLASHAXI_BASE && FLASH_BASE (= Legacy) :
	#define FLASH_BASE_ADR		FLASH_BASE		// 0x08000000UL = FLASH base address
	#ifdef FLASH_BANK1_END
		#define FLASH_MAX_SIZE	(FLASH_BANK1_END +1 - FLASH_BASE_ADR) // Calcul automatique si possible
	#elif defined(FLASH_BANK_SIZE)	// define pour les model STM32H562
		#define FLASH_MAX_SIZE FLASH_BANK_SIZE
	#else // !FLASH_BANK1_END
	#endif // FLASH_BANK1_END
#endif // FLASHAXI_BASE

// Configuration manuelle si la détection Auto n'a pas réussi :
#ifndef FLASH_BASE_ADR	// Configuration manuelle FLASH_BASE_ADR requise :
	#define FLASH_BASE_ADR		0x08000000UL	// Flash @ 0x08000000 sur STM32F732VE (cf. STM32F732VE_FLASH.ld)
	#warning "FLASH_BASE_ADR configurée manuellement !"
#endif // FLASH_BASE_ADR

#ifndef FLASH_DEVICE_SIZE	// Configuration manuelle FLASH_DEVICE_SIZE requise :
//	#define FLASH_DEVICE_SIZE	  (64 *1024UL)	// Flash =   64K sur STM32F7xxx8 (STM32F730x8, STM32F750x8)
//	#define FLASH_DEVICE_SIZE	 (256 *1024UL)	// Flash =  256K sur STM32F7xxxC (STM32F722xx, STM32F723xx, STM32F732xx, STM32F733xx)
//	#define FLASH_DEVICE_SIZE 	 (512 *1024UL)	// Flash =  512K sur STM32F7xxxE (STM32F722xx, STM32F723xx, STM32F732xx, STM32F733xx, STM32F745xx, STM32F746xx)
	#define FLASH_DEVICE_SIZE	(1024 *1024UL)	/* Flash = 1024K sur STM32F7xxxG (STM32F745xx, STM32F746xx, STM32F765xx, STM32F767xx, STM32F768Ax, STM32F769xx)
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 STM32H5xxxG (STM32H562xx)*/
//	#define FLASH_DEVICE_SIZE	(2048 *1024UL)	// Flash = 2048K sur STM32F7xxxI (STM32F765xx, STM32F767xx, STM32F768Ax, STM32F769xx)
#endif // FLASH_MAX_SIZE

#ifndef FLASH_MAX_SIZE
	#warning "FLASH_MAX_SIZE could not be detected !"
#elif !defined(FLASH_DEVICE_SIZE)
	#error "FLASH_DEVICE_SIZE may be configured manually !"
#elif FLASH_DEVICE_SIZE > FLASH_MAX_SIZE
	#error "FLASH_DEVICE_SIZE could not be greater than Internal Flash Size !"
#else // FLASH_DEVICE_SIZE <= FLASH_MAX_SIZE :
	// OK, nothing to do here :-) .
#endif //

#ifndef VECTORS_BLOC_SIZE	// Configuration manuelle VECTORS_BLOC_SIZE requise :
//	#define VECTORS_BLOC_SIZE	( 67 *sizeof(uint32_t))	//  67 Vecteurs de type .word (= UINT32) sur STM32F103RBTx (Cortex M3,  cf. "g_pfnVectors" in "startup_stm32f103rbtx.s")
//	#define VECTORS_BLOC_SIZE	(120 *sizeof(uint32_t))	// 120 Vecteurs de type .word (= UINT32) sur STM32F732xx   (Cortex M7,  cf. "g_pfnVectors" in "startup_stm32f732xx.s")
	#define VECTORS_BLOC_SIZE	(147 *sizeof(uint32_t))	// 147 Vecteurs de type .word (= UINT32) sur STM32H562xx   (Cortex M7,  cf. "g_pfnVectors" in "startup_stm32h562xx.s")
//	#define VECTORS_BLOC_SIZE	( 46 *sizeof(uint32_t))	//  46 Vecteurs de type .word (= UINT32) sur STM32G070CBTx (Cortex M0+, cf. "g_pfnVectors" in "startup_stm32g070cbtx.s")
//	#define VECTORS_BLOC_SIZE	( 47 *sizeof(uint32_t))	//  47 Vecteurs de type .word (= UINT32) sur STM32G0B1CETx (Cortex M0+, cf. "g_pfnVectors" in "startup_stm32g0b1cetx.s")
#endif // VECTORS_BLOC_SIZE

#define BOOTLOADER_BASE_ADR	(FLASH_BASE_ADR +0)
#ifndef APP_WITH_BOOT // for *ALL* non-Release Builds :
	#define BOOTLOADER_MAX_SIZE	0 // BootLoader = Absent
#else // APP_WITH_BOOT, for Release Build only, from Project Properties -> Build Settings -> Preprocessor -> Symbols :
	#define BOOTLOADER_MAX_SIZE	(128 *1024UL)	// BL = 128K => 0x20000 + 0x08000000UL = 0x8020000
// Test BL_256	#define BOOTLOADER_MAX_SIZE	(256 *1024UL)	// BL = 256K => 0x40000 + 0x08000000UL = 0x8040000 (temporaire pour les essais)
#endif // APP_WITH_BOOT

#define FIRMWARE_BASE_ADR	(BOOTLOADER_BASE_ADR + BOOTLOADER_MAX_SIZE) // FlashBase + BL
#define FIRMWARE_MAX_SIZE	(FLASH_DEVICE_SIZE - BOOTLOADER_MAX_SIZE) // SizeMax = 512K - BL
#define FIRMWARE_LAST_ADR	(FIRMWARE_BASE_ADR + FIRMWARE_MAX_SIZE -1)

#define RAM_ADR_MSK_512K	0xFFF80000UL	// Masque pour accepter 512K de RAM

#endif /* VERSIONINFOS_FLASHINFOS_H_ */
