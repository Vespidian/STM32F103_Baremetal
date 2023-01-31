#include "main.h"
#include "nvic.h"
#include "rtc.h"

void InitRTC(){
	RCC rcc;
	RTC rtc;
	unsigned int *reg;

    reg = (unsigned int *)RCC_BDCR;
	rcc.data = *reg;
		rcc.BDCR.LSEON = 1;
	*reg = rcc.data;
	while(((*reg >> 1) & 1) == 0);
	rcc.BDCR.RTCEN = 1;
	rcc.BDCR.RTCSEL = 0b01; // LSE clock selected as RTC clock source
	// rcc.BDCR.BDRST = 1;
	// rcc.BDCR.RTCEN = 0;
	*reg = rcc.data;


    reg = (unsigned int *)RTC_CRH;
	rtc.data = *reg;
		rtc.CRH.ALRIE = 1;
	*reg = rcc.data;

	// Wait until previous write operation is done and enter configuration mode
    reg = (unsigned int *)RTC_CRL;
	while(((*reg >> 5) & 1) == 0);
	rtc.data = *reg;
		rtc.CRL.CNF = 1;
	*reg = rtc.data;


	uint32_t prescaler = 32767;
	// Set RTC PRL high bits
    reg = (unsigned int *)RTC_PRLH;
	rtc.data = *reg;
		rtc.PRLH = prescaler >> 16;
	*reg = rtc.data;

	// Set RTC PRL low bits
    reg = (unsigned int *)RTC_PRLL;
	rtc.data = *reg;
		rtc.PRLL = prescaler & 0xffff;
	*reg = rtc.data;


	// Exit configuration mode and wait until the configuration write is done
    reg = (unsigned int *)RTC_CRL;
	rtc.data = *reg;
		rtc.CRL.CNF = 0;
	*reg = rtc.data;
	while(((*reg >> 5) & 1) == 0);

	// Enable the rtc interrupt in the NVIC
	NVICEnableInterrupt(3);


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
void RTCInterrupt(){
	USARTWrite("tick\n");
}