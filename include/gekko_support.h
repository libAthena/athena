#ifndef GEKKO_SUPPORT_H
#define GEKKO_SUPPORT_H

#ifdef GEKKO
#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
char *
realpath(const char *path, char *resolved);
int
vsnprintf (char *s, size_t n, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif
#endif

#endif // GEKKO_SUPPORT_H
