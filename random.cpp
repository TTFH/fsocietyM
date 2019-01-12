#include <time.h>
#include <stdint.h>
#include <stdlib.h>
//#include <stdio.h>

// Convert 8 bits to 1 byte
uint8_t toByte(bool b[8]) {
  uint8_t c = 0;
  for (int i = 0; i < 8; i++)
    if (b[i]) c |= 1 << i;
  return c;
}

// Return a random array of bool of size N with exactly N*p true
// Return the position x after dist[] is not so random
void bernoulli(bool* dist, int N, float p, int& x) {
  int i = 0;
  int ct = 0;
  int cf = 0;
  while (i < N && ct < N*p && cf < N-N*p) {
    dist[i] = rand() % 2 == 1;
    if (dist[i])
      ct++;
    else
      cf++;
    i++;
  }
  if (p == 0 || p == 1)
    x = N - 1;
  else
    x = i - 1;
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
}

// Generate 32 bytes random key
uint8_t* advandedRNG() {
  srand(time(NULL) ^ clock());
  uint8_t* key = new uint8_t[32];
  bool* prob = new bool[256];
  int x = 0, x_rel = 0, N = 256;
  float q;
  do {
    do {
      q = rand() % (N + 1);
      //printf("q = %g, %d\n", q, N);
    } while ((q == 0 || q == N) && N != 1);

    float p = q / N; // p in [0..1]
    bernoulli(&prob[x_rel], N, p, x);
    //printf("Generated from %d to %d (%d bits)\n", x_rel, x_rel + x, x + 1);
    N -= x + 1;
    x_rel += x;

    /*printf("p = %g\n", p);
    for (int i = 0; i < 256; i++) {
      if (prob[i]) printf("1"); else printf("0");
      if (i == x_rel) printf("|");
    }
    printf("\n");
    getchar();*/
    x_rel++;
  } while (x_rel != 256);

  for (int i = 0; i < 32; i++) {
    bool* pos = &prob[8 * i];
    key[i] = toByte(pos);
  }
  delete[] prob;
  return key;
}
