#include <FileUtils.h>
#include <UserFile.h>
#include <Arduino.h>
#include <Constant.h>

namespace UserFile
{
  String readUsername()
  {
    return FileUtils::readFile(Constant::USERNAME_PATH);
  }

  bool writeUsername(const String &username)
  {
    return FileUtils::writeFile(Constant::USERNAME_PATH, username);
  }

  String readPassword()
  {
    return FileUtils::readFile(Constant::PASSWORD_PATH);
  }

  bool writePassword(const String &password)
  {
    return FileUtils::writeFile(Constant::PASSWORD_PATH, password);
  }

  String readDefaultUsername()
  {
    return FileUtils::readFile(Constant::DEFAULT_USERNAME_PATH);
  }

  bool writeDefaultUsername(const String &defaultUsername)
  {
    return FileUtils::writeFile(Constant::DEFAULT_USERNAME_PATH, defaultUsername);
  }

  String readDefaultPassword()
  {
    return FileUtils::readFile(Constant::DEFAULT_PASSWORD_PATH);
  }

  bool writeDefaultPassword(const String &defaultPassword)
  {
    return FileUtils::writeFile(Constant::DEFAULT_PASSWORD_PATH, defaultPassword);
  }

  bool deleteUsername()
  {
    return FileUtils::deleteFile(Constant::USERNAME_PATH);
  }

  bool deletePassword()
  {
    return FileUtils::deleteFile(Constant::PASSWORD_PATH);
  }

  bool deleteDefaultUsername()
  {
    return FileUtils::deleteFile(Constant::DEFAULT_USERNAME_PATH);
  }

  bool deleteDefaultPassword()
  {
    return FileUtils::deleteFile(Constant::DEFAULT_PASSWORD_PATH);
  }

  bool isUsernameExists()
  {
    return FileUtils::fileExists(Constant::USERNAME_PATH);
  }

  bool isPasswordExists()
  {
    return FileUtils::fileExists(Constant::PASSWORD_PATH);
  }

  bool isDefaultUsernameExists()
  {
    return FileUtils::fileExists(Constant::DEFAULT_USERNAME_PATH);
  }

  bool isDefaultPasswordExists()
  {
    return FileUtils::fileExists(Constant::DEFAULT_PASSWORD_PATH);
  }

}