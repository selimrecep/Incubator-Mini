#include "ostring.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "Arduino.h"
namespace utils
{

  ostring::ostring() : capacityLength(MIN_STRING_SIZE), stringLength(1)
  {
    allocateMemoryForCharArr();
    charArr[0] = '\0';
  }

  ostring::ostring(const unsigned long stringSize) : capacityLength(stringSize + 1)
  {
    this->stringLength = 1;
    allocateMemoryForCharArr();
    charArr[0] = '\0';
  }
  ostring::ostring(const char *charArrToCopyFrom)
  {
    this->capacityLength = strlen(charArrToCopyFrom) + 1;
    this->stringLength = this->capacityLength;
    allocateMemoryForCharArr();
    memcpy(this->charArr, charArrToCopyFrom, this->stringLength * sizeof(char));
  }

  ostring::ostring(const char ch) : capacityLength(2), stringLength(2)
  {
    allocateMemoryForCharArr();
    charArr[0] = ch;
    charArr[1] = '\0';
  }

  ostring::ostring(const ostring &other)
  {
    this->capacityLength = other.capacityLength;
    this->stringLength = other.stringLength;
    allocateMemoryForCharArr();
    memcpy(this->charArr, other.charArr, this->stringLength * sizeof(char));
  }

  ostring::ostring(ostring &&other)
  {
    changePrecision(floatingPointPrecision);
    this->capacityLength = other.capacityLength;
    this->stringLength = other.stringLength;
    this->charArr = other.charArr;
    other.charArr = nullptr;
  }

  void ostring::resize(unsigned long newSize)
  {
    if (newSize == 0)
      newSize = 1;
    ostring temp{*this};
    deallocateMemoryForCharArr();
    capacityLength = newSize;
    allocateMemoryForCharArr();
    if (temp.stringLength < newSize)
    {
      memcpy(charArr, temp.charArr, temp.stringLength - 1);
    }
    else
    {
      stringLength = newSize;
      memcpy(charArr, temp.charArr, newSize);
      charArr[stringLength - 1] = '\0';
    }
  }

  ostring &ostring::operator=(const ostring &other)
  {
    if (this == &other)
      return *this;

    if (this->capacityLength < other.stringLength)
    {
      deallocateMemoryForCharArr();
      this->capacityLength = other.capacityLength;
      allocateMemoryForCharArr();
      memcpy(this->charArr, other.charArr, other.stringLength * sizeof(char));
    }
    else
    {
      memcpy(this->charArr, other.charArr, other.stringLength * sizeof(char));
    }
    this->stringLength = other.stringLength;
    return *this;
  }

  ostring &ostring::operator=(ostring &&other)
  {
    if (this == &other)
      return *this;
    deallocateMemoryForCharArr();
    this->capacityLength = other.capacityLength;
    this->stringLength = other.stringLength;
    this->charArr = other.charArr;
    other.charArr = nullptr;
    return *this;
  }

  ostring operator+(const ostring &lhs, const ostring &rhs)
  {
    ostring total{lhs};

    total.resizeIfNotEnoughSpace(total.stringLength + rhs.stringLength - 1);
    memcpy(total.charArr + lhs.stringLength - 1, rhs.charArr, rhs.stringLength);

    total.stringLength = total.stringLength + rhs.stringLength - 1;
    return total;
  }
  ostring &operator+=(ostring &lhs, const ostring &rhs)
  {
    unsigned long prevSpace = lhs.stringLength;
    lhs.resizeIfNotEnoughSpace(lhs.stringLength + rhs.stringLength - 1);
    memcpy(lhs.charArr + prevSpace - 1, rhs.charArr, rhs.stringLength);
    lhs.stringLength = prevSpace + rhs.stringLength - 1;
    return lhs;
  }
  ostring &operator+=(ostring &lhs, const char ch)
  {
    char text[2];
    text[0] = ch;
    text[1] = '\0';
    lhs += text;
    return lhs;
  }
  ostring &operator+=(ostring &lhs, const int num)
  {
    char text[16];
    snprintf(text, 15, "%d", num);
    lhs += text;
    return lhs;
  }
  ostring &operator+=(ostring &lhs, const float num)
  {
    char text[32];
    snprintf(text, 32, lhs.floatFormat, num);
    lhs += text;
    return lhs;
  }
  ostring &operator+=(ostring &lhs, const double num)
  {
    char text[32];
    dtostrf(num, 4, lhs.floatingPointPrecision, text);
    lhs += text;
    return lhs;
  }
  bool operator==(const ostring &lhs, const ostring &rhs)
  {
    if (&lhs == &rhs)
      return true;
    if (lhs.stringLength != rhs.stringLength)
      return false;

    unsigned i = 0;
    while (i < lhs.stringLength && lhs.charArr[i] == rhs.charArr[i])
      i++;

    return i == lhs.stringLength;
  }

  bool operator==(const ostring &lhs, const char *&rhs)
  {
    unsigned i = 0;
    while (rhs[i] != '\0' && i < lhs.stringLength && lhs.charArr[i] == rhs[i])
      i++;

    return (i == lhs.stringLength - 1) && rhs[i] == '\0';
  }

  bool operator==(const char *&lhs, const ostring &rhs)
  {
    return rhs == lhs;
  }
  ostring ostring::operator()(unsigned long start, unsigned long end) const
  {
    if (end > stringLength - 1)
    {
      end = stringLength - 2;
    }
    if (start == end)
      return charArr[start];
    if (start > end)
      return ostring{};
    ostring substring{end - start + 1};
    memcpy(substring.charArr, charArr + start, end - start + 1);
    substring.charArr[end - start + 1] = '\0';
    substring.stringLength = end - start + 2;
    return substring;
  }
  const char ostring::operator()(unsigned long index) const
  {
    if (index > stringLength - 1)
      return '\0';
    return this->charArr[index];
  }
  const char ostring::operator[](unsigned long index) const
  {
    if (index > stringLength - 1)
      return '\0';
    return charArr[index];
  }

  char &ostring::operator[](unsigned long index)
  {
    return charArr[index];
  }

  void ostring::allocateMemoryForCharArr()
  {
    charArr = new char[capacityLength];
  }

  void ostring::deallocateMemoryForCharArr()
  {
    delete[] charArr;
  }
  void ostring::resizeIfNotEnoughSpace(unsigned long needed)
  {
    if (needed <= capacityLength)
      return;
    if (needed > capacityLength * 4)
    {
      resize(needed * 2);
    }
    else
    {
      resize(capacityLength * 4);
    }
  }
  void ostring::removeFromEnd(unsigned long count)
  {
    if (count >= stringLength)
    {
      stringLength = 1;
    }
    else
    {
      stringLength -= count;
    }

    charArr[stringLength - 1] = '\0';
  }
  const ostring *ostring::createSplit(char delimiter, int &n) const
  {
    const unsigned splitCount = occurenceCount(delimiter) + 1;
    ostring *strings = new ostring[splitCount];
    int *addressList = new int[splitCount];
    // First occurence
    addressList[0] = 0;
    int occurence = 0;

    for (unsigned i = 1; i < stringLength - 1; i++)
    {
      if (this->charArr[i] == delimiter)
      {
        // strings[occurence].charArr[i] = '\0';
        occurence++;
        addressList[occurence] = i + 1;
      }
    }

    //strings[occurence].charArr[stringLength - 1] = '\0';
    occurence++;
    for (int i = 0; i < occurence; i++)
    {
      int until;
      if (i == occurence - 1)
      {
        until = stringLength - 1;
      }
      else
      {
        until = addressList[i + 1] - 2;
      }
      strings[i] = (*this)(addressList[i], until) + "\0";
    }

    n = occurence;

    delete[] addressList;
    return strings;
  }

  unsigned long ostring::occurenceCount(char delimiter) const
  {
    unsigned long n = 0;
    for (unsigned i = 0; i < stringLength - 1; i++)
    {
      if (charArr[i] == delimiter)
        n++;
    }
    return n;
  }

  void ostring::changePrecision(unsigned precision)
  {
    if (precision == 0)
      precision = 1;
    floatFormat[2] = '0' + precision;
    doubleFormat[2] = '0' + precision;
  }
  bool ostring::endsWith(ostring substring)
  {
    if (substring.stringLength > this->stringLength)
      return false;
    int i = stringLength - 2;
    int j = substring.stringLength - 2;

    while (i >= 0 && j >= 0 && substring.charArr[j] == this->charArr[i])
    {
      j--;
      i--;
    }

    return j == -1;
  }
  void ostring::trimEnd(ostring delimiter)
  {
    if (!this->endsWith(delimiter))
      return;
    int i = stringLength - 2;
    int j = delimiter.stringLength - 2;

    while (i >= 0 && j >= 0 && delimiter.charArr[j] == this->charArr[i])
    {
      this->charArr[i] = '\0';
      this->stringLength--;
      j--;
      i--;
    }
  }
  void ostring::cutEnd(unsigned long length)
  {
    if (length > stringLength - 1)
    {
      length = stringLength - 1;
    }

    stringLength -= length;
    charArr[stringLength - 1] = '\0';
  }
  void ostring::changeStrLengthUnsafe(unsigned long lengthWithoutNull)
  {
    stringLength = lengthWithoutNull + 1;
    charArr[lengthWithoutNull] = '\0';
  }

  const char *ostring::c_str() const
  {
    return this->charArr;
  }

  long int ostring::toInt() const
  {
    long int ret;
    char *cp;
    ret = strtol(charArr, &cp, 10);
    return ret;
  }
  float ostring::toFloat() const
  {
    float ret;
    char *cp;
    ret = strtod(charArr, &cp);
    return ret;
  }
  double ostring::toDouble() const
  {
    double ret;
    char *cp;
    ret = strtod(charArr, &cp);
    return ret;
  }
  const unsigned long ostring::length() const
  {
    return this->stringLength;
  }
  const unsigned long ostring::capacity() const
  {
    return this->capacityLength;
  }
  ostring::~ostring()
  {
    deallocateMemoryForCharArr();
  }
}