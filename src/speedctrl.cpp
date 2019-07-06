#include "Arduino.h"

#include "motors.h"
#include "sensors.h"

void correctMotors()
{
  int *avgSensors = getAvgSensors();
  int commonSensorsAvg = getCommonSensorsAvg();
  if (commonSensorsAvg == 0)
  {
    return;
  }
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
  }
}
