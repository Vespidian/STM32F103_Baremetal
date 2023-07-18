#include "global.h"
#include "stdlib.h"
#include "rcc.h"
#include "nvic.h"
#include "stm32f103xb.h"

#include "rtc.h"


void InitRTC(){
	// PWR *pwr = (PWR *)PWR_CR;
	// BKP *bkp = (BKP *)BKP_RTCCR;

	// Enable power and backup interfaces
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// Disable backup register write protection
	PWR->CR = PWR_CR_DBP;

	// Set RTC calibration value
	// bkp->RTCCR.CAL = 25; // Adjusted by 62 seconds per month

	RCC->BDCR |= RCC_BDCR_LSEON;
	RCC->BDCR |= RCC_BDCR_RTCEN;
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE; // LSE clock selected as RTC clock source
	RCC->BDCR |= RCC_BDCR_RTCSEL;
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0); // Wait for LSE to be ready

	// Enter configuration mode
	RTC->CRL |= RTC_CRL_CNF;

	RTC->CRH |= RTC_CRH_ALRIE;
	RTC->CRH |= RTC_CRH_SECIE;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed


	/* --- Set up RTC prescaler --- */
	uint32_t prescaler = 32767;
	
	// Set RTC PRL high bits
	RTC->PRLH = prescaler >> 16;

	// Set RTC PRL low bits
	RTC->PRLL = prescaler & 0xffff;


	/* --- Initialize RTC Counter --- */
	RTC->CNTH = 0x0000;

	// Set RTC PRL low bits
	RTC->CNTL = 0x0000;

	/* --- Initialize Alarm --- */
	RTC->ALRH = 0x0000;

	// Set RTC ALR low bits
	RTC->ALRL = 10;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed


	// Exit configuration mode and wait until the configuration write is done
	RTC->CRL &= ~RTC_CRL_CNF;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed

	// Enable the rtc interrupt in the NVIC
	NVICEnableInterrupt(3);

}

unsigned int RTCGetTime(){
	unsigned int time;

	time = RTC->CNTH << 16;
	time |= RTC->CNTL & 0xffff;

	return time;
}

void RTCSetCounter(uint32_t value){

	// Enter configuration mode
	RTC->CRL |= RTC_CRL_CNF;

	/* --- Initialize RTC Counter --- */
	RTC->CNTH = value >> 16;

	// Set RTC PRL low bits
	RTC->CNTL = value & 0xffff;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed

	// Exit configuration mode and wait until the configuration write is done
	RTC->CRL &= ~RTC_CRL_CNF;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed
}

void RTCSetAlarm(uint32_t value){

	// Enter configuration mode
	RTC->CRL |= RTC_CRL_CNF;

	/* --- Initialize Alarm --- */
	RTC->ALRH = value >> 16;

	// Set RTC ALR low bits
	RTC->ALRL = value & 0xffff;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed

	// Exit configuration mode and wait until the configuration write is done
	RTC->CRL &= ~RTC_CRL_CNF;

	while((RTC->CRL & RTC_CRL_RTOFF) == 0); // Wait for the RTC write operation to be completed
}

unsigned int RTCGetAlarm(){
	unsigned int alarm;

	alarm = RTC->ALRH << 16;
	alarm |= RTC->ALRL & 0xffff;

	return alarm;
}

#include "usart.h"
// #include "gpio.h"
bool alarm_flag = false;
void RTCInterrupt(){
	if(RTC->CRL & RTC_CRL_ALRF){
		alarm_flag = true;
		USARTWrite("Tick\n");
	}
	RTC->CRL &= ~RTC_CRL_ALRF;
	RTC->CRL &= ~RTC_CRL_SECF;
	RTC->CRL &= ~RTC_CRL_OWF;

	// GPIOWrite(GPIO_PORT_C, 13, led);
	// led = !led;
}