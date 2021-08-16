#ifndef OUTGOING_SENDER_H
#define OUTGOING_SENDER_H
#include "Arduino.h"
#include "Utils/ostring.h"
constexpr unsigned MAX_CHAR_ARR_LEN = 32;

class OutgoingSender
{
public:
  /* NULL Sender*/
  OutgoingSender();
  OutgoingSender(const utils::ostring commandName, unsigned maxStringLength = MAX_CHAR_ARR_LEN);
  ~OutgoingSender();

  OutgoingSender &addDouble(double floatData);
  OutgoingSender &addInt(int intData);
  OutgoingSender &addString(utils::ostring str);

  void serializeAndSend(Stream *stream);
  bool isNull();

private:
  //int lastAddedArgIndex = 0;
  bool nullSender = false;
  utils::ostring command;
  //unsigned argumentCount;
  unsigned maxStringLength;
};

#endif