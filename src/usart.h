#ifndef USART_H_
#define USART_H_

#define BAUD 9600

void USARTInterrupt(void);

extern char USART1_buffer_rx[256];
extern uint8_t usart_buffer_rx_head;
extern uint8_t usart_buffer_rx_tail;

void USARTInit();

void USARTWriteByte(uint8_t byte);
void USARTWrite(const char *str);
void USARTWriteInt(uint32_t num);
void USARTWriteHex(uint8_t num);

uint8_t USARTReadByte();

#endif