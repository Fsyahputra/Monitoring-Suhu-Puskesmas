#ifndef WIFI_FILE_H
#define WIFI_FILE_H

#include <Arduino.h>

namespace WiFiFile
{

  String readHotspotSsid();
  bool writeHotspotSsid(const String &ssid);
  String readHotspotPassword();
  bool writeHotspotPassword(const String &password);

  String readWifiSsid();
  bool writeWifiSsid(const String &ssid);
  String readWifiPassword();
  bool writeWifiPassword(const String &password);

  bool deleteHotspotSsid();
  bool deleteHotspotPassword();
  bool deleteWifiSsid();
  bool deleteWifiPassword();

  bool isHotspotSsidExists();
  bool isHotspotPasswordExists();
  bool isWifiSsidExists();
  bool isWifiPasswordExists();

  bool saveWiFiCredentials(const String &ssid, const String &password);

}

#endif