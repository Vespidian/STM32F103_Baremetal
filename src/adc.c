#include "main.h"
#include "usart.h"

#include "adc.h"

#define ADC1 0x40012400
// #define ADC1_SR 	ADC1 + 0x00
// #define ADC1_CR1 	ADC1 + 0x04
// #define ADC1_CR2 	ADC1 + 0x08
// #define ADC1_DR 	ADC1 + 0x4C
#define ADC1_SR 	0x40012400
#define ADC1_CR1 	0x40012404
#define ADC1_CR2 	0x40012408
#define ADC1_DR 	0x4001244C

typedef union ADC{
	uint32_t data;

	struct SR{
		uint8_t AWD		: 1; // Analog watchdog flag
		uint8_t EOC		: 1; // End of conversion
		uint8_t JEOC	: 1; // Injected channel end of conversion
		uint8_t JSTRT	: 1; // Injected channel start flag
		uint8_t STRT	: 1; // Regular channel start flag
		uint32_t		: 27;// .
	}SR;

	struct CR1{
		uint8_t AWDCH	: 5; // Analog watchdog channel select bits
		uint8_t EOCIE	: 1; // Interrupt enable for EOC
		uint8_t AWDIE	: 1; // Analog watchdog interrupt enable
		uint8_t JEOCIE	: 1; // Interrupt enable for injected channels
		uint8_t SCAN	: 1; // Scan mode
		uint8_t AWDSGL	: 1; // Enable the watchdog on a single channel in scan mode
		uint8_t JAUTO	: 1; // Automatic injected group conversion
		uint8_t DISCEN	: 1; // Discontinuous mode on regular channels
		uint8_t JDISCEN	: 1; // Discontinuous mode on injected channels
		uint8_t DISCNUM	: 3; // Discontinuous mode channel count
		uint8_t DUALMOD	: 4; // Dual mode selection
		uint8_t 		: 2; // .
		uint8_t JAWDEN	: 1; // Analog watchdog enable on injected channels
		uint8_t AWDEN	: 1; // Analog watchdog enable on regular channels
		uint8_t 		: 8; // .
	}CR1;

	struct CR2{
		uint8_t ADON	: 1; // A/D converter ON / OFF
		uint8_t CONT	: 1; // Contiuous conversion
		uint8_t CAL		: 1; // A/D calibration
		uint8_t RSTCAL	: 1; // Reset calibration
		uint8_t 		: 4; // .
		uint8_t DMA		: 1; // Direct memory access mode
		uint8_t 		: 2; // .
		uint8_t ALIGN	: 1; // Data alignment
		uint8_t JEXTSEL	: 3; // External event select for injected group
		uint8_t JEXTTRIG: 1; // External trigger conversion mode for injected channels
		uint8_t 		: 1; // .
		uint8_t EXTSEL	: 3; // External event select for regular group
		uint8_t EXTTRIG	: 1; // External trigger conversion mode for regular channels
		uint8_t JSWSTART: 1; // Start conversion of injected channels
		uint8_t SWSTART	: 1; // Start conversion of regular channels
		uint8_t TSVREFE	: 1; // Temperature sensor and V_(REFINT) enable
		uint8_t 		: 8; // .
	}CR2;
}ADC;


void ADCInit(){
	// Enable ADC1 in the RCC
	ADC adc;
	unsigned int *reg;

	rcc->APB2ENR.ADC1EN = 1;
	rcc->CFGR.PPRE2 = 0b110; // ADC clock can be max 14MHz (Set to divide by 8)
	rcc->CFGR.ADCPRE = 0b010; // ADC clock can be max 14MHz (Set to divide by 6)

	USARTWrite("RCC enabled ADC\n");

	// Select trigger method and enable continuous conversion
	reg = (unsigned int *)ADC1_CR2;
	adc.data = *reg;
		adc.CR2.EXTTRIG = 1;
		adc.CR2.EXTSEL = 0b111;
		adc.CR2.SWSTART = 1;
		adc.CR2.CONT = 1;
		// adc.CR2.ADON = 1;
	*reg = adc.data;

	USARTWrite("ADC configured\n");

	// Enable the A/D converter
	reg = (unsigned int *)ADC1_CR2;
	adc.data = *reg;
		adc.CR2.ADON = 1;
	*reg = adc.data;

	USARTWrite("ADC turned on\n");

	// Start conversion
	reg = (unsigned int *)ADC1_CR2;
	adc.data = *reg;
		adc.CR2.SWSTART = 1;
	*reg = adc.data;

	USARTWrite("ADC now sampling\n");
	// Calibrate the ADC
	// reg = (unsigned int *)ADC1_CR2;
	// adc.data = *reg;
	// 	adc.CR2.CAL = 1;
	// *reg = adc.data;
	// while(((*reg >> 3) & 1) == 1);
}