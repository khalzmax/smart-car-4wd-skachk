#include "Arduino.h"

#include "speedctrl.h"
#include "motors.h"
#include "sensors.h"
#include "Timer.h"

// #define DEBUG_SPEED_CTRL
// #define CORRECTIONS_INFO
// #define DEBUG_MOTORS
// #define DEBUG_CORRECTIONS

const uint8_t MOTORS_SPEED_CORRECTION_VALUE = 1;
const uint8_t MOTORS_MAXIMUM_SPEED_CORRECTION = 50;
// when all motor correction values are above this treashold then it's time to reset the corrections
const uint8_t MOTORS_CORRECTION_THRESHOLD = 10;

int motorsSpeedCorrection[] = {0, 0, 0, 0};

int getSafeSpeed(int speed);

#ifdef DEBUG_MOTORS
Timer debugTmr(1000); // this one is not working :( other timers are also enabled btw
#endif

void speed_setSpeed(int speed)
{
  for (int i=0; i<4; i++)
  {
    motor_setSpeed(i, getSafeSpeed(speed + motorsSpeedCorrection[0]));
  }
#ifdef DEBUG_MOTORS
  if (debugTmr.timerExpired())
  {
    Serial.print("speed corrections: ");
    for (int i = 0; i < 4; i++)
    {
      Serial.print(getSafeSpeed(motorsSpeedCorrection[i]));
      Serial.print(',');
    }
    Serial.println();
    debugTmr.resetTimer();
  }
#endif
}
// returns 'safe' speed like the following:
// minSpeed >= speed <= maxSpeed
// 0 is also possible value
int getSafeSpeed(int speed)
{
  return (speed > maxSpeed)
             ? maxSpeed
             : (speed < minSpeed)
                   ? (speed == 0 ? 0 : minSpeed)
                   : speed;
}

void speed_correctMotorSpeed(byte motorIndex, int value = MOTORS_SPEED_CORRECTION_VALUE)
{
  motorsSpeedCorrection[motorIndex] += value;
  int absCorrection = abs(motorsSpeedCorrection[motorIndex]);
  // limit for maximum speed correction
  if (absCorrection > MOTORS_MAXIMUM_SPEED_CORRECTION)
  {
    motorsSpeedCorrection[motorIndex] = (motorsSpeedCorrection[motorIndex] > 0) ? MOTORS_MAXIMUM_SPEED_CORRECTION : -1 * MOTORS_MAXIMUM_SPEED_CORRECTION;
  }

#ifdef DEBUG_CORRECTIONS
  Serial.print("Correct speed m #");
  Serial.print(motorIndex);
  Serial.print(" val=");
  Serial.print(value);
  Serial.print(" abs=");
  Serial.print(absCorrection);
  Serial.print(" final=");
  Serial.print(motorsSpeedCorrection[motorIndex]);
  Serial.println();
#endif
#ifdef CORRECTIONS_INFO
  // Serial.print("Speed corrections: ");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(motorsSpeedCorrection[i]);
    Serial.print(',');
  }
  Serial.println();
#endif
};

void speed_flushCorrections()
{
  boolean allAboveThreshold = true;
  int absCorrectoin = 0;
  for (int i = 0; i < 4; i++)
  {
    absCorrectoin = abs(motorsSpeedCorrection[i]);
    if (absCorrectoin < MOTORS_CORRECTION_THRESHOLD)
      allAboveThreshold = false;
  }

  if (allAboveThreshold)
  {
    // reset speed corrections
    speed_resetCorrections();
  }
}

void speed_resetCorrections()
{
  // reset speed corrections
  for (int i = 0; i < 4; i++)
  {
    motorsSpeedCorrection[i] = 0;
  }
#ifdef CORRECTIONS_INFO
  // Serial.println("RESET CORRECTIONS");
#endif
}

void correctMotors()
{
  if (!isSensorsHistoryFull()) {
    return ;
  }

  speed_flushCorrections();

  int *avgSensors = getAvgSensors();
  int commonSensorsAvg = getTotalSensorsAvg();
  if (commonSensorsAvg == 0)
  {
    return;
  }
#ifdef DEBUG_SPEED_CTRL
      Serial.print("Speed controller. \ncommon_avg: ");
      Serial.print(commonSensorsAvg);
#endif
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    int8_t diff = commonSensorsAvg - avgSensors[i];
    uint8_t absDiff = abs(diff);
    if (absDiff < MIN_SENSOR_DIFF)
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
    speed_correctMotorSpeed(i, speedCorrectionValue);
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
