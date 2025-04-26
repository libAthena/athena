#pragma once

#include "athena/Types.hpp"
namespace bignum {
int compare(const uint8_t* a, const uint8_t* b, uint32_t n);
void subModulus(uint8_t* a, const uint8_t* N, uint32_t n);
void add(uint8_t* d, uint8_t* a, const uint8_t* b, const uint8_t* N, uint32_t n);
void mul(uint8_t* d, uint8_t* a, const uint8_t* b, const uint8_t* N, uint32_t n);
void exp(uint8_t* d, const uint8_t* a, const uint8_t* N, uint32_t n, uint8_t* e, uint32_t en);
void inv(uint8_t* d, uint8_t* a, const uint8_t* N, uint32_t n);
} // namespace bignum
