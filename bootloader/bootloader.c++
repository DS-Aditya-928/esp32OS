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

   char* a = (char*)malloc(sizeof(char) * 2);
   a = "a";
   bool* y = (bool*)malloc(1);
   int* x = (int*)malloc(sizeof(int));
   *x = 19283;
   free(x);
   int* z = (int*)malloc(sizeof(int));
   *z = 90;

   testClass* mmu = new testClass();
   //free(z);

   getHeapMD();
   
   UART::print(*z);
   UART::print(a);
   while(1)
   {    
   }
}