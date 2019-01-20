#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <fileapi.h>
#include <windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

#include "aes.h"

unsigned int cant_files = 0;

bool isEncryptedFile(char* file) {
  char* ext = strrchr(file, '.');
  if (ext == NULL) return false;
  ext++;
  return strcmp(ext, "encrypted") == 0;
}

void DecryptFile(char* fname, const uint8_t* key) {
  if (!isEncryptedFile(fname)) return;
  printf("Decrypting File: %s\n", fname);
  AES_stream_decrypt(fname, key);
  cant_files++;
  printf("File Decrypted!\n");
}

void SearchFiles(const char* sDir, const uint8_t* key) {
#ifdef _WIN32
  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;
  char sPath[512];
  sprintf(sPath, "%s\\*.*", sDir); // Search all files
  hFind = FindFirstFile(sPath, &fdFile);
  do {
    if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
      // Build up file path
      sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);
      // Is it a file or folder?
      if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        SearchFiles(sPath, key); // Recursion
      else
        DecryptFile(sPath, key);
    }
  } while (FindNextFile(hFind, &fdFile)); // Find the next file
  FindClose(hFind); // Clean
#else
  DIR* dir = opendir(sDir);
  dirent* entry = readdir(dir);
  char path[512];
  while (entry != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      sprintf(path, "%s/%s", sDir, entry->d_name);  
      if (entry->d_type == DT_DIR)
        SearchFiles(path, key);
      else
        DecryptFile(path, key);
    }
    entry = readdir(dir);
  }
  closedir(dir);
#endif
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

void PrintHex(const uint8_t* str, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (i > 0 && i % 16 == 0)
      printf("\n");
    printf("%.2X ", str[i]);
  }
  printf("\n");
}

int main() {
  printf("\n");
  uint8_t key[32];
  char digit1, digit2;
  printf("Enter the key (64 hexadecimal digits):\n");
  for (uint8_t i = 0; i < 32; i++) {
    do {
      digit1 = getchar();
    } while (digit1 == ' ' || digit1 == '\n');
    do {
      digit2 = getchar();
    } while (digit2 == ' ' || digit2 == '\n');
    key[i] = chartohex(digit1) * 16 + chartohex(digit2);
  }
  printf("\nKey:\n");
  PrintHex(key, 32);
  printf("\n");

  char c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);

  printf("Are you sure you wanna decrypt?\nA wrong key will make your files unrecoverable forever");
  printf(" (Y/n): ");
  char opt;
  scanf("%c", &opt);
  if (opt == 'Y') {
#ifdef _WIN32
    SearchFiles("..\\testfolder", key);
#else
    SearchFiles("../testfolder", key);
#endif
  }
  printf("%u files has been decrypted\n", cant_files);

  return 0;
}
