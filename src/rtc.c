#include "main.h"
#include "nvic.h"
#include "rtc.h"

void InitRTC(){
	RCC rcc;
	PWR pwr;
	BKP bkp;
	RTC rtc;
	unsigned int *reg;

	// Enable power and backup interfaces
    reg = (unsigned int *)RCC_APB1ENR;
	rcc.data = *reg;
		rcc.APB1ENR.BKPEN = 1;
		rcc.APB1ENR.PWREN = 1;
	*reg = rcc.data;

	// Disable backup register write protection
    reg = (unsigned int *)PWR_CR;
	pwr.data = *reg;
		pwr.CR.DBP = 1;
	*reg = pwr.data;

	// Set RTC calibration value
    reg = (unsigned int *)PWR_CR;
	bkp.data = *reg;
		bkp.RTCCR.CAL = 25; // Adjusted by 62 seconds per month
	*reg = bkp.data;

    reg = (unsigned int *)RCC_BDCR;
	rcc.data = *reg;
		rcc.BDCR.LSEON = 1;
		rcc.BDCR.RTCEN = 1;
		rcc.BDCR.RTCSEL = 0b01; // LSE clock selected as RTC clock source
		rcc.BDCR.BDRST = 1;
	*reg = rcc.data;

	while(((*(unsigned int *)RCC_BDCR >> 1) & 1) == 0); // Wait for LSE to be ready


	// Enter configuration mode
    reg = (unsigned int *)RTC_CRL;
	rtc.data = *reg;
		rtc.CRL.CNF = 1;
	*reg = rtc.data;

    // reg = (unsigned int *)RTC_CRH;
	// rtc.data = *reg;
	// 	rtc.CRH.ALRIE = 1;
	// *reg = rcc.data;

	while(((*(unsigned int *)RTC_CRL >> 5) & 1) == 0); // Wait for the RTC write operation to be completed


	/* --- Set up RTC prescaler --- */
	uint32_t prescaler = 32767;
	// Set RTC PRL high bits
    reg = (unsigned int *)RTC_PRLH;
	rtc.data = *reg;
		// rtc.PRLH = prescaler >> 16;
		rtc.PRLH = 0;
	*reg = rtc.data;

	// Set RTC PRL low bits
    reg = (unsigned int *)RTC_PRLL;
	rtc.data = *reg;
		// rtc.PRLL = prescaler & 0xffff;
		rtc.PRLL = prescaler;
	*reg = rtc.data;


	/* --- Initialize RTC Counter --- */
	reg = (unsigned int *)RTC_CNTH;
	*reg &= 0x0000;

	// Set RTC PRL low bits
    reg = (unsigned int *)RTC_CNTL;
	*reg &= 0x0000;

	while(((*(unsigned int *)RTC_CRL >> 5) & 1) == 0); // Wait for the RTC write operation to be completed


	// Exit configuration mode and wait until the configuration write is done
    reg = (unsigned int *)RTC_CRL;
	rtc.data = *reg;
		rtc.CRL.CNF = 0;
	*reg = rtc.data;

	while(((*(unsigned int *)RTC_CRL >> 5) & 1) == 0); // Wait for the RTC write operation to be completed

	// Enable the rtc interrupt in the NVIC
	// NVICEnableInterrupt(3);


}

unsigned int RTCGetTime(){
	unsigned int time;

	unsigned int *reg = (unsigned int *)RTC_CNTH;
	time = *reg << 16;

	reg = (unsigned int *)RTC_CNTL;
	time |= *reg & 0xffff;

	return time;
}

#include "usart.h"
#include "gpio.h"
bool led = false;
void RTCInterrupt(){
	RTC rtc;
	unsigned int *reg = (unsigned int *)RTC_CRL;
	rtc.data = *reg;
		rtc.CRL.ALRF = 0;
	*reg = rtc.data;

	GPIOWrite(GPIO_PORT_C, 13, led);
	led = !led;
}