#include "Buzzer.h"
#include "EasyBuzzer.h"

Buzzer::Buzzer(int pin) : buzzerPin(pin)
{
  EasyBuzzer.setPin(pin);
}

void Buzzer::initialize()
{
  pinMode(buzzerPin, OUTPUT);
}

void Buzzer::emergencyBuzz()
{
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
  delay(300);
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
  delay(300);
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);
}