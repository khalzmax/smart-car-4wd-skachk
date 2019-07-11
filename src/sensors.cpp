#include <Arduino.h>

#include "sensors.h"
#include "constants.h"
#include "timers.h"

// const uint8_t NUMBER_OF_SENSORS = 4;

int sensors[NUMBER_OF_SENSORS] = {0, 0, 0, 0}; // current sensors values
int sensorsCounter[NUMBER_OF_SENSORS] = {0, 0, 0, 0}; // sensors speed

int avgSensors[NUMBER_OF_SENSORS] = {0, 0, 0, 0}; // average sensors speed
int totalSensorsAvg = 0;

const uint8_t SENSORS_HISTORY_LENGTH = 10;
int sensorsHistory[NUMBER_OF_SENSORS][SENSORS_HISTORY_LENGTH];
bool isHistoryFull = false;
int lastHistoryNumber = 0;

void initSensorsHistory()
{
  for (int i=0; i<NUMBER_OF_SENSORS; i++) {
    for (int j=0; j<SENSORS_HISTORY_LENGTH; j++) {
      sensorsHistory[i][j] = 0;
    }
  }
}

void readSensors()
{
  int i;
  if (!readSensorsTimerExpired())
  {
    return;
  }
  // for (i = 0; i < 4; i++)
  //   {
  // save prev sensors state
  //       sensorsHistory[i] = sensors[i];
  //   }

  // TODO not here! save history should be another task with its own timer
  // saveToHistory(sensorsCounter);

  sensors[0] = digitalRead(motor1_sensor);
  sensors[1] = digitalRead(motor2_sensor);
  sensors[2] = digitalRead(motor3_sensor);
  sensors[3] = digitalRead(motor4_sensor);
  for (i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    // check for sensor value update
    if (sensorsHistory[0][i] != sensors[i])
    {
      sensorsCounter[i]++;
    }
  }
  resetReadSensorsTimer();
}
void resetSensors()
{
  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
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
  for (int i = 0; i <= 3; i++)
  {
    Serial.print(sensorsCounter[i]);
    Serial.print(",");
  }
  Serial.print(totalSensorsAvg);
  // Serial.print(",");
  Serial.println();
  //  for (int i=0; i<=3; i++) {
  //    Serial.print(sensors[i]);
  //    Serial.print(" ");
  //  }
  //  Serial.println();
  resetSensors();
  resetPrintSensorsTimer();
}

void saveToHistory(int *updates)
{
  if (!saveHistoryTimerExpired())
  {
    return;
  }
  int nextHistoryNumber = lastHistoryNumber + 1;
  // get nextHistoryNumber
  if (nextHistoryNumber >= SENSORS_HISTORY_LENGTH)
  {
    nextHistoryNumber = 0;
  }
  // update history
  for (int i=0; i< NUMBER_OF_SENSORS; i++)
  {
      sensorsHistory[nextHistoryNumber][i] = updates[i];
  }
  lastHistoryNumber = nextHistoryNumber;
  // check if history is full
  if (lastHistoryNumber == 0)
  {
    isHistoryFull = true;
    calculateAvgSensorsFromHistory();
  } else {
    isHistoryFull = false;
  }
  resetSaveHistoryTimer()
}
// calculater average value for all sensor values base on avgSensors
// int calculateCommonAvg()
// {
//   int result = 0;
//   for (int i = 0; i++; i < NUMBER_OF_SENSORS)
//   {
//     result += avgSensors[i];
//   }
//   return result / NUMBER_OF_SENSORS;
// }

int getTotalSensorsAvg() {
  return totalSensorsAvg;
}

// supposed to be private
void calculateAvgSensorsFromHistory() {
  int totalAvgSensors = 0;
  for (int i=0; i< NUMBER_OF_SENSORS; i++)
  {
    avgSensors[i] = 0;
    for (int j=0; j<SENSORS_HISTORY_LENGTH; j++)
    {
      avgSensors[i] += sensorsHistory[i][j];
      totalAvgSensors += sensorsHistory[i][j];
    }
    avgSensors[i] = avgSensors[i] / SENSORS_HISTORY_LENGTH;
  }
  // calculate total avg speed
  totalSensorsAvg = totalAvgSensors / (NUMBER_OF_SENSORS * SENSORS_HISTORY_LENGTH);
}

// getters

int *getAvgSensors() {
  return avgSensors;
}
bool isSensorsHistoryFull() {
  return isHistoryFull;
}
