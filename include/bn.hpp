#ifndef BN_H
#define BN_H

#include "athena/Types.hpp"
namespace bignum
{
int compare(const atUint8* a, const atUint8* b, atUint32 n);
void subModulus(atUint8* a, const atUint8* N, atUint32 n);
void add(atUint8* d, atUint8* a, const atUint8* b, const atUint8* N, atUint32 n);
void mul(atUint8* d, atUint8* a, const atUint8* b, const atUint8* N, atUint32 n);
void exp(atUint8* d, const atUint8* a, const atUint8* N, atUint32 n, atUint8* e, atUint32 en);
void inv(atUint8* d, atUint8* a, const atUint8* N, atUint32 n);
}

#endif // BN_H
