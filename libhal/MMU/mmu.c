#include "MMU.h"

#define FLASHRAM 0x3F800000
#define FLASHRAMSIZE 0x8000

#define DPORT_PRO_CACHE_CTRL1_REG 0x3FF00044

void cMMU::init()
{
    unsigned int st2 = *(unsigned int*)(DPORT_PRO_CACHE_CTRL1_REG);
    st2 &= ~(1 << 3);//only enable dram1 access
    *(unsigned int*)(DPORT_PRO_CACHE_CTRL1_REG) = st2;
    //also set up basic heap md. entry at 0x8000 - 2 should be number of entries. start at 1.
     *(unsigned short*)(getHeapStart() + getHeapSize() - 2) = 1;
     //entry 4 bytes prev (s of md struct) should be 0, 0x8000 - 2 - 4 * -1 because its free
     // this should be 6 bytes before the end. short takes up 2 + 2*2 shorts of struct
     heapMD m = {.regAddr = 0, .regLength = (-1*(0x8000 - 6))};
     *(heapMD*)(getHeapStart() + getHeapSize() - 6) = m;

     //done??
}

unsigned int cMMU::getHeapStart()
{
    return(FLASHRAM);
}

unsigned short cMMU::getHeapSize()
{
    return(FLASHRAMSIZE);
}