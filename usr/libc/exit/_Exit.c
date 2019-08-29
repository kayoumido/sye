#include <stdlib.h>
#include <syscall.h>

_Noreturn void _Exit(int ec)
{
	sys_exit(ec);
}
