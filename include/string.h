#ifndef __STRING_H__
#define __STRING_H__
#define __libc 1
#include <stddef.h>


extern "C" int memcmp(const void*, const void*, size_t);
extern "C" void* memcpy(void* __restrict, const void* __restrict, size_t);//implicit calls need it 2 be unmangled
extern "C" void* memmove(void*, const void*, size_t);
extern "C" void* memset(void*, int, size_t);
extern "C" void* malloc(size_t);
extern "C" void free(void*);
extern "C" size_t strlen(const char*);
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* p) noexcept;
void operator delete(void* p, size_t size) noexcept;
void operator delete[](void* p) noexcept;
void operator delete[](void* p, size_t size) noexcept;

void getHeapMD();

#endif // __STRING_H__