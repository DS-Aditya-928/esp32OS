#include "MMU.h"

#define DPORT_PRO_CACHE_CTRL1_REG 0x3FF00044

void MMU::init()
{
    unsigned int st2 = *(unsigned int*)(DPORT_PRO_CACHE_CTRL1_REG);
    st2 &= ~(1 << 3);//only enable dram1 access
    *(unsigned int*)(DPORT_PRO_CACHE_CTRL1_REG) = st2;
}