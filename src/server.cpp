#include <Server.h>
#include <Constant.h>
#include <WiFiFile.h>
#include <UserFile.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include <Constant.h>

namespace Server
{
  void serverProcess(bool isInfraReady, FirebaseData &fbdo, AsyncWebServer &server)
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, Constant::FRONT_END_PATH, "text/html"); });

    server.serveStatic("/", LittleFS, Constant::FRONT_END_STATIC_PATH)
        .setDefaultFile("index.html")
        .setCacheControl("max-age=3600");
  }
}
