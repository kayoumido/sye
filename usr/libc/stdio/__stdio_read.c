#include <stdio_impl.h>
#include <sys/uio.h>
#include <syscall.h>

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{

	sys_read(f->fd, buf, len);

	return len;
}
