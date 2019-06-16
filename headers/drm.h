#ifndef DRM_H
#define DRM_H

#define DEBUG 1

#define RED    "\x1b[91m"
#define BLUE   "\x1b[94m"
#define NORMAL "\x1b[0m"

enum WinVer { Win10, Win8_1, Win8, Win7, WinVista, WinXP64, WinXP32, Win2000, Other, Error };

WinVer WindowsVersion();
bool KillSwitch();

#endif

