#ifndef BN_H
#define BN_H

#ifndef __DOXYGEN_IGNORE__

#include "athena/Types.hpp"
int  bn_compare(atUint8* a, atUint8* b, atUint32 n);
void bn_sub_modulus(atUint8* a, atUint8* N, atUint32 n);
void bn_add(atUint8* d, atUint8* a, atUint8* b, atUint8* N, atUint32 n);
void bn_mul(atUint8* d, atUint8* a, atUint8* b, atUint8* N, atUint32 n);
void bn_exp(atUint8* d, atUint8* a, atUint8* N, atUint32 n, atUint8* e, atUint32 en);
void bn_inv(atUint8* d, atUint8* a, atUint8* N, atUint32 n);

#endif // __DOXYGEN_IGNORE__
#endif // BN_H
