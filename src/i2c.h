#ifndef I2C_H_
#define I2C_H_

void I2CInit();
uint8_t I2CReadByte(uint8_t address);
void I2CWriteByte(uint8_t data);

#endif