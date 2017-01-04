#ifndef EC_H
#define EC_H
#include "athena/Types.hpp"

namespace ecc
{
void checkEC(atUint8* ng, atUint8* ap, atUint8* sig, atUint8* sigHash, bool& apValid, bool& ngValid);
void makeECCert(atUint8* cert, atUint8* sig, const char* signer, const char* name, atUint8* priv, atUint32 keyId);
void createECDSA(atUint8* R, atUint8* S, atUint8* k, atUint8* hash);
}

#endif // EC_H

