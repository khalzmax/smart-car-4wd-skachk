#include <Arduino.h>
#include "constants.h"
#include "motors.h"

// #define CORRECTIONS_INFO
// #define DEBUG_MOTORS
// #define DEBUG_CORRECTIONS

#include "Timer.h"
#ifdef DEBUG_MOTORS
Timer debugTmr(1000); // this one is not working :( other timers are also enabled btw
#endif

// const int minSpeed = 100;
// const int maxSpeed = 255;
const uint8_t MOTORS_SPEED_CORRECTION_VALUE = 1;
const uint8_t MOTORS_MAXIMUM_SPEED_CORRECTION = 50;
// when all motor correction values are above this treashold then it's time to reset the corrections
const uint8_t MOTORS_CORRECTION_THRESHOLD = 10;

int motorsSpeedCorrection[] = {0, 0, 0, 0};

int getSafeSpeed(int speed);

void motors_setModeStop()
{
  digitalWrite(motor13_pin1, LOW);
  digitalWrite(motor13_pin2, LOW);
  digitalWrite(motor24_pin1, LOW);
  digitalWrite(motor24_pin2, LOW);
}

void motors_setModeBkw()
{
  digitalWrite(motor13_pin1, HIGH);
  digitalWrite(motor13_pin2, LOW);
  digitalWrite(motor24_pin1, LOW);
  digitalWrite(motor24_pin2, HIGH);
}

void motors_setModeFwd()
{
  digitalWrite(motor13_pin1, LOW);
  digitalWrite(motor13_pin2, HIGH);
  digitalWrite(motor24_pin1, HIGH);
  digitalWrite(motor24_pin2, LOW);
}

void motors_setModeTurnRight()
{
  digitalWrite(motor13_pin1, HIGH);
  digitalWrite(motor13_pin2, LOW);
  digitalWrite(motor24_pin1, HIGH);
  digitalWrite(motor24_pin2, LOW);
}

void motors_setModeTurnLeft()
{
  digitalWrite(motor13_pin1, LOW);
  digitalWrite(motor13_pin2, HIGH);
  digitalWrite(motor24_pin1, LOW);
  digitalWrite(motor24_pin2, HIGH);
}

void motors_setSpeed(int speed)
{
  analogWrite(motor1_pwm, getSafeSpeed(speed + motorsSpeedCorrection[0]));
  analogWrite(motor2_pwm, getSafeSpeed(speed + motorsSpeedCorrection[1]));
  analogWrite(motor3_pwm, getSafeSpeed(speed + motorsSpeedCorrection[2]));
  analogWrite(motor4_pwm, getSafeSpeed(speed + motorsSpeedCorrection[3]));
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

void motors_stop()
{
  motors_setSpeed(0);
}

void motors_correctMotorSpeed(byte motorIndex, int value = MOTORS_SPEED_CORRECTION_VALUE)
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
// todo this is not needed
void motors_initialCorrections()
{
  motorsSpeedCorrection[0] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[1] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[2] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[3] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
#ifdef DEBUG_CORRECTIONS
  // Serial.println("Speed corrections:");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(motorsSpeedCorrection[i]);
    Serial.print(',');
  }
  Serial.println();
#endif
}

void motors_flushCorrections()
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
    motors_resetCorrections();
  }
}

void motors_resetCorrections()
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
