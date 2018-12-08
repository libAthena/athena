#include <cstring>
#include <stdio.h>

#include "bn.hpp"

namespace bignum {

void subModulus(atUint8* a, const atUint8* N, atUint32 n) {
  atUint8 c = 0;

  for (atUint32 i = n - 1; i < n; i--) {
    atUint32 dig = N[i] + c;
    c = (a[i] < dig);
    a[i] -= dig;
  }
}

void add(atUint8* d, atUint8* a, const atUint8* b, const atUint8* N, atUint32 n) {
  atUint8 c = 0;

  for (atUint32 i = n - 1; i < n; i--) {
    atUint32 dig = a[i] + b[i] + c;
    c = (dig >= 0x100);
    d[i] = dig;
  }

  if (c)
    subModulus(d, N, n);

  if (compare(d, N, n) >= 0)
    subModulus(d, N, n);
}

void mul(atUint8* d, atUint8* a, const atUint8* b, const atUint8* N, atUint32 n) {
  memset(d, 0, n);

  for (atUint32 i = 0; i < n; i++) {
    for (atUint8 mask = 0x80; mask != 0; mask >>= 1) {
      add(d, d, d, N, n);

      if ((a[i] & mask) != 0)
        add(d, d, b, N, n);
    }
  }
}

void exp(atUint8* d, const atUint8* a, const atUint8* N, atUint32 n, atUint8* e, atUint32 en) {
  atUint8 t[512];
  memset(d, 0, n);
  d[n - 1] = 1;

  for (atUint32 i = 0; i < en; i++) {
    for (atUint8 mask = 0x80; mask != 0; mask >>= 1) {
      mul(t, d, d, N, n);

      if ((e[i] & mask) != 0)
        mul(d, t, a, N, n);
      else
        memcpy(d, t, n);
    }
  }
}

void inv(atUint8* d, atUint8* a, const atUint8* N, atUint32 n) {
  atUint8 t[512], s[512];

  memcpy(t, N, n);
  memset(s, 0, n);
  s[n - 1] = 2;
  subModulus(t, s, n);
  exp(d, a, N, n, t, n);
}

int compare(const atUint8* a, const atUint8* b, atUint32 n) {
  for (atUint32 i = 0; i < n; i++) {
    if (a[i] < b[i])
      return -1;

    if (a[i] > b[i])
      return 1;
  }
  return 0;
}
} // namespace bignum
