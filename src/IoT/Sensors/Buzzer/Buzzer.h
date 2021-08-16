#ifndef BUZZER_H
#define BUZZER_H
class Buzzer
{
public:
  Buzzer(int pin);
  void initialize();
  void emergencyBuzz();

private:
  int buzzerPin;
};
#endif