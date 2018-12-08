#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>

namespace athena {

class IAES {
public:
  virtual ~IAES() {}
  virtual void encrypt(const uint8_t* iv, const uint8_t* inbuf, uint8_t* outbuf, uint64_t len) = 0;
  virtual void decrypt(const uint8_t* iv, const uint8_t* inbuf, uint8_t* outbuf, uint64_t len) = 0;
  virtual void setKey(const uint8_t* key) = 0;
};

std::unique_ptr<IAES> NewAES();

} // namespace athena
