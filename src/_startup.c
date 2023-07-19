#include "stdlib.h"
#include "rcc.h"
#include "gpio.h"
#include "rtc.h"

#include "usart.h"
#include "usb.h"
#include "terminal.h"
#include "stm32f103xb.h"


#define STACK_TOP 0x20005000
void startup(void);
void default_handler(void);

// Vector table
uint32_t *VectorTable[] __attribute__((section("vector_table"))) = {
	(uint32_t *) STACK_TOP,			// Initial Stack Pointer (SP)
	(uint32_t *) startup,			// Reset Handler function
	/* -14*/ 0,									// Non maskable interrupt (NMI)
	/* -13*/ 0,									// Hard fault (all class of fault)
	/* -12*/ 0,									// Memory management
	/* -11*/ 0,									// Bus fault (prefetch fault, memory access fault)
	/* -10*/ 0,									// Usage fault (undefined instruction or illegal state)
	/* -9 */ 0,									// * (Reserved)
	/* -8 */ 0,									// *
	/* -7 */ 0,									// *
	/* -6 */ 0,									// *
	/* -5 */ (uint32_t *) default_handler,		// SVCall (system service call via SWI instruction)
	/* -4 */ (uint32_t *) default_handler,		// Debug Monitor
	/* -3 */ 0,									// *
	/* -2 */ (uint32_t *) default_handler,		// PendSV (Pendable request for system service)
	/* -1 */ (uint32_t *) default_handler,				// SysTick (System tick timer)

	/* 0  */ (uint32_t *) default_handler,		// WWDG (Window watchdog interrupt)
	/* 1  */ (uint32_t *) default_handler,		// PVD (PVD through EXTI line detection interrupt)
	/* 2  */ (uint32_t *) default_handler,		// Tamper interrupt
	/* 3  */ (uint32_t *) RTCInterrupt,			// RTC global interrupt
	/* 4  */ (uint32_t *) default_handler,		// Flash global interrupt
	/* 5  */ (uint32_t *) default_handler,		// RCC global interrupt
	/* 6  */ (uint32_t *) default_handler,		// EXTI0 (EXTI Line0 interrupt)
	/* 7  */ (uint32_t *) default_handler,		// EXTI1
	/* 8  */ (uint32_t *) default_handler,		// EXTI2
	/* 9  */ (uint32_t *) default_handler,		// EXTI3
	/* 10 */ (uint32_t *) default_handler,		// EXTI4
	/* 11 */ (uint32_t *) default_handler,		// DMA1_Channel1 (DMA 1 Channel 1 global interrupt)
	/* 12 */ (uint32_t *) default_handler,		// DMA1_Channel2
	/* 13 */ (uint32_t *) default_handler,		// DMA1_Channel3
	/* 14 */ (uint32_t *) default_handler,		// DMA1_Channel4
	/* 15 */ (uint32_t *) default_handler,		// DMA1_Channel5
	/* 16 */ (uint32_t *) default_handler,		// DMA1_Channel6
	/* 17 */ (uint32_t *) default_handler,		// DMA1_Channel7
	/* 18 */ (uint32_t *) default_handler,		// ADC1_2 (ADC 1 and 2 global interrupt)
	/* 19 */ (uint32_t *) USBInterrupt,			// USB_HP_CAN_TX (USB High Priority or CAN TX interrupts)
	/* 20 */ (uint32_t *) USBInterrupt,			// USB_LP_CAN_RX0 (USB Low Priority or CAN RX0 interrupts)
	/* 21 */ (uint32_t *) default_handler,		// CAN_RX1 (CAN RX1 interrupt)
	/* 22 */ (uint32_t *) default_handler,		// CAN_SCE (CAN SCE interrupt)
	/* 23 */ (uint32_t *) default_handler,		// EXTI9_5 (EXTI Line[9:5] interrupts)
	/* 24 */ (uint32_t *) default_handler,		// TIM1_BRK (TIM1 break interrupt)
	/* 25 */ (uint32_t *) default_handler,		// TIM1_UP (TIM1 update interrupt)
	/* 26 */ (uint32_t *) default_handler,		// TIM1_TRG_COM (TIM1 trigger and commutation interrupts)
	/* 27 */ (uint32_t *) default_handler,		// TIM1_CC (TIM1 capture compare interrupt)
	/* 28 */ (uint32_t *) default_handler,		// TIM2
	/* 29 */ (uint32_t *) default_handler,		// TIM3
	/* 30 */ (uint32_t *) default_handler,		// TIM4
	/* 31 */ (uint32_t *) default_handler,		// I2C1_EV (Event interrupt)
	/* 32 */ (uint32_t *) default_handler,		// I2C1_ER (Error interrupt)
	/* 33 */ (uint32_t *) default_handler,		// I2C2_EV (Event interrupt)
	/* 34 */ (uint32_t *) default_handler,		// I2C2_ER (Error interrupt)
	/* 35 */ (uint32_t *) default_handler,		// SPI1
	/* 36 */ (uint32_t *) default_handler,		// SPI2
	/* 37 */ (uint32_t *) USARTInterrupt,		// USART1
	/* 38 */ (uint32_t *) default_handler,		// USART2
	/* 39 */ (uint32_t *) default_handler,		// USART3
	/* 40 */ (uint32_t *) default_handler,		// EXTI15_10 (EXTI Line[15:10] interrupts)
	/* 41 */ (uint32_t *) default_handler,		// RTCAlarm (RTC alarm through EXTI line interrupt)
	/* 42 */ (uint32_t *) USBInterrupt,			// USBWakeup
	/* 43 */ (uint32_t *) default_handler,		// 
	/* 44 */ (uint32_t *) default_handler,		// 
	/* 45 */ (uint32_t *) default_handler,		// 
	/* 46 */ (uint32_t *) default_handler,		// 
	/* 47 */ (uint32_t *) default_handler,		// 
	/* 48 */ (uint32_t *) default_handler,		// 
	/* 49 */ (uint32_t *) default_handler,		// 
	/* 50 */ (uint32_t *) default_handler,		// 
	/* 51 */ (uint32_t *) default_handler,		// 
	/* 52 */ (uint32_t *) default_handler,		// 
	/* 53 */ (uint32_t *) default_handler,		// 
	/* 54 */ (uint32_t *) default_handler,		// TIM6 global interrupt
	/* 55 */ (uint32_t *) default_handler,		// TIM7 global interrupt

	// TODO: Copy vector table to ram so we can modify interrupts inside of functions
};

// SYSTICK *systick = (SYSTICK *)0xE000E010;

char *test_str = "lets see something";
bool led_state = LOW;

void default_handler(void){}

void *memcpy(void *dst, const void *src, size_t len){
	char *d = dst;
	const char *s = src;

	for(size_t i = 0; i < len; i++){
		d[i] = s[i];
	}

	return dst;
}

void *memset(void *dst, int value, size_t len){
	char *d = dst;
	for(size_t i = 0; i < len; i++){
		d[i] = value;
	}
	return dst;
}

extern unsigned int _data_s;
extern unsigned int _data_e;
extern unsigned int _data_flash_addr;

extern unsigned int _bss_s;
extern unsigned int _bss_e;


void startup(void){
	// Copy .data section to memory and zero out .bss section
	memcpy(&_data_s, &_data_flash_addr, sizeof(size_t) * (&_data_e - &_data_s));
	memset(&_bss_s, 0, &_bss_e - &_bss_s);

	SetClock_48MHz();

/* --- Set up systick --- */
	// wait 500ms
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // Set the clock source to directly the system clock
	SysTick->LOAD = 400000;
	SysTick->VAL = 0;

/* --- Enable GPIO ports --- */
	GPIOEnable(GPIO_PORT_A);
	GPIOEnable(GPIO_PORT_C);


/* --- SETUP PC13 (LED) --- */
	GPIOSetPinMode(GPIO_PORT_C, 13, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_GP_PUSHPULL);
	GPIOWrite(GPIO_PORT_C, 13, HIGH);

/* --- SETUP UART --- */
	USARTInit();
	USARTWrite("USART Initialized!\n");


/* --- SETUP RTC --- */
	InitRTC();


/* --- SETUP USB --- */
	USBInit();

	while(1){

		if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0){
			SysTick->CTRL &= ~1;
			GPIOWrite(GPIO_PORT_C, 13, led_state);
			led_state = !led_state;
			SysTick->CTRL |= 1;
		}

		Terminal();
	}
	
}