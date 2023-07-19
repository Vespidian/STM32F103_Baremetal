#include "stdlib.h"
#include "stm32f103xb.h"
#include "rcc.h"

// RCC *rcc = (RCC *)RCC_ADDR;

// uint32_t hse_oscillator_speed = 8000000;

// uint32_t frequency = 48000000;

void SetClock_48MHz(){

	// Set flash latency for higher clock speed
	// reg = (unsigned int *)FLASH_ACR;
	// FLASH flash;
	// FLASH->data = *reg;
	FLASH->ACR |= FLASH_ACR_LATENCY_2;
	// *reg = flash.data;

	// Enable the HSE and wait for it to be stable (HSERDY), disable the PLL so we can modify it
	RCC->CR |= RCC_CR_HSEON;
	RCC->CR &= ~RCC_CR_PLLON;
	RCC->CR |= RCC_CR_CSSON;
	// while((((*reg) >> 17) & 1) == 0);
	while((RCC->CR & RCC_CR_HSERDY) == 0);

	// Set PLL source and set PLL speed
	RCC->CFGR |= RCC_CFGR_PLLSRC; // Src set to HSE
	RCC->CFGR |= RCC_CFGR_PLLMULL6; // Multiplier x6
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // Set APB1 divider to not divide

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for PLLRDY

	// Switch system clock to PLL
	// RCC->CFGR = RCC_CFGR_SW_PLL;
}

/**
 * Sets the frequency multiplier of the pll to mul or as close as possible to it (72 Mhz max)
 * min value of mul is 2
*/
// void RCCSetPLLMul(uint8_t mul){
//     if(mul <= 1){
//         return;
//     }

//     if(rcc->CFGR.SWS == 0b10){ // Check if PLL is the current system clock
       
//         // If it is, switch the sysclk to HSI
//         RCCSetSystemClockSrc(SYS_CLOCK_HSI);
//     }

//     // Make sure the pll output never exceeds 72 Mhz (Only possible with HSE)
//     if(rcc->CFGR.PLLSRC == SYS_CLOCK_HSE){

//         // Set a max number of loops so we dont loop indefinitely
//         uint8_t iter = 0;
//         do{
//             uint32_t pll_freq = hse_oscillator_speed / (rcc->CFGR.PLLXTPRE + 1) * (mul);
//             if(pll_freq <= 72000000){
//                 break;
//             }
//             iter++;
//         }while(iter < 8);
//     }

//     rcc->CFGR.PLLMUL = mul - 2;
// }

// void RCCSetPLLSrc(){

// }

// bool RCCSetSysClkState(SYS_CLK clk, bool state){
//     bool ret = false;

//     // Make sure we arent trying to set the state of the current system clock
//     if(rcc->CFGR.SWS != clk){
//         switch(clk){
//             case SYS_CLOCK_HSI:

//                 // Set and wait for the clock to be ready
//                 rcc->CR.HSION = state;
//                 while(!rcc->CR.HSIRDY);

//             break;

//             case SYS_CLOCK_HSE:

//                 // Set and wait for the clock to be ready
//                 rcc->CR.HSEON = state;
//                 while(!rcc->CR.HSERD);
                
//             break;

//             case SYS_CLOCK_PLL:

//                 // Set and wait for the clock to be ready
//                 rcc->CR.PLLON = state;
//                 while(!rcc->CR.PLLRDY);

//             break;
//         }


//         ret = true;
//     }

//     return ret;
// }

// void RCCSetSystemClockSrc(SYS_CLK clk){
    
//     // Turn on 'clk' 
//     // We can ignore the return of this function since it just tells us 
//     // whether or not the specified clock is already the system clock
//     (void)RCCSetSysClkState(clk, true);

//     // Set sysclk to 'clk'
//     rcc->CFGR.SW = clk;

// }

// void RCCDefineHSEOscillatorFreq(uint32_t frequency){
//     hse_oscillator_speed = frequency;
// }

// void RCCSetAFIO(bool is_enabled){
// 	rcc->APB2ENR.AFIOEN = 1;
// }