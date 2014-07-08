How to compile the kernel module:

gcc -D_KERNEL -m64 -mcmodel=kernel -mno-red-zone -ffreestanding -nodefaultlibs -c kbp-test.c
ld -r -o kbp-test kbp-test.o

modload kbp-test
