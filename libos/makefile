CFLAGS?=-g

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -mtext-section-literals
CPPFLAGS:=$(CPPFLAGS) -Iinclude -I../include
LIBK_CFLAGS:=$(CFLAGS) -L../libc -Lk
LIBK_CPPFLAGS:=$(CPPFLAGS)

FREEOBJS=\
SCHEDULER/scheduler.o \

HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libos.o)

BINARIES=libos.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libos.o .c .s

all: $(BINARIES)

libos.a: $(LIBK_OBJS)
	xtensa-esp32-elf-ar rcs $@ $(LIBK_OBJS)

.cpp.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.cpp.s:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.cpp.libos.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libos.o:
	xtensa-esp32-elf-g++ -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)