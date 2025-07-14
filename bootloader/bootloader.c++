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

void init_task_context(TaskContext* ctx, void (*entry)(void), void* stack_top) {
    // Zero out all registers
    for (int i = 0; i < 16; i++) ctx->regs[i] = 0;

    // Set a1 = stack pointer (aligned 16 bytes)
    ctx->regs[1] = ((uint32_t)stack_top) & ~0xF;

    // Set a0 = return address, in case task ever returns (optional)
    ctx->regs[0] = (uint32_t)task_exit;  // a safe infinite loop or crash handler

    // Manually inject initial PC by faking a yield-to-entry
    // On resume, instead of `ret`, jump to entry:
    ctx->regs[2] = (uint32_t)entry;  // Save entry address for bootstrapping
}

void* oldSP;

extern "C" void  __attribute__((noreturn)) call_start_cpu0(void)
{
    WDT::disableBootProtection(WDT::RTC);
    WDT::disableBootProtection(WDT::TIMG0);
    cMMU::init();
    UART::setActive(UART::UART0);
    UART::init();

    UART::print("Kernel loaded!\r\n");
    UART::print("Compiled on "); UART::print(__DATE__); UART::print(" at "); UART::print(__TIME__); UART::print(".\r\n");

    void* oldSP;

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

    void* newStack = (void*)(malloc(2048) + 2048); // Allocate 2kB of memory for the new stack
    regs1[1] = (uint32_t)newStack; // Set a1 to the new stack pointer
    regs1[9] = (uint32_t)testFunc1; // Set a9 to the entry point of task1
    bool fCall = true;
    //yield(&dummyTask, task1); // Start with task1
    int x = 0;
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
}