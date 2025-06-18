#include "FileUtils.h"
#include <LittleFS.h>

namespace FileUtils
{
  String readFile(const char *path)
  {
    String content;
    File file = LittleFS.open(path, FILE_READ);
    if (file)
    {
      while (file.available())
      {
        content += (char)file.read();
      }
    }

    file.close();
    return content;
  }

  bool writeFile(const char *path, const String &data)
  {
    return false;
  }

  bool writeFile(const char *path, const char *data)
  {
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file)
    {
      return false; // Failed to open file for writing
    }

    size_t written = file.print(data);
    file.close();
    return written > 0; // Return true if data was written successfully
  }

  bool deleteFile(const char *path)
  {
    return LittleFS.remove(path); // Delete the file at the specified path
  }

  bool fileExists(const char *path)
  {
    return LittleFS.exists(path); // Check if the file exists at the specified path
  }

}