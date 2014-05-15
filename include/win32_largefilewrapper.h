#ifndef WIN32_LARGEFILEWRAPPER_H
#define WIN32_LARGEFILEWRAPPER_H

#ifdef _WIN32
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
int fseeko64(FILE* fp, off64_t offset, int whence);
int ftello64(FILE* fp);
#ifdef __cplusplus
}
#endif
#endif

#endif // WIN32_LARGEFILEWRAPPER_H
