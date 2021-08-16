#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include "Arduino.h"
#include "Incoming/IncomingHandler.h"
#include "DataHandlers.h"
#include "Outgoing/OutgoingSender.h"
#include "../../Utils/ostring.h"
class CommunicationManager
{
public:
  CommunicationManager(Stream &serial);
  void addCharToQueue(char input);
  void sendDataToStream(OutgoingSender &packet);
  Stream *getStream();
  bool ready = false;

private:
  DataHandlers packetHandlers;
  size_t commandLength = 0;
  //char lastCommand[32];
  utils::ostring lastCommand;
  Stream &serial;
  void incomingData(utils::ostring lastCommand);
  bool lockDown = false;
};

#endif