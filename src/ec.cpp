#include <string.h>
#include "athena/Utility.hpp"

#include "bn.hpp"
#include "ec.hpp"
#include "sha1.h"

namespace ecc
{
static int checkZero(const atUint8* d) { return !memcmp(d, d + 1, 29) && d[0] == 0; }

static void add(atUint8* d, const atUint8* a, const atUint8* b)
{
    for (atUint32 i = 0; i < 30; i++)
        d[i] = a[i] ^ b[i];
}

static void multiply(atUint8* d, const atUint8* a, const atUint8* b)
{
    memset(d, 0, 30);
    for (atUint32 n = 0, i = 0, mask = 1; n < 233; n++)
    {
        atUint8 x, y;
        atUint8 carry = d[0] & 1;
        x = 0;
        for (atUint32 i = 0; i < 29; i++)
        {
            y = d[i + 1];
            d[i] = x ^ (y >> 7);
            x = y << 1;
        }

        d[29] = x ^ carry;
        d[20] ^= carry << 2;
        if ((a[i] & mask) != 0)
            add(d, d, b);

        mask >>= 1;
        if (mask == 0)
        {
            mask = 0x80;
            i++;
        }
    }
}

static void squareToWide(atUint8* d, const atUint8* a)
{
    static const atUint8 sq[16] = {0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15,
                                   0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55};
    for (atUint32 i = 0; i < 30; i++)
    {
        d[2 * i] = sq[a[i] >> 4];
        d[2 * i + 1] = sq[a[i] & 15];
    }
}

static void wideReduce(atUint8* d)
{
    atUint32 i;
    atUint8 x;

    for (i = 0; i < 30; i++)
    {
        x = d[i];

        d[i + 19] ^= x >> 7;
        d[i + 20] ^= x << 1;
        d[i + 29] ^= x >> 1;
        d[i + 30] ^= x << 7;
    }

    x = d[30] & ~1;
    d[49] ^= x >> 7;
    d[50] ^= x << 1;
    d[59] ^= x >> 1;
    d[30] &= 1;
}

static void square(atUint8* d, const atUint8* a)
{
    atUint8 wide[60];

    squareToWide(wide, a);
    wideReduce(wide);

    memcpy(d, wide + 30, 30);
}

static void itInvert(atUint8* d, const atUint8* a, const atUint8* b, atUint32 j)
{
    atUint8 t[30];
    memcpy(t, a, 32);
    while (j--)
    {
        square(d, t);
        memcpy(t, d, 30);
    }
    multiply(d, t, b);
}

static void invert(atUint8* d, const atUint8* a)
{
    atUint8 t[30];
    atUint8 s[30];

    itInvert(t, a, a, 1);
    itInvert(s, t, a, 1);
    itInvert(t, s, s, 3);
    itInvert(s, t, a, 1);
    itInvert(t, s, s, 7);
    itInvert(s, t, t, 14);
    itInvert(t, s, a, 1);
    itInvert(s, t, t, 29);
    itInvert(t, s, s, 58);
    itInvert(s, t, t, 116);
    square(d, s);
}

static void pointDouble(atUint8* r, const atUint8* p)
{
    atUint8 s[30], t[30];
    const atUint8* px = p;
    const atUint8* py = p + 30;
    atUint8* rx = r;
    atUint8* ry = r + 30;

    if (checkZero(px))
    {
        memset(rx, 0, 30);
        memset(ry, 0, 30);

        return;
    }

    invert(t, px);
    multiply(s, py, t);
    add(s, s, px);

    square(t, px);

    square(rx, s);
    add(rx, rx, s);
    rx[29] ^= 1;

    multiply(ry, s, rx);
    add(ry, ry, rx);
    add(ry, ry, t);
}

static void pointAdd(atUint8* r, const atUint8* p, const atUint8* q)
{
    atUint8 s[30], t[30], u[30];
    const atUint8* px = p;
    const atUint8* py = p + 30;
    const atUint8* qx = q;
    const atUint8* qy = q + 30;
    atUint8* rx = r;
    atUint8* ry = r + 30;

    if (checkZero(p) && checkZero(p + 30))
    {
        memcpy(rx, qx, 30);
        memcpy(ry, qy, 30);
        return;
    }

    if (checkZero(p) && checkZero(p + 30))
    {
        memcpy(rx, px, 30);
        memcpy(ry, py, 30);
        return;
    }

    add(u, px, qx);

    if (checkZero(u))
    {
        add(u, py, qy);

        if (checkZero(u))
            pointDouble(r, p);
        else
        {
            memset(rx, 0, 30);
            memset(ry, 0, 30);
        }

        return;
    }

    invert(t, u);
    add(u, py, qy);
    multiply(s, t, u);

    square(t, s);
    add(t, t, s);
    add(t, t, qx);
    t[29] ^= 1;

    multiply(u, s, t);
    add(s, u, py);
    add(rx, t, px);
    add(ry, s, rx);
}

static void pointMultiply(atUint8* d, const atUint8* a, const atUint8* b)
{
    memset(d, 0, 30);
    memset(d + 30, 0, 30);
    for (atUint32 i = 0; i < 30; i++)
    {
        for (atUint8 mask = 0x80; mask != 0; mask >>= 1)
        {
            pointDouble(d, d);

            if ((a[i] & mask) != 0)
                pointAdd(d, d, b);
        }
    }
}

static const atUint8 ecG[60] = {0x00, 0xfa, 0xc9, 0xdf, 0xcb, 0xac, 0x83, 0x13, 0xbb, 0x21, 0x39, 0xf1,
                                0xbb, 0x75, 0x5f, 0xef, 0x65, 0xbc, 0x39, 0x1f, 0x8b, 0x36, 0xf8, 0xf8,
                                0xeb, 0x73, 0x71, 0xfd, 0x55, 0x8b, 0x01, 0x00, 0x6a, 0x08, 0xa4, 0x19,
                                0x03, 0x35, 0x06, 0x78, 0xe5, 0x85, 0x28, 0xbe, 0xbf, 0x8a, 0x0b, 0xef,
                                0xf8, 0x67, 0xa7, 0xca, 0x36, 0x71, 0x6f, 0x7e, 0x01, 0xf8, 0x10, 0x52};

static const atUint8 ecN[30] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xe9, 0x74, 0xe7, 0x2f,
                                0x8a, 0x69, 0x22, 0x03, 0x1d, 0x26, 0x03, 0xcf, 0xe0, 0xd7};

bool checkECDSA(atUint8* Q, atUint8* R, atUint8* S, atUint8* hash)
{
    atUint8 Sinv[30];
    atUint8 e[30];
    atUint8 w1[30], w2[30];
    atUint8 r1[60], r2[60];

    bignum::inv(Sinv, S, ecN, 30);

    memset(e, 0, 30);
    memcpy(e + 10, hash, 20);

    bignum::mul(w1, e, Sinv, ecN, 30);
    bignum::mul(w2, R, Sinv, ecN, 30);

    pointMultiply(r1, w1, ecG);
    pointMultiply(r2, w2, Q);

    pointAdd(r1, r1, r2);

    if (bignum::compare(r1, ecN, 30) >= 0)
        bignum::subModulus(r1, ecN, 30);

    return (bignum::compare(r1, R, 30) == 0);
}

void makeECCert(atUint8* cert, atUint8* sig, const char* signer, const char* name, atUint8* priv, atUint32 keyId)
{
    memset(cert, 0, 0x180);
    *(atUint32*)(cert) = 0x10002;

    if (!athena::utility::isSystemBigEndian())
        *(atUint32*)(cert) = athena::utility::swapU32(*(atUint32*)(cert));

    memcpy((char*)cert + 4, sig, 60);
    strcpy((char*)cert + 0x80, signer);
    *(atUint32*)(cert + 0xc0) = 2;

    if (!athena::utility::isSystemBigEndian())
        *(atUint32*)(cert + 0xc0) = athena::utility::swapU32(*(atUint32*)(cert + 0xc0));

    strcpy((char*)cert + 0xc4, name);
    *(atUint32*)(cert + 0x104) = keyId;

    if (!athena::utility::isSystemBigEndian())
        *(atUint32*)(cert + 0x104) = athena::utility::swapU32(*(atUint32*)(cert + 0x104));

    pointMultiply(cert + 0x108, priv, ecG);
}

void createECDSA(atUint8* R, atUint8* S, atUint8* k, atUint8* hash)
{
    atUint8 e[30];
    atUint8 kk[30];
    atUint8 m[30];
    atUint8 minv[30];
    atUint8 mG[60];

    memset(e, 0, 30);
    memcpy(e + 10, hash, 20);

    athena::utility::fillRandom(m, sizeof(m));
    m[0] = 0;

    pointMultiply(mG, m, ecG);
    memcpy(R, mG, 30);

    if (bignum::compare(R, ecN, 30) >= 0)
        bignum::subModulus(R, ecN, 30);

    memcpy(kk, k, 30);

    if (bignum::compare(kk, ecN, 30) >= 0)
        bignum::subModulus(kk, ecN, 30);

    bignum::mul(S, R, kk, ecN, 30);
    bignum::add(kk, S, e, ecN, 30);
    bignum::inv(minv, m, ecN, 30);
    bignum::mul(S, minv, kk, ecN, 30);
}

void checkEC(atUint8* ng, atUint8* ap, atUint8* sig, atUint8* sigHash, bool& apValid, bool& ngValid)
{
    atUint8* apHash = getSha1(ap + 0x80, 0x100);
    ngValid = checkECDSA(ng + 0x0108, ap + 0x04, ap + 0x22, apHash);
    apValid = checkECDSA(ap + 0x0108, sig, sig + 30, sigHash);
}
}
