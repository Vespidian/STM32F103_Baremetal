#include "main.h"
#include "nvic.h"
#include "rtc.h"

RTC *rtc = (RTC *)RTC_ADDR;

void InitRTC(){
	PWR *pwr = (PWR *)PWR_CR;
	BKP *bkp = (BKP *)BKP_RTCCR;

	// Enable power and backup interfaces
	rcc->APB1ENR.BKPEN = 1;
	rcc->APB1ENR.PWREN = 1;

	// Disable backup register write protection
	pwr->CR.DBP = 1;

	// Set RTC calibration value
	bkp->RTCCR.CAL = 25; // Adjusted by 62 seconds per month

	rcc->BDCR.LSEON = 1;
	rcc->BDCR.RTCEN = 1;
	rcc->BDCR.RTCSEL = 0b01; // LSE clock selected as RTC clock source
	rcc->BDCR.BDRST = 1;
	while(rcc->BDCR.LSERDY == 0); // Wait for LSE to be ready

	// Enter configuration mode
	rtc->CRL.CNF = 1;

	rtc->CRH.ALRIE = 1;
	rtc->CRH.SECIE = 1;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed


	/* --- Set up RTC prescaler --- */
	uint32_t prescaler = 32767;
	
	// Set RTC PRL high bits
	rtc->PRLH = prescaler >> 16;

	// Set RTC PRL low bits
	rtc->PRLL = prescaler & 0xffff;


	/* --- Initialize RTC Counter --- */
	rtc->CNTH = 0x0000;

	// Set RTC PRL low bits
	rtc->CNTL = 0x0000;

	/* --- Initialize Alarm --- */
	rtc->ALRH = 0x0000;

	// Set RTC ALR low bits
	rtc->ALRL = 10;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed


	// Exit configuration mode and wait until the configuration write is done
	rtc->CRL.CNF = 0;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed

	// Enable the rtc interrupt in the NVIC
	NVICEnableInterrupt(3);

}

unsigned int RTCGetTime(){
	unsigned int time;

	time = rtc->CNTH << 16;
	time |= rtc->CNTH & 0xffff;

	return time;
}

void RTCSetCounter(uint32_t value){

	// Enter configuration mode
	rtc->CRL.CNF = 1;

	/* --- Initialize RTC Counter --- */
	rtc->CNTH = value >> 16;

	// Set RTC PRL low bits
	rtc->CNTL = value & 0xffff;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed

	// Exit configuration mode and wait until the configuration write is done
	rtc->CRL.CNF = 0;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed
}

void RTCSetAlarm(uint32_t value){

	// Enter configuration mode
	rtc->CRL.CNF = 1;

	/* --- Initialize Alarm --- */
	rtc->ALRH = value >> 16;

	// Set RTC ALR low bits
	rtc->ALRL = value & 0xffff;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed

	// Exit configuration mode and wait until the configuration write is done
	rtc->CRL.CNF = 0;

	while(rtc->CRL.RTOFF == 0); // Wait for the RTC write operation to be completed
}

unsigned int RTCGetAlarm(){
	unsigned int alarm;

	alarm = rtc->ALRH << 16;
	alarm |= rtc->ALRL & 0xffff;

	return alarm;
}

#include "usart.h"
#include "gpio.h"
bool led = false;
bool alarm_flag = false;
void RTCInterrupt(){
	if(rtc->CRL.ALRF){
		alarm_flag = true;
		USARTWrite("Tick\n");
	}
	rtc->CRL.ALRF = 0;
	rtc->CRL.SECF = 0;
	rtc->CRL.OWF = 0;

	GPIOWrite(GPIO_PORT_C, 13, led);
	led = !led;
}