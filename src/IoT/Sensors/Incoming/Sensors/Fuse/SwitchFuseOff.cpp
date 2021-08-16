#include "Arduino.h"
#include "SwitchFuseOff.h"
#include "../../../CommunicationManager.h"
#include "GlobalVars.h"

OutgoingSender SwitchFuseOff::handle(const utils::ostring *arguments, int length)
{
  OutgoingSender sender;
  sdm.switchOffFuse();
  return sender;
}

SwitchFuseOff::~SwitchFuseOff()
{
}