#include "process.h"

void unit_time(void) {
	volatile unsigned long i;
		for(i = 0; i < 1000000UL; ++i);
}
void set_priority(pid_t pid, int priority) {
	struct sched_param param;
    	param.sched_priority = priority;
	if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1){
		ERR_EXIT("set scheduler error");
	}
}
/**
int* share_var(char filename[]) { 
	int shfd = shm_open(filename , O_RDWR | O_CREAT, 0777);	
	if (shfd<0) { perror("shm_open"); exit(EXIT_FAILURE); };
	ftruncate(shfd , sizeof(int));
	int ad = (int)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shfd, (off_t)0);
	if (ad==MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); };
	return ad;
}
**/
void assign_cpu(pid_t pid, int core) {
	if(core > sizeof(cpu_set_t)) 
		ERR_EXIT("Core error\n");
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
		
	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) 
		ERR_EXIT("Set affinity error\n");
}
int min(int a, int b) {return a < b? a: b;}
