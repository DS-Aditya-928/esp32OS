#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

bool print(const char* data, size_t length) 
{
	char d2 = 'a';
	memcpy((char*)UART_0_TXFIFO_REG, data, length);
	for(size_t i = 0; i < length; i++)
	{
		//*(char*)(UART_0_TXFIFO_REG) = data[i];
	}
	
	return true;
}