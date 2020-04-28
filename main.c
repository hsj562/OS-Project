#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <sys/syscall.h>
#include "process.h"

void assign_FIFO(process *cur_proc) {
	pid_t cpid = fork();
	assign_cpu(cpid, 1);	// set child process cpu = 1
	set_priority(cpid, 50);
	if(cpid == 0) { // child process
		long long start_time = syscall(333);
		for(int i = 0; i < cur_proc->exec_time ; i++)	unit_time();
		long long end_time = syscall(333);
		syscall(334,getpid(), start_time, end_time);
		printf("%s %d\n", cur_proc->p_name, getpid());
		fflush(stdout);
		exit(0);		
	}
	cur_proc->pid = cpid;
	cur_proc->status = 1;	// ready
}

void assign_RR(process *cur_proc) {
	pid_t cpid = fork();
	if(cpid == 0) { // child process
		//printf("start%d\n", getpid());
		assign_cpu(getpid(), 1);	// set child process cpu = 1
		set_priority(getpid(), 99);
		long long start_time = syscall(333);
		for(int i = 0; i < cur_proc->exec_time; i++) {
			if(i != 0 || i != cur_proc->exec_time-1) {
				if(i % 500 == 0) {
					sched_yield();
				}
			}
			unit_time();
		}
		long long end_time = syscall(333);
		syscall(334,getpid(), start_time, end_time);
		printf("%s %d\n", cur_proc->p_name, getpid());
		fflush(stdout);
		exit(0);		
	}
	//printf("create %d\n", cpid);
	//fflush(stdout);
}

void scheduler_FIFO(int N, process proc[]) {
	int ass_task = 0;
	int next_task = 0;
	int timer = 0;
	while(ass_task < N) {
		while(proc[next_task].ready_time > timer) {
			unit_time();
			timer++;
		}
		while(next_task < N && timer >= proc[next_task].ready_time) {
			assign_FIFO(&proc[next_task]);			
			wait(NULL);
			timer += proc[next_task].exec_time;		
			next_task++;
		}
		ass_task = next_task;
	}
}

void scheduler_RR(int N, process proc[]) {
	int ass_task = 0;
	int next_task = 0;
	int timer = 0;
	while(ass_task < N) {
		while(proc[next_task].ready_time > timer) {
			unit_time();
			timer++;
		}		
		while(next_task < N && timer >= proc[next_task].ready_time) {
			assign_RR(&proc[next_task]);
			next_task++;
		}
		ass_task = next_task;
		unit_time();
		timer++;
	}
	for(int i = 0; i < N; ++i) wait(NULL);
}
int select_task(int N, process proc[], int *timer) {
	for(int i = 0; i < N; ++i) 
		if(proc[i].status == 0 && proc[i].ready_time <= *timer) proc[i].status = 1;
	int next = -1;
	int min_exec = 1<<30;
	for(int i = 0; i < N; ++i) {
		if(proc[i].status == 1) {
			if(min_exec > proc[i].exec_time) {
				next = i;
				min_exec = proc[i].exec_time;
			}	
		}
	}
	return next;
}

void scheduler_SJF(int N, process proc[]) {
	int next_task = 0;
	int timer = 0;
	int finish = 0;
	while(finish < N) {
		next_task = select_task(N, proc, &timer);
		while(finish < N && next_task == -1) {
			unit_time();
			timer++;
			next_task = select_task(N, proc, &timer);
		}
		//printf("select: %d\n", next_task);
		assign_FIFO(&proc[next_task]);
		set_priority(proc[next_task].pid, 99);		
		timer += proc[next_task].exec_time;
		finish++;
		wait(NULL);
		proc[next_task].status = 2;
	}
}
void scheduler_PSJF(int N, process proc[]) {
	int next_task = 0;
	int cur_task = 0;
	int timer = 0;
	int running_time = 0;
	int finish = 0;
	while(timer < proc[next_task].ready_time) {
		unit_time();
		timer++;
	}
	while(finish < N) {
	/**
		printf("timer = %d\n", timer);
		for(int i = 0; i < N; ++i) {
			printf("id %d exec %d\n", i, proc[i].exec_time);
		}
	**/
		if(next_task == N) {
			running_time = proc[cur_task].exec_time;
		} else if(next_task > 0) {
			running_time = min(proc[next_task].ready_time - timer, proc[cur_task].exec_time);
		}
		proc[cur_task].exec_time -= running_time;
		for(int r = 0; r < running_time; ++r) unit_time();
		if(proc[cur_task].exec_time == 0) {
			wait(NULL);
			proc[cur_task].status = 2;
			finish++;
			//printf("finish %d\n", finish);
		} else {
			set_priority(proc[cur_task].pid, 50);
		}
		timer += running_time;
		running_time = 0;
		while(next_task < N && timer >= proc[next_task].ready_time) {
			assign_FIFO(&proc[next_task]);
			next_task++;
		}
		if(finish == N) continue;
		cur_task = select_task(N, proc, &timer);
		//printf("select %d\n", cur_task);
		set_priority(proc[cur_task].pid, 99);		
	}

}
int main() {
	char policy[16];
	int N;
	scanf("%s %d", policy, &N);
	process proc[128];
	for(int i = 0; i < N; ++i) {
		scanf("%s %d %d", proc[i].p_name, &proc[i].ready_time, &proc[i].exec_time);
		proc[i].status = 0;
	}
	assign_cpu(getpid(), 0);	// set scheduler core = 0
	set_priority(getpid(), 99);
	// sort proc by ready time
	if(strcmp(policy, "FIFO") == 0) {
		scheduler_FIFO(N, proc);	
	} else if(strcmp(policy, "RR") == 0) {
		scheduler_RR(N, proc);	
	} else if(strcmp(policy, "SJF") == 0) {
		scheduler_SJF(N, proc);	
	} else if(strcmp(policy, "PSJF") == 0) {
		scheduler_PSJF(N, proc);	
	} else {
		ERR_EXIT("No matched policy found!\n");
	}
	return 0;
}
