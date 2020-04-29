#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_print_time(int pid, long long st, long long ft) {
	long long st_s = st / 1000000000;
	long long st_ns = st % 1000000000;
	long long ft_s = ft / 1000000000;
	long long ft_ns = ft % 1000000000;
	printk("[Project1] %d %lld.%.9lld %lld.%.9lld\n", pid, st_s, st_ns, ft_s, ft_ns);
}
