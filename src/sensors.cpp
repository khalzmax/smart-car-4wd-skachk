#include <Arduino.h>

// #define DEBUG_SAVE_COUNTERS
// #define DEBUG_AVG_SENSORS

#include "sensors.h"
#include "constants.h"
#include "timers.h"
#include "speedctrl.h"

// const uint8_t NUMBER_OF_SENSORS = 4;

int sensors[NUMBER_OF_SENSORS] = {0, 0, 0, 0};        // current sensors values
int prevSensors[NUMBER_OF_SENSORS] = {0, 0, 0, 0};    // last sensors values
int sensorsCounter[NUMBER_OF_SENSORS] = {0, 0, 0, 0}; // sensors speed

int avgSensors[NUMBER_OF_SENSORS] = {0, 0, 0, 0}; // average sensors speed
int totalSensorsAvg = 0;

const uint8_t SENSORS_HISTORY_LENGTH = 1;
int sensorsHistory[NUMBER_OF_SENSORS][SENSORS_HISTORY_LENGTH];
bool isHistoryFull = false;
int lastHistoryNumber = -1;

boolean sensors_speedCorrectionEnabled;

// debug timers
#include "Timer.h"
#ifdef DEBUG_SAVE_COUNTERS
    Timer saveCountersTmr(200);
#endif
#ifdef DEBUG_AVG_SENSORS
Timer avgSensorsTmr(200);
#endif

void initSensorsHistory()
{
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    for (int j = 0; j < SENSORS_HISTORY_LENGTH; j++)
    {
      sensorsHistory[i][j] = 0;
    }
  }
  sensors_speedCorrectionEnabled = true;
}

void readSensors()
{
  int i;
  if (!readSensorsTimerExpired())
  {
    return;
  }

  sensors[0] = digitalRead(motor1_sensor);
  sensors[1] = digitalRead(motor2_sensor);
  sensors[2] = digitalRead(motor3_sensor);
  sensors[3] = digitalRead(motor4_sensor);
  for (i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    // check for sensor value update
    if (prevSensors[i] != sensors[i])
    {
      sensorsCounter[i]++;
    }
    prevSensors[i] = sensors[i];
  }
  resetReadSensorsTimer();
}
void resetSensorsCounter()
{
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    sensorsCounter[i] = 0;
  }
}

void handleSensors()
{
  if (!handleSensorsTimerExpired())
  {
    return;
  }
  for (int i = 0; i <= 3; i++)
  {
    Serial.print(sensorsCounter[i]);
    Serial.print(",");
  }
  // Serial.print(totalSensorsAvg);
  // Serial.print(",");
  Serial.println();
  //  for (int i=0; i<=3; i++) {
  //    Serial.print(sensors[i]);
  //    Serial.print(" ");
  //  }
  //  Serial.println();

  // But maybe here is nice! it has timer inside and remember last sensors speed
  saveCountersToHistory();
  if (sensors_speedCorrectionEnabled)
  {
    correctMotors();
  }

  resetHandleSensorsTimer();
}

void saveCountersToHistory()
{
  if (!saveHistoryTimerExpired())
  {
    return;
  }
  int currentHistoryNumber = lastHistoryNumber + 1;
  // get currentHistoryNumber
  if (currentHistoryNumber >= SENSORS_HISTORY_LENGTH)
  {
    // reset currentHistoryNumber
    currentHistoryNumber = 0;
  }
  // update history
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    sensorsHistory[i][currentHistoryNumber] = sensorsCounter[i];
  }
  lastHistoryNumber = currentHistoryNumber;
  // check if history is full
  if (lastHistoryNumber == (SENSORS_HISTORY_LENGTH - 1))
  {
    isHistoryFull = true;
    calculateAvgSensorsFromHistory();
  }
  else
  {
    isHistoryFull = false;
  }
  // reset sensorsCounter
  resetSensorsCounter();
  resetSaveHistoryTimer();

#ifdef DEBUG_SAVE_COUNTERS
  if (saveCountersTmr.timerExpired())
  {
    Serial.print("save counters info: ");
    for (int i = 0; i < 4; i++)
    {
      Serial.print(sensorsHistory[i][currentHistoryNumber]);
      Serial.print(',');
    }
    Serial.print(currentHistoryNumber);
    Serial.println();
    saveCountersTmr.resetTimer();
  }
#endif
}

int getTotalSensorsAvg()
{
  return totalSensorsAvg;
}

// supposed to be private
void calculateAvgSensorsFromHistory()
{
  int totalAvgSensors = 0;
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    avgSensors[i] = 0;
    for (int j = 0; j < SENSORS_HISTORY_LENGTH; j++)
    {
      avgSensors[i] += sensorsHistory[i][j];
      totalAvgSensors += sensorsHistory[i][j];

#ifdef DEBUG_AVG_SENSORS
      if (avgSensorsTmr.timerExpired())
      {
        Serial.print(sensorsHistory[i][j]);
        Serial.print(',');
      }
#endif
    }
    avgSensors[i] = avgSensors[i] / SENSORS_HISTORY_LENGTH;
#ifdef DEBUG_AVG_SENSORS
    if (avgSensorsTmr.timerExpired())
    {
      Serial.print(" avg: ");
      Serial.print(avgSensors[i]);
      Serial.print("\n");
    }
#endif
  }
  // calculate total avg speed
  totalSensorsAvg = totalAvgSensors / (NUMBER_OF_SENSORS * SENSORS_HISTORY_LENGTH);
#ifdef DEBUG_AVG_SENSORS
  if (avgSensorsTmr.timerExpired())
  {
    Serial.print(totalSensorsAvg);
    Serial.println();
    avgSensorsTmr.resetTimer();
  }
#endif
}

// getters

int *getAvgSensors()
{
  return avgSensors;
}
bool isSensorsHistoryFull()
{
  return isHistoryFull;
}
