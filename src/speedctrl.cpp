#include "Arduino.h"

#include "motors.h"
#include "sensors.h"

// #include "Timer.h"

#define DEBUG_SPEED_CTRL

// #ifdef DEBUG_SPEED_CTRL
// Timer debugTmr(500);
// #endif

// TODO this works not properly!
// need to reset it during speed up / down and enable when going stable
// or reset when corrections going out of control (>100 ets)
// or get avg history every time when save history
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
      Serial.print("speed controller: common_avg: ");
      Serial.print(commonSensorsAvg);
#endif
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    int8_t diff = commonSensorsAvg - avgSensors[i];
    uint8_t absDiff = abs(diff);
    if (absDiff < MIN_SENSOR_DIFF) // TODO get module value
    {
      continue;
    }
    // scale sensor diff and motor speed
    int speedCorrectionValue = map(absDiff, 0, MAX_SENSOR_VALUE, 0, maxSpeed);
    // check correction direction + / -
    if (diff < 0) {
      speedCorrectionValue *= -1;
    }
    // correct motor speed
    motors_correctMotorSpeed(i, speedCorrectionValue);
#ifdef DEBUG_SPEED_CTRL
    Serial.println();
    Serial.print(">> motor #");
    Serial.print(i);
    Serial.print(": diff: ");
    Serial.print(diff);
    Serial.print(", corr: ");
    Serial.print(speedCorrectionValue);
#endif
  }
#ifdef DEBUG_SPEED_CTRL
  Serial.println();
#endif
}
