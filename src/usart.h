#ifndef USART_H_
#define USART_H_

#define BAUD 9600

void USARTInterrupt(void);

void USARTInit();

void USARTWriteByte(uint8_t byte);
void USARTWrite(const char *str);
void USARTWriteInt(uint32_t num);

uint8_t USARTReadByte();

#endif