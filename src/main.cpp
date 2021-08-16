#include <Arduino.h>
#include "IoT/Sensors/CommunicationManager.h"
#include "IoT/Sensors/SensorDataManager.h"
#include <SoftwareSerial.h>

//SoftwareSerial esp8266Serial(2, 3);

CommunicationManager comm{Serial};
SensorDataManager sdm{comm};

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(38400);
  //esp8266Serial.begin(9600);
  //pinMode(6, OUTPUT);
  //digitalWrite(6, LOW);
  sdm.initialize();
  pinMode(LED_BUILTIN, OUTPUT);

  comm.ready = true;
}

void loop()
{
  if (Serial.available())
  {
    char ch(Serial.read());
    //esp8266Serial.write(ch);
    //digitalWrite(6, HIGH);
    //delay(200);
    //digitalWrite(6, LOW);
    comm.addCharToQueue(ch);
  }
  /*
  if (esp8266Serial.available())
  {
    char ch(Serial.read());
    Serial.write(ch);
    comm.addCharToQueue(ch);
  }*/
  sdm.loop();
}
