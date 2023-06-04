#include "main.h"
#include "gpio.h"
#include "timer.h"

#include "i2c.h"

//TMP
#include "usart.h"

#define TIMEOUT_TIME 16384

#define I2C1_ADDR		0x40005400

#define I2C1_ADDR_CR1		0x40005400
#define I2C1_ADDR_CR2		0x40005404
#define I2C1_ADDR_DR		0x40005410
#define I2C1_ADDR_SR1		0x40005414
#define I2C1_ADDR_CCR		0x4000541c
#define I2C1_ADDR_TRISE		0x40005420

typedef struct I2C{
	// Configuration register 1
	struct {
		uint32_t PE 		: 1;	// Peripheral enable
		uint32_t SMBUS		: 1;	// SMBus mode
		uint32_t  			: 1;	// .
		uint32_t SMBTYPE	: 1;	// SMBus type
		uint32_t ENARP		: 1;	// ARP enable
		uint32_t ENPEC		: 1;	// PEC enable
		uint32_t ENGC		: 1;	// General call enable
		uint32_t NOSTRETCH 	: 1;	// Clock stretching disable (Slave mode)
		uint32_t START		: 1;	// Start generation
		uint32_t STOP		: 1;	// Stop generation
		uint32_t ACK		: 1;	// Acknowledge enable
		uint32_t POS		: 1;	// Acknowledge / PEC position (for data reception)
		uint32_t PEC		: 1;	// Packet error checking
		uint32_t ALERT		: 1;	// SMBus alert
		uint32_t  			: 1;	// .
		uint32_t SWRST		: 1;	// Software reset
		uint32_t			: 16;	// .
	}CR1;

	// Configuration register 2
	struct {
		uint32_t FREQ		: 6;	// Peripheral clock frequency
		uint32_t 			: 2;	// .
		uint32_t ITERREN	: 1;	// Error interrupt enable
		uint32_t ITEVTEN	: 1;	// Event interrupt enable
		uint32_t ITBUFEN	: 1;	// Buffer interrupt enable
		uint32_t DMAEN		: 1;	// DMA requests enable
		uint32_t LAST		: 1;	// DMA last transfer
		uint32_t 			: 3;	// .
		uint32_t			: 16;	// .
	}CR2;

	uint32_t OAR1; // padding (not yet defined)
	uint32_t OAR2; // padding (not yet defined)

	// struct OAR2{
	// 	uint8_t ENDUAL	: 1;	// 
	// 	uint8_t ADD2	: 7;	// 
	// 	uint16_t 		: 16;	// .
	// 	uint16_t 		: 8;	// .
	// }OAR2;

	// Data register
	uint32_t DR			: 8;

	// Status register 1
	struct {
		uint32_t SB			: 1;	// Start bit (master mode)
		uint32_t ADDR		: 1;	// Addr sent (master mode) / matched (slave mode)
		uint32_t BTF		: 1;	// Byte transfer finished
		uint32_t ADD10		: 1;	// 10-bit header sent (Master mode)
		uint32_t STOPF		: 1;	// Stop detection (slave mode)
		uint32_t 			: 1;	// .
		uint32_t RXNE		: 1;	// Data register not empty (receivers)
		uint32_t TXE		: 1;	// Data register empty (transmitters)
		uint32_t BERRR		: 1;	// Bus error
		uint32_t ARLO		: 1;	// Arbitration lost (master mode)
		uint32_t AF			: 1;	// Acknowledge failure
		uint32_t OVR		: 1;	// Overrun / Underrun
		uint32_t PECERR		: 1;	// PEC error in reception
		uint32_t 			: 1;	// .
		uint32_t TIMEOUT	: 1;	// Timeout or Tlow error
		uint32_t SMBALERT	: 1;	// SMBus alert
		uint32_t			: 16;	// .
	}SR1;

	struct {
		uint32_t MSL		: 1;	// Master / slave
		uint32_t BUSY		: 1;	// Bus busy
		uint32_t TRA		: 1;	// Transmitter / receiver
		uint32_t 			: 1;	// .
		uint32_t GENCALL	: 1;	// General call address (slave mode)
		uint32_t SMBDEFAULT	: 1;	// SMBus device default address (slave mode)
		uint32_t SMBHOST	: 1;	// SMBus host header (slave mode)
		uint32_t DUALF		: 1;	// Dual flag (slave mode)
		uint32_t PEC		: 8;	// Packet error checking register
		uint32_t 			: 16;	// .
	}SR2;

	// Clock control register
	struct {
		uint32_t CCR		: 12;	// Clock control register (master mode)
		uint32_t 			: 2;	// .
		uint32_t DUTY		: 1;	// Fm mode duty cycle
		uint32_t FS			: 1;	// I2C master mode selection (Fm / Sm)
		uint32_t			: 16;	// .
	}CCR;

	uint32_t TRISE			: 16;	// 
}I2C;

I2C *i2c1 = (I2C *)I2C1_ADDR;

// #include "usart.h"
// static void test(uint8_t address, uint8_t data){
// 	// uint8_t value;
// 	// Start
// 	i2c1->CR1.ACK = 1;
// 	i2c1->CR1.START = 1;
// 	while(i2c1->SR1.SB != 1); // Wait to get put into master mode
// 	// value = i2c1->SR1.ADDR;

// 	//send address
// 	i2c1->DR = address << 1;
// 	while(!i2c1->SR1.ADDR){
// 		if(i2c1->SR1.AF){
// 			return;
// 		}
// 	}
// 	// Clear the ADDR bit by reading SR1 followed by SR2
// 	volatile uint32_t read_reg;
// 	read_reg = i2c1->SR1.ADDR;
// 	read_reg = i2c1->SR2.BUSY;

// 	//send data
// 	while(!i2c1->SR1.TXE); // Wait for data register to be empty
// 	i2c1->DR = data;
// 	while(!i2c1->SR1.BTF);

// 	USARTWrite("Sent: ");
// 	USARTWriteInt(address);
// 	USARTWrite("\n");

// 	// stop
// 	i2c1->CR1.STOP = 1;
// }



/**
 *  In 7-bit addressing mode,
 *  Transmitter mode (write): 	address with LSB reset
 *  Receiver mode (read): 		address with LSB set
*/

void I2CStart(){
	// i2c1->CR1.ACK = 1;
	i2c1->CR1.START = 1;

	uint16_t count = 0;
	while(i2c1->SR1.SB != 1 && (count++ < TIMEOUT_TIME)); // Wait to get put into master mode
}

void I2CStop(){
	i2c1->CR1.STOP = 1;
}

bool I2CAddress_w(uint8_t address){
	// Write the destination address to the I2C data bus
	i2c1->DR = address << 1;

	// Wait for the address to be sent
	uint16_t count = 0;
	while(!i2c1->SR1.ADDR){
		if(i2c1->SR1.AF || count > (1 << 14)){
			i2c1->SR1.AF = 0;
			return false;
		}
		count++;
	}

	// Clear the ADDR bit by reading SR1 followed by SR2
	volatile uint32_t read_reg;
	read_reg = i2c1->SR1.ADDR;
	read_reg = i2c1->SR2.BUSY;

	// while(!i2c1->SR1.TXE); // Wait for data register to be empty

	return true;
}


uint8_t I2CAddress_r(uint8_t address){
	uint8_t data = 0;

	// Write the destination address to the I2C data bus
	i2c1->DR = (address << 1) + 1;

	// Wait for the address to be sent
	uint16_t count = 0;
	while(!i2c1->SR1.ADDR){
		if(i2c1->SR1.AF || count > (1 << 14)){
			i2c1->SR1.AF = 0;
			goto exit;
		}
		count++;
	}

	i2c1->CR1.ACK = 0;


	// Clear the ADDR bit by reading SR1 followed by SR2
	volatile uint32_t read_reg;
	read_reg = i2c1->SR1.ADDR;
	read_reg = i2c1->SR2.BUSY;

	I2CStop();

	count = 0;
	while(!i2c1->SR1.RXNE && (count++ < TIMEOUT_TIME)); // Wait for data register to be filled
	data = i2c1->DR;

	exit:
	return data;
}

void I2CWriteByte(uint8_t data){
	while(!i2c1->SR1.TXE); // Wait for data register to be empty
	i2c1->DR = data;
	uint16_t count = 0;
	while(!i2c1->SR1.BTF){
		if(i2c1->SR1.AF || count > (1 << 14)){
			i2c1->SR1.AF = false;
			// USARTWrite("NACK!\n");
			break;
		}
		count++;
	}
}

void I2CInit(){
	GPIOEnable(GPIO_PORT_B);
	GPIOSetPinMode(GPIO_PORT_B, 7, GPIO_MODE_OUTPUT_50MHZ, GPIO_CONFIG_OUTPUT_AF_OPENDRAIN); // SDA
	GPIOSetPinMode(GPIO_PORT_B, 6, GPIO_MODE_OUTPUT_50MHZ, GPIO_CONFIG_OUTPUT_AF_OPENDRAIN); // SCL
	// Pullup both pins
	GPIOWrite(GPIO_PORT_B, 7, HIGH);
	GPIOWrite(GPIO_PORT_B, 6, HIGH);


	// Make sure the timer's counter is off
	// timer4->CR1.CEN = false;
	// timer4->SR.UIF = false;

	// rcc->APB1ENR.TIM4EN = 1; // Enable timer 4 (for i2c1)

	// timer4->CR1.UDIS = 1; // Assign this so no register values are updated
	// timer4->PSC = 8*2;
	// timer4->ARR = 0xffff;

	// timer4->CR1.UDIS = 1; // Let assigned values move to preload registers
	// timer4->EGR.UG = 1; // Enable update generation for updating shadow register with preload register
	// timer4->SR.UIF = 0; // Make sure update interrupt flag is disabled

	rcc->APB1ENR.I2C1 = 1; // Enable I2C1 peripheral clock

	i2c1->CR1.SWRST = 1;
	i2c1->CR1.SWRST = 0;

	for(int i = 0; i < 10000; i++);

	// Configure the peripheral clock frequency
	i2c1->CR2.FREQ = FREQUENCY / 1000000;
	// i2c1->CR2.DMAEN = 0;

	// Configure the SCL clock
	i2c1->CCR.CCR = FREQUENCY / 100000;
	
	// Configure the max rise time
	i2c1->TRISE = FREQUENCY / 1000000 + 1;
	// Configure the peripheral clock frequency
	// i2c1->CR2.FREQ = 8;

	// // Configure the SCL clock
	// i2c1->CCR.CCR = 80;
	
	// // Configure the max rise time
	// i2c1->TRISE = 9;
	
	// Enable to i2c peripheral
	i2c1->CR1.PE = 1;



	// uint8_t address = 0x3d;
	// uint8_t data = 0x02;

	// for(int i = 0; i < 120;  i++){
	// 	// test(i, 0xaa);
	// 	// test(0x3D, 0xaa);
	// 	// test(0x3C, 0xCC);
	// 	// stop
	// 	// i2c1->CR1.STOP = 1;
	// 	// I2CStart();
	// 	// if(I2CAddress_w(i)){
	// 	// 	I2CWriteByte(10);
	// 	// }
	// 	// I2CStop();





	// 	for(int i = 0; i < 10; i++);
	// }
	I2CStart();
	I2CAddress_w(0);
	I2CStop();

	uint8_t address = 0x68; // MPU6050 / MPU9250 (0x68)
	// uint8_t address = 0x0D; // Magnetic Field sensor (0x0D)

	// Set power management register to 0
	// I2CStart();
	// if(I2CAddress_w(address)){
	// 	I2CWriteByte(0x6b); // register
	// }

	// I2CStart();
	// if(I2CAddress_w(address)){
	// 	I2CWriteByte(0); // data
	// }
	// I2CStop();

	// Loop through all addresses and see which respond
	// for(int i = 0; i < 128; i++){
	// 	I2CStart();
	// 	if(I2CAddress_w(i)){
	// 		I2CWriteByte(0xaa); // register
	// 	}
	// 	I2CStop();
	// }

// for(int i = 0; i < 0xff; i++){
// 	// uint8_t address = 0x0d;
	uint16_t num;
// 	// while(true){
// 		I2CStart();
// 		if(I2CAddress_w(address)){
// 			I2CWriteByte(i); // register
// 		}

// 		I2CStart();
// 		num = I2CAddress_r(address) >> 1;
// 		I2CStop();

// 		USARTWriteHex(i);
// 		USARTWrite(" <register:val> ");
// 		// USARTWriteInt(num);
// 		USARTWriteHex(num);
// 		USARTWrite("\n");
// 		USARTWriteByte(0);

// 		for(int i = 0; i < 100; i++);
// 	}
// }

	const int avg_samples = 8;
	int16_t values[8];
	uint8_t high, low;
	unsigned long t = 0;
	while(true){
		high = low = 0;
		num = 0;

		I2CStart();
		if(I2CAddress_w(address)){
			I2CWriteByte(67); // GYRO_XOUT_H
		}
		
		I2CStart();
		high = I2CAddress_r(address);
		I2CStop();

		I2CStart();
		if(I2CAddress_w(address)){
			I2CWriteByte(68); // GYRO_XOUT_L
		}

		I2CStart();
		low = I2CAddress_r(address);
		I2CStop();

		num = high << 8;
		num |= low;
		int16_t num_signed = num - (1 << 16);
		values[t % avg_samples] = num_signed;

		int32_t avg = 0;
		for(int i = 0; i < avg_samples; i++){
			avg += values[i];
		}
		avg /= avg_samples;

		// Accel
		USARTWriteByte(avg & 0xff);
		USARTWriteByte((uint8_t)(avg >> 8));

		t++;
		// for(int i = 0; i < 1000; i++); // Delay
	}
}