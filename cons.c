#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include "stru.h"
//#include <native/task.h>

int main()
{
	int fd = shm_open(SHM_NAME, O_RDONLY, 0);
	if(fd < 0){
		printf("%s\n", strerror(errno));
		return 1;
	}
	printf("Opened\n");
	size_t len = sizeof(stru);
	void* addr = mmap(0, len, PROT_READ, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED){
		printf("Error: %s %p\n", strerror(errno));
		return 1;
	}
	close(fd);
	printf("mmapped\n");
	stru* str = (stru*)addr;
	printf("sharedname %s\n", str->name);
	printf("struct name: %s\n", str->name);
	unsigned int n = 0;
	int count = 0;
	while(++count < 10){
		for(n = 0; n < STRU_VALUES_SIZE; ++n){
			printf("values[%d]: %.3f\n", n, str->values[n]);
		}
		usleep(1000000);
	}
	
}
