#ifndef CAR_TIMER
#define CAR_TIMER

#include <Arduino.h>

class Timer
{
public:
  Timer(uint16_t delay);
  // int8_t add(uint16_t delay);
  void resetTimer();
  boolean timerExpired();

private:
  uint16_t delayTime;
  unsigned long startTime;
  // int8_t nextTimerId = 0;
};

#endif
