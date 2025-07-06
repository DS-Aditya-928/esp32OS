#ifndef dsmmu
#define dsmmu

typedef struct
{
    unsigned short regAddr;
    short regLength;//negative if free
} heapMD;

class cMMU
{
public:
    static void init();//map basic
    static unsigned int getHeapStart();
    static unsigned short getHeapSize();
};

#endif