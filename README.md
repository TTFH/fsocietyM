# fsocietyM  
Cryptowall Ransomware from Mr Robot serie S2E1 and S2E2  

# [DEPRECATED] DO NOT ATTEMPT TO USE. This repo is very old and contains random nonsense.

This project is purely academic, use at your own risk. I do not encourage in any way the use of this software illegally.  

Files are encrypted with AES256 ECB ANSI X9.23  

![diff](https://raw.githubusercontent.com/TTFH/fsocietyM/master/example/Cryptowall3.png)  

Download Msys2 from:  
https://www.msys2.org/  

Download Mozilla Firefox from:  
https://www.mozilla.org/es-ES/firefox/all/  
(you can edit main.cpp to use Chrome or other browser)  

Download HxD Hex Editor (optional) from:  
https://mh-nexus.de/en/hxd/  
(to view the file containing the decryption key)  

Install Compiler:  
pacman -S mingw-w64-x86_64-toolchain  

Compile:  
cd cryptowall  
g++ -Wall -Werror embed.cpp -o pack  
echo #include "../headers/embedded_file.h" > ..\cpp\embedded_file.cpp  
pack flag.png index.htm jester.png styles.css getip.bat fullscreen.vbs >> ..\cpp\embedded_file.cpp  
cd ..  
make  
g++ -Wall -Werror server.cpp cpp\base64.cpp cpp\random.cpp -o server  

