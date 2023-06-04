#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "rtc.h"
#include "adc.h"
#include "i2c.h"

#include "terminal.h"

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
	/* -1 */ (uint32_t *) default_handler,		// SysTick (System tick timer)

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
	/* 19 */ (uint32_t *) default_handler,		// USB_HP_CAN_TX (USB High Priority or CAN TX interrupts)
	/* 20 */ (uint32_t *) default_handler,		// USB_LP_CAN_RX0 (USB Low Priority or CAN RX0 interrupts)
	/* 21 */ (uint32_t *) default_handler,		// CAN_RX1 (CAN RX1 interrupt)
	/* 22 */ (uint32_t *) default_handler,		// CAN_SCE (CAN SCE interrupt)
	/* 23 */ (uint32_t *) default_handler,		// EXTI9_5 (EXTI Line[9:5] interrupts)
	/* 24 */ (uint32_t *) default_handler,		// TIM1_BRK (TIM1 break interrupt)
	/* 25 */ (uint32_t *) default_handler,		// TIM1_UP (TIM1 update interrupt)
	/* 26 */ (uint32_t *) default_handler,		// TIM1_TRG_COM (TIM1 trigger and commutation interrupts)
	/* 27 */ (uint32_t *) Tim1Ch1_IRQ,			// TIM1_CC (TIM1 capture compare interrupt)
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
	/* 42 */ (uint32_t *) default_handler,		// 
	/* 43 */ (uint32_t *) default_handler,		// 

	// TODO: Copy vector table to ram so we can modify interrupts inside of functions
};

RCC *rcc = (RCC *)RCC_ADDR;

char *test_str = "lets see something";
bool led_state = LOW;
// bool is_running = true;

bool GetRegisterBit(uint32_t *reg, uint8_t bit){
	unsigned int test = *reg;
	return ((test >> bit) & 1);
}

void default_handler(void){}

void *memcpy(void *dst, const void *src, size_t len){
	// size_t i;

	// // // If data is 32 bit aligned, copy a whole 32 bit chunk at once
	// if ((uintptr_t)dst % sizeof(long) == 0 && (uintptr_t)src % sizeof(long) == 0 && len % sizeof(long) == 0) {
	// 	long *d = dst;
	// 	const long *s = src;

	// 	for (i=0; i<len/sizeof(long); i++) {
	// 			d[i] = s[i];
	// 	}
	// }else {
		char *d = dst;
		const char *s = src;

	// 	for (i=0; i<len; i++) {
	// 			d[i] = s[i];
	// 	}
	// }

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

short custom_sin(unsigned char x){
	static const short sin_lut[64] = {0, 804, 1607, 2410, 3211, 4011, 4807, 5601, 6392, 7179, 7961, 8739, 9511, 10278, 11038, 11792, 12539, 13278, 14009, 14732, 15446, 16150, 16845, 17530, 18204, 18867, 19519, 20159, 20787, 21402, 22004, 22594, 23169, 23731, 24278, 24811, 25329, 25831, 26318, 26789, 27244, 27683, 28105, 28510, 28897, 29268, 29621, 29955, 30272, 30571, 30851, 31113, 31356, 31580, 31785, 31970, 32137, 32284, 32412, 32520, 32609, 32678, 32727, 32767};
    short value = sin_lut[x % 64];
    if(x >= 192){
        value = -sin_lut[63 - x % 64];
    }else if(x >= 128){
        value = -sin_lut[x % 64];
    }else if(x >= 64){
        value = sin_lut[63 - x % 64];
    }
    return value;
}

void SetClock_72MHz(){
	unsigned int *reg = 0;

	// Set flash latency for higher clock speed
	reg = (unsigned int *)FLASH_ACR;
	FLASH flash;
	flash.data = *reg;
	flash.ACR.LATENCY = 0b010;
	*reg = flash.data;

	// Enable the HSE and wait for it to be stable (HSERDY), disable the PLL so we can modify it
	rcc->CR.HSEON = 1;
	rcc->CR.PLLON = 0;
	rcc->CR.CSSON = 1;
	// while((((*reg) >> 17) & 1) == 0);
	while(rcc->CR.HSERD == 0);

	// Set PLL source and set PLL speed
	rcc->CFGR.PLLSRC = 1; // Src set to HSE
	rcc->CFGR.PLLMUL = 0b0111; // Multiplier x9
	rcc->CFGR.PPRE1 = 0b100; // Set APB1 to 36MHz

	// Enable PLL
	rcc->CR.PLLON = 1;
	// while((((*reg) >> 25) & 1) == 0); // Wait for PLLRDY
	while(rcc->CR.PLLRDY == 0);

	// Switch system clock to PLL
	rcc->CFGR.SW = 0b10;
}


static void TurnOn(){
	GPIOWrite(GPIO_PORT_A, 6, HIGH);


	// Fade led in using pwm and a sine function
	unsigned int *reg;
	for(size_t i = 0; i < 1000000; i++){
		reg = (unsigned int *)TIMER1_ADDR_CCR1;
		*reg = (custom_sin(i / 7812 + 196) + 32767) / 512;
	}
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

/* --- CHANGE CLOCK SPEED --- */
	// SetClock_72MHz();

/* --- Enable GPIO ports --- */
	GPIOEnable(GPIO_PORT_A);
	GPIOEnable(GPIO_PORT_C);

/* --- SETUP TIMER 1 --- */
	TimerInit();

/* --- SETUP PC13 (LED) --- */
	GPIOSetPinMode(GPIO_PORT_C, 13, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_GP_PUSHPULL);

	// Set up PA6 for led
	GPIOSetPinMode(GPIO_PORT_A, 6, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_GP_PUSHPULL);
	GPIOWrite(GPIO_PORT_A, 6, LOW);


/* --- ENABLE USART 1 --- */
	USARTInit();
	// USARTWrite("USART Initialized!\n");
	// GPIOWrite(GPIO_PORT_C, 13, LOW); // Turn the onboard LED on (low is on)
	// GPIOWrite(GPIO_PORT_C, 13, HIGH); // Turn the onboard LED OFF

/* --- SETUP USB --- */

/* --- SETUP ADC --- */
	// GPIOSetPinMode(GPIO_PORT_A, 0, GPIO_MODE_INPUT, 0); // Analog mode
	// USARTWrite("Enabling ADC\n");
	// ADCInit();

/* --- SETUP RTC --- */
	InitRTC();

	command_buffer_index = 0;
	// unsigned int *reg = 0;
	// unsigned int loop_counter = 0;
	// unsigned int previous_time = RTCGetTime();
	
	
	I2CInit();
	// for(int i = 0; i < 128; i++){
	// // 	// if(I2CReadByte(i) == 1){
	// 		// USARTWriteInt(I2CReadByte(0b01010101));
	// 		I2CWriteByte(i, 'a');
	// 		// USARTWrite("\n");
	// // 	// }
	// }


	while(1){


		// reg = (unsigned int *)ADC1_SR;
		// if(((*reg >> 1) & 1) == 1 && loop_counter % 256 == 0){
			// Print the data register to UART and start another conversion
			// reg = (unsigned int *)ADC1_DR;
			// USARTWriteInt(*reg & 0xffff);
			// USARTWriteInt((*reg << 16) >> 16);
			// USARTWriteByte('\n');

			// reg = (unsigned int *)ADC1_CR2;
			// *reg |= 1 << 22; // SWSTART
			// ADC adc; adc.data = *reg;
			// adc.CR2.SWSTART = 1;
			// *reg = adc.data;
		// }
		// if(RTCGetTime() == previous_time){
		// 	USARTWriteInt(previous_time = RTCGetTime());
		// 	USARTWriteByte('\n');
		// 	// if(previous_time == 30){
		// 	// 	USARTWrite("TIME!\n");
		// 	// }
		// }
		if(alarm_flag){
			TurnOn();
			alarm_flag = false;
		}



		// if(USARTReadByte() == 'f'){
		// 	USARTWrite("bar!\n");
		// }

		Terminal();


		// Fade led in and out using pwm and a sine function
		// reg = (unsigned int *)TIMER1_ADDR_CCR1;
		// *reg = (custom_sin((loop_counter / 500)) + 32767) / 512;
		// loop_counter++;

	}
	
}