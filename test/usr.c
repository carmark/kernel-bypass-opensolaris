#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define KERNEL_VA 0xffffff01e7f3c000
int main()
{
        int fd;
        char *buf;

        fd = open("/dev/kmem", O_RDONLY);
        if (fd == -1) {
                perror("open");
                return 1;
        }

        buf = mmap(0, 4096, PROT_READ, MAP_SHARED, fd, KERNEL_VA);
        if (buf == MAP_FAILED)
                perror("mmap");

        puts(buf);

        munmap(buf, 4096);
        close(fd);
        return 0;
}
