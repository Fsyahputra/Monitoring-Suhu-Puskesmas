#ifndef USER_FILE_H
#define USER_FILE_H
#include <Arduino.h>

namespace UserFile
{
  String readUsername();
  bool writeUsername(const String &username);
  String readPassword();
  bool writePassword(const String &password);
  String readDefaultUsername();
  bool writeDefaultUsername(const String &defaultUsername);
  String readDefaultPassword();
  bool writeDefaultPassword(const String &defaultPassword);
  bool deleteUsername();
  bool deletePassword();
  bool deleteDefaultUsername();
  bool deleteDefaultPassword();

  bool isUsernameExists();
  bool isPasswordExists();
  bool isDefaultUsernameExists();
  bool isDefaultPasswordExists();

}

#endif