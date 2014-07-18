This directory include the kernel driver code.
How to compile this driver:
  * gcc -D_KERNEL -m64 -mcmodel=kernel -mno-red-zone -ffreestanding -nodefaultlibs -c fastnet.c fastnet.h
  * ld -r -o fastnet fastnet.o

So you need copy the fastnet to /usr/kernel/drv/amd64/ and copy the fastnet.conf to /usr/kernel/drv/ of the test machine, and run:
  * add_drv fastnet
