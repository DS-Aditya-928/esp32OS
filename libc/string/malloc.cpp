#include <string.h>
#include <MMU.h>
#include <UART.h>

extern "C" void* malloc(size_t s)
{
    //first print heap metadata
    unsigned int heapStart = cMMU::getHeapStart();
    unsigned short heapSize = cMMU::getHeapSize();
    unsigned short numEnt = *(unsigned short*)(heapStart + heapSize - 2);
    UART::print("Number of heap entries: "); UART::print(numEnt); UART::print("\r\n");

    for(int i = 0; i < numEnt; i++)
    {
        heapMD md = *(heapMD*)(heapStart + heapSize - 2 - ((i + 1) * sizeof(heapMD)));
        UART::print(md.regAddr);UART::print(" ");UART::print(md.regLength);UART::print("\r\n");
    }
    return(0);
}