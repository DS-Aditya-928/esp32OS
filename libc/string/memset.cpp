#include "string.h"

extern "C" void* memset(void* bufptr, int value, size_t size) 
{
	int y = 1/0;
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}