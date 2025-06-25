#define __libc 1
#include <stdbool.h>

#define UART_0_TXFIFO_REG 0x3FF40000

#define EOF (-1)

bool print(const char* data, size_t length);