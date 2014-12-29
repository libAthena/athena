#ifndef WIN32_LARGEFILEWRAPPER_H
#define WIN32_LARGEFILEWRAPPER_H

#if defined(_MSC_VER)
#include <stdio.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#ifndef off64_t
typedef uint64_t off64_t;
#endif

int fseeko64(FILE* fp, off64_t offset, int whence);
off64_t ftello64(FILE* fp);
#ifdef __cplusplus
}
#endif
#endif

#endif // WIN32_LARGEFILEWRAPPER_H
