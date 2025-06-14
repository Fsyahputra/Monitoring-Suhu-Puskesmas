#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>
#include <json_generator.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>

const int DHT_PIN = 4;
const int DHT_TYPE = DHT22;
const int LED_PIN = 2;
const char *AUTH_TOKEN = "your_auth_token";
const char *BLYNK_SERVER = "blynk-cloud.com";
const int BLYNK_PORT = 80;

String ssid;
String password;

unsigned long lastTime = 0;
unsigned long timeOutInterval = 60000;
unsigned long reconnectInterval = 3000;

const char *PARAM_1 = "ssid";
const char *PARAM_2 = "password";

const char *ssid_path = "/ssid.txt";
const char *password_path = "/password.txt";

AsyncWebServer server(80);
LiquidCrystal_I2C *lcd = nullptr;
DHT dht(DHT_PIN, DHT_TYPE);

struct DHTData
{
  float temperature;
  float humidity;
};

String processor(const String &var)
{
  if (var == "IP_ADDRESS")
  {
    String ipAddress = WiFi.localIP().toString();
    return ipAddress;
  }

  if (var == "SSID")
  {
    return ssid.isEmpty() ? "Not Set" : ssid;
  }

  if (var == "TEMPERATURE")
  {
    DHTData data = readDHT();
    return String(data.temperature);
  }
  if (var == "HUMIDITY")
  {
    DHTData data = readDHT();
    return String(data.humidity);
  }
  return String();
}

void initLittleFS()
{
  if (!LittleFS.begin())
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  Serial.println("LittleFS Mounted Successfully");
}

DHTData readDHT()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT sensor!");
    return {0, 0};
  }

  return {temperature, humidity};
}

String getSensorData()
{
  DHTData data = readDHT();
  StaticJsonDocument<300> SensorData;
  SensorData['temperature'] = data.temperature;
  SensorData['humidity'] = data.humidity;
  String jsonString;
  serializeJson(SensorData, jsonString);
  return jsonString;
}

LiquidCrystal_I2C *initLCD()
{
  byte lcdAddress = findLCD();
  if (lcdAddress == 0)
  {
    Serial.println("No I2C LCD Found");
    return nullptr;
  }

  Serial.printf("I2C LCD found at address 0x%02X\n", lcdAddress);
  LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(lcdAddress, 16, 2);
  lcd->init();
  lcd->backlight();
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Initializing...");
  delay(1000);
  lcd->clear();
  return lcd;
}

bool isValid(File file)
{
  if (!file || file.isDirectory())
  {
    Serial.println("Failed to open file for reading");
    file.close();
    return false;
  }
  return true;
}

byte findLCD()
{
  byte error, address;
  Wire.begin();
  Serial.println("Scanning for I2C devices...");
  for (address = 1; address <= 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      return address;
    }
    else if (error == 4)
    {
      Serial.printf("Unknown error at address 0x%02X\n", address);
    }
    else if (error == 2)
    {
      Serial.printf("Address 0x%02X is not responding\n", address);
    }
    else if (error == 1)
    {
      Serial.printf("Address 0x%02X is busy\n", address);
    }
    else if (error == 3)
    {
      Serial.printf("Address 0x%02X is not connected\n", address);
    }
  }

  return 0;
}

String readFile(fs::FS &fs, const char *path)
{
  File file = fs.open(path, FILE_READ);
  if (!isValid(file))
  {
    Serial.println("File not found or is a directory");
    return String();
  }

  String content;
  while (file.available())
  {
    content += file.readStringUntil('\n');
    break;
  }
  file.close();
  return content;
}

String readSSID()
{
  return readFile(LittleFS, ssid_path);
}

String readPassword()
{
  return readFile(LittleFS, password_path);
}

bool updateConfig(AsyncWebServerRequest *request)
{

  if (!request->hasParam(PARAM_1) || !request->hasParam(PARAM_2))
  {
    Serial.println("Missing parameters");
    return false;
  }
  if (request->params() > 2)
  {
    Serial.println("Too many parameters");
    return false;
  }
  if (request->params() < 2)
  {
    Serial.println("Not enough parameters");
    return false;
  }
  if (request->params() == 0)
  {
    Serial.println("No parameters found");
    return false;
  }
  int paramsCount = request->params();
  for (int i = 0; i < paramsCount; i++)
  {
    const AsyncWebParameter *p = request->getParam(i);
    if (p->name() == PARAM_1)
    {
      ssid = p->value();
      writeFile(LittleFS, ssid_path, ssid);
    }
    else if (p->name() == PARAM_2)
    {
      password = p->value();
      writeFile(LittleFS, password_path, password);
    }
  }

  return true;
}

void writeFile(fs::FS &fs, const char *path, const String &message)

{
  File file = fs.open(path, FILE_WRITE);
  if (!isValid(file))
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  if (file.print(message))
  {
    Serial.println("File written successfully");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

void configUpdated()
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Config Updated");
  lcd->setCursor(0, 1);
  lcd->print("Restarting...");
  delay(2000);
}

void serveOnConnect()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html", false, processor); });

  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String sensorData = getSensorData();
              request->send(200, "application/json", sensorData); });

  server.on("/changeConfig", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/wifiManager.html", "text/html", false, processor); });

  server.on("/changeConfig", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (updateConfig(request))
              {
                configUpdated();
                request->send(200, "text/plain", "Config updated");
                ESP.restart();
              }
              else
              {
                request->send(400, "text/plain", "Invalid request");
              } });

  server.serveStatic("/", LittleFS, "/");
  server.begin();
}

void serveOnDisconnect()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/wifiManager.html", "text/html", false, processor); });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    if (updateConfig(request))
    {
      configUpdated();
      request->send(200, "text/plain", "Config updated");
      ESP.restart();
    }
    else
    {
      request->send(400, "text/plain", "Invalid request");
    } });
  server.serveStatic("/", LittleFS, "/");
}

void deleteFile(fs::FS &fs, const char *path)
{
  if (fs.exists(path))
  {
    if (fs.remove(path))
    {
      Serial.printf("File %s deleted successfully\n", path);
    }
    else
    {
      Serial.printf("Failed to delete file %s\n", path);
    }
  }
  else
  {
    Serial.printf("File %s does not exist\n", path);
  }
}

void deleteSSID()
{
  deleteFile(LittleFS, ssid_path);
}

void deletePassword()
{
  deleteFile(LittleFS, password_path);
}

bool initWifi()
{
  if (ssid.isEmpty() || password.isEmpty())
  {
    Serial.println("SSID or Password is empty, cannot connect to WiFi");
    return false;
  }
  unsigned long currentTime;
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    currentTime = millis();
    if (currentTime - lastTime < timeOutInterval)
    {
      if (currentTime - lastTime > reconnectInterval)
      {
        Serial.print(".");
        lastTime = currentTime;
      }
    }
    else
    {
      Serial.println();
      Serial.println("Connection timed out, restarting...");
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("Connection timed out");
      lcd->setCursor(0, 1);
      lcd->print("Restarting...");
      delay(2000);
      lcd->clear();
      WiFi.disconnect();
      deleteSSID();
      deletePassword();
      ESP.restart();
      return false;
    }
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void setup()
{
  Serial.begin(115200);
  initLittleFS();
  ssid = readSSID();
  password = readPassword();
  dht.begin();
  lcd = initLCD();
  Blynk.config(AUTH_TOKEN, BLYNK_SERVER, BLYNK_PORT);
}