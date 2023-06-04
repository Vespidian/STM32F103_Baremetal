#ifndef RTC_H_
#define RTC_H_

#define RTC_ADDR 	0x40002800

#define RTC_CRH 	0x40002800
#define RTC_CRL 	0x40002804

#define RTC_PRLH 	0x40002808
#define RTC_PRLL 	0x4000280C

#define RTC_CNTH 	0x40002818
#define RTC_CNTL 	0x4000281C

#define RTC_ALRH 	0x40002820
#define RTC_ALRL 	0x40002824

typedef struct RTC{
	// RTC control register high
	struct CRH{
		uint8_t SECIE	: 1; // Second interrupt enable
		uint8_t ALRIE	: 1; // Alarm interrupt enable
		uint8_t OWIE	: 1; // Overflow interrupt enable
		uint16_t 		: 13; // .
		uint16_t 		: 16; // .
	}CRH;

	// RTC control register low
	struct CRL{
		uint8_t SECF	: 1; // Second flag
		uint8_t ALRF	: 1; // Alarm flag
		uint8_t OWF		: 1; // Overflow flag
		uint8_t RSF		: 1; // Registers synchronized flag
		uint8_t CNF		: 1; // Configuration flag
		uint8_t RTOFF	: 1; // RTC operation OFF
		uint16_t 		:10; // .
		uint16_t 		:16; // .
	}CRL;

	uint32_t PRLH		: 4;	 // RTC prescaler reload value high
	uint32_t PRLL		: 16;	 // RTC prescaler reload value low
	uint32_t DIVH		: 16;	 // 
	uint32_t DIVL		: 16;	 // 
	uint32_t CNTH		: 16;	 // RTC counter register high
	uint32_t CNTL		: 16;	 // RTC counter register low
	uint32_t ALRH		: 16;	 // RTC alarm register high
	uint32_t ALRL		: 16;	 // RTC alarm register low
}RTC;

extern bool alarm_flag;

void InitRTC();
void RTCInterrupt();

unsigned int RTCGetTime();
void RTCSetCounter(uint32_t value);

unsigned int RTCGetAlarm();
void RTCSetAlarm(uint32_t value);

#endif
