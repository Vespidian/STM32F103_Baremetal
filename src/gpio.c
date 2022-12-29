#include "main.h"
#include "gpio.h"


// #define GPIOC_CRH 		0x40011004
// #define GPIOC_ODR 		0x4001100C
// #define GPIOC_BSRR	 	0x40011010
// #define GPIOC_BRR	 	0x40011014
// #define GPIOC_LCKR	 	0x40011018

void GPIOEnable(GPIO_PORTS port){
	unsigned int *reg = (unsigned int *)RCC_APB2ENR;
	*reg &= (1 << ((port >> 10) & 0b1111)) ^ 0xffffffff; // TODO: Is this line necessary?
	*reg |= 1 << ((port >> 10) & 0b1111);
}

void GPIOSetPinMode(GPIO_PORTS port, unsigned char pin, GPIO_MODES mode, GPIO_CONFIGS config){
	// Use CRL for pins 0-7 and CRH for pins 8-15
	unsigned int *reg = (unsigned int *)(port + ((pin > 7) * 4));
	
	// Clear the mode bits for 'pin'
	*reg &= (0xffffffff ^ (0b1111 << ((pin % 8) * 4)));

	// Set the mode bits how we want
	*reg |= (((config << 2) | mode) << ((pin % 8) * 4));

	// TODO: possibly lock the mode now as well
}

void GPIOWrite(GPIO_PORTS port, unsigned char pin, bool state){
	// BSRR to set a pin HIGH and BRR to set a pin LOW (set and reset)
	unsigned int *reg = (unsigned int *)(port + (state ? GPIO_BSRR : GPIO_BRR));

	*reg |= (1 << pin);
}

void GPIOSetPullDirection(GPIO_PORTS port, unsigned char pin, bool pull){
	unsigned int *reg = (unsigned int *)(port + GPIO_ODR);
	*reg &= (0xffffffff ^ (1 << pin));
	*reg |= (pull << pin);
}


// bool GPIORead(GPIO_PORTS port, unsigned char pin){

// }

// uint16_t GPIOAnalogRead(GPIO_PORTS port, unsigned char pin){

// }