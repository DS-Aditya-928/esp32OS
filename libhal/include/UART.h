#ifndef dsuart
#define dsuart

#include <stdbool.h>
#define UART_0_TXFIFO_REG 0x3FF40000
#define UART_0_CONF_REG 0x3FF40020
#define UART_0_STATUS_REG 0x3FF4001C


class UART
{
private:
    static unsigned int uart;
    static void sendCharW(char x);
public:
    static void setActive(int);
    static int init();
    static bool print(const char* data);
    static bool print(int x);

    enum UARTN
    {
        UART0,
        UART1,
        UART2
    };
};

#endif