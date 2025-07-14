#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>
#include <MMU.h>

class testClass
{
public:
    testClass()
    {
        UART::print("testClass constructor called!\r\n");
    }
    ~testClass()
    {
        UART::print("testClass destructor called!\r\n");
    }
};


extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   WDT::disableBootProtection(WDT::RTC);
   WDT::disableBootProtection(WDT::TIMG0);
   cMMU::init();
   UART::setActive(UART::UART0);
   UART::init();

   UART::print("Kernel loaded!\r\n");
   UART::print("Compiled on "); UART::print(__DATE__); UART::print(" at "); UART::print(__TIME__); UART::print(".\r\n");

    int test = 1234567;
    void* sp;

    __asm__ volatile ("mov %0, a1" : "=r"(sp));
    UART::print("\r\nStack pointer: ");
    UART::print((int)sp);
    UART::print("\r\n");
    UART::print("Test value: ");
    UART::print(test); 
    UART::print("\r\n");
    UART::print("Changing stack\r\n");
    void* stackPointer = (void*)(0x3FFE0000 + 2048);
    UART::print("New stack pointer: ");
    UART::print((int)stackPointer);
    UART::print("\r\n");
    __asm__ volatile ("mov a1, %0" : : "r"(stackPointer));
    UART::print(test);
    //UART::print("\r\n");
    UART::print("Changing 2 stack\r\n");
    __asm__ volatile ("mov a1, %0" : : "r"(sp));
    UART::print(test);

   while(1)
   {    
   }
}