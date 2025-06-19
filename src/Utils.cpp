#include <Constant.h>
#include <WiFiFile.h>
#include <Utils.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include <Firebase_ESP_Client.h>
#include <Utils.h>

namespace Utils
{
  void resetWiFiConf()
  {
    WiFiFile::deleteWifiPassword();
    WiFiFile::deleteWifiSsid();
    Serial.println(Constant::RESET_CONF_MESSAGE);
    ESP.restart();
  }

  bool initWifi()
  {
    String ssid = WiFiFile::readWifiSsid();
    String password = WiFiFile::readWifiPassword();
    if (ssid.length() == 0 || password.length() == 0)
    {
      Serial.println(Constant::WIFI_CREDENTIALS_NOT_FOUND_MESSAGE);
      return false;
    }

    WiFi.begin(ssid, password);
    Serial.println(Constant::WIFI_CONNECTING_MESSAGE);

    unsigned long previousTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      unsigned long currentTime = millis();
      if (currentTime < Constant::CONNECTION_TIMEOUT)
      {
        if (currentTime - previousTime >= Constant::RECONNECT_INTERVAL)
        {
          Serial.print(".");
          previousTime = currentTime;
        }
      }
      else
      {
        return false;
      }
    }

    Serial.println(Constant::WIFI_CONNECTED_MESSAGE);
    return true;
  }

  bool initDisplay(Adafruit_SSD1306 &display, TwoWire &oledI2c)
  {
    if (!oledI2c.begin(Constant::OLED_SDA_PIN, Constant::OLED_SCL_PIN))
    {
      Serial.println(Constant::OLED_INIT_FAILED_MESSAGE);
      return false;
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      Serial.println(Constant::OLED_INIT_FAILED_MESSAGE);
      return false;
    }

    return true;
  }
  bool isInternetConnected(int timeout)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      return false;
    }

    HTTPClient http;
    http.setTimeout(timeout);
    http.begin(Constant::INTERNET_CHECK_URL);
    int httpCode = http.GET();
    if (!httpCode > 0 && httpCode != HTTP_CODE_NO_CONTENT)
    {
      http.end();
      return false;
    }
    http.end();
    return true;
  }

  bool isInternetConnectedForeground()
  {
    return isInternetConnected(Constant::INTERNET_CHECK_TIMEOUT);
  }

  bool isInternetConnectedBackground()
  {
    return isInternetConnected(Constant::INTERNET_CHECK_BACKGROUND_TIMEOUT);
  }

  bool connectToBlynk()
  {
    Blynk.config(Constant::BLYNK_AUTH_TOKEN, Constant::BLYNK_SERVER, Constant::BLYNK_PORT);
    unsigned long startTime = millis();
    while (!Blynk.connect())
    {
      if (millis() - startTime > Constant::BLYNK_TIMEOUT)
      {
        return false;
      }
    }

    return true;
  }

  bool connectToFirebase(FirebaseAuth &auth, FirebaseConfig &config)
  {
    config.api_key = Constant::FIREBASE_AUTH_TOKEN;
    config.database_url = Constant::FIREBASE_DB_URL;

    auth.user.email = Constant::FIREBASE_AUTH_EMAIL;
    auth.user.password = Constant::FIREBASE_AUTH_PASSWORD;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    if (!Firebase.ready())
    {
      return false;
    }

    return true;
  }

  bool initHotspot()
  {
    String ssid = WiFiFile::readHotspotSsid();
    String password = WiFiFile::readHotspotPassword();
    if (ssid.length() == 0 || password.length() == 0)
    {
      ssid = Constant::DEFAULT_HOTSPOT_SSID;
      password = Constant::DEFAULT_HOTSPOT_PASSWORD;
      WiFiFile::writeHotspotSsid(ssid);
      WiFiFile::writeHotspotPassword(password);
    }

    WiFi.softAP(ssid, password);
    Serial.println(Constant::HOTSPOT_STARTED_MESSAGE);
    return true;
  }
}
