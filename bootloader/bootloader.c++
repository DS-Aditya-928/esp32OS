#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>
#include <math.h>

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   WDT::disableBootProtection(WDT::RTC);
   WDT::disableBootProtection(WDT::TIMG0);
   UART::setActive(UART::UART0);
   UART::init();

   UART::print("Kernel loaded!\r\n");
   UART::print("Compiled on "); UART::print(__DATE__); UART::print(" at "); UART::print(__TIME__); UART::print(".\r\n");

   //0x3FF1_0000 is the cache MMU table. 16KB
   #define cacheMMU 0x3FF10000//each entry is 32 bits
   #define MMUCONF 0x3FF00040
   #define MMUCONF2 0x3FF00044

   unsigned int st = *(unsigned int*)(MMUCONF);
   UART::print(st);
   //st |= (1U << 3);
   st &= ~(1U << 4);
   //*(unsigned int*)(MMUCONF) = st;
   UART::print("\r\n");

   unsigned int st2 = *(unsigned int*)(MMUCONF2);
   //UART::print(st);
   //st2 &= ~(1 << 0);
   //st2 &= ~(1 << 1);
   //st2 &= ~(1 << 2);
   st2 &= ~(1 << 3);//only enable dram1 access
   *(unsigned int*)(MMUCONF2) = st2;//enable iram to cache access
   //UART::print("\r\n");

   unsigned int st3 = *(unsigned int*)(0x3FF10000 + (1152 * 4));
   *(unsigned char*)(0x3FF10000 + (1152 * 4)) = (0x20); 
    unsigned int st4 = *(unsigned int*)(0x3FF10000 + (1153 * 4));
   //*(unsigned char*)(0x3FF10000 + (1153 * 4)) = (0x40); 

   //int* x = (int*)0x3F800000;//MMU entry 1152
   //int* y = (int*)(0x3F800000 + 0x8000);
   //*(x) = 96;
   //*(y) = 112;
   //UART::print(*x);

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