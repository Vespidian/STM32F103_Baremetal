#ifndef MAIN_H_
#define MAIN_H_

#define bool	_Bool
#define true	1
#define false	0
#define __bool_true_false_are_defined 1

#define NULL 0

#define HIGH true
#define LOW false

typedef unsigned int uintptr_t;
typedef unsigned int size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
// typedef unsigned long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

// #define FREQUENCY 72000000
#define FREQUENCY 8000000

// Flash storage access control register?
#define FLASH_ACR		0x40022000


#define PWR_CR			0x40007000
#define PWR_CSR			0x40007004

typedef struct PWR{
	struct {
		uint32_t LPDS	: 1; // Low-power deepsleep
		uint32_t PDDS	: 1; // Power down deepsleep
		uint32_t CWUF	: 1; // Clear wakeup flag
		uint32_t CSBF	: 1; // Clear standby flag
		uint32_t PVDE	: 1; // Programmable voltage detector enable
		uint32_t PLS	: 3; // PVD level selection
		uint32_t DBP	: 1; // Disable backup domain write protection
		uint32_t 		: 23; // .
	}CR;

	struct {
		uint32_t WUF	: 1; // Wakeup flag
		uint32_t SBO	: 1; // Standby flag
		uint32_t PVDO	: 1; // PVD output
		uint32_t 		: 5; // .
		uint32_t EWUP	: 1; // Enable WKUP pin
		uint32_t 		: 23; // .
	}CSR;
}PWR;

#define BKP_DR1 0x40006c04
#define BKP_RTCCR 0x40006c04

typedef union BKP{
	uint32_t data;

	uint16_t DR;

	struct{
		uint8_t CAL		: 7; // Calibration value
		uint8_t CCO		: 1; // Calibration clock output
		uint8_t ASOE	: 1; // Alarm or second output enable
		uint8_t ASOS	: 1; // Alarm or second output selection
		uint8_t			: 6; // .
	}RTCCR;
}BKP;

// Register addresses can be found on page 40 of the stm32f103 manual
// #define RCC_ADDR		0x40021000
// #define RCC_CR		 	RCC_ADDR + 0x00
// #define RCC_CFGR	 	RCC_ADDR + 0x04
// #define RCC_APB2ENR 	RCC_ADDR + 0x18
// #define RCC_APB1ENR 	RCC_ADDR + 0x1C
// #define RCC_BDCR	 	RCC_ADDR + 0x20
#define RCC_ADDR	 	0x40021000

#define RCC_CR		 	0x40021000
#define RCC_CFGR	 	0x40021004
#define RCC_APB2ENR 	0x40021018
#define RCC_APB1ENR 	0x4002101C
#define RCC_BDCR	 	0x40021020

typedef struct RCC{
	struct CR{
		uint32_t HSION 		: 1;	// Internal high-speed clock enable
		uint32_t HSIRDY 	: 1;	// Internal high-speed clock ready flag
		uint32_t 	 		: 1;	// .
		uint32_t HSITRIM	: 5;	// Internal high-speed clock trimming
		uint32_t HSICAL		: 8;	// Internal high-speed clock calibration
		uint32_t HSEON		: 1;	// HSE clock enable
		uint32_t HSERD		: 1;	// External high-speed clock ready flag
		uint32_t HSEBYP		: 1;	// External high-speed clock bypass
		uint32_t CSSON		: 1;	// Clock security system enable
		uint32_t 			: 4;	// .
		uint32_t PLLON		: 1;	// PLL enable
		uint32_t PLLRDY		: 1;	// PLL clock ready flag
		uint32_t			: 6;	// .
	}CR;

	struct CFGR{
		uint32_t SW			: 2;	// System clock switch
		uint32_t SWS		: 2;	// System clock switch status
		uint32_t HPRE		: 4;	// AHB prescaler
		uint32_t PPRE1		: 3;	// APB low-speed prescaler (APB1)
		uint32_t PPRE2		: 3;	// APB high-speed prescaler (APB2)
		uint32_t ADCPRE		: 2;	// ADC prescaler
		uint32_t PLLSRC		: 1;	// PLL entry clock source
		uint32_t PLLXTPRE	: 1;	// HDE Divider for PLL entry
		uint32_t PLLMUL		: 4;	// PLL multiplication factor
		uint32_t USBPRE		: 1;	// USB Prescaler
		uint32_t 			: 1;	// .
		uint32_t MCO		: 3;	// Microcontroller clock output
		uint32_t 			: 5;	// .
	}CFGR;

	uint32_t CIR; // padding (not yet defined)
	uint32_t APB2RSTR; // padding (not yet defined)
	uint32_t APB1RSTR; // padding (not yet defined)
	uint32_t AHBENR; // padding (not yet defined)


	struct APB2ENR{
		uint32_t AFIOEN		: 1; // Alternate function IO clock enable
		uint32_t 			: 1; // .
		uint32_t IOPAEN		: 1; // IO port A clock enable
		uint32_t IOPBEN		: 1; // IO port B clock enable
		uint32_t IOPCEN		: 1; // IO port C clock enable
		uint32_t IOPDEN		: 1; // IO port D clock enable
		uint32_t IOPEEN		: 1; // IO port E clock enable
		uint32_t IOPFEN		: 1; // IO port F clock enable
		uint32_t IOPGEN		: 1; // IO port G clock enable
		uint32_t ADC1EN		: 1; // ADC1 interface clock enable
		uint32_t ADC2EN		: 1; // ADC2 interface clock enable
		uint32_t TIM1EN		: 1; // TIM1 timer clock enable
		uint32_t SPI1EN		: 1; // SPI1 clock enable
		uint32_t TIM8EN		: 1; // TIM8 timer clock enable
		uint32_t USART1EN	: 1; // USART1 clock enable
		uint32_t ADC3EN		: 1; // ADC3 interface clock enable
		uint32_t 			: 3; // .
		uint32_t TIM9EN		: 1; // TIM9 timer clock enable
		uint32_t TIM10EN	: 1; // TIM10 timer clock enable
		uint32_t TIM11EN	: 1; // TIM11 timer clock enable
		uint32_t 			: 10;// .
	}APB2ENR;

	struct APB1ENR{
		uint32_t TIM2EN		: 1; // TIM2 timer clock enable
		uint32_t TIM3EN		: 1; // TIM3 timer clock enable
		uint32_t TIM4EN		: 1; // TIM4 timer clock enable
		uint32_t TIM5EN		: 1; // TIM5 timer clock enable
		uint32_t TIM6EN		: 1; // TIM6 timer clock enable
		uint32_t TIM7EN		: 1; // TIM7 timer clock enable
		uint32_t TIM12EN	: 1; // TIM12 timer clock enable
		uint32_t TIM13EN	: 1; // TIM13 timer clock enable
		uint32_t TIM14EN	: 1; // TIM14 timer clock enable
		uint32_t 			: 2; // .
		uint32_t WWDGEN		: 1; // Window watchdog clock enable
		uint32_t 			: 2; // .
		uint32_t SPI2EN		: 1; // SPI2 clock enable
		uint32_t SPI3EN		: 1; // SPI3 clock enable
		uint32_t 			: 1; // .
		uint32_t USART2EN	: 1; // USART2 clock enable
		uint32_t USART3EN	: 1; // USART3 clock enable
		uint32_t USART4EN	: 1; // USART4 clock enable
		uint32_t USART5EN	: 1; // USART5 clock enable
		uint32_t I2C1		: 1; // I2C1 clock enable
		uint32_t I2C2		: 1; // I2C2 clock enable
		uint32_t USBEN		: 1; // USB clock enable
		uint32_t 			: 1; // .
		uint32_t CANEN		: 1; // CAN clock enable
		uint32_t 			: 1; // .
		uint32_t BKPEN		: 1; // Backup interface clock enable
		uint32_t PWREN		: 1; // Power interface clock enable
		uint32_t DACEN		: 1; // DAC interface clock enable
		uint32_t 			: 2; // .
	}APB1ENR;

	// Backup domain control register
	struct BDCR{
		uint32_t LSEON		: 1; // External low-speed oscillator enable
		uint32_t LSERDY		: 1; // External low-speed oscillator ready
		uint32_t LSYBYP		: 1; // External low-speed oscillator bypass
		uint32_t 			: 5; // .
		uint32_t RTCSEL		: 1; // RTC clock source selection
		uint32_t 			: 5; // .
		uint32_t RTCEN		: 1; // RTC clock enable
		uint32_t BDRST		: 1; // Back up domain software reset
		uint32_t 			:16; // .
	}BDCR;

	uint32_t CSR; // padding (not yet defined)

}RCC;

extern RCC *rcc;

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
short custom_sin(unsigned char x);


#endif