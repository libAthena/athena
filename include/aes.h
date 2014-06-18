#ifndef __AES_H_
#define __AES_H_

#include "Athena/Types.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void aes_encrypt(atUint8 *iv, const atUint8 *inbuf, atUint8 *outbuf, atUint64 len);
void aes_decrypt(atUint8 *iv, const atUint8 *inbuf, atUint8 *outbuf, atUint64 len);
void aes_set_key(const atUint8 *key );

#ifdef __cplusplus
}
#endif

#endif //__AES_H_
