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
    //int* txfifo = (int*)(((*(int*)(0x3FF4005C)) >> 2) & 0x7FF);
    //*(char*)(UART_0_TXFIFO_REG) = '0';
    while(x > 0)
    {
        int digit = x%10;
        char status = *(char*)(UART_0_TXFIFO_REG);
        
        if(status)
        {
            *(char*)(UART_0_TXFIFO_REG) = digit + '0';
            i++;
            x = x/10;
        }

        else
        {
            *(char*)(UART_0_TXFIFO_REG) = status;
        }
    }

    return(true);
}