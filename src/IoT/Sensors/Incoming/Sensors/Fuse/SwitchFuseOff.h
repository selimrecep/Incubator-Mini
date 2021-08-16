#ifndef SWITCH_FUSE_OFF_H
#define SWITCH_FUSE_OFF_H
#include "../../IncomingHandler.h"
#include "../../../CommunicationManager.h"
#include "../../../Outgoing/OutgoingSender.h"
class SwitchFuseOff : public IncomingHandler
{
public:
  OutgoingSender handle(const utils::ostring *arguments, int length);
  ~SwitchFuseOff();
};
#endif