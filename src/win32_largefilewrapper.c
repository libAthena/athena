#include "win32_largefilewrapper.h"
#include <stdio.h>

#if defined(_WIN32) && defined(_MSC_VER)
int fseeko64(FILE* fp, off64_t offset, int whence)
{
    return _fseeki64(fp, offset, whence);
}

off64_t ftello64(FILE* fp)
{
    return _ftelli64(fp);
}
#endif
