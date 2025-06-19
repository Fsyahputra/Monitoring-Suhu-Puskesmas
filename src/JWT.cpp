#include <JWT.h>
#include <ArduinoJson.h>
#include <CustomJWT.h>

namespace JWT
{
  String createToken(const String &username, CustomJWT &jwt)
  {
    DynamicJsonDocument doc(512);
    doc["username"] = username;
    doc["exp"] = millis() / 1000 + 3600;

    String payload;
    serializeJson(doc, payload);

    char payloadChar[payload.length() + 1];
    payload.toCharArray(payloadChar, sizeof(payloadChar));

    jwt.encodeJWT(payloadChar);

    String token = jwt.out;

    return token;
  }

  bool validateToken(const String &token, CustomJWT &jwt)
  {
    char tokenChar[token.length() + 1];
    token.toCharArray(tokenChar, sizeof(tokenChar));

    int result = jwt.decodeJWT(tokenChar);
    if (result != 0)
    {
      return false;
    }

    return true;
  }
}
