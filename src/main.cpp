
#include <Arduino.h>

#include "main.h"
#include "constants.h"
#include "motors.h"
#include "timers.h"
#include "sensors.h"
#include "speedctrl.h"

void setup()
{
  Serial.begin(9600);

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
  pinMode(motor1_sensor, INPUT);
  pinMode(motor2_sensor, INPUT);
  pinMode(motor3_sensor, INPUT);
  pinMode(motor4_sensor, INPUT);

  while (!Serial) ;
  Serial.write("let's begin!");

  motors_initialCorrections();
  void initSensorsHistory();

  resetMotorsTimer();
  resetSensors();
  resetPrintSensorsTimer();
}


int speed = 0;

// STATE

/**
 0 - default state;
 1 - speed up;
 2 - speed down;
*/
int state = 0;

void loop()
{

  readSensors();
  // handle state;
  switch (state)
  {
  case 0:
    // start
    break;
  case 1: // speed up
    stateSpeedUp();
    break;
  case 2: // speed down
    stateSpeedDown();
    break;
  case 3:
    // continue
    break;
  default:
    break;
  }
  printSensors();
  correctMotors();
  updateState();

  // OLD CODE
  /*
//  int speed;
  motors_setModeFwd();
  Serial.println("FORWARD");
  for (speed=minSpeed; speed<=maxSpeed; speed++) {
      motors_setSpeed(speed);
      Serial.println(speed);
//      readSensors();
//      printSensors();
      delay(100);
  }
  motors_setModeBkw();
  Serial.println("BACKWARD");
  for (speed=minSpeed; speed<=maxSpeed; speed++) {
      motors_setSpeed(speed);
      Serial.println(speed);
//      readSensors();
//      printSensors();
      delay(100);
  }
  */
  /*if (Serial.available()) {
    int speed = Serial.parseInt();
  }*/
}

// HANDLE STATE
void initFirstState()
{
  speed = minSpeed;
  motors_setModeFwd();
}
// here we decide the main state
void updateState()
{
  if (state == 0 && speed == 0)
  {
    initFirstState();
    state = 1;
    Serial.println("State: Initial setup");

    resetPrintSensorsTimer();
    resetSensors();
  }
  else if (state == 1 && speed == maxSpeed)
  {
    // rich maximum speed => change to continues state
    resetContinueTimer();
    state = 3;
    Serial.println("State: Continues");
  }
  else if (state == 2 && speed == minSpeed)
  {
    // rich minimum speed => change to continues state
    resetContinueTimer();
    state = 3;
    Serial.println("State: Continues");
  }
  else if (state == 3 && continueTimerExpired())
  {
    // end of continues state
    if (speed == minSpeed)
    {
      // change to speed up
      motors_setModeFwd();
      state = 1;
      resetPrintSensorsTimer();
      resetSensors();
      Serial.println("State: Speed up");
    }
    else if (speed == maxSpeed)
    {
      // change to speed down
      //      motors_setModeBkw();
      motors_setModeFwd();
      state = 2;
      resetPrintSensorsTimer();
      resetSensors();
      Serial.println("State: Speed down");
    }
  }
}

void stateSpeedUp()
{
  if (motorsTimerExpired())
  {
    return;
  }
  if (speed == 0)
  {
    // set speed for first run
    speed = minSpeed;
  }
  if (speed < maxSpeed)
  {
    motors_setSpeed(++speed);
  }
  resetMotorsTimer();
}
void stateSpeedDown()
{
  if (motorsTimerExpired())
  {
    return;
  }
  if (speed > minSpeed)
  {
    motors_setSpeed(--speed);
  }
  resetMotorsTimer();
}

// motor correction
/* void correctMotors() {
  int *avgSensors;
  avgSensors = getAvgSensors();
  // TODO how to calculate the correction value for the motor?
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    int sensorValue = avgSensors[i];
    if (sensorValue < MIN_SENSOR_DIFF) {
      return;
    }
    // TODO scale sensors and motos speed
    int scaledSpeed = map(sensorValue, 0, MAX_SENSOR_VALUE, 0, maxSpeed);
    // TODO correct motors speed
  }
} */
