#if defined(__APPLE__) || defined(GEKKO)
#include "osx_largefilewrapper.h"
#include <stdio.h>

int fseeko64(FILE* fp, off_t offset, int whence)
{
    return fseeko(fp, offset, whence);
}

off_t ftello64(FILE* fp)
{
    return ftello(fp);
}
#endif


