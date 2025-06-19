#include <Arduino.h>
#include <Constant.h>
#include <UserFile.h>
#include <WiFiFile.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Sensor.h>
#include <Utils.h>
#include <CustomJWT.h>

CustomJWT jwt(Constant::JWT_SECRET, 512);

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

volatile bool isInternetConnected = false;
volatile bool isBlynkConnected = false;
volatile bool isFirebaseConnected = false;

void reconnectTask(void *pvParameters)
{
  unsigned long internetPreviousTime = millis();
  unsigned long blynkPreviousTime = millis();
  unsigned long firebasePreviousTime = millis();
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      isInternetConnected = false;
    }
    else
    {
      if (millis() - internetPreviousTime >= Constant::INTERNET_CHECK_BACKGROUND_INTERVAL)
      {
        internetPreviousTime = millis();
        isInternetConnected = Utils::isInternetConnectedBackground();
      }
    }

    if (!isInternetConnected)
    {
      isFirebaseConnected = false;
      isBlynkConnected = false;
      vTaskDelay(5000 / portTICK_PERIOD_MS); // Wait for 5 seconds before next check
      continue;
    }

    if (millis() - blynkPreviousTime >= Constant::BLYNK_CHECK_BACKGROUND_INTERVAL)
    {
      blynkPreviousTime = millis();
      isBlynkConnected = Utils::connectToBlynk();
    }

    if (millis() - firebasePreviousTime >= Constant::FIREBASE_CHECK_BACKGROUND_INTERVAL)
    {
      firebasePreviousTime = millis();
      isFirebaseConnected = Utils::connectToFirebase(auth, config);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
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

bool handleWiFiConnection()
{
  if (!Utils::initWifi())
  {
    Serial.println(Constant::CONNECTION_TIMEOUT_MESSAGE);
    Utils::resetWiFiConf();
    return false;
  }

  bool isInternetConnected = Utils::isInternetConnectedForeground();
  bool isBlynkConnected = Utils::connectToBlynk();
  bool isFirebaseConnected = Utils::connectToFirebase(auth, config);
  Serial.println(Constant::WIFI_CONNECTED_MESSAGE);
  Serial.println(Constant::BLYNK_INIT_MESSAGE);
  Serial.println(Constant::FIREBASE_INIT_MESSAGE);
  if (!isInternetConnected)
  {
    Serial.println(Constant::NO_INTERNET_MESSAGE);
  }

  if (!isFirebaseConnected)
  {
    Serial.println(Constant::FIREBASE_INIT_FAILED_MESSAGE);
  }

  if (!isBlynkConnected)
  {
    Serial.println(Constant::BLYNK_INIT_FAILED_MESSAGE);
  }

  if (isInternetConnected)
  {
    Serial.println(Constant::INTERNET_CONNECTED_MESSAGE);
  }

  if (isFirebaseConnected)
  {
    Serial.println(Constant::FIREBASE_INIT_SUCCES_MESSAGE);
  }

  if (isBlynkConnected)
  {
    Serial.println(Constant::BLYNK_INIT_SUCCES_MESSAGE);
  }

  bool isInfraReady = isInternetConnected && isBlynkConnected && isFirebaseConnected;

  return isInfraReady;
}

bool handleHotspotConnection()
{
}

void setup()
{
  dhtSensor.begin();
  Serial.begin(115200);

  if (!Utils::initDisplay(display, oledI2c))
  {
    Serial.println(Constant::OLED_INIT_FAILED_MESSAGE);
  }

  if (!findHotspotCredentials())
  {
    Serial.println(Constant::WIFI_CREDENTIALS_NOT_FOUND_MESSAGE);
  }

  if (!findUserCredentials())
  {
    Serial.println(Constant::USER_CREDENTIALS_NOT_FOUND_MESSAGE);
  }

  if (!findWifiCredentials())
  {
    bool isHotspotReady = handleHotspotConnection();
  }
  else
  {
    bool isInfraReady = handleWiFiConnection();
    xTaskCreatePinnedToCore(reconnectTask, "Reconnect Task", 4096, NULL, 1, NULL, 1);
  }
}
