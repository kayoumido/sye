#include <syscall.h>

void hidestr(char* str, size_t sz) {
	sys_hidestr(str, sz);
}
