#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>
#include <MMU.h>
#include <math.h>

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   WDT::disableBootProtection(WDT::RTC);
   WDT::disableBootProtection(WDT::TIMG0);
   MMU::init();
   UART::setActive(UART::UART0);
   UART::init();

   UART::print("Kernel loaded!\r\n");
   UART::print("Compiled on "); UART::print(__DATE__); UART::print(" at "); UART::print(__TIME__); UART::print(".\r\n");

   for(int i = 0; i < 0x2000; i++)//this is first 32kb (0x2000). once we go past, unmapped and corruption occurs? should examine.
   {
      //UART::print(i);
      //UART::print(" ");
      unsigned int* p = (unsigned int*)(0x3F800000 + (i * 0x4));
      //UART::print(*p);
      *p = (unsigned int)i;
      //UART::print(*p);
      //UART::print("\r\n");
   }
   UART::print("Writes done\r\n");
   for(int i = 0; i < 0x2000; i++)
   {
      
      unsigned int* p = (unsigned int*)(0x3F800000 + (i * 0x4));

      if((*p != i) || (i == 0x2000 - 1))
      {
         UART::print(i);
         UART::print(" ");
         UART::print((unsigned int)*p);
         UART::print("\r\n");
      }
   }

   UART::print("Done!");
   while(1)
   {    
   }
}