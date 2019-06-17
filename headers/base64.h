#ifndef BASE64_H
#define BASE64_H

uint8_t chartohex(char c);
void PrintHex(const uint8_t* str, uint8_t len);
void PrintBase64(const char* src, unsigned int len);
char* EncodeBase64(const uint8_t* src, unsigned int len, unsigned int &out_len);
uint8_t* DecodeBase64(const char* src, unsigned int len, unsigned int &out_len);

#endif
