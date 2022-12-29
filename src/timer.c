#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h" // TMP

typedef union Timer1{
	uint32_t data;

	// Control regster 1
	struct CR1{
		uint32_t CEN		: 1;	// Counter enable
		uint32_t UDIS		: 1;	// Update disable
		uint32_t URS		: 1;	// Update request source
		uint32_t OPM		: 1;	// One-pulse mode
		uint32_t DIR		: 1;	// Count Direction
		uint32_t CMS		: 2;	// Center-aligned mode selection
		uint32_t ARPE		: 1;	// Auto-reload preload enable
		uint32_t CKD		: 2;	// Clock division
		uint32_t 			: 6;	// .
	}CR1;

	// DMA / Interrupt enable register
	struct DIER{
		uint32_t UIE		: 1; // Update interrupt enable
		uint32_t CC1IE		: 1; // Capture / compare interrupt enable
		uint32_t CC2IE		: 1; // Capture / compare interrupt enable
		uint32_t CC3IE		: 1; // Capture / compare interrupt enable
		uint32_t CC4IE		: 1; // Capture / compare interrupt enable
		uint32_t COMIE		: 1; // COM interrupt enable
		uint32_t TIE		: 1; // Trigger interrupt enable
		uint32_t BIE		: 1; // Break interrupt enable

		uint32_t UDE		: 1; // Update DMA request enable
		uint32_t CC1DE		: 1; // Capture / compare DMA request enable
		uint32_t CC2DE		: 1; // Capture / compare DMA request enable
		uint32_t CC3DE		: 1; // Capture / compare DMA request enable
		uint32_t CC4DE		: 1; // Capture / compare DMA request enable
		uint32_t COMDE		: 1; // COM DMA request enable
		uint32_t TDE		: 1; // Trigger DMA request enable
		uint32_t 			: 1; // .
	}DIER;

	// Status register
	struct SR{
		uint32_t UIF		: 1;	// Update interrupt flag
		uint32_t CC1IF		: 1;	// Capture / compare 1 interrupt flag
		uint32_t CC2IF		: 1;	// Capture / compare 2 interrupt flag
		uint32_t CC3IF		: 1;	// Capture / compare 3 interrupt flag
		uint32_t CC4IF		: 1;	// Capture / compare 4 interrupt flag
		uint32_t 			: 1;	// .
		uint32_t TIF		: 1;	// Trigger interrupt flag
		uint32_t 			: 2;	// .
		uint32_t CC1OF		: 1;	// Capture / compare 1 overcapture flag
		uint32_t CC2OF		: 1;	// Capture / compare 2 overcapture flag
		uint32_t CC3OF		: 1;	// Capture / compare 3 overcapture flag
		uint32_t CC4OF		: 1;	// Capture / compare 4 overcapture flag
		uint32_t 			: 3;	// .
	}SR;

	// Event generation register
	struct EGR{
		uint32_t UG			: 1;	// Update generation
		uint32_t CC1G		: 1;	// Capture / compare 1 generation
		uint32_t CC2G		: 1;	// Capture / compare 2 generation
		uint32_t CC3G		: 1;	// Capture / compare 3 generation
		uint32_t CC4G		: 1;	// Capture / compare 4 generation
		uint32_t 			: 1;	// .
		uint32_t TG			: 1;	// Trigger generation
		uint32_t 			: 9;	// .
	}EGR;

	// Capture / compare mode register
	struct CCMR_OUTPUT_COMPARE{
		uint32_t CC1S		: 2;	// Capture / compare 1 selection
		uint32_t OC1FE		: 1;	// Fast enable
		uint32_t OC1PE		: 1;	// Preload enable
		uint32_t OC1M		: 3;	// Output compare mode
		uint32_t OC1CE		: 1;	// Clear enable

		uint32_t CC2S		: 2;	// Capture / compare 1 selection
		uint32_t OC2FE		: 1;	// Fast enable
		uint32_t OC2PE		: 1;	// Preload enable
		uint32_t OC2M		: 3;	// Output compare mode
		uint32_t OC2CE		: 1;	// Clear enable
	}CCMR_OUTPUT_COMPARE;
	struct CCMR_INPUT_CAPTURE{
		uint32_t CC1S		: 2;	// Capture / compare 1 selection
		uint32_t IC1PSC		: 2;	// Capture prescaler
		uint32_t IC1F		: 4;	// Capture filter

		uint32_t CC2S		: 2;	// Capture / compare 1 selection
		uint32_t IC2PSC		: 2;	// Capture prescaler
		uint32_t IC2F		: 4;	// Capture filter
	}CCMR_INPUT_CAPTURE;

	struct CCER{
		uint32_t CC1E		: 1;	// 
		uint32_t CC1P		: 1;	// 
		uint32_t CC1NE		: 1;	// 
		uint32_t CC1NP		: 1;	// 
	}CCER;

	uint16_t CNT;	// Timer counter value
	uint16_t ARR;	// Value to be loaded into the actual auto-reload register
	uint16_t CCR;	/* When CC is configered as:
						Output: Value to be compared to CNT register
						Input: Value most recently captured (read only)
					*/

	struct BDTR{
		uint32_t DTG		: 8;	// Dead-time generator setup
		uint32_t LOCK		: 2;	// Lock configuration
		uint32_t OSSI		: 1;	// Off-state selection for Idle mode
		uint32_t OSSR		: 1;	// Off-state selection for Run mode
		uint32_t BKE		: 1;	// Break enable
		uint32_t BKP		: 1;	// Break polarity
		uint32_t AOE		: 1;	// Automatic output enable
		uint32_t MOE		: 1;	// Main output enable
	}BDTR;
	

}Timer1;

void TimerInit(){
	// Set the PWM output pin
	GPIOSetPinMode(GPIO_PORT_A, 8, GPIO_MODE_OUTPUT_2MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL); 
	// GPIOSetPinMode(GPIO_PORT_A, 12, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL); 

	Timer1 tim;
	volatile unsigned int *reg = 0;

	// Enable the usart interrupt in the NVIC
	reg = (unsigned int *)(NVIC); // Register NVIC_ISER1
	*reg |= (1 << 27);

	// Disable the timer
	reg = (unsigned int *)TIMER1_ADDR_CR1;
	tim.data = *reg;
	tim.CR1.CEN = 0;
	*reg = tim.data;

	// Enable the TIM1 peripheral
	reg = (unsigned int *)RCC_APB2ENR;
	*reg |= (1 << 11); // Set TIM1EN
	*reg |= 1; // Set AFIOEN

	reg = (unsigned int *)TIMER1_ADDR_CCER;
	tim.data = *reg;
	tim.CCER.CC1E = 1;
	*reg = tim.data;

	// Set timer to output compare mode and pwm mode 1
	reg = (unsigned int *)(TIMER1_ADDR_CCMR1);
	tim.data = *reg;
	tim.CCMR_OUTPUT_COMPARE.CC1S = 0; 	// Output compare mode
	tim.CCMR_OUTPUT_COMPARE.OC1M = TIMER_OC_MODE_PWM_1;
	// tim.CCMR_OUTPUT_COMPARE.OC1M = TIMER_OC_MODE_TOGGLE;
	tim.CCMR_OUTPUT_COMPARE.OC1PE = 1; 	// Enable preload
	*reg = tim.data;

	// Enable auto reload preload
	reg = (unsigned int *)(TIMER1_ADDR_CR1);
	tim.data = *reg;
	tim.CR1.ARPE = 1;
	*reg = tim.data;

	// Enable output compare ch1 interrupt
	reg = (unsigned int *)(TIMER1_ADDR_DIER);
	tim.data = *reg;
	tim.DIER.CC1IE = 1;
	*reg = tim.data;

	// Enable update generation
	reg = (unsigned int *)(TIMER1_ADDR_EGR);
	tim.data = *reg;
	tim.EGR.UG = 1;
	tim.EGR.CC1G = 1;
	*reg = tim.data;

	// Enable main output (Only for OC mode)
	reg = (unsigned int *)(TIMER1_ADDR_BDTR);
	tim.data = *reg;
	tim.BDTR.MOE = 1;
	*reg = tim.data;

	// Set the timer's prescaler
	reg = (unsigned int *)TIMER1_ADDR_PSC;
	*reg = 32;

	// Set the timer's reset value
	reg = (unsigned int *)TIMER1_ADDR_ARR;
	*reg = 128;

	// Set the PWM duty cycle
	reg = (unsigned int *)TIMER1_ADDR_CCR1;
	*reg = 1;

	// Enable the timer
	reg = (unsigned int *)TIMER1_ADDR_CR1;
	tim.data = *reg;
	tim.CR1.CEN = 1;
	*reg = tim.data;
}

void Tim1Ch1_IRQ(){
	unsigned int *reg = (unsigned int *)(TIMER1_ADDR_SR);
	Timer1 tim;
	tim.data = *reg;

	// if(tim.SR.CC1IF == 1){
	// 	GPIOWrite(GPIO_PORT_C, 13, led_state);
	// 	led_state = !led_state;
	// }

	tim.SR.CC1IF = 0;
	*reg = tim.data;
}

// (unsigned int *)TIMER1_ADDR_CNT