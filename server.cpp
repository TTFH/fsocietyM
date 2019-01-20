#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "base64.h"
#include "random.h"

void PrintHex(const uint8_t* str, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (i > 0 && i % 16 == 0)
      printf("\n");
    printf("%.2X ", str[i]);
  }
  printf("\n");
}

int main() {
  printf("Insert Used ID: ");
  char* src = new char[512];
  scanf("%[^\n]s\n", src);
  unsigned int seed_len;
  uint8_t* out = DecodeBase64(src, strlen(src), seed_len);
  unsigned int seed = (out[0] << 24) + (out[1] << 16) + (out[2] << 8) + out[3];
  char* id; unsigned int len;
  uint8_t* key = advandedRNG(id, len, seed);
  PrintHex(key, 32);
  delete[] src;
  delete[] out;
  delete[] key;
  return 0;
}
