#include <unistd.h>
#include <syscall.h>

int execve(const char *path, char *const argv[], char *const envp[])
{
	/* do we need to use environ if envp is null? */

	return sys_execve(path, argv, envp);
}
