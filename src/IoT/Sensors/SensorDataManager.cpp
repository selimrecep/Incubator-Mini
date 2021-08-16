#include "SensorDataManager.h"
#include "CommunicationManager.h"
#include "AsyncDelay.h"
#include <Servo.h>
#include "Arduino.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "EEPROM.h"
#include "Pins.h"

SensorDataManager::SensorDataManager(CommunicationManager &comm) : comm(comm), dht(TEMP_HUMID_SENSOR_PIN, DHT_TYPE), buzzer{BUZZER_PIN}, ldr{LDR_PIN}, emergencyController{comm}
{
  fuseServoLockerDelay.expire();
}
void SensorDataManager::initialize()
{
  EEPROM.get(0, humidAddition);
  EEPROM.get(0 + sizeof(float), tempAddition);

  pinMode(EMERGENCY_BUTT_PIN, INPUT);

  buzzer.initialize();
  dht.begin();

  fuseServo.attach(SERVO_PIN);
  fuseServo.write(120);

  dht.temperature().getSensor(&humTempSensor);
  dht.humidity().getSensor(&humTempSensor);
  tempHumidReadingDelay.start(humTempSensor.min_delay / 1000, AsyncDelay::MILLIS);
}
void SensorDataManager::loop()
{
  innerLoop();
}

void SensorDataManager::switchOffFuse()
{
  if (fuseServoLockerDelay.isExpired())
  {
    for (int i = 0; i < 3; i++)
    {
      fuseServo.write(120);
      delay(300);
      fuseServo.write(15);
      delay(600);
    }

    fuseServo.write(120);
    OutgoingSender sender{"rsfuse", 16};
    sender.addInt(65);
    sender.serializeAndSend(comm.getStream());
    fuseServoLockerDelay.start(3000, AsyncDelay::MILLIS);
  }
}

void SensorDataManager::sendHumidity(float humidity)
{
  OutgoingSender packet{"humid"};
  packet
      .addDouble(humidity);

  packet.serializeAndSend(comm.getStream());
}

void SensorDataManager::sendTemp(float temp)
{
  OutgoingSender packet{"temp"};
  packet
      .addDouble(temp);

  packet.serializeAndSend(comm.getStream());
}

void SensorDataManager::calibrateHumid(double offsetHumid)
{
  Serial.print("humid: ");
  Serial.println(offsetHumid);
  EEPROM.put(0, offsetHumid);
  humidAddition = offsetHumid;
}
void SensorDataManager::calibrateTemp(double offsetTemp)
{
  Serial.print("temp: ");
  Serial.println(offsetTemp);
  EEPROM.put(0 + sizeof(double), offsetTemp);
  tempAddition = offsetTemp;
}

void SensorDataManager::innerLoop()
{
  emergencyController.loop();

  delay(5);

  if (tempHumidReadingDelay.isExpired())
  {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    delay(50);
    if (!isnan(event.temperature))
    {

      emergencyController.logTemperature(event.temperature);
      sendTemp(event.temperature);
    }
    dht.humidity().getEvent(&event);

    if (!isnan(event.relative_humidity))
    {
      sendHumidity(event.relative_humidity);
    }
    tempHumidReadingDelay.repeat();
  }
  delay(5);

  /* ANALOG READS */
  if (ldrReading.isExpired())
  {
    ldrReading.start(1000, AsyncDelay::MILLIS);
    unsigned val = ldr.readValue();
    OutgoingSender ldrSender{"ldr", 12};
    ldrSender.addInt(val);
    ldrSender.serializeAndSend(comm.getStream());
  }
  delay(5);
}

void SensorDataManager::forceDanger()
{
  emergencyController.kickInEmergencyState();
}

EmergencyStateController &SensorDataManager::getEmergencyController()
{
  return emergencyController;
}
Buzzer &SensorDataManager::getBuzzer()
{
  return buzzer;
}
SensorDataManager::~SensorDataManager()
{
}
