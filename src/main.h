#ifndef MAIN_H_
#define MAIN_H_

#define bool	_Bool
#define true	1
#define false	0
#define __bool_true_false_are_defined 1

#define NULL 0

#define HIGH true
#define LOW false

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

#define FREQUENCY 72000000

// NVIC addresses found in the Cortex-M3 programming manual
#define NVIC			0xE000E100

#define FLASH_ACR		0x40022000

// Register addresses can be found on page 40 of the stm32f103 manual
#define RCC_CR		 	0x40021000
#define RCC_CFGR	 	0x40021004
#define RCC_APB2ENR 	0x40021018
#define RCC_APB1ENR 	0x4002101C



typedef union RCC{
	uint32_t data;

	struct CR{
		uint8_t HSION 	: 1;	// Internal high-speed clock enable
		uint8_t HSIRDY 	: 1;	// Internal high-speed clock ready flag
		uint8_t 	 	: 1;	// .
		uint8_t HSITRIM	: 5;	// Internal high-speed clock trimming
		uint8_t HSICAL	: 8;	// Internal high-speed clock calibration
		uint8_t HSEON	: 1;	// HSE clock enable
		uint8_t HSERD	: 1;	// External high-speed clock ready flag
		uint8_t HSEBYP	: 1;	// External high-speed clock bypass
		uint8_t CSSON	: 1;	// Clock security system enable
		uint8_t 		: 4;	// .
		uint8_t PLLON	: 1;	// PLL enable
		uint8_t PLLRDY	: 1;	// PLL clock ready flag
		uint8_t			: 6;	// .
	}CR;

	struct CFGR{
		uint8_t SW		: 2;	// System clock switch
		uint8_t SWS		: 2;	// System clock switch status
		uint8_t HPRE	: 4;	// AHB prescaler
		uint8_t PPRE1	: 3;	// APB low-speed prescaler (APB1)
		uint8_t PPRE2	: 3;	// APB high-speed prescaler (APB2)
		uint8_t ADCPRE	: 2;	// ADC prescaler
		uint8_t PLLSRC	: 1;	// PLL entry clock source
		uint8_t PLLXTPRE: 1;	// HDE Divider for PLL entry
		uint8_t PLLMUL	: 4;	// PLL multiplication factor
		uint8_t USBPRE	: 1;	// USB Prescaler
		uint8_t 		: 1;	// .
		uint8_t MCO		: 3;	// Microcontroller clock output
		uint8_t 		: 5;	// .
	}CFGR;

	struct ABP2ENR{

	}ABP2ENR;
}RCC;

typedef union FLASH{
	uint32_t data;

	struct ACR{
		uint8_t LATENCY	: 3;	// Latency
		uint8_t HLFCYA	: 1;	// Flash half cycle access enable
		uint8_t PRFTBE	: 1;	// Prefetch buffer enable
		uint8_t PRFTBS	: 1;	// Prefetch buffer status
		uint8_t 		: 8;	// .
		uint8_t 		: 2;	// .
	}ACR;
}FLASH;

// TMP Variables
extern bool led_state;

#endif