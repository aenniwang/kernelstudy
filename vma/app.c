#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int fd;
	void *v_addr;
	printf("\nApplication to test mmap usage.\n");
	fd = open("/dev/vma",O_RDWR);
	if(fd == -1)
	{
		printf("cannot access /dev/vma\n");
		return -1;
	}
	
	v_addr = mmap(0,1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	printf("v_addr is 0x%08x\n",(int)v_addr);
	return 0;
}

