#include <syscall.h>
#include <unistd.h>

extern char **__environ;

int execv_safe(const char *path, char *const argv[], const char *password) {
  return sys_execve(path, argv, __environ, password);
}
