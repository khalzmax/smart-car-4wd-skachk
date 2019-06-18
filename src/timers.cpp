#include <Arduino.h>

unsigned long motorsStartTime = 0;
const int motorsDelayTime = 50;
void resetMotorsTimer()
{
    motorsStartTime = millis();
}

unsigned long printSensorsStartTime = 0;
const int printSensorsDelayTime = 1000;
void resetPrintSensorsTimer()
{
    printSensorsStartTime = millis();
}
int printSensorsTimerExpired() {
    return (millis() - printSensorsStartTime > printSensorsDelayTime);
}


unsigned long readSensorsStartTime = 0;
const int readSensorsDelayTime = 1;
void resetReadSensorsTimer()
{
    readSensorsStartTime = millis();
}
int readSensorsTimerExpired()
{
    return (millis() - readSensorsStartTime > readSensorsDelayTime);
}

unsigned long continueStartTime = 0;
const int continueDelayTime = 2000;
void resetContinueTimer()
{
    continueStartTime = millis();
}
int continueTimerExpired()
{
    return (millis() - continueStartTime > continueDelayTime);
}