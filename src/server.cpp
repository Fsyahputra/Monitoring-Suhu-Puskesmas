#include <Server.h>
#include <Constant.h>
#include <WiFiFile.h>
#include <UserFile.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include <Constant.h>
#include <Sensor.h>
#include <ArduinoJson.h>
#include <CustomJWT.h>
#include <JWT.h>

namespace Server
{
  void serverProcess(FirebaseData &fbdo, AsyncWebServer &server, DHT &dhtSensor, CustomJWT &jwt)
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, Constant::FRONT_END_PATH, "text/html"); });

    server.on("/", HTTP_POST, [jwt](AsyncWebServerRequest *request) {

    });

    server.on("/user", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                String username = request->arg("username");
                String password = request->arg("password");
                if (UserFile::saveUserCredentials(username, password))
                {
                  request->send(200, "text/plain", "User credentials saved");
                }
                else
                {
                  request->send(500, "text/plain", "Failed to save user credentials");
                } });

    server.on("/user", HTTP_GET, [](AsyncWebServerRequest *request)
              {
      String username = UserFile::readUsername();
      if (username.length() > 0)
      {
        DynamicJsonDocument doc(128);
        doc["username"] = username;
        String jsonString;
        serializeJson(doc, jsonString);
        request->send(200, "application/json", jsonString);
      }
      else
      {
        request->send(404, "text/plain", "User credentials not found");
      } });

    server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String ssid = request->arg("ssid");
                  String password = request->arg("password");
                  if (WiFiFile::saveWiFiCredentials(ssid, password))
                  {
                    request->send(200, "text/plain", "WiFi credentials saved");
                  }
                  else
                  {
                    request->send(500, "text/plain", "Failed to save WiFi credentials");
                  } });

    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
              {
      String ssid = WiFiFile::readWifiSsid();
      if (ssid.length() > 0)
      {
        DynamicJsonDocument doc(128);
        doc["ssid"] = ssid;
        String jsonString;
        serializeJson(doc, jsonString);
        request->send(200, "application/json", jsonString);
      }
      else
      {
        request->send(404, "text/plain", "WiFi credentials not found");
      } });

    server.on("/sensor", HTTP_GET, [&dhtSensor](AsyncWebServerRequest *request)
              {
      Constant::SensorPayload sensorData = Sensor::readSensorData(dhtSensor);

      DynamicJsonDocument doc(256);
      doc["temperature"] = sensorData.temperature;
      doc["humidity"] = sensorData.humidity;
      doc["timestamp"] = sensorData.timestamp;

      String jsonString;
      serializeJson(doc, jsonString);
      request->send(200, "application/json", jsonString); });

    server.serveStatic("/", LittleFS, Constant::FRONT_END_STATIC_PATH)
        .setDefaultFile("index.html")
        .setCacheControl("max-age=3600");
  }
}
