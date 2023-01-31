#include "main.h"
#include "nvic.h"

// NVIC addresses found in the Cortex-M3 programming manual
#define NVIC 0xE000E100

void NVICEnableInterrupt(uint8_t irq_num){
	unsigned int *reg = (unsigned int *)(NVIC + (irq_num / 8));
	*reg |= (1 << irq_num % 32);
}

void NVICDisableInterrupt(uint8_t irq_num){
	unsigned int *reg = (unsigned int *)(NVIC + (irq_num / 8));
	*reg &= ~(1 << irq_num % 32);
}