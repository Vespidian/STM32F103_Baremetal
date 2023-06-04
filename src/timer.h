#ifndef TIMER_H_
#define TIMER_H_

#define TIMER1_ADDR_CR1		0x40012C00
#define TIMER1_ADDR_DIER	0x40012C0C
#define TIMER1_ADDR_SR		0x40012C10
#define TIMER1_ADDR_EGR		0x40012C14
#define TIMER1_ADDR_CCMR1	0x40012C18
#define TIMER1_ADDR_CCER	0x40012C20
#define TIMER1_ADDR_BDTR	0x40012C44

#define TIMER1_ADDR_CNT		0x40012C24
#define TIMER1_ADDR_PSC		0x40012C28
#define TIMER1_ADDR_ARR		0x40012C2C
#define TIMER1_ADDR_CCR1	0x40012C34

typedef enum TIMER_OC_MODE{
	TIMER_OC_MODE_FROZEN			= 0b000,
	TIMER_OC_MODE_ACTIVE_ON_MATCH	= 0b001,
	TIMER_OC_MODE_INACTIVE_ON_MATCH	= 0b010,
	TIMER_OC_MODE_TOGGLE			= 0b011,
	TIMER_OC_MODE_FORCE_INACTIVE	= 0b100,
	TIMER_OC_MODE_FORCE_ACTIVE		= 0b101,
	TIMER_OC_MODE_PWM_1				= 0b110,
	TIMER_OC_MODE_PWM_2				= 0b111,
}TIMER_OC_MODE;

typedef struct Timer{
	// Control regster 1
	struct {
		uint32_t CEN		: 1;	// Counter enable
		uint32_t UDIS		: 1;	// Update disable
		uint32_t URS		: 1;	// Update request source
		uint32_t OPM		: 1;	// One-pulse mode
		uint32_t DIR		: 1;	// Count Direction
		uint32_t CMS		: 2;	// Center-aligned mode selection
		uint32_t ARPE		: 1;	// Auto-reload preload enable
		uint32_t CKD		: 2;	// Clock division
		uint32_t 			: 6;	// .
		uint32_t 			: 16;	// .
	}CR1;

	// Control register 2
	struct{
		uint32_t CCPC 		: 1;	// Capture / compare preload control
		uint32_t 	 		: 1;	// .
		uint32_t CCUS 		: 1;	// Capture / compare control update selection
		uint32_t CCDS 		: 1;	// Capture / compare DMA selection
		uint32_t MMS 		: 3;	// Master mode selection
		uint32_t TI1S 		: 1;	// TI1 selection
		uint32_t OIS1 		: 1;	// Output idle state 1 (depends on OISxN)
		uint32_t OIS1N 		: 1;	// Output idle state 1 
		uint32_t OIS2 		: 1;	// Output idle state 2 (depends on OISxN)
		uint32_t OIS2N 		: 1;	// Output idle state 2
		uint32_t OIS3 		: 1;	// Output idle state 3 (depends on OISxN)
		uint32_t OIS3N 		: 1;	// Output idle state 3
		uint32_t OIS4 		: 1;	// Output idle state 4
		uint32_t 	 		: 1;	// .
		uint32_t 	 		: 16;	// .
	}CR2;

	uint32_t SMCR; // Slave mode control register

	// DMA / Interrupt enable register
	struct {
		uint32_t UIE		: 1; // Update interrupt enable
		uint32_t CC1IE		: 1; // Capture / compare interrupt enable
		uint32_t CC2IE		: 1; // Capture / compare interrupt enable
		uint32_t CC3IE		: 1; // Capture / compare interrupt enable
		uint32_t CC4IE		: 1; // Capture / compare interrupt enable
		uint32_t COMIE		: 1; // COM interrupt enable
		uint32_t TIE		: 1; // Trigger interrupt enable
		uint32_t BIE		: 1; // Break interrupt enable

		uint32_t UDE		: 1; // Update DMA request enable
		uint32_t CC1DE		: 1; // Capture / compare DMA request enable
		uint32_t CC2DE		: 1; // Capture / compare DMA request enable
		uint32_t CC3DE		: 1; // Capture / compare DMA request enable
		uint32_t CC4DE		: 1; // Capture / compare DMA request enable
		uint32_t COMDE		: 1; // COM DMA request enable
		uint32_t TDE		: 1; // Trigger DMA request enable
		uint32_t 			: 17; // .
	}DIER;

	// Status register
	struct {
		uint32_t UIF		: 1;	// Update interrupt flag
		uint32_t CC1IF		: 1;	// Capture / compare 1 interrupt flag
		uint32_t CC2IF		: 1;	// Capture / compare 2 interrupt flag
		uint32_t CC3IF		: 1;	// Capture / compare 3 interrupt flag
		uint32_t CC4IF		: 1;	// Capture / compare 4 interrupt flag
		uint32_t 			: 1;	// .
		uint32_t TIF		: 1;	// Trigger interrupt flag
		uint32_t 			: 2;	// .
		uint32_t CC1OF		: 1;	// Capture / compare 1 overcapture flag
		uint32_t CC2OF		: 1;	// Capture / compare 2 overcapture flag
		uint32_t CC3OF		: 1;	// Capture / compare 3 overcapture flag
		uint32_t CC4OF		: 1;	// Capture / compare 4 overcapture flag
		uint32_t 			: 19;	// .
	}SR;

	// Event generation register
	struct {
		uint32_t UG			: 1;	// Update generation
		uint32_t CC1G		: 1;	// Capture / compare 1 generation
		uint32_t CC2G		: 1;	// Capture / compare 2 generation
		uint32_t CC3G		: 1;	// Capture / compare 3 generation
		uint32_t CC4G		: 1;	// Capture / compare 4 generation
		uint32_t 			: 1;	// .
		uint32_t TG			: 1;	// Trigger generation
		uint32_t 			: 24;	// .
	}EGR;

	// Capture / compare mode register 1
	union CCMR{
		// Output compare mode register
		struct{
			uint32_t CC1S	: 2;	// Capture / compare 1 selection
			uint32_t OC1FE	: 1;	// Fast enable
			uint32_t OC1PE	: 1;	// Preload enable
			uint32_t OC1M	: 3;	// Output compare mode
			uint32_t OC1CE	: 1;	// Clear enable

			uint32_t CC2S	: 2;	// Capture / compare 1 selection
			uint32_t OC2FE	: 1;	// Fast enable
			uint32_t OC2PE	: 1;	// Preload enable
			uint32_t OC2M	: 3;	// Output compare mode
			uint32_t OC2CE	: 1;	// Clear enable

			uint32_t 		: 16;	// .
		}OUTPUT_COMPARE;
		// Input capture mode register
		struct{
			uint32_t CC1S	: 2;	// Capture / compare 1 selection
			uint32_t IC1PSC	: 2;	// Capture prescaler
			uint32_t IC1F	: 4;	// Capture filter

			uint32_t CC2S	: 2;	// Capture / compare 1 selection
			uint32_t IC2PSC	: 2;	// Capture prescaler
			uint32_t IC2F	: 4;	// Capture filter

			uint32_t 		: 16;	// .
		}INPUT_CAPTURE;
	}CCMR1;

	union CCMR CCMR2;	// Capture / compare mode register 2

	// Capture / compare enable register
	struct CCER{
		uint32_t CC1E		: 1;	// Output enable 
		uint32_t CC1P		: 1;	// Output polarity
		uint32_t CC1NE		: 1;	// Complementary output enable
		uint32_t CC1NP		: 1;	// Complementary output polarity

		uint32_t CC2E		: 1;	// Output enable 
		uint32_t CC2P		: 1;	// Output polarity
		uint32_t CC2NE		: 1;	// Complementary output enable
		uint32_t CC2NP		: 1;	// Complementary output polarity

		uint32_t CC3E		: 1;	// Output enable 
		uint32_t CC3P		: 1;	// Output polarity
		uint32_t CC3NE		: 1;	// Complementary output enable
		uint32_t CC3NP		: 1;	// Complementary output polarity

		uint32_t CC4E		: 1;	// Output enable 
		uint32_t CC4P		: 1;	// Output polarity
		uint32_t 			: 1;	// .
		uint32_t CC4NP		: 1;	// Complementary output polarity

		uint32_t			: 16;	// .	
	}CCER;

	uint32_t CNT;	// Timer counter value
	uint32_t PSC;	// Timer prescaler (clock divisor)
	uint32_t ARR;	// Value to be loaded into the actual auto-reload register
	uint32_t RCR;	// Repetition counter register
	
	/**
	 * When CC is configered as:
	 * - Output: Value to be compared to CNT register
	 * - Input: Value most recently captured (read only)
	*/
	uint32_t CCR1;
	uint32_t CCR2;	// See CCR1
	uint32_t CCR3;	// See CCR1
	uint32_t CCR4;	// See CCR1

	// Break and dead-time register
	struct {
		uint32_t DTG		: 8;	// Dead-time generator setup
		uint32_t LOCK		: 2;	// Lock configuration
		uint32_t OSSI		: 1;	// Off-state selection for Idle mode
		uint32_t OSSR		: 1;	// Off-state selection for Run mode
		uint32_t BKE		: 1;	// Break enable
		uint32_t BKP		: 1;	// Break polarity
		uint32_t AOE		: 1;	// Automatic output enable
		uint32_t MOE		: 1;	// Main output enable
		uint32_t 			: 16;	// .
	}BDTR;
	
	uint32_t DMAR; // DMA address for full transfer

}Timer;


// typedef struct Timer{
// 	TimerReg *reg;
// 	TIMER type;
// }Timer;

extern Timer *timer1;
extern Timer *timer2;
extern Timer *timer3;
extern Timer *timer4;
extern Timer *timer5;


void Tim1Ch1_IRQ();

void TimerSetEnable(Timer *t, bool state);

void TimerPause(Timer *t);
void TimerStart(Timer *t);

inline void TimerSetCounter(Timer *t, uint16_t value);
inline uint16_t TimerGetCounter(Timer *t);

inline void TimerSetPrescaler(Timer *t, uint16_t value);
inline uint16_t TimerGetPrescaler(Timer *t);

inline void TimerSetReload(Timer *t, uint16_t value);
inline uint16_t TimerGetReload(Timer *t);

// functions we are gonna need
// bool TimerRunning(TIMER tim, CHANNEL ch);
// void TimerSetPrescaler(TIMER tim, uint32_t prescaler);
// void TimerSetCounter(TIMER tim, uint32_t counter);
// void TimerSetPreload(TIMER tim, uint32_t val);
// void TimerSetCompare(TIMER tim, CHANNEL ch, uint32_t val);
// Void TimerSetClockSrc(TIMER tim, CLK c);

// void TimerStart(TIMER tim);
// void TimerStop(TIMER tim);


#endif