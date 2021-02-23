
#include <Arduino.h>

#include "constants.h"
#include "motors.h"
#include "speedctrl.h"
#include "timers.h"
#include "sensors.h"

// functions declaration
void setup();
void loop();
void initFirstState();
void updateState();
void stateSpeedUp();
void stateSpeedDown();

// #define STATE_INFO
// #define DEBUG_STATE
#include "Timer.h"
#ifdef DEBUG_STATE
Timer debugTmr(1000);
#endif

void setup()
{
  Serial.begin(9600);

  motors_init();
  sensors_init();

  while (!Serial) ;
#ifdef DEBUG_STATE
  Serial.write("let's begin!\n");
#endif

  initSensorsHistory();

  resetMotorsTimer();
  resetSensorsCounter();
  resetHandleSensorsTimer();
  resetSaveHistoryTimer();
}


int speed = 0;

// STATE

/**
 0 - default state;
 1 - speed up;
 2 - speed down;
 3 - continue
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
  handleSensors();
  updateState();
}

// HANDLE STATE
void initFirstState()
{
  speed = minSpeed;
  motors_setModeFwd();
}
// here we decide the main state
void updateState(int forceState)
{
#ifdef DEBUG_STATE
  if (!debugTmr.timerExpired())
  {
    Serial.print("update state: ");
    Serial.print(state);
    Serial.print(" | ");
    Serial.print(speed);
    Serial.println();
    debugTmr.resetTimer();
  }
#endif
  if (state == 0 && speed == 0)
  {
    initFirstState();
    state = 1;
#ifdef STATE_INFO
    Serial.println("State: Initial setup");
#endif
    resetHandleSensorsTimer();
    resetSensorsCounter();
  }
  else if (state == 1 && speed == maxSpeed)
  {
    // rich maximum speed => change to continues state
    resetContinueTimer();
    state = 3;
#ifdef STATE_INFO
    Serial.println("State: Continues");
#endif
  }
  else if (state == 2 && speed == minSpeed)
  {
    // rich minimum speed => change to continues state
    resetContinueTimer();
    state = 3;
#ifdef STATE_INFO
    Serial.println("State: Continues");
#endif
  }
  else if (state == 3 && continueTimerExpired())
  {
    // end of continues state
    if (speed <= minSpeed)
    {
      // change to speed up
      motors_setModeFwd();
      state = 1;
      resetHandleSensorsTimer();
      resetSensorsCounter();
#ifdef STATE_INFO
      Serial.println("State: Speed up");
#endif
    }
    else if (speed >= maxSpeed)
    {
      // change to speed down
      motors_setModeFwd();
      state = 2;
      resetHandleSensorsTimer();
      resetSensorsCounter();
#ifdef STATE_INFO
      Serial.println("State: Speed down");
#endif
    }
  }
}

void stateSpeedUp()
{
  state = 1;
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
    speed_setSpeed(++speed);
  }
  resetMotorsTimer();
}
void stateSpeedDown()
{
  state = 3;
  if (motorsTimerExpired())
  {
    return;
  }
  if (speed > minSpeed)
  {
    speed_setSpeed(--speed);
  }
  resetMotorsTimer();
}
