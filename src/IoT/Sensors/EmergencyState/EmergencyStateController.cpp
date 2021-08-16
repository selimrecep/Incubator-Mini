#include "EmergencyStateController.h"
#include "IoT/Sensors/Outgoing/OutgoingSender.h"
#include "IoT/Sensors/CommunicationManager.h"
#include "AsyncDelay.h"
#include "GlobalVars.h"
#include "Pins.h"

EmergencyStateController::EmergencyStateController(CommunicationManager &comm) : comm(comm)
{
  loopDelay.start(4000, AsyncDelay::MILLIS);
}

void EmergencyStateController::logTemperature(float temp)
{
  if (coolDownForHeatInjuries.isExpired())
  {
    if (temp + sdm.tempAddition > HOT_INJURY)
    {
      Serial.println("HOTINJURY|0000");
      sdm.switchOffFuse();
      kickInEmergencyState();
      tempMeausurementTick = 0;
      coolDownForHeatInjuries.start(MAX_ALLOWED_TIME_FOR_INJURY, AsyncDelay::MILLIS);
      coolDownForColdInjuries.start(1000, AsyncDelay::MILLIS);
    }
    else
    {
      coolDownForHeatInjuries.start(1000, AsyncDelay::MILLIS);
    }
  }

  if (coolDownForColdInjuries.isExpired())
  {
    if (temp + sdm.tempAddition < COLD_INJURY)
    {
      Serial.println("COLDINJURY|0000");
      kickInEmergencyState(false);
      OutgoingSender inDangerSender{"inDanger|2", 14};
      inDangerSender.serializeAndSend(comm.getStream());
      tempMeausurementTick = 0;
      coolDownForColdInjuries.start(MAX_ALLOWED_TIME_FOR_INJURY, AsyncDelay::MILLIS);
      coolDownForHeatInjuries.start(1000, AsyncDelay::MILLIS);
    }
    else
    {
      coolDownForColdInjuries.start(1000, AsyncDelay::MILLIS);
    }
  }

  if (tempMeausurementTick == 0)
  {
    prevTime = millis();
    prevTemp = temp;
    tempMeausurementTick++;
  }
  else if (tempMeausurementTick < MAX_TEMP_MEASURUMENT_TICKS)
  {
    tempMeausurementTick++;
  }
  else
  {
    currTime = millis();
    currTemp = temp;
    if ((currTemp - prevTemp) / (currTime - prevTime) >= POSSIBLE_DANGER_TEMPERATURE_RISE)
      dangerAttempts++;

    if (dangerAttempts > MAX_DANGER_ATTEMPT)
    {
      sdm.switchOffFuse();
      kickInEmergencyState();
      dangerAttempts = 0;
    }

    tempMeausurementTick = 0;
  }
}

void EmergencyStateController::loop()
{
  if (inDanger && digitalRead(EMERGENCY_BUTT_PIN))
  {
    userKnowsDanger();
  }
  if (loopDelay.isExpired())
  {
    if (inDanger)
    {
      sdm.getBuzzer().emergencyBuzz();
    }
    loopDelay.start(3000, AsyncDelay::MILLIS);
  }
}

void EmergencyStateController::userKnowsDanger()
{
  inDanger = false;
  OutgoingSender inDangerSender{"inDanger|0", 14};
  inDangerSender.serializeAndSend(comm.getStream());
}

void EmergencyStateController::kickInEmergencyState(bool sendMsg = true)
{
  inDanger = true;
  if (sendMsg)
  {
    OutgoingSender inDangerSender{"inDanger|1", 14};
    inDangerSender.serializeAndSend(comm.getStream());
  }
}