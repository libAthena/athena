#ifndef _SHA1_H_
#define _SHA1_H_

#include "athena/Types.hpp"

#ifndef __DOXYGEN_IGNORE__
#ifdef __cplusplus
extern "C" {
#endif

/*
 *  This structure will hold context information for the hashing
 *  operation
 */
typedef struct SHA1Context
{
    unsigned Message_Digest[5]; /* Message Digest (output)          */

    unsigned Length_Low;        /* Message length in bits           */
    unsigned Length_High;       /* Message length in bits           */

    unsigned char Message_Block[64]; /* 512-bit message blocks      */
    int Message_Block_Index;    /* Index into message block array   */

    int Computed;               /* Is the digest computed?          */
    int Corrupted;              /* Is the message digest corruped?  */
} SHA1Context;

/*
 *  Function Prototypes
 */
void SHA1Reset(SHA1Context*);
int SHA1Result(SHA1Context*);
void SHA1Input(SHA1Context*,
               const unsigned char*,
               unsigned);



uint8_t* getSha1(uint8_t* stuff, uint32_t stuff_size);

#ifdef __cplusplus
}
#endif

#endif // __DOXYGEN_IGNORE__
#endif
