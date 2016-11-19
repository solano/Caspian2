#! /bin/sh

ddd &
qemu-system-i386 -s -S -kernel build/kernel/kernel.bin
