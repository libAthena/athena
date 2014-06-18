#ifndef EC_H
#define EC_H
#include "Athena/Types.hpp"

bool  check_ec      ( atUint8 *ng, atUint8 *ap, atUint8 *sig, atUint8 *sig_hash );
void  make_ec_cert  ( atUint8 *cert, atUint8 *sig, char *signer, char *name, atUint8 *priv, atUint32 key_id );
void  generate_ecdsa( atUint8 *R, atUint8 *S, atUint8 *k, atUint8 *hash );

#endif // EC_H

