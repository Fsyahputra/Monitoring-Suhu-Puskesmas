#ifndef JWT_H
#define JWT_H

#include <Arduino.h>
#include <CustomJWT.h>

namespace JWT
{
  String createToken(const String &username, CustomJWT &jwt);
  bool validateToken(const String &token, CustomJWT &jwt);

}

#endif