#ifndef USART_H_
#define USART_H_

#define USART_SR		0x40013800
#define USART_DR		0x40013804
#define USART_BRR		0x40013808
#define USART_CR1		0x4001380c

typedef enum USART_SR_BITS{
	USART_SR_PE 		= 1 << 0,
	USART_SR_FE 		= 1 << 1,
	USART_SR_NE 		= 1 << 2,
	USART_SR_ORE 		= 1 << 3,
	USART_SR_IDLE 		= 1 << 4,
	USART_SR_RXNE 		= 1 << 5,
	USART_SR_TC 		= 1 << 6,
	USART_SR_TXE 		= 1 << 7,
	USART_SR_LBD 		= 1 << 8,
	USART_SR_CTS 		= 1 << 9,
}USART_SR_BITS;

typedef enum USART_CR1_BITS{
	USART_CR1_SBK 		= 1 << 0,
	USART_CR1_RWU 		= 1 << 1,
	USART_CR1_RE 		= 1 << 2,
	USART_CR1_TE 		= 1 << 3,
	USART_CR1_IDLEIE 	= 1 << 4,
	USART_CR1_RXNEIE 	= 1 << 5,
	USART_CR1_TCIE 		= 1 << 6,
	USART_CR1_TXEIE 	= 1 << 7,
	USART_CR1_PEIE 		= 1 << 8,
	USART_CR1_PS 		= 1 << 9,
	USART_CR1_PCE 		= 1 << 10,
	USART_CR1_WAKE 		= 1 << 11,
	USART_CR1_M 		= 1 << 12,
	USART_CR1_UE 		= 1 << 13,
}USART_CR1_BITS;

typedef union USART{
	uint32_t data;
	struct SR{
		uint8_t PE 		: 1;	// Parity error
		uint8_t FE 		: 1;	// Framing error
		uint8_t NE 		: 1;	// Noise error flag
		uint8_t ORE 	: 1;	// Overrun error
		uint8_t IDLE 	: 1;	// IDLE line detected
		uint8_t RXNE 	: 1;	// Read data register not empty
		uint8_t TC 		: 1;	// Transmission complete
		uint8_t TXE 	: 1;	// Transmit data register empty
		uint8_t LBD 	: 1;	// LIN break detection flag
		uint8_t CTS 	: 1;	// CTS flag
	}SR;
	struct CR1{
		uint8_t SBK 	: 1;	// Send break
		uint8_t RWU 	: 1;	// Receiver wakeup
		uint8_t RE 		: 1;	// Receiver enable
		uint8_t TE 		: 1;	// Transmitter enable
		uint8_t IDLEIE 	: 1;	// IDLE interrupt enable
		uint8_t RXNEIE 	: 1;	// RXNE interrupt enable
		uint8_t TCIE 	: 1;	// TC interrupt enable
		uint8_t TXEIE 	: 1;	// TXE interrupt enable
		uint8_t PEIE 	: 1;	// PE interrupt enable
		uint8_t PS 		: 1;	// Parity selection
		uint8_t PCE 	: 1;	// Parity control enable
		uint8_t WAKE 	: 1;	// Wakeup method
		uint8_t M 		: 1;	// World length
		uint8_t UE 		: 1;	// USART enable
	}CR1;
}USART;


void USARTInterrupt(void);
void USARTSetBaud(unsigned int current_clock_speed, unsigned int baud);
void USARTWriteByte(uint8_t byte);
void USARTWrite(const char *str);

#endif