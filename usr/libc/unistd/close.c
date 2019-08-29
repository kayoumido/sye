#include <unistd.h>
#include <errno.h>
#include <syscall.h>
#include <libc.h>

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int close(int fd)
{
	fd = __aio_close(fd);

	return sys_close(fd);

}
