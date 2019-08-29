#include <sys/ptrace.h>
#include <stdarg.h>
#include <unistd.h>
#include <syscall.h>

long ptrace(int req, ...)
{
#if 0 /* to be activated for ptrace */
    va_list ap;
    pid_t pid;
	void *addr, *data;
#endif
	long ret=0, result=0;

#if 0 /* to be activated for ptrace */
	va_start(ap, req);
	pid = va_arg(ap, pid_t);
	addr = va_arg(ap, void *);
	data = va_arg(ap, void *);
	va_end(ap);

	if (req-1U < 3) data = &result;
#endif
    

	/* Should call the newly created ptrace syscall :-) */
    	/* ... */
    

	if (ret < 0 || req-1U >= 3) return ret;
	return result;
}
