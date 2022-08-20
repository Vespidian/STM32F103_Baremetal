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


// NVIC addresses found in the Cortex-M3 programming manual
#define NVIC			0xE000E100

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
		uint8_t 	 	: 1;	// RESERVED
		uint8_t HSITRIM	: 5;	// Internal high-speed clock trimming
		uint8_t HSICAL	: 8;	// Internal high-speed clock calibration
		uint8_t HSEON	: 1;	// HSE clock enable
		uint8_t HSERD	: 1;	// External high-speed clock ready flag
		uint8_t HSEBYP	: 1;	// External high-speed clock bypass
		uint8_t CSSON	: 1;	// Clock security system enable
		uint8_t 		: 4;	// RESERVED
		uint8_t PLLON	: 1;	// PLL enable
		uint8_t PLLRDY	: 1;	// PLL clock ready flag
	}CR;
	struct ABP2ENR{

	}ABP2ENR;
	struct CFGR{

	}CFGR;
}RCC;



// TMP Variables
extern bool led_state;

#endif