#include <string.h>
#include <MMU.h>
#include <UART.h>

extern "C" void* malloc(size_t s)
{
    //first print heap metadata
    unsigned int heapStart = cMMU::getHeapStart();
    unsigned short heapSize = cMMU::getHeapSize();
    unsigned short numEnt = *(unsigned short*)(heapStart + heapSize - 2);
    //UART::print("Number of heap entries: "); UART::print(numEnt); UART::print("\r\n");
    
    for(int i = 1; i <= numEnt ; i++)
    {
        heapMD md = *(heapMD*)(heapStart + heapSize - 2 - (i * sizeof(heapMD)));
        //UART::print(md.regAddr);UART::print(" ");UART::print(md.regLength);UART::print("\r\n");
        if((md.regLength < 0))
        {
            if((md.regAddr + s) <= (heapSize - 2 - ((numEnt + 1) * sizeof(heapMD))))//make sure hnew region end wont clash with metadatas new entry
            {
                //modify existing md region to account for its shrining and append new region to end.
                heapMD nmd = {.regAddr = md.regAddr, .regLength = s};
                md.regAddr = md.regAddr + s;
                md.regLength = md.regLength + s;

                *(heapMD*)(heapStart + heapSize - 2 - (i * sizeof(heapMD))) = md;//modify current entry to account for its shrinkage
                *(heapMD*)(heapStart + heapSize - 2 - ((numEnt + 1) * sizeof(heapMD))) = nmd;//append new entry
                *(unsigned short*)(heapStart + heapSize - 2) = numEnt + 1;//increase number of entries by 1.
                return((void*)(nmd.regAddr + heapStart));
            }
        }
    }
    return(0);
}

extern "C" void free(void* p)
{
    unsigned int heapStart = cMMU::getHeapStart();
    unsigned short heapSize = cMMU::getHeapSize();
    unsigned short numEnt = *(unsigned short*)(heapStart + heapSize - 2);
    //UART::print("(free)Number of heap entries: "); UART::print(numEnt); UART::print("\r\n");

    for(int i = 1; i <= numEnt; i++)
    {
        heapMD md = *(heapMD*)(heapStart + heapSize - 2 - (i * sizeof(heapMD)));
        //UART::print(md.regAddr);UART::print(" ");UART::print(md.regLength);UART::print("\r\n");
        if((void*)(md.regAddr) == (p - heapStart))
        {
            //found the valid one???
            //UART::print("Found: "); UART::print((int)p); UART::print("\r\n");
            md.regLength = md.regLength * -1;
            *(heapMD*)(heapStart + heapSize - 2 - (i * sizeof(heapMD))) = md;
        }
    }
}

void getHeapMD()
{
    unsigned int heapStart = cMMU::getHeapStart();
    unsigned short heapSize = cMMU::getHeapSize();
    unsigned short numEnt = *(unsigned short*)(heapStart + heapSize - 2);
    UART::print("Number of heap entries: "); UART::print(numEnt); UART::print("\r\n");
    
    for(int i = 1; i <= numEnt; i++)
    {
        heapMD md = *(heapMD*)(heapStart + heapSize - 2 - (i * sizeof(heapMD)));
        UART::print(md.regAddr);UART::print(" ");UART::print(md.regLength);UART::print("\r\n");
    }
}


void* operator new(size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size)
{
    return malloc(size);
}

void operator delete(void* p) noexcept
{
    free(p);
}


void operator delete(void* p, size_t size) noexcept
{
    free(p);
}

void operator delete[](void* p) noexcept
{
    free(p);
}

void operator delete[](void* p, size_t size) noexcept
{
    free(p);
}