CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=../sysroot
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -mtext-section-literals
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -I../include
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

FREEOBJS=\
string/memmove.o \
string/memcpy.o \
string/memset.o \
string/strlen.o \
string/malloc.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

BINARIES=libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .cpp .s

all: $(BINARIES)

libc.a: $(OBJS)
	xtensa-esp32-elf-ar rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	xtensa-esp32-elf-ar rcs $@ $(LIBK_OBJS)

.cpp.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.cpp.s:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.cpp.libk.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.s.libk.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)