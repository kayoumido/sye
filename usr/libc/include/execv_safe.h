#ifndef _EXECV_SAFE_H
#define _EXECV_SAFE_H

#ifdef __cplusplus
extern "C" {
#endif

int execv_safe(const char *path, char *const argv[], const char *password);

#ifdef __cplusplus
}
#endif
#endif
