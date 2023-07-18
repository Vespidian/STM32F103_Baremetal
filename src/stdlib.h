#ifndef MAIN_H_
#define MAIN_H_

#define bool	_Bool
#define true	1
#define false	0
#define __bool_true_false_are_defined 1

#define NULL 0

#define HIGH true
#define LOW false

typedef unsigned int uintptr_t;
typedef unsigned int size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
// typedef unsigned long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

void *memcpy(void *dst, const void *src, size_t len);
void *memset(void *dst, int value, size_t len);


#endif