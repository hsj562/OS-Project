#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ERR_EXIT(msg) {char _buf[BUFSIZ]; sprintf(_buf, "%s", msg); perror(_buf); exit(127);}

typedef struct process {
	char p_name[64];
	int ready_time;
	int exec_time;
	pid_t pid;
	int status;
} process;

void unit_time(void);
void set_priority(pid_t pid, int priority);
//int* share_var(char filename[]);
void assign_cpu(pid_t pid, int core);
int min(int a, int b);
