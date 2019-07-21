#ifndef CAR_TIMERS
#define CAR_TIMERS

#include <Arduino.h>

void resetMotorsTimer();
boolean motorsTimerExpired();

void resetHandleSensorsTimer();
boolean handleSensorsTimerExpired();

void resetReadSensorsTimer();
boolean readSensorsTimerExpired();

void resetContinueTimer();
boolean continueTimerExpired();

void resetSaveHistoryTimer();
boolean saveHistoryTimerExpired();

#endif
