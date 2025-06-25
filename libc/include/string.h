#define __libc 1
#include <stddef.h>



int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
void* malloc(size_t);
size_t strlen(const char*);