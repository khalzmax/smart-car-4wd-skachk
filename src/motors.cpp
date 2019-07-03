#include "constants.h"
#include "motors.h"

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
    analogWrite(motor1_pwm, speed + motorsSpeedCorrection[0]);
    analogWrite(motor2_pwm, speed + motorsSpeedCorrection[1]);
    analogWrite(motor3_pwm, speed + motorsSpeedCorrection[2]);
    analogWrite(motor4_pwm, speed + motorsSpeedCorrection[3]);
}

void motors_stop()
{
    motors_setSpeed(0);
}

void motors_correctMotorSpeed(byte motorIndex, int value = MOTORS_SPEED_CORRECTION_VALUE)
{
  if ( (motorsSpeedCorrection[motorIndex] + value <= maxSpeed) &&
       (motorsSpeedCorrection[motorIndex] + value >= minSpeed) )
    {
      motorsSpeedCorrection[motorIndex] += value;
    }
};

void motors_initialCorrections() {
    motorsSpeedCorrection[0] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
    motorsSpeedCorrection[1] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
    motorsSpeedCorrection[2] = 4 * MOTORS_SPEED_CORRECTION_VALUE;
    motorsSpeedCorrection[3] = 0 * MOTORS_SPEED_CORRECTION_VALUE;
}
