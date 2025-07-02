#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>
#include <math.h>

extern unsigned int _sidata, _sbss, _ebss, _sdata, _edata;

#define UART_0_TXFIFO_REG 0x3FF40000

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   WDT::disableBootProtection(WDT::RTC);
   WDT::disableBootProtection(WDT::TIMG0);
   UART::setActive(UART::UART0);
   UART::init();

   char* p = "pqrs\r\n";
   while(1)
   {      
      UART::print(p);
      UART::print(1234);
   }
}

