
#include <errno.h>
#include <syscall.h>

int thrd_sleep(const struct timespec *req, struct timespec *rem)
{
#if 0
	int ret = __syscall(SYS_nanosleep, req, rem);
#endif
	int ret = sys_nanosleep(req, rem);

	switch (ret) {
	case 0:      return 0;
	case -EINTR: return -1; /* value specified by C11 */
	default:     return -2;
	}
}
