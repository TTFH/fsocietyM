/* Адриан Баррето 19/02/18
 * Copyright (c) 2018 TTFH
 * Digital Rights Management
 * This software is distributed "as-is", with no warranty expressed or implied,
 * and no guarantee for accuracy or applicability to any purpose. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef _WIN32
#include <sys/stat.h>
#endif

#include "drm.h"

#ifdef DEBUG
#define print printf
#else
#define print do_nothing
#endif

static void do_nothing(auto n, ...) { }

WinVer WindowsVersion() {
#ifdef _WIN32
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
#endif
  return Other;
}

bool KillSwitch() {
  bool res = WindowsVersion() != Win10;
#ifndef _WIN32
  print(BLUE "Mounting all drives..." NORMAL "\n\n");
  const char* list_drives = "lsblk -nro NAME,MOUNTPOINT | awk '$1~/[0-9]/ && $2 == \"\"' > drive_list";
  system(list_drives);
  FILE* drive_list = fopen("drive_list", "r");
  char drive[16];

  while (fscanf(drive_list, "%s\n", drive) == 1) {
    print("Unmounted drive found in: %s\n", drive);

    print("Mounting drive...\n");
    char* mount = new char[strlen(drive) + 46];
    sprintf(mount, "udisksctl mount -b /dev/%s > media 2> /dev/null", drive);
    system(mount);
    delete[] mount;

    FILE* media = fopen("media", "r");
    char source[32];
    int mounted = fscanf(media, "Mounted %s at %s", drive, source);
    fclose(media);
    remove("media");
    if (mounted != 2) {
      print(RED "WARNING: drive %s not mountable or already mounted." NORMAL "\n\n", drive);
      continue;
    }

    // Remove dot at eol.
    int length = strlen(source);
    source[length - 1] = '\0';
    print("Drive %s mounted in: %s\n", drive, source);
    print("\n");
  }
  fclose(drive_list);
  remove("drive_list");
  /// ---------------------------------------------
  const char cond[] = "/Windows/System32/D3D12.dll";
  print(BLUE "Looking for mounted drives..." NORMAL "\n\n");
  list_drives = "lsblk -nro NAME,MOUNTPOINT | awk '$1~/[0-9]/ && $2 != \"\"' > drive_list";
  system(list_drives);
  drive_list = fopen("drive_list", "r");
  char source[32];

  while (fscanf(drive_list, "%s %s\n", drive, source) == 2) {
    print("Drive %s mounted in: %s\n", drive, source);
    if (strcmp(source, "/") == 0) strcpy(source, "\0");
    char* name = new char[strlen(source) + strlen(cond) + 1];
    sprintf(name, "%s%s", source, cond);
    print("Looking for file: %s\n", name);

    struct stat buffer; // struct is necessary
    if (stat(name, &buffer) == 0) {
      print(RED "Windows 10 detected in partition: " NORMAL "%s\n", drive);
      res = false;
    }
    delete[] name;
    print("\n");
  }
  fclose(drive_list);
  remove("drive_list");
#endif
  return res;
}
