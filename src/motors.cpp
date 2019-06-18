#include "constants.h"
#include "motors.h"

void setModeStop()
{
    digitalWrite(motor13_pin1, LOW);
    digitalWrite(motor13_pin2, LOW);
    digitalWrite(motor24_pin1, LOW);
    digitalWrite(motor24_pin2, LOW);
}

void setModeBkw()
{
    digitalWrite(motor13_pin1, HIGH);
    digitalWrite(motor13_pin2, LOW);
    digitalWrite(motor24_pin1, LOW);
    digitalWrite(motor24_pin2, HIGH);
}

void setModeFwd()
{
    digitalWrite(motor13_pin1, LOW);
    digitalWrite(motor13_pin2, HIGH);
    digitalWrite(motor24_pin1, HIGH);
    digitalWrite(motor24_pin2, LOW);
}

void setModeTurnRight()
{
    digitalWrite(motor13_pin1, HIGH);
    digitalWrite(motor13_pin2, LOW);
    digitalWrite(motor24_pin1, HIGH);
    digitalWrite(motor24_pin2, LOW);
}

void setModeTurnLeft()
{
    digitalWrite(motor13_pin1, LOW);
    digitalWrite(motor13_pin2, HIGH);
    digitalWrite(motor24_pin1, LOW);
    digitalWrite(motor24_pin2, HIGH);
}

void setMotorsSpeed(int speed)
{
    analogWrite(motor1_pwm, speed);
    analogWrite(motor2_pwm, speed);
    analogWrite(motor3_pwm, speed);
    analogWrite(motor4_pwm, speed);
}

void motorsStop()
{
    setMotorsSpeed(0);
}