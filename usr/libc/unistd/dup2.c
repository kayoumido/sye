#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syscall.h>

int dup2(int old, int new)
{
	return sys_dup2(old, new);
}
