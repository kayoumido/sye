#include <unistd.h>
#include <syscall.h>

int pipe(int fd[2])
{
	return sys_pipe(fd);
}
