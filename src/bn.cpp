#include <cstring>
#include <stdio.h>

#include "bn.hpp"

namespace bignum {

void subModulus(uint8_t* a, const uint8_t* N, uint32_t n) {
  uint8_t c = 0;

  for (uint32_t i = n - 1; i < n; i--) {
    uint32_t dig = N[i] + c;
    c = (a[i] < dig);
    a[i] -= dig;
  }
}

void add(uint8_t* d, uint8_t* a, const uint8_t* b, const uint8_t* N, uint32_t n) {
  uint8_t c = 0;

  for (uint32_t i = n - 1; i < n; i--) {
    uint32_t dig = a[i] + b[i] + c;
    c = (dig >= 0x100);
    d[i] = dig;
  }

  if (c)
    subModulus(d, N, n);

  if (compare(d, N, n) >= 0)
    subModulus(d, N, n);
}

void mul(uint8_t* d, uint8_t* a, const uint8_t* b, const uint8_t* N, uint32_t n) {
  memset(d, 0, n);

  for (uint32_t i = 0; i < n; i++) {
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
      add(d, d, d, N, n);

      if ((a[i] & mask) != 0)
        add(d, d, b, N, n);
    }
  }
}

void exp(uint8_t* d, const uint8_t* a, const uint8_t* N, uint32_t n, uint8_t* e, uint32_t en) {
  uint8_t t[512];
  memset(d, 0, n);
  d[n - 1] = 1;

  for (uint32_t i = 0; i < en; i++) {
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
      mul(t, d, d, N, n);

      if ((e[i] & mask) != 0)
        mul(d, t, a, N, n);
      else
        memcpy(d, t, n);
    }
  }
}

void inv(uint8_t* d, uint8_t* a, const uint8_t* N, uint32_t n) {
  uint8_t t[512], s[512];

  memcpy(t, N, n);
  memset(s, 0, n);
  s[n - 1] = 2;
  subModulus(t, s, n);
  exp(d, a, N, n, t, n);
}

int compare(const uint8_t* a, const uint8_t* b, uint32_t n) {
  for (uint32_t i = 0; i < n; i++) {
    if (a[i] < b[i])
      return -1;

    if (a[i] > b[i])
      return 1;
  }
  return 0;
}
} // namespace bignum
