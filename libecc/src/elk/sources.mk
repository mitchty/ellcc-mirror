CFLAGS += -Werror -Wall -std=c99 -D_XOPEN_SOURCE=700 -D_BSD_SOURCE
CFLAGS += -I$(SRCPATH)/$(LIB)/include/$(ARCH) \
          -I$(SRCPATH)/$(LIB)/include

VPATH := $(VPATH):$(SRCPATH)/$(LIB)/$(ARCH)
# Startup code.
CRTSRCS += crt1.S
# Target specific code.
SRCS.arm += arm_gic.c arm_sp804.c
SRCS.i386 +=
SRCS.microblaze +=
SRCS.mips +=
SRCS.ppc +=
SRCS.x86_64 +=
SRCS += $(SRCS.$(ARCH))

VPATH := $(VPATH):$(SRCPATH)/$(LIB)/kernel
# Target independent code.
SRCS += simple_console.c console.c simple_exit.c simple_memman.c semaphore.c \
	command.c test.c timer.c irq.c scheduler.c
