#ifndef SENSOR_H
#define SENSOR_H

#include <Constant.h>
#include <DHT.h>

namespace Sensor
{
  Constant::SensorPayload readSensorData(DHT sensor);
}

#endif // SENSOR_H