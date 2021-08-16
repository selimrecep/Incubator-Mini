#include "DataHandlers.h"
#include "Incoming/Sensors/TestHandler.h"
#include "Incoming/Sensors/Fuse/SwitchFuseOff.h"
DataHandlers::DataHandlers()
{
  initializeHandlers();
}
void DataHandlers::initializeHandlers()
{
  // Don't forget to change MAX handler count!
  registerHandler(IncomingHandler_t::TestHandler_t, new TestHandler());
  registerHandler(IncomingHandler_t::SwitchOffFuseHandler_t, new SwitchFuseOff());
}

void DataHandlers::registerHandler(IncomingHandler_t handlerType, IncomingHandler *handler)
{
  handlers[handlerType] = handler;
}

IncomingHandler *DataHandlers::getHandler(IncomingHandler_t handlerType)
{
  return handlers[handlerType];
}

DataHandlers::~DataHandlers()
{
  for (int i = 0; i < handlerCount; i++)
  {
    delete handlers[i];
  }
}