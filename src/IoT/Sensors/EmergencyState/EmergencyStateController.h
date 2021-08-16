#ifndef EMERGENCY_STATE_H
#define EMERGENCY_STATE_H

#include "IoT/Sensors/CommunicationManager.h"
#include "AsyncDelay.h"

constexpr int MAX_DANGER_ATTEMPT = 3;
constexpr int MAX_TEMP_MEASURUMENT_TICKS = 5;
constexpr float POSSIBLE_DANGER_TEMPERATURE_RISE = 6.6e-5;

constexpr unsigned long MAX_ALLOWED_TIME_FOR_INJURY = 10 * 60 * (unsigned long)1000;

constexpr float HOT_INJURY = 38.6;
constexpr float COLD_INJURY = 36.0;

class EmergencyStateController
{
public:
  EmergencyStateController(CommunicationManager &comm);
  void logTemperature(float temp);
  void loop();
  void userKnowsDanger();

  void kickInEmergencyState(bool sendMsg = true);

private:
  int tempMeausurementTick = 0;
  int dangerAttempts = 0;
  bool inDanger = false;
  float prevTemp = 0;
  float currTemp = 0;

  AsyncDelay coolDownForHeatInjuries{1000, AsyncDelay::MILLIS};
  AsyncDelay coolDownForColdInjuries{MAX_ALLOWED_TIME_FOR_INJURY, AsyncDelay::MILLIS};

  unsigned long prevTime;
  unsigned long currTime;

  CommunicationManager &comm;
  AsyncDelay loopDelay;
};
#endif