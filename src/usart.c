#include "main.h"
#include "gpio.h"
#include "usart.h"

#define MANTISSA FREQUENCY / (BAUD * 16)
#define FRACTION (((((long long)FREQUENCY * 100) / (BAUD * 16)) - (MANTISSA * 100)) * 16) / 100

typedef union USART{
	uint32_t data;
	struct SR{ // State flags register
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
	}SR;
	struct CR1{ // Settings 1 register
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
	}CR1;
}USART;

char USART1_buffer_tx[256];
uint8_t usart_buffer_tx_head = 0;
uint8_t usart_buffer_tx_tail = 0;

#define USART1_RX_BUFFER_SIZE 256
char USART1_buffer_rx[256];
uint8_t usart_buffer_rx_head = 0;
uint8_t usart_buffer_rx_tail = 0;

USART USART_SR_data;
bool usart_interrupt_ready = false;

void USARTInit(unsigned int baud_rate){
	// GCC doesnt 0 initialize variables properly so I do it here
	usart_buffer_tx_head = 0;
	usart_buffer_tx_tail = 0;
	usart_buffer_rx_head = 0;
	usart_buffer_rx_tail = 0;

	// Set the IRP (Interrupt priority) of the usart 1 register
	volatile unsigned int *reg = (unsigned int *)(0xE000E400 + 0x37); // Register NVIC_IPR
	*reg |= 0x20;

	// Enable the usart interrupt in the NVIC
	reg = (unsigned int *)(NVIC + 0x4); // Register NVIC_ISER1
	*reg |= (1 << 5);

	// Set Tx pin as output alternate function push-pull
	GPIOSetPinMode(GPIO_PORT_A, 9, GPIO_MODE_OUTPUT_10MHZ, GPIO_CONFIG_OUTPUT_AF_PUSHPULL);
	
	// Set Rx pin as input pull-up
	GPIOSetPinMode(GPIO_PORT_A, 10, GPIO_MODE_INPUT, GPIO_CONFIG_INPUT_FLOATING);

	// Enable USART clock
	reg = (unsigned int *)RCC_APB2ENR;
	*reg |= (1 << 14);

	// Set baud rate
	USARTSetBaud(72000000, 9600); // Set BRR register (baud rate)

	// Set the usart control register (Enable peripheral and interrupts)
	reg = (unsigned int *)USART_ADDR_CR1;
	USART CR1_reg;
	CR1_reg.data = *reg;	
	CR1_reg.CR1.UE = true;
	CR1_reg.CR1.TE = true;
	CR1_reg.CR1.RE = true;
	CR1_reg.CR1.RXNEIE = true;
	CR1_reg.CR1.TCIE = true;
	*reg = CR1_reg.data;
}

void USARTInterrupt(void){
	// usart_interrupt_ready = true;


	// unsigned int *reg = (unsigned int *)(0xE000E280 + 0x4); // Register NVIC_ISER1 (Clear pending register)
	// *reg |= (1 << 5);


	// GPIOWrite(GPIO_PORT_C, 13, led_state);
	// led_state = !led_state;


	volatile unsigned int *reg = (unsigned int *)USART_ADDR_SR;
	USART_SR_data.data = *reg;
	*reg &= 0b0010000000;


	reg = (unsigned int *)USART_ADDR_DR;
	if(USART_SR_data.SR.RXNE == 1){
		USART1_buffer_rx[usart_buffer_rx_head++] = (char)*reg;
		if(usart_buffer_rx_head == USART1_RX_BUFFER_SIZE){
			usart_buffer_rx_head = 0;
		}

	}else if(USART_SR_data.SR.TC == 1){
		if(usart_buffer_tx_tail != usart_buffer_tx_head){
			*reg = USART1_buffer_tx[usart_buffer_tx_tail++];
			if(usart_buffer_tx_tail == USART1_RX_BUFFER_SIZE){
				usart_buffer_tx_tail = 0;
			}
		}
	}
}

void CheckUsartInterrupt(){
	if(usart_interrupt_ready == true){
		// volatile unsigned int *reg = (unsigned int *)USART_ADDR_SR;
		// USART_SR_data.data = *reg;

		// if(USART_SR_data.SR.TC == 1){
		// 	// Send data to buffer byte
		// 	// reg = (unsigned int *)USART_ADDR_DR;
		// 	// char tmp = (char)*reg;
		// 	// *reg = tmp;
		// 	// if(usart_buffer_tx_tail != usart_buffer_tx_head){
		// 	// 	reg = (unsigned int *)USART_ADDR_DR;
		// 	// 	*reg = USART1_buffer_tx[usart_buffer_tx_tail++];
		// 	// 	if(usart_buffer_tx_tail == 256){
		// 	// 		usart_buffer_tx_tail = 0;
		// 	// 	}
		// 	// }

		// }else if(USART_SR_data.SR.RXNE == 1){
		// 	// Read data from buffer byte
		// 	reg = (unsigned int *)USART_ADDR_DR;
		// 	char tmp = (char)*reg;
		// 	*reg = tmp;

		// }
		// *reg = 0;

		usart_interrupt_ready = false;
	}
}

void USARTSetBaud(){
    // unsigned short mantissa = current_clock_speed / (baud * 16);
    // unsigned short fraction = (((((long long)current_clock_speed * 100) / (baud * 16)) - (mantissa * 100)) * 16) / 100;
	unsigned short mantissa = MANTISSA;
	unsigned short fraction = FRACTION;

	unsigned int *reg = (unsigned int *)USART_ADDR_BRR;
    *reg = fraction | (mantissa << 4);
}

void USARTWriteByte(uint8_t byte){
	unsigned int *reg = (unsigned int *)USART_ADDR_SR;

	while(((*reg >> 7) & 1) == 0); // TXE (Wait for the transmit data register to be empty (1))

	reg = (unsigned int *)USART_ADDR_DR;
	*reg = byte; // Set the data register's data to 'byte'
}

void USARTWrite(const char *str){
	if(str != NULL){
		// Option 1: Put the string into the tx buffer and make the interrupt tx it all
		// for(int i = 0; (str[i] != 0) && (usart_buffer_tx_head != usart_buffer_tx_tail); i++){
		for(int i = 0; (str[i] != 0); i++){
			USART1_buffer_tx[usart_buffer_tx_head++] = str[i];
			if(usart_buffer_tx_head == USART1_RX_BUFFER_SIZE){
				usart_buffer_tx_head = 0;
			}
		}
		// Now tx the 1st character to allow the ISR to do the rest
		// USARTWriteByte(str[0]);

		// Option 2: Send all the characters from within this function
		// volatile unsigned int *reg = (unsigned int *)USART_ADDR_SR;
		// for(int i = 0; (str[i] != 0); i++){
		// 	reg = (unsigned int *)USART_ADDR_SR;
		// 	while(((*reg >> 7) & 1) == 0); // TXE (Wait for the transmit data register to be empty (1))
			
		// 	reg = (unsigned int *)USART_ADDR_DR;
		// 	*reg = str[i];
		// }
	}
}

void USARTWriteInt(uint32_t num){
	uint8_t num_digits = 0;
	char digits[10] = {0};
	while(num > 0){
		digits[num_digits] = num % 10;
		num /= 10;
		num_digits++;
	}

	for(int i = 0; i < num_digits; i++){
	    USARTWriteByte('0' + digits[num_digits - i - 1]);
	}
}

uint8_t USARTReadByte(){
	uint8_t c = 0;
	if(usart_buffer_rx_head != usart_buffer_rx_tail){
		c = USART1_buffer_rx[usart_buffer_rx_tail++];
		USARTWriteByte(c);
		if(usart_buffer_rx_tail == USART1_RX_BUFFER_SIZE){
			usart_buffer_rx_tail = 0;
		}
	}
	return c;
}