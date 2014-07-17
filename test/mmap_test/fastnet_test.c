#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "fastnet.h"

int 
main(int argc, char *argv[])
{
	int rval = 0;
	int fd = open("/dev/fastnet", 0);
	struct foostatus fs;
	char * buf;
	rval = ioctl(fd, GETSTATUS, &fs);

	printf("The status from the kernel is %d\n", fs.status);

	if ((buf = mmap((caddr_t)NULL, 4*1024, PROT_READ, (MAP_SHARED|MAP_ALIGN), fd, (off_t)0)) == MAP_FAILED) {
		printf("Can not map, the error code is : %d\n", errno);
	} else {
		printf("Success to map the memory, buf = %s\n", buf);
		munmap(buf, 2);
	}
	close(fd);
	return (rval);
}
