#include "CommunicationManager.h"
#include "Utils/ostring.h"
#include "DataHandlers.h"
#include "GlobalVars.h"

int countOccurences(char *charArr, int length, char delim);

CommunicationManager::CommunicationManager(Stream &serial) : packetHandlers{}, lastCommand{(unsigned long)32}, serial(serial)
{
}

void CommunicationManager::incomingData(utils::ostring lastCommand)
{
  int argumentCount;
  /*
  Serial.print("> ");
  Serial.println(lastCommand.c_str());
  Serial.print(lastCommand.capacity());
  Serial.print("_");
  Serial.println(lastCommand.length());*/
  const utils::ostring *arguments = lastCommand.createSplit('|', argumentCount);
  if (argumentCount < 2)
  {
    delete[] arguments;
    return;
  }
  /* Handling incoming packets */
  //Serial.println(arguments[0].c_str());
  /*
  Serial.println("a");
  Serial.println(arguments[0].c_str());*/
  IncomingHandler_t handlerType = NullHandler_t;
  if (ready)
  {
    if (arguments[0] == "test")
    {
      handlerType = IncomingHandler_t::TestHandler_t;
    }
    else if (arguments[0] == "sfuse")
    {
      handlerType = IncomingHandler_t::SwitchOffFuseHandler_t;
    }
    else if (arguments[0] == "danger")
    {
      sdm.switchOffFuse();
      sdm.getEmergencyController().kickInEmergencyState();
    }
    else if (arguments[0] == "calibrate" && argumentCount == 3)
    {
      if (arguments[1].toInt())
      {
        // Temp
        sdm.calibrateTemp(arguments[2].toDouble());
      }
      else
      {
        // Humid
        sdm.calibrateHumid(arguments[2].toDouble());
      }
    }
    if (handlerType != IncomingHandler_t::NullHandler_t)
    {
      OutgoingSender responseSender = packetHandlers.getHandler(handlerType)->handle(arguments, argumentCount);
      if (!responseSender.isNull())
      {
        responseSender.serializeAndSend(&serial);
      }
    }
  }
  delete[] arguments;
}

void CommunicationManager::addCharToQueue(char ch)
{
  // Umm remove ch == '\n' ?
  if (ch == '\0' || ch == '\r' || ch == '\n' || commandLength >= 31)
  {
    if (commandLength >= 1)
    {
      if (lockDown)
      {
        lockDown = false;
        return;
      }
      lastCommand.changeStrLengthUnsafe(commandLength);
      /* Serial.print("loopback>");
      Serial.println(lastCommand.c_str());*/
      incomingData(lastCommand);
      commandLength = 0;
    }
  }
  else if (lockDown == false)
  {
    if (commandLength == 0 && ch == 'l')
    {
      lockDown = true;
      return;
    }
    lastCommand[commandLength++] = ch;
  }
}

Stream *CommunicationManager::getStream()
{
  return &serial;
}

int countOccurences(char *charArr, int length, char delim)
{
  int i = 0;
  int n = 0;
  for (i = 0; i < length; i++)
  {
    if (charArr[i] == delim)
      n++;
  }
  return n;
}