#include <Sensor.h>
#include <Constant.h>

namespace Sensor
{
  Constant::SensorPayload readSensorData(DHT sensor)
  {
    float temperature = sensor.readTemperature();
    float humidity = sensor.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
      Serial.println("Failed to read from DHT sensor!");
      return {NAN, NAN, "1970-01-01 00:00:00"};
    }

    Constant::SensorPayload payload;
    payload.temperature = temperature;
    payload.humidity = humidity;
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
      strftime(payload.timestamp, sizeof(payload.timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
    }
    else
    {
      strncpy(payload.timestamp, "1970-01-01 00:00:00", sizeof(payload.timestamp)); // Fallback if time is not available
    }

    return payload;
  }
}