#ifndef LDR_H
#define LDR_H
class LDR
{
public:
  LDR(int pin);
  unsigned readValue();

private:
  int ldrPin;
};
#endif