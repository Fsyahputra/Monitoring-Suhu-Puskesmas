#ifndef SERVER_H
#define SERVER_H
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>

namespace Server
{
  void serverProcess(FirebaseData &fbdo, AsyncWebServer &server, DHT &dhtSensor, CustomJWT &jwt);
}

#endif // SERVER_H
