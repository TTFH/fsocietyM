#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "../headers/base64.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* EncodeBase64(const uint8_t* src, unsigned int len, unsigned int &out_len) {
  char* out = new char[len * 4 / 3 + 4];
  unsigned int i = 0, j = 0;
  while (len - i >= 3) {
    out[j] = base64_table[src[i] >> 2];
    out[j+1] = base64_table[((src[i] & 0x03) << 4) | (src[i+1] >> 4)];
    out[j+2] = base64_table[((src[i+1] & 0x0f) << 2) | (src[i+2] >> 6)];
    out[j+3] = base64_table[src[i+2] & 0x3f];
    i += 3;
    j += 4;
  }
  if (i != len) {
    out[j] = base64_table[src[i] >> 2];
    if (len - i == 2) {
      out[j+1] = base64_table[((src[i] & 0x03) << 4) | (src[i+1] >> 4)];
      out[j+2] = base64_table[(src[i+1] & 0x0f) << 2];
    } else {
      out[j+1] = base64_table[(src[i] & 0x03) << 4];
      out[j+2] = '=';
    }
    out[j+3] = '=';
    j += 4;
  }
  out_len = j;
  return out;
}

uint8_t* DecodeBase64(const char* src, unsigned int len, unsigned int &out_len) {
  uint8_t dtable[256];
  memset(dtable, 0x80, 256);
  for (unsigned int i = 0; i < 64; i++)
    dtable[(int)base64_table[i]] = (uint8_t)i;
  dtable['='] = 0;
  unsigned int pad = 0;
  uint8_t* out = new uint8_t[len / 4 * 3 + 1];
  unsigned int i = 0, j = 0;
  while (i < len) {
    out[j] = (dtable[(int)src[i]] << 2) | (dtable[(int)src[i+1]] >> 4);
	out[j+1] = (dtable[(int)src[i+1]] << 4) | (dtable[(int)src[i+2]] >> 2);
	out[j+2] = (dtable[(int)src[i+2]] << 6) | dtable[(int)src[i+3]];
	i += 4;
    j += 3;
  }
  if (src[i-1] == '=')
    pad++;
  if (src[len-2] == '=')
    pad++;
  out_len = j - pad;
  out[out_len] = '\0';
  return out;
}

void PrintBase64(const char* src, unsigned int len) {
  for (unsigned int i = 0; i < len; i++) {
  	printf("%c", src[i]);
  	if ((i+1) % 64 == 0) printf("\n");
  }
  printf("\n");
}

void PrintHex(const uint8_t* str, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (i > 0 && i % 16 == 0)
      printf("\n");
    printf("%.2X ", str[i]);
  }
  printf("\n");
}

uint8_t chartohex(char c) {
  assert(isxdigit(c));
  uint8_t res;
  if (c >= '0' && c <= '9')
    res = c - '0';
  else if (c >= 'a' && c <= 'f')
    res = c - 'a' + 10;
  else //if (c >= 'A' && c <= 'F')
    res = c - 'A' + 10;
  return res;
}
