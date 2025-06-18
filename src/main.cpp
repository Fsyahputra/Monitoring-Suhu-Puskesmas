#include <Arduino.h>
#include <Constant.h>
#include <UserFile.h>
#include <WiFiFile.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include <BlynkSimpleEsp32.h>

unsigned long currentTime = 0;
unsigned long previousTime = 0;

TwoWire oledI2c = TwoWire(0);
Adafruit_SSD1306 display(Constant::OLED_WIDTH, Constant::OLED_HEIGHT, &oledI2c, -1);
DHT dhtSensor(Constant::DHT_SDA_PIN, Constant::DHT_TYPE);

String WiFiSsid;
String WiFiPassword;

String username;
String password;

String hotspotSsid;
String hotspotPassword;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool isBlynkConnected = false;
bool isFirebaseConnected = false;
bool isInternetConnected = false;

void resetWiFiConf()
{
  WiFiFile::deleteWifiPassword();
  WiFiFile::deleteWifiSsid();
  Serial.println(Constant::RESET_CONF_MESSAGE);
  ESP.restart();
}

bool initWifi(const char *ssid, const char *password)
{
  WiFi.begin(ssid, password);
  Serial.println(Constant::WIFI_CONNECTING_MESSAGE);

  while (WiFi.status() != WL_CONNECTED)
  {
    currentTime = millis();
    if (Constant::CONNECTION_TIMEOUT - currentTime >= 0)
    {
      if (currentTime - previousTime >= Constant::RECONNECT_INTERVAL)
      {
        Serial.print(".");
        previousTime = currentTime;
      }
    }
    else
    {
      Serial.println(Constant::CONNECTION_TIMEOUT_MESSAGE);
      return false;
    }
  }

  return true;
}

void initDisplay()
{
  if (!oledI2c.begin(Constant::OLED_SDA_PIN, Constant::OLED_SCL_PIN))
  {
    Serial.println(Constant::OLED_INIT_FAILED_MESSAGE);
    return;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(Constant::OLED_INIT_FAILED_MESSAGE);
    return;
  }
}

bool initHotspot(const char *ssid, const char *password)
{
  if (WiFi.softAP(ssid, password))
  {
    Serial.println(Constant::HOTSPOT_STARTED_MESSAGE);
    return true;
  }
  else
  {
    Serial.println(Constant::HOTSPOT_START_FAILED_MESSAGE);
    return false;
  }
}

bool initFireBase()
{
  config.api_key = Constant::FIREBASE_AUTH_TOKEN;
  config.database_url = Constant::FIREBASE_DB_URL;
  auth.user.email = Constant::FIREBASE_AUTH_EMAIL;
  auth.user.password = Constant::FIREBASE_AUTH_PASSWORD;
  config.signer.test_mode = true; // Set to false for production
  config.signer.signup = true;    // Set to false for production
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println(Constant::FIREBASE_INIT_MESSAGE);
  if (!Firebase.ready())
  {
    isFirebaseConnected = false;
    return false;
  }

  Serial.println(Constant::FIREBASE_INIT_MESSAGE);
  isFirebaseConnected = true;
  return true;
}

bool initBlynk()
{
  Blynk.config(Constant::BLYNK_AUTH_TOKEN, Constant::BLYNK_SERVER, Constant::BLYNK_PORT);
  unsigned long startTime = millis();
  Serial.println(Constant::BLYNK_INIT_MESSAGE);
  while (!Blynk.connected())
  {
    if (millis() - startTime > Constant::BLYNK_TIMEOUT)
    {
      isBlynkConnected = false;
      return false;
    }
    Serial.print(".");
    delay(1000);
  }

  Serial.println(Constant::BLYNK_INIT_SUCCES_MESSAGE);
  isBlynkConnected = true;
  return true;
}

bool findWifiCredentials()
{

  WiFiSsid = WiFiFile::readWifiSsid();
  WiFiPassword = WiFiFile::readWifiPassword();
  if (WiFiSsid.isEmpty() || WiFiPassword.isEmpty())
  {
    return false;
  }
  return true;
}

bool findHotspotCredentials()
{
  bool hotspotSsidExists = WiFiFile::isHotspotSsidExists();
  bool hotspotPasswordExists = WiFiFile::isHotspotPasswordExists();
  if (!hotspotSsidExists || !hotspotPasswordExists)
  {
    hotspotSsid = Constant::DEFAULT_HOTSPOT_SSID;
    hotspotPassword = Constant::DEFAULT_HOTSPOT_PASSWORD;
    if (!WiFiFile::writeHotspotSsid(hotspotSsid) ||
        !WiFiFile::writeHotspotPassword(hotspotPassword))
    {
      return false;
    }

    return true;
  }

  hotspotSsid = WiFiFile::readHotspotSsid();
  hotspotPassword = WiFiFile::readHotspotPassword();

  return true;
}

bool findUserCredentials()
{
  bool usernameExists = UserFile::isUsernameExists();
  bool passwordExists = UserFile::isPasswordExists();
  if (!usernameExists || !passwordExists)
  {
    username = Constant::DEFAULT_USERNAME;
    password = Constant::DEFAULT_PASSWORD;
    if (!UserFile::writeUsername(username) || !UserFile::writePassword(password))
    {
      return false;
    }

    return true;
  }

  username = UserFile::readUsername();
  password = UserFile::readPassword();

  return true;
}

bool checkInternetConnection()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println(Constant::CHECKING_INTERNET_MESSAGE);
    HTTPClient http;
    http.setTimeout(Constant::INTERNET_CHECK_TIMEOUT);
    http.begin(Constant::INTERNET_CHECK_URL);
    int httpCode = http.GET();
    if (httpCode > 0 && httpCode == HTTP_CODE_OK)
    {
      Serial.println(Constant::INTERNET_CONNECTED_MESSAGE);
      isInternetConnected = true;
      return true;
    }
    http.end();
  }
  isInternetConnected = false;
  return false;
}

void handleWiFiConnection()
{
  if (initWifi(WiFiSsid.c_str(), WiFiPassword.c_str()))
  {
    if (!checkInternetConnection())
    {
      Serial.println(Constant::NO_INTERNET_MESSAGE);
      return;
    }

    if (!initFireBase())
    {
      Serial.println(Constant::FIREBASE_INIT_FAILED_MESSAGE);
      return;
    }

    if (!initBlynk())
    {
      Serial.println(Constant::BLYNK_INIT_FAILED_MESSAGE);
      return;
    }
  }
  else
  {
    resetWiFiConf();
  }
}

void handleHotspotConnection()
{
}

void setup()
{
  oledI2c.begin(Constant::OLED_SDA_PIN, Constant::OLED_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dhtSensor.begin();
  Serial.begin(115200);
  if (!findHotspotCredentials())
  {
    Serial.println("Failed to find or write hotspot credentials.");
    return;
  }

  if (!findUserCredentials())
  {
    Serial.println("Failed to find or write user credentials.");
    return;
  }

  if (findWifiCredentials())
  {
    handleWiFiConnection();
  }
  else
  {
    handleHotspotConnection();
  }
}
