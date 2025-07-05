CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=../sysroot
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -mtext-section-literals
CPPFLAGS:=$(CPPFLAGS) -Iinclude -I../include
LIBK_CFLAGS:=$(CFLAGS) -L../libc -Lk
LIBK_CPPFLAGS:=$(CPPFLAGS)

FREEOBJS=\
UART/uart.o \
WDT/wdt.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libhal.o)

BINARIES=libhal.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libhal.o .c .s

all: $(BINARIES)

libhal.a: $(LIBK_OBJS)
	xtensa-esp32-elf-ar rcs $@ $(LIBK_OBJS)

.c.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.s:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libhal.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libhal.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)