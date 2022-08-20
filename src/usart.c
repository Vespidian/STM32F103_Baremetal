#include "main.h"
#include "gpio.h" // tmp
#include "usart.h"

char USART1_buffer_send[256];
uint8_t usart_buffer_send_head = 0;
uint8_t usart_buffer_send_tail = 0;

// char USART1_buffer_recv[256] = {};
// uint8_t usart_buffer_recv_head = 0;
// uint8_t usart_buffer_recv_tail = 0;

void USARTInterrupt(void){
	// Blink the LED


	volatile unsigned int *reg = (unsigned int *)USART_SR;
	USART test;
	test.data = *reg;

	if(test.SR.RXNE == 1){ // Read data register not empty (Move byte from DR reg to recv buffer)
		reg = (unsigned int *)USART_DR;
	// 	volatile uint32_t read = *reg;
	// 	(void)read;
	// // 	// Echo the byte back
		char receive = *reg;
		*reg = receive;
	// 	// *reg = 'b';
		if(receive == 'f'){
			GPIOWrite(GPIO_PORT_C, 13, led_state);
			led_state = !led_state;
		}
	}
	if(test.SR.TXE == 1){ // Transmit data register empty (Send the next byte in the send buffer)
	// if(test.SR.TC == 1){ // Transmit data register empty (Send the next byte in the send buffer)
		// if((usart_buffer_send_tail < usart_buffer_send_head) || (usart_buffer_send_tail > usart_buffer_send_head)){
		if(usart_buffer_send_tail != usart_buffer_send_head){
			// reg = (unsigned int *)USART_DR;
		// 	*reg = USART1_buffer_send[usart_buffer_send_tail++];
			USARTWriteByte(USART1_buffer_send[usart_buffer_send_tail++]);
		}
		// USARTWriteByte(loop_num);
		
		// if(loop_num == 'Z'){
		// 	USARTWriteByte('\n');
		// }
		// USARTWriteByte('0' + loop_num % 10);
		// USARTWriteByte(led_state == HIGH ? '1' : '0');
		// USARTWriteByte(13);
		// USARTWriteByte(10);
	}

	// }

	// if(test.SR.IDLE == 1){ // IDLE line detected (put an 'end of text' bit in the recv buffer)

	// }

}

void USARTSetBaud(unsigned int current_clock_speed, unsigned int baud){
    unsigned short mantissa = current_clock_speed / (baud * 16);
    unsigned short fraction = ((((current_clock_speed * 100) / (baud * 16)) - (mantissa * 100)) * 16) / 100;

	unsigned int *reg = (unsigned int *)USART_BRR;
    *reg = fraction | (mantissa << 4);
}

void USARTWriteByte(uint8_t byte){
	unsigned int *reg = (unsigned int *)USART_SR;
	USART test;
	test.data = *reg;
	if(test.SR.TXE == 1){	
		reg = (unsigned int *)USART_DR;
		*reg = byte; // Set the data register's data to 'byte'
		// reg = (unsigned int *)USART_SR;
		// // while(((*reg >> 7) & 1) == 1); // Wait for the data to be put into the shift register (TC)
		// while(((*reg >> 6) & 1) == 0); // Wait for the data to be put into the shift register (TC)
	}
}

void USARTWrite(const char *str){
	// if(str != NULL){
		// Add the characters to the send buffer (skipping the first one)
		for(int i = 0; (str[i] != 0) && (usart_buffer_send_head != usart_buffer_send_tail); i++){
		// for(unsigned int i = 0; i < 21; i++){
			USART1_buffer_send[usart_buffer_send_head++] = str[i];
		}

		// Now send the 1st character to allow the ISR to do the rest
		// USARTWriteByte(str[0]);
	// }
}
