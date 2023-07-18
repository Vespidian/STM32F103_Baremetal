#include "stdlib.h"
#include "usart.h"

#include "rtc.h"

typedef void (*CommandFunction)(const char *command_buffer);

char command_buffer[256];
uint8_t command_buffer_index;

char previous_char;

void FuncHelp(const char *command_buffer);
void FuncBlinkTime(const char *command_buffer);
void FuncTime(const char *command_buffer);
void FuncAlarm(const char *command_buffer);
void FuncPing(const char *command_buffer);
void FuncLed(const char *command_buffer);


const char *command_list[] = {
	"help",
	"blinktime",
	"time",
	"alarm",
	"ping",
	"led",
	NULL
};

CommandFunction function_list[] = {
	FuncHelp,
	FuncBlinkTime,
	FuncTime,
	FuncAlarm,
	FuncPing,
	FuncLed,
	NULL
};

/**
 * Compare two strings until either a null terminator or the first instance of 'delimiter'
*/
bool StringCompare(const char *str1, const char *str2, char delimeter){
	bool is_same = true;
	for(size_t i = 0; (str1[i] != '\0') && (str2[i] != '\0') && (str1[i] != delimeter) && (str2[i] != delimeter); i++){
		if(str1[i] != str2[i]){
			is_same = false;
			break;
		}
	}
	if(str1[0] == 0 || str2[0] == 0){
		is_same = false;
	}
	return is_same;
}

/**
 * Finds and returns the length until the terminating character or the first instance of 'delimiter'
*/
size_t StringLength(const char *str, char delimiter){
	size_t len = 0;
	while(str[len] != '\0' && str[len] != delimiter){
		len++;
	}
	return len;
}

/**
 * Finds the first instance of 'c' and returns a pointer to it
*/
char *FindChar(const char *str, char c){
	char *found_char = NULL;

	for(int i = 0; str[i] != '\0'; i++){
		if(str[i] == c){
			found_char = (char *)&str[i];
			break;
		}
	}
	return found_char;
}

/**
 * Iterates through the 'command_list' array and finds which matches the input
*/
unsigned int FindCommand(const char *command){
	int index = 0;
	bool found = false;
	while(command_list[index] != NULL){
		if(StringCompare(command_list[index], command, ' ')){
			found = true;
			break;
		}
		index++;
	}

	if(!found){
		index = -1;
	}
	return index;
}

/**
 * Convert string to integer value
*/
unsigned int StrToInt(const char *str, char delimiter){
	unsigned int num = 0;
	unsigned int digit = 1;
	if(str != NULL){
		for(int i = StringLength(str, ' ') - 1; i >= 0; i--){
			if(str[i] >= '0' && str[i] <= '9'){
				num += (str[i] - '0') * digit;
				digit *= 10;
			}
		}
	}
	return num;
}

void GetCommand(){
	// for(uint8_t i = usart_buffer_rx_head, buffer_index = 0; (i != usart_buffer_rx_tail) && (USART1_buffer_rx[i] != '\n'); i++, buffer_index++){
	// 	command_buffer[buffer_index] = USART1_buffer_rx[i];
	// }
	// if(previous_char != '\n' && previous_char != '\r'){
	if(command_buffer_index != 0){

		command_buffer[command_buffer_index] = 0;
		command_buffer_index = 0;

		// switch(FindCommand(command_buffer)){
		// 	case 0: // help
		// 		USARTWrite("The following commands are currently defined:\n\n");
		// 		for(int i = 0; command_list[i] != NULL; i++){
		// 			USARTWrite(command_list[i]);
		// 			USARTWrite(" ");
		// 		}
		// 		USARTWrite("\n");
		// 		break;
		// 	case 1: // time
		// 		USARTWriteInt(RTCGetTime());
		// 		break;
		// 	case 2: // time_set
		// 		RTCSetCounter(StrToInt(FindChar(command_buffer, ' ') + 1, ' '));
		// 		break;
		// 	case 3: // alarm
		// 		USARTWriteInt(RTCGetAlarm());
		// 		break;
		// 	case 4: // alarm_set
		// 		RTCSetAlarm(StrToInt(FindChar(command_buffer, ' ') + 1, ' '));
		// 		break;
		// 	case 5: // ping
		// 		for(int i = 0; i < StrToInt(FindChar(command_buffer, ' ') + 1, ' '); i++){
		// 			USARTWrite("pong\n");
		// 		}
		// 		break;


		// 	case -1:
		// 		USARTWrite(command_buffer);
		// 		USARTWrite(": command not found");
		// 		break;
		// 	default: // Command is found but calling it does nothing
		// 		USARTWrite(command_buffer);
		// 		USARTWrite(": command not implemented");
		// 		break;
		// }
		unsigned int function_id = FindCommand(command_buffer);
		if(function_id == -1){
			USARTWrite(command_buffer);
			USARTWrite(": command not found");
		}else if(function_list[function_id] == NULL){
			USARTWrite(command_buffer);
			USARTWrite(": command not implemented");
		}else{
			function_list[function_id](command_buffer);
		}

		command_buffer[0] = 0;
		
		// Display the prompt
		USARTWrite("\nstm32$ ");
	}else if(previous_char == '\n'){
		// Display the prompt
		USARTWrite("stm32$ ");
	}
}

// #include "gpio.h"
void Terminal(){
	char c;
	c = USARTReadByte();
	previous_char = c;
	switch(c){
		case 0:
			break;
		case 0x03: // Ctrl + c
			command_buffer_index = 0;
			break;
		case 0x7f: // Backspace 
			if(command_buffer_index != 0){
				command_buffer_index--;
				USARTWrite("\x1b[D \x1b[D");
			}
			break;
		case '\r': // Carriage return
			break;
		case '\n': // Line feed
			// Process the command
			GetCommand();
			break;
		default:
			command_buffer[command_buffer_index++] = c;
			break;
	}
	if(c != 0){
		// Return the character to the sender, for terminal like operation
		USARTWriteByte(c);
	}
}

/* --- COMMAND FUNCTIONS --- */

void FuncHelp(const char *command_buffer){
	USARTWrite("The following commands are currently defined:\n\n");
	for(int i = 0; command_list[i] != NULL; i++){
		USARTWrite(command_list[i]);
		USARTWrite(" ");
	}
	USARTWrite("\n");
}

#include "stm32f103xb.h"
void FuncBlinkTime(const char *command_buffer){
	if(StringCompare(command_buffer = (FindChar(command_buffer, ' ') + 1), "set", ' ')){
		// Set
		SysTick->LOAD = StrToInt(FindChar(command_buffer, ' ') + 1, ' ');
	}else{
		// Get
		USARTWriteInt(SysTick->LOAD);
		USARTWrite("\n");
	}
}

void FuncTime(const char *command_buffer){
	if(StringCompare(command_buffer = (FindChar(command_buffer, ' ') + 1), "set", ' ')){
		// Set
		RTCSetCounter(StrToInt(FindChar(command_buffer, ' ') + 1, ' '));
		USARTWriteInt(RTCGetTime());
	}else{
		// Get
		USARTWriteInt(RTCGetTime());
	}
}

void FuncAlarm(const char *command_buffer){
	if(StringCompare(command_buffer = (FindChar(command_buffer, ' ') + 1), "set", ' ')){
		// Set
		RTCSetAlarm(StrToInt(FindChar(command_buffer, ' ') + 1, ' '));
	}else{
		// Get
		USARTWriteInt(RTCGetAlarm());
	}
}

void FuncPing(const char *command_buffer){
	RTCSetCounter(4);
	for(int i = 0; i < StrToInt(FindChar(command_buffer, ' ') + 1, ' '); i++){
		USARTWrite("pong\n");
	}
}

#include "gpio.h"
// #include "timer.h"
void FuncLed(const char *command_buffer){
	// unsigned int *reg;
	// if(StringCompare((FindChar(command_buffer, ' ') + 1), "on", ' ')){
	// 	GPIOWrite(GPIO_PORT_A, 6, HIGH);

	// 	// Fade led in and out using pwm and a sine function
	// 	for(size_t i = 0; i < 100000; i++){
	// 		reg = (unsigned int *)TIMER1_ADDR_CCR1;
	// 		*reg = (custom_sin(i / 781 + 196) + 32767) / 512;
	// 	}
	// }
	// if(StringCompare((FindChar(command_buffer, ' ') + 1), "off", ' ')){
	// 	GPIOWrite(GPIO_PORT_A, 6, LOW);

	// 	for(size_t i = 0; i < 100000; i++){
	// 		reg = (unsigned int *)TIMER1_ADDR_CCR1;
	// 		*reg = (custom_sin(i / 781 + 64) + 32767) / 512;
	// 	}
	// }
}