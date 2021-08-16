#ifndef OSTRING_H
#define OSTRING_H

constexpr unsigned MIN_STRING_SIZE = 16;

namespace utils
{
  class ostring
  {
  public:
    ostring();
    ostring(const unsigned long stringSize);
    ostring(const char *charArr);
    ostring(const char ch);

    /* Copy cont */
    ostring(const ostring &other);

    /* Move cont */
    ostring(ostring &&other);

    /* Methods */
    void resize(unsigned long newSize);
    void resizeIfNotEnoughSpace(unsigned long needed);
    void removeFromEnd(unsigned long count);
    const ostring *createSplit(char delimiter, int &n) const;
    unsigned long occurenceCount(char delimiter) const;
    void changePrecision(unsigned precision);
    bool endsWith(ostring delimiter);
    void trimEnd(ostring delimiter);
    void cutEnd(unsigned long length);
    void changeStrLengthUnsafe(unsigned long length);

    /* copy assignment */
    ostring &operator=(const ostring &other);

    /* Move assignment operator */
    ostring &operator=(ostring &&other);

    /* Arithmetic operators */
    friend ostring operator+(const ostring &lhs, const ostring &rhs);
    friend ostring &operator+=(ostring &lhs, const ostring &rhs);
    friend ostring &operator+=(ostring &lhs, const char ch);
    friend ostring &operator+=(ostring &lhs, const int num);
    friend ostring &operator+=(ostring &lhs, const float num);
    friend ostring &operator+=(ostring &lhs, const double num);
    friend bool operator==(const ostring &lhs, const ostring &rhs);
    friend bool operator==(const ostring &lhs, const char *&rhs);
    friend bool operator==(const char *&lhs, const ostring &rhs);
    ostring operator()(unsigned long start, unsigned long end) const;
    const char operator()(unsigned long index) const;
    const char operator[](unsigned long index) const;
    char &operator[](unsigned long index);

    /* Conversion Methods */
    long int toInt() const;
    float toFloat() const;
    double toDouble() const;

    /* Get sizes */
    const unsigned long length() const;
    const unsigned long capacity() const;
    const char *c_str() const;

    ~ostring();

    void pseudoSplit(char delimiter) = delete;

  private:
    char floatFormat[5] = "%.3f";
    char doubleFormat[6] = "%.3lf";
    int floatingPointPrecision = 3;
    void allocateMemoryForCharArr();
    void deallocateMemoryForCharArr();
    /* Includes NULL */
    unsigned long capacityLength;
    /* Includes NULL */
    unsigned long stringLength;
    char *charArr;
  };
};

#endif