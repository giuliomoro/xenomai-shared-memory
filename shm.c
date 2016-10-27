#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include "stru.h"
#include <native/task.h>

int main (int argc, char *argv[])
{
	rt_print_auto_init(1); // enable rt_printf output 
	// turn ourselves into a xenomai task, in primary mode
	rt_task_shadow(NULL, "main", 95, T_FPU);
	int fd;
	int count = 0;
	// this will switch us to secondary mode
	//fd = __wrap_shm_open(SHM_NAME, O_RDWR | O_CREAT | O_DIRECT, 0); // this would probably fix it, but may require a different kernel? currently returns error
	fd = __wrap_shm_open(SHM_NAME, O_RDWR | O_CREAT, 0);
	if(fd < 0){
		printf("%s\n", strerror(errno));
		return 1;
	}

	rt_printf("Opened\n");
	size_t len = sizeof(stru);
	ftruncate(fd, len);
	rt_printf("size: %d\n", len);
	//this causes a switch to secondary mode 
	void* ignored;
	void* addr = __wrap_mmap(ignored, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED){
		rt_printf("Error: %s %p\n", strerror(errno));
		return 1;
	}
	rt_printf("mmapped\n");

	// can now force to go back to primary mode
	//rt_task_shadow(NULL, "main", 95, T_FPU);
	stru* str = (stru*)addr;

	//accessing the mapped memory can be done without switching mode
	rt_printf("set name\n");
	snprintf(str->name, STRU_NAME_SIZE, "my shared name");

	rt_printf("sleeping...\n");
	rt_task_sleep(50000000); //sleep 5 seconds
	unsigned int n = 0;
	unsigned int j = 0;
	rt_printf("accessing mapped memory\n");
	for(j = 0; j < 10; ++j){
		for(n = 0; n < STRU_VALUES_SIZE; ++n){
			// from here we should be able to access memory without mode switches
			// so if this line was to cause switches, you would 
			// see them incrementing
			str->values[n] = (float)n;
			rt_printf("accessed %d\n", ((void*)&str->values[n]) - addr );
		}
		rt_task_sleep(200000000);  // sleep, switches to primary mode
	}
	//usleep(40000000);
	rt_task_sleep(5000000000); //sleep 5 seconds
	// uncommenting the following line will make the mapped 
	//memory file unaccessible upon exit
	//shm_unlink(SHM_NAME); printf("Unlinked\n");
	//fd can be closed as soon as it has been mmapped
	close(fd);
	return 0;
}
