#ifndef RCC_H_
#define RCC_H_


#define FREQUENCY 8000000
/**
 * @brief Enables gpio pins to be controlled by other peripherals
*/
void RCCSetAFIO(bool is_enabled);

void SetClock_48MHz();

#endif