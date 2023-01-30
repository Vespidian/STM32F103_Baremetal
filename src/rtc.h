#ifndef RTC_H_
#define RTC_H_

#define RTC_BASE 0x40002800
// #define RTC_CRH 	RTC_BASE + 0x00
// #define RTC_CRL 	RTC_BASE + 0x04
// #define RTC_PRLH 	RTC_BASE + 0x08
// #define RTC_PRLL 	RTC_BASE + 0x0C
#define RTC_CRH 	0x40002800
#define RTC_CRL 	0x40002804

#define RTC_PRLH 	0x40002808
#define RTC_PRLL 	0x4000280C

#define RTC_CNTH 	0x40002818
#define RTC_CNTL 	0x4000281C

typedef union RTC{
	uint32_t data;

	// RTC control register high
	struct CRH{
		uint8_t SECIE	: 1; // Second interrupt enable
		uint8_t ALRIE	: 1; // Alarm interrupt enable
		uint8_t OWIE	: 1; // Overflow interrupt enable
		uint16_t 		:13; // .
	}CRH;

	// RTC control register low
	struct CRL{
		uint8_t SECF	: 1; // Second flag
		uint8_t ALRF	: 1; // Alarm flag
		uint8_t OWF		: 1; // Overflow flag
		uint8_t RSF		: 1; // Registers synchronized flag
		uint8_t CNF		: 1; // Configuration flag
		uint8_t RTOFF	: 1; // RTC operation OFF
		uint16_t		:10; // .
	}CRL;

	uint16_t PRLH; // RTC prescaler reload value high
	uint16_t PRLL; // RTC prescaler reload value low


}RTC;

void InitRTC();
void RTCInterrupt();

unsigned int RTCGetTime();

#endif
