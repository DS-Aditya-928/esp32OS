#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>

extern unsigned int _sidata, _sbss, _ebss, _sdata, _edata;

#define UART_0_TXFIFO_REG 0x3FF40000

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   memset( &_sbss, 0, ( &_ebss - &_sbss ) * sizeof( _sbss ) );//zero .bss
   memmove( &_sidata, &_sdata, ((uintptr_t)&_edata - (uintptr_t)&_sdata ) ); //copy.data to ram

   WDT::disableBootProtection(WDT::RTC);
   WDT::disableBootProtection(WDT::TIMG0);
   static const char p[] = "xyz";
   
   while(1)
   {
      int txfifo = (int)(((*(int*)(0x3FF4005C)) >> 2) & 0x7FF);

      for(unsigned char i = 0; i < 3; i++)
      {
         
         *(char*)(UART_0_TXFIFO_REG) = p[i];
      }
      
      //UART::print(1234);
   }
}

