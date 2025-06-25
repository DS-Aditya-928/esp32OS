#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern unsigned int _sidata, _sbss, _ebss, _sdata, _edata;

#define GPIO_OUT_W1TS_REG 0x3FF44008
#define GPIO_OUT_W1TC_REG 0x3FF4400C
#define GPIO_ENABLE_REG   0x3FF44020
#define GPIO5             5

int global_data = 42;         // Goes to .data
int p;

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
   //memset( &_sbss, 0, ( &_ebss - &_sbss ) * sizeof( _sbss ) );//zero .bss
   memmove( &_sdata, &_sidata, ( &_edata - &_sdata ) * sizeof( _sdata ) ); //copy.data to ram

   /*sdgf
   volatile uint32_t* gpio_out_w1ts_reg = (volatile uint32_t*) GPIO_OUT_W1TS_REG;
   volatile uint32_t* gpio_out_w1tc_reg = (volatile uint32_t*) GPIO_OUT_W1TC_REG;
   volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;

    // Set the mode for GPIO5 to output
   *gpio_enable_reg = (1 << GPIO5);
   *gpio_out_w1tc_reg = (1 << GPIO5);
   */

   //*(int*)(0x3FF40000) = 10;

   //char x[] = "abc\n";
   //char* y = (char*)malloc(10);
   for(int i = 0; i < 1000; i++)
   {
      //print("hello\n", 7);
   }
   
   while(1)
   {
      *(char*)(UART_0_TXFIFO_REG) = 'a';
      //void* x = malloc(10);
   }
}

