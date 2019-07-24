#include <Arduino.h>
#include "constants.h"
#include "motors.h"

// const int minSpeed = 100;
// const int maxSpeed = 255;

void motors_init()
{
  pinMode(motor13_pin1, OUTPUT);
  pinMode(motor13_pin2, OUTPUT);
  pinMode(motor24_pin1, OUTPUT);
  pinMode(motor24_pin2, OUTPUT);
  motors_setModeStop();
  pinMode(motor1_pwm, OUTPUT);
  pinMode(motor2_pwm, OUTPUT);
  pinMode(motor3_pwm, OUTPUT);
  pinMode(motor4_pwm, OUTPUT);
  motors_stop();
}

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

void motor_setSpeed(int motorIndex, int speed)
{
  switch (motorIndex)
  {
  case 0:
      analogWrite(motor1_pwm, speed);
      break;
  case 1:
      analogWrite(motor2_pwm, speed);
      break;
  case 2:
      analogWrite(motor3_pwm, speed);
      break;
  case 3:
      analogWrite(motor4_pwm, speed);
      break;
  }
}

void motors_stop()
{
  // motors_setSpeed(0);
  motors_setModeStop();
}
