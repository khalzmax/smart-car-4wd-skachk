#include <Arduino.h>

const uint8_t NUMBER_OF_SENSORS = 4;
const uint8_t MAX_SENSOR_VALUE = 190;
const uint8_t MIN_SENSOR_DIFF = 5;

void initSensorsHistory();
void saveCountersToHistory();

void calculateAvgSensorsFromHistory();
// int calculateCommonAvg();
int getTotalSensorsAvg();

void readSensors();
void resetSensorsCounter();
void printSensors();

int *getAvgSensors();
bool isSensorsHistoryFull();
