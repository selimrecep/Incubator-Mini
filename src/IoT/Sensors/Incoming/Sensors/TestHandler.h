#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H
#include "../IncomingHandler.h"
#include "../../CommunicationManager.h"
#include "../../Outgoing/OutgoingSender.h"
class TestHandler : public IncomingHandler
{
public:
  OutgoingSender handle(const utils::ostring *arguments, int length);
  ~TestHandler();
};
#endif