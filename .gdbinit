file build/kernel/kernel.bin
target remote localhost:1234
set directories kernel
b ktest
