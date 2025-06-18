#ifndef CONSTANT_H
#define CONSTANT_H

namespace Constant
{
  // File paths
  const char *const HOTSPOT_SSID_PATH = "/config/hotspotSsid.txt";
  const char *const HOTSPOT_PASSWORD_PATH = "/config/hotspotPassword.txt";

  const char *const WIFI_SSID_PATH = "/config/wifiSsid.txt";
  const char *const WIFI_PASSWORD_PATH = "/config/wifiPassword.txt";

  const char *const USERNAME_PATH = "/config/username.txt";
  const char *const PASSWORD_PATH = "/config/password.txt";
  const char *const DEFAULT_USERNAME_PATH = "/config/defaultUsername.txt";
  const char *const DEFAULT_PASSWORD_PATH = "/config/defaultPassword.txt";

  const char *const FRONT_END_PATH = "/fe/index.html";
  const char *const FRONT_END_STATIC_PATH = "/fe/";

  // FireBase

  const char *const FIREBASE_DB_URL = "https://your-database.firebaseio.com/";
  const char *const FIREBASE_AUTH_EMAIL = "your-email@example.com";
  const char *const FIREBASE_AUTH_PASSWORD = "your-password";
  const char *const FIREBASE_AUTH_TOKEN = "your-auth-token";

  // Blynk

  const char *const BLYNK_AUTH_TOKEN = "your-blynk-auth-token";
  const char *const BLYNK_SERVER = "blynk-cloud.com";
  const int BLYNK_PORT = 80;
  const char *const BLYNK_DEVICE_NAME = "your-device-name";
  const char *const BLYNK_DEVICE_TYPE = "your-device-type";
  const char *const BLYNK_TEMPLATE_ID = "your-template-id";

  // Hardware

  const int RESET_BUTTON_PIN = 13; // GPIO pin for reset button
  const int BUZZER_PIN = 15;       // GPIO pin for buzzer
  const int LED_PIN = 2;           // GPIO pin for LED
  const int DHT_SDA_PIN = 21;      // GPIO pin for DHT sensor data
  const int OLED_SCL_PIN = 23;     // GPIO pin for OLED SCL
  const int OLED_SDA_PIN = 22;     // GPIO pin for OLED SDA
  const int DHT_TYPE = 1;          // DHT11 sensor type (0 for DHT11, 1 for DHT22)
  const int OLED_WIDTH = 128;      // OLED width
  const int OLED_HEIGHT = 64;      // OLED height

  // Timeout and Intervals
  const int CONNECTION_TIMEOUT = 10000;    // 10 seconds
  const int SENSOR_READ_INTERVAL = 5000;   // 5 seconds
  const int RECONNECT_INTERVAL = 3000;     // 3 seconds
  const int INTERNET_CHECK_TIMEOUT = 5000; // 5 seconds
  const int BLYNK_TIMEOUT = 15000;

  // Messages
  const char *const BOOT_MESSAGE = "Memulai Sistem...";
  const char *const SENSOR_INIT_MESSAGE = "Menginisialisasi Sensor Suhu dan RH";
  const char *const WIFI_CONNECTING_MESSAGE = "Menghubungkan ke WiFi...";
  const char *const WIFI_CONNECTED_MESSAGE = "Terhubung ke WiFi";
  const char *const CHECKING_INTERNET_MESSAGE = "Memeriksa Koneksi Internet...";
  const char *const INTERNET_CONNECTED_MESSAGE = "Koneksi Internet Tersedia";
  const char *const CONNECTION_TIMEOUT_MESSAGE = "Timeout Tercapai, Wifi Yang anda Masukkan Tidak Terdeteksi Silakan Configurasi Ulang";
  const char *const HOTSPOT_STARTED_MESSAGE = "Hotspot Dimulai";
  const char *const HOTSPOT_START_FAILED_MESSAGE = "Gagal Memulai Hotspot";
  const char *const RESET_CONF_MESSAGE = "Mengatur Ulang Konfigurasi...";
  const char *const OLED_INIT_FAILED_MESSAGE = "Gagal Inisialisasi OLED";
  const char *const FIREBASE_INIT_MESSAGE = "Menginisialisasi Firebase...";
  const char *const FIREBASE_INIT_FAILED_MESSAGE = "Gagal Inisialisasi Firebase";
  const char *const FIREBASE_INIT_SUCCES_MESSAGE = "Firebase Berhasil Diinisialisasi";
  const char *const NO_INTERNET_MESSAGE = "Tidak Ada Koneksi Internet";
  const char *const BLYNK_TIMEOUT_MESSAGE = "Blynk Timeout, Periksa Koneksi Internet";
  const char *const BLYNK_INIT_MESSAGE = "Menginisialisasi Blynk...";
  const char *const BLYNK_INIT_SUCCES_MESSAGE = "Blynk Berhasil Diinisialisasi";
  const char *const BLYNK_INIT_FAILED_MESSAGE = "Gagal Inisialisasi Blynk";

  // Server

  const int SERVER_PORT = 80;
  const char *const NTP_SERVER = "pool.ntp.org";

  // Timezone
  const char *const TIMEZONE = "WITA+8"; // Indonesia Western Time (WITA)
  const int TIMEZONE_OFFSET = 8 * 3600;  // Offset in seconds (WITA is UTC+8)

  // Internet Check
  const char *const INTERNET_CHECK_URL = "http://client3.google.com/generate_204";

  // Sensor Payload Structure
  struct SensorPayload
  {
    float temperature;
    float humidity;
    char timestamp[20]; // Format: YYYY-MM-DD HH:MM:SS
  };

  extern SensorPayload sensorPayload;

  // Default Values

  const char *const DEFAULT_HOTSPOT_SSID = "MONITORING_HOTSPOT";
  const char *const DEFAULT_HOTSPOT_PASSWORD = "12345678";

  const char *const DEFAULT_USERNAME = "admin";
  const char *const DEFAULT_PASSWORD = "admin123";

  const char *const DEFAULT_WIFI_SSID = "Your_WiFi_SSID";
  const char *const DEFAULT_WIFI_PASSWORD = "Your_WiFi_Password";
}
#endif