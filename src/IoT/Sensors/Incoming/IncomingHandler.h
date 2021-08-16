#ifndef INCOM_PACKET_H
#define INCOM_PACKET_H

#include "../Outgoing/OutgoingSender.h"
#include "Utils/ostring.h"
class IncomingHandler
{
public:
  virtual OutgoingSender handle(const utils::ostring *arguments, int length) = 0;
  virtual ~IncomingHandler();
};
#endif
