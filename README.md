# fsocietyM
Cryptowall Ransomware from Mr Robot serie S2E1 and S2E2

This project is purely academic, use at your own risk. I do not encourage in any way the use of this software illegally or to attack targets without their previous authorization.

Files are encrypted with AES256 ECB ANSI X9.23, key is stored in file "key.bin"

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
g++ -Wall -Werror -O3 main.cpp aes.cpp base64.cpp random.cpp DRM.cpp -o fsocietyM -Wno-unused-result -fconcepts  
g++ -Wall -Werror -O3 decrypter.cpp aes.cpp -o decrypter  
g++ -Wall -Werror -O3 server.cpp base64.cpp random.cpp -o server
