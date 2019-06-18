#include <Arduino.h>

const int motor13_pin1 = 4;
const int motor13_pin2 = 2;
const int motor24_pin1 = 8;
const int motor24_pin2 = 7;

const int motor1_pwm = 3;
const int motor2_pwm = 5;
const int motor3_pwm = 6;
const int motor4_pwm = 9;

const uint8_t motor1_sensor = 14; // A0;
const uint8_t motor2_sensor = 15; // A1;
const uint8_t motor3_sensor = 16; // A2;
const uint8_t motor4_sensor = 17; // A3;

const int minSpeed = 100;
const int maxSpeed = 255;