#ifndef SERVER_H
#define SERVER_H
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>

namespace Server
{
  void serverProcess(bool isInfraReady, FirebaseData &fbdo, AsyncWebServer &server);
}

#endif // SERVER_H
