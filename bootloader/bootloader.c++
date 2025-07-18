#include <stdint.h>
#include <string.h>
#include <UART.h>
#include <WDT.h>
#include <MMU.h>

int curTask = 1;

typedef struct {
    uint32_t regs[16];  // a0–a15
} TaskContext;

TaskContext* task1 = 0;
TaskContext* task2 = 0;

uint32_t regs1[16]; 
uint32_t regs2[16];

void yield2()
{
    //save a0 of task 2
    regs2[0] = 0x40080208;
    /*
    asm volatile (
        "s32i  a0,  %0,  0\n"
        :
        : "r"(0x400801fc)             // %0 = C variable regs
        :
    );
    */
}
    
void yield1()
{
    //save a0 of task 1 to a9 of regs1, so when its loaded by the asm in the main loop, auto jmp to this address.
    regs1[9] = 0x400800b0;
    /*
    asm volatile (
        "s32i  a0,  %0,  36\n"
        :
        : "r"(regs1)             // %0 = C variable regs
        :
    );
    */
}

void testFunc1(void)
{
    int x = 0;
    while(true)
    {
        UART::print("1 ");
        UART::print(x);
        UART::print("\r\n");
        x++;

        asm volatile (
        "s32i  a0,  %0,  0\n"
        "s32i  a1,  %0,  4\n"
        "s32i  a2,  %0,  8\n"
        "s32i  a3,  %0, 12\n"
        "s32i  a4,  %0, 16\n"
        "s32i  a5,  %0, 20\n"
        "s32i  a6,  %0, 24\n"
        "s32i  a7,  %0, 28\n"
        "s32i  a8,  %0, 32\n"
        "s32i  a9,  %0, 36\n"   // a9 contains original pointer
        "s32i  a10, %0, 40\n"
        "s32i  a11, %0, 44\n"
        "s32i  a12, %0, 48\n"
        "s32i  a13, %0, 52\n"
        "s32i  a14, %0, 56\n"
        "s32i  a15, %0, 60\n"
        :
        : "r"(regs1)             // %0 = C variable regs
        :      
        );

        //saved all of f1s regs.
        yield1();//now a9 of regs1 holds the return point of task 1.
        //load in all of task 2's registers
        asm volatile (
        "l32i a0,  %0,  0\n"
        "l32i a1,  %0,  4\n"
        "l32i a2,  %0,  8\n"
        "l32i a3,  %0, 12\n"
        "l32i a4,  %0, 16\n"
        "l32i a5,  %0, 20\n"
        "l32i a6,  %0, 24\n"
        "l32i a7,  %0, 28\n"
        "l32i a9,  %0, 36\n"
        "l32i a10, %0, 40\n"
        "l32i a11, %0, 44\n"
        "l32i a12, %0, 48\n"
        "l32i a13, %0, 52\n"
        "l32i a14, %0, 56\n"
        "l32i a15, %0, 60\n"
        "jx a0\n" // Jump to the ret? should crash rn, should be this + whatever the asm takes up.
        :
        : "r"(regs2)             // %0 = C variable regs
        :  // Clobber list: a9 and memory
        );
    }
}

void* oldSP;

extern "C" void isr(void)
{
    UART::print("ISR called!\r\n");
}

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
    WDT::disableBootProtection(WDT::RTC);
    WDT::disableBootProtection(WDT::TIMG0);
    cMMU::init();
    UART::setActive(UART::UART0);
    UART::init();

    UART::print("Kernel loaded!\r\n");
    UART::print("Compiled on "); UART::print(__DATE__); UART::print(" at "); UART::print(__TIME__); UART::print(".\r\n");

    

    /*
    __asm__ volatile ("mov %0, a1" : "=r"(oldSP));
    UART::print("\r\nStack pointer: ");
    UART::print((int)oldSP);
    UART::print("\r\n");
    UART::print("Changing stack\r\n");
    void* stackPointer = (void*)(malloc(2048) + 2048);//allocate 2kB of memory for the new stack
    UART::print("New stack pointer: ");
    UART::print((int)stackPointer);
    UART::print("\r\n");
    __asm__ volatile ("mov a1, %0" : : "r"(stackPointer));
    test2();
    
    __asm__ volatile ("mov a1, %0" : : "r"(oldSP));
    UART::print("Changing 2 stack\r\n");
    */

    /*
    void* newStack = (void*)(malloc(2048) + 2048); // Allocate 2kB of memory for the new stack
    regs1[1] = (uint32_t)newStack; // Set a1 to the new stack pointer
    regs1[9] = (uint32_t)testFunc1; // Set a9 to the entry point of task1
    bool fCall = true;
    //yield(&dummyTask, task1); // Start with task1
    int x = 0;
    */
    
    *(unsigned int*)(0x3FF5F010) = 5500000; // Set the alarm timer


    unsigned int r = *(unsigned int*)(0x3FF5F000);
    r |= (1U << 31);//enable the counter
    r |= (1U << 29);//enable autoreload
    r |= (1U << 10);//enable alarm
    r |= (1U << 11);//enable level interrupt
    
    *(unsigned int*)(0x3FF5F098) |= (1U << 0); // Enable the counter interrupt

    /*
    *(void (**)(void))(0x3FF1F014) = &isr;//write func to the isr.
    
    unsigned int iMap = *(unsigned int*)(0x3FF0013C);
    iMap = (iMap & ~0x1F) | 0x1F; // set the first 5 bits of the interrupt map
    *(unsigned int*)(0x3FF0013C) = iMap; // Write back the modified interrupt map

    *(unsigned int*)(0x3FF1F000) |= (1U << 5); // Enable interrupt processing
    */
    *(unsigned int*)(0x3FF1F058) |= 1U;
    
    unsigned int iMap = *(unsigned int*)(0x3FF0013C);
    iMap = (iMap & ~0x1F) | (1 & 0x1F);; // set the first 5 bits of the interrupt map to 1 i.e 1
    *(unsigned int*)(0x3FF0013C) = iMap; // Write back the modified interrupt map
    *(unsigned int*)(0x3FF1F000) |= (1U << 1); // Enable interrupt processing 

    *(unsigned int*)(0x3FF5F000) = r;
    for(int i = 0; i < 7; i++)
    {
       void (**fLoc)() = *(void (***)())(0x3FF1F004 + (i * 4));
        *fLoc = isr;
    }
    
    while(true)
    {
        /*
        *(unsigned int*)(0x3FF5F00C) = 1;//cpy value to counter regs
        unsigned int lowBits = *(unsigned int*)(0x3FF5F004);
        unsigned int highBits = *(unsigned int*)(0x3FF5F008);
        UART::print("Counter: ");   
        unsigned long long counter = ((unsigned long long)highBits << 32) | lowBits;
        UART::print(counter);
        UART::print("\r\n");
        */
        bool intTrig = (bool)((*(unsigned int*)(0x3FF000EC) >> 14) & 1U);

        unsigned int currInt = *(unsigned int*)(0x3FF1F028);
        UART::print("Current interrupt status: ");
        UART::print(currInt);
        UART::print("\r\n");
        
        //unsigned int tS = *(unsigned int*)(0x3FF000EC);
        //tS = (tS >> 14) & 1U;
        //UART::print(tS);
        //UART::print("\r\n");
        //check int status
        if(intTrig) // Read the interrupt status register (interrupt matrix gets it)
        {
            UART::print("Counter interrupt!\r\n");
            
            *(unsigned int*)(0x3FF5F0A4) |= (1U); // Clear the interrupt status
            unsigned int r = *(unsigned int*)(0x3FF5F000);
            r |= (1U << 10);
            *(unsigned int*)(0x3FF5F000) = r; // Set the bit to enable the alarm
        }
    }

    while(true)
    {

    }
    
    /*
    while(1)
    {    
        UART::print("2 ");
        UART::print(x);
        UART::print("\r\n");
        x++;
        //save all the registers manually
        asm volatile (
        "s32i  a0,  %0,  0\n"
        "s32i  a1,  %0,  4\n"
        "s32i  a2,  %0,  8\n"
        "s32i  a3,  %0, 12\n"
        "s32i  a4,  %0, 16\n"
        "s32i  a5,  %0, 20\n"
        "s32i  a6,  %0, 24\n"
        "s32i  a7,  %0, 28\n"
        "s32i  a8,  %0, 32\n"
        "s32i  a9,  %0, 36\n"   // a9 contains original pointer
        "s32i  a10, %0, 40\n"
        "s32i  a11, %0, 44\n"
        "s32i  a12, %0, 48\n"
        "s32i  a13, %0, 52\n"
        "s32i  a14, %0, 56\n"
        "s32i  a15, %0, 60\n"
        :
        : "r"(regs2)             // %0 = C variable regs
        :      
        );

        yield2();//this saves the return point.
        //now, regs2 holds all the registers of task 2
        //now the switch
        //load all of task 1's registers
        asm volatile (
        "l32i a0,  %0,  0\n"
        "l32i a1,  %0,  4\n"
        "l32i a2,  %0,  8\n"
        "l32i a3,  %0, 12\n"
        "l32i a4,  %0, 16\n"
        "l32i a5,  %0, 20\n"
        "l32i a6,  %0, 24\n"
        "l32i a7,  %0, 28\n"
        "l32i a9,  %0, 36\n"
        "l32i a10, %0, 40\n"
        "l32i a11, %0, 44\n"
        "l32i a12, %0, 48\n"
        "l32i a13, %0, 52\n"
        "l32i a14, %0, 56\n"
        "l32i a15, %0, 60\n"
        "jx a9\n" // Jump to the entry point of task1
        :
        : "r"(regs1)             // %0 = C variable regs
        :  // Clobber list: a9 and memory
        );
    }
    */
}