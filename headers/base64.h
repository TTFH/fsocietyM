#ifndef BASE64_H
#define BASE64_H

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void PrintBase64(const char* src, unsigned int len);
char* EncodeBase64(const uint8_t* src, unsigned int len, unsigned int &out_len);
uint8_t* DecodeBase64(const char* src, unsigned int len, unsigned int &out_len);
#endif
