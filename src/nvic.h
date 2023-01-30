#ifndef NVIC_H_
#define NVIC_H_

// NVIC addresses found in the Cortex-M3 programming manual
#define NVIC 0xE000E100
void NVICEnableInterrupt(uint8_t irq_num);
void NVICDisableInterrupt(uint8_t irq_num);

#endif