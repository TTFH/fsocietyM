# fsocietyM
Cryptowall Ransomware from Mr Robot serie S2E1 and S2E2

This project is purely academic, use at your own risk. I do not encourage in any way the use of this software illegally or to attack targets without their previous authorization.

Files are encrypted with AES256 ECB ANSI X9.23, key is stored in file "key.bin"

# On movie

![movie](https://raw.githubusercontent.com/TTFH/fsocietyM/master/example/Cryptowall1.png)

# This One

![this](https://raw.githubusercontent.com/TTFH/fsocietyM/master/example/Cryptowall2.png)

Download XAMPP Apache from:
https://www.apachefriends.org/es/index.html

Download Firefox from:
https://www.mozilla.org/es-ES/firefox/all/
(you can edit main.cpp to use Chrome or other browser)

Compile:
g++ -Wall -Werror -O3 main.cpp aes.cpp -o fsocietyM.exe
