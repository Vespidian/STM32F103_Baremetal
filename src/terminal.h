#ifndef TERMINAL_H_
#define TERMINAL_H_

extern char command_buffer[256];
extern uint8_t command_buffer_index;

void GetCommand();
void Terminal();

#endif