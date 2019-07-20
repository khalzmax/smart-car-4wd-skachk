#include "Arduino.h"

#include "motors.h"
#include "sensors.h"

// #include "Timer.h"

#define DEBUG_SPEED_CTRL

// #ifdef DEBUG_SPEED_CTRL
// Timer debugTmr(500);
// #endif

// TODO this works not properly!
void correctMotors()
{
  if (!isSensorsHistoryFull()) {
    return ;
  }
  int *avgSensors = getAvgSensors();
  int commonSensorsAvg = getTotalSensorsAvg();
  if (commonSensorsAvg == 0)
  {
    return;
  }
#ifdef DEBUG_SPEED_CTRL
    // if (debugTmr.timerExpired())
    // {
      Serial.print("speed controller: ");
    // }
#endif
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    int diff = commonSensorsAvg - avgSensors[i];
    if (diff < MIN_SENSOR_DIFF)
    {
      continue;
    }
    // scale sensor diff and motor speed
    int speedCorrectionValue = map(diff, 0, MAX_SENSOR_VALUE, 0, maxSpeed);
    // correct motor speed
    motors_correctMotorSpeed(i, speedCorrectionValue);
#ifdef DEBUG_SPEED_CTRL
    // if (debugTmr.timerExpired())
    // {
      Serial.print(i);
      Serial.print(':');
      Serial.print(speedCorrectionValue);
      Serial.print(", ");
    // }
#endif
  }
#ifdef DEBUG_SPEED_CTRL
  // if (debugTmr.timerExpired())
  // {
    Serial.println();
    // debugTmr.resetTimer();
  // }
#endif
}
