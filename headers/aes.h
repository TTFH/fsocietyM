#ifndef AES_H
#define AES_H

#define AES_BLOCKLEN 16 // Block length in bytes, AES is 128bits block
#define AES_keyExpSize 240

struct AES_ctx {
  uint8_t RoundKey[AES_keyExpSize];
};

AES_ctx AES_init_ctx(const uint8_t* key);

void AES_ECB_encrypt(AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(AES_ctx* ctx, uint8_t* buf);

#endif
