#include "string.h"

#define UNALIGNED_X_Y(X, Y) \
  (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLE_BLOCK_SIZE (sizeof (long))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIG_BLOCK_SIZE (sizeof (long) << 2)

/* Threshold for punting to the little block byte copier.  */
#define TOO_SMALL_LITTLE_BLOCK(LEN)  ((LEN) < LITTLE_BLOCK_SIZE)

/* Threshold for punting to the big block byte copier.  */
#define TOO_SMALL_BIG_BLOCK(LEN)  ((LEN) < BIG_BLOCK_SIZE)

void* memmove(void *dst_void,
        const void *src_void,
        size_t length)
{
    char *dst = (char*)dst_void;
    const char *src = (char*)src_void;
    long *aligned_dst;
    const long *aligned_src;

    if (src < dst && dst < src + length) {
        /* Destructive overlap...have to copy backwards */
        src += length;
        dst += length;

        if (!TOO_SMALL_LITTLE_BLOCK(length) && !UNALIGNED_X_Y(src, dst)) {
            aligned_dst = (long*)dst;
            aligned_src = (long*)src;

            /* Copy one long word at a time if possible.  */
            while (!TOO_SMALL_LITTLE_BLOCK(length)) {
                *--aligned_dst = *--aligned_src;
                length -= LITTLE_BLOCK_SIZE;
            }

            /* Pick up any residual with a byte copier.  */
            dst = (char*)aligned_dst;
            src = (char*)aligned_src;
        }

        while (length--) {
            *--dst = *--src;
        }
    } else {
        /* Use optimizing algorithm for a non-destructive copy to closely
           match memcpy. If the size is small or either SRC or DST is unaligned,
           then punt into the byte copy loop.  This should be rare.  */
        if (!TOO_SMALL_LITTLE_BLOCK(length) && !UNALIGNED_X_Y(src, dst)) {
            aligned_dst = (long*)dst;
            aligned_src = (long*)src;

            /* Copy 4X long words at a time if possible.  */
            while (!TOO_SMALL_BIG_BLOCK(length)) {
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                *aligned_dst++ = *aligned_src++;
                length -= BIG_BLOCK_SIZE;
            }

            /* Copy one long word at a time if possible.  */
            while (!TOO_SMALL_LITTLE_BLOCK(length)) {
                *aligned_dst++ = *aligned_src++;
                length -= LITTLE_BLOCK_SIZE;
            }

            /* Pick up any residual with a byte copier.  */
            dst = (char*)aligned_dst;
            src = (char*)aligned_src;
        }

        while (length--) {
            *dst++ = *src++;
        }
    }

    return dst_void;
}