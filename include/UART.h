#ifndef dsuart
#define dsuart

#include <stdbool.h>
#include <stdint.h>

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
    static bool print(uint32_t x);

    enum UARTN
    {
        UART0,
        UART1,
        UART2
    };
};

#endif