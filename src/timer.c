#include "main.h"
#include "nvic.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h" // TMP

#define TIMER1_ADDR	0x40012C00
#define TIMER2_ADDR 0x40000000
#define TIMER3_ADDR 0x40000400
#define TIMER4_ADDR 0x40000800
#define TIMER5_ADDR 0x40000c00

Timer *timer1 = (Timer *)TIMER1_ADDR;
Timer *timer2 = (Timer *)TIMER2_ADDR;
Timer *timer3 = (Timer *)TIMER3_ADDR;
Timer *timer4 = (Timer *)TIMER4_ADDR;
Timer *timer5 = (Timer *)TIMER5_ADDR;

// typedef enum TIM_TYPE{
// 	// Advanced control timers
// 	TIM1 	= 1 >> 0,
// 	TIM8 	= 1 >> 0,

// 	// General purpose timers (4 channels)
// 	TIM2 	= 1 >> 1,
// 	TIM3 	= 1 >> 1,
// 	TIM4 	= 1 >> 1,
// 	TIM5 	= 1 >> 1,

// 	// General purpose timers (2 channels)
// 	TIM9 	= 1 >> 2,
// 	TIM12 	= 1 >> 2,
	
// 	// General purpose timers (2 channels)
// 	TIM10 	= 1 >> 3,
// 	TIM11 	= 1 >> 3,
// 	TIM13 	= 1 >> 3,
// 	TIM14 	= 1 >> 3,

// 	// Basic timers
// 	TIM6 	= 1 >> 4,
// 	TIM7 	= 1 >> 4,
// }TIM_TYPE;

typedef enum TIM_ADDR{
	TIM1 = 0x40012C00, // T1
	TIM2 = 0x40000000, // T2
	TIM3 = 0x40000400, // T3
	TIM4 = 0x40000800, // T4
	TIM5 = 0x40000c00, // T5
	TIM6 = 0x40001000, // T6
	TIM7 = 0x40001400, // T7
	TIM8 = 0x40013400, // T8
	TIM9 = 0x40014C00, // T9
	TIM10 = 0x40015000, // T10
	TIM11 = 0x40015400, // T11
	TIM12 = 0x40001800, // T12
	TIM13 = 0x40001C00, // T13
	TIM14 = 0x40002000, // T14
}TIM_ADDR;

// static const uint32_t timer_addresses[14] = {
// 	0x40012C00, // T1
// 	0x40000000, // T2
// 	0x40000400, // T3
// 	0x40000800, // T4
// 	0x40000c00, // T5
// 	0x40001000, // T6
// 	0x40001400, // T7
// 	0x40013400, // T8
// 	0x40014C00, // T9
// 	0x40015000, // T10
// 	0x40015400, // T11
// 	0x40001800, // T12
// 	0x40001C00, // T13
// 	0x40002000, // T14
// };

// static const uint8_t timer_types[14] = {
// 	1 << 0, // T1
// 	1 << 1, // T2
// 	1 << 1, // T3
// 	1 << 1, // T4
// 	1 << 1, // T5
// 	1 << 4, // T6
// 	1 << 4, // T7
// 	1 << 0, // T8
// 	1 << 2, // T9
// 	1 << 3, // T10
// 	1 << 3, // T11
// 	1 << 2, // T12
// 	1 << 3, // T13
// 	1 << 3, // T14
// };

// static uint8_t TimerGetType(Timer *t){
// 	uint8_t type = 0;
// 	for(int i = 0; i < 14; i++){
// 		if(t == timer_addresses[i]){
// 			type = timer_types[i];
// 			break;
// 		}
// 	}
// 	return type;
// }

// void TimerSetMode(Timer t, ){}

void TimerSetEnable(Timer *t, bool state){
	switch((uint32_t)&t){
		case TIM1:
			rcc->APB2ENR.TIM1EN = state;
			break;
		case TIM2:
			rcc->APB1ENR.TIM2EN = state;
			break;
		case TIM3:
			rcc->APB1ENR.TIM3EN = state;
			break;
		case TIM4:
			rcc->APB1ENR.TIM4EN = state;
			break;
		case TIM5:
			rcc->APB1ENR.TIM5EN = state;
			break;
		case TIM6:
			rcc->APB1ENR.TIM6EN = state;
			break;
		case TIM7:
			rcc->APB1ENR.TIM7EN = state;
			break;
		case TIM8:
			rcc->APB2ENR.TIM8EN = state;
			break;
		case TIM9:
			rcc->APB2ENR.TIM9EN = state;
			break;
		case TIM10:
			rcc->APB2ENR.TIM10EN = state;
			break;
		case TIM11:
			rcc->APB2ENR.TIM11EN = state;
			break;
		case TIM12:
			rcc->APB1ENR.TIM12EN = state;
			break;
		case TIM13:
			rcc->APB1ENR.TIM13EN = state;
			break;
		case TIM14:
			rcc->APB1ENR.TIM14EN = state;
			break;
	}
}

void TimerPause(Timer *t){
	t->CR1.CEN = 0;
}
void TimerStart(Timer *t){
	t->CR1.CEN = 1;
}

inline void TimerSetCounter(Timer *t, uint16_t value){
	t->CNT = value;
}
inline uint16_t TimerGetCounter(Timer *t){
	return t->CNT;
}

inline void TimerSetPrescaler(Timer *t, uint16_t value){
	t->PSC = value;
}
inline uint16_t TimerGetPrescaler(Timer *t){
	return t->PSC;
}

inline void TimerSetReload(Timer *t, uint16_t value){
	t->ARR = value;
}
inline uint16_t TimerGetReload(Timer *t){
	return t->ARR;
}

void TimerInit(){
	// Set the PWM output pin
	GPIOSetPinMode(GPIO_PORT_A, 8, GPIO_MODE_OUTPUT_2MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL); 

	// Enable the tim1 interrupt in the NVIC
	NVICEnableInterrupt(27);

	// Disable the timer
	timer1->CR1.CEN = 0;
	// Enable the TIM1 peripheral
	rcc->APB2ENR.TIM1EN = 1;
	rcc->APB2ENR.AFIOEN = 1; // Enables the clock to control GPIO pins (should go in 'TimerSetMode')

	// Enable channel 1
	timer1->CCER.CC1E = 1;

	// Set timer to output compare mode and pwm mode 1
	timer1->CCMR1.OUTPUT_COMPARE.CC1S = 0; 	// Output compare mode
	timer1->CCMR1.OUTPUT_COMPARE.OC1M = TIMER_OC_MODE_PWM_1;
	timer1->CCMR1.OUTPUT_COMPARE.OC1PE = 1; 	// Enable preload
	// timer1.CCMR_OUTPUT_COMPARE.OC1M = TIMER_OC_MODE_TOGGLE;

	// Enable auto reload preload
	timer1->CR1.ARPE = 1;

	// Enable output compare ch1 interrupt
	timer1->DIER.CC1IE = 1;

	// Enable update generation
	timer1->EGR.UG = 1;
	timer1->EGR.CC1G = 1;

	// Enable main output (Only for OC mode) (should be set in 'TimerSetMode')
	timer1->BDTR.MOE = 1;

	// Set the timer's prescaler
	timer1->PSC = 32;

	// Set the timer's reset value
	timer1->ARR = 128;

	// Set the PWM duty cycle
	timer1->CCR1 = 1;

	// Enable the timer
	timer1->CR1.CEN = 1;
}

void Tim1Ch1_IRQ(){

	// if(timer1.SR.CC1IF == 1){
	// 	GPIOWrite(GPIO_PORT_C, 13, led_state);
	// 	led_state = !led_state;
	// }

	// timer1->SR.CC1IF = 0;
}

// (unsigned int *)TIMER1_ADDR_CNT