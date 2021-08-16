#include "Arduino.h"
#include "TestHandler.h"
#include "../../CommunicationManager.h"

OutgoingSender TestHandler::handle(const utils::ostring *arguments, int length)
{
  /*Serial.print("Hey gtfo: ");
  Serial.println(arguments[1][0] == '1');*/
  OutgoingSender sender("shutup", 15);
  sender.addString("u suck").addInt(5);
  //OutgoingSender sender;
  return sender;
}

TestHandler::~TestHandler()
{
}