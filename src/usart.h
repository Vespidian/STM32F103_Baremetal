#ifndef USART_H_
#define USART_H_

#define BAUD 9600

#define USART_ADDR_SR		0x40013800
#define USART_ADDR_DR		0x40013804
#define USART_ADDR_BRR		0x40013808
#define USART_ADDR_CR1		0x4001380c

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

void USARTInit(unsigned int baud_rate);
void USARTInterrupt(void);
void USARTSetBaud();
void USARTWriteByte(uint8_t byte);
void USARTWrite(const char *str);
void USARTWriteInt(uint32_t num);

uint8_t USARTReadByte();

void CheckUsartInterrupt();

#endif