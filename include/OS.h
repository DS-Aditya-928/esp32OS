#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "string.h"

#define MAX_TASKS 32

struct TaskInfo 
{
    uint32_t registers[16]; // General-purpose registers
    bool isActive = false;
    uint32_t stackPointer;
};

class dsOS
{
private:
    static int curTask; // Current task index
    static int taskCount; // Total number of tasks
    static TaskInfo taskData[MAX_TASKS];
protected:
    static void hiddenYield(uint32_t* cRegs);

    friend void yield();
public:
    static bool createTask(void (*entry)(void), uint32_t stackSize);
    static void startScheduler();
};

inline  __attribute__((always_inline)) void yield()
{
    /*
    1.) Save all the current function's registers
    2.) Call hiddenYield to save ret point and load second task's registers. hiddenYield will also jump.
    */
    uint32_t regBuf[16]; // Buffer to save registers
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
        "s32i  a9,  %0, 36\n"
        "s32i  a10, %0, 40\n"
        "s32i  a11, %0, 44\n"
        "s32i  a12, %0, 48\n"
        "s32i  a13, %0, 52\n"
        "s32i  a14, %0, 56\n"
        "s32i  a15, %0, 60\n"
        :
        : "r"(regBuf)             // %0 = C variable regs
        :      
        );
        dsOS::hiddenYield(regBuf);
        //loading in of all of task 2's registers happens in hiddenYield
}