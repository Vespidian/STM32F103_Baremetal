#ifndef PWR_H_
#define PWR_H_

#define PWR_CR			0x40007000
#define PWR_CSR			0x40007004

typedef union PWR{
	uint16_t data;

	struct {
		uint8_t LPDS	: 1; // Low-power deepsleep
		uint8_t PDDS	: 1; // Power down deepsleep
		uint8_t CWUF	: 1; // Clear wakeup flag
		uint8_t CSBF	: 1; // Clear standby flag
		uint8_t PVDE	: 1; // Programmable voltage detector enable
		uint8_t PLS		: 3; // PVD level selection
		uint8_t DBP		: 1; // Disable backup domain write protection
	}CR;

	struct CSR{
		uint8_t WUF		: 1; // Wakeup flag
		uint8_t SBO		: 1; // Standby flag
		uint8_t PVDO	: 1; // PVD output
		uint8_t 		: 5; // .
		uint8_t EWUP	: 1; // Enable WKUP pin
	}CSR;
}PWR;

#endif