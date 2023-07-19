#include "global.h"
#include "stdlib.h"
#include "stm32f103xb.h"
#include "nvic.h"
#include "gpio.h"
#include "usart.h"

#include "usb.h"

void USBInit(void){
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    GPIOSetPinMode(GPIO_PORT_A, 11, GPIO_MODE_OUTPUT_50MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL); // D-
    GPIOSetPinMode(GPIO_PORT_A, 12, GPIO_MODE_OUTPUT_50MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL); // D+

    RCC->APB1ENR |= RCC_APB1ENR_USBEN;

    // USB_BTABLE_BTABLE
	NVICEnableInterrupt(USB_LP_IRQn);
	NVICEnableInterrupt(USB_HP_IRQn);
	NVICEnableInterrupt(USBWakeUp_IRQn);

    USB->CNTR &= ~USB_CNTR_PDWN;

    // Wait 1μs until clock is stable
    SysTick->CTRL = 0;
    SysTick->LOAD = 48;
    SysTick->VAL = 0;
    SysTick->CTRL = 1;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    // SysTick->CTRL = 0;

    USB->CNTR &= ~USB_CNTR_FRES;
    USB->ISTR = 0;

    USB->CNTR |= USB_CNTR_RESETM | USB_CNTR_CTRM | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_ESOFM;
}

void USBInterrupt(){
    USARTWrite("\nUSB Interrupt triggered\n");
}