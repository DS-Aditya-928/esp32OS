#include "UART.h"
#include "string.h"

typedef unsigned long uintptr_t;

int UART::init()
{
    return(0);
}

bool UART::print(const char* x)
{
    *(char*)UART_0_TXFIFO_REG = x[0];
    //memcpy((char*)UART_0_TXFIFO_REG, x, strlen(x));
    //*(char*)UART_0_TXFIFO_REG = *x;

    return(true);
}

bool UART::print(int x)
{
    int i = 0;
    while(x > 0)
    {
        int digit = x%10;
        *(char*)(UART_0_TXFIFO_REG) = digit + '0';
        i++;
        x = x/10;
    }

    //*(char*)(UART_0_TXFIFO_REG) = 'E';
    return(true);
}