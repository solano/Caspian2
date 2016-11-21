DEFAULT_HOST:=i586-elf
HOST?=DEFAULT_HOST
HOSTARCH:=i386

CFLAGS?=-g -O2
CPPFLAGS?=
LDFLAGS?=
ASFLAGS?=
LIBS?=

BUILD_DIR?=build
KERNELBUILD:=$(BUILD_DIR)/kernel
SYSROOT?=sysroot

# Will be inside sysroot
BOOTDIR?=/boot
INCLUDEDIR?=/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
ASFLAGS:=$(ASFLAGS) -g -f elf
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

# Kernel and architecture-specific object-files
KERNEL_OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/string.o \
kernel/kio.o \

OBJS:=\
$(ARCHDIR)/boot.o \
$(ARCHDIR)/tty.o \
$(KERNEL_OBJS)

.PHONY: all clean install install-headers
.SUFFIXES: .o .c .s

all: $(KERNELBUILD)/kernel.bin

$(KERNELBUILD)/kernel.bin: $(OBJS)
	@echo "Linking everything into $@..."
	@cd .. && $(LD) $(LDFLAGS) -T kernel/$(ARCHDIR)/linker.ld -o $@ $(addprefix $(KERNELBUILD)/,$^)
	@echo "Linked. Checking whether file is bootable..."
	@grub-file --is-x86-multiboot $@
	@echo "File OK."

%.o: %.c
	@echo "Compiling $<"
	@mkdir -p $(dir $(KERNELBUILD)/$@)
	@$(CC) -c -MD $< -o $(KERNELBUILD)/$@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	@echo "Assembling $<"
	@mkdir -p $(dir $(KERNELBUILD)/$@)
	@$(AS) -MD $(KERNELBUILD)/$(@:.o=.d) -o $(KERNELBUILD)/$@ $(ASFLAGS) $<

clean:
	@rm -rf $(KERNELBUILD)

install: install-headers install-kernel

install-headers:
	@mkdir -p $(SYSROOT)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(SYSROOT)$(INCLUDEDIR)/.

install-kernel: $(KERNELBUILD)/kernel.bin
	@mkdir -p $(SYSROOT)$(BOOTDIR)
	@cp $(KERNELBUILD)/kernel.bin $(SYSROOT)$(BOOTDIR)

-include $(=$(KERNELBUILD):OBJS:.o=.d)
