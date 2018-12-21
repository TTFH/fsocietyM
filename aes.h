#ifndef AES_H
#define AES_H

#define AES_BLOCKLEN 16 // Block length in bytes, AES is 128bits block only

void AES_encrypt(const uint8_t* key, uint8_t* data, uint32_t size);
void AES_decrypt(const uint8_t* key, uint8_t* data, uint32_t size);
#endif
