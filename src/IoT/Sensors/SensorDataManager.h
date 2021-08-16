#ifndef SENSOR_DATA_MANAGER_H
#define SENSOR_DATA_MANAGER_H
#include "CommunicationManager.h"
#include "AsyncDelay.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Servo.h>
#include "Buzzer/Buzzer.h"
#include "LDR/LDR.h"
#include "EmergencyState/EmergencyStateController.h"

#define DHT_TYPE DHT22

constexpr unsigned MAX_SENSOR_COUNT = 1;

enum Sensor_t
{
  DHT11_t = 0
};

class SensorDataManager
{
public:
  SensorDataManager(CommunicationManager &comm);
  ~SensorDataManager();

  void switchOffFuse();
  void sendHumidity(float humidity);
  void sendTemp(float temperature);

  void calibrateHumid(double offsetHumid);
  void calibrateTemp(double offsetTemp);

  void initialize();
  void setMinDelay(unsigned long d);
  void loop();

  EmergencyStateController &getEmergencyController();
  Buzzer &getBuzzer();

  void forceDanger();

  double tempAddition = 0.0;
  double humidAddition = 0.0;

private:
  CommunicationManager &comm;
  sensor_t humTempSensor;
  DHT_Unified dht;
  unsigned sensorCount = 0;

  bool test = 0;

  Buzzer buzzer;
  LDR ldr;

  AsyncDelay tempHumidReadingDelay;
  AsyncDelay fuseServoLockerDelay{3000, AsyncDelay::MILLIS};
  AsyncDelay ldrReading{1000, AsyncDelay::MILLIS};

  Servo fuseServo;
  EmergencyStateController emergencyController;

  void innerLoop();
};
#endif