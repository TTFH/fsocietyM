#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "headers/base64.h"
#include "headers/random.h"

int main() {
  printf("Insert User ID: ");
  char* src = new char[512];
  scanf("%[^\n]s\n", src);
  unsigned int seed_len;
  uint8_t* out = DecodeBase64(src, strlen(src), seed_len);
  unsigned int seed = (out[0] << 24) + (out[1] << 16) + (out[2] << 8) + out[3];
  char* id; unsigned int len;
  uint8_t* key = advandedRNG(id, len, seed);
  printf("The key to decrypt the files is:\n");
  PrintHex(key, 32);
  delete[] src;
  delete[] out;
  delete[] key;
  return 0;
}
