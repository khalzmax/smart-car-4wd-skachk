#include <Arduino.h>
#include "timers.h"

unsigned long motorsStartTime = 0;
const int motorsDelayTime = 50;
void resetMotorsTimer()
{
    motorsStartTime = millis();
}
boolean motorsTimerExpired()
{
    return (millis() - motorsStartTime < motorsDelayTime); // TODO replace < with > ??
}

unsigned long handleSensorsStartTime = 0;
const int handleSensorsDelayTime = 1000;
void resetHandleSensorsTimer()
{
    handleSensorsStartTime = millis();
}
boolean handleSensorsTimerExpired()
{
  return (millis() - handleSensorsStartTime > handleSensorsDelayTime);
}


unsigned long readSensorsStartTime = 0;
const int readSensorsDelayTime = 0;
void resetReadSensorsTimer()
{
    readSensorsStartTime = millis();
}
boolean readSensorsTimerExpired()
{
    return (millis() - readSensorsStartTime > readSensorsDelayTime);
}

unsigned long continueStartTime = 0;
const int continueDelayTime = 4000;
void resetContinueTimer()
{
    continueStartTime = millis();
}
boolean continueTimerExpired()
{
    return (millis() - continueStartTime > continueDelayTime);
}

unsigned long saveHistoryStartTime = 0;
const int saveHistoryDelayTime = 500;
void resetSaveHistoryTimer()
{
  saveHistoryStartTime = millis();
}
boolean saveHistoryTimerExpired()
{
  return (millis() - saveHistoryStartTime > saveHistoryDelayTime);
}
