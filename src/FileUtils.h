#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <Arduino.h>

namespace FileUtils
{
  String readFile(const char *path);
  bool writeFile(const char *path, const String &data);
  bool deleteFile(const char *path);
  bool fileExists(const char *path);
}

#endif