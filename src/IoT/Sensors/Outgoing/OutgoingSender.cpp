#include "OutgoingSender.h"
#include "string.h"
#include "Utils/ostring.h"

OutgoingSender::OutgoingSender()
{
  nullSender = true;
}
OutgoingSender::OutgoingSender(const utils::ostring commandName, unsigned maxStringLengthWithoutTerminator) : command{(unsigned long)maxStringLengthWithoutTerminator + 1}, maxStringLength(maxStringLengthWithoutTerminator + 1)
{
  if (maxStringLength == 0)
  {
    nullSender = true;
    return;
  }
  addString(commandName);
}

OutgoingSender::~OutgoingSender()
{
}

OutgoingSender &OutgoingSender::addString(utils::ostring str)
{
  command += str;
  command += '|';
  return *this;
}

OutgoingSender &OutgoingSender::addDouble(double doubleData)
{
  command += doubleData;
  command += '|';
  return *this;
}
OutgoingSender &OutgoingSender::addInt(int intData)
{
  command += intData;
  command += '|';
  return *this;
}
void OutgoingSender::serializeAndSend(Stream *stream)
{
  command.cutEnd(1);
  command += '\n';
  stream->print(command.c_str());
}

bool OutgoingSender::isNull()
{
  return nullSender;
}