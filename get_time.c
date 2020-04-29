#include <linux/ktime.h>
#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage long long sys_get_time(void) {
	struct timespec t;
	getnstimeofday(&t);
	return (long long)t.tv_sec * 1000000000 + (long long)t.tv_nsec;
}

