#include "main.h"
#include "nvic.h"
#include "gpio.h"
#include "usart.h"

#define MANTISSA FREQUENCY / (BAUD * 16)
#define FRACTION (((((long long)FREQUENCY * 100) / (BAUD * 16)) - (MANTISSA * 100)) * 16) / 100

#define USART_ADDR			0x40013800

#define USART_ADDR_SR		0x40013800
#define USART_ADDR_DR		0x40013804
#define USART_ADDR_BRR		0x40013808
#define USART_ADDR_CR1		0x4001380c
#define USART_ADDR_CR2		0x40013810

typedef struct USART{

	// Status register
	struct SR{
		uint32_t PE 		: 1;	// Parity error
		uint32_t FE 		: 1;	// Framing error
		uint32_t NE 		: 1;	// Noise error flag
		uint32_t ORE 		: 1;	// Overrun error
		uint32_t IDLE 		: 1;	// IDLE line detected
		uint32_t RXNE 		: 1;	// Read data register not empty
		uint32_t TC 		: 1;	// Transmission complete
		uint32_t TXE 		: 1;	// Transmit data register empty
		uint32_t LBD 		: 1;	// LIN break detection flag
		uint32_t CTS 		: 1;	// CTS flag
		uint32_t 			: 22;	// .
	}SR;

	uint32_t DR;	// Data register

	struct{
		uint32_t DIV_Fraction 	: 4;	// 
		uint32_t DIV_Mantissa 	: 12;	// 
		uint32_t				: 16;	// .
	}BRR;

	// Configuration register 1
	struct CR1{
		uint32_t SBK 		: 1;	// Send break
		uint32_t RWU 		: 1;	// Receiver wakeup
		uint32_t RE 		: 1;	// Receiver enable
		uint32_t TE 		: 1;	// Transmitter enable
		uint32_t IDLEIE 	: 1;	// IDLE interrupt enable
		uint32_t RXNEIE 	: 1;	// RXNE interrupt enable
		uint32_t TCIE 		: 1;	// TC interrupt enable
		uint32_t TXEIE 		: 1;	// TXE interrupt enable
		uint32_t PEIE 		: 1;	// PE interrupt enable
		uint32_t PS 		: 1;	// Parity selection
		uint32_t PCE 		: 1;	// Parity control enable
		uint32_t WAKE 		: 1;	// Wakeup method
		uint32_t M 			: 1;	// World length
		uint32_t UE 		: 1;	// USART enable
		uint32_t			: 18;	// .
	}CR1;

	// Configuration register 2
	struct CR2{
		uint32_t ADD		: 4;	// Address of the USART node
		uint32_t 			: 1;	// .
		uint32_t LBDL		: 1;	// LIN break detection length
		uint32_t LBDIE		: 1;	// LIN break detection interrupt enable
		uint32_t 			: 1;	// .
		uint32_t LBCL		: 1;	// Last bit clock pulse
		uint32_t CPHA		: 1;	// Clock phase
		uint32_t CPOL		: 1;	// Clock polarity
		uint32_t CLKEN		: 1;	// Clock enable
		uint32_t STOP		: 2;	// STOP bits
		uint32_t LINEN		: 1;	// LIN mode enable
		uint32_t 			: 17;	// .
	}CR2;
}USART;

USART *usart1 = (USART *)USART_ADDR;

typedef struct USARTBuffer{
	char buffer[256];
	uint8_t head;
	uint8_t tail;	
}USARTBuffer;

char USART1_buffer_tx[256];
uint8_t usart_buffer_tx_head = 0;
uint8_t usart_buffer_tx_tail = 0;

#define USART1_BUFFER_SIZE 256
char USART1_buffer_rx[256];
uint8_t usart_buffer_rx_head = 0;
uint8_t usart_buffer_rx_tail = 0;

USART USART_SR_data;
bool usart_interrupt_ready = false;

void USARTSetBaud(){
    // unsigned short mantissa = current_clock_speed / (baud * 16);
    // unsigned short fraction = (((((long long)current_clock_speed * 100) / (baud * 16)) - (mantissa * 100)) * 16) / 100;
	
	usart1->BRR.DIV_Fraction = FRACTION;
	usart1->BRR.DIV_Mantissa = MANTISSA;

}

void USARTInit(){
	
	// Enable the usart interrupt in the NVIC
	NVICEnableInterrupt(37);

	// Set Tx pin as output alternate function push-pull
	GPIOSetPinMode(GPIO_PORT_A, 9, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL);
	
	// Set Rx pin as input pull-up
	GPIOSetPinMode(GPIO_PORT_A, 10, GPIO_MODE_INPUT, GPIO_CONFIG_INPUT_FLOATING);

	// Enable USART clock
	rcc->APB2ENR.USART1EN = 1;

	// Set baud rate
	USARTSetBaud(); // Set BRR register (baud rate)

	// Set the usart control register (Enable peripheral and interrupts)
	usart1->CR1.UE = true;
	usart1->CR1.TE = true;
	usart1->CR1.RE = true;
	usart1->CR1.RXNEIE = true;
	usart1->CR1.TCIE = true;
}

void USARTInterrupt(void){

	usart1->DR &= 0b0010000000;

	if(usart1->SR.RXNE == 1){
		USART1_buffer_rx[usart_buffer_rx_head++] = (char)usart1->DR;
		// if(usart_buffer_rx_head == USART1_BUFFER_SIZE){
		// 	usart_buffer_rx_head = 0;
		// }

	}else if(usart1->SR.TC == 1){
		if(usart_buffer_tx_tail != usart_buffer_tx_head){
			usart1->DR = USART1_buffer_tx[usart_buffer_tx_tail++];
			// if(usart_buffer_tx_tail == USART1_BUFFER_SIZE){
			// 	usart_buffer_tx_tail = 0;
			// }
		}
	}
}

void USARTWriteByte(uint8_t byte){

	// TXE (Wait for the transmit data register to be empty (1))
	while(usart1->SR.TXE == 0);

	// Set the data register's data to 'byte'
	usart1->DR = byte;


	/** -- OR -- **/
	// USART1_buffer_tx[usart_buffer_tx_head++] = byte; // Doesnt work for some reason
}

void USARTWrite(const char *str){
	if(str != NULL){
		// Put the string into the tx buffer and make the interrupt tx it all
		// for(int i = 0; (str[i] != 0) && (usart_buffer_tx_head != usart_buffer_tx_tail); i++){
		for(int i = 0; (str[i] != 0); i++){
			USART1_buffer_tx[usart_buffer_tx_head++] = str[i];
			// if(usart_buffer_tx_tail == USART1_BUFFER_SIZE){
			// 	usart_buffer_tx_tail = 0;
			// }
		}
		// Now tx the 1st character to allow the ISR to do the rest
		// USARTWriteByte(str[0]);
	}
}

void USARTWriteInt(uint32_t num){
	if(num == 0){
		// USARTWriteByte('0');
		USARTWrite("0");
		return;
	}

	uint8_t num_digits = 0;
	char digits[10] = {0};
	while(num > 0){
		digits[num_digits] = num % 10;
		num /= 10;
		num_digits++;
	}

	char str[11];
	for(int i = 0; i < num_digits; i++){
		str[i] = '0' + digits[num_digits - i - 1];
	}
	str[num_digits] = 0;
	USARTWrite(str);
}

void USARTWriteHex(uint8_t num){
	char str[4] = "0x00";

	uint8_t val = (num >> 4);
	str[2] = ((val > 9) ? (val + 'A' - 10) : (val + '0'));
	
	val = (num & 0x0f);
	str[3] = ((val > 9) ? (val + 'A' - 10) : (val + '0'));

	USARTWrite(str);
}

uint8_t USARTReadByte(){
	uint8_t c = 0;
	if(usart_buffer_rx_head != usart_buffer_rx_tail){
		c = USART1_buffer_rx[usart_buffer_rx_tail++];
		if(usart_buffer_rx_tail == USART1_BUFFER_SIZE){
			usart_buffer_rx_tail = 0;
		}
	}
	return c;
}