#ifndef dsuart
#define dsuart

#include <stdbool.h>

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