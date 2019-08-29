#include <fcntl.h>
#include <stdarg.h>
#include <syscall.h>
#include <libc.h>

int open(const char *filename, int flags, ...)
{
	mode_t mode = 0;

	if ((flags & O_CREAT) || (flags & O_TMPFILE) == O_TMPFILE) {
		va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	return sys_open(filename, flags, mode);

}


