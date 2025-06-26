#include <stdint.h>
#include <string.h>
#include <UART.h>

extern unsigned int _sidata, _sbss, _ebss, _sdata, _edata;

#define UART_0_TXFIFO_REG 0x3FF40000

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   memset( &_sbss, 0, ( &_ebss - &_sbss ) * sizeof( _sbss ) );//zero .bss
   memmove( &_sdata, &_sidata, ( &_edata - &_sdata ) * sizeof( _sdata ) ); //copy.data to ram

   static const char x[] = "abc";
   while(1)
   {
      //UART::print(1234);
      //*(int*)(0x3FF5F064) = 0x050D83AA1;//write protection register
      //*(int*)(0x3FF5F060) = 1;
      *(unsigned int*)(0x3FF480A4) = 0x50D83AA1;//rtc disable pro
      *(unsigned int*)(0x3FF5F064) = 0x50D83AA1;//wdt0 disable pro

      unsigned int x = *(unsigned int*)(0x3FF4808C);
      x &= ~(1U << 10);
      *(unsigned int*)(0x3FF4808C) = x;//disable rtc boot protection
      
      unsigned short y = *(unsigned short*)(0x3FF5F048);
      y &= ~(1U << 15);
      *(unsigned short*)(0x3FF4808C) = y;

      *(unsigned int*)(0x3FF480A0) = 1;//feed rtc wdt
      *(unsigned int*)(0x3FF5F060) = 1;//feed t0 wdt

      *(unsigned int*)(0x3FF480A4) = 1;//reenable rtc protection
      *(unsigned int*)(0x3FF5F064) = 1;
      //*(char*)(UART_0_TXFIFO_REG) = *x;
   }
}

