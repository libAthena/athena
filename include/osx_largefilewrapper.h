#ifndef OSX_LARGEFILEWRAPPER_H
#define OSX_LARGEFILEWRAPPER_H

#if defined(__APPLE__) || defined(GEKKO) || defined(__FreeBSD__)
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
int fseeko64(FILE* fp, off_t offset, int whence);
off_t ftello64(FILE* fp);
#ifdef __cplusplus
}
#endif
#endif

#endif // OSX_LARGEFILEWRAPPER_H
