#ifndef AES_H
#define AES_H

#define AES_BLOCKLEN 16 // Block length in bytes, AES is 128bits block

void AES_encrypt(uint8_t* data, uint32_t size, const uint8_t* key);
void AES_decrypt(uint8_t* data, uint32_t size, const uint8_t* key);

void AES_stream_encrypt(char* filename, const uint8_t* key);
void AES_stream_decrypt(char* filename, const uint8_t* key);
#endif
