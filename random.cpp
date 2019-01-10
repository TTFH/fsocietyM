#include <time.h>
#include <stdint.h>
#include <stdlib.h>

// Generate random number between 0 and 256
float rng() {
  return rand() % (256 + 1);
}

// Convert 8 bits to 1 byte
uint8_t toByte(bool b[8]) {
  uint8_t c = 0;
  for (int i = 0; i < 8; i++)
    if (b[i]) c |= 1 << i;
  return c;
}

bool* bernoulli(float p, int N) {
  bool* dist = new bool[N];
  bool b;
  int i = 0;
  int ct = 0;
  int cf = 0;
  while (i < N && ct < N*p && cf < N-N*p) {
    b = rand() % 2 == 1;
    dist[i] = b;
    if (b)
      ct++;
    else
      cf++;
    i++;
  }
  while (i < N && ct < N*p) {
    dist[i] = true;
    ct++;
    i++;
  }
  while (i < N && cf < N-N*p) {
    dist[i] = false;
    cf++;
    i++;
  }
  return dist;
}

// Generate 32 bytes random key
uint8_t* advandedRNG() {
  srand(time(NULL) ^ clock());
  float p = rng() / 256; // p in [0..1]
  bool* prob = bernoulli(p, 256);
  uint8_t* key = new uint8_t[32];
  for (int i = 0; i < 32; i++) {
    bool* pos = &prob[8 * i];
    key[i] = toByte(pos);
  }
  delete[] prob;
  for (int i = 0; i < 32; i++)
	  if (key[i] == 0x00 || key[i] == 0xFF) {
		  p = rng() / 256;
		  bool* sec = bernoulli(p, 8);
		  key[i] = toByte(sec);
		  delete[] sec;
	  } 
  return key;
}
