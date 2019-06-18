#include <Arduino.h>

unsigned long motorsStartTime = 0;
int motorsDelayTime = 50;
void resetMotorsTimer()
{
    motorsStartTime = millis();
}

unsigned long printSensorsStartTime = 0;
int printSensorsDelayTime = 1000;
void resetPrintSensorsTimer()
{
    printSensorsStartTime = millis();
}
int printSensorsTimerExpired() {
    return (millis() - printSensorsStartTime > printSensorsDelayTime);
}


unsigned long readSensorsStartTime = 0;
int readSensorsDelayTime = 1;
void resetReadSensorsTimer()
{
    readSensorsStartTime = millis();
}
int readSensorsTimerExpired()
{
    return (millis() - readSensorsStartTime > readSensorsDelayTime);
}

unsigned long continueStartTime = 0;
int continueDelayTime = 2000;
void resetContinueTimer()
{
    continueStartTime = millis();
}
int continueTimerExpired()
{
    return (millis() - continueStartTime > continueDelayTime);
}