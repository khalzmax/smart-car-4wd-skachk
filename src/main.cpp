
#include <Arduino.h>

#include "main.h"
#include "constants.h"
#include "motors.h"
#include "timers.h"


void setup()
{
  Serial.begin(9600);

  pinMode(motor13_pin1, OUTPUT);
  pinMode(motor13_pin2, OUTPUT);
  pinMode(motor24_pin1, OUTPUT);
  pinMode(motor24_pin2, OUTPUT);
  setModeStop();
  pinMode(motor1_pwm, OUTPUT);
  pinMode(motor2_pwm, OUTPUT);
  pinMode(motor3_pwm, OUTPUT);
  pinMode(motor4_pwm, OUTPUT);
  motorsStop();
  pinMode(motor1_sensor, INPUT);
  pinMode(motor2_sensor, INPUT);
  pinMode(motor3_sensor, INPUT);
  pinMode(motor4_sensor, INPUT);

  while (!Serial) ;
  Serial.write("let's begin!");

  resetMotorsTimer();
  resetSensors();
  resetPrintSensorsTimer();
}

int sensors[] = {0, 0, 0, 0};
int sensorsCounter[] = {0, 0, 0, 0};
int prevSensors[] = {0, 0, 0, 0};
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
  updateState();

  // OLD CODE
  /*
//  int speed;
  setModeFwd();
  Serial.println("FORWARD");
  for (speed=minSpeed; speed<=maxSpeed; speed++) {
      setMotorsSpeed(speed);
      Serial.println(speed);
//      readSensors();
//      printSensors();
      delay(100);
  }
  setModeBkw();
  Serial.println("BACKWARD");
  for (speed=minSpeed; speed<=maxSpeed; speed++) {
      setMotorsSpeed(speed);
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

// TIMERS



// HANDLE STATE
void initFirstState()
{
  speed = minSpeed;
  setModeFwd();
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
  else if (state == 3 && !continueTimerExpired())
  {
    // end of continues state
    if (speed == minSpeed)
    {
      // change to speed up
      setModeFwd();
      state = 1;
      resetPrintSensorsTimer();
      resetSensors();
      Serial.println("State: Speed up");
    }
    else if (speed == maxSpeed)
    {
      // change to speed down
      //      setModeBkw();
      setModeFwd();
      state = 2;
      resetPrintSensorsTimer();
      resetSensors();
      Serial.println("State: Speed down");
    }
  }
}

void stateSpeedUp()
{
  if (!continueTimerExpired())
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
    setMotorsSpeed(++speed);
  }
  resetMotorsTimer();
}
void stateSpeedDown()
{
  if (!continueTimerExpired())
  {
    return;
  }
  if (speed > minSpeed)
  {
    setMotorsSpeed(--speed);
  }
  resetMotorsTimer();
}

void readSensors()
{
  int i;
  if (!readSensorsTimerExpired())
  {
    return;
  }
  for (i = 0; i < 4; i++)
  {
    // save prev sensors state
    prevSensors[i] = sensors[i];
  }
  //  if (analogRead(motor1_sensor)) sensors[0]++;
  //  if (analogRead(motor2_sensor)) sensors[1]++;
  //  if (analogRead(motor3_sensor)) sensors[2]++;
  //  if (analogRead(motor4_sensor)) sensors[3]++;
  sensors[0] = digitalRead(motor1_sensor);
  sensors[1] = digitalRead(motor2_sensor);
  sensors[2] = digitalRead(motor3_sensor);
  sensors[3] = digitalRead(motor4_sensor);
  for (i = 0; i < 4; i++)
  {
    // save prev sensors state
    if (prevSensors[i] != sensors[i])
    {
      sensorsCounter[i]++;
    }
  }
  resetReadSensorsTimer();
}
void resetSensors()
{
  for (int i = 0; i <= 3; i++)
  {
    sensorsCounter[i] = 0;
  }
}

void printSensors()
{
  if (!printSensorsTimerExpired())
  {
    return;
  }
  Serial.print("Sensors: ");
  for (int i = 0; i <= 3; i++)
  {
    Serial.print(sensorsCounter[i]);
    Serial.print(" ");
  }
  Serial.println();
  //  for (int i=0; i<=3; i++) {
  //    Serial.print(sensors[i]);
  //    Serial.print(" ");
  //  }
  //  Serial.println();
  resetSensors();
  resetPrintSensorsTimer();
}

// MOTORS


