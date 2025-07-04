#include "UART.h"
#include "string.h"

#define UART_0 0x3FF40000
#define UART_1 0x3FF50000
#define UART_2 0x3FF6E000

#define UART_FIFO_REG  0x0
#define UART_INT_ST_REG 0x8
#define UART_INT_ENA_REG 0xC
#define UART_INT_CLR_REG 0x10

unsigned int UART::uart = UART_0;

void UART::setActive(int x)
{
    switch(x)
    {
        case 0:
        {
            uart = UART_0;
            break;
        }

        case 1:
        {
            uart = UART_1;
            break;
        }

        case 2:
        {
            uart = UART_2;
            break;
        }
    }
}

int UART::init()
{
    unsigned int intEnable = *(unsigned int*)(uart + UART_INT_ENA_REG);
    intEnable |= (1 << 14);
   *(unsigned int*)(uart + UART_INT_ENA_REG) = intEnable;

   sendCharW('\r');
   sendCharW('\n');
    return(0);
}


void UART::sendCharW(char c)
{
    *(char*)(uart + UART_FIFO_REG) = c;

    while(((*(unsigned int*)(uart + UART_INT_ST_REG) >> 14) & 1U) == 0)//endlessly loop while txdone bit is not set 
    {
        
    }
    *(unsigned int*)(uart + UART_INT_CLR_REG) |= (1U << 14);//clear the interrupt bit i.e set it to 0.
}

bool UART::print(const char* x)
{
    int l = strlen(x);
    for(int i = 0; i < l; i++)
    {
        sendCharW(x[i]);
    }   

    return(true);
}

bool UART::print(int x)
{
    int y = x;
    if(y < 0)
    {
        y = y * -1;
        sendCharW('-');
    }
    int b10 = 1;
    while(b10 * 10 <= y)
    {
        b10 = b10 * 10;
    }
    while(b10 > 0)
    {
        int digit = y/b10;
        y = y%b10;
        b10 = b10/10;
        sendCharW((char)(digit + '0'));
    }
    return(true);
}