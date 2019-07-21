#include <Arduino.h>
#include "constants.h"
#include "motors.h"

// #define DEBUG_MOTORS
// #define DEBUG_CORRECTIONS

#include "Timer.h"
#ifdef DEBUG_MOTORS
Timer debugTmr(1000); // this one is not working :( other timers are also enabled btw
#endif

// const int minSpeed = 100;
// const int maxSpeed = 255;
const uint8_t MOTORS_SPEED_CORRECTION_VALUE = 3;

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
#ifdef DEBUG_CORRECTIONS
  Serial.println();
  Serial.print("Correct speed m #");
  Serial.print(motorIndex);
  Serial.print(" val=");
  Serial.print(value);
#endif
  motorsSpeedCorrection[motorIndex] += value;
#ifdef DEBUG_CORRECTIONS
  Serial.print(" final=");
  Serial.print(motorsSpeedCorrection[motorIndex]);

  Serial.println();
  Serial.print("Speed corrections:");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(motorsSpeedCorrection[i]);
    Serial.print(',');
  }
#endif
};
void motors_initialCorrections()
{
  motorsSpeedCorrection[0] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[1] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[2] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
  motorsSpeedCorrection[3] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
#ifdef DEBUG_CORRECTIONS
  Serial.println("Speed corrections:");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(motorsSpeedCorrection[i]);
    Serial.print(',');
  }
  Serial.println();
#endif
}
