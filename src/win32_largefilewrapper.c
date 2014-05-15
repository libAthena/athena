#include "win32_largefilewrapper.h"
#include <stdio.h>

#ifdef _WIN32
int fseeko64(FILE* fp, off64_t offset, int whence)
{
    return _fseeki64(fp, offset, whence);
}

int ftello64(FILE* fp)
{
    return _ftelli64(fp);
}
#endif
