#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_SSD1306.h>

namespace Utils
{
    void resetWiFiConf();
    bool initWifi();
    bool isInternetConnected(int timeout);
    bool connectToBlynk();
    bool connectToFirebase(FirebaseAuth &auth, FirebaseConfig &config);
    bool initDisplay(Adafruit_SSD1306 &display, TwoWire &oledI2c);
    bool initHotspot();
    bool isInternetConnectedForeground();
    bool isInternetConnectedBackground();
}

#endif // UTILS_H