#ifndef dsuart
#define dsuart

#include <stdbool.h>
#define UART_0_TXFIFO_REG 0x3FF40000
#define UART_0_CONF_REG 0x3FF40020
#define UART_0_STATUS_REG 0x3FF4001C

namespace UART
{
    int init();
    bool print(const char* data);
    bool print(int x);
}

#endif