#include "LDR.h"
#include "Arduino.h"

LDR::LDR(int pin) : ldrPin(pin) {}
unsigned LDR::readValue()
{
  return 1023 - analogRead(ldrPin);
}