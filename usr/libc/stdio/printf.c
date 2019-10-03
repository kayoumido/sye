#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int __printf(const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfprintf(stdout, fmt, ap);
	va_end(ap);

	return ret;
}

extern int __vfprintf(char *dest, const char *fmt0, va_list ap);

int printf(const char *restrict fmt, ...)
{
	va_list args;
	int ret;
	char __str[256];

	memset(__str, 0, 256);

	va_start(args, fmt);
	ret = __vfprintf(__str, fmt, args);
	va_end(args);

	/* Go to stdout */
	__printf("%s", __str);

	return ret;
}
