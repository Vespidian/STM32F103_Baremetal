#include "main.h"
#include "gpio.h"
#include "usart.h"

#define STACK_TOP 0x20005000
void USARTInterrupt(void);
void startup(void);
void default_interrupt(void);

// Vector table
uint32_t *VectorTable[] __attribute__((section("vector_table"))) = {
	(uint32_t *) STACK_TOP,			// Initial Stack Pointer (SP)
	(uint32_t *) startup,			// Reset Handler function
	(uint32_t *) default_interrupt,	// Non maskable interrupt (NMI)
	(uint32_t *) default_interrupt,	// Hard fault (all class of fault)
	(uint32_t *) default_interrupt,	// Memory management
	(uint32_t *) default_interrupt,	// Bus fault (prefetch fault, memory access fault)
	(uint32_t *) default_interrupt,	// Usage fault (undefined instruction or illegal state)
	0,									// * (Reserved)
	0,									// *
	0,									// *
	0,									// *
	(uint32_t *) default_interrupt,	// SVCall (system service call via SWI instruction)
	(uint32_t *) default_interrupt,	// Debug Monitor
	0,									// *
	(uint32_t *) default_interrupt,	// PendSV (Pendable request for system service)
	(uint32_t *) default_interrupt,	// SysTick (System tick timer)

	/* 0  */ (uint32_t *) default_interrupt,	// WWDG (Window watchdog interrupt)
	/* 1  */ (uint32_t *) default_interrupt,	// PVD (PVD through EXTI line detection interrupt)
	/* 2  */ (uint32_t *) default_interrupt,	// Tamper interrupt
	/* 3  */ (uint32_t *) default_interrupt,	// RTC global interrupt
	/* 4  */ (uint32_t *) default_interrupt,	// Flash global interrupt
	/* 5  */ (uint32_t *) default_interrupt,	// RCC global interrupt
	/* 6  */ (uint32_t *) default_interrupt,	// EXTI0 (EXTI Line0 interrupt)
	/* 7  */ (uint32_t *) default_interrupt,	// EXTI1
	/* 8  */ (uint32_t *) default_interrupt,	// EXTI2
	/* 9  */ (uint32_t *) default_interrupt,	// EXTI3
	/* 10 */ (uint32_t *) default_interrupt,	// EXTI4
	/* 11 */ (uint32_t *) default_interrupt,	// DMA1_Channel1 (DMA 1 Channel 1 global interrupt)
	/* 12 */ (uint32_t *) default_interrupt,	// DMA1_Channel2
	/* 13 */ (uint32_t *) default_interrupt,	// DMA1_Channel3
	/* 14 */ (uint32_t *) default_interrupt,	// DMA1_Channel4
	/* 15 */ (uint32_t *) default_interrupt,	// DMA1_Channel5
	/* 16 */ (uint32_t *) default_interrupt,	// DMA1_Channel6
	/* 17 */ (uint32_t *) default_interrupt,	// DMA1_Channel7
	/* 18 */ (uint32_t *) default_interrupt,	// ADC1_2 (ADC 1 and 2 global interrupt)
	/* 19 */ (uint32_t *) default_interrupt,	// USB_HP_CAN_TX (USB High Priority or CAN TX interrupts)
	/* 20 */ (uint32_t *) default_interrupt,	// USB_LP_CAN_RX0 (USB Low Priority or CAN RX0 interrupts)
	/* 21 */ (uint32_t *) default_interrupt,	// CAN_RX1 (CAN RX1 interrupt)
	/* 22 */ (uint32_t *) default_interrupt,	// CAN_SCE (CAN SCE interrupt)
	/* 23 */ (uint32_t *) default_interrupt,	// EXTI9_5 (EXTI Line[9:5] interrupts)
	/* 24 */ (uint32_t *) default_interrupt,	// TIM1_BRK (TIM1 break interrupt)
	/* 25 */ (uint32_t *) default_interrupt,	// TIM1_UP (TIM1 update interrupt)
	/* 26 */ (uint32_t *) default_interrupt,	// TIM1_TRG_COM (TIM1 trigger and commutation interrupts)
	/* 27 */ (uint32_t *) default_interrupt,	// TIM1_CC (TIM1 capture compare interrupt)
	/* 28 */ (uint32_t *) default_interrupt,	// TIM2
	/* 29 */ (uint32_t *) default_interrupt,	// TIM3
	/* 30 */ (uint32_t *) default_interrupt,	// TIM4
	/* 31 */ (uint32_t *) default_interrupt,	// I2C1_EV (Event interrupt)
	/* 32 */ (uint32_t *) default_interrupt,	// I2C1_ER (Error interrupt)
	/* 33 */ (uint32_t *) default_interrupt,	// I2C2_EV (Event interrupt)
	/* 34 */ (uint32_t *) default_interrupt,	// I2C2_ER (Error interrupt)
	/* 35 */ (uint32_t *) default_interrupt,	// SPI1
	/* 36 */ (uint32_t *) default_interrupt,	// SPI2
	/* 37 */ (uint32_t *) USARTInterrupt,		// USART1
	/* 38 */ (uint32_t *) default_interrupt,	// USART2
	/* 39 */ (uint32_t *) default_interrupt,	// USART3
	/* 40 */ (uint32_t *) default_interrupt,	// EXTI15_10 (EXTI Line[15:10] interrupts)
	/* 41 */ (uint32_t *) default_interrupt,	// RTCAlarm (RTC alarm through EXTI line interrupt)
	/* 42 */ (uint32_t *) default_interrupt,	// 
	/* 43 */ (uint32_t *) default_interrupt,	// 

	// TODO: Copy vector table to ram so we can modify interrupts inside of functions
};

bool led_state = false;


bool GetRegisterBit(uint32_t *reg, uint8_t bit){
	unsigned int test = *reg;
	return ((test >> bit) & 1);
}

void default_interrupt(void){}


void startup(void){
	
	volatile unsigned int *reg = (unsigned int *)RCC_APB2ENR; // RCC_APB2ENR

	GPIOEnable(GPIO_PORT_A);
	GPIOEnable(GPIO_PORT_C);

/* --- CHANGE CLOCK SPEED --- */
	// Enable the HSE and wait for it to be stable (HSERDY)
	// reg = (unsigned int *)RCC_CR;
	// *reg |= 1 << 16;
	// while((((*reg) >> 17) & 1) == 0);

	// Make sure the PLL is disabled before changing its values
	// reg = (unsigned int *)RCC_CR;
	// *reg &= (0xffffffff ^ (1 << 24));

	// Set PLL source and set PLL speed
	// reg = (unsigned int *)RCC_CFGR;
	// unsigned int tmp = *reg;
	// // tmp &= 0xff00ffff; // Set PLLMUL to 9x and PLLSRC to HSE
	// tmp &= 0xff00ffff; // Set PLLMUL to 9x and PLLSRC to HSE
	// tmp |= 0x00090000;
	// *reg = tmp;

	// Enable PLL
	// reg = (unsigned int *)RCC_CR;
	// *reg |= 1 << 24; // Enable the PLL (Set bit PLLON)
	// while((((*reg) >> 25) & 1) == 0); // Wait for PLLRDY


	// Switch system clock to PLL
	// reg = (unsigned int *)RCC_CFGR;
	// // // unsigned int tmp = *reg;
	// tmp &= 0xfffffffc; // Set SW (System clock switch) to use PLL
	// tmp |= 0x00000002;
	// *reg = tmp;
	// GPIOEnable(GPIO_PORT_A);
	// GPIOEnable(GPIO_PORT_C);

	// GPIOSetPinMode(GPIO_PORT_C, 13, GPIO_MODE_OUTPUT_10MHZ);
	// GPIOWrite(GPIO_PORT_C, 13, HIGH);
/* --- SETUP USB --- */


/* --- SETUP PC13 (LED) --- */
	GPIOSetPinMode(GPIO_PORT_C, 13, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_GP_PUSHPULL);

	// Lock the mode (output, push-pull) of PC13
	// reg = (unsigned int *)GPIOC_LCKR;
	// *reg &= 0x00002000;
	// *reg |= 0x00010000;

	// Set LED to on
	// GPIOWrite(GPIO_PORT_C, 13, HIGH);


	// bool fast = false;

	// reg = (unsigned int *)USART_SR;
	// USART tmpusart;
	// tmpusart.SR = *reg;
/* --- ENABLE USART 1 --- */
	// Set Tx pin as output alternate function push-pull
	GPIOSetPinMode(GPIO_PORT_A, 9, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL);
	
	// Set Rx pin as input pull-up
	GPIOSetPinMode(GPIO_PORT_A, 10, GPIO_MODE_INPUT, GPIO_CONFIG_INPUT_FLOATING);

	reg = (unsigned int *)RCC_APB2ENR;
	*reg |= (1 << 14); // Enable USART clock


	// Set baud rate
	USARTSetBaud(8000000, 9600); // Set BRR register (baud rate)

	reg = (unsigned int *)USART_CR1;
	USART CR1_reg;
	CR1_reg.data = *reg;
	
	CR1_reg.CR1.UE = true;
	CR1_reg.CR1.TE = true;
	CR1_reg.CR1.RE = true;
	CR1_reg.CR1.RXNEIE = true;
	CR1_reg.CR1.TXEIE = true;
	// CR1_reg.CR1.TCIE = true;
	/* -- OR -- */
	// CR1_reg.data |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_TCIE;
	// CR1_reg.data |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_TXEIE;
	// CR1_reg.data |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_TCIE;
	// CR1_reg.data |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

	*reg = CR1_reg.data;

	// Enable the usart interrupt in the NVIC
	reg = (unsigned int *)(NVIC + 0x4); // Register NVIC_ISER1
	*reg |= (1 << 5);

	// Send an a char
	reg = (unsigned int *)USART_DR;
	// char tmpchar;
	// tmpchar = *reg;
	// *reg = 'a';
	reg = (unsigned int *)USART_SR;
	*reg &= (0xffffffff ^ (1 << 7));
	
	// USARTWriteByte('g');

	while(1){
		// Delay
		// for(int i = 0; i < 2; i++){}

		// memcpy(USART1_buffer_send[usart_buffer_send_head], "test", 4);
		// USART1_buffer_send[usart_buffer_send_head++] = 'n';
		// USART1_buffer_send[usart_buffer_send_head++] = 'e';
		// USART1_buffer_send[usart_buffer_send_head++] = 'x';
		// USART1_buffer_send[usart_buffer_send_head++] = 't';
		// USARTWrite("TESing!\n");
		// char array[24] = "This is a test string!\n";
		// for(int i = 1; i < 23; i++){
		// 	USART1_buffer_send[usart_buffer_send_head++] = array[i];
		// }
		// USART usart;
		// reg = (unsigned int *)USART_SR;
		// usart.data = *reg;
		// while(!usart.SR.TXE);
		// reg = (unsigned int *)USART_DR;
		// *reg = array[0];
		// *reg = 'f';

		// if(led_state == LOW){
		// 	GPIOWrite(GPIO_PORT_C, 13, LOW);
		// 	for(int i = 0; i < 125000; i++){}
		// 	GPIOWrite(GPIO_PORT_C, 13, HIGH);
		// 	led_state = HIGH;
		// }

	}
	
}