#include <Arduino.h>

const uint8_t NUMBER_OF_SENSORS = 4;
const uint8_t MAX_SENSOR_VALUE = 190;
const uint8_t MIN_SENSOR_DIFF = 5;

void initSensorsHistory();
void saveToHistory(int *updates);

void calculateAvgSensorsFromHistory();
// int calculateCommonAvg();
int getCommonSensorsAvg();

void readSensors();
void resetSensors();
void printSensors();

int *getAvgSensors();
bool isSensorsHistoryFull();
