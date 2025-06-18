#include <WiFiFile.h>
#include <FileUtils.h>
#include <Arduino.h>
#include <Constant.h>

namespace WiFiFile
{
  String readHotspotSsid()
  {
    return FileUtils::readFile(Constant::HOTSPOT_SSID_PATH);
  }

  bool writeHotspotSsid(const String &ssid)
  {
    return FileUtils::writeFile(Constant::HOTSPOT_SSID_PATH, ssid);
  }

  String readHotspotPassword()
  {
    return FileUtils::readFile(Constant::HOTSPOT_PASSWORD_PATH);
  }

  bool writeHotspotPassword(const String &password)
  {
    return FileUtils::writeFile(Constant::HOTSPOT_PASSWORD_PATH, password);
  }

  String readWifiSsid()
  {
    return FileUtils::readFile(Constant::WIFI_SSID_PATH);
  }

  bool writeWifiSsid(const String &ssid)
  {
    return FileUtils::writeFile(Constant::WIFI_SSID_PATH, ssid);
  }

  String readWifiPassword()
  {
    return FileUtils::readFile(Constant::WIFI_PASSWORD_PATH);
  }

  bool writeWifiPassword(const String &password)
  {
    return FileUtils::writeFile(Constant::WIFI_PASSWORD_PATH, password);
  }

  bool deleteHotspotSsid()
  {
    return FileUtils::deleteFile(Constant::HOTSPOT_SSID_PATH);
  }

  bool deleteHotspotPassword()
  {
    return FileUtils::deleteFile(Constant::HOTSPOT_PASSWORD_PATH);
  }

  bool deleteWifiSsid()
  {
    return FileUtils::deleteFile(Constant::WIFI_SSID_PATH);
  }

  bool deleteWifiPassword()
  {
    return FileUtils::deleteFile(Constant::WIFI_PASSWORD_PATH);
  }

  bool isHotspotSsidExists()
  {
    return FileUtils::fileExists(Constant::HOTSPOT_SSID_PATH);
  }

  bool isHotspotPasswordExists()
  {
    return FileUtils::fileExists(Constant::HOTSPOT_PASSWORD_PATH);
  }

  bool isWifiSsidExists()
  {
    return FileUtils::fileExists(Constant::WIFI_SSID_PATH);
  }

  bool isWifiPasswordExists()
  {
    return FileUtils::fileExists(Constant::WIFI_PASSWORD_PATH);
  }

}