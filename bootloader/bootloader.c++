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
   unsigned int intEnable = *(unsigned int*)(0x3FF4000C);
   intEnable |= (1 << 14);
   *(unsigned int*)(0x3FF4000C) = intEnable;
   static const char p[] = "xyz\r\n";
   
   while(1)
   {
      for(unsigned char i = 0; i < 6; i++)
      {
         /*
         unsigned char inBuf = 0;
         for(int i = 0; i < 100000; i++)
         {
            unsigned int txStatus = *(unsigned int*)(0x3FF4001C);
            inBuf = (txStatus >> 8) & 0xF;
            char t = *(char*)(UART_0_TXFIFO_REG);
            if(t != 0)
            {
               *(char*)(UART_0_TXFIFO_REG) = t;
            }
         }
         */
         *(char*)(UART_0_TXFIFO_REG) = p[i];
         while(((*(unsigned int*)(0x3FF40008) >> 14) & 1U) == 0)//endlessly loop while txdone bit is not set 
         {
            //*(char*)(UART_0_TXFIFO_REG) = ((intEnable >> 1) & 1U) + '0';
            //*(unsigned int*)(0x3FF40010) |= (1U << 1);
         }
         *(unsigned int*)(0x3FF40010) |= (1U << 14);//clear the interrupt bit i.e set it to 0.
      }
      
      //UART::print(1234);
   }
}

