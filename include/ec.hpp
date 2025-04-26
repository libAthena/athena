#pragma once
#include "athena/Types.hpp"

namespace ecc {
void checkEC(uint8_t* ng, uint8_t* ap, uint8_t* sig, uint8_t* sigHash, bool& apValid, bool& ngValid);
void makeECCert(uint8_t* cert, uint8_t* sig, const char* signer, const char* name, uint8_t* priv, uint32_t keyId);
void createECDSA(uint8_t* R, uint8_t* S, uint8_t* k, uint8_t* hash);
} // namespace ecc
