#include "Timer.h"

Timer::Timer(uint16_t delay = 0)
{
  delayTime = delay;
  startTime = millis();
}
void Timer::resetTimer()
{
  startTime = millis();
}
boolean Timer::timerExpired()
{
  return (millis() - startTime > delayTime);
}
