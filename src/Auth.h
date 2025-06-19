#ifndef AUTH_H
#define AUTH_H
#include <Arduino.h>

namespace Auth
{
  bool authenticateUser(const String &username, const String &password);

  bool isAuthenticated();

  void logout();

  String getCurrentUsername();
}

#endif